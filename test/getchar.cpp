#include <stdio.h>

#include "blet/mockf.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

MOCKF_FUNCTION0(int, getchar, ());

struct mockf : public ::testing::Test {
    void SetUp() {
        MOCKF_ENABLE(getchar);
    }
    void TearDown() {
        MOCKF_DISABLE(getchar);
    }
    MOCKF_INIT(getchar);
};

TEST_F(mockf, example_getchar) {
    EXPECT_CALL(MOCKF_INSTANCE(getchar), getchar()).WillOnce(Return(-42));

    EXPECT_EQ(getchar(), -42);
}
