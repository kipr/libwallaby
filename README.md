# libkipr
Library for interfacing with KIPR Robot Controllers.

Documentation can be viewed at https://www.kipr.org/doc/index.html or by clicking the "Help" button in the KIPR Software Suite IDE.

# Cross-compiling to aarch64-linux-gnu (e.g., Wombat)

```bash
apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
cd libkipr
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=$(pwd)/toolchain/aarch64-linux-gnu.cmake .
```

# Cross-compiling to JavaScript/WASM (e.g., Simulator)

```
source emsdk/emsdk_env.sh
cd libkipr
emcmake cmake -Bbuild -Dwith_graphics=OFF -Dwith_camera=OFF -Dwith_python_binding=OFF -Dwith_tello=OFF .
```

# Contributing

Want to Contribute? Start Here!:
https://github.com/kipr/KIPR-Development-Toolkit
