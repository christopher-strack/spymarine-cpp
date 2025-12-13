add_library(spymarine)

target_sources(
    spymarine
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES
            ${PROJECT_SOURCE_DIR}/include/spymarine/byte_operations.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/client.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/crc.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/defaults.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/device.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/error.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/file_descriptor.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/hub.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message_value.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message_values_iterator.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/message_values_view.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/overloaded.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_device2.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_error.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_message_header.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_message_type.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/parse_message.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/rational.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/sensor2.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/tcp_socket.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/udp_socket.hpp
            ${PROJECT_SOURCE_DIR}/include/spymarine/unit.hpp
            $<$<BOOL:${SPYMARINE_ENABLE_HOME_ASSISTANT_SUPPORT}>:${PROJECT_SOURCE_DIR}/include/spymarine/home_assistant2.hpp>
    PRIVATE
        error.cpp
        file_descriptor.cpp
        tcp_socket.cpp
        udp_socket.cpp
)

set_target_properties(spymarine PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

target_compile_definitions(
    spymarine
    PUBLIC SPYMARINE_PROJECT_VERSION="${PROJECT_VERSION}"
)

target_enable_warnings(spymarine)
