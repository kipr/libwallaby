option(with_camera "Enable camera API" ON)

if(with_camera)
  add_module(camera export geometry)
  add_subdirectory(dependencies)
endif()