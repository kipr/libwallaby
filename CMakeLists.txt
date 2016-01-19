cmake_minimum_required(VERSION 2.8)

project(libwallaby)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


set(INCLUDE ${libwallaby_SOURCE_DIR}/include)
set(SRC ${libwallaby_SOURCE_DIR}/src)

include_directories(${INCLUDE})


file(GLOB SOURCES ${SRC}/*.cpp)
file(GLOB C_TESTS tests/*.c)

# Cross-compile
# set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
# set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)


# C++11
# http://www.guyrutenberg.com/2014/01/05/enabling-c11-c0x-in-cmake/
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
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

find_package( OpenCV REQUIRED )
find_package (Threads)


include_directories(${CMAKE_INSTALL_PREFIX}/include)
include_directories(${OPENCV_INCLUDE})
include_directories(${ZBAR_DIR}/include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${OPENCV_LIB})
link_directories(${ZBAR_DIR}/lib)  


# Library: libwallaby
add_library(wallaby SHARED
	${SOURCES})
	
target_link_libraries(wallaby ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if(UNIX AND NOT APPLE)
    target_link_libraries(wallaby rt)
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

