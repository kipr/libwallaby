cmake_minimum_required(VERSION 2.8)

project(libwallaby)

set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)

include_directories(include/)

add_library(wallaby STATIC
    src/battery_c.cpp
    src/battery.cpp
    src/wallaby_p.cpp)

add_executable(battery_test_c tests/battery_test.c)

target_link_libraries(battery_test_c wallaby)
