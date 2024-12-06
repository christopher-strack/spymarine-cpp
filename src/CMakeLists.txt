add_library(spymarine)

target_sources(
    spymarine
    PRIVATE
        device_ostream.cpp
        device.cpp
        discover.cpp
        error.cpp
        file_descriptor.cpp
        home_assistant.cpp
        message.cpp
        message_values_iterator.cpp
        message_values_view.cpp
        message_value.cpp
        parse_device.cpp
        parse_message.cpp
        read_devices.cpp
        sensor.cpp
        sensor_reader.cpp
        tcp_socket.cpp
        udp_socket.cpp
)

target_sources(
    spymarine
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES
            ${PROJECT_SOURCE_DIR}/include/spymarine/defaults.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/device.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/device_ostream.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/discover.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/error.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/file_descriptor.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/home_assistant.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message_value.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message_values_iterator.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message_values_view.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/overloaded.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_device.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_error.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_message.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/read_devices.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/sensor.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/sensor_reader.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/tcp_socket.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/udp_socket.hpp
)

set_target_properties(spymarine PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

target_compile_definitions(spymarine PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")

target_compile_options(spymarine PRIVATE -Wall -Werror)
