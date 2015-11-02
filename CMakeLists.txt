cmake_minimum_required(VERSION 2.8)

project(libwallaby)

set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)

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

include_directories(include/)

# TODO: glob input files
add_library(wallaby STATIC
    src/battery_c.cpp
    src/battery.cpp
    src/digital_c.cpp
    src/digital.cpp
    src/digital_p.cpp
    src/wallaby_p.cpp)

add_executable(battery_test_c tests/battery_test.c)
target_link_libraries(battery_test_c wallaby)

add_executable(led_test_c tests/led_test.c)
target_link_libraries(led_test_c wallaby)
