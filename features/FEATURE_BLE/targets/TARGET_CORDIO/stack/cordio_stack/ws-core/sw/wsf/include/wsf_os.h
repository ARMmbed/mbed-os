/*************************************************************************************************/
/*!
 *  \file   wsf_os.h
 *
 *  \brief  Software foundation OS API.
 *
 *          $Date: 2017-03-02 16:50:43 -0600 (Thu, 02 Mar 2017) $
 *          $Revision: 11350 $
 *
 *  Copyright (c) 2009-2017 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef WSF_OS_H
#define WSF_OS_H

#include "wsf_os_int.h"
#include "wsf_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Configuration
**************************************************************************************************/

/* OS Diagnistics */
#ifndef WSF_OS_DIAG
#define WSF_OS_DIAG                             FALSE
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Invalid Task Identifier */
#define WSF_INVALID_TASK_ID                     0xFF

#if WSF_OS_DIAG == TRUE
#define WSF_OS_GET_ACTIVE_HANDLER_ID()          WsfActiveHandler
#else
#define WSF_OS_GET_ACTIVE_HANDLER_ID()          WSF_INVALID_TASK_ID
#endif /* WSF_OS_DIAG */

/**************************************************************************************************
  External Variables
**************************************************************************************************/

/* Diagnistic Task Identifier */
extern wsfHandlerId_t WsfActiveHandler;

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Common message structure passed to event handler */
typedef struct
{
  uint16_t        param;          /*! General purpose parameter passed to event handler */
  uint8_t         event;          /*! General purpose event value passed to event handler */
  uint8_t         status;         /*! General purpose status value passed to event handler */
} wsfMsgHdr_t;

/**************************************************************************************************
  Callback Function Types
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     wsfEventHandler_t
 *
 *  \brief  Event handler callback function.
 *
 *  \param  event    Mask of events set for the event handler.
 *  \param  pMsg     Pointer to message for the event handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*wsfEventHandler_t)(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfSetEvent
 *
 *  \brief  Set an event for an event handler.
 *
 *  \param  handlerId   Handler ID.
 *  \param  event       Event or events to set.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfSetEvent(wsfHandlerId_t handlerId, wsfEventMask_t event);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskLock
 *
 *  \brief  Lock task scheduling.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTaskLock(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskUnlock
 *
 *  \brief  Unlock task scheduling.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTaskUnlock(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskSetReady
 *
 *  \brief  Set the task used by the given handler as ready to run.
 *
 *  \param  handlerId   Event handler ID.
 *  \param  event       Task event mask.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfTaskSetReady(wsfHandlerId_t handlerId, wsfTaskEvent_t event);

/*************************************************************************************************/
/*!
 *  \fn     WsfTaskMsgQueue
 *
 *  \brief  Return the task message queue used by the given handler.
 *
 *  \param  handlerId   Event handler ID.
 *
 *  \return Task message queue.
 */
/*************************************************************************************************/
wsfQueue_t *WsfTaskMsgQueue(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \fn     WsfOsSetNextHandler
 *
 *  \brief  Set the next WSF handler function in the WSF OS handler array.  This function
 *          should only be called as part of the OS initialization procedure.
 *
 *  \param  handler    WSF handler function.
 *
 *  \return WSF handler ID for this handler.
 */
/*************************************************************************************************/
wsfHandlerId_t WsfOsSetNextHandler(wsfEventHandler_t handler);

#ifdef __cplusplus
};
#endif

#endif /* WSF_OS_H */
