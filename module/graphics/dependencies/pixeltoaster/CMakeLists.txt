file(GLOB PIXELTOASTER_SOURCES *.cpp)

if(APPLE)
  file(GLOB OBJC_SOURCES *.mm)
elseif(WIN32)
  add_definitions(-fno-check-new)
endif()

add_library(pixeltoaster STATIC ${PIXELTOASTER_SOURCES} ${OBJC_SOURCES})
target_include_directories(pixeltoaster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})