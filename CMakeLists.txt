cmake_minimum_required(VERSION 3.1)

project(libwallaby)

#set(ENV{OPENCV_INCLUDE_DIR} /home/root/Documents/KISS/bin/wallaby7/libwallaby/opencv/include)
#set(OpenCV_DIR /home/root/Documents/KISS/bin/wallaby7/libwallaby/opencv/cmake)


# Use these options based on platform:
# platform										no_wallaby	no_zoobee_wallaby
# desktop (Ubuntu, etc.)					ON					ON or OFF (don't care)
# wallaby-wallaby controller			OFF					ON
# zoobee-wallaby controller			OFF					OFF	

option(no_wallaby "Not a wallaby controller" OFF)
option(emscripten "Build for use with emscripten (for simulator)" OFF)
option(with_vision_support "Controls camera support" ON)

option(no_zoobee_wallaby "Not a zoobee wallaby controller" OFF)

if(no_wallaby)
  add_definitions(-DNOT_A_WALLABY)
endif()

if(emscripten)
  add_definitions(-DTARGET_EMSCRIPTEN)
endif()

if(with_vision_support)
  add_definitions(-DWITH_VISION_SUPPORT)
endif()

if(USE_ARUCO)
	add_definitions(-DARUCO)
endif()


if(no_zoobee_wallaby)
  add_definitions(-DNOT_A_ZOOBEE_WALLABY)
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


set(INCLUDE ${libwallaby_SOURCE_DIR}/include)
set(SRC ${libwallaby_SOURCE_DIR}/src)
set(PIXELTOASTER ${SRC}/pixeltoaster)
set(BINDINGS ${CMAKE_SOURCE_DIR}/bindings)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

include_directories(${INCLUDE})


file(GLOB SOURCES ${SRC}/*.cpp)
file(GLOB C_TESTS tests/*.c)
file(GLOB PIXELTOASTER_SOURCES ${PIXELTOASTER}/*.cpp)

# Cross-compile
# set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
# set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)


# C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# REMOVE ME
include_directories(${SRC}/pixeltoaster)

#############################
#   PixelToaster settings   #
#############################

if(APPLE)
	file(GLOB OBJC_SOURCES ${PIXELTOASTER}/*.mm)
elseif(WIN32)
	add_definitions(-fno-check-new)
endif()



if(DEFINED ENV{OPENCV_INCLUDE_DIR})
  set(OPENCV_INCLUDE $ENV{OPENCV_INCLUDE_DIR})
elseif(WIN32)
  set(OPENCV_INCLUDE ${CMAKE_SOURCE_DIR}/../opencv/build/install/include)
endif()

if(DEFINED ENV{OPENCV_LIB_DIR})
  set(OPENCV_LIB $ENV{OPENCV_LIB_DIR})
elseif(WIN32)
  set(OPENCV_LIB ${CMAKE_SOURCE_DIR}/../opencv/build/install/x86/vc11/lib)
endif()

if(DEFINED ENV{ZBAR_DIR})
  set(ZBAR_DIR $ENV{ZBAR_DIR})
elseif(WIN32)
  set(ZBAR_DIR "C:/Program Files (x86)/ZBar")
endif()

if(with_vision_support)
  find_package(OpenCV REQUIRED )
  find_package(JPEG REQUIRED)
endif()
find_package (Threads)

if(build_python)
  find_package( PythonInterp 2.7 REQUIRED )
  find_package( PythonLibs 2.7 REQUIRED )
endif()

include_directories(${CMAKE_INSTALL_PREFIX}/include)
if(with_vision_support)
  include_directories(${OPENCV_INCLUDE})
  include_directories(${JPEG_INCLUDE_DIR})
  include_directories(${ZBAR_DIR}/include)

  link_directories(${OPENCV_LIB})
  link_directories(${ZBAR_DIR}/lib)
endif()

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

option(build_python "Build python bindings" ON)
option(USE_ARUCO "Use Aruco Markers" OFF)

file(GLOB SWIG_SOURCES ${BINDINGS}/*.i)

#if(build_python)
#  find_package(SWIG REQUIRED)
#  find_package(PythonLibs 2.7 REQUIRED)
 # message("${PYTHON_LIBRARY}")
 # set(PYTHON_BINDING_SOURCES ${CMAKE_BINARY_DIR}/wallaby_wrap_py.cxx)
 # set(PYTHON_BINDING_LIBRARIES ${PYTHON_LIBRARY})
 # include_directories(${PYTHON_INCLUDE_DIR})
 # add_custom_command(COMMAND ${SWIG_EXECUTABLE} -Wall -python -c++ -I${INCLUDE}/kipr -outdir ${LIBRARY_OUTPUT_PATH} -o ${CMAKE_BINARY_DIR}/wallaby_wrap_py.cxx ${BINDINGS}/wallaby.i
  #  DEPENDS ${SWIG_SOURCES}
  #  OUTPUT ${CMAKE_BINARY_DIR}/wallaby_wrap_py.cxx ${LIBRARY_OUTPUT_PATH}/wallaby.py)
   # install(FILES ${LIBRARY_OUTPUT_PATH}/wallaby.py DESTINATION lib)
#endif()

if(build_python)
  find_package(SWIG REQUIRED)
  find_package(PythonLibs 2.7 REQUIRED)
  message("${PYTHON_LIBRARY}")
  set(PYTHON_BINDING_SOURCES ${CMAKE_BINARY_DIR}/kipr_wrap_py.cxx)
  set(PYTHON_BINDING_LIBRARIES ${PYTHON_LIBRARY})
  include_directories(${PYTHON_INCLUDE_DIR})
  add_custom_command(COMMAND ${SWIG_EXECUTABLE} -Wall -python -c++ -I${INCLUDE}/kipr -outdir ${LIBRARY_OUTPUT_PATH} -o ${CMAKE_BINARY_DIR}/kipr_wrap_py.cxx ${BINDINGS}/kipr.i
    DEPENDS ${SWIG_SOURCES}
    OUTPUT ${CMAKE_BINARY_DIR}/kipr_wrap_py.cxx ${LIBRARY_OUTPUT_PATH}/kipr.py)
    install(FILES ${LIBRARY_OUTPUT_PATH}/kipr.py DESTINATION lib)
  
endif()


if(build_python)
  add_library(wallaby_python SHARED ${PYTHON_BINDING_SOURCES})
  set_target_properties(wallaby_python PROPERTIES PREFIX _ OUTPUT_NAME wallaby SUFFIX .so)
  target_link_libraries(wallaby_python wallaby ${PYTHON_BINDING_LIBRARIES} )
  install(TARGETS wallaby_python DESTINATION lib)

  add_library(wombat_python SHARED ${PYTHON_BINDING_SOURCES})
  set_target_properties(wombat_python PROPERTIES PREFIX _ OUTPUT_NAME kipr SUFFIX .so)
  target_link_libraries(wombat_python kipr ${PYTHON_BINDING_LIBRARIES} )
  install(TARGETS wombat_python DESTINATION lib)
endif()



##Changed wallaby to KIPR under three instances below: 148, 151,165
# Library: libwallaby
add_library(wallaby SHARED ${SOURCES} ${PIXELTOASTER_SOURCES} ${OBJC_SOURCES})
add_library(kipr SHARED ${SOURCES} ${PIXELTOASTER_SOURCES} ${OBJC_SOURCES})

if(with_vision_support)
  target_link_libraries(wallaby ${OpenCV_LIBS} ${JPEG_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} zbar opencv_core opencv_highgui opencv_imgproc)
  target_link_libraries(kipr ${OpenCV_LIBS} ${JPEG_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} zbar opencv_core opencv_highgui opencv_imgproc)
endif()


if(USE_ARUCO)
    target_link_libraries(wallaby opencv_aruco)
    target_link_libraries(kipr opencv_aruco)
endif()

if(APPLE)
  find_library(OPENGL_LIBRARY OpenGL)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(CORESERVICE_LIBRARY CoreServices)
  target_link_libraries(wallaby ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${CORESERVICE_LIBRARY})
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(wallaby rt X11)
endif()


# Executables: test programs
# TODO: use a list and foreach to make this more concise
set(TESTS )

foreach(C_TEST ${C_TESTS})
  get_filename_component(C_TEST_NAME ${C_TEST} NAME_WE)
  set(TEST_BIN_NAME ${C_TEST_NAME}_c)
  add_executable(${TEST_BIN_NAME} ${C_TEST})
  target_link_libraries(${TEST_BIN_NAME} kipr)
  list(APPEND TESTS ${TEST_BIN_NAME})
endforeach()


########################################
#   Doxygen documentation (optional)   #
########################################

option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" ON)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  file(MAKE_DIRECTORY doc)
  set(DOXYGEN_DIR ${PROJECT_SOURCE_DIR}/doxygen)
  add_custom_target (Docs++ ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_DIR}/Doxyfile++ SOURCES ${DOXYGEN_DIR}/Doxyfile++
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  add_custom_target (Docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_DIR}/Doxyfile SOURCES ${DOXYGEN_DIR}/Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  add_custom_target(DocMetadata ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${DOXYGEN_DIR}/metadata.ini ${PROJECT_SOURCE_DIR}/doc)
endif()





install(TARGETS ${TESTS} DESTINATION bin/wallaby)
install(TARGETS ${TESTS} DESTINATION bin/kipr)
install(TARGETS kipr DESTINATION lib)
install(TARGETS wallaby DESTINATION lib)
install(DIRECTORY ${INCLUDE}/wallaby DESTINATION include)
install(DIRECTORY ${INCLUDE}/kipr DESTINATION include)

set(CMAKE_INSTALL_PREFIX /usr)

# Install the manpage (appropriate for .deb package)
# manpage should have -r-wr--r-- or 644 permission (owner r-w, group r--, others r--)
install(
	FILES libwallaby.1
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ 
	DESTINATION ${DESTDIR}${CMAKE_INSTALL_PREFIX}/share/man/man1 
)