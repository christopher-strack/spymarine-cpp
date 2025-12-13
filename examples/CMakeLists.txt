add_executable(print_devices_and_sensors_example)
target_sources(
    print_devices_and_sensors_example
    PRIVATE print_devices_and_sensors.cpp
)
target_link_libraries(print_devices_and_sensors_example PRIVATE spymarine)
target_enable_warnings(print_devices_and_sensors_example)

add_executable(moving_average)
target_sources(moving_average PRIVATE moving_average.cpp)
target_link_libraries(moving_average PRIVATE spymarine)
target_enable_warnings(moving_average)

if(SPYMARINE_ENABLE_HOME_ASSISTANT_SUPPORT)
    add_executable(print_home_assistant_discovery)
    target_sources(
        print_home_assistant_discovery
        PRIVATE print_home_assistant_discovery.cpp
    )
    target_link_libraries(print_home_assistant_discovery PRIVATE spymarine)
    target_enable_warnings(print_home_assistant_discovery)
endif()
