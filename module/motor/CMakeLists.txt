option(with_motor "Enable motor API" ON)

if(with_motor)
  add_module(motor core export time)
endif()
