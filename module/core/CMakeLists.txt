file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*)

if (EMSCRIPTEN)
  if (wasm)
    file(GLOB EMSCRIPTEN_FS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/device/emscripten_fs/*.c*)
    list(APPEND SOURCES ${EMSCRIPTEN_FS_SOURCES})
  else()
    file(GLOB EMSCRIPTEN_JS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/device/emscripten_js/*.c*)
    list(APPEND SOURCES ${EMSCRIPTEN_JS_SOURCES})
  endif()
elseif(DUMMY)
  file(GLOB DUMMY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/device/dummy/*.c*)
  list(APPEND SOURCES ${DUMMY_SOURCES})
else()
  file(GLOB WOMBAT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/device/wombat/*.c*)
  list(APPEND SOURCES ${WOMBAT_SOURCES})
endif()


add_library(core INTERFACE)

add_library(core_objects OBJECT ${SOURCES})
target_compile_options(core_objects PRIVATE -fPIC)
target_include_directories(core_objects PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/public
  ${CMAKE_CURRENT_SOURCE_DIR}/protected
  ${CMAKE_BINARY_DIR}/include
)
target_link_libraries(core_objects PRIVATE log)
target_link_libraries(core INTERFACE m)

target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/protected)
target_link_libraries(core INTERFACE pthread log)

get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
set_property(GLOBAL PROPERTY kipr_modules ${KIPR_MODULES} core)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/public)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/public/ DESTINATION include)
endif()

get_property(KIPR_BINDINGS GLOBAL PROPERTY kipr_bindings)
set_property(GLOBAL PROPERTY kipr_bindings ${KIPR_BINDINGS} core)
