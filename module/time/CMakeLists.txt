option(with_time "Enable time API" ON)

if(with_time)
  add_module(time compat)
endif()