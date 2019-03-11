/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Pal NVM driver.
 */
/*************************************************************************************************/

#ifndef PAL_NVM_H
#define PAL_NVM_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_NVM
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! NVM sector size. */
#define PAL_NVM_SECTOR_SIZE 0x1000

/*! NVM word size. */
#define PAL_NVM_WORD_SIZE   4

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Callback. This may be returned in interrupt context. */
typedef void (*PalNvmCback_t)(bool_t status);

/*! \brief      Operational states. */
typedef enum
{
  PAL_NVM_STATE_UNINIT = 0,   /*!< Uninitialized state. */
  PAL_NVM_STATE_ERROR  = 0,   /*!< Error state. */
  PAL_NVM_STATE_READY,        /*!< Ready state. */
  PAL_NVM_STATE_BUSY          /*!< Busy state. */
} PalNvmState_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalNvmInit(PalNvmCback_t actCback);
void PalNvmDeInit(void);

/* Control and Status */
PalNvmState_t PalNvmGetState(void);

/* Data Transfer */
void PalNvmRead(void *pBuf, uint32_t size, uint32_t srcAddr);
void PalNvmWrite(void *pBuf, uint32_t size, uint32_t dstAddr);
void PalNvmEraseSector(uint32_t size, uint32_t startAddr);

/*! \} */    /* PAL_NVM */

#ifdef __cplusplus
};
#endif

#endif  /* PAL_NVM_H */
