added from codeaurora branch le-gst.lnx.2.1.r6-rel

https://source.codeaurora.org/quic/le/platform/vendor/qcom-opensource/gst-plugins-qti-oss

gstreamer python metadata plugin

https://github.com/jackersson/gstreamer-python

http://lifestyletransfer.com/how-to-write-gstreamer-plugin-with-python/


http://lifestyletransfer.com/how-to-install-gstreamer-python-bindings/

http://lifestyletransfer.com/

https://github.com/jackersson/gst-plugins-tf


https://github.com/jackersson/gst-python-plugins

http://lifestyletransfer.com/how-to-add-metadata-to-gstreamer-buffer-in-python/


https://github.com/GStreamer/gst-python

https://github.com/jackersson/gst-python-hacks/tree/master/gst-metadata

https://gstreamer.freedesktop.org/documentation/additional/design/meta.html?gi-language=c#requirements

sudo apt install python-gi-dev

gst-python$ ./autogen.sh --disable-gtk-doc --noconfigure

./configure --with-libpython-dir=/usr/lib/x86_64-linux-gnu --prefix /home/ivo/.pyenv/versions/3.6.12/envs/venv36

make
make install

Libraries have been installed in:
   /home/ivo/.pyenv/versions/3.6.12/envs/venv36/lib/gstreamer-1.0

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'



sudo snap install cmake --classic
(venv36) 06:06:41 gstreamer-python$ cmake --version
cmake version 3.20.2

rm -rf ~/.cache/gstreamer-1.0

sudo apt-get update
sudo apt-get upgrade
sudo apt-get build-essential
sudo apt-get pkg-config
sudo apt-get install libgstreamer1.0-0 gstreamer1.0-dev gstreamer1.0-tools gstreamer1.0-doc
sudo apt-get install gstreamer1.0-plugins-base gstreamer1.0-plugins-good
sudo apt-get install gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
sudo apt-get install gstreamer1.0-libav
sudo apt-get install gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio

sudo apt install libgstreamer-plugins-base1.0-dev

https://gstreamer.freedesktop.org/modules/


https://github.com/ivost/gstreamer-python

update-alternatives --config python3
python3 -V
Python 3.6.9

sudo apt-get install python3.6-dev

cd /home/ivo/github/gstreamer-python

pyenv local venv36
python3 -m pip install --no-use-pep517 --upgrade --requirement requirements.txt

pip -V
pip 21.1.1 from /home/ivo/.local/lib/python3.6/site-packages/pip (python 3.6)

pip install --upgrade --requirement requirements.txt

Installing collected packages: zipp, typing-extensions, pyparsing, importlib-metadata, toml, py, pluggy, packaging, iniconfig, attrs, pytest, pyflakes, pycodestyle, pycairo, py-cpuinfo, mccabe, pytest-benchmark, PyGObject, numpy, flake8, autopep8
Successfully installed PyGObject-3.40.1 attrs-21.1.0 autopep8-1.5.7 flake8-3.9.1 importlib-metadata-4.0.1 iniconfig-1.1.1 mccabe-0.6.1 numpy-1.19.5 packaging-20.9 pluggy-0.13.1 py-1.10.0 py-cpuinfo-8.0.0 pycairo-1.20.0 pycodestyle-2.7.0 pyflakes-2.3.1 pyparsing-2.4.7 pytest-6.2.4 pytest-benchmark-3.4.1 toml-0.10.2 typing-extensions-3.10.0.0 zipp-3.4.1

sudo ./build-3rd-party.sh
Linking C shared library libgst_objects_info_meta.so
Built target gst_objects_info_meta

sudo ./build-gst-python.sh
Libraries have been installed in:
   /usr/lib/gstreamer-1.0

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'


lat  /usr/lib/gstreamer-1.0
libgstpython.cpython-36m-x86_64-linux-gnu.so
libgstpython.la

Export LIB_GSTREAMER_PATH with custom path to libgstreamer.so ???
export LIB_GSTREAMER_PATH=/home/ivo/.pyenv/lib/gstreamer-1.0/libgstreamer-1.0.so.0

find ~ -name libgstreamer*.so
/home/ivo/turbox/apps_proc/build-qti-distro-fullstack-virtualization-perf/tmp-glibc/sysroots-components/qcs610_odk_64/gstreamer1.0/usr/lib/libgstreamer-1.0.so

gst-inspect-1.0 --version
gst-inspect-1.0 version 1.14.5
GStreamer 1.14.5
https://launchpad.net/distros/ubuntu/+source/gstreamer1.0

find /usr/local -name libgstreamer*.so
find /usr -name libgstreamer*.so

/usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so

PYTHONPATH=. pytest tests/ -s --verbose

Using /home/ivo/.local/lib/python3.6/site-packages

sudo python3 setup.py install

python3 -c 'import gstreamer'

examples$ python3 gst_launch.py -p "videotestsrc num-buffers=100 ! fakesink sync=false"

gst-launch-1.0 videotestsrc num-buffers=100 ! fakesink sync=false

find /usr/lib -name *gstbase-1*
/usr/lib/x86_64-linux-gnu/libgstbase-1.0.so.0.1405.0
/usr/lib/x86_64-linux-gnu/libgstbase-1.0.so.0
/usr/lib/x86_64-linux-gnu/libgstbase-1.0.so


 /usr/lib/x86_64-linux-gnu/gstreamer-1.0

pkg-config --cflags  gstreamer-1.0
-pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include

This library should be linked to by getting cflags and libs from gstreamer-plugins-base-1.0.pc and adding -lgstallocators-1.0 to the library flags.
    
pkg-config --cflags --libs gstreamer-plugins-base-1.0
pkg-config --cflags --libs gstreamer-allocators-1.0

-pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgstallocators-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
