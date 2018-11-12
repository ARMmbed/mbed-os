/**
 *
 * This sourcecode is derived from STMicroelectronics CubeMX generated code.
 * It is modified to work with mbed RTOS.
 *
 ******************************************************************************
  * @file    bsp_driver_sd.h for F4 (based on stm324x9i_eval_sd.h)
  * @brief   This file contains the common defines and functions prototypes for 
  *          the bsp_driver_sd.c driver.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDIO_DEVICE_H
#define __SDIO_DEVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f7xx_hal.h"

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
