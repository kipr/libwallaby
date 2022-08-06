function(add_module NAME)
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*)

  # If there are no sources, this is an include-only module
  list(LENGTH SOURCES SOURCES_LEN)

  if (SOURCES_LEN GREATER 0)
    add_library(${NAME} OBJECT ${SOURCES})
    target_compile_options(${NAME} PRIVATE -fPIC)

    target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
    target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/protected)
  else()
    add_library(${NAME} INTERFACE)
    target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/public)
    target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/protected)
  endif()

  target_link_libraries(${NAME} ${ARGN})

  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
  set_property(GLOBAL PROPERTY kipr_modules ${KIPR_MODULES} ${NAME})
endfunction()

add_subdirectory(analog)
add_subdirectory(button)
add_subdirectory(camera)
add_subdirectory(compat)
add_subdirectory(console)
add_subdirectory(core)
add_subdirectory(digital)
add_subdirectory(export)
add_subdirectory(geometry)
add_subdirectory(network)
add_subdirectory(motor)
add_subdirectory(sensor)
add_subdirectory(servo)
add_subdirectory(thread)
