/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef MBED_FLASHIAP_BLOCK_DEVICE_H
#define MBED_FLASHIAP_BLOCK_DEVICE_H

#if DEVICE_FLASH

#include "FlashIAP.h"
#include "features/storage/blockdevice/BlockDevice.h"
#include "platform/mbed_toolchain.h"

/** BlockDevice using the FlashIAP API
 *
 */
class FlashIAPBlockDevice : public mbed::BlockDevice {
public:

    /** Creates a FlashIAPBlockDevice
     *
     *  @param address  Physical address where the block device start
     *  @param size     The block device size
     */
    FlashIAPBlockDevice(uint32_t address = MBED_CONF_FLASHIAP_BLOCK_DEVICE_BASE_ADDRESS,
                        uint32_t size = MBED_CONF_FLASHIAP_BLOCK_DEVICE_SIZE);

    virtual ~FlashIAPBlockDevice();

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
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int erase(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual mbed::bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual mbed::bd_size_t get_program_size() const;

    /** Get the size of a eraseable block
     *
     *  @return         Size of a eraseable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size() const;

    /** Get the size of an erasable block given address
     *
     *  @param addr     Address within the erasable block
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size(mbed::bd_addr_t addr) const;

    /** Get the value of storage when erased
     *
     *  @return         The value of storage when erased
     */
    virtual int get_erase_value() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual mbed::bd_size_t size() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const;

    /** Convenience function for checking block erase validity
    *
    *  @param addr     Address of block to begin erasing
    *  @param size     Size to erase in bytes
    *  @return         True if erase is valid for underlying block device
    */
    virtual bool is_valid_erase(bd_addr_t addr, bd_size_t size) const;


private:
    // Device configuration
    mbed::FlashIAP _flash;
    mbed::bd_addr_t _base;
    mbed::bd_size_t _size;
    bool _is_initialized;
    uint32_t _init_ref_count;
};

#endif /* DEVICE_FLASH */
#endif /* MBED_FLASHIAP_BLOCK_DEVICE_H */
