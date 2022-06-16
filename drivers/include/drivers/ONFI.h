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

#ifndef MBED_ONFI_H
#define MBED_ONFI_H

#if DEVICE_ONFI

#include "onfi_api.h"
#include "platform/PlatformMutex.h"
#include <algorithm>
#include "platform/SingletonPtr.h"

namespace mbed {

/** \addtogroup drivers-public-api */
/** @{*/

/**
 * \defgroup drivers_ONFI ONFI class
 * @{
 */

/** ONFI driver. It invokes ONFI HAL functions.
 *
 * @note Synchronization level: Thread safe
 */
class ONFI : private NonCopyable<ONFI> {
public:
    ONFI(PinName d0, PinName d1, PinName d2, PinName d3, PinName d4, PinName d5, PinName d6, PinName d7,
              PinName add09, PinName add11, PinName add12, PinName ba1, PinName rdy, PinName csx);

    /** Initialize a ONFI NAND Flash device
     *
     *  Should be called once per lifetime of the object.
     *  @return 0 on success or a negative error code on failure
     */
    int init();

    /** Deinitialize a ONFI NAND Flash device
     *
     *  @return 0 on success or a negative error code on failure
     */
    int deinit();

    /** Read data from a ONFI NAND Flash device.
     *
     *  This method invokes memcpy - reads number of bytes from the address
     *
     *  @param addr   Flash address to begin reading from
     *  @param buffer Buffer to write to     
     *  @param size   Size to read in bytes
     *  @return       0 on success, negative error code on failure
     */
    int read(uint32_t addr, uint8_t *buffer, uint32_t size);

    /** write data to pages
     *
     *  The sectors must have been erased prior to being programmed
     *
     *  @param addr   Address of a page to begin writing to
     *  @param buffer Buffer of data to be written     
     *  @param size   Size to write in bytes, must be a multiple of program size
     *  @return       0 on success, negative error code on failure
     */
    int write(uint32_t addr, const void *buffer, uint32_t size);

    /** Erase blocks
     *
     *  The state of an erased sector is undefined until it has been programmed
     *
     *  @param addr Address of a sector to begin erasing, must be a multiple of the sector size
     *  @param size Size to erase in bytes, must be a multiple of the sector size
     *  @return     0 on success, negative error code on failure
     */
    int erase(uint32_t addr, uint32_t size);

protected:
    /** Acquire exclusive access to this SPI bus
     */
    virtual void lock(void);

    /** Release exclusive access to this SPI bus
     */
    virtual void unlock(void);

    onfi_t _onfi;

    bool _initialized;

    PinName _onfi_d0, _onfi_d1, _onfi_d2, _onfi_d3, _onfi_d4, _onfi_d5, _onfi_d6, _onfi_d7,
            _onfi_add09, _onfi_add11, _onfi_add12, _onfi_ba1, _onfi_rdy, _onfi_csx;

    static SingletonPtr<PlatformMutex> _mutex;

};

} /* namespace mbed */
#endif /* DEVICE_NANDFLASH */

#endif  /* MBED_NANDFLASH_H */
