[![Bob the Builder](https://github.com/kipr/libwallaby/actions/workflows/bob.yml/badge.svg)](https://github.com/kipr/libwallaby/actions/workflows/bob.yml)

# libwallaby
Library for interfacing with hardware on the 2016-2020 KIPR Wallaby and 2020-PRESENT KIPR Wombat controllers.

Documentation can be viewed at https://www.kipr.org/doc/index.html or by clicking the "Help" button in the KIPR Software Suite IDE.

It was designed based on (and to be mostly compatible with) https://github.com/kipr/libkovan 

### Debian Package Install (.deb):
```bash
sudo dpkg -i libwallaby.deb
```

### Build Instructions:
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

### Tarball Build:
```bash
tar -xvf libwallaby.tar.gz
cd libwallaby
sudo chmod 777 configure
sudo ./configure
cd build
sudo make -j4
sudo make install
```

Want to Contribute? Start Here!:
https://github.com/kipr/KIPR-Development-Toolkit
