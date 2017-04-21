/*************************************************************************************************/
/*!
 *  \file   l2c_handler.h
 *        
 *  \brief  L2CAP handler interface.
 *
 *          $Date $
 *          $Revision $
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

#ifdef __cplusplus
};
#endif

#endif /* L2C_HANDLER_H */
