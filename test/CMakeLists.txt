find_package(Catch2 CONFIG REQUIRED)

add_executable(
    spymarine_tests
    tst_device.cpp
    tst_parse_message.cpp
    tst_read_devices.cpp
)

target_link_libraries(spymarine_tests PRIVATE Catch2::Catch2WithMain spymarine)
