option(with_analog "Enable analog API" ON)

if(with_analog)
  add_module(analog core sensor)
endif()