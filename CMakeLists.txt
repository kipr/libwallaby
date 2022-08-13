cmake_minimum_required(VERSION 3.1)

project(libkipr)

option(package_debian "Build a Debian package" OFF)

set(KIPR_VERSION_MAJOR 1)
set(KIPR_VERSION_MINOR 0)
set(KIPR_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_subdirectory(module)

add_subdirectory(binding)

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

execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE KIPR_GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND git log -1 --format=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE KIPR_GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(include/kipr/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/kipr/config.h)

# Build the library.
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp "")
add_library(kipr SHARED ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(kipr PRIVATE "-Wl,-Bsymbolic" "-Wl,--whole-archive"  ${STATIC_KIPR_MODULES} "-Wl,--no-whole-archive")
target_link_libraries(kipr INTERFACE ${STATIC_KIPR_MODULES})

if (TARGET kipr_python)
  target_link_libraries(kipr PRIVATE kipr_python)
endif()

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/ DESTINATION include)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include PATTERN "*.in" EXCLUDE)
install(TARGETS kipr DESTINATION lib)

add_subdirectory(documentation)


set(CPACK_PACKAGE_NAME kipr)
set(CPACK_GENERATOR DEB ZIP)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KISS Institute for Practical Robotics")
set(CPACK_PACKAGE_VERSION_MAJOR ${KIPR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${KIPR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${KIPR_VERSION_PATCH})
include(CPack)