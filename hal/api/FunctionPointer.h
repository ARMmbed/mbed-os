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

/* If we had variaditic templates, this wouldn't be a problem, but until C++11 is enabled, we are stuck with multiple classes... */

/** A class for storing and calling a pointer to a static or member function
 */
template <typename R, typename A1>
class FunctionPointerArg1{
public:
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FunctionPointerArg1(R (*function)(A1) = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    FunctionPointerArg1(T *object, R (T::*member)(A1)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The static function to attach (default is none)
     */
    void attach(R (*function)(A1)) {
        _p.function = function;
        _membercaller = 0;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(A1)) {
        _p.object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(A1)>(_member) = member;
        _membercaller = &FunctionPointerArg1::membercaller<T>;
    }

    /** Call the attached static or member function
     */
    R call(A1 a) {
        if (_membercaller == 0 && _p.function) {
           return _p.function(a);
        } else if (_membercaller && _p.object) {
           return _membercaller(_p.object, _member, a);
        }
        return (R)0;
    }

    /** Get registered static function
     */
    R(*get_function(A1))() {
        return _membercaller ? (R(*)(A1))0 : (R(*)(A1))_p.function;
    }

#ifdef MBED_OPERATORS
    R operator ()(A1 a) {
        return call(a);
    }
    operator bool(void) const {
        return (_membercaller != NULL ? _p.object : (void*)_p.function) != NULL;
    }
#endif
private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member, A1 a) {
        T* o = static_cast<T*>(object);
        R (T::**m)(A1) = reinterpret_cast<R (T::**)(A1)>(member);
        return (o->**m)(a);
    }

    union {
        R (*function)(A1); // static function pointer
        void *object;      // object this pointer
    } _p;
    uintptr_t _member[4]; // aligned raw member function pointer storage - converted back by registered _membercaller
    R (*_membercaller)(void*, uintptr_t*, A1); // registered membercaller function to convert back and call _m.member on _object
};

/** A class for storing and calling a pointer to a static or member function (R ()(void))
 */
template <typename R>
class FunctionPointerArg1<R, void>{
public:
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The static function to attach (default is none)
     */
    FunctionPointerArg1(R (*function)(void) = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointerArg1(T *object, R (T::*member)(void)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(R (*function)(void)) {
        _p.function = function;
        _membercaller = 0;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(void)) {
        _p.object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(void)>(_member) = member;
        _membercaller = &FunctionPointerArg1::membercaller<T>;
    }

    /** Call the attached static or member function
     */
    R call(){
        if (_membercaller == 0 && _p.function) {
            return _p.function();
        } else if (_membercaller && _p.object) {
            return _membercaller(_p.object, _member);
        }
        return (R)0;
    }

    /** Get registered static function
     */
    R(*get_function())() {
        return _membercaller ? (R(*)())0 : (R(*)())_p.function;
    }

#ifdef MBED_OPERATORS
    R operator ()(void) {
        return call();
    }
    operator bool(void) const {
        return (_membercaller != NULL ? _p.object : (void*)_p.function) != NULL;
    }
#endif

private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member) {
        T* o = static_cast<T*>(object);
        R (T::**m)(void) = reinterpret_cast<R (T::**)(void)>(member);
        return (o->**m)();
    }

    union {
        R (*function)(void); // static function pointer
        void *object;        // object this pointer
    } _p;
    uintptr_t _member[4]; // aligned raw member function pointer storage - converted back by registered _membercaller
    R (*_membercaller)(void*, uintptr_t*); // registered membercaller function to convert back and call _m.member on _object
};

typedef FunctionPointerArg1<void, void> FunctionPointer;
typedef FunctionPointerArg1<void, int> event_callback_t;

} // namespace mbed

#endif
