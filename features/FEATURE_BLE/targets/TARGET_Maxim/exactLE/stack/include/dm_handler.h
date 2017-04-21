/*************************************************************************************************/
/*!
 *  \file   dm_handler.h
 *
 *  \brief  Interface to DM event handler.
 *
 *          $Date: 2012-03-29 13:24:04 -0700 (Thu, 29 Mar 2012) $
 *          $Revision: 287 $
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
#ifndef DM_HANDLER_H
#define DM_HANDLER_H

#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     DmHandlerInit
 *        
 *  \brief  DM handler init function called during system initialization.
 *
 *  \param  handlerID  WSF handler ID for DM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmHandlerInit(wsfHandlerId_t handlerId);


/*************************************************************************************************/
/*!
 *  \fn     DmHandler
 *        
 *  \brief  WSF event handler for DM.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* DM_HANDLER_H */
