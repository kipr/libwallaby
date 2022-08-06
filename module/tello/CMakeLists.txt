option(with_tello "Enable tello API" ON)

if(with_tello)
  add_subdirectory(dependencies)
  add_module(tello time camera)
  target_link_libraries(tello PRIVATE wpa_ctrl)
endif()