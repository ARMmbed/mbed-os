/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  PAL flash driver.
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef PAL_FLASH_H
#define PAL_FLASH_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_FLASH
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Callback. This may be returned in interrupt context. */
typedef void (*PalFlashCback_t)(bool_t status);

/*! \brief      Operational states. */
typedef enum
{
  PAL_FLASH_STATE_UNINIT = 0,   /*!< Uninitialized state. */
  PAL_FLASH_STATE_ERROR  = 0,   /*!< Error state. */
  PAL_FLASH_STATE_READY,        /*!< Ready state. */
  PAL_FLASH_STATE_BUSY          /*!< Busy state. */
} PalFlashState_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalFlashInit(PalFlashCback_t actCback);
void PalFlashDeInit(void);

/* Control and Status */
PalFlashState_t PalNvmGetState(void);
uint32_t PalNvmGetTotalSize(void);
uint32_t PalNvmGetSectorSize(void);

/* Data Transfer */
void PalFlashRead(void *pBuf, uint32_t size, uint32_t srcAddr);
void PalFlashWrite(void *pBuf, uint32_t size, uint32_t dstAddr);
void PalFlashEraseSector(uint32_t size, uint32_t startAddr);
void PalFlashEraseChip(void);

/*! \} */    /* PAL_FLASH */

#ifdef __cplusplus
};
#endif

#endif  /* PAL_FLASH_H */
