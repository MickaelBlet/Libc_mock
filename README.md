# MockC
Mock **C** function with lower or equal version `1.8.1` of google mock.  
Compile with dl library
```cpp
using ::testing::_;
// create class mock
MOCKC_METHOD3(write, ssize_t(int /* fd */, const void* /* buf */, size_t /* nbytes */)) // at top of source
MOCKC_DECLTYPE_METHOD3(write) // same __cplusplus >= 201103L

// in test method
MOCKC_NEW_INSTANCE(write); // create a local mock
MOCKC_INSTANCE(write) // get mockc instance of getchar
MOCKC_EXPECT_CALL(write, (_, _, _)) // simply use EXPECT_CALL
// same EXPECT_CALL(MOCKC_INSTANCE(write), write(_, _, _))

// in test method
MOCKC_ENABLE(write) // active mock method
MOCKC_DISABLE(write) // desactive mock method
MOCKC_GUARD(write) // create a guard for mock method
MOCKC_GUARD_REVERSE(write) // disable at constructor > enable at destructor
```

### Example:
```cpp
#include "unistd.h"

#include "mockc.h"

using ::testing::_;

MOCKC_METHOD3(write, ssize_t(int /* fd */, const void* /* buf */, size_t /* nbytes */)); // cpp98

ACTION(write) {
    write(STDOUT_FILENO, "write: real in mock!\n", sizeof("write: real in mock!\n") - 1);
    return -42;
}

GTEST_TEST(mockc, example_write) {
    MOCKC_NEW_INSTANCE(write);

    // simply use EXPECT_CALL with MOCKC
    MOCKC_EXPECT_CALL(write, (0, _, 42)).WillRepeatedly(write());

    {
        MOCKC_GUARD(write);                   // enable call to mock
        EXPECT_EQ(write(0, "mock", 42), -42); // use mock
    }                                         // disable call to mock

    write(STDOUT_FILENO, "write: use real\n", sizeof("write: use real\n") - 1); // use real
}
```
### Result:
```
Running main() from gmock_main.cc
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from mockc
[ RUN      ] mockc.example_write
write: real in mock!
write: use real
[       OK ] mockc.example_write (3 ms)
[ RUN      ] mockc.example_write2
write: real in mock!
write: use real
[       OK ] mockc.example_write2 (0 ms)
[----------] 2 tests from mockc (3 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (3 ms total)
[  PASSED  ] 2 tests.
```