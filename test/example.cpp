#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstring>
#include <cstdio>

#define MOCK_WEAK_DLFCN 1
#include "mock_weak.h"

#define DGTEST_TEST(a, b) GTEST_TEST(a, DISABLED_##b)

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCK_WEAK_METHOD0(getchar, int (void));

GTEST_TEST(mock_WEAK, example_getchar)
{
    MOCK_WEAK_GUARD(getchar); // enable call to mock

    // use instance of mock for EXPECT_CALL
    EXPECT_CALL(MOCK_WEAK_INSTANCE(getchar), getchar())
    .WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
} // disable  call to mock

// auto find argument type with decltype
MOCK_WEAK_DECLTYPE_METHOD3(write);

GTEST_TEST(mock_WEAK, example_write)
{
    // simply use EXPECT_CALL with MOCK_WEAK
    MOCK_WEAK_EXPECT_CALL(write, (0, _, 42))
    .WillRepeatedly(
        Invoke([](int /* fd */, const void* /* buf */, size_t /* nbytes */) {
            // use real (not recursive call to mock)
            write(STDOUT_FILENO, "write: real in mock!\n", 21);
            return -42;
        })
    );

    {
        MOCK_WEAK_GUARD(write); // enable call to mock
        EXPECT_EQ(write(0, "mock", 42), -42); // use mock
    } // disable call to mock
    write(STDOUT_FILENO, "write: use real\n", 16); // use real
}