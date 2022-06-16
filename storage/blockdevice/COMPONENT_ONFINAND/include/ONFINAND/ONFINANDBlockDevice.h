/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
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

#ifndef MBED_ONFINAND_BLOCK_DEVICE_H
#define MBED_ONFINAND_BLOCK_DEVICE_H

#include "drivers/ONFI.h"
#include "blockdevice/BlockDevice.h"
#include "platform/Callback.h"

#ifndef MBED_CONF_ONFINAND_D0
#define MBED_CONF_ONFINAND_D0 NC
#endif
#ifndef MBED_CONF_ONFINAND_D1
#define MBED_CONF_ONFINAND_D1 NC
#endif
#ifndef MBED_CONF_ONFINAND_D2
#define MBED_CONF_ONFINAND_D2 NC
#endif
#ifndef MBED_CONF_ONFINAND_D3
#define MBED_CONF_ONFINAND_D3 NC
#endif
#ifndef MBED_CONF_ONFINAND_D4
#define MBED_CONF_ONFINAND_D4 NC
#endif
#ifndef MBED_CONF_ONFINAND_D5
#define MBED_CONF_ONFINAND_D5 NC
#endif
#ifndef MBED_CONF_ONFINAND_D6
#define MBED_CONF_ONFINAND_D6 NC
#endif
#ifndef MBED_CONF_ONFINAND_D7
#define MBED_CONF_ONFINAND_D7 NC
#endif
#ifndef MBED_CONF_ONFINAND_ADD09
#define MBED_CONF_ONFINAND_ADD09 NC
#endif
#ifndef MBED_CONF_ONFINAND_ADD11
#define MBED_CONF_ONFINAND_ADD11 NC
#endif
#ifndef MBED_CONF_ONFINAND_ADD12
#define MBED_CONF_ONFINAND_ADD12 NC
#endif
#ifndef MBED_CONF_ONFINAND_BA1
#define MBED_CONF_ONFINAND_BA1 NC
#endif
#ifndef MBED_CONF_ONFINAND_RDY
#define MBED_CONF_ONFINAND_RDY NC
#endif
#ifndef MBED_CONF_ONFINAND_CSX
#define MBED_CONF_ONFINAND_CSX NC
#endif

enum onfinand_bd_error {
    ONFINAND_BD_ERROR_OK                    = 0,     /*!< no error */
    ONFINAND_BD_ERROR_DEVICE_ERROR          = BD_ERROR_DEVICE_ERROR, /*!< device specific error -4001 */
    ONFINAND_BD_ERROR_INVALID_ERASE_PARAMS  = -4005, /* Erase command not on sector aligned addresses or exceeds device size */
};

class ONFINANDBlockDevice : public mbed::BlockDevice {
public:
    /** Create ONFINANDBlockDevice - An ONFI NAND Flash Block Device over SEMC bus
     *
     *  @param d0 1st IO pin used for sending/receiving data during data phase of a transaction
     *  @param d1 2nd IO pin used for sending/receiving data during data phase of a transaction
     *  @param d2 3rd IO pin used for sending/receiving data during data phase of a transaction
     *  @param d3 4th IO pin used for sending/receiving data during data phase of a transaction
     *  @param d4 5th IO pin used for sending/receiving data during data phase of a transaction
     *  @param d5 6th IO pin used for sending/receiving data during data phase of a transaction
     *  @param d6 7th IO pin used for sending/receiving data during data phase of a transaction
     *  @param d7 8th IO pin used for sending/receiving data during data phase of a transaction
     *  @param add09 1st IO pin used for sending address during address phase
     *  @param add11 2nd IO pin used for sending address during address phase
     *  @param add12 3rd IO pin used for sending address during address phase
     *  @param ba1 SEMC_BA1
     *  @param rdy pin used for checking Ready/Busy status
     *  @param csx chip select pin
     */
    ONFINANDBlockDevice(PinName d0 = MBED_CONF_ONFINAND_D0, PinName d1 = MBED_CONF_ONFINAND_D1, PinName d2 = MBED_CONF_ONFINAND_D2, PinName d3 = MBED_CONF_ONFINAND_D3,
                        PinName d4 = MBED_CONF_ONFINAND_D4, PinName d5 = MBED_CONF_ONFINAND_D5, PinName d6 = MBED_CONF_ONFINAND_D6, PinName d7 = MBED_CONF_ONFINAND_D7,
                        PinName add09 = MBED_CONF_ONFINAND_ADD09, PinName add11 = MBED_CONF_ONFINAND_ADD11, PinName add12 = MBED_CONF_ONFINAND_ADD12, PinName ba1 = MBED_CONF_ONFINAND_BA1,
                        PinName rdy = MBED_CONF_ONFINAND_RDY, PinName csx = MBED_CONF_ONFINAND_CSX);

    /** Initialize a block device
     *
     *  @return         ONFINAND_BD_ERROR_OK(0) - success
     *                  ONFINAND_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         ONFINAND_BD_ERROR_OK(0) - success
     *                  ONFINAND_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int deinit();

    /** Destruct ONFINANDBlockDevie
      */
    ~ONFINANDBlockDevice()
    {
        deinit();
    }

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         ONFINAND_BD_ERROR_OK(0) - success
     *                  ONFINAND_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         ONFINAND_BD_ERROR_OK(0) - success
     *                  ONFINAND_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         ONFINAND_BD_ERROR_OK(0) - success
     *                  ONFINAND_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  ONFINAND_BD_ERROR_INVALID_ERASE_PARAMS - Trying to erase unaligned address or size
     */
    virtual int erase(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual mbed::bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a program block size in bytes
     *  @note Must be a multiple of the read size
     */
    virtual mbed::bd_size_t get_program_size() const;

    /** Get the size of a eraseable block
     *
     *  @return         Size of a minimal erase block, common to all regions, in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size() const;

    virtual mbed::bd_size_t get_erase_size(bd_addr_t addr) const;

    /** Get the value of storage byte after it was erased
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
    virtual mbed::bd_size_t size() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const;

private:

    mbed:: ONFI _onfi;

    static SingletonPtr<PlatformMutex> _devices_mutex;

    uint32_t _init_ref_count;

    bool _is_initialized;

    PlatformMutex _mutex;
};

#endif
