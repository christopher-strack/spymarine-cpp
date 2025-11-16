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

include(cmake/warnings.cmake)

option(SPYMARINE_BUILD_TESTS "Build tests" OFF)
option(SPYMARINE_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(SPYMARINE_BUILD_EXAMPLES "Build examples" OFF)
option(SPYMARINE_ENABLE_ALL_WARNINGS "Enable almost all warnings" OFF)
option(SPYMARINE_ENABLE_HOME_ASSISTANT_SUPPORT "Enable Home Assistant support" ON)
option(SPYMARINE_ENABLE_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)

add_subdirectory(src)

if(SPYMARINE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (SPYMARINE_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if(SPYMARINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
