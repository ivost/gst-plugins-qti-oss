
import logging as log
import timeit
import traceback
import time

import gi
gi.require_version('GstBase', '1.0')
gi.require_version('Gst', '1.0')
from gi.repository import Gst, GObject, GstBase

import gi
gi.require_version("Gst", "1.0")
gi.require_version("GstApp", "1.0")
gi.require_version("GstVideo", "1.0")
from gi.repository import Gst, GLib, GObject, GstApp, GstVideo  # noqa:F401,F402

from gst_tools import GstVideoSink, GstVideoSource, GstPipeline, GstContext  # noqa:F401,F402

# must be first
Gst.init(None)


class MyPlugin(Gst.Element):
# class MyPlugin(GstBase.BaseTransform):

    __gstmeta__ = ("myplugin",
                   "Transform",
                   "Python",
                   "Ivo Stoyanov, Inseego")

    __gstmetadata__ = __gstmeta__

    _srctemplate = Gst.PadTemplate.new('src', Gst.PadDirection.SRC,
                                       Gst.PadPresence.ALWAYS,
                                       Gst.Caps.from_string("video/x-raw,format=RGB"))

    _sinktemplate = Gst.PadTemplate.new('sink', Gst.PadDirection.SINK,
                                        Gst.PadPresence.ALWAYS,
                                        Gst.Caps.from_string("video/x-raw,format=RGB"))

    __gsttemplates__ = (_srctemplate, _sinktemplate)

    __gproperties__ = {
        "model": (GObject.TYPE_PYOBJECT,
                  "model",
                  "Contains model that implements IDataTransform",
                  GObject.ParamFlags.READWRITE)
    }

    def __init__(self):
        Gst.Element.__init__(self)

        self.sinkpad = Gst.Pad.new_from_template(self._sinktemplate, 'sink')
        self.sinkpad.set_chain_function_full(self.chainfunc, None)
        self.sinkpad.set_event_function_full(self.eventfunc, None)
        self.add_pad(self.sinkpad)

        self.srcpad = Gst.Pad.new_from_template(self._srctemplate, 'src')
        self.srcpad.set_event_function_full(self.srceventfunc, None)
        self.srcpad.set_query_function_full(self.srcqueryfunc, None)
        self.add_pad(self.srcpad)

        self.model = None

    def chainfunc(self, pad, parent, buffer):

        try:
            if self.model is not None:
                item = {
                    "pad": pad,
                    "buffer": buffer,
                    "timeout": 0.01
                }
                self.model.process(**item)
        except Exception as e:
            log.error(e)
            traceback.print_exc()

        return self.srcpad.push(buffer)

    def do_get_property(self, prop):
        if prop.name == 'model':
            return self.model
        else:
            raise AttributeError('unknown property %s' % prop.name)

    def do_set_property(self, prop, value):
        if prop.name == 'model':
            self.model = value
        else:
            raise AttributeError('unknown property %s' % prop.name)

    def eventfunc(self, pad, parent, event):
        return self.srcpad.push_event(event)

    def srcqueryfunc(self, pad, object, query):
        return self.sinkpad.query(query)

    def srceventfunc(self, pad, parent, event):
        return self.sinkpad.push_event(event)


def simple_register(class_info):
    GObject.type_register(MyPlugin)
    __gstelementfactory__ = ("myplugin", Gst.Rank.NONE, MyPlugin)
    return True

def register(class_info):
    version = '1.0'
    gstlicense = 'LGPL'
    origin = ''
    name = class_info.__gstmetadata__[0]
    source = class_info.__gstmetadata__[1]
    package = class_info.__gstmetadata__[0]
    description = class_info.__gstmetadata__[2]

    def init_function(plugin, userarg):
        MyType = GObject.type_register(MyPlugin)
        Gst.Element.register(plugin, 'myplugin', 0, MyType)
        return True

    ok = Gst.Plugin.register_static_full(
        Gst.VERSION_MAJOR, Gst.VERSION_MINOR,
        name, description,
        init_function,
        version,
        gstlicense,
        source,
        package,
        origin,
        None)
    # ignore warning: .... has no valid long-name field
    if not ok:
        raise ImportError("Plugin {} not registered".format(name))

    return ok

def run_pipeline():
    command = "videotestsrc num-buffers=1000 ! myplugin | fakesink sync=false"
    print("running pipeline: " + command)
    with GstContext(), GstPipeline(command) as pipeline:
        assert isinstance(pipeline, GstPipeline)
    return


if __name__ == "__main__":
    log.basicConfig(format='%(levelname)s\t%(message)s', level=log.DEBUG)
    log.debug("registering my plugin")
    #ok = simple_register(MyPlugin)
    ok = register(MyPlugin)
    log.info("ok: " + str(ok))

    run_pipeline()
