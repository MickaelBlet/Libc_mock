# MockF

Mock **C**/**C++** functions using gmock.  
Work with the `dl` library.  
For more examples, see [docs/examples.md](docs/examples.md).

## Quickstart

```cpp
#include "unistd.h" // write

#include "blet/mockf.h"

using ::testing::_;

// Create a mock class for write
MOCKF_FUNCTION3(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));

ACTION(write) {
    write(STDOUT_FILENO, "write: real in mock!\n", sizeof("write: real in mock!\n") - 1);
    return -42;
}

TEST(mockf, example_write) {
    // Initialize an instance of the write mock
    MOCKF_INIT(write);

    // Use EXPECT_CALL with MOCKF
    MOCKF_EXPECT_CALL(write, (0, _, _))
        .WillRepeatedly(write()); // Use the write action

    {
        // Enable mock
        MOCKF_GUARD(write);
        // Use mock
        EXPECT_EQ(write(0, "mock", sizeof("mock") - 1), -42); 
    } // Mock is disabled here

    // Use the real write function
    write(STDOUT_FILENO, "write: use real\n", sizeof("write: use real\n") - 1);
}
```

### Result

```
Running main() from gmock_main.cc
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from mockf
[ RUN      ] mockf.example_write
write: real in mock!
write: use real
[       OK ] mockf.example_write (0 ms)
[----------] 1 test from mockf (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.
```

## Macros

```cpp
// Create a mock class
// Place this at the top of the test source file after includes
MOCKF_FUNCTION(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));
// Or, if using C++98 with the pedantic flag
MOCKF_FUNCTION3(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));

// With attributes
MOCKF_ATTRIBUTE_FUNCTION(int, stat, (const char* /* file */, struct stat* /* buf */), throw());
// Or, if using C++98 with the pedantic flag
MOCKF_ATTRIBUTE_FUNCTION2(int, stat, (const char* /* file */, struct stat* /* buf */), throw());

// Create a variadic mock class
// Place this at the top of the test source file after includes
// Replace '...' with 'va_list'
MOCKF_VARIADIC_FUNCTION(int, fcntl (int /* fd */, int /* cmd */, ...));
// Or, if using C++98 with the pedantic flag
MOCKF_VARIADIC_FUNCTION3(int, fcntl (int /* fd */, int /* cmd */, ...));

// With attributes
MOCKF_VARIADIC_ATTRIBUTE_FUNCTION(int, ioctl, (int /* fd */, unsigned long int /* request */, ...), throw());
// Or, if using C++98 with the pedantic flag
MOCKF_VARIADIC_ATTRIBUTE_FUNCTION3(int, ioctl, (int /* fd */, unsigned long int /* request */, ...), throw());

// Create an instance of the write mock class (mockf_write)
MOCKF_INIT(write);
// Get the mockf instance of write
MOCKF_INSTANCE(write)

// Enable the mock function
MOCKF_ENABLE(write);
// Disable the mock function
MOCKF_DISABLE(write);
// Create a guard for the mock function
MOCKF_GUARD(write);
// Disable at construction, enable at destruction
MOCKF_GUARD_REVERSE(write);

// Use EXPECT_CALL
MOCKF_EXPECT_CALL(write, (::testing::_, ::testing::_, ::testing::_))
// Equivalent to EXPECT_CALL(MOCKF_INSTANCE(write), write(::testing::_, ::testing::_, ::testing::_))

// For C++98 with the pedantic flag, define 'MOCKF_DISABLE_VARIADIC_MACROS' to disable variadic macros
```