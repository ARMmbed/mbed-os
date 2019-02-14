/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2019 ARM Limited
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
#include <string.h>
#include <stdlib.h>
#include "FragBDWrapper.h"

FragBDWrapper::FragBDWrapper(BlockDevice *bd)
    : _block_device(bd), _page_size(0), _total_size(0), _page_buffer(NULL), _last_page(0xffffffff)
{

}

FragBDWrapper::~FragBDWrapper()
{
    if (_page_buffer) {
        free(_page_buffer);
    }
}

int FragBDWrapper::init()
{
    // already initialised
    if (_page_buffer) {
        return BD_ERROR_OK;
    }

    int init_ret = _block_device->init();
    if (init_ret != 0) {
        return init_ret;
    }

    _page_size = _block_device->get_read_size();
    _total_size = _block_device->size();

    void *buffer = calloc((size_t)_page_size, 1);
    _page_buffer = static_cast<uint8_t *>(buffer);
    if (!_page_buffer) {
        return BD_ERROR_NO_MEMORY;
    }

    return BD_ERROR_OK;
}

int FragBDWrapper::program(const void *a_buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_page_buffer) {
        return BD_ERROR_NOT_INITIALIZED;
    }

    // Q: a 'global' _page_buffer makes this code not thread-safe...
    // is this a problem? don't really wanna malloc/free in every call

    uint8_t *buffer = (uint8_t *)a_buffer;

    frag_debug("[FBDW] write addr=%lu size=%d\n", addr, size);

    // find the page
    size_t bytes_left = size;
    while (bytes_left > 0) {
        uint32_t page = addr / _page_size; // this gets auto-rounded
        uint32_t offset = addr % _page_size; // offset from the start of the _page_buffer
        uint32_t length = _page_size - offset; // number of bytes to write in this _page_buffer
        if (length > bytes_left) {
            length = bytes_left; // don't overflow
        }

        frag_debug("[FBDW] writing to page=%lu, offset=%lu, length=%lu\n", page, offset, length);

        int r;

        // retrieve the page first, as we don't want to overwrite the full page
        if (_last_page != page) {
            r = _block_device->read(_page_buffer, page * _page_size, _page_size);
            if (r != 0) {
                return r;
            }
        }

        // now memcpy to the _page_buffer
        memcpy(_page_buffer + offset, buffer, length);

        // and write back
        r = _block_device->program(_page_buffer, page * _page_size, _page_size);
        if (r != 0) {
            return r;
        }

        // change the page
        bytes_left -= length;
        addr += length;
        buffer += length;

        _last_page = page;
    }

    return BD_ERROR_OK;
}

int FragBDWrapper::read(void *a_buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_page_buffer) {
        return BD_ERROR_NOT_INITIALIZED;
    }

    frag_debug("[FBDW] read addr=%lu size=%d\n", addr, size);

    uint8_t *buffer = (uint8_t *)a_buffer;

    size_t bytes_left = size;
    while (bytes_left > 0) {
        uint32_t page = addr / _page_size; // this gets auto-rounded
        uint32_t offset = addr % _page_size; // offset from the start of the _page_buffer
        uint32_t length = _page_size - offset; // number of bytes to read in this _page_buffer
        if (length > bytes_left) {
            length = bytes_left; // don't overflow
        }

        frag_debug("[FBDW] Reading from page=%lu, offset=%lu, length=%lu\n", page, offset, length);

        if (_last_page != page) {
            int r = _block_device->read(_page_buffer, page * _page_size, _page_size);
            if (r != 0) {
                return r;
            }
        }

        // copy into the provided buffer
        memcpy(buffer, _page_buffer + offset, length);

        // change the page
        bytes_left -= length;
        addr += length;
        buffer += length;

        _last_page = page;
    }

    return BD_ERROR_OK;
}

