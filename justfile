# Justfile for building the BPE tokenizer

# Compiler and flags
CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude

# Default target
default: build

# Ensure dist/ exists
@dist_dir := "dist"

# Build binary (default: release)
build:
    mkdir -p {{@dist_dir}}
    {{CXX}} {{CXXFLAGS}} -O2 src/bpe.cc -o {{@dist_dir}}/bpe
    echo "✅ Built dist/bpe"

# Debug build
debug:
    mkdir -p {{@dist_dir}}
    {{CXX}} {{CXXFLAGS}} -g -O0 src/bpe.cc -o {{@dist_dir}}/bpe
    echo "🐞 Debug build ready: dist/bpe"

# Run the binary
run:
    just build
    ./dist/bpe

# Clean build artifacts
clean:
    rm -rf {{@dist_dir}}
    echo "🧹 Cleaned dist/"
