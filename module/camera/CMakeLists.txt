option(with_camera "Enable camera API" ON)

if(with_camera)
  add_subdirectory(dependencies)
  add_module(camera export geometry color config log)
  target_link_libraries(camera PUBLIC
    opencv_videoio_wrapper
    opencv_imgcodecs_wrapper
    opencv_imgproc_wrapper
    opencv_core_wrapper
    jpeg_wrapper
    z
    zbar_wrapper
  )
endif()