
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_SDIO_API_H
#define MBED_SDIO_API_H

#include "device.h"
#include "pinmap.h"

#if DEVICE_SDIO

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \defgroup SDIO Configuration Functions
 * @{
 */

typedef struct {
    uint32_t CardType;     /* Specifies the card Type                         */
    uint32_t CardVersion;  /* Specifies the card version                      */
    uint32_t Class;        /* Specifies the class of the card class           */
    uint32_t RelCardAdd;   /* Specifies the Relative Card Address             */
    uint32_t BlockNbr;     /* Specifies the Card Capacity in blocks           */
    uint32_t BlockSize;    /* Specifies one block size in bytes               */
    uint32_t LogBlockNbr;  /* Specifies the Card logical Capacity in blocks   */
    uint32_t LogBlockSize; /* Specifies logical block size in bytes           */
} SDIO_Cardinfo_t;

/**
  * @brief  SD status structure definition
  */
#define MSD_OK ((int)0x00)
#define MSD_ERROR ((int)0x01)

/**
  * @brief  SD transfer state definition
  */
#define SD_TRANSFER_OK ((int)0x00)
#define SD_TRANSFER_BUSY ((int)0x01)

/**
 * @brief  Initializes the SD card device.
 * @retval SD status
 */
int sdio_init(void);

/**
 * @brief  DeInitializes the SD card device.
 * @retval SD status
 */
int sdio_deinit(void);

/**
 * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @retval SD status
 */
int sdio_readblocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);

/**
 * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @retval SD status
 */
int sdio_writeblocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);

#if DEVICE_SDIO_ASYNC

/**
 * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @retval SD status
 */
int sdio_readblocks_async(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);

/**
 * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @retval SD status
 */
int sdio_writeblocks_async(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);

/**
 * @brief  Check if a DMA operation is pending
 * @retval DMA operation is pending
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
int sdio_read_pending(void);

/**
 * @brief  Check if a DMA operation is pending
 * @retval DMA operation is pending
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
int sdio_write_pending(void);

#endif // DEVICE_SDIO_ASYNC

/**
 * @brief  Erases the specified memory area of the given SD card.
 * @param  StartAddr: Start byte address
 * @param  EndAddr: End byte address
 * @retval SD status
 */
int sdio_erase(uint32_t StartAddr, uint32_t EndAddr);

/**
 * @brief  Gets the current SD card data status.
 * @param  None
 * @retval Data transfer state.
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
int sdio_get_card_state(void);

/**
 * @brief  Get SD information about specific SD card.
 * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
 * @retval None
 */
void sdio_get_card_info(SDIO_Cardinfo_t *CardInfo);

/**@}*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DEVICE_SDIO

#endif // MBED_SDIO_API_H
