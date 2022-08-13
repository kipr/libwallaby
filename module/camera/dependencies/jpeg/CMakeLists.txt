include(ExternalProject)

set(JPEG_DIR ${CMAKE_CURRENT_BINARY_DIR}/dep_jpeg)
set(JPEG_LIB_DIR ${JPEG_DIR}/lib)
set(JPEG_INCLUDE_DIR ${JPEG_DIR}/include)

set(JPEG_COMMON_OPTIONS
  CFLAGS=-fPIC  
  --prefix=${JPEG_DIR}
  --enable-shared=no
)

if(CMAKE_CROSSCOMPILING)
  get_property(HOST GLOBAL PROPERTY host)
  get_property(CROSS_PREFIX GLOBAL PROPERTY cross_prefix)
  ExternalProject_Add(dep_jpeg
    URL https://www.ijg.org/files/jpegsrc.v9e.tar.gz
    CONFIGURE_COMMAND ${JPEG_DIR}-prefix/src/dep_jpeg/configure
      ${JPEG_COMMON_OPTIONS}
      --host=${HOST}
      CC=${CROSS_PREFIX}gcc
      LD=${CROSS_PREFIX}ld
    INSTALL_COMMAND make install
  )

else()
  ExternalProject_Add(dep_jpeg
  URL https://www.ijg.org/files/jpegsrc.v9e.tar.gz
  CONFIGURE_COMMAND ${JPEG_DIR}-prefix/src/dep_jpeg/configure
    ${JPEG_COMMON_OPTIONS}
  INSTALL_COMMAND make install
  )
endif()

# jpeg
add_library(jpeg_wrapper INTERFACE)
add_dependencies(jpeg_wrapper dep_jpeg)
target_link_directories(jpeg_wrapper INTERFACE ${JPEG_LIB_DIR})
target_include_directories(jpeg_wrapper INTERFACE ${JPEG_INCLUDE_DIR})
target_link_libraries(jpeg_wrapper INTERFACE jpeg)
