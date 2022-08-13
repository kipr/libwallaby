option(with_python_binding "Build python bindings" ON)

if(with_python_binding)
  find_package(SWIG REQUIRED)
  find_package(PythonLibs 3 REQUIRED)

  set(KIPR_PYTHON_WRAPPER ${CMAKE_CURRENT_BINARY_DIR}/kipr_python.cpp)

  add_custom_target(
    swig_kipr_python
    COMMAND ${SWIG_EXECUTABLE}
      -python
      -o ${KIPR_PYTHON_WRAPPER}
      -I${CMAKE_BINARY_DIR}/include
      -I${CMAKE_SOURCE_DIR}/include
      -I${CMAKE_SOURCE_DIR}
      ${KIPR_SWIG_OPTIONS}
      ${KIPR_SWIG_WRAPPER}
    SOURCES ${KIPR_SWIG_WRAPPER}
    BYPRODUCTS ${KIPR_PYTHON_WRAPPER} ${CMAKE_CURRENT_BINARY_DIR}/kipr.py
  )

  add_library(kipr_python STATIC ${KIPR_PYTHON_WRAPPER})
  target_link_libraries(kipr_python PRIVATE ${PYTHON_LIBRARIES})
  target_include_directories(kipr_python PRIVATE ${PYTHON_INCLUDE_DIRS})

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/kipr.py
    DESTINATION lib/python${PYTHONLIBS_VERSION_STRING}/site-packages/
  )
endif()