option(with_compass "Enable compass API" ON)

if(with_compass)
  add_module(compass core magneto time)
endif()