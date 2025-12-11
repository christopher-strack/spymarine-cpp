# spymarine-cpp

A C++ 23 library for spying on Simarine devices and their sensor values

Based on the fantastic reverse engineering work of https://github.com/htool/pico2signalk

Tested with Simarine Pico rev2 and firmware 1.17

Used by https://github.com/christopher-strack/esp-simarine-home-assistant

## Requirements

Requires C++ 23

The library itself does not depend on any third-party library.
Catch2 and nlohman JSON is used for unit testing.

## Build

Use CMake to build the project

```
cmake -B build --preset default
cmake --build build --preset default
```

## Usage

See the examples directory on how to use the library:
- `print_devices_and_sensors.cpp`: most basic example that prints all devices and sensors
- `moving_average.cpp`: print the moving average of current sensors
- `print_home_assistant_discovery.cpp`: prints JSON for discovering devices and updating sensor values

## Known Issues

- Only tested by my own personal Simarine setup
- Non-ASCII characters in device names will not be represented correctly and replaced with a
  placeholder. A non-standard encoding seems to be used.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request on GitHub.

## References

See https://github.com/christopher-strack/esp-simarine-home-assistant for an
example usage

See https://github.com/christopher-strack/spymarine for a Python
version of the library.

## Author

Christopher Strack
