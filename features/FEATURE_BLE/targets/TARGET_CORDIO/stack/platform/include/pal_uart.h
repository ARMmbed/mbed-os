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
 *  \brief UART driver definition.
 */
/*************************************************************************************************/

#ifndef PAL_UART_H
#define PAL_UART_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_UART
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Completion callback. */
typedef void (*PalUartCompCback_t)(void);

/*! \brief      Peripheral configuration. */
typedef struct
{
  uint32_t baud;                /*!< Baud rate. */
  PalUartCompCback_t rdCback;   /*!< Read data completion callback. */
  PalUartCompCback_t wrCback;   /*!< Write data completion callback. */
  bool_t hwFlow;                /*!< Use HW Flow control */
} PalUartConfig_t;

/*! \brief      Operational states. */
typedef enum
{
  PAL_UART_STATE_UNINIT = 0,    /*!< Uninitialized state. */
  PAL_UART_STATE_ERROR  = 0,    /*!< Error state. */
  PAL_UART_STATE_READY  = 1,    /*!< Ready state. */
  PAL_UART_STATE_BUSY   = 2,    /*!< Busy state. */
} PalUartState_t;

/*! \brief      Reserved UART ID. */
typedef enum
{
  PAL_UART_ID_USER      = 0x00,  /*!< UART 0. */
  PAL_UART_ID_CHCI      = 0xF0,  /*!< UART CHCI. */
  PAL_UART_ID_TERMINAL  = 0xF1,  /*!< UART TERMINAL. */
} PalUartId_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalUartInit(PalUartId_t id, const PalUartConfig_t *pCfg);
void PalUartDeInit(PalUartId_t id);

/* Control and Status */
PalUartState_t PalUartGetState(PalUartId_t id);

/* Data Transfer */
void PalUartReadData(PalUartId_t id, uint8_t *pData, uint16_t len);
void PalUartWriteData(PalUartId_t id, const uint8_t *pData, uint16_t len);

/*! \} */    /* PAL_UART */

#ifdef __cplusplus
};
#endif

#endif /* PAL_UART_H */
