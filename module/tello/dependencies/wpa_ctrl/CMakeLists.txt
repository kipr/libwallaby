file(GLOB_RECURSE SOURCES common/*.c utils/*.c)
add_library(wpa_ctrl STATIC ${SOURCES})
target_include_directories(wpa_ctrl PUBLIC common)