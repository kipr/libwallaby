include(ExternalProject)

set(ZLIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/dep_zlib)
set(ZLIB_LIB_DIR ${ZLIB_DIR}/lib)
set(ZLIB_INCLUDE_DIR ${ZLIB_DIR}/include)

set(ZLIB_COMMON_OPTIONS
  --prefix=${ZLIB_DIR}
)

if (CMAKE_CROSSCOMPILING)
  get_property(ARCH GLOBAL PROPERTY arch)
  ExternalProject_Add(dep_zlib
    URL https://www.zlib.net/zlib-1.2.12.tar.gz
    CMAKE_ARGS
      -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    INSTALL_COMMAND make install
  )
else()
  ExternalProject_Add(dep_zlib
    URL https://www.zlib.net/zlib-1.2.12.tar.gz
    CONFIGURE_COMMAND ${ZLIB_DIR}-prefix/src/dep_zlib/configure
      ${ZLIB_COMMON_OPTIONS}
    INSTALL_COMMAND make install
  )
endif()

# zlib

add_library(zlib_wrapper INTERFACE)
add_dependencies(zlib_wrapper dep_zlib)
target_link_directories(zlib_wrapper INTERFACE ${ZLIB_LIB_DIR})
target_include_directories(zlib_wrapper INTERFACE ${ZLIB_INCLUDE_DIR})

# z
add_library(z_wrapper INTERFACE)
add_dependencies(z_wrapper zlib_wrapper)
target_link_libraries(z_wrapper INTERFACE zlib_wrapper z)