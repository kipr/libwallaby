[![Bob the Builder](https://github.com/kipr/libwallaby/actions/workflows/bob.yml/badge.svg)](https://github.com/justinmerrell/libwallaby/actions/workflows/bob.yml)

# libwallaby
Library for interfacing with the "wallaby" firmware on the 2016 controller board

It was designed based on (and to be mostly compatible with) https://github.com/kipr/libkovan 


Ubuntu Build Instructions
```` bash
#Installing dependencies: 
sudo apt-get install libzbar-dev libopencv-dev libjpeg-dev python-dev doxygen swig

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

Want to Contribute? Start Here!:
https://github.com/kipr/KIPR-Development-Toolkit
