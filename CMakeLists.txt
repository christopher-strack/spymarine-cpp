cmake_minimum_required(VERSION 3.28)

project(spymarine_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(spymarine
    src/client.cpp
    src/device_ostream.cpp
    src/device.cpp
    src/parsing.cpp
)

target_include_directories(spymarine PUBLIC include)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    find_package(Catch2 CONFIG REQUIRED)

    add_executable(spymarine_tests test/tst_client.cpp test/tst_parsing.cpp)

    target_link_libraries(spymarine_tests PRIVATE Catch2::Catch2WithMain spymarine)
endif()
