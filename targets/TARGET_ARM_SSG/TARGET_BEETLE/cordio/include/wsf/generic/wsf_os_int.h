/*************************************************************************************************/
/*!
 *  \file   wsf_os_int.h
 *        
 *  \brief  Software foundation OS platform-specific interface file.
 *
 *          $Date: 2012-10-01 13:53:07 -0700 (Mon, 01 Oct 2012) $
 *          $Revision: 357 $
 *  
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
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
