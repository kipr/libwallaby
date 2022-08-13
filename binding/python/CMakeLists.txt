option(with_python_binding "Build python bindings" ON)

if(with_python_binding)
  find_package(SWIG REQUIRED)
  find_package(PythonLibs 3 REQUIRED)

  set(KIPR_PYTHON_WRAPPER ${CMAKE_CURRENT_BINARY_DIR}/kipr_python.cpp)

  set(PACKAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/package)

  make_directory(${PACKAGE_DIR}/src/kipr)

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
  add_dependencies(kipr_python swig_kipr_python)
  target_link_libraries(kipr_python PRIVATE ${PYTHON_LIBRARIES})
  target_include_directories(kipr_python PRIVATE ${PYTHON_INCLUDE_DIRS})
  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
  target_link_libraries(kipr_python PRIVATE ${KIPR_MODULES})


  file(COPY pyproject.toml DESTINATION ${PACKAGE_DIR})
  file(COPY ${CMAKE_CURRENT_BINARY_DIR}/kipr.py DESTINATION ${PACKAGE_DIR}/src/kipr)
  configure_file(setup.cfg.in ${PACKAGE_DIR}/setup.cfg)
  file(WRITE ${PACKAGE_DIR}/src/kipr/__init__.py "")
endif()