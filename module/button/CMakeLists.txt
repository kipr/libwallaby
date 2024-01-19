option(with_button "Enable button API" ON)

if(with_button)
  add_module(button core compat sensor export digital)
endif()