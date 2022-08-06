option(with_accel "Enable accel API" ON)

if(with_accel)
  add_module(accel core time sensor)
endif()