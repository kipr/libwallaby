file(GLOB_RECURSE SOURCES common/*.c utils/*.c)
add_library(wpa_ctrl_objects OBJECT ${SOURCES})

target_include_directories(wpa_ctrl_objects PUBLIC common)
target_compile_options(wpa_ctrl_objects PRIVATE -fPIC)

add_library(wpa_ctrl INTERFACE)
target_link_libraries(wpa_ctrl INTERFACE wpa_ctrl_objects $<TARGET_OBJECTS:wpa_ctrl_objects>)
target_include_directories(wpa_ctrl INTERFACE common)
