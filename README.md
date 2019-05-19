# Mock_weak

### example

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>

#include "mock_weak.h"

using ::testing::_;
using ::testing::Invoke;

MOCK_WEAK_METHOD3(read, ssize_t (int, void*, size_t));

// GTEST_TEST(mock_WEAK, DISABLED_example)
GTEST_TEST(mock_WEAK, example)
{
    EXPECT_CALL(MOCK_WEAK_INSTANCE(read), read(_,_,_))
    .WillOnce(
        Invoke([](int fd, void *buf, size_t nbytes) -> ssize_t {
            EXPECT_EQ(fd, 0);
            EXPECT_EQ(nbytes, 42);
            std::memcpy(buf, "fake read", 10);
            return 1;
        })
    );

    char buffer[42];
    EXPECT_EQ(read(0, buffer, 42), 1);
    EXPECT_STREQ(buffer, "fake read");
}
```
