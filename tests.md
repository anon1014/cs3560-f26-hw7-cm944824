# Test design: `BoundedStack<T>`

## The rule (one sentence)

> **A BoundedStack of capacity N returns items in last-in-first-out order and refuses any push once it already holds N items.**

This sentence is also the name of the main test case in `test_bounded_stack.cpp`.

## The unit

```cpp
template <typename T>
class BoundedStack {
public:
    explicit BoundedStack(std::size_t capacity); // throws std::invalid_argument if capacity == 0
    void push(const T& item);                    // throws std::overflow_error if full
    T pop();                                     // throws std::underflow_error if empty
    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;
    bool full() const;
};
```

## Inputs that matter

The behaviour depends on two things: the **capacity chosen at construction**, and the
**number of items currently held** (call it `n`) when `push` or `pop` is called.

## Equivalence classes

| # | Class | Representative | Expected behaviour |
|---|-------|----------------|--------------------|
| E1 | Valid capacity (≥ 1) | capacity 3 | Constructs; empty, size 0 |
| E2 | Invalid capacity (0) — *malformed input* | capacity 0 | Constructor throws `std::invalid_argument` |
| E3 | Partly filled stack (0 < n < N) | 2 of 3 items | Push succeeds; pops come out in LIFO order |
| E4 | Full stack (n == N) | 3 of 3 items | Push throws `std::overflow_error`; contents unchanged |
| E5 | Empty stack (n == 0) | fresh stack | Pop throws `std::underflow_error` |

## Boundaries

| # | Boundary | What it checks |
|---|----------|----------------|
| B1 | **Empty** (n = 0) | `empty()` true, `size()` 0, pop throws |
| B2 | **One element** (n = 1) | Pop returns that element and the stack is empty again |
| B3 | **The maximum** (n = N) | The N-th push succeeds; `full()` becomes true |
| B4 | **One past the maximum** (push when n = N) | Push throws; size stays N; top item unchanged |
| B5 | **Smallest legal capacity** (N = 1) | Boundary of the capacity input: holds one item, refuses a second |
| B6 | **Malformed input** (capacity 0) | Constructor rejects it (same as E2) |
| B7 | **Recovery after the maximum** | After pop from a full stack, a push works again (n goes N → N-1 → N) |

## Test plan (one test per class, plus the boundaries)

| Test name | Covers |
|-----------|--------|
| A BoundedStack of capacity N returns items in last-in-first-out order and refuses any push once it already holds N items | E3, E4 (the whole rule end to end) |
| A newly constructed stack is empty and reports its capacity | E1, B1 |
| Constructing a stack with capacity zero is rejected | E2, B6 |
| Popping an empty stack throws underflow_error | E5, B1 |
| A stack holding one element pops that element and becomes empty | B2 |
| Pushing exactly capacity items fills the stack without error | B3 |
| Pushing one item past capacity throws and leaves the contents unchanged | B4, E4 |
| A stack of capacity one holds one item and refuses a second | B5 |
| Popping from a full stack makes room for exactly one more push | B7 |

## REQUIRE vs CHECK

- `REQUIRE` is used where a failure makes the rest of the test meaningless — e.g. the
  setup pushes must succeed, and the stack must be full before we test "one past the maximum".
- `CHECK` is used for the observable outcomes we want reported individually — every
  popped value, `size()`, `empty()`, `full()` — so a single run shows *all* the things that are wrong.
