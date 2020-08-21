/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Timer interface file.
 *
 *  Copyright (c) 2016-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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

#ifndef PAL_TIMER_H
#define PAL_TIMER_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_TIMER
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Operational states. */
typedef enum
{
  PAL_TIMER_STATE_UNINIT = 0,    /*!< Uninitialized state. */
  PAL_TIMER_STATE_ERROR  = 0,    /*!< Error state. */
  PAL_TIMER_STATE_READY,         /*!< Ready state. */
  PAL_TIMER_STATE_BUSY           /*!< Busy state. */
} PalTimerState_t;

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Completion callback. */
typedef void (*PalTimerCompCback_t)(void);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalTimerInit(PalTimerCompCback_t expCback);
void PalTimerDeInit(void);

/* Control and Status */
PalTimerState_t PalTimerGetState(void);
void PalTimerStart(uint32_t expUsec);
void PalTimerStop(void);
uint32_t PalTimerGetCurrentTime(void);

/*! \} */    /* PAL_TIMER */

#ifdef __cplusplus
};
#endif

#endif
