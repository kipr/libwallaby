include(ExternalProject)

set(OPENCV_CONTRIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/dep_opencv_contrib-prefix/src/dep_opencv_contrib)

ExternalProject_Add(dep_opencv_contrib
  URL https://github.com/opencv/opencv_contrib/archive/refs/tags/4.6.0.tar.gz
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

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
    -DWITH_PNG=OFF
    -DWITH_TIFF=OFF
    -DCMAKE_INSTALL_PREFIX=${OPENCV_DIR}
    -DOPENCV_EXTRA_MODULES_PATH=${OPENCV_CONTRIB_DIR}/modules
    -DBUILD_opencv_dnn=OFF
    -DBUILD_opencv_flann=ON
    -DBUILD_opencv_features2d=ON
    -DBUILD_opencv_gapi=OFF
    -DBUILD_opencv_ml=OFF
    -DBUILD_opencv_objdetect=OFF
    -DBUILD_opencv_stitching=OFF
    -DBUILD_opencv_alphamat=OFF
    -DBUILD_opencv_aruco=ON
    -DBUILD_opencv_barcode=OFF
    -DBUILD_opencv_bgsegm=OFF
    -DBUILD_opencv_bioinspired=OFF
    -DBUILD_opencv_ccalib=OFF
    -DBUILD_opencv_cnn_3dobj=OFF
    -DBUILD_opencv_cubaarithm=OFF
    -DBUILD_opencv_cudacodec=OFF
    -DBUILD_opencv_cudafeatures2d=OFF
    -DBUILD_opencv_cudafilters=OFF
    -DBUILD_opencv_cudaimgproc=OFF
    -DBUILD_opencv_cudalegacy=OFF
    -DBUILD_opencv_cudaobjdetect=OFF
    -DBUILD_opencv_cudaoptflow=OFF
    -DBUILD_opencv_cudastereo=OFF
    -DBUILD_opencv_cudawarping=OFF
    -DBUILD_opencv_cudev=OFF
    -DBUILD_opencv_cvv=OFF
    -DBUILD_opencv_datasets=OFF
    -DBUILD_opencv_dnn_objdetect=OFF
    -DBUILD_opencv_dnn_superres=OFF
    -DBUILD_opencv_dnn_easily_fooled=OFF
    -DBUILD_opencv_dpm=OFF
    -DBUILD_opencv_face=OFF
    -DBUILD_opencv_freetype=OFF
    -DBUILD_opencv_fuzzy=OFF
    -DBUILD_opencv_hdf=OFF
    -DBUILD_opencv_hfs=OFF
    -DBUILD_opencv_img_hash=OFF
    -DBUILD_opencv_intensity_transform=OFF
    -DBUILD_opencv_julia=OFF
    -DBUILD_opencv_line_descriptor=OFF
    -DBUILD_opencv_matlab=OFF
    -DBUILD_opencv_mcc=OFF
    -DBUILD_opencv_optflow=OFF
    -DBUILD_opencv_ovis=OFF
    -DBUILD_opencv_phase_unwrapping=OFF
    -DBUILD_opencv_plot=OFF
    -DBUILD_opencv_quality=OFF
    -DBUILD_opencv_rapid=OFF
    -DBUILD_opencv_reg=OFF
    -DBUILD_opencv_rgbd=OFF
    -DBUILD_opencv_saliency=OFF
    -DBUILD_opencv_sfm=OFF
    -DBUILD_opencv_shape=OFF
    -DBUILD_opencv_stereo=OFF
    -DBUILD_opencv_structured_light=OFF
    -DBUILD_opencv_superres=OFF
    -DBUILD_opencv_surface_matching=OFF
    -DBUILD_opencv_text=OFF
    -DBUILD_opencv_tracking=OFF
    -DBUILD_opencv_videostab=OFF
    -DBUILD_opencv_viz=OFF
    -DBUILD_opencv_wechat_qrcode=OFF
    -DBUILD_opencv_xfeatures2d=OFF
    -DBUILD_opencv_ximgproc=OFF
    -DBUILD_opencv_xobjdetect=OFF
    -DBUILD_opencv_xphoto=OFF
)

add_dependencies(dep_opencv dep_opencv_contrib)

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