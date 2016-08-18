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

namespace mbed {


/** Callback class based on template specialization
 *
 * @Note Synchronization level: Not protected
 */
template <typename F>
class Callback;

/** Templated function class
 */
template <typename R>
class Callback<R()> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)() = 0) {
        attach(func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R()> &func) {
        attach(func);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template<typename T>
    Callback(T *obj, R (*func)(T*)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (*func)(const T*)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (*func)(volatile T*)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (*func)(const volatile T*)) {
        attach(obj, func);
    }

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)()) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (T::*func)() const) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (T::*func)() volatile) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (T::*func)() const volatile) {
        attach(obj, func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)()) {
        struct local {
            static R _thunk(void*, void *func) {
                return (*(R (**)())func)(
                        );
            }
        };

        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &local::_thunk : 0;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R()> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*)) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (*(R (**)(T*))func)(
                        (T*)obj);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const T *obj, R (*func)(const T*)) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (*(R (**)(const T*))func)(
                        (const T*)obj);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(volatile T *obj, R (*func)(volatile T*)) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (*(R (**)(volatile T*))func)(
                        (volatile T*)obj);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const volatile T *obj, R (*func)(const volatile T*)) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (*(R (**)(const volatile T*))func)(
                        (const volatile T*)obj);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)()) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (((T*)obj)->*(*(R (T::**)())func))(
                        );
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const T *obj, R (T::*func)() const) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (((const T*)obj)->*(*(R (T::**)() const)func))(
                        );
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(volatile T *obj, R (T::*func)() volatile) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (((volatile T*)obj)->*(*(R (T::**)() volatile)func))(
                        );
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const volatile T *obj, R (T::*func)() const volatile) {
        struct local {
            static R _thunk(void *obj, void *func) {
                return (((const volatile T*)obj)->*(*(R (T::**)() const volatile)func))(
                        );
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Call the attached function
     */
    R call() {
        if (!_thunk) {
            return (R)0;
        }
        return _thunk(_obj, &_func);
    }

    /** Call the attached function
     */
    R operator()() {
        return call();
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _thunk;
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func) {
        return static_cast<Callback<R()>*>(func)->call(
                );
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)();
        void (*_boundfunc)(_class *);
        void (_class::*_methodfunc)();
    } _func;

    void *_obj;

    // Thunk registered on attach to dispatch calls
    R (*_thunk)(void*, void*);
};

/** Templated function class
 */
template <typename R, typename A0>
class Callback<R(A0)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0) = 0) {
        attach(func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0)> &func) {
        attach(func);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template<typename T>
    Callback(T *obj, R (*func)(T*, A0)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (*func)(const T*, A0)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (*func)(volatile T*, A0)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (*func)(const volatile T*, A0)) {
        attach(obj, func);
    }

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (T::*func)(A0) const) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (T::*func)(A0) volatile) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (T::*func)(A0) const volatile) {
        attach(obj, func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0)) {
        struct local {
            static R _thunk(void*, void *func, A0 a0) {
                return (*(R (**)(A0))func)(
                        a0);
            }
        };

        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &local::_thunk : 0;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (*(R (**)(T*, A0))func)(
                        (T*)obj, a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const T *obj, R (*func)(const T*, A0)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (*(R (**)(const T*, A0))func)(
                        (const T*)obj, a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(volatile T *obj, R (*func)(volatile T*, A0)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (*(R (**)(volatile T*, A0))func)(
                        (volatile T*)obj, a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const volatile T *obj, R (*func)(const volatile T*, A0)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (*(R (**)(const volatile T*, A0))func)(
                        (const volatile T*)obj, a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (((T*)obj)->*(*(R (T::**)(A0))func))(
                        a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const T *obj, R (T::*func)(A0) const) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (((const T*)obj)->*(*(R (T::**)(A0) const)func))(
                        a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(volatile T *obj, R (T::*func)(A0) volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (((volatile T*)obj)->*(*(R (T::**)(A0) volatile)func))(
                        a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const volatile T *obj, R (T::*func)(A0) const volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0) {
                return (((const volatile T*)obj)->*(*(R (T::**)(A0) const volatile)func))(
                        a0);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Call the attached function
     */
    R call(A0 a0) {
        if (!_thunk) {
            return (R)0;
        }
        return _thunk(_obj, &_func, a0);
    }

    /** Call the attached function
     */
    R operator()(A0 a0) {
        return call(a0);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _thunk;
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0) {
        return static_cast<Callback<R(A0)>*>(func)->call(
                a0);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)();
        void (*_boundfunc)(_class *);
        void (_class::*_methodfunc)();
    } _func;

    void *_obj;

    // Thunk registered on attach to dispatch calls
    R (*_thunk)(void*, void*, A0);
};

/** Templated function class
 */
template <typename R, typename A0, typename A1>
class Callback<R(A0, A1)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0, A1) = 0) {
        attach(func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1)> &func) {
        attach(func);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template<typename T>
    Callback(T *obj, R (*func)(T*, A0, A1)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (*func)(const T*, A0, A1)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (*func)(volatile T*, A0, A1)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1)) {
        attach(obj, func);
    }

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0, A1)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (T::*func)(A0, A1) const) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (T::*func)(A0, A1) volatile) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (T::*func)(A0, A1) const volatile) {
        attach(obj, func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1)) {
        struct local {
            static R _thunk(void*, void *func, A0 a0, A1 a1) {
                return (*(R (**)(A0, A1))func)(
                        a0, a1);
            }
        };

        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &local::_thunk : 0;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (*(R (**)(T*, A0, A1))func)(
                        (T*)obj, a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const T *obj, R (*func)(const T*, A0, A1)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (*(R (**)(const T*, A0, A1))func)(
                        (const T*)obj, a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(volatile T *obj, R (*func)(volatile T*, A0, A1)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (*(R (**)(volatile T*, A0, A1))func)(
                        (volatile T*)obj, a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const volatile T *obj, R (*func)(const volatile T*, A0, A1)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (*(R (**)(const volatile T*, A0, A1))func)(
                        (const volatile T*)obj, a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (((T*)obj)->*(*(R (T::**)(A0, A1))func))(
                        a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const T *obj, R (T::*func)(A0, A1) const) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (((const T*)obj)->*(*(R (T::**)(A0, A1) const)func))(
                        a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(volatile T *obj, R (T::*func)(A0, A1) volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (((volatile T*)obj)->*(*(R (T::**)(A0, A1) volatile)func))(
                        a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const volatile T *obj, R (T::*func)(A0, A1) const volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1) {
                return (((const volatile T*)obj)->*(*(R (T::**)(A0, A1) const volatile)func))(
                        a0, a1);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1) {
        if (!_thunk) {
            return (R)0;
        }
        return _thunk(_obj, &_func, a0, a1);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1) {
        return call(a0, a1);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _thunk;
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1) {
        return static_cast<Callback<R(A0, A1)>*>(func)->call(
                a0, a1);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)();
        void (*_boundfunc)(_class *);
        void (_class::*_methodfunc)();
    } _func;

    void *_obj;

    // Thunk registered on attach to dispatch calls
    R (*_thunk)(void*, void*, A0, A1);
};

/** Templated function class
 */
template <typename R, typename A0, typename A1, typename A2>
class Callback<R(A0, A1, A2)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0, A1, A2) = 0) {
        attach(func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2)> &func) {
        attach(func);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template<typename T>
    Callback(T *obj, R (*func)(T*, A0, A1, A2)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (*func)(const T*, A0, A1, A2)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (*func)(volatile T*, A0, A1, A2)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2)) {
        attach(obj, func);
    }

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0, A1, A2)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (T::*func)(A0, A1, A2) const) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (T::*func)(A0, A1, A2) volatile) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (T::*func)(A0, A1, A2) const volatile) {
        attach(obj, func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1, A2)) {
        struct local {
            static R _thunk(void*, void *func, A0 a0, A1 a1, A2 a2) {
                return (*(R (**)(A0, A1, A2))func)(
                        a0, a1, a2);
            }
        };

        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &local::_thunk : 0;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1, A2)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1, A2)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (*(R (**)(T*, A0, A1, A2))func)(
                        (T*)obj, a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const T *obj, R (*func)(const T*, A0, A1, A2)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (*(R (**)(const T*, A0, A1, A2))func)(
                        (const T*)obj, a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(volatile T *obj, R (*func)(volatile T*, A0, A1, A2)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (*(R (**)(volatile T*, A0, A1, A2))func)(
                        (volatile T*)obj, a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (*(R (**)(const volatile T*, A0, A1, A2))func)(
                        (const volatile T*)obj, a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1, A2)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (((T*)obj)->*(*(R (T::**)(A0, A1, A2))func))(
                        a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const T *obj, R (T::*func)(A0, A1, A2) const) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (((const T*)obj)->*(*(R (T::**)(A0, A1, A2) const)func))(
                        a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(volatile T *obj, R (T::*func)(A0, A1, A2) volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (((volatile T*)obj)->*(*(R (T::**)(A0, A1, A2) volatile)func))(
                        a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const volatile T *obj, R (T::*func)(A0, A1, A2) const volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
                return (((const volatile T*)obj)->*(*(R (T::**)(A0, A1, A2) const volatile)func))(
                        a0, a1, a2);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1, A2 a2) {
        if (!_thunk) {
            return (R)0;
        }
        return _thunk(_obj, &_func, a0, a1, a2);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1, A2 a2) {
        return call(a0, a1, a2);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _thunk;
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2) {
        return static_cast<Callback<R(A0, A1, A2)>*>(func)->call(
                a0, a1, a2);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)();
        void (*_boundfunc)(_class *);
        void (_class::*_methodfunc)();
    } _func;

    void *_obj;

    // Thunk registered on attach to dispatch calls
    R (*_thunk)(void*, void*, A0, A1, A2);
};

/** Templated function class
 */
template <typename R, typename A0, typename A1, typename A2, typename A3>
class Callback<R(A0, A1, A2, A3)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0, A1, A2, A3) = 0) {
        attach(func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2, A3)> &func) {
        attach(func);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template<typename T>
    Callback(T *obj, R (*func)(T*, A0, A1, A2, A3)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (*func)(const T*, A0, A1, A2, A3)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (*func)(volatile T*, A0, A1, A2, A3)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2, A3)) {
        attach(obj, func);
    }

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0, A1, A2, A3)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (T::*func)(A0, A1, A2, A3) const) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (T::*func)(A0, A1, A2, A3) volatile) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (T::*func)(A0, A1, A2, A3) const volatile) {
        attach(obj, func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1, A2, A3)) {
        struct local {
            static R _thunk(void*, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (*(R (**)(A0, A1, A2, A3))func)(
                        a0, a1, a2, a3);
            }
        };

        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &local::_thunk : 0;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1, A2, A3)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1, A2, A3)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (*(R (**)(T*, A0, A1, A2, A3))func)(
                        (T*)obj, a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const T *obj, R (*func)(const T*, A0, A1, A2, A3)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (*(R (**)(const T*, A0, A1, A2, A3))func)(
                        (const T*)obj, a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(volatile T *obj, R (*func)(volatile T*, A0, A1, A2, A3)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (*(R (**)(volatile T*, A0, A1, A2, A3))func)(
                        (volatile T*)obj, a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2, A3)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (*(R (**)(const volatile T*, A0, A1, A2, A3))func)(
                        (const volatile T*)obj, a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1, A2, A3)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (((T*)obj)->*(*(R (T::**)(A0, A1, A2, A3))func))(
                        a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const T *obj, R (T::*func)(A0, A1, A2, A3) const) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (((const T*)obj)->*(*(R (T::**)(A0, A1, A2, A3) const)func))(
                        a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(volatile T *obj, R (T::*func)(A0, A1, A2, A3) volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (((volatile T*)obj)->*(*(R (T::**)(A0, A1, A2, A3) volatile)func))(
                        a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const volatile T *obj, R (T::*func)(A0, A1, A2, A3) const volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
                return (((const volatile T*)obj)->*(*(R (T::**)(A0, A1, A2, A3) const volatile)func))(
                        a0, a1, a2, a3);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1, A2 a2, A3 a3) {
        if (!_thunk) {
            return (R)0;
        }
        return _thunk(_obj, &_func, a0, a1, a2, a3);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1, A2 a2, A3 a3) {
        return call(a0, a1, a2, a3);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _thunk;
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
        return static_cast<Callback<R(A0, A1, A2, A3)>*>(func)->call(
                a0, a1, a2, a3);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)();
        void (*_boundfunc)(_class *);
        void (_class::*_methodfunc)();
    } _func;

    void *_obj;

    // Thunk registered on attach to dispatch calls
    R (*_thunk)(void*, void*, A0, A1, A2, A3);
};

/** Templated function class
 */
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
class Callback<R(A0, A1, A2, A3, A4)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0, A1, A2, A3, A4) = 0) {
        attach(func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2, A3, A4)> &func) {
        attach(func);
    }

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template<typename T>
    Callback(T *obj, R (*func)(T*, A0, A1, A2, A3, A4)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (*func)(const T*, A0, A1, A2, A3, A4)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (*func)(volatile T*, A0, A1, A2, A3, A4)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2, A3, A4)) {
        attach(obj, func);
    }

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0, A1, A2, A3, A4)) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const T *obj, R (T::*func)(A0, A1, A2, A3, A4) const) {
        attach(obj, func);
    }

    template<typename T>
    Callback(volatile T *obj, R (T::*func)(A0, A1, A2, A3, A4) volatile) {
        attach(obj, func);
    }

    template<typename T>
    Callback(const volatile T *obj, R (T::*func)(A0, A1, A2, A3, A4) const volatile) {
        attach(obj, func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1, A2, A3, A4)) {
        struct local {
            static R _thunk(void*, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (*(R (**)(A0, A1, A2, A3, A4))func)(
                        a0, a1, a2, a3, a4);
            }
        };

        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &local::_thunk : 0;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1, A2, A3, A4)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1, A2, A3, A4)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (*(R (**)(T*, A0, A1, A2, A3, A4))func)(
                        (T*)obj, a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const T *obj, R (*func)(const T*, A0, A1, A2, A3, A4)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (*(R (**)(const T*, A0, A1, A2, A3, A4))func)(
                        (const T*)obj, a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(volatile T *obj, R (*func)(volatile T*, A0, A1, A2, A3, A4)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (*(R (**)(volatile T*, A0, A1, A2, A3, A4))func)(
                        (volatile T*)obj, a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template <typename T>
    void attach(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2, A3, A4)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (*(R (**)(const volatile T*, A0, A1, A2, A3, A4))func)(
                        (const volatile T*)obj, a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1, A2, A3, A4)) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (((T*)obj)->*(*(R (T::**)(A0, A1, A2, A3, A4))func))(
                        a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const T *obj, R (T::*func)(A0, A1, A2, A3, A4) const) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (((const T*)obj)->*(*(R (T::**)(A0, A1, A2, A3, A4) const)func))(
                        a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(volatile T *obj, R (T::*func)(A0, A1, A2, A3, A4) volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (((volatile T*)obj)->*(*(R (T::**)(A0, A1, A2, A3, A4) volatile)func))(
                        a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    template<typename T>
    void attach(const volatile T *obj, R (T::*func)(A0, A1, A2, A3, A4) const volatile) {
        struct local {
            static R _thunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
                return (((const volatile T*)obj)->*(*(R (T::**)(A0, A1, A2, A3, A4) const volatile)func))(
                        a0, a1, a2, a3, a4);
            }
        };

        _obj = (void*)obj;
        memcpy(&_func, &func, sizeof func);
        _thunk = &local::_thunk;
    }

    /** Call the attached function
     */
    R call(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        if (!_thunk) {
            return (R)0;
        }
        return _thunk(_obj, &_func, a0, a1, a2, a3, a4);
    }

    /** Call the attached function
     */
    R operator()(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return call(a0, a1, a2, a3, a4);
    }

    /** Test if function has been attached
     */
    operator bool() const {
        return _thunk;
    }

    /** Static thunk for passing as C-style function
     *  @param func Callback to call passed as void pointer
     */
    static R thunk(void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return static_cast<Callback<R(A0, A1, A2, A3, A4)>*>(func)->call(
                a0, a1, a2, a3, a4);
    }

private:
    // Stored as pointer to function and pointer to optional object
    // Function pointer is stored as union of possible function types
    // to garuntee proper size and alignment
    struct _class;
    union {
        void (*_staticfunc)();
        void (*_boundfunc)(_class *);
        void (_class::*_methodfunc)();
    } _func;

    void *_obj;

    // Thunk registered on attach to dispatch calls
    R (*_thunk)(void*, void*, A0, A1, A2, A3, A4);
};

typedef Callback<void(int)> event_callback_t;


/** Create a callback class with type infered from the arguments
 *
 *  @param obj  Optional pointer to object to bind to function
 *  @param func Static function to attach
 *  @return     Callback with type infered from arguments
 */
template <typename R>
Callback<R()> callback(R (*func)() = 0) {
    return Callback<R()>(func);
}

template <typename R>
Callback<R()> callback(const Callback<R()> &func) {
    return Callback<R()>(func);
}
template <typename T, typename R>
Callback<R()> callback(T *obj, R (*func)(T*)) {
    return Callback<R()>(obj, func);
}

template <typename T, typename R>
Callback<R()> callback(const T *obj, R (*func)(const T*)) {
    return Callback<R()>(obj, func);
}

template <typename T, typename R>
Callback<R()> callback(volatile T *obj, R (*func)(volatile T*)) {
    return Callback<R()>(obj, func);
}

template <typename T, typename R>
Callback<R()> callback(const volatile T *obj, R (*func)(const volatile T*)) {
    return Callback<R()>(obj, func);
}

template<typename T, typename R>
Callback<R()> callback(T *obj, R (T::*func)()) {
    return Callback<R()>(obj, func);
}

template<typename T, typename R>
Callback<R()> callback(const T *obj, R (T::*func)() const) {
    return Callback<R()>(obj, func);
}

template<typename T, typename R>
Callback<R()> callback(volatile T *obj, R (T::*func)() volatile) {
    return Callback<R()>(obj, func);
}

template<typename T, typename R>
Callback<R()> callback(const volatile T *obj, R (T::*func)() const volatile) {
    return Callback<R()>(obj, func);
}

template <typename R, typename A0>
Callback<R(A0)> callback(R (*func)(A0) = 0) {
    return Callback<R(A0)>(func);
}

template <typename R, typename A0>
Callback<R(A0)> callback(const Callback<R(A0)> &func) {
    return Callback<R(A0)>(func);
}
template <typename T, typename R, typename A0>
Callback<R(A0)> callback(T *obj, R (*func)(T*, A0)) {
    return Callback<R(A0)>(obj, func);
}

template <typename T, typename R, typename A0>
Callback<R(A0)> callback(const T *obj, R (*func)(const T*, A0)) {
    return Callback<R(A0)>(obj, func);
}

template <typename T, typename R, typename A0>
Callback<R(A0)> callback(volatile T *obj, R (*func)(volatile T*, A0)) {
    return Callback<R(A0)>(obj, func);
}

template <typename T, typename R, typename A0>
Callback<R(A0)> callback(const volatile T *obj, R (*func)(const volatile T*, A0)) {
    return Callback<R(A0)>(obj, func);
}

template<typename T, typename R, typename A0>
Callback<R(A0)> callback(T *obj, R (T::*func)(A0)) {
    return Callback<R(A0)>(obj, func);
}

template<typename T, typename R, typename A0>
Callback<R(A0)> callback(const T *obj, R (T::*func)(A0) const) {
    return Callback<R(A0)>(obj, func);
}

template<typename T, typename R, typename A0>
Callback<R(A0)> callback(volatile T *obj, R (T::*func)(A0) volatile) {
    return Callback<R(A0)>(obj, func);
}

template<typename T, typename R, typename A0>
Callback<R(A0)> callback(const volatile T *obj, R (T::*func)(A0) const volatile) {
    return Callback<R(A0)>(obj, func);
}

template <typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(R (*func)(A0, A1) = 0) {
    return Callback<R(A0, A1)>(func);
}

template <typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const Callback<R(A0, A1)> &func) {
    return Callback<R(A0, A1)>(func);
}
template <typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(T *obj, R (*func)(T*, A0, A1)) {
    return Callback<R(A0, A1)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const T *obj, R (*func)(const T*, A0, A1)) {
    return Callback<R(A0, A1)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(volatile T *obj, R (*func)(volatile T*, A0, A1)) {
    return Callback<R(A0, A1)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1)) {
    return Callback<R(A0, A1)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(T *obj, R (T::*func)(A0, A1)) {
    return Callback<R(A0, A1)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const T *obj, R (T::*func)(A0, A1) const) {
    return Callback<R(A0, A1)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(volatile T *obj, R (T::*func)(A0, A1) volatile) {
    return Callback<R(A0, A1)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1>
Callback<R(A0, A1)> callback(const volatile T *obj, R (T::*func)(A0, A1) const volatile) {
    return Callback<R(A0, A1)>(obj, func);
}

template <typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(R (*func)(A0, A1, A2) = 0) {
    return Callback<R(A0, A1, A2)>(func);
}

template <typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const Callback<R(A0, A1, A2)> &func) {
    return Callback<R(A0, A1, A2)>(func);
}
template <typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(T *obj, R (*func)(T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const T *obj, R (*func)(const T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(volatile T *obj, R (*func)(volatile T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(T *obj, R (T::*func)(A0, A1, A2)) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const T *obj, R (T::*func)(A0, A1, A2) const) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(volatile T *obj, R (T::*func)(A0, A1, A2) volatile) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2>
Callback<R(A0, A1, A2)> callback(const volatile T *obj, R (T::*func)(A0, A1, A2) const volatile) {
    return Callback<R(A0, A1, A2)>(obj, func);
}

template <typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(R (*func)(A0, A1, A2, A3) = 0) {
    return Callback<R(A0, A1, A2, A3)>(func);
}

template <typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const Callback<R(A0, A1, A2, A3)> &func) {
    return Callback<R(A0, A1, A2, A3)>(func);
}
template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(T *obj, R (*func)(T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const T *obj, R (*func)(const T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(volatile T *obj, R (*func)(volatile T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(T *obj, R (T::*func)(A0, A1, A2, A3)) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const T *obj, R (T::*func)(A0, A1, A2, A3) const) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(volatile T *obj, R (T::*func)(A0, A1, A2, A3) volatile) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3>
Callback<R(A0, A1, A2, A3)> callback(const volatile T *obj, R (T::*func)(A0, A1, A2, A3) const volatile) {
    return Callback<R(A0, A1, A2, A3)>(obj, func);
}

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(R (*func)(A0, A1, A2, A3, A4) = 0) {
    return Callback<R(A0, A1, A2, A3, A4)>(func);
}

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const Callback<R(A0, A1, A2, A3, A4)> &func) {
    return Callback<R(A0, A1, A2, A3, A4)>(func);
}
template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(T *obj, R (*func)(T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const T *obj, R (*func)(const T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(volatile T *obj, R (*func)(volatile T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}

template <typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const volatile T *obj, R (*func)(const volatile T*, A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(T *obj, R (T::*func)(A0, A1, A2, A3, A4)) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const T *obj, R (T::*func)(A0, A1, A2, A3, A4) const) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(volatile T *obj, R (T::*func)(A0, A1, A2, A3, A4) volatile) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}

template<typename T, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
Callback<R(A0, A1, A2, A3, A4)> callback(const volatile T *obj, R (T::*func)(A0, A1, A2, A3, A4) const volatile) {
    return Callback<R(A0, A1, A2, A3, A4)>(obj, func);
}


} // namespace mbed

#endif
