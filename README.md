# spymarine-cpp

A C++ 23 library for spying on Simarine devices and their sensor values

Based on the fantastic reverse engineering work of https://github.com/htool/pico2signalk

Tested with Simarine Pico rev2 and firmware 1.17

## Requirements

Requires C++ 23

The library itself does not depend on any third-party library.
Catch2 and nlohman JSON is used for unit testing.

## Build

Setup [vcpkg](https://vcpkg.io) and use CMake to build the project:

```
cmake -B build --preset default
cmake --build build --preset default
```

## Usage

See the examples directory on how to use the library:
- `print_devices.cpp`: most basic example that prints all devices
- `print_sensor_states.cpp`: print the current sensor states of each device
- `moving_average.cpp`: uses a moving average sensor reader
- `print_home_assistant_discovery.cpp`: prints JSON for discovering devices and updating sensor values

## Known Issues

- Only tested by my own personal Simarine setup
- Non-ASCII characters in device names will not be represented correctly and replaced with a
  placeholder. A non-standard encoding seems to be used.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request on GitHub.

## References

See https://github.com/christopher-strack/spymarine for a Python
version of the library.

## Author

Christopher Strack
