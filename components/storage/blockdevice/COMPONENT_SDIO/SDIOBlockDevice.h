/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_OS_SDIO_BLOCK_DEVICE_H_
#define MBED_OS_SDIO_BLOCK_DEVICE_H_

#if DEVICE_SDIO

#include "BlockDevice.h"
#include "DigitalIn.h"
#include "PlatformMutex.h"
#include "sdio_api.h"

class SDIOBlockDevice : public mbed::BlockDevice {
public:
    SDIOBlockDevice(PinName cardDetect = NC);
    virtual ~SDIOBlockDevice();
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
    virtual int trim(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const;

    /** Enable or disable debugging
     *
     *  @param dbg        State of debugging
     */
    virtual void debug(bool dbg);

    /** Set the transfer frequency
     *
     *  @param freq     Transfer frequency
     *  @note Max frequency supported is 25MHZ
     */
    virtual int frequency(uint64_t freq)
    {
        return BD_ERROR_OK;
    };

    /** check if SD is present
     *
     *  @note check physical present switch. Maybe not support by hardware, then function will always return true.
     */
    virtual bool isPresent(void);

    /** Get the BlockDevice class type.
     *
     *  @return         A string representation of the BlockDevice class type.
     */
    virtual const char *get_type() const;

private:
    mbed::DigitalIn _cardDetect;
    bool _is_initialized;
    bd_size_t _block_size;
    bd_size_t _erase_size;
    bd_size_t _sectors;
    uint32_t _init_ref_count;
    SDIO_Cardinfo_t _cardInfo;

    PlatformMutex _mutex;
    virtual void lock()
    {
        _mutex.lock();
    }

    virtual void unlock()
    {
        _mutex.unlock();
    }

    bool _is_valid_trim(bd_addr_t addr, bd_size_t size);
};

#endif // DEVICE_SDIO
#endif // MBED_OS_SDIO_BLOCK_DEVICE_H_
