/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2018 ARM Limited
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

#ifndef _MBED_LORAWAN_UPDATE_CLIENT_FRAGMENTATION_BDWRAPPER
#define _MBED_LORAWAN_UPDATE_CLIENT_FRAGMENTATION_BDWRAPPER

/**
 * This is a class that wraps around a block device and allows for unaligned
 * operations on the underlying BD. This is useful because the packets are not
 * written in aligned mode, and this way we have a central place where the
 * block alignment happens.
 *
 * Note that this class initializes a buffer that is one page size long, and
 * that access to this buffer is not thread safe.
 */

#include "mbed.h"
#include "BlockDevice.h"

#if !defined(FRAG_BLOCK_DEVICE_DEBUG)
#define frag_debug(...) do {} while(0)
#else
#define frag_debug(...) debug(__VA_ARGS__)
#endif

enum frag_bd_error {
    BD_ERROR_NO_MEMORY          = -4002,
    BD_ERROR_NOT_INITIALIZED    = -4003,
};

class FragBDWrapper {
public:

    /**
     * Wrap a block device for unaligned operations, note that you still need to initialize this class (by calling 'init')
     *
     * @param bd A block device (can be uninitialized)
     */
    FragBDWrapper(BlockDevice *bd);

    ~FragBDWrapper();

    /**
     * Initialize the block device and the wrapper, this will allocate one page of memory
     */
    int init();

    /**
     * Write a buffer to the block device
     *
     * @param a_buffer Buffer to write
     * @param addr Address on the block device to write
     * @param size Size of a_buffer
     *
     * @returns 0 if the write succeeded, negative value if it failed
     */
    int program(const void *a_buffer, bd_addr_t addr, bd_size_t size);

    /**
     * Read a buffer from the block device
     *
     * @param a_buffer Buffer to read into
     * @param addr Address on the block device to write
     * @param size Size of a_buffer
     *
     * @returns 0 if the read succeeded, negative value if it failed
     */
    int read(void *a_buffer, bd_addr_t addr, bd_size_t size);

private:
    BlockDevice*    _block_device;
    bd_size_t       _page_size;
    bd_size_t       _total_size;
    uint8_t*        _page_buffer;
    uint32_t        _last_page;
};

#endif // _MBED_LORAWAN_UPDATE_CLIENT_FRAGMENTATION_BDWRAPPER
