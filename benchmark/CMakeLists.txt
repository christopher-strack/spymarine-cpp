enable_testing()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(spymarine_benchmarks)

target_sources(
    spymarine_benchmarks
    PRIVATE
        benchmark_message_values_view.cpp
)

target_link_libraries(
    spymarine_benchmarks
    PRIVATE
        Catch2::Catch2WithMain
        spymarine
)

target_enable_warnings(spymarine_benchmarks)
