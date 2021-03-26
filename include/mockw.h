/**
* MockW
*
* Licensed under the MIT License <http://opensource.org/licenses/MIT>.
* Copyright (c) 2021 BLET Mickaël.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef _MOCKW_H_
#define _MOCKW_H_

#ifdef MOCKW_DLFCN
    #include <dlfcn.h>
#endif

#include <gmock/gmock.h>

#define MOCKW_CAT_IMPL_(x, y) x ## y
#define MOCKW_CAT_(x, y) MOCKW_CAT_IMPL_(x, y)
#define MOCKW_CAT2_(x, y, z) MOCKW_CAT_(MOCKW_CAT_(x, y), z)
#define MOCKW_STRINGIFY_(x) #x
#define MOCKW_TO_STRING_(x) MOCKW_STRINGIFY_(x)

#define MOCKW_REPEAT_0_(m, f) /* nothing */
#define MOCKW_REPEAT_1_(m, f) M(1, f)
#define MOCKW_REPEAT_2_(m, f) MOCKW_REPEAT_1_(m, f), m(2, f)
#define MOCKW_REPEAT_3_(m, f) MOCKW_REPEAT_2_(m, f), m(3, f)
#define MOCKW_REPEAT_4_(m, f) MOCKW_REPEAT_3_(m, f), m(4, f)
#define MOCKW_REPEAT_5_(m, f) MOCKW_REPEAT_4_(m, f), m(5, f)
#define MOCKW_REPEAT_6_(m, f) MOCKW_REPEAT_5_(m, f), m(6, f)
#define MOCKW_REPEAT_7_(m, f) MOCKW_REPEAT_6_(m, f), m(7, f)
#define MOCKW_REPEAT_8_(m, f) MOCKW_REPEAT_7_(m, f), m(8, f)
#define MOCKW_REPEAT_9_(m, f) MOCKW_REPEAT_8_(m, f), m(9, f)
#define MOCKW_REPEAT_10_(m, f) MOCKW_REPEAT_9_(m, f), m(10, f)

#define MOCKW_ARG_(i, f) MOCKW_CAT_(var_, i)
#define MOCKW_ARG_DECLARATION_(i, f) GMOCK_ARG_(, i, f) MOCKW_ARG_(i, f)

#ifdef MOCKW_DLFCN
/* template guard */
template<typename T>
struct MockWeakGuard {
    MockWeakGuard(T& mockWeak): refMockWeak(mockWeak) {
        refMockWeak.isActive = true;
    }
    ~MockWeakGuard() {
        refMockWeak.isActive = false;
    }
    T& refMockWeak;
};
template<typename T>
struct MockWeakGuardReverse {
    MockWeakGuardReverse(T& mock_weak): refMockWeak(mock_weak) {
        refMockWeak.isActive = false;
    }
    ~MockWeakGuardReverse() {
        refMockWeak.isActive = true;
    }
    T& refMockWeak;
};
#define MOCKW_GUARD(n) MockWeakGuard<MOCKW_CAT_(MockWeak_, n)> MOCKW_CAT_(mock_weak_guard_, n)(MOCKW_CAT_(MockWeak_, n)::instance());
#define MOCKW_GUARD_REVERSE(n) MockWeakGuardReverse<MOCKW_CAT_(MockWeak_, n)> MOCKW_CAT_(mock_weak_guard_reverse_, n)(MOCKW_CAT_(MockWeak_, n)::instance());

#define MOCKW_(i, n, f) \
\
struct MOCKW_CAT_(MockWeak_, n) { \
    typedef GMOCK_RESULT_(,f) (*func_t)(MOCKW_CAT2_(MOCKW_REPEAT_, i, _)(MOCKW_ARG_DECLARATION_, f)); \
    MOCKW_CAT_(MockWeak_, n)(): \
        isActive(false), \
        real((func_t)dlsym(RTLD_NEXT, #n)) { \
        return ; \
    } \
    MOCKW_CAT_(MOCK_METHOD, i)(n, f); \
    static MOCKW_CAT_(MockWeak_, n) &instance() { \
        static MOCKW_CAT_(MockWeak_, n) singleton; \
        return singleton; \
    } \
    bool isActive; \
    func_t real; \
}; \
\
struct MOCKW_CAT_(Exception_, n): public std::exception { \
    MOCKW_CAT_(Exception_, n)(const char* str) throw() : \
        str(str) { \
        return ; \
    } \
    virtual ~MOCKW_CAT_(Exception_, n)() throw() { \
        return ; \
    } \
    virtual const char* what() const throw() { \
        return str.c_str(); \
    } \
    const std::string str; \
}; \
\
GMOCK_RESULT_(, f) n(MOCKW_CAT2_(MOCKW_REPEAT_, i, _)(MOCKW_ARG_DECLARATION_, f)) { \
    if (MOCKW_CAT_(MockWeak_, n)::instance().isActive) { \
        MOCKW_GUARD_REVERSE(n); \
        return MOCKW_CAT_(MockWeak_, n)::instance().n(MOCKW_CAT2_(MOCKW_REPEAT_, i, _)(MOCKW_ARG_, f)); \
    } \
    if (MOCKW_CAT_(MockWeak_, n)::instance().real == NULL) { \
        throw MOCKW_CAT_(Exception_, n)(__FILE__ ":" MOCKW_TO_STRING_(__LINE__) ": real function \"" #n "\" not found"); \
    } \
    return MOCKW_CAT_(MockWeak_, n)::instance().real(MOCKW_CAT2_(MOCKW_REPEAT_, i, _)(MOCKW_ARG_, f)); \
}
#define MOCKW_ENABLE(n) MOCKW_CAT_(MockWeak_, n)::instance().isActive = true;
#define MOCKW_DISABLE(n) MOCKW_CAT_(MockWeak_, n)::instance().isActive = false;

#else // ! MOCKW_DLFCN

#define MOCKW_(i, n, f) \
struct MOCKW_CAT_(MockWeak_, n) { \
    MOCKW_CAT_(MOCK_METHOD, i)(n, f); \
    static MOCKW_CAT_(MockWeak_, n) &instance() { \
        static MOCKW_CAT_(MockWeak_, n) singleton; \
        return singleton; \
    } \
}; \
GMOCK_RESULT_(, f) n(MOCKW_CAT2_(MOCKW_REPEAT_, i, _)(MOCKW_ARG_DECLARATION_, f)) { \
    return MOCKW_CAT_(MockWeak_, n)::instance().n(MOCKW_CAT2_(MOCKW_REPEAT_, i, _)(MOCKW_ARG_, f)); \
}

#endif // MOCKW_DLFCN

#define MOCKW_INSTANCE(n) MOCKW_CAT_(MockWeak_, n)::instance()

#define MOCKW_METHOD0(n, f) MOCKW_(0, n, f)
#define MOCKW_METHOD1(n, f) MOCKW_(1, n, f)
#define MOCKW_METHOD2(n, f) MOCKW_(2, n, f)
#define MOCKW_METHOD3(n, f) MOCKW_(3, n, f)
#define MOCKW_METHOD4(n, f) MOCKW_(4, n, f)
#define MOCKW_METHOD5(n, f) MOCKW_(5, n, f)
#define MOCKW_METHOD6(n, f) MOCKW_(6, n, f)
#define MOCKW_METHOD7(n, f) MOCKW_(7, n, f)
#define MOCKW_METHOD8(n, f) MOCKW_(8, n, f)
#define MOCKW_METHOD9(n, f) MOCKW_(9, n, f)
#define MOCKW_METHOD10(n, f) MOCKW_(10, n, f)

#define MOCKW_EXPECT_CALL(n, p) EXPECT_CALL(MOCKW_INSTANCE(n), n p)

#ifdef MOCKW_DLFCN
#define MOCKW_DECLTYPE_METHOD0(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD0(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD1(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD1(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD2(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD2(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD3(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD3(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD4(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD4(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD5(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD5(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD6(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD6(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD7(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD7(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD8(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD8(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD9(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD9(n, MOCKW_CAT_(decltype_, n))
#define MOCKW_DECLTYPE_METHOD10(n) using MOCKW_CAT_(decltype_, n) = decltype(n); MOCKW_METHOD10(n, MOCKW_CAT_(decltype_, n))
#endif // MOCKW_DLFCN

#endif // _MOCKW_H_
