/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
 *  @enum bd_error_t
 */
enum bd_error {
    BD_ERROR_OK                 = 0,     /*!< no error */
    BD_ERROR_WOULD_BLOCK        = -4001, /*!< operation would block */
    BD_ERROR_UNSUPPORTED        = -4002, /*!< unsupported operation */
    BD_ERROR_PARAMETER          = -4003, /*!< invalid parameter */
    BD_ERROR_NO_INIT            = -4004, /*!< uninitialized */
    BD_ERROR_NO_DEVICE          = -4005, /*!< device is missing or not connected */
    BD_ERROR_WRITE_PROTECTED    = -4006, /*!< write protected */
    BD_ERROR_DEVICE_ERROR       = -4007, /*!< device specific error */
};

/** Type representing error codes
 */
typedef int32_t bd_error_t;

/** Type representing the address of a specific block
 */
typedef uint32_t bd_block_t;

/** Type representing a quantity of 8-bit bytes
 */
typedef uint32_t bd_size_t;

/** Type representing a count of blocks
 */
typedef uint32_t bd_count_t;

/** Type representing either a count of blocks or a negative error code
 */
typedef int32_t bd_count_or_error_t;


/** A hardware device capable of writing and reading blocks
 */
class BlockDevice {
public:
    /** Lifetime of a block device
     */
    virtual ~BlockDevice() {};

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t deinit();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write block to
     *  @param block    Index of block to begin reading from
     *  @param count    Number of blocks to read (defaults to 1)
     *  @return         Either the number of blocks read or a negative error code on failure
     */
    virtual bd_count_or_error_t read(void *buffer, bd_block_t block, bd_count_t count = 1) = 0;

    /** Write blocks to a block device
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param block    Index of block to begin writing to
     *  @param count    Number of blocks to write (defaults to 1)
     *  @return         Either the number of blocks written or a negative error code on failure
     */
    virtual bd_count_or_error_t write(const void *buffer, bd_block_t block, bd_count_t count = 1) = 0;

    /** Sync an underlying block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t sync();

    /** Queries the status of the underlying block device
     *
     *  @return         Status of the underlying device, this is 0 for
     *                  a ready device, or a negative error code if a read
     *                  or write could fail.
     */
    virtual bd_error_t status();

    /** Get the block size of the underlying device
     *
     *  @return         The number of 8-bit bytes in a block
     */
    virtual bd_size_t get_block_size() = 0;

    /** Get the block count of the underlying device
     *
     *  @return         The number of blocks on the device
     */
    virtual bd_count_t get_block_count() = 0;
};


#endif
