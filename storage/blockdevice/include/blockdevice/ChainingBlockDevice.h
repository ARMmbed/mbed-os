/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 ARM Limited
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

/** \addtogroup storage */
/** @{*/

#ifndef MBED_CHAINING_BLOCK_DEVICE_H
#define MBED_CHAINING_BLOCK_DEVICE_H

#include "BlockDevice.h"
#include "platform/mbed_assert.h"
#include <stdlib.h>

namespace mbed {

/** Block device for chaining multiple block devices
 *  with the similar block sizes at sequential addresses
 *
 *  @code
 *  #include "mbed.h"
 *  #include "HeapBlockDevice.h"
 *  #include "ChainingBlockDevice.h"
 *
 *  // Create two smaller block devices with
 *  // 64 and 32 blocks of size 512 bytes
 *  HeapBlockDevice mem1(64*512, 512);
 *  HeapBlockDevice mem2(32*512, 512);
 *
 *  // Create a block device backed by mem1 and mem2
 *  // contains 96 blocks of size 512 bytes
 *  BlockDevice *bds[] = {&mem1, &mem2};
 *  ChainingBlockDevice chainmem(bds);
 *  @endcode
 */
class ChainingBlockDevice : public BlockDevice {
public:
    /** Lifetime of the memory block device
     *
     *  @param bds         Array of block devices to chain with sequential block addresses
     *  @param bd_count    Number of block devices to chain
     *  @note All block devices must have the same block size
     */
    ChainingBlockDevice(BlockDevice **bds, size_t bd_count);

    /** Lifetime of the memory block device
     *
     *  @param bds          Array of block devices to chain with sequential block addresses
     *  @note All block devices must have the same block size
     */
    template <size_t Size>
    ChainingBlockDevice(BlockDevice * (&bds)[Size])
        : _bds(bds), _bd_count(sizeof(bds) / sizeof(bds[0]))
        , _read_size(0), _program_size(0), _erase_size(0), _size(0),  _init_ref_count(0)
    {
    }

    /** Lifetime of the memory block device
     *
     */
    virtual ~ChainingBlockDevice() {}

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int deinit();

    /** Ensure data on storage is in sync with the driver
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int sync();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed,
     *  unless get_erase_value returns a non-negative byte value
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int erase(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const;

    /** Get the size of a programmable block
     *
     *  @return         Size of a programmable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size() const;

    /** Get the size of an eraseable block
     *
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size() const;

    /** Get the size of an erasable block given address
     *
     *  @param addr     Address within the erasable block
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size(bd_addr_t addr) const;

    /** Get the value of storage when erased
     *
     *  If get_erase_value returns a non-negative byte value, the underlying
     *  storage is set to that value when erased, and storage containing
     *  that value can be programmed without another erase.
     *
     *  @return         The value of storage when erased, or -1 if you can't
     *                  rely on the value of erased storage
     */
    virtual int get_erase_value() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const;

protected:
    BlockDevice **_bds;
    size_t _bd_count;
    bd_size_t _read_size;
    bd_size_t _program_size;
    bd_size_t _erase_size;
    bd_size_t _size;
    int _erase_value;
    uint32_t _init_ref_count;
    bool _is_initialized;
};

} // namespace mbed

// Added "using" for backwards compatibility
#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using mbed::ChainingBlockDevice;
#endif

#endif

/** @}*/
