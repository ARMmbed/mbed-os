/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef __CTHUNK_BASE_H__
#define __CTHUNK_BASE_H__

/* IRQ/Exception compatible thunk entry function */
typedef void (*CThunkEntry)(void);

class CThunkBase {
protected:
    typedef void (*Trampoline)(CThunkBase *);

    Trampoline _trampoline;

    /*
     * Allocate a CThunkEntry which can be called without arguments
     *
     * Calling the CThunkEntry invokes the _trampoline of the
     * given cthunk. This function traps if there are no more
     * free thunks.
     */
    static CThunkEntry cthunk_alloc(CThunkBase *cthunk);

    /*
     * Free a cthunk_entry so it can be reused
     */
    static void cthunk_free(CThunkEntry cthunk_entry);

private:
    typedef void (*CthunkFree)(CThunkEntry cthunk_entry);

    /*
     * Table of thunk functions
     */
    static const CThunkEntry _thunk_table[MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX];

    /*
     * Table of active CThunk classes
     */
    static CThunkBase *_thunk_storage[MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX];

    /*
     * Lazily initialized free function pointer
     */
    static CthunkFree _cthunk_free_real;

    /*
     * Actual free function
     */
    static void cthunk_free_real(CThunkEntry cthunk_entry);

    /*
     * Template function which stored in the _thunk_table
     */
    template<int N>
    static void thunk_entry()
    {
        _thunk_storage[N]->_trampoline(_thunk_storage[N]);
    }
};

#endif/*__CTHUNK_BASE_H__*/

