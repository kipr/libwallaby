include(ExternalProject)

ExternalProject_Add(opencv
  URL https://github.com/opencv/opencv/archive/refs/tags/4.6.0.tar.gz
  CMAKE_ARGS
    -DWITH_FFMPEG=OFF
    -DWITH_GSTREAMER=OFF
    -DMSMF=OFF
    -DWITH_DSHOW=OFF
    -DWITH_AVFOUNDATION=OFF
    -DWITH_GTK=OFF
    -DWITH_WIN32UI=OFF
    -DWITH_JAVA=OFF
    -DBUILD_opencv_python2=OFF
    -DBUILD_opencv_python3=OFF
    -DBUILD_TESTS=OFF
    -DBUILD_PERF_TESTS=OFF
    -DBUILD_EXAMPLES=OFF
    -DBUILD_opencv_apps=OFF
    -DWITH_OPENEXR=OFF
    -DWITH_JASPER=OFF
    -DWITH_OPENJPEG=OFF
    -DWITH_WEBP=OFF
    -DWITH_TIFF=OFF
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/opencv
)