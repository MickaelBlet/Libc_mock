#include <sys/ioctl.h> // ioctl

#include "blet/mockf.h"

using ::testing::_;

// tranform '...' to 'va_list'
MOCKF_VARIADIC_ATTRIBUTE_FUNCTION3(int, ioctl, (int /* fd */, unsigned long int /* request */, ...), throw());

ACTION(ioctl) {
    // take the argument from va_list
    int i = (int)va_arg(arg2, int);
    int j = (int)va_arg(arg2, int);

    printf("%i, %lu, %i, %i\n", arg0, arg1, i, j);

    return -42;
}

TEST(mockf, ioctl) {
    MOCKF_INIT(ioctl);

    // simply use EXPECT_CALL with MOCKF
    MOCKF_EXPECT_CALL(ioctl, (_, _, _)).WillOnce(ioctl());

    {
        MOCKF_GUARD(ioctl);                 // enable call to mock
        EXPECT_EQ(ioctl(42, 0, 1, 2), -42); // use mock
    }                                       // disable call to mock
}
