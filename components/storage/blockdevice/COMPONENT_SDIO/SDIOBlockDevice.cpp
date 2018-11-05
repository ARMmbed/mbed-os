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

#include <errno.h>
#include "platform/mbed_debug.h"
#include "platform/mbed_wait_api.h"
#include "SDIOBlockDevice.h"

namespace mbed
{

/*
 *  defines
 */

#define SD_DBG 0       /*!< 1 - Enable debugging */
#define SD_CMD_TRACE 0 /*!< 1 - Enable SD command tracing */

#define SD_BLOCK_DEVICE_ERROR_WOULD_BLOCK -5001           /*!< operation would block */
#define SD_BLOCK_DEVICE_ERROR_UNSUPPORTED -5002           /*!< unsupported operation */
#define SD_BLOCK_DEVICE_ERROR_PARAMETER -5003             /*!< invalid parameter */
#define SD_BLOCK_DEVICE_ERROR_NO_INIT -5004               /*!< uninitialized */
#define SD_BLOCK_DEVICE_ERROR_NO_DEVICE -5005             /*!< device is missing or not connected */
#define SD_BLOCK_DEVICE_ERROR_WRITE_PROTECTED -5006       /*!< write protected */
#define SD_BLOCK_DEVICE_ERROR_UNUSABLE -5007              /*!< unusable card */
#define SD_BLOCK_DEVICE_ERROR_NO_RESPONSE -5008           /*!< No response from device */
#define SD_BLOCK_DEVICE_ERROR_CRC -5009                   /*!< CRC error */
#define SD_BLOCK_DEVICE_ERROR_ERASE -5010                 /*!< Erase error: reset/sequence */
#define SD_BLOCK_DEVICE_ERROR_WRITE -5011                 /*!< SPI Write error: !SPI_DATA_ACCEPTED */
#define SD_BLOCK_DEVICE_ERROR_UNSUPPORTED_BLOCKSIZE -5012 /*!< unsupported blocksize, only 512 byte supported */
#define SD_BLOCK_DEVICE_ERROR_READBLOCKS -5013            /*!< read data blocks from SD failed */
#define SD_BLOCK_DEVICE_ERROR_WRITEBLOCKS -5014           /*!< write data blocks to SD failed */
#define SD_BLOCK_DEVICE_ERROR_ERASEBLOCKS -5015           /*!< erase data blocks to SD failed */

#define BLOCK_SIZE_HC 512 /*!< Block size supported for SD card is 512 bytes  */

// Types
#define SDCARD_NONE 0  /**< No card is present */
#define SDCARD_V1 1    /**< v1.x Standard Capacity */
#define SDCARD_V2 2    /**< v2.x Standard capacity SD card */
#define SDCARD_V2HC 3  /**< v2.x High capacity SD card */
#define CARD_UNKNOWN 4 /**< Unknown or unsupported card */

#ifndef MBED_CONF_SD_TIMEOUT
#define MBED_CONF_SD_TIMEOUT (30 * 1000) /* ms */
#endif

SDIOBlockDevice::SDIOBlockDevice(PinName cardDetect) : _cardDetect(cardDetect),
                                                       _is_initialized(0),
                                                       _sectors(0),
                                                       _init_ref_count(0)
{
    _card_type = SDCARD_NONE;

    // Only HC block size is supported.
    _block_size = BLOCK_SIZE_HC;
    _erase_size = BLOCK_SIZE_HC;
}

SDIOBlockDevice::~SDIOBlockDevice()
{
    if (_is_initialized)
    {
        deinit();
    }
}

int SDIOBlockDevice::init()
{
    debug_if(SD_DBG, "init Card...\r\n");

    lock();

    if (!_is_initialized)
    {
        _init_ref_count = 0;
    }

    _init_ref_count++;

    if (_init_ref_count != 1)
    {
        unlock();
        return BD_ERROR_OK;
    }

    if (isPresent() == false)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_DEVICE;
    }

    int status = SD_Init();
    if (BD_ERROR_OK != status)
    {
        unlock();
        return BD_ERROR_DEVICE_ERROR;
    }

    SD_GetCardInfo(&_cardInfo);
    _is_initialized = true;
    debug_if(SD_DBG, "SD initialized: type: %d  version: %d  class: %d\n",
             _cardInfo.CardType, _cardInfo.CardVersion, _cardInfo.Class);
    debug_if(SD_DBG, "SD size: %d MB\n",
             _cardInfo.LogBlockNbr / 2 / 1024);

    // get sectors count from cardinfo
    _sectors = _cardInfo.LogBlockNbr;
    if (BLOCK_SIZE_HC != _cardInfo.BlockSize)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_UNSUPPORTED_BLOCKSIZE;
    }

    unlock();
    return status;
}

int SDIOBlockDevice::deinit()
{
    debug_if(SD_DBG, "deinit Card...\r\n");
    lock();

    if (!_is_initialized)
    {
        _init_ref_count = 0;
        unlock();
        return BD_ERROR_OK;
    }

    _init_ref_count--;

    if (_init_ref_count)
    {
        unlock();
        return BD_ERROR_OK;
    }

    int status = SD_DeInit();
    _is_initialized = false;

    _sectors = 0;

    unlock();
    return status;
}

int SDIOBlockDevice::read(void *b, bd_addr_t addr, bd_size_t size)
{
    //debug_if(SD_DBG, "read Card...\r\n");
    lock();
    if (isPresent() == false)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_DEVICE;
    }
    if (!is_valid_read(addr, size))
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }

    if (!_is_initialized)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_INIT;
    }

    uint32_t *buffer = static_cast<uint32_t *>(b);

    // ReadBlocks uses byte unit address
    // SDHC and SDXC Cards different addressing is handled in ReadBlocks()
    bd_addr_t blockCnt = size / _block_size;
    addr = addr / _block_size;

    // make sure card is ready
    while (SD_GetCardState() != SD_TRANSFER_OK)
    {
        // wait until SD ready
        wait_ms(1);
    }

    // receive the data : one block/ multiple blocks is handled in ReadBlocks()
    int status = SD_ReadBlocks_DMA(buffer, addr, blockCnt);
    debug_if(SD_DBG, "ReadBlocks dbgtest addr: %lld  blockCnt: %lld \n", addr, blockCnt);

    if (status == MSD_OK)
    {
        // wait until DMA finished
        while (SD_DMA_ReadPending() != SD_TRANSFER_OK)
        {
            uint32_t tickstart = HAL_GetTick();
            if ((HAL_GetTick() - tickstart) >= MBED_CONF_SD_TIMEOUT)
            {
                unlock();
                return SD_BLOCK_DEVICE_ERROR_READBLOCKS;
            }
        }
        // make sure card is ready
        while (SD_GetCardState() != SD_TRANSFER_OK)
        {
            // wait until SD ready
            wait_ms(10);
        }
    }
    else
    {
        debug_if(SD_DBG, "ReadBlocks failed! addr: %lld  blockCnt: %lld \n", addr, blockCnt);
        debug_if(SD_DBG, "  hsd.errorcode: %lu  0x%lx\n", hsd.ErrorCode, hsd.ErrorCode);
        status = SD_BLOCK_DEVICE_ERROR_READBLOCKS;
    }

    unlock();
    return status;
}

int SDIOBlockDevice::program(const void *b, bd_addr_t addr, bd_size_t size)
{
    //debug_if(SD_DBG, "program Card...\r\n");
    lock();

    if (isPresent() == false)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_DEVICE;
    }
    if (!is_valid_program(addr, size))
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }

    if (!_is_initialized)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_INIT;
    }

    //uint32_t *buffer = static_cast< uint32_t *>(b);
    // HAL layer uses uint32_t for addr/size
    uint32_t *buffer = (uint32_t *)(b);

    // Get block count
    bd_size_t blockCnt = size / _block_size;
    addr = addr / _block_size;

    // make sure card is ready
    while (SD_GetCardState() != SD_TRANSFER_OK)
    {
        // wait until SD ready
        wait_ms(1);
    }

    int status = SD_WriteBlocks_DMA(buffer, addr, blockCnt);
    debug_if(SD_DBG, "WriteBlocks dbgtest addr: %lld  blockCnt: %lld \n", addr, blockCnt);

    if (status == MSD_OK)
    {
        // wait until DMA finished
        while (SD_DMA_WritePending() != SD_TRANSFER_OK)
        {
            uint32_t tickstart = HAL_GetTick();
            if ((HAL_GetTick() - tickstart) >= MBED_CONF_SD_TIMEOUT)
            {
                unlock();
                status = SD_BLOCK_DEVICE_ERROR_WRITEBLOCKS;
            }
        }
        // make sure card is ready
        while (SD_GetCardState() != SD_TRANSFER_OK)
        {
            // wait until SD ready
            wait_ms(1);
        }
    }
    else
    {
        debug_if(SD_DBG, "WriteBlocks failed! addr: %lld  blockCnt: %lld \n", addr, blockCnt);
        debug_if(SD_DBG, "  hsd.errorcode: %lu  0x%lx\n", hsd.ErrorCode, hsd.ErrorCode);
        status = SD_BLOCK_DEVICE_ERROR_WRITEBLOCKS;
    }

    unlock();
    return status;
}

int SDIOBlockDevice::trim(bd_addr_t addr, bd_size_t size)
{
    debug_if(SD_DBG, "trim Card...\r\n");
    lock();
    if (isPresent() == false)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_DEVICE;
    }
    if (!_is_valid_trim(addr, size))
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_PARAMETER;
    }

    if (!_is_initialized)
    {
        unlock();
        return SD_BLOCK_DEVICE_ERROR_NO_INIT;
    }

    bd_size_t blockCnt = size / _block_size;
    addr = addr / _block_size;

    int status = SD_Erase(addr, blockCnt);
    if (status != 0)
    {
        debug_if(SD_DBG, "Erase blocks failed! addr: %lld  blockCnt: %lld \n", addr, blockCnt);
        status = SD_BLOCK_DEVICE_ERROR_ERASEBLOCKS;
    }
    else
    {
        while (SD_GetCardState() != SD_TRANSFER_OK)
        {
            // wait until SD ready
            wait_ms(10);
        }
    }

    unlock();
    return status;
}

bd_size_t SDIOBlockDevice::get_read_size() const
{
    return _block_size;
}

bd_size_t SDIOBlockDevice::get_program_size() const
{
    return _block_size;
}

bd_size_t SDIOBlockDevice::size() const
{
    return _block_size * _sectors;
}

void SDIOBlockDevice::debug(bool dbg)
{
}

bool SDIOBlockDevice::_is_valid_trim(bd_addr_t addr, bd_size_t size)
{
    return (
        addr % _erase_size == 0 &&
        size % _erase_size == 0 &&
        addr + size <= this->size());
}

bool SDIOBlockDevice::isPresent(void)
{
    if (_cardDetect.is_connected())
        return (_cardDetect.read() == 0);
    else
        return true;
}

} // namespace mbed
