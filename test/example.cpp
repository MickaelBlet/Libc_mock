#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#include "mockw.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

#define STR_AND_SIZE(a) a, sizeof(a) - 1

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCKW_METHOD0(getchar, int (void))

GTEST_TEST(mock_WEAK, example_getchar) {
    MOCKW_GUARD(getchar); // enable call to mock

    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCKW_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
} // disable  call to mock

// auto find argument type with decltype
MOCKW_METHOD3(write, ssize_t (int /* fd */, const void* /* buf */, size_t /* nbytes */))

ACTION(write) {
    write(STDOUT_FILENO, STR_AND_SIZE("write: real in mock!\n"));
    return -42;
}

GTEST_TEST(mock_WEAK, example_write) {
    // simply use EXPECT_CALL with MOCKW
    MOCKW_EXPECT_CALL(write, (0, _, 42))
    .WillRepeatedly(write());

    {
        MOCKW_GUARD(write); // enable call to mock
        EXPECT_EQ(write(0, "mock", 42), -42); // use mock
    } // disable call to mock

    write(STDOUT_FILENO, STR_AND_SIZE("write: use real\n")); // use real
}