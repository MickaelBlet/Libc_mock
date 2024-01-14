#include "mockc.h"
#include "unistd.h"

using ::testing::_;

MOCKC_METHOD3(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */)); // cpp98

ACTION(write) {
    write(STDOUT_FILENO, "write: real in mock!\n", sizeof("write: real in mock!\n") - 1);
    return -42;
}

GTEST_TEST(mockc, example_write) {
    MOCKC_NEW_INSTANCE(write);

    // simply use EXPECT_CALL with MOCKC
    MOCKC_EXPECT_CALL(write, (0, _, 42)).WillRepeatedly(write());

    {
        MOCKC_GUARD(write);                   // enable call to mock
        EXPECT_EQ(write(0, "mock", 42), -42); // use mock
    }                                         // disable call to mock

    write(STDOUT_FILENO, "write: use real\n", sizeof("write: use real\n") - 1); // use real
}