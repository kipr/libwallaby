add_module(core)

target_link_libraries(core INTERFACE pthread)
target_include_directories(core_objects PRIVATE ${CMAKE_BINARY_DIR}/include)