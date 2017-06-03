/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "rtos/Mutex.h"

#include <string.h>
#include "mbed_error.h"
#include "mbed_assert.h"

namespace rtos {

Mutex::Mutex()
{
    constructor();
}

Mutex::Mutex(const char *name)
{
    constructor(name);
}

void Mutex::constructor(const char *name)
{
    memset(&_obj_mem, 0, sizeof(_obj_mem));
    memset(&_attr, 0, sizeof(_attr));
    _attr.name = name ? name : "aplication_unnamed_mutex";
    _attr.cb_mem = &_obj_mem;
    _attr.cb_size = sizeof(_obj_mem);
    _attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
    _id = osMutexNew(&_attr);
    MBED_ASSERT(_id);
}

osStatus Mutex::lock(uint32_t millisec) {
    return osMutexAcquire(_id, millisec);
}

bool Mutex::trylock() {
    return (osMutexAcquire(_id, 0) == osOK);
}

osStatus Mutex::unlock() {
    return osMutexRelease(_id);
}

Mutex::~Mutex() {
    osMutexDelete(_id);
}

}
