#include <stdio.h>

#include "mockc.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

MOCKC_METHOD0(getchar, int(void));

GTEST_TEST(mockc, example_getchar) {
    MOCKC_NEW_INSTANCE(getchar); // create new mock instance

    MOCKC_GUARD(getchar); // enable call to mock

    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCKC_INSTANCE(getchar), getchar()).WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
} // disable  call to mock