#include <unistd.h>

#include "blet/mockf.h"

using ::testing::_;

MOCKF_FUNCTION3(ssize_t, write, (int /* fd */, const void* /* buf */, size_t /* nbytes */));

ACTION(write) {
    write(STDOUT_FILENO, "write: real in mock!\n", sizeof("write: real in mock!\n") - 1);
    return -42;
}

struct WriteTest : public ::testing::Test {
    void SetUp() {
        MOCKF_ENABLE(write);
    }
    void TearDown() {
        MOCKF_DISABLE(write);
    }
    MOCKF_INIT(write);
};

TEST_F(WriteTest, example_write) {
    MOCKF_INIT(write);

    // simply use EXPECT_CALL with MOCKF
    MOCKF_EXPECT_CALL(write, (0, _, _)).WillRepeatedly(write());

    {
        MOCKF_GUARD(write);                                   // enable call to mock
        EXPECT_EQ(write(0, "mock", sizeof("mock") - 1), -42); // use mock
    }                                                         // disable call to mock

    write(STDOUT_FILENO, "write: use real\n", sizeof("write: use real\n") - 1); // use real
}

TEST_F(WriteTest, example_write2) {
    // simply use EXPECT_CALL with MOCKF
    MOCKF_EXPECT_CALL(write, (0, _, _)).WillRepeatedly(write());

    EXPECT_EQ(write(0, "mock", sizeof("mock") - 1), -42); // use mock

    {
        MOCKF_GUARD_REVERSE(write);                                                 // disable call to mock
        write(STDOUT_FILENO, "write: use real\n", sizeof("write: use real\n") - 1); // use real
    }
}
