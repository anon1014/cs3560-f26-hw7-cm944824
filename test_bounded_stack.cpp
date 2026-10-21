#include "catch_amalgamated.hpp"
#include "bounded_stack.hpp"

#include <stdexcept>
#include <string>

// ---------------------------------------------------------------------------
// The rule, as one sentence, used verbatim as the test name.
// Covers E3 (partly filled) and E4 (full) end to end.
// ---------------------------------------------------------------------------
TEST_CASE("A BoundedStack of capacity N returns items in last-in-first-out order "
          "and refuses any push once it already holds N items",
          "[rule]") {
    BoundedStack<int> stack(3);

    stack.push(10);
    stack.push(20);
    stack.push(30);
    REQUIRE(stack.full());  // if it isn't full, the overflow check below means nothing

    CHECK_THROWS_AS(stack.push(40), std::overflow_error);

    CHECK(stack.pop() == 30);
    CHECK(stack.pop() == 20);
    CHECK(stack.pop() == 10);
    CHECK(stack.empty());
}

// E1, B1 — valid capacity, empty boundary
TEST_CASE("A newly constructed stack is empty and reports its capacity", "[class][boundary]") {
    BoundedStack<int> stack(5);

    CHECK(stack.empty());
    CHECK_FALSE(stack.full());
    CHECK(stack.size() == 0);
    CHECK(stack.capacity() == 5);
}

// E2, B6 — malformed input: capacity zero
TEST_CASE("Constructing a stack with capacity zero is rejected", "[class][malformed]") {
    CHECK_THROWS_AS(BoundedStack<int>(0), std::invalid_argument);
}

// E5, B1 — pop on the empty boundary
TEST_CASE("Popping an empty stack throws underflow_error", "[class][boundary]") {
    BoundedStack<int> stack(3);
    REQUIRE(stack.empty());

    CHECK_THROWS_AS(stack.pop(), std::underflow_error);
    CHECK(stack.size() == 0);  // the failed pop must not corrupt the size
}

// B2 — one element
TEST_CASE("A stack holding one element pops that element and becomes empty", "[boundary]") {
    BoundedStack<std::string> stack(3);
    stack.push("only");
    REQUIRE(stack.size() == 1);

    CHECK(stack.pop() == "only");
    CHECK(stack.empty());
    CHECK(stack.size() == 0);
}

// B3 — the maximum
TEST_CASE("Pushing exactly capacity items fills the stack without error", "[boundary]") {
    BoundedStack<int> stack(4);

    for (int i = 1; i <= 4; ++i) {
        REQUIRE_NOTHROW(stack.push(i));  // a failure here makes the remaining checks meaningless
    }

    CHECK(stack.full());
    CHECK(stack.size() == 4);
    CHECK(stack.pop() == 4);  // last one in is on top
}

// B4, E4 — one past the maximum
TEST_CASE("Pushing one item past capacity throws and leaves the contents unchanged", "[boundary]") {
    BoundedStack<int> stack(2);
    stack.push(1);
    stack.push(2);
    REQUIRE(stack.full());

    CHECK_THROWS_AS(stack.push(3), std::overflow_error);
    CHECK(stack.size() == 2);
    CHECK(stack.pop() == 2);  // 3 must not have replaced the top
    CHECK(stack.pop() == 1);
}

// B5 — smallest legal capacity
TEST_CASE("A stack of capacity one holds one item and refuses a second", "[boundary]") {
    BoundedStack<char> stack(1);

    REQUIRE_NOTHROW(stack.push('a'));
    CHECK(stack.full());
    CHECK_THROWS_AS(stack.push('b'), std::overflow_error);
    CHECK(stack.pop() == 'a');
    CHECK(stack.empty());
}

// B7 — dropping back below the maximum and returning to it
TEST_CASE("Popping from a full stack makes room for exactly one more push", "[boundary]") {
    BoundedStack<int> stack(2);
    stack.push(1);
    stack.push(2);
    REQUIRE(stack.full());

    stack.pop();
    CHECK_FALSE(stack.full());
    CHECK_NOTHROW(stack.push(99));
    CHECK(stack.full());
    CHECK_THROWS_AS(stack.push(100), std::overflow_error);
    CHECK(stack.pop() == 99);
}
