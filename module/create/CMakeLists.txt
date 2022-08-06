option(with_create "Enable Create API" ON)

if(with_create)
  add_module(create compat core button battery)
endif()