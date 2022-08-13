# libkipr
Library for interfacing with KIPR Robot Controllers.

Documentation can be viewed at https://www.kipr.org/doc/index.html or by clicking the "Help" button in the KIPR Software Suite IDE.

# CMake Options

Each of the following options may be specified when executing CMake by prefixing the option with `-D` (e.g., `-Dwith_accel=OFF`).

## Modules
  - `with_accel` (default: `ON`) - Build accelerometer support.
  - `with_analog` (default: `ON`) - Build analog sensor support.
  - `with_audio` (default: `ON`) - Build audio support.
  - `with_battery` (default: `ON`) - Build battery support.
  - `with_botball` (default: `ON`) - Build botball support.
  - `with_camera` (default: `ON`) - Build camera support.
  - `with_compass` (default: `ON`) - Build compass support.
  - `with_console` (default: `ON`) - Build console support.
  - `with_create` (default: `ON`) - Build iRobot Create 2 support.
  - `with_digital` (default: `ON`) - Build digital sensor support.
  - `with_graphics` (default: `ON`) - Build graphics support (requires X11 development files, such as `x11proto-dev` on Debian/Ubuntu).
  - `with_gyro` (default: `ON`) - Build gyroscope support.
  - `with_magneto` (default: `ON`) - Build magnetometer support.
  - `with_motor` (default: `ON`) - Build motor support.
  - `with_network` (default: `ON`) - Build network support.
  - `with_servo` (default: `ON`) - Build servo support.
  - `with_tello` (default: `ON`) - Build Tello support.
  - `with_thread` (default: `ON`) - Build thread support.
  - `with_time` (default: `ON`) - Build time support.
  - `with_wait_for` (default: `ON`) - Build wait_for support.

## Bindings
  - `with_python_binding` (default: `ON`) - Build Python binding (requires Python 3+ development files, such as `libpython3.10-dev` on Debian/Ubuntu).

## Miscellaneous
  - `with_documentation` (default: `ON`) - Build documentation support (requires `doxygen` installed on system).
  - `with_tests` (default: `ON`) - Build tests.

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
