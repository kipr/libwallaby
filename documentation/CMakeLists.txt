option(with_documentation "Enable documentation" ON)

if (with_documentation)
  find_package(Doxygen REQUIRED)

  # aliases
  set(DOXYGEN_ALIASES description="@details" detailed="@details" [[blocks="@par @b This function is blocking."]] [[blocksuntil="@par @b This function blocks until "]])
  set(DOXYGEN_VERBATIM_VARS DOXYGEN_ALIASES)

  set(DOXYGEN_WARN_LOGFILE warnings.txt)
  
  set(DOXYGEN_IMAGE_PATH ${CMAKE_SOURCE_DIR}/documentation/pages)
  set(DOXYGEN_PROJECT_LOGO ${CMAKE_SOURCE_DIR}/documentation/pages/logo.png)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_GENERATE_MAN YES)
  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_PROJECT_NUMBER ${KIPR_VERSION_MAJOR}.${KIPR_VERSION_MINOR}.${KIPR_VERSION_PATCH})

  set(DOCUMENTATION_DIRECTORIES)
  get_property(KIPR_MODULES GLOBAL PROPERTY kipr_modules)

  foreach(MODULE ${KIPR_MODULES})
      list(APPEND DOCUMENTATION_DIRECTORIES ${CMAKE_SOURCE_DIR}/module/${MODULE}/public)
  endforeach()
  list(APPEND DOCUMENTATION_DIRECTORIES ${CMAKE_SOURCE_DIR}/documentation/pages)

 #Project Settings
  set(DOXYGEN_PROJECT_LOGO ${CMAKE_SOURCE_DIR}/documentation/kipr.png)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/documentation)
  #Build Settings
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_GENERATE_TODOLIST NO)
  #Input Settings
  set(DOXYGEN_RECURSIVE YES)
  #Source Browser Settings
  set(DOXYGEN_REFERENCED_BY_RELATION YES)
  set(DOXYGEN_REFERENCES_RELATION YES)
  #HTML
    #Note: Finding a red color that wasn't ugly was a challenge, so it's purple for now
    #(because purple is an awesome color)
  set(DOXYGEN_HTML_COLORSTYLE_HUE 243)
  set(DOXYGEN_HTML_COLORSTYLE_SAT 96)
  set(DOXYGEN_HTML_COLORSTYLE_GAMMA 95)
  set(DOXYGEN_DISABLE_INDEX YES)
  set(DOXYGEN_GENERATE_TREEVIEW YES)

  doxygen_add_docs(
    documentation
    ${DOCUMENTATION_DIRECTORIES}
    ALL
    
  )
endif()

