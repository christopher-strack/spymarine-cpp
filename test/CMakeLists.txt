enable_testing()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(Catch2 nlohmann_json)

add_executable(spymarine_tests)

target_sources(
    spymarine_tests
    PRIVATE
        tst_parse_message.cpp
        tst_read_devices.cpp
        tst_sensor_reader.cpp
        tst_home_assistant.cpp
)

target_link_libraries(spymarine_tests PRIVATE Catch2::Catch2WithMain nlohmann_json spymarine)

target_compile_definitions(spymarine_tests PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")

target_compile_options(spymarine_tests PRIVATE -Wall -Werror)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)
catch_discover_tests(spymarine_tests)
