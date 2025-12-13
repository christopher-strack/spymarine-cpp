include_guard(GLOBAL)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

if(SPYMARINE_ENABLE_SANITIZERS)
    if(
        CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin"
        AND CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64|aarch64"
    )
        # Apple Silicon doesn't support all sanitizers with GCC
        set(SANITIZER_FLAGS
            "-fsanitize=address -fsanitize=undefined -fsanitize-undefined-trap-on-error"
        )
    else()
        set(SANITIZER_FLAGS
            "-fsanitize=address -fsanitize=leak -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fsanitize-undefined-trap-on-error"
        )
    endif()
endif()

set(CMAKE_CXX_FLAGS "${SANITIZER_FLAGS}")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}")
