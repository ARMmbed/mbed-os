/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef MBED_BLOCK_DEVICE_H
#define MBED_BLOCK_DEVICE_H

#include <stdint.h>


/** Enum of standard error codes
 *
 *  @enum bd_error
 */
enum bd_error {
    BD_ERROR_OK                 = 0,     /*!< no error */
    BD_ERROR_DEVICE_ERROR       = -4001, /*!< device specific error */
};

/** Type representing the address of a specific block
 */
typedef uint64_t bd_addr_t;

/** Type representing a quantity of 8-bit bytes
 */
typedef uint64_t bd_size_t;


/** A hardware device capable of writing and reading blocks
 */
class BlockDevice
{
public:
    /** Lifetime of a block device
     */
    virtual ~BlockDevice() {};

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int init() = 0;

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int deinit() = 0;

    /** Ensure data on storage is in sync with the driver
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int sync()
    {
        return 0;
    }

    /** Read blocks from a block device
     *
     *  If a failure occurs, it is not possible to determine how many bytes succeeded
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size) = 0;

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  If a failure occurs, it is not possible to determine how many bytes succeeded
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size) = 0;

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed,
     *  unless get_erase_value returns a non-negative byte value
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int erase(bd_addr_t addr, bd_size_t size)
    {
        return 0;
    }

    /** Mark blocks as no longer in use
     *
     *  This function provides a hint to the underlying block device that a region of blocks
     *  is no longer in use and may be erased without side effects. Erase must still be called
     *  before programming, but trimming allows flash-translation-layers to schedule erases when
     *  the device is not busy.
     *
     *  @param addr     Address of block to mark as unused
     *  @param size     Size to mark as unused in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int trim(bd_addr_t addr, bd_size_t size)
    {
        return 0;
    }

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const = 0;

    /** Get the size of a programmable block
     *
     *  @return         Size of a programmable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size() const = 0;

    /** Get the size of an erasable block
     *
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size() const
    {
        return get_program_size();
    }

    /** Get the size of an erasable block given address
     *
     *  @param addr     Address within the erasable block
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size(bd_addr_t addr) const
    {
        return get_erase_size();
    }

    /** Get the value of storage when erased
     *
     *  If get_erase_value returns a non-negative byte value, the underlying
     *  storage is set to that value when erased, and storage containing
     *  that value can be programmed without another erase.
     *
     *  @return         The value of storage when erased, or -1 if you can't
     *                  rely on the value of erased storage
     */
    virtual int get_erase_value() const
    {
        return -1;
    }

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const = 0;

    /** Convenience function for checking block read validity
     *
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes
     *  @return         True if read is valid for underlying block device
     */
    bool is_valid_read(bd_addr_t addr, bd_size_t size) const
    {
        return (
            addr % get_read_size() == 0 &&
            size % get_read_size() == 0 &&
            addr + size <= this->size());
    }

    /** Convenience function for checking block program validity
     *
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes
     *  @return         True if program is valid for underlying block device
     */
    bool is_valid_program(bd_addr_t addr, bd_size_t size) const
    {
        return (
            addr % get_program_size() == 0 &&
            size % get_program_size() == 0 &&
            addr + size <= this->size());
    }

    /** Convenience function for checking block erase validity
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes
     *  @return         True if erase is valid for underlying block device
     */
    bool is_valid_erase(bd_addr_t addr, bd_size_t size) const
    {
        return (
            addr % get_erase_size() == 0 &&
            size % get_erase_size() == 0 &&
            addr + size <= this->size());
    }
};


#endif
