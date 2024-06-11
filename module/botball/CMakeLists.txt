option(with_botball "Enable botball API" ON)

if(with_botball)
  add_module(analog botball button console core graphics thread time)
endif()