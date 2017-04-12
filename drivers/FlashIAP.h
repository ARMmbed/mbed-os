/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MBED_FLASHIAP_H
#define MBED_FLASHIAP_H

#if defined (DEVICE_FLASH) || defined(DOXYGEN_ONLY)

#include "flash_api.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"

namespace mbed {

/** \addtogroup drivers */

/** Flash IAP driver. It invokes flash HAL functions.
 *
 * @note Synchronization level: Thread safe
 * @ingroup drivers
 */
class FlashIAP {
public:
    FlashIAP();
    ~FlashIAP();

    /** Initialize a flash IAP device
     *
     *  Should be called once per lifetime of the object.
     *  @return 0 on success or a negative error code on failure
     */
    int init();

    /** Deinitialize a flash IAP device
     *
     *  @return 0 on success or a negative error code on failure
     */
    int deinit();

    /** Read data from a flash device. 
     *
     *  This method invokes memcpy - reads number of bytes from the address
     *
     *  @param buffer Buffer to write to
     *  @param addr   Flash address to begin reading from
     *  @param size   Size to read in bytes
     *  @return       0 on success, negative error code on failure
     */
    int read(void *buffer, uint32_t addr, uint32_t size);

    /** Program data to pages
     *
     *  The sectors must have been erased prior to being programmed
     *
     *  @param buffer Buffer of data to be written
     *  @param addr   Address of a page to begin writing to, must be a multiple of program and sector sizes
     *  @param size   Size to write in bytes, must be a multiple of program and sector sizes
     *  @return       0 on success, negative error code on failure
     */
    int program(const void *buffer, uint32_t addr, uint32_t size);

    /** Erase sectors
     *
     *  The state of an erased sector is undefined until it has been programmed
     *
     *  @param addr Address of a sector to begin erasing, must be a multiple of the sector size
     *  @param size Size to erase in bytes, must be a multiple of the sector size
     *  @return     0 on success, negative error code on failure
     */
    int erase(uint32_t addr, uint32_t size);

    /** Get the sector size at the defined address
     *
     *  Sector size might differ at address ranges. 
     *  An example <0-0x1000, sector size=1024; 0x10000-0x20000, size=2048>
     *
     *  @param addr Address of or inside the sector to query
     *  @return Size of a sector in bytes or MBED_FLASH_INVALID_SIZE if not mapped
     */
    uint32_t get_sector_size(uint32_t addr) const;

    /** Get the flash start address 
     *
     *  @return Flash start address 
     */
    uint32_t get_flash_start() const;

    /** Get the flash size
     *
     *  @return Flash size 
     */
    uint32_t get_flash_size() const;

    /** Get the program page size
     *
     *  @return Size of a program page in bytes
     */
    uint32_t get_page_size() const;

private:

    /* Check if address and size are aligned to a sector
     *
     *  @param addr Address of block to check for alignment
     *  @param size Size of block to check for alignment
     *  @return true if the block is sector aligned, false otherwise
     */
    bool is_aligned_to_sector(uint32_t addr, uint32_t size);

    flash_t _flash;
    static SingletonPtr<PlatformMutex> _mutex;
};

} /* namespace mbed */

#endif  /* DEVICE_FLASH */

#endif  /* MBED_FLASHIAP_H */
