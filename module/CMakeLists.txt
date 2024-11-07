option(with_tests "Enable tests" ON)

function(add_module NAME)
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*)

  # If there are no sources, this is an include-only module
  list(LENGTH SOURCES SOURCES_LEN)

  add_library(${NAME} INTERFACE)
  if (SOURCES_LEN GREATER 0)
    add_library(${NAME}_objects OBJECT ${SOURCES})
    target_compile_options(${NAME}_objects PRIVATE -fPIC)
    target_include_directories(${NAME}_objects PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/public
      ${CMAKE_CURRENT_SOURCE_DIR}/protected
    )
    target_link_libraries(${NAME}_objects PRIVATE ${ARGN})
    target_link_libraries(${NAME} INTERFACE m)
  endif()

  target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/public)
  target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/protected)
  target_link_libraries(${NAME} INTERFACE ${ARGN})
  
  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
  set_property(GLOBAL PROPERTY kipr_modules ${KIPR_MODULES} ${NAME})

  
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
  endif()

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/public)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/public/ DESTINATION include)
  endif()

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/binding.i)
    get_property(KIPR_BINDINGS GLOBAL PROPERTY kipr_bindings)
    set_property(GLOBAL PROPERTY kipr_bindings ${KIPR_BINDINGS} ${NAME})
  endif()
endfunction()

function(module_link_libraries NAME)
  target_link_libraries(${NAME}_objects PRIVATE ${ARGN})
  target_link_libraries(${NAME} INTERFACE ${ARGN})
endfunction()

function(add_test NAME)
  if(with_tests AND NOT EMSCRIPTEN)
    file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)
    set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
    add_executable(${NAME} ${SOURCES})

    target_link_libraries(${NAME} PUBLIC kipr)
  endif()
endfunction()

function(add_custom_linker_options)
  get_property(LINKER_OPTIONS GLOBAL PROPERTY kipr_linker_options)
  list(APPEND LINKER_OPTIONS ${ARGN})
  set_property(GLOBAL PROPERTY kipr_linker_options ${LINKER_OPTIONS})
endfunction()

add_subdirectory(accel)
add_subdirectory(analog)
add_subdirectory(audio)
add_subdirectory(battery)
add_subdirectory(botball)
add_subdirectory(button)
add_subdirectory(camera)
add_subdirectory(color)
add_subdirectory(compat)
add_subdirectory(compass)
add_subdirectory(config)
add_subdirectory(console)
add_subdirectory(core)
add_subdirectory(create)
add_subdirectory(digital)
add_subdirectory(export)
add_subdirectory(geometry)
add_subdirectory(graphics)
add_subdirectory(gyro)
add_subdirectory(log)
add_subdirectory(magneto)
add_subdirectory(motor)
add_subdirectory(network)
add_subdirectory(sensor)
add_subdirectory(servo)
add_subdirectory(tello)
add_subdirectory(thread)
add_subdirectory(time)
add_subdirectory(wait_for)
