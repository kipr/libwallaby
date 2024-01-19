option(with_create3 "Enable Create 3 API" ON)
option(create3_build_local "Build Create 3 locally" ON)

if(with_create3)
  add_module(create3)

  if(create3_build_local)  
    include(FetchContent)

    set(server OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(create3_ext
      GIT_REPOSITORY https://github.com/kipr/create3
      GIT_TAG main
    )

    FetchContent_MakeAvailable(create3_ext)

    # Print create3_ext variables
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
      message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()


    # This is very hacky
    add_custom_linker_options(-L${CMAKE_BINARY_DIR}/_deps/capnproto-build/c++/src/capnp)
    add_custom_linker_options(-L${CMAKE_BINARY_DIR}/_deps/capnproto-build/c++/src/kj)
    add_custom_linker_options(-L${create3_ext_BINARY_DIR}/client)
    add_custom_linker_options(-L${create3_ext_BINARY_DIR}/common)
  endif()

  # FIXME: create3 uses CMAKE_PROJECT_NAME to name its targets. This means
  # instead of create3_client, it's libkipr_client. We should remove the usage
  # of CMAKE_PROJECT_NAME in create3.
  add_custom_linker_options(-Wl,--whole-archive -lkj -lkj-async -lcapnp -lcapnp-rpc -lcreate3_common -lcreate3_client -Wl,--no-whole-archive)

  endif()