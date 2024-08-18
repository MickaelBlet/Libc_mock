#include <unistd.h>

#include "blet/mockf.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

MOCKF_FUNCTION3(ssize_t, read, (int fd, void* buf, size_t nbyte));

TEST(mockf, example_read) {
    MOCKF_INIT(read);
    MOCKF_GUARD(read);

    // use test class mock attibute for EXPECT_CALL
    EXPECT_CALL(MOCKF_INSTANCE(read), read(_, _, _)).WillOnce(Return(-42));

    EXPECT_EQ(read(0, NULL, 0), -42);
}
