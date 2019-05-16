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
 *  \brief Button driver definition.
 */
/*************************************************************************************************/

#ifndef PAL_BTN_H
#define PAL_BTN_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_BUTTON
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Button state. */
typedef enum
{
  PAL_BTN_STATE_UP,                       /*!< Button is released. */
  PAL_BTN_STATE_DOWN,                     /*!< Button is pressed. */
} PalBtnState_t;

/*! \brief      Action callback. This may be returned in interrupt context. */
typedef void (*PalBtnActionCback_t)(uint8_t btnId, PalBtnState_t state);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalBtnInit(void);
void PalBtnDeInit(void);
void PalBtnRegister(PalBtnActionCback_t actCback);

/* Control and Status */
PalBtnState_t  PalBtnGetState(uint8_t id);

/*! \} */    /* PAL_BUTTON */

#ifdef __cplusplus
};
#endif

#endif /* PAL_BTN_H */
