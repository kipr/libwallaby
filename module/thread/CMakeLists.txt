option(with_thread "Enable thread/mutex API" ON)

if(with_threading)
  add_module(thread export)
endif()