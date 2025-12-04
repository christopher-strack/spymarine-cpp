enable_testing()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

if (SPYMARINE_ENABLE_HOME_ASSISTANT_SUPPORT)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()


add_executable(spymarine_tests)

target_sources(
    spymarine_tests
    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/test
        FILES
            ${PROJECT_SOURCE_DIR}/test/data.hpp
            ${PROJECT_SOURCE_DIR}/test/raw_data.hpp
            ${PROJECT_SOURCE_DIR}/test/test_sockets.hpp
    PRIVATE
        tst_message_values_view.cpp
        tst_message_value.cpp
        tst_parse_device.cpp
        tst_parse_message.cpp
        tst_read_devices.cpp
        tst_sensor_reader.cpp
        $<$<BOOL:${SPYMARINE_ENABLE_HOME_ASSISTANT_SUPPORT}>:tst_home_assistant.cpp>
)

target_link_libraries(
    spymarine_tests
    PRIVATE
        Catch2::Catch2WithMain
        spymarine
        $<$<BOOL:${SPYMARINE_ENABLE_HOME_ASSISTANT_SUPPORT}>:nlohmann_json>
)

target_compile_definitions(
    spymarine_tests
    PRIVATE
        PROJECT_VERSION="${PROJECT_VERSION}"
        CATCH_CONFIG_ENABLE_TUPLE_STRINGMAKER=1
)

target_enable_warnings(spymarine_tests)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)
catch_discover_tests(spymarine_tests)
