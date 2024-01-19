option(with_graphics "Enable graphics API" ON)

if(with_graphics)
  add_subdirectory(dependencies)
  add_module(graphics)
  module_link_libraries(graphics pixeltoaster export)
endif()
