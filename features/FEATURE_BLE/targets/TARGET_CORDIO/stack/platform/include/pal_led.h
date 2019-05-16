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
 *  \brief LED driver definition.
 */
/*************************************************************************************************/

#ifndef PAL_LED_H
#define PAL_LED_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_LED
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Reserved LED IDs. */
typedef enum
{
  /* Common debug signals. */
  PAL_LED_ID_CPU_ACTIVE     = 0x18,  /*!< CPU active LED ID. */
  PAL_LED_ID_ERROR          = 0x19,  /*!< Error LED ID. */
} PalLedId_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalLedInit(void);
void PalLedDeInit(void);

/* Control and Status */
void PalLedOn(uint8_t id);
void PalLedOff(uint8_t id);
void PalLedOnGroup(uint32_t mask);
void PalLedOffGroup(uint32_t mask);

/*! \} */    /* PAL_LED */

#ifdef __cplusplus
};
#endif

#endif /* PAL_LED_H */
