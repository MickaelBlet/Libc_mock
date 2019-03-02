# Libc_mock

### example

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>

#include "mock_libc.h"

using ::testing::_;
using ::testing::Invoke;

MOCK_LIBC_METHODE3(read, ssize_t (int, void*, size_t));

// GTEST_TEST(mock_libc, DISABLED_example)
GTEST_TEST(mock_libc, example)
{
    EXPECT_CALL(MOCK_LIBC_INSTANCE(read), read(_,_,_))
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
