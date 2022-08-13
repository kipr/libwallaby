option(with_documentation "Enable documentation" ON)

if (with_documentation)
  find_package(Doxygen REQUIRED)
  
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_GENERATE_MAN YES)
  set(DOXYGEN_PROJECT_NUMBER ${KIPR_VERSION_MAJOR}.${KIPR_VERSION_MINOR}.${KIPR_VERSION_PATCH})

  set(DOCUMENTATION_DIRECTORIES)
  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)

  foreach(MODULE ${KIPR_MODULES})
      list(APPEND DOCUMENTATION_DIRECTORIES ${CMAKE_SOURCE_DIR}/module/${MODULE}/public)
  endforeach()

  doxygen_add_docs(
    documentation
    ${DOCUMENTATION_DIRECTORIES}
    ALL
    
  )
endif()

