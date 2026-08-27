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

## Dummy Build
  - `DUMMY` (default: `OFF`) - Build a dummy build for use on computer

# Cross-compiling to aarch64-linux-gnu (e.g., Wombat)

```bash
apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
cd libkipr
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=$(pwd)/toolchain/arm-linux-gnueabihf.cmake .
```

# Cross-compiling to JavaScript/WASM (e.g., Simulator)

## Without Python Support
libkipr can be compiled to statically link to a C program that has been compiled to emscripten. The resulting `libkipr.a` should be used.
```
source emsdk/emsdk_env.sh
cd libkipr
emcmake cmake -Bbuild -Dwith_graphics=OFF -Dwith_camera=OFF -Dwith_tello=OFF -Dwith_python_binding=OFF .
```

## With Python Support
libkipr can be compiled to dynamically link to a `cpython` that has been compiled to emscripten. The resulting `kipr.wasm` should be placed in `cpython`'s virtual filesystem on the `PYTHONPATH` (see simulator for details).
```
source emsdk/emsdk_env.sh
cd libkipr
emcmake cmake -Bbuild -Dwith_graphics=OFF -Dwith_camera=OFF -Dwith_tello=OFF -DPYTHON_LIBRARY=$PYTHON_LIBRARY -DPYTHON_INCLUDE_DIR=$PYTHON_INCLUDE_DIR -Dwasm=ON .
```
where:
  - `$PYTHON_LIBRARY` is the `libpythonVERSION.a` file that has been compiled to emscripten-browser.
  - `$PYTHON_INCLUDE_DIR` is that python's include directory.

# AI Camera Testing

If you just want to test the libwallaby changes without worrying about installation, you can flash your Wombat with a [testing image](https://files.kipr.org/wombat/Wombat_v32.0.0-aicam_beta_20251119.img.gz).
The latest was built on 2025-11-19.

You can install the `.deb` file hosted [here](https://files.kipr.org/devel/kipr-1.2.2-Linux.deb), which should include all changes up to 2025-11-19.
If you download the prebuild `.deb` file, proceed to **installation** below.
If you want to test newer changes, you can compile using the instructions above or using the new Docker build.

Get the sysroot for cross-compilation with git lfs. This may take a while depending on your connection speed.

```bash
git lfs pull
```

Make sure docker is set up properly for your user and run the build script:

```bash
./build.sh
```

This will produce a `.deb` file in the `build` directory.

## Installation

First, plug the Pi Zero into the Wombat, then SSH into the Wombat and create the the necessary connection.

```
sudo nmcli connection add type ethernet con-name ai-camera-link ifname usb0 ipv4.addresses 192.168.1.1/24 ipv4.method manual ipv6.method auto ipv6.addresses "fe80::c3ed:d6c2:1e76:cebe/64" ipv6.routes "fe80::/64 :: 101"
```

The connection should come up pretty quickly (<30 sec).

You can verify the connection by `ssh`ing from the Wombat to the Pi Zero:

```
ssh kipr@192.168.1.2
```

We will be rebooting the Wombat later, so you may wish to power off the Pi Zero now to avoid potential disk corruption.

```
sudo poweroff
```

Now, install the testing version of libwallaby.

1. Copy the updated libwallaby `.deb` file to the wombat.

```
scp kipr-<ver>-Linux.deb kipr@<wombat_ip>:~
```

2. `ssh` into the wombat and update:

```
sudo apt install ./kipr-<ver>.deb
```

3. Reboot

```
sudo reboot
```

### Debugging With systemd

`botui` on the Wombat runs as a systemd unit.
You can view a unit's logs by running:

```
sudo journalctl -b -f -u <service-name>.service
```

So, for `botui`:

```
sudo journalctl -b -f -u botui.service
```

Flags:

- `-b`: Only show logs from current boot.
- `-f`: Emulates `tail -f` with a continuous stream.
- `-u UNIT`: Only view logs for `UNIT`, rather than viewing logs for everything systemd is tracking.

See `man journalctl` for more info.

You can also list everything systemd is currently tracking by running:

```
systemctl
```

Generally the entries suffixed with `.service` are the most pertinent.


## Notes on AI Camera beta

This patch should only be used for testing as it will cause only the AI camera to be supported.
This restriction will be lifted in an upcoming release.
Important parts not included in this release:

1. AI camera auto-detect. Important because the AI camera uses IP over USB as its connection. Auto-detect will look for the camera over the USB IP network.
2. Model download. Download AI models from the Wombat.

The object data structures found by the AI camera are transferred into a configured blob tracking channel, which can be done through BOTUI->settings->channels.
The added channel is currently configured using "QR code scanner."
A future release will use a more appropriate name.

The data fields of the channel object structure are used as follows:

- `m_centroid` - centroid
- `m_bounding_box` - bounding box
- `m_confidence` - confidence
- `m_data` - string from model that indicates object type
- `m_length` - length of string

In short, the same as the blob tracking object and with the added bonus of being accessed by existing functions.

# License

libkipr is licensed under the terms of the [GPLv3 License](LICENSE).

# Contributing

Want to Contribute? Start Here!:
https://github.com/kipr/KIPR-Development-Toolkit
