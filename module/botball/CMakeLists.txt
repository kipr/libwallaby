option(with_botball "Enable botball API" ON)

if(with_botball)
  add_module(botball core thread analog button console time create3)
endif()