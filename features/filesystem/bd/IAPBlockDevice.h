/* Copyright (c) 2018, James Wang
 * SPDX-License-Identifier: Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.

 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.

 * See the License for the specific language governing permissions and limitations under the License.
 */
#ifndef MBED_IAP_BLOCK_DEVICE_H
#define MBED_IAP_BLOCK_DEVICE_H

#include "BlockDevice.h"
#include "FlashIAP.h"
#if defined(DEVICE_FLASH) || defined(DOXYGEN_ONLY)
/** In Application Programming block device
 *
 * Useful for store data in User Flash
 *
 * @code
 * #include "mbed.h"
 * #include "IAPBlockDevice.h"
 *
 * #define USER_FLASH_STORE_START (0x00040000)
 * #define USER_FLASH_STORE_SIZE  (256 * 1024)
 *
 *
 * int main() {
 *     FlashIAP flash;
 *     flash.init();
 *     const uint32_t flash_end = flash.get_flash_start() + flash.get_flash_size();
 *     const uint32_t last_sector_size = flash.get_sector_size(flash_end - 1);
 * 
 *     // use the last sector for test
 *     const uint32_t bd_start_addr = flash_end - last_sector_size;
 *     const uint32_t bd_size = last_sector_size;
 *     IAPBlockDevice bd(bd_start_addr, bd_size);
 * 
 *     bd.init();
 *     char * block = static_cast<char*>(malloc(bd_size));
 *     if (block != 0)
 *     {
 *         strcpy(block, "Hello World!\n");
 *         bd.erase(0, BLOCK_SIZE);
 *         bd.program(block, 0, BLOCK_SIZE);
 *         memset(block, 0, bd_size);
 *         bd.read(block, 0, BLOCK_SIZE);
 *         printf("%s", block);
 *         free(block);
 *         block = 0;
 *     }
 *     bd.deinit();
 * }
 * @endcode
 */

enum
{
    BD_ERROR_INVALID_ADDR = -5101,
    BD_ERROR_INVALID_SIZE = -5102,
    BD_ERROR_IAP_ERROR = -5103,
};

class IAPBlockDevice : public BlockDevice
{
  public:
    /** Lifetime of the In Application Programming
     *
     * @param addr     Block start addr in User Flash, must be a multiple of Flash program and sector sizes
     * @param size     Block size in bytes. must be a multiple of Flash program and sector sizes
     */
    IAPBlockDevice(uint32_t addr, uint32_t size)
        : _addr(addr), _size(size)
    {
    }

    /** Lifetime of the In Application Programming     
     * 
     */
    virtual ~IAPBlockDevice()
    {
    }

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int init()
    {
        const int flash_init_ret = _internal_flash.init();
        if (flash_init_ret != 0)
        {
            return BD_ERROR_IAP_ERROR;
        }
        const uint32_t flash_start = _internal_flash.get_flash_start();
        if (flash_start > _addr)
        {
            return BD_ERROR_INVALID_ADDR;
        }
        const uint32_t flash_size = _internal_flash.get_flash_size();
        if (flash_start + flash_size < _addr + _size)
        {
            return BD_ERROR_INVALID_SIZE;
        }
        const uint32_t page_size = _internal_flash.get_page_size();
        if (_size % page_size != 0)
        {
            return BD_ERROR_INVALID_SIZE;
        }
        if ((_addr - flash_start) % page_size != 0)
        {
            return BD_ERROR_INVALID_SIZE;
        }
        return BD_ERROR_OK;
    }

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int deinit()
    {
        const int flash_deinit_ret = _internal_flash.deinit();
        if (flash_deinit_ret != 0)
        {
            return BD_ERROR_IAP_ERROR;
        }
        return BD_ERROR_OK;
    }

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to read blocks into
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size)
    {
        if (addr + size > _size)
        {
            return BD_ERROR_INVALID_SIZE;
        }
        const int flash_ret = _internal_flash.read(buffer, _addr + addr, size);
        if (flash_ret != 0)
        {
            return BD_ERROR_IAP_ERROR;
        }
        return BD_ERROR_OK;
    }

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size)
    {
        if (addr + size > _size)
        {
            return BD_ERROR_INVALID_SIZE;
        }
        const int flash_ret = _internal_flash.program(buffer, _addr + addr, size);
        if (flash_ret != 0)
        {
            return BD_ERROR_IAP_ERROR;
        }
        return BD_ERROR_OK;
    }

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int erase(bd_addr_t addr, bd_size_t size)
    {
        if (addr + size > _size)
        {
            return BD_ERROR_INVALID_SIZE;
        }
        const int flash_ret = _internal_flash.erase(_addr + addr, size);
        if (flash_ret != 0)
        {
            return BD_ERROR_IAP_ERROR;
        }
        return BD_ERROR_OK;
    }

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const
    {
        return _internal_flash.get_page_size();
    }

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     */
    virtual bd_size_t get_program_size() const
    {
        return _internal_flash.get_page_size();
    }

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const
    {
        return _size;
    }

  private:
    mbed::FlashIAP _internal_flash;
    uint32_t _addr;
    uint32_t _size;
};
#endif /* DEVICE_FLASH */
#endif /* MBED_IAP_BLOCK_DEVICE_H */