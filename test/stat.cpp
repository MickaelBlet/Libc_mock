#include <sys/stat.h> // stat

#include "blet/mockf.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

MOCKF_ATTRIBUTE_FUNCTION2(int, stat, (const char* __restrict __file, struct stat* __restrict __buf), throw());

ACTION_P2(stat, st_mode, ret) {
    arg1->st_mode = st_mode;
    return ret;
}

struct mockf : public ::testing::Test {
    void SetUp() {
        MOCKF_ENABLE(stat);
    }
    void TearDown() {
        MOCKF_DISABLE(stat);
    }
    MOCKF_INIT(stat);
};

TEST_F(mockf, example_getchar) {
    // use instance of mock for EXPECT_CALL
    MOCKF_EXPECT_CALL(stat, (_, _)).WillOnce(stat(__S_IFREG, -42));

    struct stat statOfMock;
    EXPECT_EQ(stat("nothing", &statOfMock), -42);
    EXPECT_EQ(statOfMock.st_mode, static_cast<mode_t>(__S_IFREG));
}
