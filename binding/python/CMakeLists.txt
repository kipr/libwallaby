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
      -outdir ${PACKAGE_DIR}/src/kipr
      ${KIPR_SWIG_OPTIONS}
      ${KIPR_SWIG_WRAPPER}
    SOURCES ${KIPR_SWIG_WRAPPER} ${CMAKE_BINARY_DIR}/include/kipr/config.h
    BYPRODUCTS ${KIPR_PYTHON_WRAPPER} ${PACKAGE_DIR}/src/kipr/kipr.py
  )
  
  # get required global properties
  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
  get_property(KIPR_MODULE_OBJECTS GLOBAL PROPERTY kipr_module_objects)

  # add python binding library
  add_library(kipr_python SHARED ${KIPR_PYTHON_WRAPPER})
  add_dependencies(kipr_python swig_kipr_python ${KIPR_MODULES})
  if (NOT EMSCRIPTEN)
    target_link_libraries(kipr_python PRIVATE ${PYTHON_LIBRARIES})
  endif()
  target_include_directories(kipr_python PRIVATE ${PYTHON_INCLUDE_DIRS})
  
  # link library to kipr objects
  target_link_libraries(kipr_python PRIVATE ${KIPR_MODULES})
  target_link_libraries(kipr_python PRIVATE "-Wl,-Bsymbolic" ${KIPR_MODULE_OBJECTS})

  # according to swig specifications, the output library must be
  # named _kipr.so
  set_target_properties(kipr_python PROPERTIES OUTPUT_NAME _kipr)
  set_target_properties(kipr_python PROPERTIES PREFIX "")

  file(COPY pyproject.toml DESTINATION ${PACKAGE_DIR})
  configure_file(setup.cfg.in ${PACKAGE_DIR}/setup.cfg)
  file(WRITE ${PACKAGE_DIR}/src/kipr/__init__.py "")

  # install necessary .so file and .py file to /usr/lib
  install(TARGETS kipr_python DESTINATION lib)
  install(FILES ${PACKAGE_DIR}/src/kipr/kipr.py DESTINATION lib)
endif()