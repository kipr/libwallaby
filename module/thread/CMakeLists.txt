option(with_thread "Enable thread/mutex API" ON)

if(with_thread)
  add_module(thread export)
endif()