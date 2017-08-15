/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
/** \addtogroup platform */
/** @{*/


/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
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
    struct enable_if { typedef R type; };

    template <typename R>
    struct enable_if<false, R> {};

    template <typename M, M>
    struct is_type {
        static const bool value = true;
    };
}

/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
 */
template <typename R>
class Callback<R()> {
public:
    /** Create a Callback with a static function
     *  @param func     Static function to attach
     */
    Callback(R (*func)() = 0) {
        if (!func) {
            _ops = 0;
        } else {
            generate(func);
        }
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     */
    Callback(const Callback<R()> &func) {
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
    Callback(U *obj, R (T::*method)()) {
        generate(method_context<T, R (T::*)()>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const U *obj, R (T::*method)() const) {
        generate(method_context<const T, R (T::*)() const>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(volatile U *obj, R (T::*method)() volatile) {
        generate(method_context<volatile T, R (T::*)() volatile>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const volatile U *obj, R (T::*method)() const volatile) {
        generate(method_context<const volatile T, R (T::*)() const volatile>(obj, method));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(T*), U *arg) {
        generate(function_context<R (*)(T*), T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const T*), const U *arg) {
        generate(function_context<R (*)(const T*), const T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(volatile T*), volatile U *arg) {
        generate(function_context<R (*)(volatile T*), volatile T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const volatile T*), const volatile U *arg) {
        generate(function_context<R (*)(const volatile T*), const volatile T>(func, arg));
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)() const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)() volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)() const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    Callback(U *obj, R (*func)(T*)) {
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
    Callback(const U *obj, R (*func)(const T*)) {
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
    Callback(volatile U *obj, R (*func)(volatile T*)) {
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
    Callback(const volatile U *obj, R (*func)(const volatile T*)) {
        new (this) Callback(func, obj);
    }

    /** Destroy a callback
     */
    ~Callback() {
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
    void attach(R (*func)()) {
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
    void attach(const Callback<R()> &func) {
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
    void attach(U *obj, R (T::*method)()) {
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
    void attach(const U *obj, R (T::*method)() const) {
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
    void attach(volatile U *obj, R (T::*method)() volatile) {
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
    void attach(const volatile U *obj, R (T::*method)() const volatile) {
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
    void attach(R (*func)(T*), U *arg) {
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
    void attach(R (*func)(const T*), const U *arg) {
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
    void attach(R (*func)(volatile T*), volatile U *arg) {
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
    void attach(R (*func)(const volatile T*), const volatile U *arg) {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)() const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)() volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)() const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    void attach(U *obj, R (*func)(T*)) {
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
    void attach(const U *obj, R (*func)(const T*)) {
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
    void attach(volatile U *obj, R (*func)(volatile T*)) {
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
    void attach(const volatile U *obj, R (*func)(const volatile T*)) {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Assign a callback
     */
    Callback &operator=(const Callback &that) {
        if (this != &that) {
            this->~Callback();
            new (this) Callback(that);
        }

        return *this;
    }

    /** Call the attached function
     */
    R call() const {
        MBED_ASSERT(_ops);
        return _ops->call(this);
    }

    /** Call the attached function
     */
    R operator()() const {
        return call();
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _ops;
    }

    /** Test for equality
     */
    friend bool operator==(const Callback &l, const Callback &r) {
        return memcmp(&l, &r, sizeof(Callback)) == 0;
    }

    /** Test for inequality
     */
    friend bool operator!=(const Callback &l, const Callback &r) {
        return !(l == r);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func) {
        return static_cast<Callback*>(func)->call();
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)();
        void (*_boundfunc)(_class*);
        void (_class::*_methodfunc)();
    } _func;
    void *_obj;

    // Dynamically dispatched operations
    const struct ops {
        R (*call)(const void*);
        void (*move)(void*, const void*);
        void (*dtor)(void*);
    } *_ops;

    // Generate operations for function object
    template <typename F>
    void generate(const F &f) {
        static const ops ops = {
            &Callback::function_call<F>,
            &Callback::function_move<F>,
            &Callback::function_dtor<F>,
        };

        MBED_STATIC_ASSERT(sizeof(Callback) - sizeof(_ops) >= sizeof(F),
                "Type F must not exceed the size of the Callback class");
        new (this) F(f);
        _ops = &ops;
    }

    // Function attributes
    template <typename F>
    static R function_call(const void *p) {
        return (*(F*)p)();
    }

    template <typename F>
    static void function_move(void *d, const void *p) {
        new (d) F(*(F*)p);
    }

    template <typename F>
    static void function_dtor(void *p) {
        ((F*)p)->~F();
    }

    // Wrappers for functions with context
    template <typename O, typename M>
    struct method_context {
        M method;
        O *obj;

        method_context(O *obj, M method)
            : method(method), obj(obj) {}

        R operator()() const {
            return (obj->*method)();
        }
    };

    template <typename F, typename A>
    struct function_context {
        F func;
        A *arg;

        function_context(F func, A *arg)
            : func(func), arg(arg) {}

        R operator()() const {
            return func(arg);
        }
    };
};

/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
 */
template <typename R, typename A0>
class Callback<R(A0)> {
public:
    /** Create a Callback with a static function
     *  @param func     Static function to attach
     */
    Callback(R (*func)(A0) = 0) {
        if (!func) {
            _ops = 0;
        } else {
            generate(func);
        }
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     */
    Callback(const Callback<R(A0)> &func) {
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
    Callback(U *obj, R (T::*method)(A0)) {
        generate(method_context<T, R (T::*)(A0)>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const U *obj, R (T::*method)(A0) const) {
        generate(method_context<const T, R (T::*)(A0) const>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(volatile U *obj, R (T::*method)(A0) volatile) {
        generate(method_context<volatile T, R (T::*)(A0) volatile>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const volatile U *obj, R (T::*method)(A0) const volatile) {
        generate(method_context<const volatile T, R (T::*)(A0) const volatile>(obj, method));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(T*, A0), U *arg) {
        generate(function_context<R (*)(T*, A0), T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const T*, A0), const U *arg) {
        generate(function_context<R (*)(const T*, A0), const T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(volatile T*, A0), volatile U *arg) {
        generate(function_context<R (*)(volatile T*, A0), volatile T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const volatile T*, A0), const volatile U *arg) {
        generate(function_context<R (*)(const volatile T*, A0), const volatile T>(func, arg));
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    Callback(U *obj, R (*func)(T*, A0)) {
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
    Callback(const U *obj, R (*func)(const T*, A0)) {
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
    Callback(volatile U *obj, R (*func)(volatile T*, A0)) {
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
    Callback(const volatile U *obj, R (*func)(const volatile T*, A0)) {
        new (this) Callback(func, obj);
    }

    /** Destroy a callback
     */
    ~Callback() {
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
    void attach(R (*func)(A0)) {
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
    void attach(const Callback<R(A0)> &func) {
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
    void attach(U *obj, R (T::*method)(A0)) {
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
    void attach(const U *obj, R (T::*method)(A0) const) {
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
    void attach(volatile U *obj, R (T::*method)(A0) volatile) {
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
    void attach(const volatile U *obj, R (T::*method)(A0) const volatile) {
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
    void attach(R (*func)(T*, A0), U *arg) {
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
    void attach(R (*func)(const T*, A0), const U *arg) {
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
    void attach(R (*func)(volatile T*, A0), volatile U *arg) {
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
    void attach(R (*func)(const volatile T*, A0), const volatile U *arg) {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    void attach(U *obj, R (*func)(T*, A0)) {
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
    void attach(const U *obj, R (*func)(const T*, A0)) {
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
    void attach(volatile U *obj, R (*func)(volatile T*, A0)) {
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
    void attach(const volatile U *obj, R (*func)(const volatile T*, A0)) {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Assign a callback
     */
    Callback &operator=(const Callback &that) {
        if (this != &that) {
            this->~Callback();
            new (this) Callback(that);
        }

        return *this;
    }

    /** Call the attached function
     */
    R call(A0 a0) const {
        MBED_ASSERT(_ops);
        return _ops->call(this, a0);
    }

    /** Call the attached function
     */
    R operator()(A0 a0) const {
        return call(a0);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _ops;
    }

    /** Test for equality
     */
    friend bool operator==(const Callback &l, const Callback &r) {
        return memcmp(&l, &r, sizeof(Callback)) == 0;
    }

    /** Test for inequality
     */
    friend bool operator!=(const Callback &l, const Callback &r) {
        return !(l == r);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0) {
        return static_cast<Callback*>(func)->call(a0);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)(A0);
        void (*_boundfunc)(_class*, A0);
        void (_class::*_methodfunc)(A0);
    } _func;
    void *_obj;

    // Dynamically dispatched operations
    const struct ops {
        R (*call)(const void*, A0);
        void (*move)(void*, const void*);
        void (*dtor)(void*);
    } *_ops;

    // Generate operations for function object
    template <typename F>
    void generate(const F &f) {
        static const ops ops = {
            &Callback::function_call<F>,
            &Callback::function_move<F>,
            &Callback::function_dtor<F>,
        };

        MBED_STATIC_ASSERT(sizeof(Callback) - sizeof(_ops) >= sizeof(F),
                "Type F must not exceed the size of the Callback class");
        new (this) F(f);
        _ops = &ops;
    }

    // Function attributes
    template <typename F>
    static R function_call(const void *p, A0 a0) {
        return (*(F*)p)(a0);
    }

    template <typename F>
    static void function_move(void *d, const void *p) {
        new (d) F(*(F*)p);
    }

    template <typename F>
    static void function_dtor(void *p) {
        ((F*)p)->~F();
    }

    // Wrappers for functions with context
    template <typename O, typename M>
    struct method_context {
        M method;
        O *obj;

        method_context(O *obj, M method)
            : method(method), obj(obj) {}

        R operator()(A0 a0) const {
            return (obj->*method)(a0);
        }
    };

    template <typename F, typename A>
    struct function_context {
        F func;
        A *arg;

        function_context(F func, A *arg)
            : func(func), arg(arg) {}

        R operator()(A0 a0) const {
            return func(arg, a0);
        }
    };
};

/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
 */
template <typename R, typename A0, typename A1>
class Callback<R(A0, A1)> {
public:
    /** Create a Callback with a static function
     *  @param func     Static function to attach
     */
    Callback(R (*func)(A0, A1) = 0) {
        if (!func) {
            _ops = 0;
        } else {
            generate(func);
        }
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     */
    Callback(const Callback<R(A0, A1)> &func) {
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
    Callback(U *obj, R (T::*method)(A0, A1)) {
        generate(method_context<T, R (T::*)(A0, A1)>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const U *obj, R (T::*method)(A0, A1) const) {
        generate(method_context<const T, R (T::*)(A0, A1) const>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(volatile U *obj, R (T::*method)(A0, A1) volatile) {
        generate(method_context<volatile T, R (T::*)(A0, A1) volatile>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const volatile U *obj, R (T::*method)(A0, A1) const volatile) {
        generate(method_context<const volatile T, R (T::*)(A0, A1) const volatile>(obj, method));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(T*, A0, A1), U *arg) {
        generate(function_context<R (*)(T*, A0, A1), T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const T*, A0, A1), const U *arg) {
        generate(function_context<R (*)(const T*, A0, A1), const T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(volatile T*, A0, A1), volatile U *arg) {
        generate(function_context<R (*)(volatile T*, A0, A1), volatile T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const volatile T*, A0, A1), const volatile U *arg) {
        generate(function_context<R (*)(const volatile T*, A0, A1), const volatile T>(func, arg));
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    Callback(U *obj, R (*func)(T*, A0, A1)) {
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
    Callback(const U *obj, R (*func)(const T*, A0, A1)) {
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
    Callback(volatile U *obj, R (*func)(volatile T*, A0, A1)) {
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
    Callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1)) {
        new (this) Callback(func, obj);
    }

    /** Destroy a callback
     */
    ~Callback() {
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
    void attach(R (*func)(A0, A1)) {
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
    void attach(const Callback<R(A0, A1)> &func) {
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
    void attach(U *obj, R (T::*method)(A0, A1)) {
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
    void attach(const U *obj, R (T::*method)(A0, A1) const) {
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
    void attach(volatile U *obj, R (T::*method)(A0, A1) volatile) {
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
    void attach(const volatile U *obj, R (T::*method)(A0, A1) const volatile) {
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
    void attach(R (*func)(T*, A0, A1), U *arg) {
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
    void attach(R (*func)(const T*, A0, A1), const U *arg) {
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
    void attach(R (*func)(volatile T*, A0, A1), volatile U *arg) {
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
    void attach(R (*func)(const volatile T*, A0, A1), const volatile U *arg) {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    void attach(U *obj, R (*func)(T*, A0, A1)) {
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
    void attach(const U *obj, R (*func)(const T*, A0, A1)) {
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
    void attach(volatile U *obj, R (*func)(volatile T*, A0, A1)) {
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
    void attach(const volatile U *obj, R (*func)(const volatile T*, A0, A1)) {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Assign a callback
     */
    Callback &operator=(const Callback &that) {
        if (this != &that) {
            this->~Callback();
            new (this) Callback(that);
        }

        return *this;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1) const {
        MBED_ASSERT(_ops);
        return _ops->call(this, a0, a1);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1) const {
        return call(a0, a1);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _ops;
    }

    /** Test for equality
     */
    friend bool operator==(const Callback &l, const Callback &r) {
        return memcmp(&l, &r, sizeof(Callback)) == 0;
    }

    /** Test for inequality
     */
    friend bool operator!=(const Callback &l, const Callback &r) {
        return !(l == r);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1) {
        return static_cast<Callback*>(func)->call(a0, a1);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)(A0, A1);
        void (*_boundfunc)(_class*, A0, A1);
        void (_class::*_methodfunc)(A0, A1);
    } _func;
    void *_obj;

    // Dynamically dispatched operations
    const struct ops {
        R (*call)(const void*, A0, A1);
        void (*move)(void*, const void*);
        void (*dtor)(void*);
    } *_ops;

    // Generate operations for function object
    template <typename F>
    void generate(const F &f) {
        static const ops ops = {
            &Callback::function_call<F>,
            &Callback::function_move<F>,
            &Callback::function_dtor<F>,
        };

        MBED_STATIC_ASSERT(sizeof(Callback) - sizeof(_ops) >= sizeof(F),
                "Type F must not exceed the size of the Callback class");
        new (this) F(f);
        _ops = &ops;
    }

    // Function attributes
    template <typename F>
    static R function_call(const void *p, A0 a0, A1 a1) {
        return (*(F*)p)(a0, a1);
    }

    template <typename F>
    static void function_move(void *d, const void *p) {
        new (d) F(*(F*)p);
    }

    template <typename F>
    static void function_dtor(void *p) {
        ((F*)p)->~F();
    }

    // Wrappers for functions with context
    template <typename O, typename M>
    struct method_context {
        M method;
        O *obj;

        method_context(O *obj, M method)
            : method(method), obj(obj) {}

        R operator()(A0 a0, A1 a1) const {
            return (obj->*method)(a0, a1);
        }
    };

    template <typename F, typename A>
    struct function_context {
        F func;
        A *arg;

        function_context(F func, A *arg)
            : func(func), arg(arg) {}

        R operator()(A0 a0, A1 a1) const {
            return func(arg, a0, a1);
        }
    };
};

/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
 */
template <typename R, typename A0, typename A1, typename A2>
class Callback<R(A0, A1, A2)> {
public:
    /** Create a Callback with a static function
     *  @param func     Static function to attach
     */
    Callback(R (*func)(A0, A1, A2) = 0) {
        if (!func) {
            _ops = 0;
        } else {
            generate(func);
        }
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2)> &func) {
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
    Callback(U *obj, R (T::*method)(A0, A1, A2)) {
        generate(method_context<T, R (T::*)(A0, A1, A2)>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const U *obj, R (T::*method)(A0, A1, A2) const) {
        generate(method_context<const T, R (T::*)(A0, A1, A2) const>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(volatile U *obj, R (T::*method)(A0, A1, A2) volatile) {
        generate(method_context<volatile T, R (T::*)(A0, A1, A2) volatile>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const volatile U *obj, R (T::*method)(A0, A1, A2) const volatile) {
        generate(method_context<const volatile T, R (T::*)(A0, A1, A2) const volatile>(obj, method));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(T*, A0, A1, A2), U *arg) {
        generate(function_context<R (*)(T*, A0, A1, A2), T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const T*, A0, A1, A2), const U *arg) {
        generate(function_context<R (*)(const T*, A0, A1, A2), const T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(volatile T*, A0, A1, A2), volatile U *arg) {
        generate(function_context<R (*)(volatile T*, A0, A1, A2), volatile T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const volatile T*, A0, A1, A2), const volatile U *arg) {
        generate(function_context<R (*)(const volatile T*, A0, A1, A2), const volatile T>(func, arg));
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    Callback(U *obj, R (*func)(T*, A0, A1, A2)) {
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
    Callback(const U *obj, R (*func)(const T*, A0, A1, A2)) {
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
    Callback(volatile U *obj, R (*func)(volatile T*, A0, A1, A2)) {
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
    Callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2)) {
        new (this) Callback(func, obj);
    }

    /** Destroy a callback
     */
    ~Callback() {
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
    void attach(R (*func)(A0, A1, A2)) {
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
    void attach(const Callback<R(A0, A1, A2)> &func) {
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
    void attach(U *obj, R (T::*method)(A0, A1, A2)) {
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
    void attach(const U *obj, R (T::*method)(A0, A1, A2) const) {
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
    void attach(volatile U *obj, R (T::*method)(A0, A1, A2) volatile) {
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
    void attach(const volatile U *obj, R (T::*method)(A0, A1, A2) const volatile) {
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
    void attach(R (*func)(T*, A0, A1, A2), U *arg) {
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
    void attach(R (*func)(const T*, A0, A1, A2), const U *arg) {
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
    void attach(R (*func)(volatile T*, A0, A1, A2), volatile U *arg) {
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
    void attach(R (*func)(const volatile T*, A0, A1, A2), const volatile U *arg) {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    void attach(U *obj, R (*func)(T*, A0, A1, A2)) {
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
    void attach(const U *obj, R (*func)(const T*, A0, A1, A2)) {
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
    void attach(volatile U *obj, R (*func)(volatile T*, A0, A1, A2)) {
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
    void attach(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2)) {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Assign a callback
     */
    Callback &operator=(const Callback &that) {
        if (this != &that) {
            this->~Callback();
            new (this) Callback(that);
        }

        return *this;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1, A2 a2) const {
        MBED_ASSERT(_ops);
        return _ops->call(this, a0, a1, a2);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1, A2 a2) const {
        return call(a0, a1, a2);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _ops;
    }

    /** Test for equality
     */
    friend bool operator==(const Callback &l, const Callback &r) {
        return memcmp(&l, &r, sizeof(Callback)) == 0;
    }

    /** Test for inequality
     */
    friend bool operator!=(const Callback &l, const Callback &r) {
        return !(l == r);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2) {
        return static_cast<Callback*>(func)->call(a0, a1, a2);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)(A0, A1, A2);
        void (*_boundfunc)(_class*, A0, A1, A2);
        void (_class::*_methodfunc)(A0, A1, A2);
    } _func;
    void *_obj;

    // Dynamically dispatched operations
    const struct ops {
        R (*call)(const void*, A0, A1, A2);
        void (*move)(void*, const void*);
        void (*dtor)(void*);
    } *_ops;

    // Generate operations for function object
    template <typename F>
    void generate(const F &f) {
        static const ops ops = {
            &Callback::function_call<F>,
            &Callback::function_move<F>,
            &Callback::function_dtor<F>,
        };

        MBED_STATIC_ASSERT(sizeof(Callback) - sizeof(_ops) >= sizeof(F),
                "Type F must not exceed the size of the Callback class");
        new (this) F(f);
        _ops = &ops;
    }

    // Function attributes
    template <typename F>
    static R function_call(const void *p, A0 a0, A1 a1, A2 a2) {
        return (*(F*)p)(a0, a1, a2);
    }

    template <typename F>
    static void function_move(void *d, const void *p) {
        new (d) F(*(F*)p);
    }

    template <typename F>
    static void function_dtor(void *p) {
        ((F*)p)->~F();
    }

    // Wrappers for functions with context
    template <typename O, typename M>
    struct method_context {
        M method;
        O *obj;

        method_context(O *obj, M method)
            : method(method), obj(obj) {}

        R operator()(A0 a0, A1 a1, A2 a2) const {
            return (obj->*method)(a0, a1, a2);
        }
    };

    template <typename F, typename A>
    struct function_context {
        F func;
        A *arg;

        function_context(F func, A *arg)
            : func(func), arg(arg) {}

        R operator()(A0 a0, A1 a1, A2 a2) const {
            return func(arg, a0, a1, a2);
        }
    };
};

/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
 */
template <typename R, typename A0, typename A1, typename A2, typename A3>
class Callback<R(A0, A1, A2, A3)> {
public:
    /** Create a Callback with a static function
     *  @param func     Static function to attach
     */
    Callback(R (*func)(A0, A1, A2, A3) = 0) {
        if (!func) {
            _ops = 0;
        } else {
            generate(func);
        }
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2, A3)> &func) {
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
    Callback(U *obj, R (T::*method)(A0, A1, A2, A3)) {
        generate(method_context<T, R (T::*)(A0, A1, A2, A3)>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const U *obj, R (T::*method)(A0, A1, A2, A3) const) {
        generate(method_context<const T, R (T::*)(A0, A1, A2, A3) const>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(volatile U *obj, R (T::*method)(A0, A1, A2, A3) volatile) {
        generate(method_context<volatile T, R (T::*)(A0, A1, A2, A3) volatile>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const volatile U *obj, R (T::*method)(A0, A1, A2, A3) const volatile) {
        generate(method_context<const volatile T, R (T::*)(A0, A1, A2, A3) const volatile>(obj, method));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(T*, A0, A1, A2, A3), U *arg) {
        generate(function_context<R (*)(T*, A0, A1, A2, A3), T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const T*, A0, A1, A2, A3), const U *arg) {
        generate(function_context<R (*)(const T*, A0, A1, A2, A3), const T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(volatile T*, A0, A1, A2, A3), volatile U *arg) {
        generate(function_context<R (*)(volatile T*, A0, A1, A2, A3), volatile T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const volatile T*, A0, A1, A2, A3), const volatile U *arg) {
        generate(function_context<R (*)(const volatile T*, A0, A1, A2, A3), const volatile T>(func, arg));
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    Callback(U *obj, R (*func)(T*, A0, A1, A2, A3)) {
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
    Callback(const U *obj, R (*func)(const T*, A0, A1, A2, A3)) {
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
    Callback(volatile U *obj, R (*func)(volatile T*, A0, A1, A2, A3)) {
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
    Callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2, A3)) {
        new (this) Callback(func, obj);
    }

    /** Destroy a callback
     */
    ~Callback() {
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
    void attach(R (*func)(A0, A1, A2, A3)) {
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
    void attach(const Callback<R(A0, A1, A2, A3)> &func) {
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
    void attach(U *obj, R (T::*method)(A0, A1, A2, A3)) {
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
    void attach(const U *obj, R (T::*method)(A0, A1, A2, A3) const) {
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
    void attach(volatile U *obj, R (T::*method)(A0, A1, A2, A3) volatile) {
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
    void attach(const volatile U *obj, R (T::*method)(A0, A1, A2, A3) const volatile) {
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
    void attach(R (*func)(T*, A0, A1, A2, A3), U *arg) {
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
    void attach(R (*func)(const T*, A0, A1, A2, A3), const U *arg) {
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
    void attach(R (*func)(volatile T*, A0, A1, A2, A3), volatile U *arg) {
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
    void attach(R (*func)(const volatile T*, A0, A1, A2, A3), const volatile U *arg) {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    void attach(U *obj, R (*func)(T*, A0, A1, A2, A3)) {
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
    void attach(const U *obj, R (*func)(const T*, A0, A1, A2, A3)) {
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
    void attach(volatile U *obj, R (*func)(volatile T*, A0, A1, A2, A3)) {
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
    void attach(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2, A3)) {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Assign a callback
     */
    Callback &operator=(const Callback &that) {
        if (this != &that) {
            this->~Callback();
            new (this) Callback(that);
        }

        return *this;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1, A2 a2, A3 a3) const {
        MBED_ASSERT(_ops);
        return _ops->call(this, a0, a1, a2, a3);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1, A2 a2, A3 a3) const {
        return call(a0, a1, a2, a3);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _ops;
    }

    /** Test for equality
     */
    friend bool operator==(const Callback &l, const Callback &r) {
        return memcmp(&l, &r, sizeof(Callback)) == 0;
    }

    /** Test for inequality
     */
    friend bool operator!=(const Callback &l, const Callback &r) {
        return !(l == r);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
        return static_cast<Callback*>(func)->call(a0, a1, a2, a3);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)(A0, A1, A2, A3);
        void (*_boundfunc)(_class*, A0, A1, A2, A3);
        void (_class::*_methodfunc)(A0, A1, A2, A3);
    } _func;
    void *_obj;

    // Dynamically dispatched operations
    const struct ops {
        R (*call)(const void*, A0, A1, A2, A3);
        void (*move)(void*, const void*);
        void (*dtor)(void*);
    } *_ops;

    // Generate operations for function object
    template <typename F>
    void generate(const F &f) {
        static const ops ops = {
            &Callback::function_call<F>,
            &Callback::function_move<F>,
            &Callback::function_dtor<F>,
        };

        MBED_STATIC_ASSERT(sizeof(Callback) - sizeof(_ops) >= sizeof(F),
                "Type F must not exceed the size of the Callback class");
        new (this) F(f);
        _ops = &ops;
    }

    // Function attributes
    template <typename F>
    static R function_call(const void *p, A0 a0, A1 a1, A2 a2, A3 a3) {
        return (*(F*)p)(a0, a1, a2, a3);
    }

    template <typename F>
    static void function_move(void *d, const void *p) {
        new (d) F(*(F*)p);
    }

    template <typename F>
    static void function_dtor(void *p) {
        ((F*)p)->~F();
    }

    // Wrappers for functions with context
    template <typename O, typename M>
    struct method_context {
        M method;
        O *obj;

        method_context(O *obj, M method)
            : method(method), obj(obj) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3) const {
            return (obj->*method)(a0, a1, a2, a3);
        }
    };

    template <typename F, typename A>
    struct function_context {
        F func;
        A *arg;

        function_context(F func, A *arg)
            : func(func), arg(arg) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3) const {
            return func(arg, a0, a1, a2, a3);
        }
    };
};

/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
 */
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
class Callback<R(A0, A1, A2, A3, A4)> {
public:
    /** Create a Callback with a static function
     *  @param func     Static function to attach
     */
    Callback(R (*func)(A0, A1, A2, A3, A4) = 0) {
        if (!func) {
            _ops = 0;
        } else {
            generate(func);
        }
    }

    /** Attach a Callback
     *  @param func     The Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2, A3, A4)> &func) {
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
    Callback(U *obj, R (T::*method)(A0, A1, A2, A3, A4)) {
        generate(method_context<T, R (T::*)(A0, A1, A2, A3, A4)>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const U *obj, R (T::*method)(A0, A1, A2, A3, A4) const) {
        generate(method_context<const T, R (T::*)(A0, A1, A2, A3, A4) const>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) volatile) {
        generate(method_context<volatile T, R (T::*)(A0, A1, A2, A3, A4) volatile>(obj, method));
    }

    /** Create a Callback with a member function
     *  @param obj      Pointer to object to invoke member function on
     *  @param method   Member function to attach
     */
    template<typename T, typename U>
    Callback(const volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) const volatile) {
        generate(method_context<const volatile T, R (T::*)(A0, A1, A2, A3, A4) const volatile>(obj, method));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(T*, A0, A1, A2, A3, A4), U *arg) {
        generate(function_context<R (*)(T*, A0, A1, A2, A3, A4), T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const T*, A0, A1, A2, A3, A4), const U *arg) {
        generate(function_context<R (*)(const T*, A0, A1, A2, A3, A4), const T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(volatile T*, A0, A1, A2, A3, A4), volatile U *arg) {
        generate(function_context<R (*)(volatile T*, A0, A1, A2, A3, A4), volatile T>(func, arg));
    }

    /** Create a Callback with a static function and bound pointer
     *  @param func     Static function to attach
     *  @param arg      Pointer argument to function 
     */
    template<typename T, typename U>
    Callback(R (*func)(const volatile T*, A0, A1, A2, A3, A4), const volatile U *arg) {
        generate(function_context<R (*)(const volatile T*, A0, A1, A2, A3, A4), const volatile T>(func, arg));
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        generate(f);
    }

    /** Create a Callback with a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     */
    template <typename F>
    Callback(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    Callback(U *obj, R (*func)(T*, A0, A1, A2, A3, A4)) {
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
    Callback(const U *obj, R (*func)(const T*, A0, A1, A2, A3, A4)) {
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
    Callback(volatile U *obj, R (*func)(volatile T*, A0, A1, A2, A3, A4)) {
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
    Callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2, A3, A4)) {
        new (this) Callback(func, obj);
    }

    /** Destroy a callback
     */
    ~Callback() {
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
    void attach(R (*func)(A0, A1, A2, A3, A4)) {
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
    void attach(const Callback<R(A0, A1, A2, A3, A4)> &func) {
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
    void attach(U *obj, R (T::*method)(A0, A1, A2, A3, A4)) {
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
    void attach(const U *obj, R (T::*method)(A0, A1, A2, A3, A4) const) {
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
    void attach(volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) volatile) {
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
    void attach(const volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) const volatile) {
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
    void attach(R (*func)(T*, A0, A1, A2, A3, A4), U *arg) {
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
    void attach(R (*func)(const T*, A0, A1, A2, A3, A4), const U *arg) {
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
    void attach(R (*func)(volatile T*, A0, A1, A2, A3, A4), volatile U *arg) {
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
    void attach(R (*func)(const volatile T*, A0, A1, A2, A3, A4), const volatile U *arg) {
        this->~Callback();
        new (this) Callback(func, arg);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4), &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4) const, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4) volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
        this->~Callback();
        new (this) Callback(f);
    }

    /** Attach a function object
     *  @param func     Function object to attach
     *  @note The function object is limited to a single word of storage
     *  @deprecated
     *      Replaced by simple assignment 'Callback cb = func'
     */
    template <typename F>
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "Replaced by simple assignment 'Callback cb = func")
    void attach(const volatile F f, typename detail::enable_if<
                detail::is_type<R (F::*)(A0, A1, A2, A3, A4) const volatile, &F::operator()>::value &&
                sizeof(F) <= sizeof(uintptr_t)
            >::type = detail::nil()) {
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
    void attach(U *obj, R (*func)(T*, A0, A1, A2, A3, A4)) {
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
    void attach(const U *obj, R (*func)(const T*, A0, A1, A2, A3, A4)) {
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
    void attach(volatile U *obj, R (*func)(volatile T*, A0, A1, A2, A3, A4)) {
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
    void attach(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2, A3, A4)) {
        this->~Callback();
        new (this) Callback(func, obj);
    }

    /** Assign a callback
     */
    Callback &operator=(const Callback &that) {
        if (this != &that) {
            this->~Callback();
            new (this) Callback(that);
        }

        return *this;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
        MBED_ASSERT(_ops);
        return _ops->call(this, a0, a1, a2, a3, a4);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
        return call(a0, a1, a2, a3, a4);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _ops;
    }

    /** Test for equality
     */
    friend bool operator==(const Callback &l, const Callback &r) {
        return memcmp(&l, &r, sizeof(Callback)) == 0;
    }

    /** Test for inequality
     */
    friend bool operator!=(const Callback &l, const Callback &r) {
        return !(l == r);
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return static_cast<Callback*>(func)->call(a0, a1, a2, a3, a4);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)(A0, A1, A2, A3, A4);
        void (*_boundfunc)(_class*, A0, A1, A2, A3, A4);
        void (_class::*_methodfunc)(A0, A1, A2, A3, A4);
    } _func;
    void *_obj;

    // Dynamically dispatched operations
    const struct ops {
        R (*call)(const void*, A0, A1, A2, A3, A4);
        void (*move)(void*, const void*);
        void (*dtor)(void*);
    } *_ops;

    // Generate operations for function object
    template <typename F>
    void generate(const F &f) {
        static const ops ops = {
            &Callback::function_call<F>,
            &Callback::function_move<F>,
            &Callback::function_dtor<F>,
        };

        MBED_STATIC_ASSERT(sizeof(Callback) - sizeof(_ops) >= sizeof(F),
                "Type F must not exceed the size of the Callback class");
        new (this) F(f);
        _ops = &ops;
    }

    // Function attributes
    template <typename F>
    static R function_call(const void *p, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return (*(F*)p)(a0, a1, a2, a3, a4);
    }

    template <typename F>
    static void function_move(void *d, const void *p) {
        new (d) F(*(F*)p);
    }

    template <typename F>
    static void function_dtor(void *p) {
        ((F*)p)->~F();
    }

    // Wrappers for functions with context
    template <typename O, typename M>
    struct method_context {
        M method;
        O *obj;

        method_context(O *obj, M method)
            : method(method), obj(obj) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
            return (obj->*method)(a0, a1, a2, a3, a4);
        }
    };

    template <typename F, typename A>
    struct function_context {
        F func;
        A *arg;

        function_context(F func, A *arg)
            : func(func), arg(arg) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
            return func(arg, a0, a1, a2, a3, a4);
        }
    };
};

// Internally used event type
typedef Callback<void(int)> event_callback_t;


/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R>
Callback<R()> callback(R (*func)() = 0) {
    return Callback<R()>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R>
Callback<R()> callback(const Callback<R()> &func) {
    return Callback<R()>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R>
Callback<R()> callback(U *obj, R (T::*method)()) {
    return Callback<R()>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R>
Callback<R()> callback(const U *obj, R (T::*method)() const) {
    return Callback<R()>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R>
Callback<R()> callback(volatile U *obj, R (T::*method)() volatile) {
    return Callback<R()>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R>
Callback<R()> callback(const volatile U *obj, R (T::*method)() const volatile) {
    return Callback<R()>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R>
Callback<R()> callback(R (*func)(T*), U *arg) {
    return Callback<R()>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R>
Callback<R()> callback(R (*func)(const T*), const U *arg) {
    return Callback<R()>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R>
Callback<R()> callback(R (*func)(volatile T*), volatile U *arg) {
    return Callback<R()>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R>
Callback<R()> callback(R (*func)(const volatile T*), const volatile U *arg) {
    return Callback<R()>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R()> callback(U *obj, R (*func)(T*)) {
    return Callback<R()>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R()> callback(const U *obj, R (*func)(const T*)) {
    return Callback<R()>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R()> callback(volatile U *obj, R (*func)(volatile T*)) {
    return Callback<R()>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R()> callback(const volatile U *obj, R (*func)(const volatile T*)) {
    return Callback<R()>(func, obj);
}


/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0>
Callback<R(A0)> callback(R (*func)(A0) = 0) {
    return Callback<R(A0)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0>
Callback<R(A0)> callback(const Callback<R(A0)> &func) {
    return Callback<R(A0)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(U *obj, R (T::*method)(A0)) {
    return Callback<R(A0)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(const U *obj, R (T::*method)(A0) const) {
    return Callback<R(A0)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(volatile U *obj, R (T::*method)(A0) volatile) {
    return Callback<R(A0)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(const volatile U *obj, R (T::*method)(A0) const volatile) {
    return Callback<R(A0)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(R (*func)(T*, A0), U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(R (*func)(const T*, A0), const U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(R (*func)(volatile T*, A0), volatile U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> callback(R (*func)(const volatile T*, A0), const volatile U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0)> callback(U *obj, R (*func)(T*, A0)) {
    return Callback<R(A0)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0)> callback(const U *obj, R (*func)(const T*, A0)) {
    return Callback<R(A0)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0)> callback(volatile U *obj, R (*func)(volatile T*, A0)) {
    return Callback<R(A0)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0)> callback(const volatile U *obj, R (*func)(const volatile T*, A0)) {
    return Callback<R(A0)>(func, obj);
}


/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(R (*func)(A0, A1) = 0) {
    return Callback<R(A0, A1)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const Callback<R(A0, A1)> &func) {
    return Callback<R(A0, A1)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(U *obj, R (T::*method)(A0, A1)) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const U *obj, R (T::*method)(A0, A1) const) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(volatile U *obj, R (T::*method)(A0, A1) volatile) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const volatile U *obj, R (T::*method)(A0, A1) const volatile) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(R (*func)(T*, A0, A1), U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(R (*func)(const T*, A0, A1), const U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(R (*func)(volatile T*, A0, A1), volatile U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(R (*func)(const volatile T*, A0, A1), const volatile U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1)> callback(U *obj, R (*func)(T*, A0, A1)) {
    return Callback<R(A0, A1)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1)> callback(const U *obj, R (*func)(const T*, A0, A1)) {
    return Callback<R(A0, A1)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1)> callback(volatile U *obj, R (*func)(volatile T*, A0, A1)) {
    return Callback<R(A0, A1)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1)> callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1)) {
    return Callback<R(A0, A1)>(func, obj);
}


/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(R (*func)(A0, A1, A2) = 0) {
    return Callback<R(A0, A1, A2)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const Callback<R(A0, A1, A2)> &func) {
    return Callback<R(A0, A1, A2)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(U *obj, R (T::*method)(A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const U *obj, R (T::*method)(A0, A1, A2) const) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(volatile U *obj, R (T::*method)(A0, A1, A2) volatile) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const volatile U *obj, R (T::*method)(A0, A1, A2) const volatile) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(R (*func)(T*, A0, A1, A2), U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(R (*func)(const T*, A0, A1, A2), const U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(R (*func)(volatile T*, A0, A1, A2), volatile U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(R (*func)(const volatile T*, A0, A1, A2), const volatile U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2)> callback(U *obj, R (*func)(T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2)> callback(const U *obj, R (*func)(const T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2)> callback(volatile U *obj, R (*func)(volatile T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2)> callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(func, obj);
}


/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(R (*func)(A0, A1, A2, A3) = 0) {
    return Callback<R(A0, A1, A2, A3)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const Callback<R(A0, A1, A2, A3)> &func) {
    return Callback<R(A0, A1, A2, A3)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(U *obj, R (T::*method)(A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const U *obj, R (T::*method)(A0, A1, A2, A3) const) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(volatile U *obj, R (T::*method)(A0, A1, A2, A3) volatile) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const volatile U *obj, R (T::*method)(A0, A1, A2, A3) const volatile) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(R (*func)(T*, A0, A1, A2, A3), U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(R (*func)(const T*, A0, A1, A2, A3), const U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(R (*func)(volatile T*, A0, A1, A2, A3), volatile U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(R (*func)(const volatile T*, A0, A1, A2, A3), const volatile U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3)> callback(U *obj, R (*func)(T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3)> callback(const U *obj, R (*func)(const T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3)> callback(volatile U *obj, R (*func)(volatile T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3)> callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(func, obj);
}


/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(R (*func)(A0, A1, A2, A3, A4) = 0) {
    return Callback<R(A0, A1, A2, A3, A4)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @return         Callback with infered type
 */
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const Callback<R(A0, A1, A2, A3, A4)> &func) {
    return Callback<R(A0, A1, A2, A3, A4)>(func);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(U *obj, R (T::*method)(A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const U *obj, R (T::*method)(A0, A1, A2, A3, A4) const) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) volatile) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj      Optional pointer to object to bind to function
 *  @param method   Member function to attach
 *  @return         Callback with infered type
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) const volatile) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(R (*func)(T*, A0, A1, A2, A3, A4), U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(R (*func)(const T*, A0, A1, A2, A3, A4), const U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(R (*func)(volatile T*, A0, A1, A2, A3, A4), volatile U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param func     Static function to attach
 *  @param arg      Pointer argument to function
 *  @return         Callback with infered type
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(R (*func)(const volatile T*, A0, A1, A2, A3, A4), const volatile U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3, A4)> callback(U *obj, R (*func)(T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3, A4)> callback(const U *obj, R (*func)(const T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3, A4)> callback(volatile U *obj, R (*func)(volatile T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, obj);
}

/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with infered type
 *  @deprecated
 *      Arguments to callback have been reordered to callback(func, arg)
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
MBED_DEPRECATED_SINCE("mbed-os-5.1",
    "Arguments to callback have been reordered to callback(func, arg)")
Callback<R(A0, A1, A2, A3, A4)> callback(const volatile U *obj, R (*func)(const volatile T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, obj);
}


} // namespace mbed

#endif


/** @}*/
