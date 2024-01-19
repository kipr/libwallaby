option(with_camera "Enable camera API" ON)

if(with_camera)
  add_subdirectory(dependencies)
  add_module(camera export geometry color config log time)
  module_link_libraries(camera
    opencv_highgui_wrapper
    opencv_photo_wrapper
    opencv_videoio_wrapper
    opencv_imgcodecs_wrapper
    opencv_calib3d_wrapper
    opencv_features2d_wrapper
    opencv_imgproc_wrapper
    opencv_flann_wrapper
    opencv_core_wrapper
    jpeg_wrapper
    z_wrapper
    avcodec_wrapper
    avformat_wrapper
    avutil_wrapper
    swscale_wrapper
    dl
  )
endif()