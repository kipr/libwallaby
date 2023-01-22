option(with_xml_binding "Build XML bindings" ON)

if(with_xml_binding)
  find_package(SWIG REQUIRED)

  add_custom_target(
    swig_kipr_xml
    ALL
    COMMAND ${SWIG_EXECUTABLE}
      -xml
      -o ${CMAKE_CURRENT_BINARY_DIR}/kipr.xml
      -I${CMAKE_BINARY_DIR}/include
      -I${CMAKE_SOURCE_DIR}/include
      -I${CMAKE_SOURCE_DIR}
      -outdir ${CMAKE_CURRENT_BINARY_DIR}
      ${KIPR_SWIG_OPTIONS}
      ${KIPR_SWIG_WRAPPER}
    SOURCES ${KIPR_SWIG_WRAPPER} ${CMAKE_BINARY_DIR}/include/kipr/config.h
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/kipr.xml
  )
endif()