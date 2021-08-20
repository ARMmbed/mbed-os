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

#ifndef MBED_MEM_BLOCK_DEVICE_H
#define MBED_MEM_BLOCK_DEVICE_H

#include "BlockDevice.h"
#include "platform/mbed_assert.h"
#include <string.h>
#include <stdlib.h>

namespace mbed {

/** Lazily allocated heap-backed block device.
 *
 * Useful for simulating a block device and tests.
 *
 * @note Each block is allocated when used, and freed when erased.
 *
 * @code
 * #include "mbed.h"
 * #include "HeapBlockDevice.h"
 *
 * #define BLOCK_SIZE 512
 *
 * HeapBlockDevice bd(2048, BLOCK_SIZE); // 2048 bytes with a block size of 512 bytes
 * uint8_t block[BLOCK_SIZE] = "Hello World!\n";
 *
 * int main() {
 *     bd.init();
 *     bd.erase(0, BLOCK_SIZE);
 *     bd.program(block, 0, BLOCK_SIZE);
 *     bd.read(block, 0, BLOCK_SIZE);
 *     printf("%s", block);
 *     bd.deinit();
 * }
 * @endcode
 */
class HeapBlockDevice : public BlockDevice {
public:

    /** Lifetime of the memory block device
     *
     * @param size      Size of the Block Device in bytes
     * @param block     Block size in bytes. Minimum read, program, and erase sizes are
     *                  configured to this value
     */
    HeapBlockDevice(bd_size_t size, bd_size_t block = 512);
    /** Lifetime of the memory block device
     *
     * @param size      Size of the Block Device in bytes
     * @param read      Minimum read size required in bytes
     * @param program   Minimum program size required in bytes
     * @param erase     Minimum erase size required in bytes
     */
    HeapBlockDevice(bd_size_t size, bd_size_t read, bd_size_t program, bd_size_t erase);
    virtual ~HeapBlockDevice();

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

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to read blocks into
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
     *  The state of an erased block is undefined until it has been programmed
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
     */
    virtual bd_size_t get_program_size() const;

    /** Get the size of an erasable block
     *
     *  @return         Size of an erasable block in bytes
     */
    virtual bd_size_t get_erase_size() const;

    /** Get the size of an erasable block given address
     *
     *  @param addr     Address within the erasable block
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size(bd_addr_t addr) const;

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

private:
    bd_size_t _read_size;
    bd_size_t _program_size;
    bd_size_t _erase_size;
    bd_size_t _count;
    uint8_t **_blocks;
    uint32_t _init_ref_count;
    bool _is_initialized;
};

} // namespace mbed

// Added "using" for backwards compatibility
#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using mbed::HeapBlockDevice;
#endif

#endif

/** @}*/
