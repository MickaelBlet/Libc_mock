# MockC
Mock **C** function with google mock

```c
// create class mock
MOCKC_METHOD0(getchar, int (void)) // at top of source
MOCKC_DECLTYPE_METHOD0(getchar) // same

// in test method
MOCKC_INSTANCE(getchar) // get mockc instance of getchar
MOCKC_EXPECT_CALL(getchar, ()) // simply use EXPECT_CALL
// same EXPECT_CALL(MOCKC_INSTANCE(getchar), getchar())
```
With *dlfcn* the mockc is not active by default
```c
// in test method
MOCKC_ENABLE(getchar) // active mock method
MOCKC_DISABLE(getchar) // desactive mock method
MOCKC_GUARD(getchar) // create a guard for mock method
MOCKC_GUARD_REVERSE(getchar) // disable at constructor > enable at destructor
```

### Example:
```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#include "mockc.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCKC_METHOD0(getchar, int (void));

GTEST_TEST(mockc, example_getchar)
{
    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCKC_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
}

// auto find argument type with decltype
MOCKC_DECLTYPE_METHOD3(read);

GTEST_TEST(mockc, example_read)
{
    // simply use EXPECT_CALL with MOCKC
    MOCKC_EXPECT_CALL(read, (0, _, 42))
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
[----------] 2 tests from mockc
[ RUN      ] mockc.example_getchar
[       OK ] mockc.example_getchar (0 ms)
[ RUN      ] mockc.example_read
[       OK ] mockc.example_read (1 ms)
[----------] 2 tests from mockc (2 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (5 ms total)
[  PASSED  ] 2 tests.
```
---
### Example with "dlfcn" :
*Need include libdl (-ldl) and define (-DMOCKC_DLFCN)*

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#include "mockc.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCKC_METHOD0(getchar, int (void));

GTEST_TEST(mockc, example_getchar)
{
    MOCKC_GUARD(getchar); // enable call to mock

    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCKC_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
} // disable  call to mock

// auto find argument type with decltype
MOCKC_DECLTYPE_METHOD3(write);

GTEST_TEST(mockc, example_write)
{
    // simply use EXPECT_CALL with MOCKC
    MOCKC_EXPECT_CALL(write, (0, _, 42))
    .WillRepeatedly(
        Invoke([](int /* fd */, const void* /* buf */, size_t /* nbytes */) {
            // use real (not recursive call to mock)
            write(STDOUT_FILENO, "write: real in mock!\n", 21);
            return -42;
        })
    );

    {
        MOCKC_GUARD(write); // enable call to mock
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
[----------] 2 tests from mockc
[ RUN      ] mockc.example_getchar
[       OK ] mockc.example_getchar (0 ms)
[ RUN      ] mockc.example_write
write: real in mock!
write: use real
[       OK ] mockc.example_write (1 ms)
[----------] 2 tests from mockc (5 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (14 ms total)
[  PASSED  ] 2 tests.
```
