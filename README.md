# Mock_weak
Mock weak **C** function with google mock

```c
// create class mock
MOCK_WEAK_METHOD0(getchar, int (void)) // at top of source
MOCK_WEAK_DECLTYPE_METHOD0(getchar) // same

// in test method
MOCK_WEAK_INSTANCE(getchar) // get mock weak instance of getchar
MOCK_WEAK_EXPECT_CALL(getchar, ()) // simply use EXPECT_CALL
// same EXPECT_CALL(MOCK_WEAK_INSTANCE(getchar), getchar())
```
With *dlfcn* the mock weak is not active by default
```c
// in test method
MOCK_WEAK_ENABLE(getchar) // active mock method
MOCK_WEAK_DISABLE(getchar) // desactive mock method
MOCK_WEAK_GUARD(getchar) // create a guard for mock method
MOCK_WEAK_GUARD_REVERSE(getchar) // disable at constructor > enable at destructor
```

### Example:
```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#include "mock_weak.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCK_WEAK_METHOD0(getchar, int (void));

GTEST_TEST(mock_WEAK, example_getchar)
{
    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCK_WEAK_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
}

// auto find argument type with decltype
MOCK_WEAK_DECLTYPE_METHOD3(read);

GTEST_TEST(mock_WEAK, example_read)
{
    // simply use EXPECT_CALL with MOCK_WEAK
    MOCK_WEAK_EXPECT_CALL(read, (0, _, 42))
    .WillOnce(
        Invoke([](int /* fd */, void *buf, size_t /* nbytes */) -> ssize_t {
            std::memcpy(buf, "fake read", 10);
            return 42;
        })
    );

    char buffer[42];
    EXPECT_EQ(read(0, buffer, 42), 42);
    EXPECT_STREQ(buffer, "fake read");
}
```
### Result:
```
Running main() from gtest_main.cc
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from mock_WEAK
[ RUN      ] mock_WEAK.example_getchar
[       OK ] mock_WEAK.example_getchar (0 ms)
[ RUN      ] mock_WEAK.example_read
[       OK ] mock_WEAK.example_read (1 ms)
[----------] 2 tests from mock_WEAK (2 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (5 ms total)
[  PASSED  ] 2 tests.
```
---
### Example with "dlfcn" :
*Need include libdl (-ldl)*

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#define MOCK_WEAK_DLFCN 1
#include "mock_weak.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCK_WEAK_METHOD0(getchar, int (void));

GTEST_TEST(mock_WEAK, example_getchar)
{
    MOCK_WEAK_GUARD(getchar); // enable call to mock

    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCK_WEAK_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
} // disable  call to mock

// auto find argument type with decltype
MOCK_WEAK_DECLTYPE_METHOD3(write);

GTEST_TEST(mock_WEAK, example_write)
{
    // simply use EXPECT_CALL with MOCK_WEAK
    MOCK_WEAK_EXPECT_CALL(write, (0, _, 42))
    .WillRepeatedly(
        Invoke([](int /* fd */, const void* /* buf */, size_t /* nbytes */) {
            // use real (not recursive call to mock)
            write(STDOUT_FILENO, "write: real in mock!\n", 21);
            return -42;
        })
    );

    {
        MOCK_WEAK_GUARD(write); // enable call to mock
        EXPECT_EQ(write(0, "mock", 42), -42); // use mock
    } // disable call to mock
    write(STDOUT_FILENO, "write: use real\n", 16); // use real
}
```
### Result:
```
Running main() from gtest_main.cc
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from mock_WEAK
[ RUN      ] mock_WEAK.example_getchar
[       OK ] mock_WEAK.example_getchar (0 ms)
[ RUN      ] mock_WEAK.example_write
write: real in mock!
write: use real
[       OK ] mock_WEAK.example_write (1 ms)
[----------] 2 tests from mock_WEAK (5 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (14 ms total)
[  PASSED  ] 2 tests.
```
