include(ExternalProject)

set(LIBAV_DIR ${CMAKE_CURRENT_BINARY_DIR}/dep_libav)
set(LIBAV_LIB_DIR ${LIBAV_DIR}/lib)
set(LIBAV_INCLUDE_DIR ${LIBAV_DIR}/include)

set(LIBAV_COMMON_OPTIONS
  --enable-pic
  --prefix=${LIBAV_DIR}
  --extra-cflags=-w
  --disable-encoders
  --disable-decoders
  --disable-muxers
  --disable-demuxers
  --disable-parsers
  --disable-protocols
  --disable-indevs
  --disable-outdevs
  --disable-filters
)

if (CMAKE_CROSSCOMPILING)
  # Make CMAKE_SYSTEM_NAME lowercase
  get_property(ARCH GLOBAL PROPERTY arch)
  get_property(TARGET_OS GLOBAL PROPERTY target_os)
  get_property(CROSS_PREFIX GLOBAL PROPERTY cross_prefix)
  ExternalProject_Add(dep_libav
    URL https://github.com/libav/libav/archive/refs/tags/v12.3.tar.gz
    CONFIGURE_COMMAND ${LIBAV_DIR}-prefix/src/dep_libav/configure
      ${LIBAV_COMMON_OPTIONS}
      --enable-cross-compile
      --cross-prefix=${CROSS_PREFIX}
      --target-os=${TARGET_OS}
      --arch=${ARCH}
      --sysroot=${CMAKE_SYSROOT}
    INSTALL_COMMAND make install
  )
else()
  ExternalProject_Add(dep_libav
    URL https://github.com/libav/libav/archive/refs/tags/v12.3.tar.gz
    CONFIGURE_COMMAND ${LIBAV_DIR}-prefix/src/dep_libav/configure
      ${LIBAV_COMMON_OPTIONS}
    INSTALL_COMMAND make install
  )
endif()

# libav

# opencv
add_library(av_wrapper INTERFACE)
add_dependencies(av_wrapper dep_libav)
target_link_directories(av_wrapper INTERFACE ${LIBAV_LIB_DIR})
target_include_directories(av_wrapper INTERFACE ${LIBAV_INCLUDE_DIR})

# libavcodec
add_library(avcodec_wrapper INTERFACE)
target_link_libraries(avcodec_wrapper INTERFACE av_wrapper avcodec)

# libavfilter
add_library(avfilter_wrapper INTERFACE)
target_link_libraries(avfilter_wrapper INTERFACE av_wrapper avfilter)

# libavdevice
add_library(avdevice_wrapper INTERFACE)
target_link_libraries(avdevice_wrapper INTERFACE av_wrapper avdevice)

# libavformat
add_library(avformat_wrapper INTERFACE)
target_link_libraries(avformat_wrapper INTERFACE av_wrapper avformat)

# libavresample
add_library(avresample_wrapper INTERFACE)
target_link_libraries(avresample_wrapper INTERFACE av_wrapper avresample)

# libavutil
add_library(avutil_wrapper INTERFACE)
target_link_libraries(avutil_wrapper INTERFACE av_wrapper avutil)

# libavutil
add_library(swscale_wrapper INTERFACE)
target_link_libraries(swscale_wrapper INTERFACE av_wrapper swscale)