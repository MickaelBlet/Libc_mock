# Mock_weak

### example

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#include "mock_weak.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

MOCK_WEAK_METHOD0(getchar, int (void));
MOCK_WEAK_METHOD3(read, ssize_t (int, void*, size_t));

// GTEST_TEST(mock_WEAK, DISABLED_example)
GTEST_TEST(mock_WEAK, example)
{
    EXPECT_CALL(MOCK_WEAK_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_CALL(MOCK_WEAK_INSTANCE(read), read(0, _, 42))
    .WillOnce(
        Invoke([](int /* fd */, void *buf, size_t /* nbytes */) -> ssize_t {
            std::memcpy(buf, "fake read", 10);
            return 42;
        })
    );

    EXPECT_EQ(getchar(), -42);

    char buffer[42];
    EXPECT_EQ(read(0, buffer, 42), 42);
    EXPECT_STREQ(buffer, "fake read");
}
```
