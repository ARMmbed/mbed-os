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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDIO_DEVICE_H
#define __SDIO_DEVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"

  /* Typedefs */

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
  } SD_Cardinfo_t;

  /* External Global var  */

  extern SD_HandleTypeDef hsd;

/* Exported types */
/** 
  * @brief SD Card information structure 
  */
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef

/* Exported constants */
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

  /* Exported functions */
  uint8_t SD_Init(void);
  uint8_t SD_DeInit(void);
  uint8_t SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
  uint8_t SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
  uint8_t SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
  uint8_t SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
  uint8_t SD_DMA_ReadPending(void);
  uint8_t SD_DMA_WritePending(void);
  uint8_t SD_Erase(uint32_t StartAddr, uint32_t EndAddr);

  uint8_t SD_GetCardState(void);
  void SD_GetCardInfo(SD_Cardinfo_t *CardInfo);

  /* callback function for DMA Rx/Tx completete, called by HAL SDIO interrupt handler */
  void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd);
  void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd);

#ifdef __cplusplus
}
#endif

#endif /* __SDIO_DEVICE_H */
