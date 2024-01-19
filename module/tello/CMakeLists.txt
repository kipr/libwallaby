option(with_tello "Enable tello API" ON)

if(with_tello)
  add_subdirectory(dependencies)
  add_module(tello time camera)
  module_link_libraries(tello wpa_ctrl)
endif()