cmake_minimum_required(VERSION 3.28)

project(
    spymarine_cpp
    VERSION 0.1
    DESCRIPTION
        "A library for spying on Simarine devices and their sensor values"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

add_subdirectory(src)

if(BUILD_TESTS)
    find_package(Catch2 CONFIG REQUIRED)

    add_executable(
        spymarine_tests
        test/tst_device.cpp
        test/tst_parse_message.cpp
        test/tst_read_devices.cpp
    )

    target_link_libraries(
        spymarine_tests
        PRIVATE Catch2::Catch2WithMain spymarine
    )
endif()

if(BUILD_EXAMPLES)
    add_executable(print_devices_example examples/print_devices.cpp)

    target_link_libraries(print_devices_example PRIVATE spymarine)
endif()
