#ifndef _MOCK_LIBC_H_
# define _MOCK_LIBC_H_

# include <gmock/gmock.h>

# ifndef __GNUC__
#  ifndef __attribute__
#   define __attribute__(X) /* do nothing */
#  endif
# endif

# define MOCK_LIBC_PRIMITIVE_CAT_(x, y) x ## y
# define MOCK_LIBC_CAT_(x, y) MOCK_LIBC_PRIMITIVE_CAT_(x, y)

# define MOCK_LIBC_REAPEAT_(N,...) MOCK_LIBC_CAT_(MOCK_LIBC_CAT_(MOCK_LIBC_REAPEAT_,N),_)(__VA_ARGS__)
# define MOCK_LIBC_REAPEAT_0_(M,...) 
# define MOCK_LIBC_REAPEAT_1_(M,...) M(1,__VA_ARGS__)
# define MOCK_LIBC_REAPEAT_2_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__)
# define MOCK_LIBC_REAPEAT_3_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__)
# define MOCK_LIBC_REAPEAT_4_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__), M(4, __VA_ARGS__)
# define MOCK_LIBC_REAPEAT_5_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__), M(4, __VA_ARGS__), M(5, __VA_ARGS__)
# define MOCK_LIBC_REAPEAT_6_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__), M(4, __VA_ARGS__), M(5, __VA_ARGS__), M(6, __VA_ARGS__)
# define MOCK_LIBC_REAPEAT_7_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__), M(4, __VA_ARGS__), M(5, __VA_ARGS__), M(6, __VA_ARGS__), M(7, __VA_ARGS__)
# define MOCK_LIBC_REAPEAT_8_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__), M(4, __VA_ARGS__), M(5, __VA_ARGS__), M(6, __VA_ARGS__), M(7, __VA_ARGS__), M(8, __VA_ARGS__)
# define MOCK_LIBC_REAPEAT_9_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__), M(4, __VA_ARGS__), M(5, __VA_ARGS__), M(6, __VA_ARGS__), M(7, __VA_ARGS__), M(8, __VA_ARGS__), M(9, __VA_ARGS__)
# define MOCK_LIBC_REAPEAT_10_(M,...) M(1,__VA_ARGS__), M(2,__VA_ARGS__), M(3,__VA_ARGS__), M(4, __VA_ARGS__), M(5, __VA_ARGS__), M(6, __VA_ARGS__), M(7, __VA_ARGS__), M(8, __VA_ARGS__), M(9, __VA_ARGS__), M(10, __VA_ARGS__)

# define MOCK_LIBC_RESULT_(...) ::testing::internal::Function<__VA_ARGS__>::Result
# define MOCK_LIBC_ARG_(N, ...) ::testing::internal::Function<__VA_ARGS__>::Argument##N

# define MOCK_LIBC_ARG_NAME_(N,...) _##N
# define MOCK_LIBC_ARG_DECLARATION_(N,...) MOCK_LIBC_ARG_(N, __VA_ARGS__) MOCK_LIBC_ARG_NAME_(N)

# define MOCK_LIBC_METHODE_(_lvl, _name, ...) \

# define MOCK_LIBC_WEAK_(_lvl, _name, ...) \
struct mock_libc_##_name \
{ \
    MOCK_METHOD##_lvl(_name, __VA_ARGS__); \
    static mock_libc_##_name &instance() \
    { \
        static mock_libc_##_name singleton; \
        return singleton; \
    } \
}; \
MOCK_LIBC_RESULT_(__VA_ARGS__) __attribute__((weak)) _name( \
    MOCK_LIBC_REAPEAT_(_lvl, MOCK_LIBC_ARG_DECLARATION_, __VA_ARGS__) ) { \
    return mock_libc_##_name::instance()._name( \
        MOCK_LIBC_REAPEAT_(_lvl, MOCK_LIBC_ARG_NAME_, __VA_ARGS__) ); \
}

# define MOCK_LIBC_INSTANCE(_name) mock_libc_##_name::instance()

# define MOCK_LIBC_METHOD0(_name, ...) MOCK_LIBC_WEAK_(0, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD1(_name, ...) MOCK_LIBC_WEAK_(1, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD2(_name, ...) MOCK_LIBC_WEAK_(2, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD3(_name, ...) MOCK_LIBC_WEAK_(3, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD4(_name, ...) MOCK_LIBC_WEAK_(4, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD5(_name, ...) MOCK_LIBC_WEAK_(5, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD6(_name, ...) MOCK_LIBC_WEAK_(6, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD7(_name, ...) MOCK_LIBC_WEAK_(7, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD8(_name, ...) MOCK_LIBC_WEAK_(8, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD9(_name, ...) MOCK_LIBC_WEAK_(9, _name, __VA_ARGS__)
# define MOCK_LIBC_METHOD10(_name, ...) MOCK_LIBC_WEAK_(10, _name, __VA_ARGS__)

#endif // _MOCK_LIBC_H_