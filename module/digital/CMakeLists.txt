option(with_digital "Enable digital API" ON)

if(with_digital)
  add_module(digital core export sensor)
endif()