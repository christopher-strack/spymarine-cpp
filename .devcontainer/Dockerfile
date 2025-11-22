FROM gcc:14.3.0

# Install CMake and other dependencies
RUN apt-get update && \
    apt-get install -y clangd clang-format cmake ninja-build && \
    rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Default command
CMD ["bash"]
