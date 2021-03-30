#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#include "mockc.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

#define STR_AND_SIZE(a) a, sizeof(a) - 1

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCKC_METHOD0(getchar, int (void));

GTEST_TEST(mockc, example_getchar) {
    MOCKC_GUARD(getchar); // enable call to mock

    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCKC_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
} // disable  call to mock

// auto find argument type with decltype
// MOCKC_METHOD3(write, ssize_t (int /* fd */, const void* /* buf */, size_t /* nbytes */)) // cpp98
MOCKC_DECLTYPE_METHOD3(write);

ACTION(write) {
    write(STDOUT_FILENO, STR_AND_SIZE("write: real in mock!\n"));
    return -42;
}

GTEST_TEST(mockc, example_write) {
    // simply use EXPECT_CALL with MOCKC
    MOCKC_EXPECT_CALL(write, (0, _, 42))
    .WillRepeatedly(write());

    {
        MOCKC_GUARD(write); // enable call to mock
        EXPECT_EQ(write(0, "mock", 42), -42); // use mock
    } // disable call to mock

    write(STDOUT_FILENO, STR_AND_SIZE("write: use real\n")); // use real
}