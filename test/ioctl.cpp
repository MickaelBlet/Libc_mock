#include <stdarg.h>
#include <sys/ioctl.h>

#include "mockc.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

MOCKC_METHOD3(int, variadic_ioctl, (int, unsigned long, va_list));

int ioctl(int fd, unsigned long request, ...) {
    int ret;
    va_list args;
    va_start(args, request);
    ret = variadic_ioctl(fd, request, args);
    va_end(args);
    return ret;
}

ACTION(ioctl) {
    int i = (int)va_arg(arg2, int);
    printf("%i, %lu, %i", arg0, arg1, i);
    return -42;
}

GTEST_TEST(mockc, ioctl) {
    MOCKC_NEW_INSTANCE(variadic_ioctl);

    // simply use EXPECT_CALL with MOCKC
    MOCKC_EXPECT_CALL(variadic_ioctl, (_, _, _)).WillRepeatedly(ioctl());

    {
        MOCKC_GUARD(variadic_ioctl);       // enable call to mock
        EXPECT_EQ(ioctl(42, 42, 42), -42); // use mock
    }                                      // disable call to mock
}