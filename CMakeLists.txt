cmake_minimum_required(VERSION 3.1)

project(libkipr)

add_subdirectory(module)

# Get all modules. These are registered in `add_module`.
get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
set(TARGET_OBJECTS)
foreach(MODULE ${KIPR_MODULES})
  get_target_property(TYPE ${MODULE} TYPE)

  # Some modules are interfaces. We need to prune them.
  if(TYPE STREQUAL OBJECT_LIBRARY)
    list(APPEND TARGET_OBJECTS $<TARGET_OBJECTS:${MODULE}>)
  endif()
endforeach()

# Build the library.
add_library(kipr SHARED ${TARGET_OBJECTS})