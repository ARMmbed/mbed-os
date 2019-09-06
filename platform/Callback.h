/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_CALLBACK_H
#define MBED_CALLBACK_H

#include <string.h>
#include <stdint.h>
#include <new>
#include "platform/mbed_assert.h"
#include "platform/mbed_toolchain.h"

namespace mbed {
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_Callback Callback class
 * @{
 */

/** Callback class based on template specialization
 *
 * @note Synchronization level: Not protected
 */
template <typename F>
class Callback;

// Internal sfinae declarations
//
// These are used to eliminate overloads based on type attributes
// 1. Does a function object have a call operator
// 2. Does a function object fit in the available storage
//
// These eliminations are handled cleanly by the compiler and avoid
// massive and misleading error messages when confronted with an
// invalid type (or worse, runtime failures)
namespace detail {
struct nil {};

template <bool B, typename R = nil>
struct enable_if {
    typedef R type;
};

template <typename R>
struct enable_if<false, R> {};

template <typename M, M>
struct is_type {
    static const bool value = true;
};
}

#define MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, M)                            \
    typename detail::enable_if<                                             \
            detail::is_type<M, &F::operator()>::value &&                    \
            sizeof(F) <= sizeof(uintptr_t)                                  \
        >::type = detail::nil()

/** Callback class based on template specialization
 *
 * @note Synchronization level: Not protected
 */
template <typename R, typename... ArgTs>
class Callback<R(ArgTs...)> {
public:
    /** Create a Callback with a static function
     *  @param func     Static function to attach
     */
    Callback(R(*func)(ArgTs...) = 0)
    {
        if (!func) {
            memset(this, 0, sizeof(Callback));
        } else {
            generate(func);
        }
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     */
    Callback(const Callback<R(ArgTs...)> &func)
    {
        memset(this, 0, sizeof(Callback));
        if (func._ops) {
            func._ops->move(this, &func);
        }
        _ops = func._ops;
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(U *obj, R(T::*method)(ArgTs...))
    {
        generate(method_context<T, R(T::*)(ArgTs...)>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const U *obj, R(T::*method)(ArgTs...) const)
    {
        generate(method_context<const T, R(T::*)(ArgTs...) const>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(volatile U *obj, R(T::*method)(ArgTs...) volatile)
    {
        generate(method_context<volatile T, R(T::*)(ArgTs...) volatile>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const volatile U *obj, R(T::*method)(ArgTs...) const volatile)
    {
        generate(method_context<const volatile T, R(T::*)(ArgTs...) const volatile>(obj, method));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T, typename U>
    Callback(R(*func)(T *, ArgTs...), U *arg)
    {
        generate(function_context<R(*)(T *, ArgTs...), T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T, typename U>
    Callback(R(*func)(const T *, ArgTs...), const U *arg)
    {
        generate(function_context<R(*)(const T *, ArgTs...), const T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T, typename U>
    Callback(R(*func)(volatile T *, ArgTs...), volatile U *arg)
    {
        generate(function_context<R(*)(volatile T *, ArgTs...), volatile T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     */
    template<typename T, typename U>
    Callback(R(*func)(const volatile T *, ArgTs...), const volatile U *arg)
    {
        generate(function_context<R(*)(const volatile T *, ArgTs...), const volatile T>(func, arg));
    }

    /** Create a Callback with a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...)))
    {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...) const))
    {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(volatile F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...) volatile))
    {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const volatile F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...) const volatile))
    {
        generate(f);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to Callback(func, arg)
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to Callback(func, arg)")
    Callback(U *obj, R(*func)(T *, ArgTs...))
    {
        new (this) Callback(func, obj);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to Callback(func, arg)
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to Callback(func, arg)")
    Callback(const U *obj, R(*func)(const T *, ArgTs...))
    {
        new (this) Callback(func, obj);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to Callback(func, arg)
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to Callback(func, arg)")
    Callback(volatile U *obj, R(*func)(volatile T *, ArgTs...))
    {
        new (this) Callback(func, obj);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to Callback(func, arg)
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to Callback(func, arg)")
    Callback(const volatile U *obj, R(*func)(const volatile T *, ArgTs...))
    {
        new (this) Callback(func, obj);
    }

    /** Destroy a callback
     */
    ~Callback()
    {
        if (_ops) {
            _ops->dtor(this);
        }
    }

    /** Attach a static function
     *  @param func     Static function to attach
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(R(*func)(ArgTs...))
    {
        this->~Callback();
        new (this) Callback(func);
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(const Callback<R(ArgTs...)> &func)
    {
        this->~Callback();
        new (this) Callback(func);
    }

    /** Attach a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(U *obj, R(T::*method)(ArgTs...))
    {
        this->~Callback();
        new (this) Callback(obj, method);
    }

    /** Attach a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(const U *obj, R(T::*method)(ArgTs...) const)
    {
        this->~Callback();
        new (this) Callback(obj, method);
    }

    /** Attach a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile U *obj, R(T::*method)(ArgTs...) volatile)
    {
        this->~Callback();
        new (this) Callback(obj, method);
    }

    /** Attach a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template<typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile U *obj, R(T::*method)(ArgTs...) const volatile)
    {
        this->~Callback();
        new (this) Callback(obj, method);
    }

    /** Attach a static function with a bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(R(*func)(T *, ArgTs...), U *arg)
    {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a static function with a bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(R(*func)(const T *, ArgTs...), const U *arg)
    {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a static function with a bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(R(*func)(volatile T *, ArgTs...), volatile U *arg)
    {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a static function with a bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(R(*func)(const volatile T *, ArgTs...), const volatile U *arg)
    {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...)))
    {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(const F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...) const))
    {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...) volatile))
    {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param f Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile F f, MBED_ENABLE_IF_CALLBACK_COMPATIBLE(F, R(F::*)(ArgTs...) const volatile))
    {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to attach(func, arg)
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to attach(func, arg)")
    void attach(U *obj, R(*func)(T *, ArgTs...))
    {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to attach(func, arg)
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to attach(func, arg)")
    void attach(const U *obj, R(*func)(const T *, ArgTs...))
    {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to attach(func, arg)
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to attach(func, arg)")
    void attach(volatile U *obj, R(*func)(volatile T *, ArgTs...))
    {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     *  @deprecated
     *      Arguments to callback have been reordered to attach(func, arg)
     */
    template <typename T, typename U>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Arguments to callback have been reordered to attach(func, arg)")
    void attach(const volatile U *obj, R(*func)(const volatile T *, ArgTs...))
    {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Assign a callback
     */
    Callback &operator=(const Callback &that)
    {
        if (this != &that) {
            this->~Callback();
            new (this) Callback(that);
        }

        return *this;
    }

    /** Call the attached function
     */
    R call(ArgTs... args) const
    {
        MBED_ASSERT(_ops);
        return _ops->call(this, args...);
    }

    /** Call the attached function
     */
    R operator()(ArgTs... args) const
    {
        return call(args...);
    }

    /** Test if function has been attached
     */
    operator bool() const
    {
        return _ops;
    }

    /** Test for equality
     */
    friend bool operator==(const Callback &l, const Callback &r)
    {
        return memcmp(&l, &r, sizeof(Callback)) == 0;
    }

    /** Test for inequality
     */
    friend bool operator!=(const Callback &l, const Callback &r)
    {
        return !(l == r);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     *  @param args Arguments to be called with function func
     *  @return the value as determined by func which is of
     *      type and determined by the signature of func
     */
    static R thunk(void *func, ArgTs... args)
    {
        return static_cast<Callback *>(func)->call(args...);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to guarantee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)(ArgTs...);
        void (*_boundfunc)(_class *, ArgTs...);
        void (_class::*_methodfunc)(ArgTs...);
    } _func;
    void *_obj;

    // Dynamically dispatched operations
    const struct ops {
        R(*call)(const void *, ArgTs...);
        void (*move)(void *, const void *);
        void (*dtor)(void *);
    } *_ops;

    // Generate operations for function object
    template <typename F>
    void generate(const F &f)
    {
        static const ops ops = {
            &Callback::function_call<F>,
            &Callback::function_move<F>,
            &Callback::function_dtor<F>,
        };

        MBED_STATIC_ASSERT(sizeof(Callback) - sizeof(_ops) >= sizeof(F),
                           "Type F must not exceed the size of the Callback class");
        memset(this, 0, sizeof(Callback));
        new (this) F(f);
        _ops = &ops;
    }

    // Function attributes
    template <typename F>
    static R function_call(const void *p, ArgTs... args)
    {
        return (*(F *)p)(args...);
    }

    template <typename F>
    static void function_move(void *d, const void *p)
    {
        new (d) F(*(F *)p);
    }

    template <typename F>
    static void function_dtor(void *p)
    {
        ((F *)p)->~F();
    }

    // Wrappers for functions with context
    template <typename O, typename M>
    struct method_context {
        M method;
        O *obj;

        method_context(O *obj, M method)
            : method(method), obj(obj) {}

        R operator()(ArgTs... args) const
        {
            return (obj->*method)(args...);
        }
    };

    template <typename F, typename A>
    struct function_context {
        F func;
        A *arg;

        function_context(F func, A *arg)
            : func(func), arg(arg) {}

        R operator()(ArgTs... args) const
        {
            return func(arg, args...);
        }
    };
};

// Internally used event type
typedef Callback<void(int)> event_callback_t;


/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with inferred type
 */
template <typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(ArgTs...) = 0)
{
    return Callback<R(ArgTs...)>(func);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with inferred type
 */
template <typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const Callback<R(ArgTs...)> &func)
{
    return Callback<R(ArgTs...)>(func);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with inferred type
 */
template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(U *obj, R(T::*method)(ArgTs...))
{
    return Callback<R(ArgTs...)>(obj, method);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with inferred type
 */
template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const U *obj, R(T::*method)(ArgTs...) const)
{
    return Callback<R(ArgTs...)>(obj, method);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with inferred type
 */
template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(volatile U *obj, R(T::*method)(ArgTs...) volatile)
{
    return Callback<R(ArgTs...)>(obj, method);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with inferred type
 */
template<typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(const volatile U *obj, R(T::*method)(ArgTs...) const volatile)
{
    return Callback<R(ArgTs...)>(obj, method);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with inferred type
 */
template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(T *, ArgTs...), U *arg)
{
    return Callback<R(ArgTs...)>(func, arg);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with inferred type
 */
template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(const T *, ArgTs...), const U *arg)
{
    return Callback<R(ArgTs...)>(func, arg);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with inferred type
 */
template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(volatile T *, ArgTs...), volatile U *arg)
{
    return Callback<R(ArgTs...)>(func, arg);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with inferred type
 */
template <typename T, typename U, typename R, typename... ArgTs>
Callback<R(ArgTs...)> callback(R(*func)(const volatile T *, ArgTs...), const volatile U *arg)
{
    return Callback<R(ArgTs...)>(func, arg);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with inferred type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename... ArgTs>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
                      "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(ArgTs...)> callback(U *obj, R(*func)(T *, ArgTs...))
{
    return Callback<R(ArgTs...)>(func, obj);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with inferred type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename... ArgTs>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
                      "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(ArgTs...)> callback(const U *obj, R(*func)(const T *, ArgTs...))
{
    return Callback<R(ArgTs...)>(func, obj);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with inferred type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename... ArgTs>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
                      "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(ArgTs...)> callback(volatile U *obj, R(*func)(volatile T *, ArgTs...))
{
    return Callback<R(ArgTs...)>(func, obj);
}

/** Create a callback class with type inferred from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with inferred type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename... ArgTs>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
                      "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(ArgTs...)> callback(const volatile U *obj, R(*func)(const volatile T *, ArgTs...))
{
    return Callback<R(ArgTs...)>(func, obj);
}

/**@}*/

/**@}*/

} // namespace mbed

#endif
