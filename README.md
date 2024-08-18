# MockF

Mock **C**/**C++** function with gmock.  
Work with `dl` library.  
For more examples, see [docs/examples.md](docs/examples.md).

## Quickstart

```cpp
#include "unistd.h" // write

#include "blet/mockf.h"

using ::testing::_;

// create mock class for write
MOCKF_FUNCTION3(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));

ACTION(write) {
    write(STDOUT_FILENO, "write: real in mock!\n", sizeof("write: real in mock!\n") - 1);
    return -42;
}

TEST(mockf, example_write) {
    // initialize a instance of write mock
    MOCKF_INIT(write);

    // simply use EXPECT_CALL with MOCKF
    MOCKF_EXPECT_CALL(write, (0, _, _))
        .WillRepeatedly(write()); // use write action

    {
        // enable mock
        MOCKF_GUARD(write);
        // use mock
        EXPECT_EQ(write(0, "mock", sizeof("mock") - 1), -42); 
    } // disable mock

    // use real write function
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
// create class mock
// at top of test source after include
MOCKF_FUNCTION(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));
// or if C++98 and pedantic flag
MOCKF_FUNCTION3(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));
// with attribute(s)
MOCKF_ATTRIBUTE_FUNCTION(int, stat, (const char* /* file */, struct stat* /* buf */), throw());
// or if C++98 and pedantic flag
MOCKF_ATTRIBUTE_FUNCTION2(int, stat, (const char* /* file */, struct stat* /* buf */), throw());

// create class variadic mock
// at top of test source after include
// create a mock replace '...' with 'va_list'
MOCKF_VARIADIC_FUNCTION(int, fcntl (int /* fd */, int /* cmd */, ...));
// or if C++98 and pedantic flag
MOCKF_VARIADIC_FUNCTION3(int, fcntl (int /* fd */, int /* cmd */, ...));
// with attribute(s)
MOCKF_VARIADIC_ATTRIBUTE_FUNCTION(int, ioctl, (int /* fd */, unsigned long int /* request */, ...), throw());
// or if C++98 and pedantic flag
MOCKF_VARIADIC_ATTRIBUTE_FUNCTION3(int, ioctl, (int /* fd */, unsigned long int /* request */, ...), throw());

// create a instance of write mock class (mockf_write)
MOCKF_INIT(write);
// get mockf instance of write
MOCKF_INSTANCE(write)

// active mock function
MOCKF_ENABLE(write);
// desactive mock function
MOCKF_DISABLE(write);
// create a guard for mock function
MOCKF_GUARD(write);
// disable at constructor, enable at destructor
MOCKF_GUARD_REVERSE(write);

// simply use EXPECT_CALL
MOCKF_EXPECT_CALL(write, (::testing::_, ::testing::_, ::testing::_))
// same EXPECT_CALL(MOCKF_INSTANCE(write), write(::testing::_, ::testing::_, ::testing::_))

// for C++98 with pedantic flag you can add 'MOCKF_DISABLE_VARIADIC_MACROS' definition for disable the variadic macros
```