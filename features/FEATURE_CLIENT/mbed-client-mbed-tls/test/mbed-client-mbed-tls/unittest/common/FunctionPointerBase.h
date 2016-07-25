/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#ifndef MBED_FUNCTIONPOINTERBASE_H
#define MBED_FUNCTIONPOINTERBASE_H

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
namespace mbed {

template<typename R>
class FunctionPointerBase {
public:
    operator bool(void) const {
        return (_membercaller != NULL) && (_object != NULL);
    }

    /**
     * Clears the current function pointer assignment
     * After clear(), this instance will point to nothing (NULL)
     */
    virtual void clear() {
        _membercaller = NULL;
        _object = NULL;
    }

protected:
    struct ArgOps {
        void (*constructor)(void *, va_list);
        void (*copy_args)(void *, void *);
        void (*destructor)(void *);
    };
    void * _object; // object Pointer/function pointer
    R (*_membercaller)(void *, uintptr_t *, void *);
    // aligned raw member function pointer storage - converted back by registered _membercaller
    uintptr_t _member[4];
    static const struct ArgOps _nullops;

protected:
    FunctionPointerBase():_object(NULL), _membercaller(NULL) {}
    FunctionPointerBase(const FunctionPointerBase<R> & fp) {
        copy(&fp);
    }
    virtual ~FunctionPointerBase() {
    }

    /**
     * Calls the member pointed to by object::member or (function)object
     * @param arg
     * @return
     */
    inline R call(void* arg) {
        return _membercaller(_object, _member, arg);
    }

    void copy(const FunctionPointerBase<R> * fp) {
        _object = fp->_object;
        memcpy (_member, fp->_member, sizeof(_member));
        _membercaller = fp->_membercaller;
    }
private:
    static void _null_constructor(void * dest, va_list args) {(void) dest;(void) args;}
    static void _null_copy_args(void *dest , void* src) {(void) dest; (void) src;}
    static void _null_destructor(void *args) {(void) args;}

};
template<typename R>
const struct FunctionPointerBase<R>::ArgOps FunctionPointerBase<R>::_nullops = {
    FunctionPointerBase<R>::_null_constructor,
    FunctionPointerBase<R>::_null_copy_args,
    FunctionPointerBase<R>::_null_destructor
};

}
#endif
