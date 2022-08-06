option(with_console "Enable console API" ON)

if(with_console)
  add_module(console export button)
endif()