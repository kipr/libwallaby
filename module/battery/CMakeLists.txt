option(with_battery "Enable battery API" ON)

if(with_battery)
  add_module(battery core)
endif()