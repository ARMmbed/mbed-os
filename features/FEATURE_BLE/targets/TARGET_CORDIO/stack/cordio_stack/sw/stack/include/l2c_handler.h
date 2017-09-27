/*************************************************************************************************/
/*!
 *  \file   l2c_handler.h
 *
 *  \brief  L2CAP handler interface.
 *
 *          $Date $
 *          $Revision $
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
