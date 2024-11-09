cmake_minimum_required(VERSION 3.28)

project(spymarine_cpp VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(spymarine src/device.cpp src/device_ostream.cpp src/parsing.cpp)
target_include_directories(spymarine PUBLIC include)

find_package(Catch2 CONFIG REQUIRED)

add_executable(spymarine_tests test/tst_parsing.cpp)

target_link_libraries(spymarine_tests PRIVATE Catch2::Catch2WithMain spymarine)

include(CTest)
include(Catch)
catch_discover_tests(spymarine_tests)
