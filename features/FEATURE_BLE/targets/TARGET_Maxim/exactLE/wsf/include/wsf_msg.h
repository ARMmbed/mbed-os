/*************************************************************************************************/
/*!
 *  \file   wsf_msg.h
 *
 *  \brief  Message passing service.
 *
 *          $Date: 2013-07-02 15:08:09 -0700 (Tue, 02 Jul 2013) $
 *          $Revision: 779 $
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
#ifndef WSF_MSG_H
#define WSF_MSG_H

#include "wsf_queue.h"
#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfMsgAlloc
 *
 *  \brief  Allocate a message buffer to be sent with WsfMsgSend().
 *
 *  \param  len   Message length in bytes.
 *
 *  \return Pointer to message buffer or NULL if allocation failed.
 */
/*************************************************************************************************/
void *WsfMsgAlloc(uint16_t len);

/*************************************************************************************************/
/*!
 *  \fn     WsfMsgFree
 *
 *  \brief  Free a message buffer allocated with WsfMsgAlloc().
 *
 *  \param  pMsg  Pointer to message buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMsgFree(void *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     WsfMsgSend
 *
 *  \brief  Send a message to an event handler.
 *
 *  \param  handlerId   Event handler ID.
 *  \param  pMsg        Pointer to message buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMsgSend(wsfHandlerId_t handlerId, void *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     WsfMsgEnq
 *
 *  \brief  Enqueue a message.
 *
 *  \param  pQueue    Pointer to queue.
 *  \param  handerId  Set message handler ID to this value.
 *  \param  pElem     Pointer to message buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfMsgEnq(wsfQueue_t *pQueue, wsfHandlerId_t handlerId, void *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     WsfMsgDeq
 *
 *  \brief  Dequeue a message.
 *
 *  \param  pQueue      Pointer to queue.
 *  \param  pHandlerId  Handler ID of returned message; this is a return parameter.
 *
 *  \return Pointer to message that has been dequeued or NULL if queue is empty.
 */
/*************************************************************************************************/
void *WsfMsgDeq(wsfQueue_t *pQueue, wsfHandlerId_t *pHandlerId);

/*************************************************************************************************/
/*!
 *  \fn     WsfMsgPeek
 *
 *  \brief  Get the next message without removing it from the queue.
 *
 *  \param  pQueue      Pointer to queue.
 *  \param  pHandlerId  Handler ID of returned message; this is a return parameter.
 *
 *  \return Pointer to the next message on the queue or NULL if queue is empty.
 */
/*************************************************************************************************/
void *WsfMsgPeek(wsfQueue_t *pQueue, wsfHandlerId_t *pHandlerId);

#ifdef __cplusplus
};
#endif

#endif /* WSF_MSG_H */
