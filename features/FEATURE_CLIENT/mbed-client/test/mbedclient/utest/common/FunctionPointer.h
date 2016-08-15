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
#include <stddef.h>
#include <stdarg.h>
#include <new>
#include "FunctionPointerBase.h"
#include "FunctionPointerBind.h"

namespace mbed {
/** A class for storing and calling a pointer to a static or member void function without arguments
 */
template <typename R>
class FunctionPointer0 : public FunctionPointerBase<R>{
public:
    typedef R(*static_fp)(void);
    typedef struct arg_struct{
    } ArgStruct;
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    FunctionPointer0(static_fp function = 0):
        FunctionPointerBase<R>()
    {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointer0(T *object, R (T::*member)(void)):
        FunctionPointerBase<R>()
    {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(static_fp function) {
        FunctionPointerBase<R>::_object = reinterpret_cast<void*>(function);
        FunctionPointerBase<R>::_membercaller = &FunctionPointer0::staticcaller;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(void)) {
        FunctionPointerBase<R>::_object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(void)>(FunctionPointerBase<R>::_member) = member;
        FunctionPointerBase<R>::_membercaller = &FunctionPointer0::membercaller<T>;
    }

    /** Call the attached static or member function
     */
    R call(){
        return FunctionPointerBase<R>::call(NULL);
    }

    FunctionPointerBind<R> bind() {
        FunctionPointerBind<R> fp;
        fp.bind(&FunctionPointerBase<R>::_nullops, (ArgStruct *) NULL, this);
        return fp;
    }

    static_fp get_function()const {
        return reinterpret_cast<static_fp>(FunctionPointerBase<R>::_object);
    }

    R operator ()(void) {
        return call();
    }

private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member, void *arg) {
        (void) arg;
        T* o = static_cast<T*>(object);
        R (T::**m)(void) = reinterpret_cast<R (T::**)(void)>(member);
        return (o->**m)();
    }
    static R staticcaller(void *object, uintptr_t *member, void *arg) {
        (void) arg;
        (void) member;
        static_fp f = reinterpret_cast<static_fp>(object);
        return f();
    }
};

/* If we had variaditic templates, this wouldn't be a problem, but until C++11 is enabled, we are stuck with multiple classes... */

/** A class for storing and calling a pointer to a static or member void function with one argument
 */
template <typename R, typename A1>
class FunctionPointer1 : public FunctionPointerBase<R> {
protected:
    typedef struct arg_struct{
        A1 a1;
        arg_struct(const A1 *b1) {
            a1 = *b1;
        }
    } ArgStruct;

public:
    typedef R(*static_fp)(A1);
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    FunctionPointer1(static_fp function = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointer1(T *object, R (T::*member)(A1)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(static_fp function) {
        FunctionPointerBase<R>::_object = reinterpret_cast<void*>(function);
        FunctionPointerBase<R>::_membercaller = &FunctionPointer1::staticcaller;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(A1))
    {
        FunctionPointerBase<R>::_object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(A1)>(FunctionPointerBase<R>::_member) = member;
        FunctionPointerBase<R>::_membercaller = &FunctionPointer1::membercaller<T>;
    }

    FunctionPointerBind<R> bind(const A1 &a1) {
        FunctionPointerBind<R> fp;
        fp.bind(&_fp1_ops, (ArgStruct *) NULL, this, &a1);
        return fp;
    }


    /** Call the attached static or member function
     */
    R call(A1 a1)
    {
        ArgStruct Args(&a1);
        return FunctionPointerBase<R>::call(&Args);
    }

    static_fp get_function()const
    {
        return reinterpret_cast<static_fp>(FunctionPointerBase<R>::_object);
    }

    R operator ()(A1 a) {
        return call(a);
    }

private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        T* o = static_cast<T*>(object);
        R (T::**m)(A1) = reinterpret_cast<R (T::**)(A1)>(member);
        return (o->**m)(Args->a1);
    }
    static R staticcaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        (void) member;
        static_fp f = reinterpret_cast<static_fp>(object);
        return f(Args->a1);
    }
/*    static void constructor(void * dest, va_list args) {
        new(dest) ArgStruct(va_arg(args,A1*));
    }
    static void copy_constructor(void *dest , void* src) {
        ArgStruct *src_args = static_cast<ArgStruct *>(src);
        new(dest) ArgStruct(&(src_args->a1));
    }
    static void destructor(void *args) {
        ArgStruct *argstruct = static_cast<ArgStruct *>(args);
        argstruct->~arg_struct();
    }
*/
protected:
    static const struct FunctionPointerBase<R>::ArgOps _fp1_ops;
};

template <typename R, typename A1>
const struct FunctionPointerBase<R>::ArgOps FunctionPointer1<R,A1>::_fp1_ops = {
    FunctionPointer1<R,A1>::constructor,
    FunctionPointer1<R,A1>::copy_constructor,
    FunctionPointer1<R,A1>::destructor
};


/** A class for storing and calling a pointer to a static or member void function with two arguments
 */
template <typename R, typename A1, typename A2>
class FunctionPointer2 : public FunctionPointerBase<R> {
protected:
    typedef struct arg_struct{
        A1 a1;
        A2 a2;
        arg_struct(const A1 *b1, const A2 *b2) {
            a1 = *b1;
            a2 = *b2;
        }
    } ArgStruct;

public:
    typedef R(*static_fp)(A1, A2);
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    FunctionPointer2(static_fp function = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointer2(T *object, R (T::*member)(A1, A2)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(static_fp function) {
        FunctionPointerBase<R>::_object = reinterpret_cast<void*>(function);
        FunctionPointerBase<R>::_membercaller = &FunctionPointer2::staticcaller;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(A1, A2))
    {
        FunctionPointerBase<R>::_object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(A1, A2)>(FunctionPointerBase<R>::_member) = member;
        FunctionPointerBase<R>::_membercaller = &FunctionPointer2::membercaller<T>;
    }

    FunctionPointerBind<R> bind(const A1 &a1, const A2 &a2) {
        FunctionPointerBind<R> fp;
        fp.bind(&_fp2_ops, (ArgStruct *) NULL, this, &a1, &a2);
        return fp;
    }


    /** Call the attached static or member function
     */
    R call(A1 a1, A2 a2)
    {
        ArgStruct Args(&a1, &a2);
        return FunctionPointerBase<R>::call(&Args);
    }

    static_fp get_function()const
    {
        return reinterpret_cast<static_fp>(FunctionPointerBase<R>::_object);
    }

    R operator ()(A1 a1, A2 a2) {
        return call(a1, a2);
    }

private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        T* o = static_cast<T*>(object);
        R (T::**m)(A1, A2) = reinterpret_cast<R (T::**)(A1, A2)>(member);
        return (o->**m)(Args->a1, Args->a2);
    }
    static R staticcaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        (void) member;
        static_fp f = reinterpret_cast<static_fp>(object);
        return f(Args->a1, Args->a2);
    }
/*    static void constructor(void * dest, va_list args) {
        A1 *a1 = va_arg(args, A1*);
        A2 *a2 = va_arg(args, A2*);
        new(dest) ArgStruct(a1, a2);
    }
    static void copy_constructor(void *dest , void* src) {
        ArgStruct *src_args = static_cast<ArgStruct *>(src);
        new(dest) ArgStruct(&(src_args->a1), &(src_args->a2));
    }
    static void destructor(void *args) {
        ArgStruct *argstruct = static_cast<ArgStruct *>(args);
        argstruct->~arg_struct();
    }
*/
protected:
    static const struct FunctionPointerBase<R>::ArgOps _fp2_ops;
};

template <typename R, typename A1, typename A2>
const struct FunctionPointerBase<R>::ArgOps FunctionPointer2<R,A1,A2>::_fp2_ops = {
    FunctionPointer2<R,A1,A2>::constructor,
    FunctionPointer2<R,A1,A2>::copy_constructor,
    FunctionPointer2<R,A1,A2>::destructor
};

/** A class for storing and calling a pointer to a static or member void function with three arguments
 */
template <typename R, typename A1, typename A2, typename A3>
class FunctionPointer3 : public FunctionPointerBase<R> {
protected:
    typedef struct arg_struct{
        A1 a1;
        A2 a2;
        A3 a3;
        arg_struct(const A1 *b1, const A2 *b2, const A3* b3) {
            a1 = *b1;
            a2 = *b2;
            a3 = *b3;
        }
    } ArgStruct;

public:
    typedef R(*static_fp)(A1, A2, A3);
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    FunctionPointer3(static_fp function = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointer3(T *object, R (T::*member)(A1, A2, A3)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(static_fp function) {
        FunctionPointerBase<R>::_object = reinterpret_cast<void*>(function);
        FunctionPointerBase<R>::_membercaller = &FunctionPointer3::staticcaller;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(A1, A2, A3))
    {
        FunctionPointerBase<R>::_object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(A1, A2, A3)>(FunctionPointerBase<R>::_member) = member;
        FunctionPointerBase<R>::_membercaller = &FunctionPointer3::membercaller<T>;
    }

    FunctionPointerBind<R> bind(const A1 &a1, const A2 &a2, const A3 &a3) {
        FunctionPointerBind<R> fp;
        fp.bind(&_fp3_ops, (ArgStruct *) NULL, this, &a1, &a2, &a3);
        return fp;
    }


    /** Call the attached static or member function
     */
    R call(A1 a1, A2 a2, A3 a3)
    {
        ArgStruct Args(&a1, &a2, &a3);
        return FunctionPointerBase<R>::call(&Args);
    }

    static_fp get_function()const
    {
        return reinterpret_cast<static_fp>(FunctionPointerBase<R>::_object);
    }

    R operator ()(A1 a1, A2 a2, A3 a3) {
        return call(a1, a2, a3);
    }

private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        T* o = static_cast<T*>(object);
        R (T::**m)(A1, A2, A3) = reinterpret_cast<R (T::**)(A1, A2, A3)>(member);
        return (o->**m)(Args->a1, Args->a2, Args->a3);
    }
    static R staticcaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        (void) member;
        static_fp f = reinterpret_cast<static_fp>(object);
        return f(Args->a1, Args->a2, Args->a3);
    }
/*    static void constructor(void * dest, va_list args) {
        A1 *a1 = va_arg(args, A1*);
        A2 *a2 = va_arg(args, A2*);
        A3 *a3 = va_arg(args, A3*);
        new(dest) ArgStruct(a1, a2, a3);
    }
    static void copy_constructor(void *dest , void* src) {
        ArgStruct *src_args = static_cast<ArgStruct *>(src);
        new(dest) ArgStruct(&(src_args->a1), &(src_args->a2), &(src_args->a3));
    }
    static void destructor(void *args) {
        ArgStruct *argstruct = static_cast<ArgStruct *>(args);
        argstruct->~arg_struct();
    }
*/
protected:
    static const struct FunctionPointerBase<R>::ArgOps _fp3_ops;
};

template <typename R, typename A1, typename A2, typename A3>
const struct FunctionPointerBase<R>::ArgOps FunctionPointer3<R,A1,A2,A3>::_fp3_ops = {
    FunctionPointer3<R,A1,A2,A3>::constructor,
    FunctionPointer3<R,A1,A2,A3>::copy_constructor,
    FunctionPointer3<R,A1,A2,A3>::destructor
};

/** A class for storing and calling a pointer to a static or member void function with four arguments
 */
template <typename R, typename A1, typename A2, typename A3, typename A4>
class FunctionPointer4 : public FunctionPointerBase<R> {
protected:
    typedef struct arg_struct{
        A1 a1;
        A2 a2;
        A3 a3;
        A4 a4;
        arg_struct(const A1 *b1, const A2 *b2, const A3* b3, const A4* b4) {
            a1 = *b1;
            a2 = *b2;
            a3 = *b3;
            a4 = *b4;
        }
    } ArgStruct;

public:
    typedef R(*static_fp)(A1, A2, A3, A4);
    /** Create a FunctionPointer, attaching a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    FunctionPointer4(static_fp function = 0) {
        attach(function);
    }

    /** Create a FunctionPointer, attaching a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    FunctionPointer4(T *object, R (T::*member)(A1, A2, A3, A4)) {
        attach(object, member);
    }

    /** Attach a static function
     *
     *  @param function The void static function to attach (default is none)
     */
    void attach(static_fp function) {
        FunctionPointerBase<R>::_object = reinterpret_cast<void*>(function);
        FunctionPointerBase<R>::_membercaller = &FunctionPointer4::staticcaller;
    }

    /** Attach a member function
     *
     *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
     *  @param function The address of the void member function to attach
     */
    template<typename T>
    void attach(T *object, R (T::*member)(A1, A2, A3, A4))
    {
        FunctionPointerBase<R>::_object = static_cast<void*>(object);
        *reinterpret_cast<R (T::**)(A1, A2, A3, A4)>(FunctionPointerBase<R>::_member) = member;
        FunctionPointerBase<R>::_membercaller = &FunctionPointer4::membercaller<T>;
    }

    FunctionPointerBind<R> bind(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4) {
        FunctionPointerBind<R> fp;
        fp.bind(&_fp4_ops, (ArgStruct *) NULL, this, &a1, &a2, &a3, &a4);
        return fp;
    }


    /** Call the attached static or member function
     */
    R call(A1 a1, A2 a2, A3 a3, A4 a4)
    {
        ArgStruct Args(&a1, &a2, &a3, &a4);
        return FunctionPointerBase<R>::call(&Args);
    }

    static_fp get_function()const
    {
        return reinterpret_cast<static_fp>(FunctionPointerBase<R>::_object);
    }

    R operator ()(A1 a1, A2 a2, A3 a3, A4 a4) {
        return call(a1, a2, a3, a4);
    }

private:
    template<typename T>
    static R membercaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        T* o = static_cast<T*>(object);
        R (T::**m)(A1, A2, A3, A4) = reinterpret_cast<R (T::**)(A1, A2, A3, A4)>(member);
        return (o->**m)(Args->a1, Args->a2, Args->a3, Args->a4);
    }
    static R staticcaller(void *object, uintptr_t *member, void *arg) {
        ArgStruct *Args = static_cast<ArgStruct *>(arg);
        (void) member;
        static_fp f = reinterpret_cast<static_fp>(object);
        return f(Args->a1, Args->a2, Args->a3, Args->a4);
    }
/*    static void constructor(void * dest, va_list args) {
        A1 *a1 = va_arg(args, A1*);
        A2 *a2 = va_arg(args, A2*);
        A3 *a3 = va_arg(args, A3*);
        A4 *a4 = va_arg(args, A4*);
        new(dest) ArgStruct(a1, a2, a3, a4);
    }
    static void copy_constructor(void *dest , void* src) {
        ArgStruct *src_args = static_cast<ArgStruct *>(src);
        new(dest) ArgStruct(&(src_args->a1), &(src_args->a2), &(src_args->a3), &(src_args->a4));
    }
    static void destructor(void *args) {
        ArgStruct *argstruct = static_cast<ArgStruct *>(args);
        argstruct->~arg_struct();
    }
*/
protected:
    static const struct FunctionPointerBase<R>::ArgOps _fp4_ops;
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
const struct FunctionPointerBase<R>::ArgOps FunctionPointer4<R,A1,A2,A3,A4>::_fp4_ops = {
    FunctionPointer4<R,A1,A2,A3,A4>::constructor,
    FunctionPointer4<R,A1,A2,A3,A4>::copy_constructor,
    FunctionPointer4<R,A1,A2,A3,A4>::destructor
};

typedef FunctionPointer0<void> FunctionPointer;
//typedef FunctionPointer1<void, int> event_callback_t;

} // namespace mbed

#endif
