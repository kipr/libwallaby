cmake_minimum_required(VERSION 2.8)

project(libwallaby)

#set(ENV{OPENCV_INCLUDE_DIR} /home/root/Documents/KISS/bin/wallaby7/libwallaby/opencv/include)
#set(OpenCV_DIR /home/root/Documents/KISS/bin/wallaby7/libwallaby/opencv/cmake)

option(no_wallaby "Not a wallaby controller" OFF)

if(no_wallaby)
  add_definitions(-DNOT_A_WALLABY)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


set(INCLUDE ${libwallaby_SOURCE_DIR}/include)
set(SRC ${libwallaby_SOURCE_DIR}/src)
set(PIXELTOASTER ${SRC}/pixeltoaster)
set(BINDINGS ${CMAKE_SOURCE_DIR}/swig)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

include_directories(${INCLUDE})


file(GLOB SOURCES ${SRC}/*.cpp)
file(GLOB C_TESTS tests/*.c)
file(GLOB PIXELTOASTER_SOURCES ${PIXELTOASTER}/*.cpp)

# Cross-compile
# set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
# set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)


# C++11
# http://www.guyrutenberg.com/2014/01/05/enabling-c11-c0x-in-cmake/
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	add_definitions(--std=c++11)
elseif(COMPILER_SUPPORTS_CXX0X)
	add_definitions(--std=c++0x)
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


# REMOVE ME
include_directories(${SRC}/pixeltoaster)

#############################
#   PixelToaster settings   #
#############################

if(APPLE)
	file(GLOB OBJC_SOURCES ${PIXELTOASTER}/*.mm)
elseif(WIN32)
	add_definitions(-fno-check-new)
endif()



if(DEFINED ENV{OPENCV_INCLUDE_DIR})
  set(OPENCV_INCLUDE $ENV{OPENCV_INCLUDE_DIR})
elseif(WIN32)
  set(OPENCV_INCLUDE ${CMAKE_SOURCE_DIR}/../opencv/build/install/include)
endif()

if(DEFINED ENV{OPENCV_LIB_DIR})
  set(OPENCV_LIB $ENV{OPENCV_LIB_DIR})
elseif(WIN32)
  set(OPENCV_LIB ${CMAKE_SOURCE_DIR}/../opencv/build/install/x86/vc11/lib)
endif()

if(DEFINED ENV{ZBAR_DIR})
  set(ZBAR_DIR $ENV{ZBAR_DIR})
elseif(WIN32)
  set(ZBAR_DIR "C:/Program Files (x86)/ZBar")
endif()

#find_package(OpenCV REQUIRED )
find_package(JPEG REQUIRED)
find_package (Threads)


include_directories(${CMAKE_INSTALL_PREFIX}/include)
include_directories(${OPENCV_INCLUDE})
include_directories(${JPEG_INCLUDE_DIR})
include_directories(${ZBAR_DIR}/include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${OPENCV_LIB})
link_directories(${ZBAR_DIR}/lib)

option(build_python "Build python bindings" ON)

file(GLOB SWIG_SOURCES ${BINDINGS}/*.i)

if(build_python)
  find_package(SWIG REQUIRED)
  find_package(PythonLibs REQUIRED)
  message("${PYTHON_LIBRARY}")
  set(PYTHON_BINDING_SOURCES ${CMAKE_BINARY_DIR}/wallaby_wrap_py.cxx)
  set(PYTHON_BINDING_LIBRARIES ${PYTHON_LIBRARY})
  include_directories(${PYTHON_INCLUDE_DIR})
  add_custom_command(COMMAND ${SWIG_EXECUTABLE} -Wall -python -c++ -I${INCLUDE} -outdir ${LIBRARY_OUTPUT_PATH} -o ${CMAKE_BINARY_DIR}/wallaby_wrap_py.cxx ${BINDINGS}/wallaby.i
    DEPENDS ${SWIG_SOURCES}
    OUTPUT ${CMAKE_BINARY_DIR}/wallaby_wrap_py.cxx ${LIBRARY_OUTPUT_PATH}/wallaby.py)
endif()

if(build_python)
  add_library(wallaby_python SHARED ${PYTHON_BINDING_SOURCES})
  set_target_properties(wallaby_python PROPERTIES PREFIX _ OUTPUT_NAME wallaby SUFFIX .so)
  target_link_libraries(wallaby_python wallaby ${PYTHON_BINDING_LIBRARIES} )
endif()


# Library: libwallaby
add_library(wallaby SHARED
	${SOURCES} ${PIXELTOASTER_SOURCES} ${OBJC_SOURCES})
  
target_link_libraries(wallaby ${OpenCV_LIBS} ${JPEG_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} zbar opencv_core opencv_highgui opencv_imgproc)

if(APPLE)
  find_library(OPENGL_LIBRARY OpenGL)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(CORESERVICE_LIBRARY CoreServices)
  target_link_libraries(wallaby ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${CORESERVICE_LIBRARY})
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(wallaby rt X11)
endif()


# Executables: test programs
# TODO: use a list and foreach to make this more concise
set(TESTS )

foreach(C_TEST ${C_TESTS})
  get_filename_component(C_TEST_NAME ${C_TEST} NAME_WE)
  set(TEST_BIN_NAME ${C_TEST_NAME}_c)
  add_executable(${TEST_BIN_NAME} ${C_TEST})
  target_link_libraries(${TEST_BIN_NAME} wallaby)
  list(APPEND TESTS ${TEST_BIN_NAME})
endforeach()

install(TARGETS ${TESTS} DESTINATION bin/wallaby)
install(TARGETS wallaby DESTINATION lib)
install(DIRECTORY ${INCLUDE}/wallaby DESTINATION include)

set(CMAKE_INSTALL_PREFIX /usr)
