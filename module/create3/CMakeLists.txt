option(with_create3 "Enable Create 3 API" ON)

if(with_create3)
  add_module(create3)

  include(create3_ext)
  include(create3_client)
  # include(FetchContent)

  # set(server OFF CACHE BOOL "" FORCE)

  # FetchContent_Declare(create3_ext
  #   GIT_REPOSITORY https://github.com/kipr/create3
  #   GIT_TAG main
  # )

  # FetchContent_MakeAvailable(create3_ext)

  # FIXME: create3 uses CMAKE_PROJECT_NAME to name its targets. This means
  # instead of create3_client, it's libkipr_client. We should remove the usage
  # of CMAKE_PROJECT_NAME in create3.
  target_link_libraries(create3 INTERFACE create3_client)
endif()