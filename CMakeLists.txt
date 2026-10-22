cmake_minimum_required(VERSION 3.19)
project(BoundedStackTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Catch2 (single-file "amalgamated" version) built once as a library.
add_library(catch2 STATIC catch_amalgamated.cpp)
target_include_directories(catch2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# The test executable.
add_executable(tests test_bounded_stack.cpp)
target_link_libraries(tests PRIVATE catch2)

# ---------------------------------------------------------------------------
# Register the tests with CTest.
# Each Catch2 TEST_CASE is registered on its own, so ctest reports them
# individually. The name passed to the executable selects exactly that case.
# ---------------------------------------------------------------------------
enable_testing()

set(TEST_CASES
    "A BoundedStack of capacity N returns items in last-in-first-out order and refuses any push once it already holds N items"
    "A newly constructed stack is empty and reports its capacity"
    "Constructing a stack with capacity zero is rejected"
    "Popping an empty stack throws underflow_error"
    "A stack holding one element pops that element and becomes empty"
    "Pushing exactly capacity items fills the stack without error"
    "Pushing one item past capacity throws and leaves the contents unchanged"
    "A stack of capacity one holds one item and refuses a second"
    "Popping from a full stack makes room for exactly one more push"
)

foreach(test_name IN LISTS TEST_CASES)
    add_test(NAME "${test_name}" COMMAND tests "${test_name}")
endforeach()
