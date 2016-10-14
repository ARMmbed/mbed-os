/*************************************************************************************************/
/*!
 *  \file   l2c_handler.h
 *        
 *  \brief  L2CAP handler interface.
 *
 *          $Date $
 *          $Revision $
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
#ifndef L2C_HANDLER_H
#define L2C_HANDLER_H

#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     L2cSlaveHandlerInit
 *        
 *  \brief  Event handler initialization function for L2C when operating as a slave.
 *
 *  \param  handlerId  ID for this event handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cSlaveHandlerInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \fn     L2cSlaveHandler
 *        
 *  \brief  The WSF event handler for L2C when operating as a slave.
 *
 *  \param  event   Event mask.
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cSlaveHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocHandlerInit
 *        
 *  \brief  Event handler initialization function for L2C with connection oriented channels.
 *
 *  \param  handlerId  ID for this event handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocHandlerInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \fn     L2cCocHandler
 *        
 *  \brief  The WSF event handler for L2C with connection oriented channels.
 *
 *  \param  event   Event mask.
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* L2C_HANDLER_H */
