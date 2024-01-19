option(with_gyro "Enable gyro API" ON)

if(with_gyro)
  add_module(gyro core time sensor)
endif()