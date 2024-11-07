cmake_minimum_required(VERSION 3.13)

project(libkipr)

option(package_debian "Build a Debian package" OFF)

set(KIPR_VERSION_MAJOR 1)
set(KIPR_VERSION_MINOR 2)
set(KIPR_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -g -O3")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC -O3 -g")


if (EMSCRIPTEN)
  option(wasm "Build for WASM dynamic linking" OFF)
endif()

function(gather_module_objects OUT)
  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
  message(STATUS "Gathering module objects... ${ARGN}")
  set(GATHERED_OBJECTS)
  foreach(MODULE ${ARGN})
    if(TARGET ${MODULE}_objects)
      list(APPEND GATHERED_OBJECTS $<TARGET_OBJECTS:${MODULE}_objects>)
    else()
      message(STATUS "No objects for ${MODULE}")
    endif()
  endforeach()

  set(${OUT} ${GATHERED_OBJECTS} PARENT_SCOPE)
endfunction()

# add all modules
add_subdirectory(module)

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

string(TIMESTAMP KIPR_BUILD_DATETIME UTC)

# Get all modules. These are registered in `add_module`.
get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
set(STATIC_KIPR_MODULES)
foreach(MODULE ${KIPR_MODULES})
  string(TOUPPER ${MODULE} MODULE_UPPER)
  set(KIPR_MODULE_${MODULE_UPPER} ON)
  list(APPEND STATIC_KIPR_MODULES ${MODULE})
endforeach()

configure_file(include/kipr/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/kipr/config.h)

# Build the library.
gather_module_objects(KIPR_MODULE_OBJECTS ${STATIC_KIPR_MODULES})
message(STATUS "KIPR module objects are: ${STATIC_KIPR_MODULES} ${KIPR_MODULE_OBJECTS}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp "")

# add bindings, which require the compiled objects
set_property(GLOBAL PROPERTY kipr_module_objects ${KIPR_MODULE_OBJECTS})
add_subdirectory(binding)

if(EMSCRIPTEN)
  if (wasm)
    add_executable(kipr ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp ${KIPR_MODULE_OBJECTS})
    set_target_properties(kipr PROPERTIES SUFFIX .wasm)
    target_link_options(kipr PRIVATE
      -sSIDE_MODULE
      -sLINKABLE
      -sNO_ERROR_ON_UNDEFINED_SYMBOLS
      -sSTANDALONE_WASM
      -sWASM_BIGINT
      -sLIBRARY_DEBUG
      --no-entry
    )
  else()
    add_library(kipr SHARED ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp ${KIPR_MODULE_OBJECTS})
  endif()
  
  target_link_libraries(kipr PRIVATE ${STATIC_KIPR_MODULES})
else()
  add_library(kipr SHARED ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp ${KIPR_MODULE_OBJECTS})
  if (UNIX AND NOT APPLE)
    target_link_libraries(kipr PRIVATE "-Wl,-Bsymbolic" ${STATIC_KIPR_MODULES})
  endif()
endif()
target_link_libraries(kipr INTERFACE ${STATIC_KIPR_MODULES})
get_property(LINKER_OPTIONS GLOBAL PROPERTY kipr_linker_options)
target_link_options(kipr PRIVATE ${LINKER_OPTIONS})

if (UNIX AND NOT APPLE)
  target_link_options(kipr PRIVATE -static-libgcc -static-libstdc++)  # since wombat is still on jessie, needs the latest g++, gcc
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
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm64)
include(CPack)
