# Compiler warning configuration functions

# Function to apply strict warning flags to targets
function(target_enable_warnings target_name)
    target_compile_options(${target_name} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Werror
    )
endfunction()
