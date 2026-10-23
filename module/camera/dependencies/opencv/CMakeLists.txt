include(ExternalProject)

set(OPENCV_DIR ${CMAKE_CURRENT_BINARY_DIR}/dep_opencv)
set(OPENCV_LIB_DIR ${OPENCV_DIR}/lib)
set(OPENCV_INCLUDE_DIR ${OPENCV_DIR}/include)

ExternalProject_Add(dep_opencv
  URL https://github.com/opencv/opencv/archive/refs/tags/4.6.0.tar.gz
  CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DBUILD_SHARED_LIBS=OFF
    -DWITH_FFMPEG=OFF
    -DWITH_1394=OFF
    -DWITH_GSTREAMER=OFF
    -DMSMF=OFF
    -DWITH_DSHOW=OFF
    -DWITH_AVFOUNDATION=OFF
    -DWITH_GTK=OFF
    -DWITH_WIN32UI=OFF
    -DWITH_JAVA=OFF
    -DWITH_ITT=OFF
    -DWITH_IPP=OFF
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
    -DWITH_JPEG=OFF
    -DWITH_TIFF=OFF
    -DCMAKE_INSTALL_PREFIX=${OPENCV_DIR}
    -DOPENCV_GENERATE_PKGCONFIG=YES
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
    -DWITH_CAROTENE=OFF
)


# opencv
add_library(opencv INTERFACE)
add_dependencies(opencv dep_opencv)
target_link_directories(opencv INTERFACE ${OPENCV_LIB_DIR} ${OPENCV_LIB_DIR}/opencv4/3rdparty)
target_include_directories(opencv INTERFACE ${OPENCV_INCLUDE_DIR}/opencv4)
target_link_libraries(opencv INTERFACE dl)

# opencv_core
add_library(opencv_core_wrapper INTERFACE)
target_link_libraries(opencv_core_wrapper INTERFACE opencv opencv_core)

# opencv_imgproc
add_library(opencv_imgproc_wrapper INTERFACE)
target_link_libraries(opencv_imgproc_wrapper INTERFACE opencv opencv_imgproc)

# opencv_flann
add_library(opencv_flann_wrapper INTERFACE)
target_link_libraries(opencv_flann_wrapper INTERFACE opencv opencv_flann)

# opencv_video
add_library(opencv_video_wrapper INTERFACE)
target_link_libraries(opencv_video_wrapper INTERFACE opencv opencv_video)

# opencv_photo
add_library(opencv_photo_wrapper INTERFACE)
target_link_libraries(opencv_photo_wrapper INTERFACE opencv opencv_photo)

# opencv_highgui
add_library(opencv_highgui_wrapper INTERFACE)
target_link_libraries(opencv_highgui_wrapper INTERFACE opencv opencv_highgui)

# opencv_features2d
add_library(opencv_features2d_wrapper INTERFACE)
target_link_libraries(opencv_features2d_wrapper INTERFACE opencv opencv_features2d)

# opencv_calib3d
add_library(opencv_calib3d_wrapper INTERFACE)
target_link_libraries(opencv_calib3d_wrapper INTERFACE opencv opencv_calib3d)

# opencv_videoio
add_library(opencv_videoio_wrapper INTERFACE)
target_link_libraries(opencv_videoio_wrapper INTERFACE opencv opencv_videoio)

# opencv_imgcodecs
add_library(opencv_imgcodecs_wrapper INTERFACE)
target_link_libraries(opencv_imgcodecs_wrapper INTERFACE opencv opencv_imgcodecs)
