find_package(Catch2 CONFIG REQUIRED)

add_executable(spymarine_tests)

target_sources(
    spymarine_tests
    PRIVATE tst_parse_message.cpp tst_read_devices.cpp tst_sensor_reader.cpp
)

target_link_libraries(spymarine_tests PRIVATE Catch2::Catch2WithMain spymarine)
