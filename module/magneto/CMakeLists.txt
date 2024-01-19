option(with_magneto "Enable magnetometer API" ON)

if(with_magneto)
  add_module(magneto core sensor)
endif()