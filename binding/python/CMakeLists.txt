option(with_python_binding "Build python bindings" ON)

if(with_python_binding)
  find_package(SWIG REQUIRED)
  find_package(PythonLibs 3 REQUIRED)

  set(KIPR_PYTHON_WRAPPER ${CMAKE_CURRENT_BINARY_DIR}/kipr_python.cpp)

  set(PACKAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/package)

  file(MAKE_DIRECTORY ${PACKAGE_DIR}/src/kipr)

  add_custom_command(
    # Make sure CMake knows that it needs to produce these files
    OUTPUT
      ${KIPR_PYTHON_WRAPPER}
      ${PACKAGE_DIR}/src/kipr/kipr.py
    COMMAND ${SWIG_EXECUTABLE}
      -python
      -o ${KIPR_PYTHON_WRAPPER}
      -I${CMAKE_BINARY_DIR}/include
      -I${CMAKE_SOURCE_DIR}/include
      -I${CMAKE_SOURCE_DIR}
      -outdir ${PACKAGE_DIR}/src/kipr
      ${KIPR_SWIG_OPTIONS}
      ${KIPR_SWIG_WRAPPER}
    DEPENDS
      ${KIPR_SWIG_WRAPPER}
      ${CMAKE_BINARY_DIR}/include/kipr/config.h
    COMMENT "SWIG: generating kipr Python wrapper and kipr.py"
    VERBATIM
  )

  # get required global properties
  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)
  get_property(KIPR_MODULE_OBJECTS GLOBAL PROPERTY kipr_module_objects)

  # add python binding library
  add_library(kipr_python SHARED ${KIPR_PYTHON_WRAPPER})
  add_dependencies(kipr_python ${KIPR_MODULES})
  if (NOT EMSCRIPTEN)
    target_link_libraries(kipr_python PRIVATE ${PYTHON_LIBRARIES})
  endif()
  # Guarantee kipr_python finds dependencies
  target_include_directories(kipr_python PRIVATE ${PYTHON_INCLUDE_DIRS}
                                             ${CMAKE_BINARY_DIR}/include
                                             ${CMAKE_SOURCE_DIR}/include)

  # link library to kipr objects
  target_link_libraries(kipr_python PRIVATE ${KIPR_MODULES})
  target_link_options(kipr_python PRIVATE "-Wl,-Bsymbolic" ${KIPR_MODULE_OBJECTS})

  # according to swig specifications, the output library must be
  # named _kipr.so
  set_target_properties(kipr_python PROPERTIES
    OUTPUT_NAME _kipr
    PREFIX ""
  )

  file(COPY pyproject.toml DESTINATION ${PACKAGE_DIR})
  configure_file(setup.cfg.in ${PACKAGE_DIR}/setup.cfg)
  file(WRITE ${PACKAGE_DIR}/src/kipr/__init__.py "")

  # install necessary .so file and .py file to /usr/lib
  install(TARGETS kipr_python DESTINATION lib)
  install(FILES ${PACKAGE_DIR}/src/kipr/kipr.py DESTINATION lib)
endif()
