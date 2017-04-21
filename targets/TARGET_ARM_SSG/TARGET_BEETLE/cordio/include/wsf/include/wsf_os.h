/*************************************************************************************************/
/*!
 *  \file   wsf_os.h
 *        
 *  \brief  Software foundation OS API.
 *
 *          $Date: 2014-08-08 06:30:50 -0700 (Fri, 08 Aug 2014) $
 *          $Revision: 1725 $
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
#ifndef WSF_OS_H
#define WSF_OS_H

#include "wsf_os_int.h"
#include "wsf_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

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
