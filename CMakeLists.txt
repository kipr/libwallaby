cmake_minimum_required(VERSION 3.1)

project(libkipr)

set(KIPR_VERSION_MAJOR 1)
set(KIPR_VERSION_MINOR 0)
set(KIPR_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(module)

# Get all modules. These are registered in `add_module`.
get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
set(STATIC_KIPR_MODULES)
foreach(MODULE ${KIPR_MODULES})
  string(TOUPPER ${MODULE} MODULE_UPPER)
  set(KIPR_MODULE_${MODULE_UPPER} ON)
  get_target_property(TYPE ${MODULE} TYPE)

  # Some modules are interfaces. We need to prune them.
  if(TYPE STREQUAL STATIC_LIBRARY)
    list(APPEND STATIC_KIPR_MODULES ${MODULE})
  endif()
endforeach()

configure_file(include/kipr/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/kipr/config.h)

# Build the library.
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp "")
add_library(kipr SHARED ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(kipr PUBLIC "-Wl,--whole-archive" ${STATIC_KIPR_MODULES} "-Wl,--no-whole-archive")

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION include)