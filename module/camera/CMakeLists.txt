option(with_camera "Enable camera API" ON)

if(with_camera)
  add_module(camera export geometry color config)
  add_subdirectory(dependencies)
  target_link_libraries(camera PUBLIC opencv_core)
  target_link_libraries(camera PRIVATE zbar)
endif()