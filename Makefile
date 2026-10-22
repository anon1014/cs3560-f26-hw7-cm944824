# Top-level Makefile: `make test` is the one command anyone needs.
#
# CMake generates its own Makefiles inside $(BUILD_DIR), so they never
# overwrite this one.

BUILD_DIR := build

# These targets are names of actions, not files. Without .PHONY, a file or
# folder called "test" (or "build", "clean") would look up to date and make
# would skip the recipe.
.PHONY: all build test clean

all: build

build:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

clean:
	rm -rf $(BUILD_DIR)
