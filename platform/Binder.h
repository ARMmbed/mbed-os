/* events
 * Copyright (c) 2016 ARM Limited
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
#ifndef BINDER_H
#define BINDER_H

#include "platform/mbed_assert.h"

namespace mbed {
/** \addtogroup mbed */
/** @{*/

/** Binder - Implementation class for dynamic binding of functions and arguments
 *
 *  Internally, the Binder class uses the system heap to allocate memory
 *  for functions and arguments which are bound together. This makes the
 *  Binder unsuitable for interrupt contexts, but it is still useful in
 *  threaded contexts.
 *
 *  @note There is limited support for interrupt-safe binding of a single
 *  pointer using the callback class, @see callback
 *
 *  @note The event queue provides full interrupt-safe binding, along with
 *  support for deferring from interrupts, @see EventQueue::event
 *
 *  @note It is not suggested to use the Binder class directly. The Binder
 *  class will always dynamically allocate its contents. The bind function
 *  supports small-object optimization and can store small objects in the
 *  callback class without a dynamic allocation.
 */
template <typename F>
class Binder;

/** Binder - Implementation class for dynamic binding of functions and arguments
 *  @see Binder
 */
template <typename R>
class Binder<R()> {
public:
    /** Create a Binder object
     *
     *  Dynamically allocates memory to wrap F in a Callback
     *
     *  @note It is not suggested to use the Binder class directly. The Binder
     *  class will always dynamically allocate its contents. The bind function
     *  supports small-object optimization and can store small objects in the
     *  callback class without a dynamic allocation.
     *
     *  @param f        The function object to bind
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on the system heap. Must be type-compatible
     *                  with b0..b4, the arguments to the underlying callback.
     */
    template <typename F>
    Binder(F f) {
        _bind = static_cast<struct bind*>(malloc(
                sizeof(struct bind) + sizeof(F)));
        MBED_ASSERT(_bind);
        _bind->ref = 1;

        _bind->call = &Binder::bind_call<F>;
        _bind->dtor = &Binder::bind_dtor<F>;

        new (_bind+1) F(f);
    }

    /** Copy constructor for a bound callback
     */
    Binder(const Binder &b) {
        _bind = b._bind;
        _bind->ref += 1;
    }

    /** Assignment operator for a bound callback
     */
    Binder &operator=(const Binder &that) {
        if (this != &that) {
            this->~Binder();
            new (this) Binder(that);
        }

        return *this;
    }

    /** Destructor for a bound callback
     */
    ~Binder() {
        _bind->ref -= 1;
        if (_bind->ref == 0) {
            _bind->dtor(_bind);
            free(_bind);
        }
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R call() const {
        return _bind->call(_bind);
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R operator()() const {
        return call();
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Binder to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     *  @return         The result of the function call
     */
    static R thunk(void *func) {
        return static_cast<Binder*>(func)->call();
    }

private:
    // Internal bind structure and attributes
    struct bind {
        unsigned ref;
        R (*call)(struct bind *);
        void (*dtor)(struct bind *);
    } *_bind;

    template <typename F>
    static R bind_call(struct bind *b) {
        return (*(F*)(b + 1))();
    }

    template <typename F>
    static void bind_dtor(struct bind *b) {
        ((F*)(b + 1))->~F();
    }

public:
    // Context for binds
    template <typename F, typename C0>
    struct context10 {
        F f; C0 c0;

        context10(F f, C0 c0)
            : f(f), c0(c0) {}

        R operator()() {
            return f(c0);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0>
    Binder(F f, C0 c0) {
        new (this) Binder(Binder::context10<F, C0>(f, c0));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(T *obj, R (T::*method)(B0), B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const T *obj, R (T::*method)(B0) const, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(volatile T *obj, R (T::*method)(B0) volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const volatile T *obj, R (T::*method)(B0) const volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    // Context for binds
    template <typename F, typename C0, typename C1>
    struct context20 {
        F f; C0 c0; C1 c1;

        context20(F f, C0 c0, C1 c1)
            : f(f), c0(c0), c1(c1) {}

        R operator()() {
            return f(c0, c1);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1>
    Binder(F f, C0 c0, C1 c1) {
        new (this) Binder(Binder::context20<F, C0, C1>(f, c0, c1));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(T *obj, R (T::*method)(B0, B1), B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const T *obj, R (T::*method)(B0, B1) const, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(volatile T *obj, R (T::*method)(B0, B1) volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const volatile T *obj, R (T::*method)(B0, B1) const volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2>
    struct context30 {
        F f; C0 c0; C1 c1; C2 c2;

        context30(F f, C0 c0, C1 c1, C2 c2)
            : f(f), c0(c0), c1(c1), c2(c2) {}

        R operator()() {
            return f(c0, c1, c2);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2>
    Binder(F f, C0 c0, C1 c1, C2 c2) {
        new (this) Binder(Binder::context30<F, C0, C1, C2>(f, c0, c1, c2));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(T *obj, R (T::*method)(B0, B1, B2), B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const T *obj, R (T::*method)(B0, B1, B2) const, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    struct context40 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3;

        context40(F f, C0 c0, C1 c1, C2 c2, C3 c3)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3) {}

        R operator()() {
            return f(c0, c1, c2, c3);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Binder(Binder::context40<F, C0, C1, C2, C3>(f, c0, c1, c2, c3));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    struct context50 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3; C4 c4;

        context50(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3), c4(c4) {}

        R operator()() {
            return f(c0, c1, c2, c3, c4);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Binder(Binder::context50<F, C0, C1, C2, C3, C4>(f, c0, c1, c2, c3, c4));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, B4), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, B4) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Binder - Implementation class for dynamic binding of functions and arguments
 *  @see Binder
 */
template <typename R, typename A0>
class Binder<R(A0)> {
public:
    /** Create a Binder object
     *
     *  Dynamically allocates memory to wrap F in a Callback
     *
     *  @note It is not suggested to use the Binder class directly. The Binder
     *  class will always dynamically allocate its contents. The bind function
     *  supports small-object optimization and can store small objects in the
     *  callback class without a dynamic allocation.
     *
     *  @param f        The function object to bind
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on the system heap. Must be type-compatible
     *                  with b0..b4, the arguments to the underlying callback.
     */
    template <typename F>
    Binder(F f) {
        _bind = static_cast<struct bind*>(malloc(
                sizeof(struct bind) + sizeof(F)));
        MBED_ASSERT(_bind);
        _bind->ref = 1;

        _bind->call = &Binder::bind_call<F>;
        _bind->dtor = &Binder::bind_dtor<F>;

        new (_bind+1) F(f);
    }

    /** Copy constructor for a bound callback
     */
    Binder(const Binder &b) {
        _bind = b._bind;
        _bind->ref += 1;
    }

    /** Assignment operator for a bound callback
     */
    Binder &operator=(const Binder &that) {
        if (this != &that) {
            this->~Binder();
            new (this) Binder(that);
        }

        return *this;
    }

    /** Destructor for a bound callback
     */
    ~Binder() {
        _bind->ref -= 1;
        if (_bind->ref == 0) {
            _bind->dtor(_bind);
            free(_bind);
        }
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R call(A0 a0) const {
        return _bind->call(_bind, a0);
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R operator()(A0 a0) const {
        return call(a0);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Binder to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     *  @return         The result of the function call
     */
    static R thunk(void *func, A0 a0) {
        return static_cast<Binder*>(func)->call(a0);
    }

private:
    // Internal bind structure and attributes
    struct bind {
        unsigned ref;
        R (*call)(struct bind *, A0 a0);
        void (*dtor)(struct bind *);
    } *_bind;

    template <typename F>
    static R bind_call(struct bind *b, A0 a0) {
        return (*(F*)(b + 1))(a0);
    }

    template <typename F>
    static void bind_dtor(struct bind *b) {
        ((F*)(b + 1))->~F();
    }

public:
    // Context for binds
    template <typename F, typename C0>
    struct context11 {
        F f; C0 c0;

        context11(F f, C0 c0)
            : f(f), c0(c0) {}

        R operator()(A0 a0) {
            return f(c0, a0);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0>
    Binder(F f, C0 c0) {
        new (this) Binder(Binder::context11<F, C0>(f, c0));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(T *obj, R (T::*method)(B0, A0), B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const T *obj, R (T::*method)(B0, A0) const, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(volatile T *obj, R (T::*method)(B0, A0) volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const volatile T *obj, R (T::*method)(B0, A0) const volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    // Context for binds
    template <typename F, typename C0, typename C1>
    struct context21 {
        F f; C0 c0; C1 c1;

        context21(F f, C0 c0, C1 c1)
            : f(f), c0(c0), c1(c1) {}

        R operator()(A0 a0) {
            return f(c0, c1, a0);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1>
    Binder(F f, C0 c0, C1 c1) {
        new (this) Binder(Binder::context21<F, C0, C1>(f, c0, c1));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(T *obj, R (T::*method)(B0, B1, A0), B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const T *obj, R (T::*method)(B0, B1, A0) const, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(volatile T *obj, R (T::*method)(B0, B1, A0) volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, A0) const volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2>
    struct context31 {
        F f; C0 c0; C1 c1; C2 c2;

        context31(F f, C0 c0, C1 c1, C2 c2)
            : f(f), c0(c0), c1(c1), c2(c2) {}

        R operator()(A0 a0) {
            return f(c0, c1, c2, a0);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2>
    Binder(F f, C0 c0, C1 c1, C2 c2) {
        new (this) Binder(Binder::context31<F, C0, C1, C2>(f, c0, c1, c2));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(T *obj, R (T::*method)(B0, B1, B2, A0), B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, A0) const, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, A0) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, A0) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    struct context41 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3;

        context41(F f, C0 c0, C1 c1, C2 c2, C3 c3)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3) {}

        R operator()(A0 a0) {
            return f(c0, c1, c2, c3, a0);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Binder(Binder::context41<F, C0, C1, C2, C3>(f, c0, c1, c2, c3));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, A0), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, A0) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    struct context51 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3; C4 c4;

        context51(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3), c4(c4) {}

        R operator()(A0 a0) {
            return f(c0, c1, c2, c3, c4, a0);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Binder(Binder::context51<F, C0, C1, C2, C3, C4>(f, c0, c1, c2, c3, c4));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Binder - Implementation class for dynamic binding of functions and arguments
 *  @see Binder
 */
template <typename R, typename A0, typename A1>
class Binder<R(A0, A1)> {
public:
    /** Create a Binder object
     *
     *  Dynamically allocates memory to wrap F in a Callback
     *
     *  @note It is not suggested to use the Binder class directly. The Binder
     *  class will always dynamically allocate its contents. The bind function
     *  supports small-object optimization and can store small objects in the
     *  callback class without a dynamic allocation.
     *
     *  @param f        The function object to bind
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on the system heap. Must be type-compatible
     *                  with b0..b4, the arguments to the underlying callback.
     */
    template <typename F>
    Binder(F f) {
        _bind = static_cast<struct bind*>(malloc(
                sizeof(struct bind) + sizeof(F)));
        MBED_ASSERT(_bind);
        _bind->ref = 1;

        _bind->call = &Binder::bind_call<F>;
        _bind->dtor = &Binder::bind_dtor<F>;

        new (_bind+1) F(f);
    }

    /** Copy constructor for a bound callback
     */
    Binder(const Binder &b) {
        _bind = b._bind;
        _bind->ref += 1;
    }

    /** Assignment operator for a bound callback
     */
    Binder &operator=(const Binder &that) {
        if (this != &that) {
            this->~Binder();
            new (this) Binder(that);
        }

        return *this;
    }

    /** Destructor for a bound callback
     */
    ~Binder() {
        _bind->ref -= 1;
        if (_bind->ref == 0) {
            _bind->dtor(_bind);
            free(_bind);
        }
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R call(A0 a0, A1 a1) const {
        return _bind->call(_bind, a0, a1);
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R operator()(A0 a0, A1 a1) const {
        return call(a0, a1);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Binder to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     *  @return         The result of the function call
     */
    static R thunk(void *func, A0 a0, A1 a1) {
        return static_cast<Binder*>(func)->call(a0, a1);
    }

private:
    // Internal bind structure and attributes
    struct bind {
        unsigned ref;
        R (*call)(struct bind *, A0 a0, A1 a1);
        void (*dtor)(struct bind *);
    } *_bind;

    template <typename F>
    static R bind_call(struct bind *b, A0 a0, A1 a1) {
        return (*(F*)(b + 1))(a0, a1);
    }

    template <typename F>
    static void bind_dtor(struct bind *b) {
        ((F*)(b + 1))->~F();
    }

public:
    // Context for binds
    template <typename F, typename C0>
    struct context12 {
        F f; C0 c0;

        context12(F f, C0 c0)
            : f(f), c0(c0) {}

        R operator()(A0 a0, A1 a1) {
            return f(c0, a0, a1);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0>
    Binder(F f, C0 c0) {
        new (this) Binder(Binder::context12<F, C0>(f, c0));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(T *obj, R (T::*method)(B0, A0, A1), B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const T *obj, R (T::*method)(B0, A0, A1) const, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(volatile T *obj, R (T::*method)(B0, A0, A1) volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const volatile T *obj, R (T::*method)(B0, A0, A1) const volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    // Context for binds
    template <typename F, typename C0, typename C1>
    struct context22 {
        F f; C0 c0; C1 c1;

        context22(F f, C0 c0, C1 c1)
            : f(f), c0(c0), c1(c1) {}

        R operator()(A0 a0, A1 a1) {
            return f(c0, c1, a0, a1);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1>
    Binder(F f, C0 c0, C1 c1) {
        new (this) Binder(Binder::context22<F, C0, C1>(f, c0, c1));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(T *obj, R (T::*method)(B0, B1, A0, A1), B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const T *obj, R (T::*method)(B0, B1, A0, A1) const, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(volatile T *obj, R (T::*method)(B0, B1, A0, A1) volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, A0, A1) const volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2>
    struct context32 {
        F f; C0 c0; C1 c1; C2 c2;

        context32(F f, C0 c0, C1 c1, C2 c2)
            : f(f), c0(c0), c1(c1), c2(c2) {}

        R operator()(A0 a0, A1 a1) {
            return f(c0, c1, c2, a0, a1);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2>
    Binder(F f, C0 c0, C1 c1, C2 c2) {
        new (this) Binder(Binder::context32<F, C0, C1, C2>(f, c0, c1, c2));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(T *obj, R (T::*method)(B0, B1, B2, A0, A1), B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, A0, A1) const, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    struct context42 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3;

        context42(F f, C0 c0, C1 c1, C2 c2, C3 c3)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3) {}

        R operator()(A0 a0, A1 a1) {
            return f(c0, c1, c2, c3, a0, a1);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Binder(Binder::context42<F, C0, C1, C2, C3>(f, c0, c1, c2, c3));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    struct context52 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3; C4 c4;

        context52(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3), c4(c4) {}

        R operator()(A0 a0, A1 a1) {
            return f(c0, c1, c2, c3, c4, a0, a1);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Binder(Binder::context52<F, C0, C1, C2, C3, C4>(f, c0, c1, c2, c3, c4));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Binder - Implementation class for dynamic binding of functions and arguments
 *  @see Binder
 */
template <typename R, typename A0, typename A1, typename A2>
class Binder<R(A0, A1, A2)> {
public:
    /** Create a Binder object
     *
     *  Dynamically allocates memory to wrap F in a Callback
     *
     *  @note It is not suggested to use the Binder class directly. The Binder
     *  class will always dynamically allocate its contents. The bind function
     *  supports small-object optimization and can store small objects in the
     *  callback class without a dynamic allocation.
     *
     *  @param f        The function object to bind
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on the system heap. Must be type-compatible
     *                  with b0..b4, the arguments to the underlying callback.
     */
    template <typename F>
    Binder(F f) {
        _bind = static_cast<struct bind*>(malloc(
                sizeof(struct bind) + sizeof(F)));
        MBED_ASSERT(_bind);
        _bind->ref = 1;

        _bind->call = &Binder::bind_call<F>;
        _bind->dtor = &Binder::bind_dtor<F>;

        new (_bind+1) F(f);
    }

    /** Copy constructor for a bound callback
     */
    Binder(const Binder &b) {
        _bind = b._bind;
        _bind->ref += 1;
    }

    /** Assignment operator for a bound callback
     */
    Binder &operator=(const Binder &that) {
        if (this != &that) {
            this->~Binder();
            new (this) Binder(that);
        }

        return *this;
    }

    /** Destructor for a bound callback
     */
    ~Binder() {
        _bind->ref -= 1;
        if (_bind->ref == 0) {
            _bind->dtor(_bind);
            free(_bind);
        }
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R call(A0 a0, A1 a1, A2 a2) const {
        return _bind->call(_bind, a0, a1, a2);
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R operator()(A0 a0, A1 a1, A2 a2) const {
        return call(a0, a1, a2);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Binder to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     *  @return         The result of the function call
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2) {
        return static_cast<Binder*>(func)->call(a0, a1, a2);
    }

private:
    // Internal bind structure and attributes
    struct bind {
        unsigned ref;
        R (*call)(struct bind *, A0 a0, A1 a1, A2 a2);
        void (*dtor)(struct bind *);
    } *_bind;

    template <typename F>
    static R bind_call(struct bind *b, A0 a0, A1 a1, A2 a2) {
        return (*(F*)(b + 1))(a0, a1, a2);
    }

    template <typename F>
    static void bind_dtor(struct bind *b) {
        ((F*)(b + 1))->~F();
    }

public:
    // Context for binds
    template <typename F, typename C0>
    struct context13 {
        F f; C0 c0;

        context13(F f, C0 c0)
            : f(f), c0(c0) {}

        R operator()(A0 a0, A1 a1, A2 a2) {
            return f(c0, a0, a1, a2);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0>
    Binder(F f, C0 c0) {
        new (this) Binder(Binder::context13<F, C0>(f, c0));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(T *obj, R (T::*method)(B0, A0, A1, A2), B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const T *obj, R (T::*method)(B0, A0, A1, A2) const, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(volatile T *obj, R (T::*method)(B0, A0, A1, A2) volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const volatile T *obj, R (T::*method)(B0, A0, A1, A2) const volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    // Context for binds
    template <typename F, typename C0, typename C1>
    struct context23 {
        F f; C0 c0; C1 c1;

        context23(F f, C0 c0, C1 c1)
            : f(f), c0(c0), c1(c1) {}

        R operator()(A0 a0, A1 a1, A2 a2) {
            return f(c0, c1, a0, a1, a2);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1>
    Binder(F f, C0 c0, C1 c1) {
        new (this) Binder(Binder::context23<F, C0, C1>(f, c0, c1));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(T *obj, R (T::*method)(B0, B1, A0, A1, A2), B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const T *obj, R (T::*method)(B0, B1, A0, A1, A2) const, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) const volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2>
    struct context33 {
        F f; C0 c0; C1 c1; C2 c2;

        context33(F f, C0 c0, C1 c1, C2 c2)
            : f(f), c0(c0), c1(c1), c2(c2) {}

        R operator()(A0 a0, A1 a1, A2 a2) {
            return f(c0, c1, c2, a0, a1, a2);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2>
    Binder(F f, C0 c0, C1 c1, C2 c2) {
        new (this) Binder(Binder::context33<F, C0, C1, C2>(f, c0, c1, c2));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2), B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    struct context43 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3;

        context43(F f, C0 c0, C1 c1, C2 c2, C3 c3)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3) {}

        R operator()(A0 a0, A1 a1, A2 a2) {
            return f(c0, c1, c2, c3, a0, a1, a2);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Binder(Binder::context43<F, C0, C1, C2, C3>(f, c0, c1, c2, c3));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    struct context53 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3; C4 c4;

        context53(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3), c4(c4) {}

        R operator()(A0 a0, A1 a1, A2 a2) {
            return f(c0, c1, c2, c3, c4, a0, a1, a2);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Binder(Binder::context53<F, C0, C1, C2, C3, C4>(f, c0, c1, c2, c3, c4));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Binder - Implementation class for dynamic binding of functions and arguments
 *  @see Binder
 */
template <typename R, typename A0, typename A1, typename A2, typename A3>
class Binder<R(A0, A1, A2, A3)> {
public:
    /** Create a Binder object
     *
     *  Dynamically allocates memory to wrap F in a Callback
     *
     *  @note It is not suggested to use the Binder class directly. The Binder
     *  class will always dynamically allocate its contents. The bind function
     *  supports small-object optimization and can store small objects in the
     *  callback class without a dynamic allocation.
     *
     *  @param f        The function object to bind
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on the system heap. Must be type-compatible
     *                  with b0..b4, the arguments to the underlying callback.
     */
    template <typename F>
    Binder(F f) {
        _bind = static_cast<struct bind*>(malloc(
                sizeof(struct bind) + sizeof(F)));
        MBED_ASSERT(_bind);
        _bind->ref = 1;

        _bind->call = &Binder::bind_call<F>;
        _bind->dtor = &Binder::bind_dtor<F>;

        new (_bind+1) F(f);
    }

    /** Copy constructor for a bound callback
     */
    Binder(const Binder &b) {
        _bind = b._bind;
        _bind->ref += 1;
    }

    /** Assignment operator for a bound callback
     */
    Binder &operator=(const Binder &that) {
        if (this != &that) {
            this->~Binder();
            new (this) Binder(that);
        }

        return *this;
    }

    /** Destructor for a bound callback
     */
    ~Binder() {
        _bind->ref -= 1;
        if (_bind->ref == 0) {
            _bind->dtor(_bind);
            free(_bind);
        }
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R call(A0 a0, A1 a1, A2 a2, A3 a3) const {
        return _bind->call(_bind, a0, a1, a2, a3);
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R operator()(A0 a0, A1 a1, A2 a2, A3 a3) const {
        return call(a0, a1, a2, a3);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Binder to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     *  @return         The result of the function call
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
        return static_cast<Binder*>(func)->call(a0, a1, a2, a3);
    }

private:
    // Internal bind structure and attributes
    struct bind {
        unsigned ref;
        R (*call)(struct bind *, A0 a0, A1 a1, A2 a2, A3 a3);
        void (*dtor)(struct bind *);
    } *_bind;

    template <typename F>
    static R bind_call(struct bind *b, A0 a0, A1 a1, A2 a2, A3 a3) {
        return (*(F*)(b + 1))(a0, a1, a2, a3);
    }

    template <typename F>
    static void bind_dtor(struct bind *b) {
        ((F*)(b + 1))->~F();
    }

public:
    // Context for binds
    template <typename F, typename C0>
    struct context14 {
        F f; C0 c0;

        context14(F f, C0 c0)
            : f(f), c0(c0) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3) {
            return f(c0, a0, a1, a2, a3);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0>
    Binder(F f, C0 c0) {
        new (this) Binder(Binder::context14<F, C0>(f, c0));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(T *obj, R (T::*method)(B0, A0, A1, A2, A3), B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const T *obj, R (T::*method)(B0, A0, A1, A2, A3) const, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) const volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    // Context for binds
    template <typename F, typename C0, typename C1>
    struct context24 {
        F f; C0 c0; C1 c1;

        context24(F f, C0 c0, C1 c1)
            : f(f), c0(c0), c1(c1) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3) {
            return f(c0, c1, a0, a1, a2, a3);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1>
    Binder(F f, C0 c0, C1 c1) {
        new (this) Binder(Binder::context24<F, C0, C1>(f, c0, c1));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3), B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2>
    struct context34 {
        F f; C0 c0; C1 c1; C2 c2;

        context34(F f, C0 c0, C1 c1, C2 c2)
            : f(f), c0(c0), c1(c1), c2(c2) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3) {
            return f(c0, c1, c2, a0, a1, a2, a3);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2>
    Binder(F f, C0 c0, C1 c1, C2 c2) {
        new (this) Binder(Binder::context34<F, C0, C1, C2>(f, c0, c1, c2));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3), B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    struct context44 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3;

        context44(F f, C0 c0, C1 c1, C2 c2, C3 c3)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3) {
            return f(c0, c1, c2, c3, a0, a1, a2, a3);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Binder(Binder::context44<F, C0, C1, C2, C3>(f, c0, c1, c2, c3));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    struct context54 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3; C4 c4;

        context54(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3), c4(c4) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3) {
            return f(c0, c1, c2, c3, c4, a0, a1, a2, a3);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Binder(Binder::context54<F, C0, C1, C2, C3, C4>(f, c0, c1, c2, c3, c4));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }
};

/** Binder - Implementation class for dynamic binding of functions and arguments
 *  @see Binder
 */
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
class Binder<R(A0, A1, A2, A3, A4)> {
public:
    /** Create a Binder object
     *
     *  Dynamically allocates memory to wrap F in a Callback
     *
     *  @note It is not suggested to use the Binder class directly. The Binder
     *  class will always dynamically allocate its contents. The bind function
     *  supports small-object optimization and can store small objects in the
     *  callback class without a dynamic allocation.
     *
     *  @param f        The function object to bind
     *  @param c0..c4   Arguments to bind to the callback, these arguments are
     *                  allocated on the system heap. Must be type-compatible
     *                  with b0..b4, the arguments to the underlying callback.
     */
    template <typename F>
    Binder(F f) {
        _bind = static_cast<struct bind*>(malloc(
                sizeof(struct bind) + sizeof(F)));
        MBED_ASSERT(_bind);
        _bind->ref = 1;

        _bind->call = &Binder::bind_call<F>;
        _bind->dtor = &Binder::bind_dtor<F>;

        new (_bind+1) F(f);
    }

    /** Copy constructor for a bound callback
     */
    Binder(const Binder &b) {
        _bind = b._bind;
        _bind->ref += 1;
    }

    /** Assignment operator for a bound callback
     */
    Binder &operator=(const Binder &that) {
        if (this != &that) {
            this->~Binder();
            new (this) Binder(that);
        }

        return *this;
    }

    /** Destructor for a bound callback
     */
    ~Binder() {
        _bind->ref -= 1;
        if (_bind->ref == 0) {
            _bind->dtor(_bind);
            free(_bind);
        }
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R call(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
        return _bind->call(_bind, a0, a1, a2, a3, a4);
    }

    /** Calls the underlying function with any bound arguments
     *
     *  @param a0..a4   Arguments to pass to the underlying function, appended
     *                  to any arguments bound to the function
     *  @return         The result of the function call
     */
    R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const {
        return call(a0, a1, a2, a3, a4);
    }

    /** Static thunk for passing as C-style function
     *
     *  @param func     Binder to call passed as a void pointer
     *  @param a0..a4   Arguments to pass to the event
     *  @return         The result of the function call
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return static_cast<Binder*>(func)->call(a0, a1, a2, a3, a4);
    }

private:
    // Internal bind structure and attributes
    struct bind {
        unsigned ref;
        R (*call)(struct bind *, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);
        void (*dtor)(struct bind *);
    } *_bind;

    template <typename F>
    static R bind_call(struct bind *b, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return (*(F*)(b + 1))(a0, a1, a2, a3, a4);
    }

    template <typename F>
    static void bind_dtor(struct bind *b) {
        ((F*)(b + 1))->~F();
    }

public:
    // Context for binds
    template <typename F, typename C0>
    struct context15 {
        F f; C0 c0;

        context15(F f, C0 c0)
            : f(f), c0(c0) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
            return f(c0, a0, a1, a2, a3, a4);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0>
    Binder(F f, C0 c0) {
        new (this) Binder(Binder::context15<F, C0>(f, c0));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4), B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0>
    Binder(const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const volatile, B0 b0) {
        new (this) Binder(callback(obj, method), b0);
    }

    // Context for binds
    template <typename F, typename C0, typename C1>
    struct context25 {
        F f; C0 c0; C1 c1;

        context25(F f, C0 c0, C1 c1)
            : f(f), c0(c0), c1(c1) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
            return f(c0, c1, a0, a1, a2, a3, a4);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1>
    Binder(F f, C0 c0, C1 c1) {
        new (this) Binder(Binder::context25<F, C0, C1>(f, c0, c1));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4), B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1) {
        new (this) Binder(callback(obj, method), b0, b1);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2>
    struct context35 {
        F f; C0 c0; C1 c1; C2 c2;

        context35(F f, C0 c0, C1 c1, C2 c2)
            : f(f), c0(c0), c1(c1), c2(c2) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
            return f(c0, c1, c2, a0, a1, a2, a3, a4);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2>
    Binder(F f, C0 c0, C1 c1, C2 c2) {
        new (this) Binder(Binder::context35<F, C0, C1, C2>(f, c0, c1, c2));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4), B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1, B2 b2) {
        new (this) Binder(callback(obj, method), b0, b1, b2);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    struct context45 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3;

        context45(F f, C0 c0, C1 c1, C2 c2, C3 c3)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
            return f(c0, c1, c2, c3, a0, a1, a2, a3, a4);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3) {
        new (this) Binder(Binder::context45<F, C0, C1, C2, C3>(f, c0, c1, c2, c3));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4), B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1, B2 b2, B3 b3) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3);
    }

    // Context for binds
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    struct context55 {
        F f; C0 c0; C1 c1; C2 c2; C3 c3; C4 c4;

        context55(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4)
            : f(f), c0(c0), c1(c1), c2(c2), c3(c3), c4(c4) {}

        R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
            return f(c0, c1, c2, c3, c4, a0, a1, a2, a3, a4);
        }
    };

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename F, typename C0, typename C1, typename C2, typename C3, typename C4>
    Binder(F f, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
        new (this) Binder(Binder::context55<F, C0, C1, C2, C3, C4>(f, c0, c1, c2, c3, c4));
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4), B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }

    /** Create a Binder object
     *  @see Binder::Binder
     */
    template <typename T, typename B0, typename B1, typename B2, typename B3, typename B4>
    Binder(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const volatile, B0 b0, B1 b1, B2 b2, B3 b3, B4 b4) {
        new (this) Binder(callback(obj, method), b0, b1, b2, b3, b4);
    }
};


/** Bind a function and arguments
 *
 *  If possible, the bind function stores small objects directly in the resulting
 *  callback. If the object and arguments are too large, bind will dynamically
 *  allocate memory to wrap F in a callback. This makes the Binder unsuitable for
 *  interrupt contexts, but it is still useful in threaded contexts.
 *
 *  @note There is limited support for interrupt-safe binding of a single
 *  pointer using the callback class, @see callback
 *
 *  @note The event queue provides full interrupt-safe binding, along with
 *  support for deferring from interrupts, @see EventQueue::event
 *
 *  @param f        The function to bind
 *  @param c0..c4   Arguments to bind to the callback, these arguments are
 *                  allocated on the system heap. Must be type-compatible
 *                  with b0..b4, the arguments to the underlying callback.
 *  @return         Callback-compatible function object that when called will
 *                  prepend the bound arguments before calling the underlying
 *                  function.
 */
template <typename R>
Callback<R()> bind(R (*func)()) {
    return Callback<R()>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R>
Callback<R()> bind(const Callback<R()> &func) {
    return Callback<R()>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R>
Callback<R()> bind(U *obj, R (T::*method)()) {
    return Callback<R()>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R>
Callback<R()> bind(const U *obj, R (T::*method)() const) {
    return Callback<R()>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R>
Callback<R()> bind(volatile U *obj, R (T::*method)() volatile) {
    return Callback<R()>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R>
Callback<R()> bind(const volatile U *obj, R (T::*method)() const volatile) {
    return Callback<R()>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R>
Callback<R()> bind(R (*func)(T*), U *arg) {
    return Callback<R()>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R>
Callback<R()> bind(R (*func)(const T*), const U *arg) {
    return Callback<R()>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R>
Callback<R()> bind(R (*func)(volatile T*), volatile U *arg) {
    return Callback<R()>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R>
Callback<R()> bind(R (*func)(const volatile T*), const volatile U *arg) {
    return Callback<R()>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0>
Callback<R(A0)> bind(R (*func)(A0)) {
    return Callback<R(A0)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0>
Callback<R(A0)> bind(const Callback<R(A0)> &func) {
    return Callback<R(A0)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(U *obj, R (T::*method)(A0)) {
    return Callback<R(A0)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(const U *obj, R (T::*method)(A0) const) {
    return Callback<R(A0)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(volatile U *obj, R (T::*method)(A0) volatile) {
    return Callback<R(A0)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(const volatile U *obj, R (T::*method)(A0) const volatile) {
    return Callback<R(A0)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(R (*func)(T*, A0), U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(R (*func)(const T*, A0), const U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(R (*func)(volatile T*, A0), volatile U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0>
Callback<R(A0)> bind(R (*func)(const volatile T*, A0), const volatile U *arg) {
    return Callback<R(A0)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(A0, A1)) {
    return Callback<R(A0, A1)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(const Callback<R(A0, A1)> &func) {
    return Callback<R(A0, A1)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(U *obj, R (T::*method)(A0, A1)) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(const U *obj, R (T::*method)(A0, A1) const) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(volatile U *obj, R (T::*method)(A0, A1) volatile) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(const volatile U *obj, R (T::*method)(A0, A1) const volatile) {
    return Callback<R(A0, A1)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(T*, A0, A1), U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(const T*, A0, A1), const U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(volatile T*, A0, A1), volatile U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(const volatile T*, A0, A1), const volatile U *arg) {
    return Callback<R(A0, A1)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const Callback<R(A0, A1, A2)> &func) {
    return Callback<R(A0, A1, A2)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(U *obj, R (T::*method)(A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const U *obj, R (T::*method)(A0, A1, A2) const) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(volatile U *obj, R (T::*method)(A0, A1, A2) volatile) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const volatile U *obj, R (T::*method)(A0, A1, A2) const volatile) {
    return Callback<R(A0, A1, A2)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(T*, A0, A1, A2), U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(const T*, A0, A1, A2), const U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(volatile T*, A0, A1, A2), volatile U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(const volatile T*, A0, A1, A2), const volatile U *arg) {
    return Callback<R(A0, A1, A2)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const Callback<R(A0, A1, A2, A3)> &func) {
    return Callback<R(A0, A1, A2, A3)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(U *obj, R (T::*method)(A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const U *obj, R (T::*method)(A0, A1, A2, A3) const) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(volatile U *obj, R (T::*method)(A0, A1, A2, A3) volatile) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const volatile U *obj, R (T::*method)(A0, A1, A2, A3) const volatile) {
    return Callback<R(A0, A1, A2, A3)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(T*, A0, A1, A2, A3), U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(const T*, A0, A1, A2, A3), const U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(volatile T*, A0, A1, A2, A3), volatile U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(const volatile T*, A0, A1, A2, A3), const volatile U *arg) {
    return Callback<R(A0, A1, A2, A3)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const Callback<R(A0, A1, A2, A3, A4)> &func) {
    return Callback<R(A0, A1, A2, A3, A4)>(func);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(U *obj, R (T::*method)(A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const U *obj, R (T::*method)(A0, A1, A2, A3, A4) const) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) volatile) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template<typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const volatile U *obj, R (T::*method)(A0, A1, A2, A3, A4) const volatile) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, method);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(T*, A0, A1, A2, A3, A4), U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(const T*, A0, A1, A2, A3, A4), const U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(volatile T*, A0, A1, A2, A3, A4), volatile U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename U, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(const volatile T*, A0, A1, A2, A3, A4), const volatile U *arg) {
    return Callback<R(A0, A1, A2, A3, A4)>(func, arg);
}
/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0>
Callback<R()> bind(R (*func)(B0), C0 c0) {
    return Binder<R()>(func, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0>
Callback<R()> bind(T *obj, R (T::*method)(B0), C0 c0) {
    return Binder<R()>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0>
Callback<R()> bind(const T *obj, R (T::*method)(B0) const, C0 c0) {
    return Binder<R()>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0>
Callback<R()> bind(volatile T *obj, R (T::*method)(B0) volatile, C0 c0) {
    return Binder<R()>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0>
Callback<R()> bind(const volatile T *obj, R (T::*method)(B0) const volatile, C0 c0) {
    return Binder<R()>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0>
Callback<R()> bind(Callback<R(B0)> cb, C0 c0) {
    return Binder<R()>(cb, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1>
Callback<R()> bind(R (*func)(B0, B1), C0 c0, C1 c1) {
    return Binder<R()>(func, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Callback<R()> bind(T *obj, R (T::*method)(B0, B1), C0 c0, C1 c1) {
    return Binder<R()>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Callback<R()> bind(const T *obj, R (T::*method)(B0, B1) const, C0 c0, C1 c1) {
    return Binder<R()>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Callback<R()> bind(volatile T *obj, R (T::*method)(B0, B1) volatile, C0 c0, C1 c1) {
    return Binder<R()>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1>
Callback<R()> bind(const volatile T *obj, R (T::*method)(B0, B1) const volatile, C0 c0, C1 c1) {
    return Binder<R()>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1>
Callback<R()> bind(Callback<R(B0, B1)> cb, C0 c0, C1 c1) {
    return Binder<R()>(cb, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Callback<R()> bind(R (*func)(B0, B1, B2), C0 c0, C1 c1, C2 c2) {
    return Binder<R()>(func, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Callback<R()> bind(T *obj, R (T::*method)(B0, B1, B2), C0 c0, C1 c1, C2 c2) {
    return Binder<R()>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Callback<R()> bind(const T *obj, R (T::*method)(B0, B1, B2) const, C0 c0, C1 c1, C2 c2) {
    return Binder<R()>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Callback<R()> bind(volatile T *obj, R (T::*method)(B0, B1, B2) volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R()>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Callback<R()> bind(const volatile T *obj, R (T::*method)(B0, B1, B2) const volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R()>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2>
Callback<R()> bind(Callback<R(B0, B1, B2)> cb, C0 c0, C1 c1, C2 c2) {
    return Binder<R()>(cb, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Callback<R()> bind(R (*func)(B0, B1, B2, B3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R()>(func, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Callback<R()> bind(T *obj, R (T::*method)(B0, B1, B2, B3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Callback<R()> bind(const T *obj, R (T::*method)(B0, B1, B2, B3) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Callback<R()> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Callback<R()> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3>
Callback<R()> bind(Callback<R(B0, B1, B2, B3)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R()>(cb, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Callback<R()> bind(R (*func)(B0, B1, B2, B3, B4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R()>(func, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Callback<R()> bind(T *obj, R (T::*method)(B0, B1, B2, B3, B4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Callback<R()> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, B4) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Callback<R()> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Callback<R()> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R()>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4>
Callback<R()> bind(Callback<R(B0, B1, B2, B3, B4)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R()>(cb, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0>
Callback<R(A0)> bind(R (*func)(B0, A0), C0 c0) {
    return Binder<R(A0)>(func, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0>
Callback<R(A0)> bind(T *obj, R (T::*method)(B0, A0), C0 c0) {
    return Binder<R(A0)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0>
Callback<R(A0)> bind(const T *obj, R (T::*method)(B0, A0) const, C0 c0) {
    return Binder<R(A0)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0>
Callback<R(A0)> bind(volatile T *obj, R (T::*method)(B0, A0) volatile, C0 c0) {
    return Binder<R(A0)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0>
Callback<R(A0)> bind(const volatile T *obj, R (T::*method)(B0, A0) const volatile, C0 c0) {
    return Binder<R(A0)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0>
Callback<R(A0)> bind(Callback<R(B0, A0)> cb, C0 c0) {
    return Binder<R(A0)>(cb, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Callback<R(A0)> bind(R (*func)(B0, B1, A0), C0 c0, C1 c1) {
    return Binder<R(A0)>(func, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Callback<R(A0)> bind(T *obj, R (T::*method)(B0, B1, A0), C0 c0, C1 c1) {
    return Binder<R(A0)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Callback<R(A0)> bind(const T *obj, R (T::*method)(B0, B1, A0) const, C0 c0, C1 c1) {
    return Binder<R(A0)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Callback<R(A0)> bind(volatile T *obj, R (T::*method)(B0, B1, A0) volatile, C0 c0, C1 c1) {
    return Binder<R(A0)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Callback<R(A0)> bind(const volatile T *obj, R (T::*method)(B0, B1, A0) const volatile, C0 c0, C1 c1) {
    return Binder<R(A0)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0>
Callback<R(A0)> bind(Callback<R(B0, B1, A0)> cb, C0 c0, C1 c1) {
    return Binder<R(A0)>(cb, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Callback<R(A0)> bind(R (*func)(B0, B1, B2, A0), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0)>(func, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Callback<R(A0)> bind(T *obj, R (T::*method)(B0, B1, B2, A0), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Callback<R(A0)> bind(const T *obj, R (T::*method)(B0, B1, B2, A0) const, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Callback<R(A0)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, A0) volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Callback<R(A0)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, A0) const volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0>
Callback<R(A0)> bind(Callback<R(B0, B1, B2, A0)> cb, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0)>(cb, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Callback<R(A0)> bind(R (*func)(B0, B1, B2, B3, A0), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0)>(func, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Callback<R(A0)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, A0), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Callback<R(A0)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, A0) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Callback<R(A0)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Callback<R(A0)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0>
Callback<R(A0)> bind(Callback<R(B0, B1, B2, B3, A0)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0)>(cb, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Callback<R(A0)> bind(R (*func)(B0, B1, B2, B3, B4, A0), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0)>(func, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Callback<R(A0)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Callback<R(A0)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Callback<R(A0)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Callback<R(A0)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0>
Callback<R(A0)> bind(Callback<R(B0, B1, B2, B3, B4, A0)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0)>(cb, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(B0, A0, A1), C0 c0) {
    return Binder<R(A0, A1)>(func, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Callback<R(A0, A1)> bind(T *obj, R (T::*method)(B0, A0, A1), C0 c0) {
    return Binder<R(A0, A1)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Callback<R(A0, A1)> bind(const T *obj, R (T::*method)(B0, A0, A1) const, C0 c0) {
    return Binder<R(A0, A1)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Callback<R(A0, A1)> bind(volatile T *obj, R (T::*method)(B0, A0, A1) volatile, C0 c0) {
    return Binder<R(A0, A1)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1>
Callback<R(A0, A1)> bind(const volatile T *obj, R (T::*method)(B0, A0, A1) const volatile, C0 c0) {
    return Binder<R(A0, A1)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1>
Callback<R(A0, A1)> bind(Callback<R(B0, A0, A1)> cb, C0 c0) {
    return Binder<R(A0, A1)>(cb, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(B0, B1, A0, A1), C0 c0, C1 c1) {
    return Binder<R(A0, A1)>(func, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Callback<R(A0, A1)> bind(T *obj, R (T::*method)(B0, B1, A0, A1), C0 c0, C1 c1) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Callback<R(A0, A1)> bind(const T *obj, R (T::*method)(B0, B1, A0, A1) const, C0 c0, C1 c1) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Callback<R(A0, A1)> bind(volatile T *obj, R (T::*method)(B0, B1, A0, A1) volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Callback<R(A0, A1)> bind(const volatile T *obj, R (T::*method)(B0, B1, A0, A1) const volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1>
Callback<R(A0, A1)> bind(Callback<R(B0, B1, A0, A1)> cb, C0 c0, C1 c1) {
    return Binder<R(A0, A1)>(cb, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(B0, B1, B2, A0, A1), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1)>(func, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Callback<R(A0, A1)> bind(T *obj, R (T::*method)(B0, B1, B2, A0, A1), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Callback<R(A0, A1)> bind(const T *obj, R (T::*method)(B0, B1, B2, A0, A1) const, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Callback<R(A0, A1)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Callback<R(A0, A1)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1) const volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1>
Callback<R(A0, A1)> bind(Callback<R(B0, B1, B2, A0, A1)> cb, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1)>(cb, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(B0, B1, B2, B3, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1)>(func, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Callback<R(A0, A1)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Callback<R(A0, A1)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Callback<R(A0, A1)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Callback<R(A0, A1)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1>
Callback<R(A0, A1)> bind(Callback<R(B0, B1, B2, B3, A0, A1)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1)>(cb, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Callback<R(A0, A1)> bind(R (*func)(B0, B1, B2, B3, B4, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1)>(func, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Callback<R(A0, A1)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Callback<R(A0, A1)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Callback<R(A0, A1)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Callback<R(A0, A1)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1>
Callback<R(A0, A1)> bind(Callback<R(B0, B1, B2, B3, B4, A0, A1)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1)>(cb, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(B0, A0, A1, A2), C0 c0) {
    return Binder<R(A0, A1, A2)>(func, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(T *obj, R (T::*method)(B0, A0, A1, A2), C0 c0) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const T *obj, R (T::*method)(B0, A0, A1, A2) const, C0 c0) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(volatile T *obj, R (T::*method)(B0, A0, A1, A2) volatile, C0 c0) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const volatile T *obj, R (T::*method)(B0, A0, A1, A2) const volatile, C0 c0) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(Callback<R(B0, A0, A1, A2)> cb, C0 c0) {
    return Binder<R(A0, A1, A2)>(cb, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(B0, B1, A0, A1, A2), C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2)>(func, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(T *obj, R (T::*method)(B0, B1, A0, A1, A2), C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const T *obj, R (T::*method)(B0, B1, A0, A1, A2) const, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2) const volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(Callback<R(B0, B1, A0, A1, A2)> cb, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2)>(cb, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(B0, B1, B2, A0, A1, A2), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2)>(func, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2) const volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(Callback<R(B0, B1, B2, A0, A1, A2)> cb, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2)>(cb, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(B0, B1, B2, B3, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2)>(func, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(Callback<R(B0, B1, B2, B3, A0, A1, A2)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2)>(cb, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(R (*func)(B0, B1, B2, B3, B4, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2)>(func, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> bind(Callback<R(B0, B1, B2, B3, B4, A0, A1, A2)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2)>(cb, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(B0, A0, A1, A2, A3), C0 c0) {
    return Binder<R(A0, A1, A2, A3)>(func, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(T *obj, R (T::*method)(B0, A0, A1, A2, A3), C0 c0) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const T *obj, R (T::*method)(B0, A0, A1, A2, A3) const, C0 c0) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) volatile, C0 c0) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3) const volatile, C0 c0) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(Callback<R(B0, A0, A1, A2, A3)> cb, C0 c0) {
    return Binder<R(A0, A1, A2, A3)>(cb, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(B0, B1, A0, A1, A2, A3), C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3)>(func, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3), C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3) const volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(Callback<R(B0, B1, A0, A1, A2, A3)> cb, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3)>(cb, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(B0, B1, B2, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3)>(func, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3) const volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(Callback<R(B0, B1, B2, A0, A1, A2, A3)> cb, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3)>(cb, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(B0, B1, B2, B3, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3)>(func, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(Callback<R(B0, B1, B2, B3, A0, A1, A2, A3)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3)>(cb, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(R (*func)(B0, B1, B2, B3, B4, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3)>(func, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> bind(Callback<R(B0, B1, B2, B3, B4, A0, A1, A2, A3)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3)>(cb, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(B0, A0, A1, A2, A3, A4), C0 c0) {
    return Binder<R(A0, A1, A2, A3, A4)>(func, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4), C0 c0) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const, C0 c0) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) volatile, C0 c0) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const volatile T *obj, R (T::*method)(B0, A0, A1, A2, A3, A4) const volatile, C0 c0) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename C0, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(Callback<R(B0, A0, A1, A2, A3, A4)> cb, C0 c0) {
    return Binder<R(A0, A1, A2, A3, A4)>(cb, c0);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(B0, B1, A0, A1, A2, A3, A4), C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3, A4)>(func, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4), C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const volatile T *obj, R (T::*method)(B0, B1, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename C0, typename C1, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(Callback<R(B0, B1, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1) {
    return Binder<R(A0, A1, A2, A3, A4)>(cb, c0, c1);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(B0, B1, B2, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3, A4)>(func, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename C0, typename C1, typename C2, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(Callback<R(B0, B1, B2, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1, C2 c2) {
    return Binder<R(A0, A1, A2, A3, A4)>(cb, c0, c1, c2);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(B0, B1, B2, B3, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3, A4)>(func, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename C0, typename C1, typename C2, typename C3, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(Callback<R(B0, B1, B2, B3, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1, C2 c2, C3 c3) {
    return Binder<R(A0, A1, A2, A3, A4)>(cb, c0, c1, c2, c3);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(R (*func)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3, A4)>(func, c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4), C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename T, typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(const volatile T *obj, R (T::*method)(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4) const volatile, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3, A4)>(callback(obj, method), c0, c1, c2, c3, c4);
}

/** Bind a function and arguments
 *  @see bind
 */
template <typename R, typename B0, typename B1, typename B2, typename B3, typename B4, typename C0, typename C1, typename C2, typename C3, typename C4, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> bind(Callback<R(B0, B1, B2, B3, B4, A0, A1, A2, A3, A4)> cb, C0 c0, C1 c1, C2 c2, C3 c3, C4 c4) {
    return Binder<R(A0, A1, A2, A3, A4)>(cb, c0, c1, c2, c3, c4);
}

}

#endif

/** @}*/
