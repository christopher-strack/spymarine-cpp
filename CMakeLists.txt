cmake_minimum_required(VERSION 3.28)

project(spymarine_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(spymarine
    src/client.cpp
    src/device_ostream.cpp
    src/device.cpp
    src/file_descriptor.cpp
    src/message.cpp
    src/message_values_iterator.cpp
    src/message_values_view.cpp
    src/message_value.cpp
    src/parse_device.cpp
    src/parse_message.cpp
    src/tcp_socket.cpp
    src/udp_socket.cpp
)

target_include_directories(spymarine PUBLIC include)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_TESTS)
    find_package(Catch2 CONFIG REQUIRED)

    add_executable(spymarine_tests test/tst_client.cpp test/tst_device.cpp test/tst_parse_message.cpp)

    target_link_libraries(spymarine_tests PRIVATE Catch2::Catch2WithMain spymarine)
endif()


if(BUILD_EXAMPLES)
    add_executable(read_devices_example examples/read_devices.cpp)

    target_link_libraries(read_devices_example PRIVATE spymarine)
endif()
