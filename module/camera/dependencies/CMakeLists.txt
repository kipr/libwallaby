include(ExternalProject)

set(OPENCV_DIR ${CMAKE_CURRENT_BINARY_DIR}/dep_opencv)
set(OPENCV_LIB_DIR ${OPENCV_DIR}/lib)
set(OPENCV_INCLUDE_DIR ${OPENCV_DIR}/include)

ExternalProject_Add(dep_opencv
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
    -DCMAKE_INSTALL_PREFIX=${OPENCV_DIR}
)

# opencv
add_library(opencv INTERFACE)
add_dependencies(opencv dep_opencv)
target_link_directories(opencv INTERFACE ${OPENCV_LIB_DIR})
target_include_directories(opencv INTERFACE ${OPENCV_INCLUDE_DIR}/opencv4)

# opencv_core
add_library(opencv_core INTERFACE)
target_link_libraries(opencv_core INTERFACE opencv opencv_core)


set(ZBAR_DIR ${CMAKE_CURRENT_BINARY_DIR}/dep_zbar)
set(ZBAR_LIB_DIR ${ZBAR_DIR}/lib)
set(ZBAR_INCLUDE_DIR ${ZBAR_DIR}/include)

ExternalProject_Add(dep_zbar
  URL https://downloads.sourceforge.net/project/zbar/zbar/0.10/zbar-0.10.tar.bz2
  PATCH_COMMAND patch -d ${ZBAR_DIR}-prefix/src/dep_zbar -p0 < ${CMAKE_CURRENT_SOURCE_DIR}/zbar/fix_dprintf_definition.patch
  CONFIGURE_COMMAND ${ZBAR_DIR}-prefix/src/dep_zbar/configure
    --prefix=${ZBAR_DIR}
    --disable-video
    --without-imagemagick
    --without-gtk
    --without-qt
    --without-python
    --without-jpeg
    --without-xv
    --without-xshm
  INSTALL_COMMAND make install
)

# zbar
add_library(zbar INTERFACE)
add_dependencies(zbar dep_zbar)
target_link_directories(zbar INTERFACE ${ZBAR_LIB_DIR})
target_include_directories(zbar INTERFACE ${ZBAR_INCLUDE_DIR})