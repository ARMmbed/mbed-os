/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_CThunk CThunk class
 * @{
 */
/* General C++ Object Thunking class
 *
 * - allows direct callbacks to non-static C++ class functions
 * - keeps track for the corresponding class instance
 * - supports an optional context parameter for the called function
 * - ideally suited for class object receiving interrupts (NVIC_SetVector)
 *
 * Copyright (c) 2014-2019 ARM Limited
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

/* General C++ Object Thunking class
 *
 * - allows direct callbacks to non-static C++ class functions
 * - keeps track for the corresponding class instance
 * - supports an optional context parameter for the called function
 * - ideally suited for class object receiving interrupts (NVIC_SetVector)
 */

#ifndef __CTHUNK_H__
#define __CTHUNK_H__

#include "platform/internal/CThunkBase.h"

/**
 * Class for created a pointer with data bound to it
 *
 * @note Synchronization level: Not protected
 */
template<class T>
class CThunk: private CThunkBase {
public:
    typedef void (T::*CCallbackSimple)(void);
    typedef void (T::*CCallback)(void *context);

    inline CThunk(T *instance)
    {
        init(instance, NULL, NULL);
    }

    inline CThunk(T *instance, CCallback callback)
    {
        init(instance, callback, NULL);
    }

    ~CThunk()
    {
        if (_entry != NULL) {
            cthunk_free(_entry);
            _entry = NULL;
        }
    }

    inline CThunk(T *instance, CCallbackSimple callback)
    {
        init(instance, (CCallback)callback, NULL);
    }

    inline CThunk(T &instance, CCallback callback)
    {
        init(instance, callback, NULL);
    }

    inline CThunk(T &instance, CCallbackSimple callback)
    {
        init(instance, (CCallback)callback, NULL);
    }

    inline CThunk(T &instance, CCallback callback, void *context)
    {
        init(instance, callback, context);
    }

    inline void callback(CCallback callback)
    {
        _callback = callback;
    }

    inline void callback(CCallbackSimple callback)
    {
        _callback_simple = callback;
    }

    inline void context(void *context)
    {
        _context = context;
    }

    inline void context(uint32_t context)
    {
        _context = (void *)context;
    }

    inline uint32_t entry(void)
    {
        if (_entry == NULL) {
            _entry = cthunk_alloc(this);
        }
        return (uint32_t)_entry;
    }

    /* get thunk entry point for connecting rhunk to an IRQ table */
    inline operator CThunkEntry(void)
    {
        return (CThunkEntry)entry();
    }

    /* get thunk entry point for connecting rhunk to an IRQ table */
    inline operator uint32_t(void)
    {
        return entry();
    }

    /* simple test function */
    inline void call(void)
    {
        (((CThunkEntry)(entry()))());
    }

private:
    T *_instance;
    void *_context;
    union {
        CCallbackSimple _callback_simple;
        CCallback _callback;
    };

    CThunkEntry _entry;

    static void trampoline(CThunkBase *base)
    {
        CThunk<T> *self = static_cast<CThunk<T>*>(base);
        T *instance = self->_instance;
        void *context = self->_context;
        CCallback callback = self->_callback;

        if (instance && callback) {
            (instance->*callback)(context);
        }
    }

    inline void init(T *instance, CCallback callback, void *context)
    {
        _instance = instance;
        _context = context;
        _callback = callback;
        _trampoline = &trampoline;
        _entry = 0;
    }
};

/**@}*/

/**@}*/

#endif/*__CTHUNK_H__*/

