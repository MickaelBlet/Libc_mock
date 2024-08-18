#include <string.h>

#include "blet/mockf.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// create new function and singleton instance for mock
MOCKF_FUNCTION2(int, strcmp, (const char* __s1, const char* __s2));

ACTION(strcmp) {
    return -42;
}

TEST(mockf, example_strcmp) {
    MOCKF_INIT(strcmp);

    // simply use EXPECT_CALL with MOCKF
    EXPECT_CALL(MOCKF_INSTANCE(strcmp), strcmp(_, _)).WillRepeatedly(strcmp());

    {
        MOCKF_GUARD(strcmp);                    // enable call to mock
        EXPECT_EQ(strcmp("mock", "mock"), -42); // use mock
    }                                           // disable call to mock

    EXPECT_EQ(strcmp("mock", "mock"), 0); // use real
}
