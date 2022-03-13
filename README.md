# PBDoom
Doom for PocketBook eInk devices.

## Building
Obtain and SDK (we use https://github.com/c3pio-man/SDK_6.3.0/releases/tag/6.3.2)
and run
```
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/arm_conf.cmake ..
cmake --build
```
