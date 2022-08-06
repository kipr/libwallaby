option(with_servo "Enable servo API" ON)

if(with_servo)
  add_module(servo core)
endif()