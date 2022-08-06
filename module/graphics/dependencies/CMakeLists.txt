file(GLOB PIXELTOASTER_SOURCES ${PIXELTOASTER}/*.cpp)

if(APPLE)
  file(GLOB OBJC_SOURCES ${PIXELTOASTER}/*.mm)
elseif(WIN32)
  add_definitions(-fno-check-new)
endif()

add_library(pixeltoaster STATIC ${PIXELTOASTER_SOURCES} ${OBJC_SOURCES})