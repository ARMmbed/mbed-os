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
template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
class Callback<R(A0, A1, A2, A3, A4)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0, A1, A2, A3, A4) = 0) {
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

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0, A1, A2, A3, A4)) {
        attach(obj, func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2, A3, A4)> &func) {
        attach(func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1, A2, A3, A4)) {
        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &Callback::_staticthunk : 0;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1, A2, A3, A4)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_boundthunk<T>;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1, A2, A3, A4)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_methodthunk<T>;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1, A2, A3, A4)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
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
        return static_cast<Callback<R(A0, A1, A2, A3, A4)>*>(func)
                ->call(a0, a1, a2, a3, a4);
    }

private:
    // Internal thunks for various function types
    static R _staticthunk(void*, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return (*reinterpret_cast<R (**)(A0, A1, A2, A3, A4)>(func))
                (a0, a1, a2, a3, a4);
    }

    template<typename T>
    static R _boundthunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return (*reinterpret_cast<R (**)(T*, A0, A1, A2, A3, A4)>(func))
                (static_cast<T*>(obj), a0, a1, a2, a3, a4);
    }

    template<typename T>
    static R _methodthunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        return (static_cast<T*>(obj)->*
                (*reinterpret_cast<R (T::**)(A0, A1, A2, A3, A4)>(func)))
                (a0, a1, a2, a3, a4);
    }

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

    /** Create a Callback with a static function and bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template<typename T>
    Callback(T *obj, R (*func)(T*, A0, A1, A2, A3)) {
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

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2, A3)> &func) {
        attach(func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1, A2, A3)) {
        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &Callback::_staticthunk : 0;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1, A2, A3)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_boundthunk<T>;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1, A2, A3)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_methodthunk<T>;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1, A2, A3)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
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
        return static_cast<Callback<R(A0, A1, A2, A3)>*>(func)
                ->call(a0, a1, a2, a3);
    }

private:
    // Internal thunks for various function types
    static R _staticthunk(void*, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
        return (*reinterpret_cast<R (**)(A0, A1, A2, A3)>(func))
                (a0, a1, a2, a3);
    }

    template<typename T>
    static R _boundthunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
        return (*reinterpret_cast<R (**)(T*, A0, A1, A2, A3)>(func))
                (static_cast<T*>(obj), a0, a1, a2, a3);
    }

    template<typename T>
    static R _methodthunk(void *obj, void *func, A0 a0, A1 a1, A2 a2, A3 a3) {
        return (static_cast<T*>(obj)->*
                (*reinterpret_cast<R (T::**)(A0, A1, A2, A3)>(func)))
                (a0, a1, a2, a3);
    }

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
template <typename R, typename A0, typename A1, typename A2>
class Callback<R(A0, A1, A2)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0, A1, A2) = 0) {
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

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0, A1, A2)) {
        attach(obj, func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1, A2)> &func) {
        attach(func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1, A2)) {
        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &Callback::_staticthunk : 0;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1, A2)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_boundthunk<T>;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1, A2)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_methodthunk<T>;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1, A2)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
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
        return static_cast<Callback<R(A0, A1, A2)>*>(func)
                ->call(a0, a1, a2);
    }

private:
    // Internal thunks for various function types
    static R _staticthunk(void*, void *func, A0 a0, A1 a1, A2 a2) {
        return (*reinterpret_cast<R (**)(A0, A1, A2)>(func))
                (a0, a1, a2);
    }

    template<typename T>
    static R _boundthunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
        return (*reinterpret_cast<R (**)(T*, A0, A1, A2)>(func))
                (static_cast<T*>(obj), a0, a1, a2);
    }

    template<typename T>
    static R _methodthunk(void *obj, void *func, A0 a0, A1 a1, A2 a2) {
        return (static_cast<T*>(obj)->*
                (*reinterpret_cast<R (T::**)(A0, A1, A2)>(func)))
                (a0, a1, a2);
    }

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
template <typename R, typename A0, typename A1>
class Callback<R(A0, A1)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0, A1) = 0) {
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

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0, A1)) {
        attach(obj, func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0, A1)> &func) {
        attach(func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0, A1)) {
        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &Callback::_staticthunk : 0;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0, A1)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_boundthunk<T>;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0, A1)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_methodthunk<T>;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0, A1)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
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
        return static_cast<Callback<R(A0, A1)>*>(func)
                ->call(a0, a1);
    }

private:
    // Internal thunks for various function types
    static R _staticthunk(void*, void *func, A0 a0, A1 a1) {
        return (*reinterpret_cast<R (**)(A0, A1)>(func))
                (a0, a1);
    }

    template<typename T>
    static R _boundthunk(void *obj, void *func, A0 a0, A1 a1) {
        return (*reinterpret_cast<R (**)(T*, A0, A1)>(func))
                (static_cast<T*>(obj), a0, a1);
    }

    template<typename T>
    static R _methodthunk(void *obj, void *func, A0 a0, A1 a1) {
        return (static_cast<T*>(obj)->*
                (*reinterpret_cast<R (T::**)(A0, A1)>(func)))
                (a0, a1);
    }

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
template <typename R, typename A0>
class Callback<R(A0)> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)(A0) = 0) {
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

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)(A0)) {
        attach(obj, func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R(A0)> &func) {
        attach(func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)(A0)) {
        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &Callback::_staticthunk : 0;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*, A0)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_boundthunk<T>;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)(A0)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_methodthunk<T>;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R(A0)> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
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
        return static_cast<Callback<R(A0)>*>(func)
                ->call(a0);
    }

private:
    // Internal thunks for various function types
    static R _staticthunk(void*, void *func, A0 a0) {
        return (*reinterpret_cast<R (**)(A0)>(func))
                (a0);
    }

    template<typename T>
    static R _boundthunk(void *obj, void *func, A0 a0) {
        return (*reinterpret_cast<R (**)(T*, A0)>(func))
                (static_cast<T*>(obj), a0);
    }

    template<typename T>
    static R _methodthunk(void *obj, void *func, A0 a0) {
        return (static_cast<T*>(obj)->*
                (*reinterpret_cast<R (T::**)(A0)>(func)))
                (a0);
    }

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
template <typename R>
class Callback<R()> {
public:
    /** Create a Callback with a static function
     *  @param func Static function to attach
     */
    Callback(R (*func)() = 0) {
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

    /** Create a Callback with a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    Callback(T *obj, R (T::*func)()) {
        attach(obj, func);
    }

    /** Create a Callback with another Callback
     *  @param func Callback to attach
     */
    Callback(const Callback<R()> &func) {
        attach(func);
    }

    /** Attach a static function
     *  @param func Static function to attach
     */
    void attach(R (*func)()) {
        memcpy(&_func, &func, sizeof func);
        _thunk = func ? &Callback::_staticthunk : 0;
    }

    /** Attach a static function with a bound pointer
     *  @param obj  Pointer to object to bind to function
     *  @param func Static function to attach
     */
    template <typename T>
    void attach(T *obj, R (*func)(T*)) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_boundthunk<T>;
    }

    /** Attach a member function
     *  @param obj  Pointer to object to invoke member function on
     *  @param func Member function to attach
     */
    template<typename T>
    void attach(T *obj, R (T::*func)()) {
        _obj = static_cast<void*>(obj);
        memcpy(&_func, &func, sizeof func);
        _thunk = &Callback::_methodthunk<T>;
    }

    /** Attach a Callback
     *  @param func The Callback to attach
     */
    void attach(const Callback<R()> &func) {
        _obj = func._obj;
        memcpy(&_func, &func._func, sizeof _func);
        _thunk = func._thunk;
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
        return static_cast<Callback<R()>*>(func)
                ->call();
    }

private:
    // Internal thunks for various function types
    static R _staticthunk(void*, void *func) {
        return (*reinterpret_cast<R (**)()>(func))
                ();
    }

    template<typename T>
    static R _boundthunk(void *obj, void *func) {
        return (*reinterpret_cast<R (**)(T*)>(func))
                (static_cast<T*>(obj));
    }

    template<typename T>
    static R _methodthunk(void *obj, void *func) {
        return (static_cast<T*>(obj)->*
                (*reinterpret_cast<R (T::**)()>(func)))
                ();
    }

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

 typedef Callback<void(int)> event_callback_t;


} // namespace mbed

#endif
