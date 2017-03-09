/*************************************************************************************************/
/*!
 *  \file   wsf_os_int.h
 *        
 *  \brief  Software foundation OS platform-specific interface file.
 *
 *          $Date: 2012-10-01 13:53:07 -0700 (Mon, 01 Oct 2012) $
 *          $Revision: 357 $
 *  
 *  Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
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
 *  \fn     wsfOsReadyToSleep
 *        
 *  \brief  Check if WSF is ready to sleep.
 *
 *  \param  None.
 *
 *  \return Return TRUE if there are no pending WSF task events set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t wsfOsReadyToSleep(void);

/*************************************************************************************************/
/*!
 *  \fn     wsfOsDispatcher
 *        
 *  \brief  Event dispatched.  Designed to be called repeatedly from infinite loop.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void wsfOsDispatcher(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfOsShutdown
 *        
 *  \brief  Shutdown OS.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfOsShutdown(void);

#ifdef __cplusplus
};
#endif

#endif /* WSF_OS_INT_H */
