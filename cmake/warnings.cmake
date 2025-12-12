# Compiler warning configuration functions

# Function to apply strict warning flags to targets
function(target_enable_warnings target_name)
    if(
        SPYMARINE_ENABLE_ALL_WARNINGS
        AND (
            CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
            OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
        )
    )
        target_compile_options(
            ${target_name}
            PRIVATE
                -Weverything
                -Wno-c++98-compat
                -Wno-c++98-compat-pedantic
                -Wno-c++20-compat
                -Wno-padded
                -Wno-poison-system-directories
                -Wno-switch-default
                -Wno-weak-vtables
                # TOOD store sensor values in their native format
                # and enable this warning again.
                -Wno-float-equal
                -Werror
        )
    else()
        target_compile_options(
            ${target_name}
            PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
        )
    endif()
endfunction()
