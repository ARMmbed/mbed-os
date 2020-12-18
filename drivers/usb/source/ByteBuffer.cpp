/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include "ByteBuffer.h"
#include "mbed_assert.h"
#include <string.h>

ByteBuffer::ByteBuffer(uint32_t size): _head(0), _tail(0), _size(0), _buf(NULL)
{
    resize(_size);
}

ByteBuffer::~ByteBuffer()
{
    delete[] _buf;
    _buf = 0;
}

void ByteBuffer::resize(uint32_t size)
{
    delete[] _buf;
    _head = 0;
    _tail = 0;
    _size = size + 1;
    _buf = new uint8_t[_size]();
}

void ByteBuffer::push(uint8_t data)
{
    _buf[_tail] = data;
    _tail++;
    if (_tail >= _size) {
        _tail -= _size;
    }
    // Overflow not allowed
    MBED_ASSERT(_head != _tail);
}

void ByteBuffer::write(uint8_t *data, uint32_t size)
{
    MBED_ASSERT(size <= free());

    if (size == 0) {
        return;
    }

    uint32_t new_tail = _tail + size;
    if (new_tail >= _size) {
        new_tail -= _size;
    }

    // Perform first memcpy
    uint32_t until_end = _size - _tail;
    uint32_t copy_size = until_end < size ? until_end : size;
    memcpy(_buf + _tail, data, copy_size);
    data += copy_size;
    size -= copy_size;

    // Perform second memcpy
    if (size > 0) {
        memcpy(_buf, data, size);
    }

    // Update tail
    _tail = new_tail;
}

uint8_t ByteBuffer::pop()
{
    // Underflow not allowed
    MBED_ASSERT(_head != _tail);
    uint8_t val = _buf[_head];
    _head++;
    if (_head >= _size) {
        _head -= _size;
    }
    return val;
}

void ByteBuffer::read(uint8_t *data, uint32_t size)
{
    MBED_ASSERT(size <= ByteBuffer::size());

    if (size == 0) {
        return;
    }

    uint32_t new_head = _head + size;
    if (new_head >= _size) {
        new_head -= _size;
    }

    // Perform first memcpy
    uint32_t until_end = _size - _head;
    uint32_t copy_size = until_end < size ? until_end : size;
    memcpy(data, _buf + _head, copy_size);
    data += copy_size;
    size -= copy_size;

    // Perform second memcpy
    if (size > 0) {
        memcpy(data, _buf, size);
    }

    // Update head
    _head = new_head;
}

uint32_t ByteBuffer::size()
{
    uint32_t size;
    if (_tail < _head) {
        size = _size + _tail - _head;
    } else {
        size = _tail - _head;
    }
    return size;
}

uint32_t ByteBuffer::free()
{
    return _size - size() - 1;
}

bool ByteBuffer::full()
{
    uint32_t next = _tail + 1;
    if (next >= _size) {
        next -= _size;
    }
    return next == _head;
}

bool ByteBuffer::empty()
{
    return _head == _tail;
}
