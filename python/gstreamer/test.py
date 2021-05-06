import os
import time
import math

import typing as typ

import logging as log
from random import randint
from fractions import Fraction

import numpy as np


import gi
gi.require_version("Gst", "1.0")
gi.require_version("GstApp", "1.0")
gi.require_version("GstVideo", "1.0")
from gi.repository import Gst, GLib, GObject, GstApp, GstVideo  # noqa:F401,F402

from gst_tools import GstVideoSink, GstVideoSource, GstPipeline, GstContext  # noqa:F401,F402

# from gstreamer import GstVideo, Gst
# import gstreamer as gst
# import gstreamer.utils as utils
from utils import ndarray_to_gst_buffer


VERSION = "0.5.6.2"

NUM_BUFFERS = 10
WIDTH, HEIGHT = 1920, 1080
FPS = 15
FORMAT = "RGB"

Frame = typ.NamedTuple(
    'Frame', [
        ('buffer_format', GstVideo.VideoFormat),
        ('buffer', np.ndarray),
    ])


FRAMES = [
    Frame(GstVideo.VideoFormat.RGB, np.random.randint(
        low=0, high=255, size=(HEIGHT, WIDTH, 3), dtype=np.uint8)),
    Frame(GstVideo.VideoFormat.RGBA, np.random.randint(
        low=0, high=255, size=(HEIGHT, WIDTH, 4), dtype=np.uint8)),
    Frame(GstVideo.VideoFormat.GRAY8, np.random.randint(
        low=0, high=255, size=(HEIGHT, WIDTH), dtype=np.uint8)),
    Frame(GstVideo.VideoFormat.GRAY16_BE, np.random.uniform(
        0, 1, (HEIGHT, WIDTH)).astype(np.float32))
]

   
# https://gstreamer.freedesktop.org/documentation/app/appsrc.html?gi-language=c
# https://gstreamer.freedesktop.org/documentation/app/appsrc.html?gi-language=python

def test_video_sink():
    num_buffers = NUM_BUFFERS

    command = "appsrc emit-signals=True is-live=True ! videoconvert ! fakesink sync=false"
    print("pipeline: " + command)

    for frame in FRAMES:
        h, w = frame.buffer.shape[:2]
        # print("h:", h, " w:", w)
        with GstContext(), GstVideoSink(command, width=w, height=h, video_frmt=frame.buffer_format) as pipeline:
            assert pipeline.total_buffers_count == 0

            # wait pipeline to initialize
            max_num_tries, num_tries = 5, 0
            while not pipeline.is_active and num_tries <= max_num_tries:
                time.sleep(.1)
                num_tries += 1

            assert pipeline.is_active

            for _ in range(num_buffers):
                pipeline.push(frame.buffer)

            assert pipeline.total_buffers_count == num_buffers

    print("test_video_sink pass")
    pass

def test_video_source():
    num_buffers = NUM_BUFFERS
    width, height = WIDTH, HEIGHT

    formats = [GstVideo.VideoFormat.to_string(f.buffer_format) for f in FRAMES]

    for fmt in formats:
        caps_filter = 'capsfilter caps=video/x-raw,format={},width={},height={}'.format(
            fmt, width, height)
        command = 'videotestsrc num-buffers={} ! {} ! appsink emit-signals=True sync=false'.format(
            num_buffers, caps_filter)
        with GstContext(), GstVideoSource(command) as pipeline:

            num_read = 0
            while num_read < num_buffers:
                buffer = pipeline.pop()
                if buffer:
                    num_read += 1
                    h, w = buffer.data.shape[:2]
                    assert h == height and w == width

            assert pipeline.total_buffers_count == num_buffers
    print("test_video_source pass")
    pass


def test_gst_pipeline():
    command = "videotestsrc num-buffers=100 ! fakesink sync=false"
    print("pipeline: " + command)
    with GstContext(), GstPipeline(command) as pipeline:
        assert isinstance(pipeline, GstPipeline)
    print("test_gst_pipeline pass")
    pass



def test_metadata():
    np_buffer = np.random.randint(
        low=0, high=255, size=(HEIGHT, WIDTH, 3), dtype=np.uint8)

    
    gst_buffer = ndarray_to_gst_buffer(np_buffer)

    from gst_objects_info_meta import gst_meta_write, gst_meta_get, gst_meta_remove

    objects = [
        {'class_name': "person", 'bounding_box': [
            8, 10, 100, 100], 'confidence': 0.6, 'track_id': 1},
        {'class_name': "person", 'bounding_box': [
            10, 9, 120, 110], 'confidence': 0.67, 'track_id': 2},
    ]

    # no metadata at the beginning
    assert len(gst_meta_get(gst_buffer)) == 0

    # write metadata
    gst_meta_write(gst_buffer, objects)

    # read metadata
    meta_objects = gst_meta_get(gst_buffer)
    assert len(gst_meta_get(gst_buffer)) == len(objects)

    for gst_meta_obj, py_obj in zip(meta_objects, objects):
        for key, val in py_obj.items():
            if isinstance(gst_meta_obj[key], float):
                assert math.isclose(gst_meta_obj[key], val, rel_tol=1e-07)
            else:
                assert gst_meta_obj[key] == val

    # remove metadata
    gst_meta_remove(gst_buffer)
    assert len(gst_meta_get(gst_buffer)) == 0
    print("test_get_metadata pass")


# def test_gst_buffer_to_ndarray():

#     caps = Gst.Caps.from_string(
#         "video/x-raw,format={},width={},height={}".format(FORMAT, WIDTH, HEIGHT))

#     video_format = utils.gst_video_format_from_string(FORMAT)
#     channels = utils.get_num_channels(video_format)
#     dtype = utils.get_np_dtype(video_format)

#     npndarray = np.random.randint(low=0, high=255, size=(
#         HEIGHT, WIDTH, channels), dtype=dtype)
#     gst_buffer = utils.ndarray_to_gst_buffer(npndarray)

#     res_npndarray = utils.gst_buffer_with_caps_to_ndarray(gst_buffer, caps)

#     assert (npndarray == res_npndarray).all()


# def test_gst_buffer_channels():

    # assert bool(8 & (1 << (4 - 1)))
    # assert utils.is_kbit_set(2, 2)
    # assert utils.is_kbit_set(8, 4)
    # assert utils.is_kbit_set(8 | 1, 1)

    # assert utils.get_num_channels(GstVideo.VideoFormat.RGB) == 3
    # assert utils.get_num_channels(GstVideo.VideoFormat.RGBA) == 4
    # assert utils.get_num_channels(GstVideo.VideoFormat.GRAY8) == 1
    # assert utils.get_num_channels(GstVideo.VideoFormat.I420) == -1


if __name__ == "__main__":
    print("python gstreamer test ", VERSION)
    log.basicConfig(format='%(process)d-%(levelname)s-%(message)s')
    log.debug("python gstreamer test " + VERSION)
    test_video_sink()
    test_video_source()
    test_gst_pipeline()
    test_metadata()
