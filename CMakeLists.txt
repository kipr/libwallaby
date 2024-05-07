cmake_minimum_required(VERSION 3.13)

project(libkipr)

option(package_debian "Build a Debian package" OFF)

set(KIPR_VERSION_MAJOR 1)
set(KIPR_VERSION_MINOR 0)
set(KIPR_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -g -O3")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC -O3 -g")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/deploy)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(INCLUDE ${CMAKE_SOURCE_DIR}/include/kipr)
set(WOMBAT ${DEVICES}/kipr)
set(RC ${CMAKE_SOURCE_DIR}/rc)
set(SRC ${CMAKE_SOURCE_DIR}/src)
set(UI ${CMAKE_SOURCE_DIR}/ui)
set(TS ${CMAKE_SOURCE_DIR}/ts)
set(CMAKE_PREFIX_PATH ~/Qt/6.2.4/gcc_64/lib/cmake)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
	${DBUS}/include
	${CMAKE_BINARY_DIR}
  ${DEVICE_DIR}/include
	${INCLUDE}
)


include_directories(~/Qt/6.2.4/gcc_64/include)
include_directories(/usr/local/include)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Quick QuickWidgets QuickControls2)

find_package(OpenSSL REQUIRED)

file(GLOB INCLUDES ${INCLUDE}/*.h ${DEVICE_DIR}/include/*.h)
file(GLOB SOURCES ${SRC}/*.cpp ${DEVICE_DIR}/src/*.cpp)
file(GLOB UIS ${UI}/*)
file(GLOB TS_FILES ${TS}/*.ts)
file(GLOB DBUS_INTERFACES ${DBUS}/*.xml)


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
    add_executable(kipr ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp ${KIPR_MODULE_OBJECTS} ${SRCS_CXX} ${QM_FILES} ${DBUS_INTERFACE_SOURCES})
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
    add_library(kipr SHARED ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp ${KIPR_MODULE_OBJECTS} ${SRCS_CXX} ${QM_FILES} ${DBUS_INTERFACE_SOURCES})
  endif()
  
  target_link_libraries(kipr PRIVATE ${STATIC_KIPR_MODULES})
else()
  add_library(kipr SHARED ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp ${KIPR_MODULE_OBJECTS})
  if (UNIX AND NOT APPLE)
    target_link_libraries(kipr PRIVATE "-Wl,-Bsymbolic" ${STATIC_KIPR_MODULES})
  endif()
endif()
add_subdirectory(dbus)
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
include_directories(~/Qt/6.2.4/gcc_64/include)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Quick QuickWidgets QuickControls2)

find_package(OpenSSL REQUIRED)


set(UI ${UIS})

set(QRC_FILES
	${RC}/icons.qrc
	${RC}/construction.qrc
	${RC}/qml.qrc
	${RC}/fonts.qrc
	${RC}/target.qrc
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(MOC_SRCS ${INCLUDES})
set(SRCS_CXX ${SOURCES})
qt6_wrap_cpp(SRCS_CXX ${MOC_SRCS} OPTIONS -DQT_VERSION=0x060000 -UQT_DBUS_LIB)
qt6_wrap_ui(SRCS_CXX ${UI})
qt6_add_resources(SRCS_CXX ${QRC_FILES})
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Quick QuickWidgets QuickControls2)
include_directories(${Qt6Core_INCLUDE_DIRS})
target_link_libraries(kipr PUBLIC
	Qt6::Core
	Qt6::Gui
	Qt6::Quick
	Qt6::QuickWidgets
	Qt6::Widgets
	Qt6::QuickControls2
	network_manager_dbus
	pcompiler
	z
	${OPENSSL_LIBRARIES}
	create3_client
)

target_link_libraries(kipr PUBLIC ${QT_LIBRARIES})


set(CPACK_PACKAGE_NAME kipr)
set(CPACK_GENERATOR DEB ZIP)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KISS Institute for Practical Robotics")
set(CPACK_PACKAGE_VERSION_MAJOR ${KIPR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${KIPR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${KIPR_VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm64)
include(CPack)
