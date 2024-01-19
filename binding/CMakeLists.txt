set(KIPR_SWIG_WRAPPER ${CMAKE_CURRENT_SOURCE_DIR}/kipr.i)

set(KIPR_SWIG_OPTIONS)
get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)

foreach(MODULE ${KIPR_MODULES})
    list(APPEND KIPR_SWIG_OPTIONS -I${CMAKE_SOURCE_DIR}/module/${MODULE}/public)
endforeach()

add_subdirectory(python)
add_subdirectory(xml)