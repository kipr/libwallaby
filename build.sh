#!/bin/bash
set -e

# Create a build directory
mkdir -p build
cd build

# Run CMake to configure the project
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../toolchain/arm-linux-gnueabihf.cmake \
    -Dpackage_debian=ON \
    -DCPACK_DEBIAN_PACKAGE_ARCHITECTURE=armhf

# Build the project and create the .deb package
make -j$(nproc --ignore=1) package