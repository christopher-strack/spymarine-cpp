# spymarine

A C++ 23 library for spying on Simarine devices and their sensor values

Based on the fantastic reverse engineering work of https://github.com/htool/pico2signalk

Tested with Simarine Pico rev2 and firmware 1.17

## Build

```
cmake -B build --preset default
cmake --build build
```

## Known Issues

- Non-ASCII characters in device names will not be represented correctly and replaced with a
  placeholder. A non-standard encoding seems to be used.

## Author

Christopher Strack
