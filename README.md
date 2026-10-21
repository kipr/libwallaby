# libwallaby

The library for interfacing with KIPR robot controllers.
The name comes from the previous generation controller, the Wallaby, but the current controller is the Wombat.
For that reason, this is sometimes referred to generically as `libkipr`.

The Wombat is built on a Raspberry Pi 3B+ with a specially designed HAT that allows interfacing with the various peripherals that comprise a KIPR robot.
Therefore, the primary `libwallaby` deployment target is 64-bit Arm, also known as `aarch64` or `arm64`.
The secondary target is WASM for use in the [KIPR online simulator](https://github.com/kipr/Simulator).

Documentation can be viewed at https://www.kipr.org/doc/index.html or by clicking the "Help" button in the KIPR Software Suite IDE.

## Building

### Wombat (`aarch64`)

Build a full-featured release for use on a KIPR Wombat with Docker:

```sh
docker build -t libwallaby-builder .
docker run --rm --mount type=bind,source=.,destination=/src/ libwallaby-builder sh -c 'cmake -B/src/build -DCMAKE_TOOLCHAIN_FILE=/src/toolchain/aarch64-linux-gnu.cmake /src && cmake --build /src/build -j "$(nproc)"'
```

Or build directly on Debian 13 (trixie).
Install the cross-compiler and the `arm64` versions of the libraries the default build links against (the `Dockerfile` installs the same packages):

```sh
sudo dpkg --add-architecture arm64
sudo apt update
sudo apt install build-essential cmake git swig doxygen \
  gcc-aarch64-linux-gnu g++-aarch64-linux-gnu \
  libx11-dev:arm64 python3.13-dev:arm64
```

Then configure and build:

```sh
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=toolchain/aarch64-linux-gnu.cmake .
cmake --build build -j "$(nproc)"
```

Either way, the build also produces a Debian package, `build/kipr-<version>-Linux.deb`. See [Installation](#installation) for how to install it on a Wombat.
The camera module downloads and builds OpenCV, libav, libjpeg and zlib, so the build needs internet access.

`toolchain/arm-linux-gnueabihf.cmake` is also included for 32-bit Arm (`armv7`), but there is generally no reason to use it, since the Pi 3B+ supports 64-bit.

### Desktop

To build on your own computer without a Wombat, use the dummy device backend:

```sh
cmake -Bbuild -DDUMMY=ON -Dpackage_debian=OFF -Dwith_camera=OFF .
cmake --build build -j "$(nproc)"
```

On Debian this needs `build-essential cmake swig doxygen libx11-dev python3-dev`.

### Simulator (JavaScript/WASM)

These builds use [Emscripten](https://emscripten.org/). The Simulator's [`dependencies/build.py`](https://github.com/kipr/Simulator/blob/master/dependencies/build.py) shows the full build in context.

`with_botball` must be turned off together with `with_graphics`, because the botball module uses the graphics API.

#### Without Python support

libwallaby can be statically linked into a C program that has been compiled with Emscripten. Use the resulting `libkipr.a`.

```sh
source emsdk/emsdk_env.sh
emcmake cmake -Bbuild -Dwith_graphics=OFF -Dwith_botball=OFF -Dwith_camera=OFF -Dwith_python_binding=OFF -Dwith_tests=OFF .
cmake --build build -j "$(nproc)"
```

#### With Python support

libwallaby can be dynamically linked into a `cpython` that has been compiled with Emscripten. Place the resulting `kipr.wasm` in `cpython`'s virtual filesystem on the `PYTHONPATH` (see the Simulator for details).

```sh
source emsdk/emsdk_env.sh
emcmake cmake -Bbuild -Dwith_graphics=OFF -Dwith_botball=OFF -Dwith_camera=OFF -Dwith_tests=OFF -Dwasm=ON -DPYTHON_LIBRARY=$PYTHON_LIBRARY -DPYTHON_INCLUDE_DIR=$PYTHON_INCLUDE_DIR .
cmake --build build -j "$(nproc)"
```

where:
  - `$PYTHON_LIBRARY` is the `libpythonVERSION.a` file that has been compiled with Emscripten.
  - `$PYTHON_INCLUDE_DIR` is that Python's include directory.

### CMake options

Pass any of these to CMake with `-D`, for example `-Dwith_camera=OFF`.
**Deps** lists the Debian 13 packages each option needs. When cross-compiling, install the `:arm64` versions of libraries (for example `python3.13-dev:arm64`).

| Option | Description | Default | Deps |
|--------|-------------|---------|------|
| `with_python_binding` | Build Python bindings | `ON` | `swig python3-dev` |
| `with_xml_binding` | Build XML bindings | `ON` | `swig` |
| `with_documentation` | Build Doxygen documentation | `ON` | `doxygen` |
| `with_tests` | Build tests (ignored for Emscripten builds) | `ON` | |
| `with_accel` | Accelerometer API | `ON` | |
| `with_analog` | Analog sensor API | `ON` | |
| `with_battery` | Battery API | `ON` | |
| `with_botball` | Botball API (requires `with_graphics`) | `ON` | |
| `with_button` | Button API | `ON` | |
| `with_camera` | Camera API | `ON` | Internet access at build time |
| `with_compass` | Compass API | `ON` | |
| `with_console` | Console API | `ON` | |
| `with_digital` | Digital sensor API | `ON` | |
| `with_graphics` | Graphics API | `ON` | `libx11-dev` |
| `with_gyro` | Gyroscope API | `ON` | |
| `with_magneto` | Magnetometer API | `ON` | |
| `with_motor` | Motor API | `ON` | |
| `with_network` | Network API | `ON` | |
| `with_servo` | Servo API | `ON` | |
| `with_thread` | Thread/mutex API | `ON` | |
| `with_time` | Time API | `ON` | |
| `with_wait_for` | `wait_for` API | `ON` | |
| `package_debian` | Build a Debian package after each build (ignored for Emscripten builds) | `ON` | |
| `DUMMY` | Use the dummy device backend, for building on a computer | `OFF` | |
| `wasm` | Build `kipr.wasm` for dynamic linking (Emscripten builds only) | `OFF` | |

## AI Camera

libwallaby supports the KIPR AI camera, a Pi Zero that connects to the Wombat over USB networking.
When a camera is opened, libwallaby checks for the AI camera at `192.168.1.2` and uses it if it responds; otherwise it uses a regular USB camera.

Wombat images and `.deb` packages with the latest AI camera changes are published on the [Releases page](https://github.com/kipr/libwallaby/releases) (currently the `v1.2.5` pre-release).
To test newer changes, build a package as described in [Building](#wombat-aarch64).

### Installation

First, plug the Pi Zero into the Wombat, then SSH into the Wombat and create the necessary connection.

```sh
sudo nmcli connection add type ethernet con-name ai-camera-link ifname usb0 ipv4.addresses 192.168.1.1/24 ipv4.method manual ipv6.method auto ipv6.addresses "fe80::c3ed:d6c2:1e76:cebe/64" ipv6.routes "fe80::/64 :: 101"
```

The connection should come up pretty quickly (<30 sec).

You can verify the connection by `ssh`ing from the Wombat to the Pi Zero:

```sh
ssh kipr@192.168.1.2
```

We will be rebooting the Wombat later, so you may wish to power off the Pi Zero now to avoid potential disk corruption.

```sh
sudo poweroff
```

Now, install the new version of libwallaby.

1. Copy the libwallaby `.deb` file to the Wombat:

   ```sh
   scp kipr-<ver>-Linux.deb kipr@<wombat_ip>:~
   ```

2. `ssh` into the Wombat and install it:

   ```sh
   sudo apt install ./kipr-<ver>-Linux.deb
   ```

3. Reboot:

   ```sh
   sudo reboot
   ```

### Using the AI camera

Objects found by the AI camera are reported through a camera channel, which you configure in BotUI under Settings → Channels.
Create the channel with the "QR Code Scanner" type. A future release will use a more appropriate name.

Read the results with the usual camera object functions:

- `get_object_centroid()` - centroid
- `get_object_bbox()` - bounding box
- `get_object_confidence()` - confidence
- `get_object_data()` - string from the model that names the object type
- `get_object_data_length()` - length of that string

### Debugging with systemd

`botui` on the Wombat runs as a systemd unit.
You can view a unit's logs by running:

```sh
sudo journalctl -b -f -u <service-name>.service
```

So, for `botui`:

```sh
sudo journalctl -b -f -u botui.service
```

Flags:

- `-b`: Only show logs from current boot.
- `-f`: Emulates `tail -f` with a continuous stream.
- `-u UNIT`: Only view logs for `UNIT`, rather than viewing logs for everything systemd is tracking.

See `man journalctl` for more info.

You can also list everything systemd is currently tracking by running:

```sh
systemctl
```

Generally the entries suffixed with `.service` are the most pertinent.

## License

libwallaby is licensed under the terms of the [GPLv3 License](LICENSE).

## Contributing

Want to contribute? Start here: https://github.com/kipr/KIPR-Development-Toolkit
