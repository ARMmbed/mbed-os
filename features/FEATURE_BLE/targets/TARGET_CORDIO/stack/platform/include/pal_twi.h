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
 *  \brief TWI driver definition.
 */
/*************************************************************************************************/

#ifndef PAL_TWI_H
#define PAL_TWI_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_TWI
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Invalid device ID. */
#define PAL_TWI_INVALID_ID      0xFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Operation ready callback. */
typedef void (*PalTwiOpReadyCback_t)(uint8_t id);

/*! \brief      Completion callback (success=TRUE if ACK received, FALSE otherwise). */
typedef void (*PalTwiCompCback_t)(uint8_t id, bool_t success);

/*! \brief      TWI device configuration. */
typedef struct
{
  uint8_t devAddr;                      /*!< Device address. */
  PalTwiOpReadyCback_t opReadyCback;    /*!< Operation ready callback. */
  PalTwiCompCback_t rdCback;            /*!< Read data completion callback. */
  PalTwiCompCback_t wrCback;            /*!< Write data completion callback. */
} PalTwiDevConfig_t;

/*! \brief      Operational states. */
typedef enum
{
  PAL_TWI_STATE_UNINIT = 0,             /*!< Uninitialized state. */
  PAL_TWI_STATE_ERROR  = 0,             /*!< Error state. */
  PAL_TWI_STATE_READY,                  /*!< Ready state. */
  PAL_TWI_STATE_BUSY                    /*!< Busy (data transfer) state. */
} PalTwiState_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalTwiInit(void);
void PalTwiDeInit(void);
uint8_t PalTwiRegisterDevice(PalTwiDevConfig_t *pDevCfg);

/* Control and Status */
PalTwiState_t PalTwiGetState(void);

/* Data Transfer */
void PalTwiStartOperation(uint8_t id);
void PalTwiStopOperation(uint8_t id);
void PalTwiReadData(uint8_t id, uint8_t *pData, uint8_t len);
void PalTwiWriteData(uint8_t id, const uint8_t *pData, uint8_t len);

/*! \} */    /* PAL_TWI */

#ifdef __cplusplus
};
#endif

#endif /* PAL_TWI_H */
