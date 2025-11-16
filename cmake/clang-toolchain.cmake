include_guard(GLOBAL)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

if (SPYMARINE_ENABLE_SANITIZERS)
    set(SANITIZER_FLAGS
        "-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined"
    )
endif()

set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${SANITIZER_FLAGS}")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}")
