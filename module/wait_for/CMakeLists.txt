option(with_wait_for "Enable wait_for API" ON)

if(with_wait_for)
  add_module(wait_for export button time digital)
endif()