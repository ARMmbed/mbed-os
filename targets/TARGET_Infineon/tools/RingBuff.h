/* mbed Example Program
 * Copyright (c) 2006-2014 ARM Limited
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

// library provided by https://github.com/wizard97/Embedded_RingBuf_CPP

#ifndef MBED_OS_CIRCULAR_BUFFER_H_
#define MBED_OS_CIRCULAR_BUFFER_H_

#include "mbed.h"
#include "rtos.h"

template <typename Type, size_t MaxElements>
class RingBuff
{
public:

RingBuff()
{
    core_util_critical_section_enter();

    _numElements = 0;
    _head = 0;

    core_util_critical_section_exit();
}

/**
 *  Add element obj to the buffer
 * Return: true on success
 */
bool add(const Type &obj)
{
    bool ret = false;
    core_util_critical_section_enter();
    {
        if (!isFull()) {
            _buf[_head] = obj;
            _head = (_head + 1)%MaxElements;
            _numElements++;

            ret = true;
        }
    }
    core_util_critical_section_exit();

    return ret;
}


/**
 * Remove last element from buffer, and copy it to dest
 * Return: true on success
 */
bool pull(Type *dest)
{
    bool ret = false;
    size_t tail;

    core_util_critical_section_enter();
    {
        if (!isEmpty()) {
            tail = getTail();
            *dest = _buf[tail];
            _numElements--;

            ret = true;
        }
    }
    core_util_critical_section_exit();

    return ret;
}


/**
 * Peek at num'th element in the buffer
 * Return: a pointer to the num'th element
 */
Type* peek(size_t num)
{
    Type *ret = NULL;

    core_util_critical_section_enter();
    {
        if (num < _numElements) //make sure not out of bounds
            ret = &_buf[(getTail() + num)%MaxElements];
    }
    core_util_critical_section_exit();

    return ret;
}


/**
 * Return: true if buffer is full
 */
bool isFull() const
{
    bool ret;

    core_util_critical_section_enter();
    {
        ret = _numElements >= MaxElements;
    }
    core_util_critical_section_exit();

    return ret;
}


/**
 * Return: number of elements in buffer
 */
size_t numElements() const
{
    size_t ret;

    core_util_critical_section_enter();
    {
        ret = _numElements;
    }
    core_util_critical_section_exit();

    return ret;
}


/**
 * Return: true if buffer is empty
 */
bool isEmpty() const
{
    bool ret;

    core_util_critical_section_enter();
    {
        ret = !_numElements;
    }
    core_util_critical_section_exit();

    return ret;
}

protected:
/**
 * Calculates the index in the array of the oldest element
 * Return: index in array of element
 */
size_t getTail() const
{
    return (_head + (MaxElements - _numElements))%MaxElements;
}


// underlying array
Type _buf[MaxElements];

size_t _head;
size_t _numElements;
private:

};

#endif /* MBED_OS_CIRCULAR_BUFFER_H_ */

/*EOF*/
