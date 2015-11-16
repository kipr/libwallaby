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


include_directories(${CMAKE_INSTALL_PREFIX}/include)
include_directories(${OPENCV_INCLUDE})
include_directories(${ZBAR_DIR}/include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)
link_directories(${OPENCV_LIB})
link_directories(${ZBAR_DIR}/lib)  


# Library: libwallaby
add_library(wallaby STATIC
	${SOURCES})


# Executables: test programs
# TODO: use a list and foreach to make this more concise
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

add_executable(motor_mtp_test_c tests/motor_mtp_test.c)
target_link_libraries(motor_mtp_test_c wallaby)

add_executable(motor_mrp_test_c tests/motor_mrp_test.c)
target_link_libraries(motor_mrp_test_c wallaby)

add_executable(audio_test_c tests/audio_test.c)
target_link_libraries(audio_test_c wallaby)

add_executable(ir_test_c tests/ir_test.c)
target_link_libraries(ir_test_c wallaby)

add_executable(button_test_c tests/button_test.c)
target_link_libraries(button_test_c wallaby)
