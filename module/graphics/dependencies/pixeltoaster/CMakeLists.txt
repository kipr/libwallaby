file(GLOB PIXELTOASTER_SOURCES *.cpp)

if(APPLE)
  file(GLOB OBJC_SOURCES *.mm)
elseif(WIN32)
  add_definitions(-fno-check-new)
endif()

add_library(pixeltoaster_objects OBJECT ${PIXELTOASTER_SOURCES} ${OBJC_SOURCES})
add_library(pixeltoaster INTERFACE)
target_include_directories(pixeltoaster_objects PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(pixeltoaster_objects PRIVATE -fPIC -w)

target_link_libraries(pixeltoaster INTERFACE pixeltoaster_objects $<TARGET_OBJECTS:pixeltoaster_objects>)
target_include_directories(pixeltoaster INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


if(UNIX AND NOT APPLE)
  target_link_libraries(pixeltoaster_objects PRIVATE X11)
  target_link_libraries(pixeltoaster INTERFACE X11)
endif()