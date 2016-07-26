/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef FUNCTIONPOINTER_H
#define FUNCTIONPOINTER_H

#include <string.h>
#include <stdint.h>


/** A class for storing and calling a pointer to a static or member void function
 */
template <typename R>
class FP0{
public:
    /** Create a Function Pointer, attaching a static function
     *
     *  \param function The void static function to attach (default is none)
     */
    FP0(R (*function)(void) = 0) {
        attach(function);
    }

    /** Create a FP, attaching a member function
     *
     *  \param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  \param function The address of the void member function to attach
     */
    template<typename T>
    FP0(T *object, R (T::*member)(void)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  \param function The void static function to attach (default is none)
     */
    void attach(R (*function)(void)) {
        _p.function = function;
        _membercaller = 0;
    }

    /** Attach a member function
     *
     *  \param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  \param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(void)) {
        _p.object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(void)>(_member) = member;
        _membercaller = &FP0::membercaller<T>;
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

    typedef R (*static_fp)();
    static_fp get_function() const {
        return (R(*)())_p.function;
    }

    R operator ()(void) {
        return call();
    }
    operator bool(void) {
        void *q = &_p.function;
        return (_membercaller != NULL) && _p.object != NULL && (*static_cast<void **>(q) != NULL);
    }

private:
    template<typename T>
    static void membercaller(void *object, uintptr_t *member) {
        T* o = static_cast<T*>(object);
        R (T::**m)(void) = reinterpret_cast<R (T::**)(void)>(member);
        (o->**m)();
    }

    union {
        R (*function)(void);               // static function pointer - 0 if none attached
        void *object;                         // object this pointer - 0 if none attached
    } _p;
    uintptr_t _member[2];                     // aligned raw member function pointer storage - converted back by registered _membercaller
    R (*_membercaller)(void*, uintptr_t*); // registered membercaller function to convert back and call _m.member on _object
};

/* If we had variadic templates, this wouldn't be a problem, but until C++11 is enabled, we are stuck with multiple classes... */

/** A class for storing and calling a pointer to a static or member void function
 */
template <typename R, typename A1>
class FP1{
public:
    /** Create a FP, attaching a static function
     *
     *  \param function The void static function to attach (default is none)
     */
    FP1(R (*function)(A1) = 0) {
        attach(function);
    }

    /** Create a FP, attaching a member function
     *
     *  \param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  \param function The address of the void member function to attach
     */
    template<typename T>
    FP1(T *object, R (T::*member)(A1)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  \param function The void static function to attach (default is none)
     */
    void attach(R (*function)(A1)) {
        _p.function = function;
        _membercaller = 0;
    }

    /** Attach a member function
     *
     *  \param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  \param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(A1)) {
        _p.object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(A1)>(_member) = member;
        _membercaller = &FP1::membercaller<T>;
    }

    /** Call the attached static or member function
     */
    R call(A1 a){
        if (_membercaller == 0 && _p.function) {
           return _p.function(a);
        } else if (_membercaller && _p.object) {
           return _membercaller(_p.object, _member, a);
        }
        return (R)0;
    }

    typedef R (*static_fp)();
    static_fp get_function() const {
        return (R(*)())_p.function;
    }

    R operator ()(A1 a) {
        return call(a);
    }
    operator bool(void)
    {
        void *q = &_p.function;
        return (_membercaller != NULL) && _p.object != NULL && (*static_cast<void **>(q) != NULL);
    }
private:
    template<typename T>
    static void membercaller(void *object, uintptr_t *member, A1 a) {
        T* o = static_cast<T*>(object);
        R (T::**m)(A1) = reinterpret_cast<R (T::**)(A1)>(member);
        (o->**m)(a);
    }

    union {
        R (*function)(A1);               // static function pointer - 0 if none attached
        void *object;                         // object this pointer - 0 if none attached
    } _p;
    uintptr_t _member[2];                     // aligned raw member function pointer storage - converted back by registered _membercaller
    R (*_membercaller)(void*, uintptr_t*, A1); // registered membercaller function to convert back and call _m.member on _object
};



typedef FP0<void> FP;

#endif
