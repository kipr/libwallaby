cmake_minimum_required(VERSION 2.8)

project(libwallaby)

set(INCLUDE ${libwallaby_SOURCE_DIR}/include)
set(SRC ${libwallaby_SOURCE_DIR}/src)

include_directories(${INCLUDE})

file(GLOB SOURCES ${SRC}/*.cpp)


# Cross-compile
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)


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



# Library: libwallaby
add_library(wallaby STATIC
	${SOURCES})


# Executables: test programs
add_executable(battery_test_c tests/battery_test.c)
target_link_libraries(battery_test_c wallaby)

add_executable(led_test_c tests/led_test.c)
target_link_libraries(led_test_c wallaby)

add_executable(comm_speed_test_c tests/comm_speed_test.c)
target_link_libraries(comm_speed_test_c wallaby)

add_executable(analog_test_c tests/analog_test.c)
target_link_libraries(analog_test_c wallaby)

add_executable(imu_test_c tests/imu_test.c)
target_link_libraries(imu_test_c wallaby)

add_executable(servo_test_c tests/servo_test.c)
target_link_libraries(servo_test_c wallaby)

add_executable(motor_pwm_test_c tests/motor_pwm_test.c)
target_link_libraries(motor_pwm_test_c wallaby)

add_executable(motor_mav_test_c tests/motor_mav_test.c)
target_link_libraries(motor_mav_test_c wallaby)