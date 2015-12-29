find_package(BSON REQUIRED)

find_path(DAYLITE_INCLUDE_DIR daylite/node.hpp)
find_library(DAYLITE_LIBRARY NAMES daylite libdaylite)
if(WIN32)
  find_program(DAYLITE_DLL daylite.dll)
endif()

set(DAYLITE_LIBRARIES ${DAYLITE_LIBRARY} ${BSON_LIBRARIES})
set(DAYLITE_INCLUDE_DIRS ${DAYLITE_INCLUDE_DIR} ${BSON_INCLUDE_DIRS})
if(WIN32)
  set(DAYLITE_DLLS ${DAYLITE_DLL} ${BSON_DLLS})
endif()

include(FindPackageHandleStandardArgs)

if(WIN32)
  find_package_handle_standard_args(Daylite DEFAULT_MSG DAYLITE_LIBRARY DAYLITE_INCLUDE_DIR DAYLITE_DLL)
  mark_as_advanced(DAYLITE_INCLUDE_DIR DAYLITE_LIBRARY DAYLITE_DLL)
else()
  find_package_handle_standard_args(Daylite DEFAULT_MSG DAYLITE_LIBRARY DAYLITE_INCLUDE_DIR)
  mark_as_advanced(DAYLITE_INCLUDE_DIR DAYLITE_LIBRARY)
endif()
