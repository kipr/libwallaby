# libwallaby
Library for interfacing with the "wallaby" firmware on the 2016 controller board

It was designed based on (and to be mostly compatible with) https://github.com/kipr/libkovan 


Ubuntu Build Instructions
```` bash
#Installing dependencies: 
sudo apt-get install libzbar-dev libopencv-dev libjpeg-dev python-dev doxygen swig cmake

# Create a build directory (inside libwallaby/) and change directory to it
mkdir build
cd build

# Initialize CMake
cmake ..

# Build
make

# Install
sudo make install
````

Author: Joshua Southerland
