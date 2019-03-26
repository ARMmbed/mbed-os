
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

typedef struct
{
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
#define MSD_OK ((uint8_t)0x00)
#define MSD_ERROR ((uint8_t)0x01)

/**
  * @brief  SD transfer state definition
  */
#define SD_TRANSFER_OK ((uint8_t)0x00)
#define SD_TRANSFER_BUSY ((uint8_t)0x01)


uint8_t sdio_init(void);
uint8_t sdio_deinit(void);
uint8_t sdio_readblocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t sdio_writeblocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);

#if DEVICE_SDIO_ASYNC

uint8_t sdio_readblocks_async(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t sdio_writeblocks_async(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t sdio_read_pending(void);
uint8_t sdio_write_pending(void);

#endif // DEVICE_SDIO_ASYNC

uint8_t sdio_erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t sdio_get_card_state(void);
void sdio_get_card_info(SDIO_Cardinfo_t *CardInfo);

/**@}*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DEVICE_SDIO

#endif // MBED_SDIO_API_H


// TODO: check if this could be usefull
/** @}*/

// #if DEVICE_SDIO_ASYNCH
// /** Asynch SPI HAL structure
//  */
// typedef struct {
//     struct spi_s spi;        /**< Target specific SPI structure */
//     struct buffer_s tx_buff; /**< Tx buffer */
//     struct buffer_s rx_buff; /**< Rx buffer */
// } spi_t;

// #else
// /** Non-asynch SPI HAL structure
//  */
// typedef struct spi_s spi_t;

// #endif
