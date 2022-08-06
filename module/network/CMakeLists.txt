option(with_network "Enable network API" ON)

if(with_network)
  add_module(network)
endif()