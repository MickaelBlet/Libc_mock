/**
 * mockf.h
 *
 * Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 * Copyright (c) 2021-2024 BLET Mickaël.
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

#ifndef BLET_MOCKF_H_
#define BLET_MOCKF_H_

#include <dlfcn.h> // dlsym
#include <gmock/gmock.h>
#include <stdarg.h> // va_list, va_start, va_end

/**
 * @brief Mockf class type from name with mockf namepsace
 * @param name Name of function
 */
#define MOCKF_CLASS(name) blet::mockf::MockF_##name

/**
 * @brief Declare a mockf class from name (name: mockf_{NAME})
 * @param name Name of function
 */
#define MOCKF_INIT(name) MOCKF_CLASS(name) mockf_##name

/**
 * @brief Get current instance of mockf from name
 * @param name Name of function
 * @throw blet::mockf::InstanceNotFound if instance of mockf is NULL
 */
#define MOCKF_INSTANCE(name)                                                                             \
    ((MOCKF_CLASS(name)::instance())                                                                     \
         ? *(MOCKF_CLASS(name)::instance())                                                              \
         : (throw ::blet::mockf::InstanceNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #name), \
            *(MOCKF_CLASS(name)::instance())))

/**
 * @brief Enable mock on scope from name
 * @param name Name of function
 * @throw blet::mockf::InstanceNotFound if instance of mockf is NULL
 */
#define MOCKF_GUARD(name) ::blet::mockf::Guard mockf_guard_##name(MOCKF_INSTANCE(name).isEnable)
/**
 * @brief Disable mock on scope from name
 * @param name Name of function
 * @throw blet::mockf::InstanceNotFound if instance of mockf is NULL
 */
#define MOCKF_GUARD_REVERSE(name) ::blet::mockf::GuardReverse mockf_guard_reverse_##name(MOCKF_INSTANCE(name).isEnable)
/**
 * @brief Enable mock from name
 * @param name Name of function
 * @throw blet::mockf::InstanceNotFound if instance of mockf is NULL
 */
#define MOCKF_ENABLE(name) MOCKF_INSTANCE(name).isEnable = true
/**
 * @brief Disable mock from name
 * @param name Name of function
 * @throw blet::mockf::InstanceNotFound if instance of mockf is NULL
 */
#define MOCKF_DISABLE(name) MOCKF_INSTANCE(name).isEnable = false

/**
 * @brief Except call of mock from name
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @throw blet::mockf::InstanceNotFound if instance of mockf is NULL
 */
#define MOCKF_EXPECT_CALL(name, arguments) EXPECT_CALL(MOCKF_INSTANCE(name), name arguments)

#ifndef MOCKF_DISABLE_VARIADIC_MACROS
#define MOCKF_INTERNAL_NARGS_SEQ_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a0, N, ...) N
#define MOCKF_INTERNAL_NARGS_(...) MOCKF_INTERNAL_NARGS_SEQ_(__VA_ARGS__, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define MOCKF_INTERNAL_PRE_NARGS_(...) MOCKF_INTERNAL_NARGS_(__VA_ARGS__)
#define MOCKF_INTERNAL_NOARGS_() 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
#define MOCKF_INTERNAL_NARG_(...) MOCKF_INTERNAL_PRE_NARGS_(MOCKF_INTERNAL_NOARGS_ __VA_ARGS__())

/**
 * @brief Mock a function
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION(return_type, name, arguments) \
    MOCKF_INTERNAL_(MOCKF_INTERNAL_NARG_ arguments, return_type, name, arguments)
/**
 * @brief Mock a function with a attribute
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(MOCKF_INTERNAL_NARG_ arguments, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION(return_type, name, arguments) \
    MOCKF_INTERNAL_VARIADIC_(MOCKF_INTERNAL_NARG_ arguments, return_type, name, arguments)
/**
 * @brief Mock a variadic function with a attribute
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(MOCKF_INTERNAL_NARG_ arguments, return_type, name, arguments, attribute)
#endif

/**
 * @brief Mock a function with 0 argument
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION0(return_type, name, arguments) MOCKF_INTERNAL_(0, return_type, name, arguments)
/**
 * @brief Mock a function with 1 argument
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION1(return_type, name, arguments) MOCKF_INTERNAL_(1, return_type, name, arguments)
/**
 * @brief Mock a function with 2 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION2(return_type, name, arguments) MOCKF_INTERNAL_(2, return_type, name, arguments)
/**
 * @brief Mock a function with 3 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION3(return_type, name, arguments) MOCKF_INTERNAL_(3, return_type, name, arguments)
/**
 * @brief Mock a function with 4 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION4(return_type, name, arguments) MOCKF_INTERNAL_(4, return_type, name, arguments)
/**
 * @brief Mock a function with 5 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION5(return_type, name, arguments) MOCKF_INTERNAL_(5, return_type, name, arguments)
/**
 * @brief Mock a function with 6 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION6(return_type, name, arguments) MOCKF_INTERNAL_(6, return_type, name, arguments)
/**
 * @brief Mock a function with 7 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION7(return_type, name, arguments) MOCKF_INTERNAL_(7, return_type, name, arguments)
/**
 * @brief Mock a function with 8 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION8(return_type, name, arguments) MOCKF_INTERNAL_(8, return_type, name, arguments)
/**
 * @brief Mock a function with 9 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION9(return_type, name, arguments) MOCKF_INTERNAL_(9, return_type, name, arguments)
/**
 * @brief Mock a function with 10 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_FUNCTION10(return_type, name, arguments) MOCKF_INTERNAL_(10, return_type, name, arguments)

/**
 * @brief Mock a function with a attribute with 0 argument
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION0(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(0, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 1 argument
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION1(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(1, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 2 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION2(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(2, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 3 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION3(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(3, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 4 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION4(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(4, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 5 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION5(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(5, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 6 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION6(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(6, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 7 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION7(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(7, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 8 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION8(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(8, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 9 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION9(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(9, return_type, name, arguments, attribute)
/**
 * @brief Mock a function with a attribute with 10 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_ATTRIBUTE_FUNCTION10(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_(10, return_type, name, arguments, attribute)

/**
 * @brief Mock a variadic function with 2 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION2(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(2, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 3 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION3(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(3, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 4 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION4(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(4, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 5 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION5(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(5, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 6 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION6(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(6, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 7 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION7(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(7, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 8 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION8(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(8, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 9 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION9(return_type, name, arguments) MOCKF_INTERNAL_VARIADIC_(9, return_type, name, arguments)
/**
 * @brief Mock a variadic function with 10 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 */
#define MOCKF_VARIADIC_FUNCTION10(return_type, name, arguments) \
    MOCKF_INTERNAL_VARIADIC_(10, return_type, name, arguments)

/**
 * @brief Mock a variadic function with a attribute with 2 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION2(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(2, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 3 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION3(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(3, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 4 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION4(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(4, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 5 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION5(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(5, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 6 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION6(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(6, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 7 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION7(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(7, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 8 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION8(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(8, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 9 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION9(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(9, return_type, name, arguments, attribute)
/**
 * @brief Mock a variadic function with a attribute with 10 arguments
 * @param return_type Return type of function
 * @param name Name of function
 * @param arguments List of arguments enclose by parenthesis
 * @param attribute Attribute of function
 */
#define MOCKF_VARIADIC_ATTRIBUTE_FUNCTION10(return_type, name, arguments, attribute) \
    MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(10, return_type, name, arguments, attribute)

struct MockFForceSemiColon {};

namespace blet {

namespace mockf {

class Exception : public std::exception {
  public:
    Exception(const char* file, const char* line, const char* name) throw() :
        std::exception() {
        message_ = file;
        message_ += ":";
        message_ += line;
        message_ += ":MockF '";
        message_ += name;
        message_ += "' ";
    }
    virtual ~Exception() throw() {}
    const char* what() const throw() {
        return message_.c_str();
    }

  protected:
    std::string message_;
};

struct InstanceNotFound : public Exception {
    InstanceNotFound(const char* file, const char* line, const char* name) throw() :
        Exception(file, line, name) {
        message_ += "instance not found.";
    }
};

struct RealFunctionNotFound : public Exception {
    RealFunctionNotFound(const char* file, const char* line, const char* name) throw() :
        Exception(file, line, name) {
        message_ += "real function not found.";
    }
};

struct RealVariadicFunctionUsed : public Exception {
    RealVariadicFunctionUsed(const char* file, const char* line, const char* name) throw() :
        Exception(file, line, name) {
        message_ += "real variadic function cannot use directly.";
    }
};

template<bool AtConstructor, bool AtDestructor>
struct GuardT {
    GuardT(bool& boolean) :
        boolean_(boolean) {
        boolean_ = AtConstructor;
    }
    ~GuardT() {
        boolean_ = AtDestructor;
    }
    bool& boolean_;
};
typedef GuardT<true, false> Guard;
typedef GuardT<false, true> GuardReverse;

template<typename T>
struct MockF {
    MockF() :
        isEnable(false),
        isMaster(false) {
        if (instance() == NULL) {
            instance() = reinterpret_cast<T*>(this);
            isMaster = true;
        }
    }
    virtual ~MockF() {
        if (isMaster) {
            instance() = NULL;
        }
    }
    static T*& instance() {
        static T* singleton = NULL;
        return singleton;
    }
    bool isEnable;
    bool isMaster;
};

} // namespace mockf

} // namespace blet

#define MOCKF_INTERNAL_STRINGIFY_(x) #x
#define MOCKF_INTERNAL_TO_STRING_(x) MOCKF_INTERNAL_STRINGIFY_(x)

#define MOCKF_INTERNAL_CAT_IMPL_(x, y) x##y
#define MOCKF_INTERNAL_CAT_(x, y) MOCKF_INTERNAL_CAT_IMPL_(x, y)
#define MOCKF_INTERNAL_CAT2_(x, y, z) MOCKF_INTERNAL_CAT_(MOCKF_INTERNAL_CAT_(x, y), z)

#define MOCKF_INTERNAL_SUB_1_ 0
#define MOCKF_INTERNAL_SUB_2_ 1
#define MOCKF_INTERNAL_SUB_3_ 2
#define MOCKF_INTERNAL_SUB_4_ 3
#define MOCKF_INTERNAL_SUB_5_ 4
#define MOCKF_INTERNAL_SUB_6_ 5
#define MOCKF_INTERNAL_SUB_7_ 6
#define MOCKF_INTERNAL_SUB_8_ 7
#define MOCKF_INTERNAL_SUB_9_ 8
#define MOCKF_INTERNAL_SUB_10_ 9
#define MOCKF_INTERNAL_SUB_(x) MOCKF_INTERNAL_CAT2_(MOCKF_INTERNAL_SUB_, x, _)

#define MOCKF_INTERNAL_REMOVE_LAST_ARG_1_(a1) ()
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_2_(a1, a2) (a1)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_3_(a1, a2, a3) (a1, a2)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_4_(a1, a2, a3, a4) (a1, a2, a3)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_5_(a1, a2, a3, a4, a5) (a1, a2, a3, a4)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_6_(a1, a2, a3, a4, a5, a6) (a1, a2, a3, a4, a5)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_7_(a1, a2, a3, a4, a5, a6, a7) (a1, a2, a3, a4, a5, a6)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_8_(a1, a2, a3, a4, a5, a6, a7, a8) (a1, a2, a3, a4, a5, a6, a7)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_9_(a1, a2, a3, a4, a5, a6, a7, a8, a9) (a1, a2, a3, a4, a5, a6, a7, a8)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_10_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) (a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define MOCKF_INTERNAL_REMOVE_LAST_ARG_(x) MOCKF_INTERNAL_CAT2_(MOCKF_INTERNAL_REMOVE_LAST_ARG_, x, _)

#define MOCKF_INTERNAL_(i, r, n, f)                                                      \
    namespace blet {                                                                     \
    namespace mockf {                                                                    \
    struct MockF_##n : public MockF<MockF_##n> {                                         \
        typedef r(*function_t) f;                                                        \
        static function_t real() {                                                       \
            static function_t func = reinterpret_cast<function_t>(dlsym(RTLD_NEXT, #n)); \
            return func;                                                                 \
        }                                                                                \
        MOCKF_INTERNAL_METHOD_(i, r, n, f);                                              \
    };                                                                                   \
    }                                                                                    \
    }                                                                                    \
    MOCKF_INTERNAL_FAKE_FUNC_(i, r, n, f) struct MockFForceSemiColon

#define MOCKF_INTERNAL_ATTRIBUTE_(i, r, n, f, a)                                         \
    namespace blet {                                                                     \
    namespace mockf {                                                                    \
    struct MockF_##n : public MockF<MockF_##n> {                                         \
        typedef r(*function_t) f;                                                        \
        static function_t real() {                                                       \
            static function_t func = reinterpret_cast<function_t>(dlsym(RTLD_NEXT, #n)); \
            return func;                                                                 \
        }                                                                                \
        MOCKF_INTERNAL_METHOD_(i, r, n, f);                                              \
    };                                                                                   \
    }                                                                                    \
    }                                                                                    \
    MOCKF_INTERNAL_FAKE_ATTRIBUTE_FUNC_(i, r, n, f, a) struct MockFForceSemiColon

#define MOCKF_INTERNAL_VARIADIC_(i, r, n, f)                                             \
    namespace blet {                                                                     \
    namespace mockf {                                                                    \
    struct MockF_##n : public MockF<MockF_##n> {                                         \
        typedef r(*function_t) f;                                                        \
        static function_t real() {                                                       \
            static function_t func = reinterpret_cast<function_t>(dlsym(RTLD_NEXT, #n)); \
            return func;                                                                 \
        }                                                                                \
        MOCKF_INTERNAL_VARIADIC_METHOD_(i, r, n, f);                                     \
    };                                                                                   \
    }                                                                                    \
    }                                                                                    \
    MOCKF_INTERNAL_FAKE_VARIADIC_FUNC_(i, r, n, f) struct MockFForceSemiColon

#define MOCKF_INTERNAL_ATTRIBUTE_VARIADIC_(i, r, n, f, a)                                \
    namespace blet {                                                                     \
    namespace mockf {                                                                    \
    struct MockF_##n : public MockF<MockF_##n> {                                         \
        typedef r(*function_t) f;                                                        \
        static function_t real() {                                                       \
            static function_t func = reinterpret_cast<function_t>(dlsym(RTLD_NEXT, #n)); \
            return func;                                                                 \
        }                                                                                \
        MOCKF_INTERNAL_VARIADIC_METHOD_(i, r, n, f);                                     \
    };                                                                                   \
    }                                                                                    \
    }                                                                                    \
    MOCKF_INTERNAL_FAKE_ATTRIBUTE_VARIADIC_FUNC_(i, r, n, f, a) struct MockFForceSemiColon

// gtest > 1.8.1
#ifdef MOCK_METHOD

#define MOCKF_INTERNAL_METHOD_(i, r, n, f) MOCK_METHOD(r, n, f, ())

#define MOCKF_INTERNAL_FAKE_FUNC_(i, r, n, f)                                                             \
    r n(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, (GMOCK_INTERNAL_SIGNATURE(r, f)), i)) {                 \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                   \
            MOCKF_GUARD_REVERSE(n);                                                                       \
            return MOCKF_INSTANCE(n).n(                                                                   \
                GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, (GMOCK_INTERNAL_SIGNATURE(r, f)), i));        \
        }                                                                                                 \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                             \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
        }                                                                                                 \
        return MOCKF_CLASS(n)::real()(                                                                    \
            GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, (GMOCK_INTERNAL_SIGNATURE(r, f)), i));            \
    }

#define MOCKF_INTERNAL_FAKE_ATTRIBUTE_FUNC_(i, r, n, f, a)                                                \
    r n(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, (GMOCK_INTERNAL_SIGNATURE(r, f)), i)) a {               \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                   \
            MOCKF_GUARD_REVERSE(n);                                                                       \
            return MOCKF_INSTANCE(n).n(                                                                   \
                GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, (GMOCK_INTERNAL_SIGNATURE(r, f)), i));        \
        }                                                                                                 \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                             \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
        }                                                                                                 \
        return MOCKF_CLASS(n)::real()(                                                                    \
            GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG, (GMOCK_INTERNAL_SIGNATURE(r, f)), i));            \
    }

#define MOCKF_INTERNAL_VARIADIC_METHOD_(i, r, n, f)                                                                    \
    MOCK_METHOD(                                                                                                       \
        r, n,                                                                                                          \
        (GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER,                                                                     \
                         (GMOCK_INTERNAL_SIGNATURE(r, MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f)), MOCKF_INTERNAL_SUB_(i)), \
         va_list),                                                                                                     \
        ())

#define MOCKF_INTERNAL_FAKE_VARIADIC_FUNC_(i, r, n, f)                                                                 \
    r n(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, (GMOCK_INTERNAL_SIGNATURE(r, MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f)), \
                        MOCKF_INTERNAL_SUB_(i)),                                                                       \
        ...) {                                                                                                         \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                                \
            MOCKF_GUARD_REVERSE(n);                                                                                    \
            va_list args;                                                                                              \
            va_start(args, MOCKF_INTERNAL_CAT_(gmock_a, MOCKF_INTERNAL_SUB_(MOCKF_INTERNAL_SUB_(i))));                 \
            r ret = MOCKF_INSTANCE(n).n(                                                                               \
                GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG,                                                            \
                                (GMOCK_INTERNAL_SIGNATURE(r, MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f)),                   \
                                MOCKF_INTERNAL_SUB_(i)),                                                               \
                args);                                                                                                 \
            va_end(args);                                                                                              \
            return ret;                                                                                                \
        }                                                                                                              \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                                          \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n);              \
        }                                                                                                              \
        throw ::blet::mockf::RealVariadicFunctionUsed(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n);              \
    }

#define MOCKF_INTERNAL_FAKE_ATTRIBUTE_VARIADIC_FUNC_(i, r, n, f, a)                                                    \
    r n(GMOCK_PP_REPEAT(GMOCK_INTERNAL_PARAMETER, (GMOCK_INTERNAL_SIGNATURE(r, MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f)), \
                        MOCKF_INTERNAL_SUB_(i)),                                                                       \
        ...) a {                                                                                                       \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                                \
            MOCKF_GUARD_REVERSE(n);                                                                                    \
            va_list args;                                                                                              \
            va_start(args, MOCKF_INTERNAL_CAT_(gmock_a, MOCKF_INTERNAL_SUB_(MOCKF_INTERNAL_SUB_(i))));                 \
            r ret = MOCKF_INSTANCE(n).n(                                                                               \
                GMOCK_PP_REPEAT(GMOCK_INTERNAL_FORWARD_ARG,                                                            \
                                (GMOCK_INTERNAL_SIGNATURE(r, MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f)),                   \
                                MOCKF_INTERNAL_SUB_(i)),                                                               \
                args);                                                                                                 \
            va_end(args);                                                                                              \
            return ret;                                                                                                \
        }                                                                                                              \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                                          \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n);              \
        }                                                                                                              \
        throw ::blet::mockf::RealVariadicFunctionUsed(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n);              \
    }

#else

#define MOCKF_INTERNAL_REPEAT_0_(m, f) /* nothing */
#define MOCKF_INTERNAL_REPEAT_1_(m, f) m(1, f)
#define MOCKF_INTERNAL_REPEAT_2_(m, f) MOCKF_INTERNAL_REPEAT_1_(m, f), m(2, f)
#define MOCKF_INTERNAL_REPEAT_3_(m, f) MOCKF_INTERNAL_REPEAT_2_(m, f), m(3, f)
#define MOCKF_INTERNAL_REPEAT_4_(m, f) MOCKF_INTERNAL_REPEAT_3_(m, f), m(4, f)
#define MOCKF_INTERNAL_REPEAT_5_(m, f) MOCKF_INTERNAL_REPEAT_4_(m, f), m(5, f)
#define MOCKF_INTERNAL_REPEAT_6_(m, f) MOCKF_INTERNAL_REPEAT_5_(m, f), m(6, f)
#define MOCKF_INTERNAL_REPEAT_7_(m, f) MOCKF_INTERNAL_REPEAT_6_(m, f), m(7, f)
#define MOCKF_INTERNAL_REPEAT_8_(m, f) MOCKF_INTERNAL_REPEAT_7_(m, f), m(8, f)
#define MOCKF_INTERNAL_REPEAT_9_(m, f) MOCKF_INTERNAL_REPEAT_8_(m, f), m(9, f)
#define MOCKF_INTERNAL_REPEAT_10_(m, f) MOCKF_INTERNAL_REPEAT_9_(m, f), m(10, f)
#define MOCKF_INTERNAL_REPEAT_(i) MOCKF_INTERNAL_CAT2_(MOCKF_INTERNAL_REPEAT_, i, _)

#define MOCKF_INTERNAL_ARG_(i, f) MOCKF_INTERNAL_CAT_(gmock_a, MOCKF_INTERNAL_SUB_(i))
#define MOCKF_INTERNAL_ARG_DECLARATION_(i, f) GMOCK_ARG_(, i, f) MOCKF_INTERNAL_ARG_(i, f)

#define MOCKF_INTERNAL_METHOD_(i, r, n, f) MOCKF_INTERNAL_CAT_(MOCK_METHOD, i)(n, r f)

#define MOCKF_INTERNAL_FAKE_FUNC_(i, r, n, f)                                                             \
    r n(MOCKF_INTERNAL_REPEAT_(i)(MOCKF_INTERNAL_ARG_DECLARATION_, r f)) {                                \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                   \
            MOCKF_GUARD_REVERSE(n);                                                                       \
            return MOCKF_INSTANCE(n).n(MOCKF_INTERNAL_REPEAT_(i)(MOCKF_INTERNAL_ARG_, f));                \
        }                                                                                                 \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                             \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
        }                                                                                                 \
        return MOCKF_CLASS(n)::real()(MOCKF_INTERNAL_REPEAT_(i)(MOCKF_INTERNAL_ARG_, f));                 \
    }

#define MOCKF_INTERNAL_FAKE_ATTRIBUTE_FUNC_(i, r, n, f, a)                                                \
    r n(MOCKF_INTERNAL_REPEAT_(i)(MOCKF_INTERNAL_ARG_DECLARATION_, r f)) a {                              \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                   \
            MOCKF_GUARD_REVERSE(n);                                                                       \
            return MOCKF_INSTANCE(n).n(MOCKF_INTERNAL_REPEAT_(i)(MOCKF_INTERNAL_ARG_, f));                \
        }                                                                                                 \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                             \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
        }                                                                                                 \
        return MOCKF_CLASS(n)::real()(MOCKF_INTERNAL_REPEAT_(i)(MOCKF_INTERNAL_ARG_, f));                 \
    }

#define MOCKF_INTERNAL_VARIADIC_METHOD_(i, r, n, f)                                               \
    MOCKF_INTERNAL_CAT_(MOCK_METHOD, i)                                                           \
    (n, r(MOCKF_INTERNAL_REPEAT_(MOCKF_INTERNAL_SUB_(i))(MOCKF_INTERNAL_ARG_DECLARATION_,         \
                                                         r MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f), \
          va_list))

#define MOCKF_INTERNAL_FAKE_VARIADIC_FUNC_(i, r, n, f, a)                                                 \
    r n(MOCKF_INTERNAL_REPEAT_(MOCKF_INTERNAL_SUB_(i))(MOCKF_INTERNAL_ARG_DECLARATION_,                   \
                                                       r MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f),           \
        ...) {                                                                                            \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                   \
            MOCKF_GUARD_REVERSE(n);                                                                       \
            va_list args;                                                                                 \
            va_start(args, MOCKF_INTERNAL_CAT_(gmock_a, MOCKF_INTERNAL_SUB_(MOCKF_INTERNAL_SUB_(i))));    \
            r ret = MOCKF_INSTANCE(n).n(MOCKF_INTERNAL_REPEAT_(MOCKF_INTERNAL_SUB_(i))(                   \
                                            MOCKF_INTERNAL_ARG_, MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f),   \
                                        args);                                                            \
            va_end(args);                                                                                 \
            return ret;                                                                                   \
        }                                                                                                 \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                             \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
        }                                                                                                 \
        throw ::blet::mockf::RealVariadicFunctionUsed(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
    }

#define MOCKF_INTERNAL_FAKE_ATTRIBUTE_VARIADIC_FUNC_(i, r, n, f, a)                                       \
    r n(MOCKF_INTERNAL_REPEAT_(MOCKF_INTERNAL_SUB_(i))(MOCKF_INTERNAL_ARG_DECLARATION_,                   \
                                                       r MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f),           \
        ...) a {                                                                                          \
        if (MOCKF_CLASS(n)::instance() && MOCKF_INSTANCE(n).isEnable) {                                   \
            MOCKF_GUARD_REVERSE(n);                                                                       \
            va_list args;                                                                                 \
            va_start(args, MOCKF_INTERNAL_CAT_(gmock_a, MOCKF_INTERNAL_SUB_(MOCKF_INTERNAL_SUB_(i))));    \
            r ret = MOCKF_INSTANCE(n).n(MOCKF_INTERNAL_REPEAT_(MOCKF_INTERNAL_SUB_(i))(                   \
                                            MOCKF_INTERNAL_ARG_, MOCKF_INTERNAL_REMOVE_LAST_ARG_(i) f),   \
                                        args);                                                            \
            va_end(args);                                                                                 \
            return ret;                                                                                   \
        }                                                                                                 \
        if (MOCKF_CLASS(n)::real() == NULL) {                                                             \
            throw ::blet::mockf::RealFunctionNotFound(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
        }                                                                                                 \
        throw ::blet::mockf::RealVariadicFunctionUsed(__FILE__, MOCKF_INTERNAL_TO_STRING_(__LINE__), #n); \
    }

#endif

#endif // #ifndef BLET_MOCKF_H_
