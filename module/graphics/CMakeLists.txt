option(with_graphics "Enable graphics API" ON)

if(with_graphics)
  add_subdirectory(dependencies)
  add_module(graphics)
  add_subdirectory(test)
endif()