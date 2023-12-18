#include <string.h>

#include "mockc.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

#define STR_AND_SIZE(a) a, sizeof(a) - 1

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCKC_METHOD2(strcmp, int(const char* __s1, const char* __s2)); // cpp98

ACTION(strcmp) {
    MOCKC_GUARD_REVERSE(strcmp);
    return -42;
}

GTEST_TEST(mockc, example_strcmp) {
    MOCKC_NEW_INSTANCE(strcmp);

    // simply use EXPECT_CALL with MOCKC
    MOCKC_EXPECT_CALL(strcmp, (_, _)).WillRepeatedly(strcmp());

    {
        MOCKC_GUARD(strcmp);                    // enable call to mock
        EXPECT_EQ(strcmp("mock", "mock"), -42); // use mock
    }                                           // disable call to mock

    EXPECT_EQ(strcmp("mock", "mock"), 0); // use real
}