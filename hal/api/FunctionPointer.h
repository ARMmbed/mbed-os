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
#ifndef MBED_FUNCTIONPOINTER_H
#define MBED_FUNCTIONPOINTER_H

#include <string.h>
#include <stdint.h>

namespace mbed {

// Reusable FuncPtr class based on template specialization
template <typename F>
class FuncPtr;

/** A class for storing and calling a pointer to a static or member function
 */
template <typename R, typename A1, typename A2, typename A3, typename A4>
class FuncPtr<R(A1, A2, A3, A4)> {
public:
    /** Create a FuncPtr, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FuncPtr(R (*function)(A1, A2, A3, A4) = 0) {
        attach(function);
    }

    /** Create a FuncPtr, attaching a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (*function)(T*, A1, A2, A3, A4)) {
        attach(object, function);
    }

    /** Create a FuncPtr, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (T::*member)(A1, A2, A3, A4)) {
        attach(object, member);
    }

    /** Create a FuncPtr, attaching a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    FuncPtr(T *object) {
        attach(object, &T::operator());
    }

    /** Create a FuncPtr from another FuncPtr
     *
     *  @param func The func to attach
     */
    FuncPtr(const FuncPtr<R(A1, A2, A3, A4)> &func) {
        attach(func);
    }

    /** Attach a static function
     *
     *  @param function The static function to attach (default is none)
     */
    void attach(R (*function)(A1, A2, A3, A4)) {
        _object = 0;
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::staticthunk;
    }

    /** Attach a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template <typename T>
    void attach(T *object, R (*function)(T*, A1, A2, A3, A4)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::boundthunk<T>;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*method)(A1, A2, A3, A4)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &method, sizeof method);
        _thunk = &FuncPtr::methodthunk<T>;
    }

    /** Attach a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    void attach(T *object) {
        attach(object, &T::operator());
    }

    /** Attach a FuncPtr
     *
     *  @param func The func to attach
     */
    void attach(const FuncPtr<R(A1, A2, A3, A4)> &func) {
        _object = func._object;
        memcpy(&_function, &func._function, sizeof _function);
        _thunk = func._thunk;
    }

    /** Call the attached static or member function
     */
    R call(A1 a1, A2 a2, A3 a3, A4 a4) {
        return _thunk(_object, _function, a1, a2, a3, a4);
    }

    /** Get registered static function
     */
    R (*get_function(A1, A2, A3, A4))() {
        return reinterpret_cast<R (*)(A1, A2, A3, A4)>(_object ? 0 : _function);
    }

#ifdef MBED_OPERATORS
    R operator ()(A1 a1, A2 a2, A3 a3, A4 a4) {
        return call(a1, a2, a3, a4);
    }
    operator bool(void) const {
        return static_cast<bool>(_function);
    }
#endif
private:
    // Static thunks for various function types
    static R staticthunk(void*, void *func, A1 a1, A2 a2, A3 a3, A4 a4) {
        R (*f)(A1, A2, A3, A4) = *reinterpret_cast<R (**)(A1, A2, A3, A4)>(func);
        return f(a1, a2, a3, a4);
    }

    template<typename T>
    static R boundthunk(void *object, void *func, A1 a1, A2 a2, A3 a3, A4 a4) {
        T *o = static_cast<T*>(object);
        R (*f)(T*, A1, A2, A3, A4) = *reinterpret_cast<R (**)(T*, A1, A2, A3, A4)>(func);
        return f(o, a1);
    }

    template<typename T>
    static R methodthunk(void *object, void *member, A1 a1, A2 a2, A3 a3, A4 a4) {
        T* o = static_cast<T*>(object);
        R (T::*m)(A1, A2, A3, A4) = *reinterpret_cast<R (T::**)(A1, A2, A3, A4)>(member);
        return (o->*m)(a1, a2, a3, a4);
    }

    // Forward declaration of an unknown class
    // this kind of declaration is authorized by the standard (see 8.3.3/2 of C++ 03 standard).  
    // As a result, the compiler will allocate for UnknownFunctionMember_t the biggest size 
    // and biggest alignment possible for member function. 
    // This type can be used inside unions, it will help to provide the storage 
    // with the proper size and alignment guarantees 
    class UnknownClass;

    // object this pointer
    void *_object;

    // aligned raw member function pointer storage - converted back by registered thunk
    char _function[sizeof(void (UnknownClass::*)())];

    // registered function to convert back and call _m.member on _object
    R (*_thunk)(void*, void*, A1, A2, A3, A4); 
};

/** A class for storing and calling a pointer to a static or member function
 */
template <typename R, typename A1, typename A2, typename A3>
class FuncPtr<R(A1, A2, A3)> {
public:
    /** Create a FuncPtr, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FuncPtr(R (*function)(A1, A2, A3) = 0) {
        attach(function);
    }

    /** Create a FuncPtr, attaching a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (*function)(T*, A1, A2, A3)) {
        attach(object, function);
    }

    /** Create a FuncPtr, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (T::*member)(A1, A2, A3)) {
        attach(object, member);
    }

    /** Create a FuncPtr, attaching a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    FuncPtr(T *object) {
        attach(object, &T::operator());
    }

    /** Create a FuncPtr from another FuncPtr
     *
     *  @param func The func to attach
     */
    FuncPtr(const FuncPtr<R(A1, A2, A3)> &func) {
        attach(func);
    }

    /** Attach a static function
     *
     *  @param function The static function to attach (default is none)
     */
    void attach(R (*function)(A1, A2, A3)) {
        _object = 0;
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::staticthunk;
    }

    /** Attach a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template <typename T>
    void attach(T *object, R (*function)(T*, A1, A2, A3)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::boundthunk<T>;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*method)(A1, A2, A3)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &method, sizeof method);
        _thunk = &FuncPtr::methodthunk<T>;
    }

    /** Attach a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    void attach(T *object) {
        attach(object, &T::operator());
    }

    /** Attach a FuncPtr
     *
     *  @param func The func to attach
     */
    void attach(const FuncPtr<R(A1, A2, A3)> &func) {
        _object = func._object;
        memcpy(&_function, &func._function, sizeof _function);
        _thunk = func._thunk;
    }

    /** Call the attached static or member function
     */
    R call(A1 a1, A2 a2, A3 a3) {
        return _thunk(_object, _function, a1, a2, a3);
    }

    /** Get registered static function
     */
    R (*get_function(A1, A2, A3))() {
        return reinterpret_cast<R (*)(A1, A2, A3)>(_object ? 0 : _function);
    }

#ifdef MBED_OPERATORS
    R operator ()(A1 a1, A2 a2, A3 a3) {
        return call(a1, a2, a3);
    }
    operator bool(void) const {
        return static_cast<bool>(_function);
    }
#endif
private:
    // Static thunks for various function types
    static R staticthunk(void*, void *func, A1 a1, A2 a2, A3 a3) {
        R (*f)(A1, A2, A3) = *reinterpret_cast<R (**)(A1, A2, A3)>(func);
        return f(a1, a2, a3);
    }

    template<typename T>
    static R boundthunk(void *object, void *func, A1 a1, A2 a2, A3 a3) {
        T *o = static_cast<T*>(object);
        R (*f)(T*, A1, A2, A3) = *reinterpret_cast<R (**)(T*, A1, A2, A3)>(func);
        return f(o, a1);
    }

    template<typename T>
    static R methodthunk(void *object, void *member, A1 a1, A2 a2, A3 a3) {
        T* o = static_cast<T*>(object);
        R (T::*m)(A1, A2, A3) = *reinterpret_cast<R (T::**)(A1, A2, A3)>(member);
        return (o->*m)(a1, a2, a3);
    }

    // Forward declaration of an unknown class
    // this kind of declaration is authorized by the standard (see 8.3.3/2 of C++ 03 standard).  
    // As a result, the compiler will allocate for UnknownFunctionMember_t the biggest size 
    // and biggest alignment possible for member function. 
    // This type can be used inside unions, it will help to provide the storage 
    // with the proper size and alignment guarantees 
    class UnknownClass;

    // object this pointer
    void *_object;

    // aligned raw member function pointer storage - converted back by registered thunk
    char _function[sizeof(void (UnknownClass::*)())];

    // registered function to convert back and call _m.member on _object
    R (*_thunk)(void*, void*, A1, A2, A3); 
};

/** A class for storing and calling a pointer to a static or member function
 */
template <typename R, typename A1, typename A2>
class FuncPtr<R(A1, A2)> {
public:
    /** Create a FuncPtr, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FuncPtr(R (*function)(A1, A2) = 0) {
        attach(function);
    }

    /** Create a FuncPtr, attaching a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (*function)(T*, A1, A2)) {
        attach(object, function);
    }

    /** Create a FuncPtr, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (T::*member)(A1, A2)) {
        attach(object, member);
    }

    /** Create a FuncPtr, attaching a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    FuncPtr(T *object) {
        attach(object, &T::operator());
    }

    /** Create a FuncPtr from another FuncPtr
     *
     *  @param func The func to attach
     */
    FuncPtr(const FuncPtr<R(A1, A2)> &func) {
        attach(func);
    }

    /** Attach a static function
     *
     *  @param function The static function to attach (default is none)
     */
    void attach(R (*function)(A1, A2)) {
        _object = 0;
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::staticthunk;
    }

    /** Attach a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template <typename T>
    void attach(T *object, R (*function)(T*, A1, A2)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::boundthunk<T>;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*method)(A1, A2)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &method, sizeof method);
        _thunk = &FuncPtr::methodthunk<T>;
    }

    /** Attach a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    void attach(T *object) {
        attach(object, &T::operator());
    }

    /** Attach a FuncPtr
     *
     *  @param func The func to attach
     */
    void attach(const FuncPtr<R(A1, A2)> &func) {
        _object = func._object;
        memcpy(&_function, &func._function, sizeof _function);
        _thunk = func._thunk;
    }

    /** Call the attached static or member function
     */
    R call(A1 a1, A2 a2) {
        return _thunk(_object, _function, a1, a2);
    }

    /** Get registered static function
     */
    R (*get_function(A1, A2))() {
        return reinterpret_cast<R (*)(A1, A2)>(_object ? 0 : _function);
    }

#ifdef MBED_OPERATORS
    R operator ()(A1 a1, A2 a2) {
        return call(a1, a2);
    }
    operator bool(void) const {
        return static_cast<bool>(_function);
    }
#endif
private:
    // Static thunks for various function types
    static R staticthunk(void*, void *func, A1 a1, A2 a2) {
        R (*f)(A1, A2) = *reinterpret_cast<R (**)(A1, A2)>(func);
        return f(a1, a2);
    }

    template<typename T>
    static R boundthunk(void *object, void *func, A1 a1, A2 a2) {
        T *o = static_cast<T*>(object);
        R (*f)(T*, A1, A2) = *reinterpret_cast<R (**)(T*, A1, A2)>(func);
        return f(o, a1);
    }

    template<typename T>
    static R methodthunk(void *object, void *member, A1 a1, A2 a2) {
        T* o = static_cast<T*>(object);
        R (T::*m)(A1, A2) = *reinterpret_cast<R (T::**)(A1, A2)>(member);
        return (o->*m)(a1, a2);
    }

    // Forward declaration of an unknown class
    // this kind of declaration is authorized by the standard (see 8.3.3/2 of C++ 03 standard).  
    // As a result, the compiler will allocate for UnknownFunctionMember_t the biggest size 
    // and biggest alignment possible for member function. 
    // This type can be used inside unions, it will help to provide the storage 
    // with the proper size and alignment guarantees 
    class UnknownClass;

    // object this pointer
    void *_object;

    // aligned raw member function pointer storage - converted back by registered thunk
    char _function[sizeof(void (UnknownClass::*)())];

    // registered function to convert back and call _m.member on _object
    R (*_thunk)(void*, void*, A1, A2); 
};

/** A class for storing and calling a pointer to a static or member function
 */
template <typename R, typename A1>
class FuncPtr<R(A1)> {
public:
    /** Create a FuncPtr, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FuncPtr(R (*function)(A1) = 0) {
        attach(function);
    }

    /** Create a FuncPtr, attaching a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (*function)(T*, A1)) {
        attach(object, function);
    }

    /** Create a FuncPtr, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (T::*member)(A1)) {
        attach(object, member);
    }

    /** Create a FuncPtr, attaching a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    FuncPtr(T *object) {
        attach(object, &T::operator());
    }

    /** Create a FuncPtr from another FuncPtr
     *
     *  @param func The func to attach
     */
    FuncPtr(const FuncPtr<R(A1)> &func) {
        attach(func);
    }

    /** Attach a static function
     *
     *  @param function The static function to attach (default is none)
     */
    void attach(R (*function)(A1)) {
        _object = 0;
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::staticthunk;
    }

    /** Attach a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template <typename T>
    void attach(T *object, R (*function)(T*, A1)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::boundthunk<T>;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*method)(A1)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &method, sizeof method);
        _thunk = &FuncPtr::methodthunk<T>;
    }

    /** Attach a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    void attach(T *object) {
        attach(object, &T::operator());
    }

    /** Attach a FuncPtr
     *
     *  @param func The func to attach
     */
    void attach(const FuncPtr<R(A1)> &func) {
        _object = func._object;
        memcpy(&_function, &func._function, sizeof _function);
        _thunk = func._thunk;
    }

    /** Call the attached static or member function
     */
    R call(A1 a1) {
        return _thunk(_object, _function, a1);
    }

    /** Get registered static function
     */
    R (*get_function(A1))() {
        return reinterpret_cast<R (*)(A1)>(_object ? 0 : _function);
    }

#ifdef MBED_OPERATORS
    R operator ()(A1 a1) {
        return call(a1);
    }
    operator bool(void) const {
        return static_cast<bool>(_function);
    }
#endif
private:
    // Static thunks for various function types
    static R staticthunk(void*, void *func, A1 a1) {
        R (*f)(A1) = *reinterpret_cast<R (**)(A1)>(func);
        return f(a1);
    }

    template<typename T>
    static R boundthunk(void *object, void *func, A1 a1) {
        T *o = static_cast<T*>(object);
        R (*f)(T*, A1) = *reinterpret_cast<R (**)(T*, A1)>(func);
        return f(o, a1);
    }

    template<typename T>
    static R methodthunk(void *object, void *member, A1 a1) {
        T* o = static_cast<T*>(object);
        R (T::*m)(A1) = *reinterpret_cast<R (T::**)(A1)>(member);
        return (o->*m)(a1);
    }

    // Forward declaration of an unknown class
    // this kind of declaration is authorized by the standard (see 8.3.3/2 of C++ 03 standard).  
    // As a result, the compiler will allocate for UnknownFunctionMember_t the biggest size 
    // and biggest alignment possible for member function. 
    // This type can be used inside unions, it will help to provide the storage 
    // with the proper size and alignment guarantees 
    class UnknownClass;

    // object this pointer
    void *_object;

    // aligned raw member function pointer storage - converted back by registered thunk
    char _function[sizeof(void (UnknownClass::*)())];

    // registered function to convert back and call _m.member on _object
    R (*_thunk)(void*, void*, A1); 
};

/** A class for storing and calling a pointer to a static or member function
 */
template <typename R>
class FuncPtr<R()> {
public:
    /** Create a FuncPtr, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FuncPtr(R (*function)() = 0) {
        attach(function);
    }

    /** Create a FuncPtr, attaching a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (*function)(T*)) {
        attach(object, function);
    }

    /** Create a FuncPtr, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    FuncPtr(T *object, R (T::*member)()) {
        attach(object, member);
    }

    /** Create a FuncPtr, attaching a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    FuncPtr(T *object) {
        attach(object, &T::operator());
    }

    /** Create a FuncPtr from another FuncPtr
     *
     *  @param func The func to attach
     */
    FuncPtr(const FuncPtr<R()> &func) {
        attach(func);
    }

    /** Attach a static function
     *
     *  @param function The static function to attach (default is none)
     */
    void attach(R (*function)()) {
        _object = 0;
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::staticthunk;
    }

    /** Attach a static function with bound pointer
     *
     *  @param object Pointer to object to bind to function
     *  @param function The static function to attach
     */
    template <typename T>
    void attach(T *object, R (*function)(T*)) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &function, sizeof function);
        _thunk = &FuncPtr::boundthunk<T>;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*method)()) {
        _object = static_cast<void*>(object);
        memcpy(&_function, &method, sizeof method);
        _thunk = &FuncPtr::methodthunk<T>;
    }

    /** Attach a function object
     *
     *  @param object Pointer to a function object to attach
     */
    template<typename T>
    void attach(T *object) {
        attach(object, &T::operator());
    }

    /** Attach a FuncPtr
     *
     *  @param func The func to attach
     */
    void attach(const FuncPtr<R()> &func) {
        _object = func._object;
        memcpy(&_function, &func._function, sizeof _function);
        _thunk = func._thunk;
    }

    /** Call the attached static or member function
     */
    R call() {
        return _thunk(_object, _function);
    }

    /** Get registered static function
     */
    R (*get_function())() {
        return reinterpret_cast<R (*)()>(_object ? 0 : _function);
    }

#ifdef MBED_OPERATORS
    R operator ()() {
        return call();
    }
    operator bool(void) const {
        return static_cast<bool>(_function);
    }
#endif
private:
    // Static thunks for various function types
    static R staticthunk(void*, void *func) {
        R (*f)() = *reinterpret_cast<R (**)()>(func);
        return f();
    }

    template<typename T>
    static R boundthunk(void *object, void *func) {
        T *o = static_cast<T*>(object);
        R (*f)(T*) = *reinterpret_cast<R (**)(T*)>(func);
        return f(o);
    }

    template<typename T>
    static R methodthunk(void *object, void *member) {
        T* o = static_cast<T*>(object);
        R (T::*m)() = *reinterpret_cast<R (T::**)()>(member);
        return (o->*m)();
    }

    // Forward declaration of an unknown class
    // this kind of declaration is authorized by the standard (see 8.3.3/2 of C++ 03 standard).  
    // As a result, the compiler will allocate for UnknownFunctionMember_t the biggest size 
    // and biggest alignment possible for member function. 
    // This type can be used inside unions, it will help to provide the storage 
    // with the proper size and alignment guarantees 
    class UnknownClass;

    // object this pointer
    void *_object;

    // aligned raw member function pointer storage - converted back by registered thunk
    union {
        char _function[sizeof(void (UnknownClass::*)())];
        void (UnknownClass::*_unknownMethod)();
    };

    // registered function to convert back and call _m.member on _object
    R (*_thunk)(void*, void*); 
};

// Overloads for backwards compatibility
typedef FuncPtr<void()> FunctionPointer;
typedef FuncPtr<void(int)> event_callback_t;

} // namespace mbed

#endif
