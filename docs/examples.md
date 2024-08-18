# Examples

## Testing class

```cpp
#include <unistd.h> // write

#include "blet/mockf.h"

using ::testing::_;

MOCKF_FUNCTION3(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));

ACTION(write) {
    write(STDOUT_FILENO, "write: real in mock!\n", sizeof("write: real in mock!\n") - 1);
    return -42;
}

class WriteTest : public ::testing::Test {
  protected:
    void SetUp() {
        MOCKF_ENABLE(write);
    }
    void TearDown() {
        MOCKF_DISABLE(write);
    }

    MOCKF_INIT(write);
};

TEST_F(WriteTest, example_write) {
    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCKF_INSTANCE(write), write(_, _, _)).WillRepeatedly(write());

    // use mock
    EXPECT_EQ(write(0, "mock", sizeof("mock") - 1), -42);

    {
        // disable call to mock
        MOCKF_GUARD_REVERSE(write);
        // use real
        write(STDOUT_FILENO, "write: use real\n", sizeof("write: use real\n") - 1);
    }

    // use mock
    EXPECT_EQ(write(0, "mock", sizeof("mock") - 1), -42);
}
```

### Result

```
Running main() from gmock_main.cc
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from WriteTest
[ RUN      ] WriteTest.example_write
write: real in mock!
write: use real
write: real in mock!
[       OK ] WriteTest.example_write (0 ms)
[----------] 1 test from WriteTest (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.
```

## Variadic function solution

```cpp
#include <sys/ioctl.h> // ioctl

#include "blet/mockf.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// tranform '...' to 'va_list'
MOCKF_VARIADIC_ATTRIBUTE_FUNCTION3(int, ioctl, (int /* fd */, unsigned long int /* request */, ...), throw());

ACTION(ioctl) {
    // take the argument from va_list
    int i = (int)va_arg(arg2, int);
    int j = (int)va_arg(arg2, int);

    printf("%i, %lu, %i, %i\n", arg0, arg1, i, j);

    return -42;
}

TEST(mockf, ioctl) {
    MOCKF_INIT(ioctl);

    // simply use EXPECT_CALL with MOCKF
    MOCKF_EXPECT_CALL(ioctl, (_, _, _)).WillOnce(ioctl());

    {
        MOCKF_GUARD(ioctl);                 // enable call to mock
        EXPECT_EQ(ioctl(42, 0, 1, 2), -42); // use mock
    }                                       // disable call to mock
}

```

```
Running main() from gmock_main.cc
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from mockf
[ RUN      ] mockf.ioctl
42, 0, 1, 2
[       OK ] mockf.ioctl (0 ms)
[----------] 1 test from mockf (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```