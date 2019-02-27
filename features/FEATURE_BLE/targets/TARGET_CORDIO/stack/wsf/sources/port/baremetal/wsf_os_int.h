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
 *  \brief Software foundation OS platform-specific interface file.
 */
/*************************************************************************************************/
#ifndef WSF_OS_INT_H
#define WSF_OS_INT_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Task events */
#define WSF_MSG_QUEUE_EVENT   0x01        /* Message queued for event handler */
#define WSF_TIMER_EVENT       0x02        /* Timer expired for event handler */
#define WSF_HANDLER_EVENT     0x04        /* Event set for event handler */

/* Derive task from handler ID */
#define WSF_TASK_FROM_ID(handlerID)       (((handlerID) >> 4) & 0x0F)

/* Derive handler from handler ID */
#define WSF_HANDLER_FROM_ID(handlerID)    ((handlerID) & 0x0F)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Event handler ID data type */
typedef uint8_t  wsfHandlerId_t;

/* Event handler event mask data type */
typedef uint8_t  wsfEventMask_t;

/* Task ID data type */
typedef wsfHandlerId_t  wsfTaskId_t;

/* Task event mask data type */
typedef uint8_t wsfTaskEvent_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Check if WSF is ready to sleep.
 *
 *  \return Return TRUE if there are no pending WSF task events set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t wsfOsReadyToSleep(void);

/*************************************************************************************************/
/*!
 *  \brief  Event dispatched.  Designed to be called repeatedly from infinite loop.
 *
 *  \return None.
 */
/*************************************************************************************************/
void wsfOsDispatcher(void);

#ifdef __cplusplus
};
#endif

#endif /* WSF_OS_INT_H */
