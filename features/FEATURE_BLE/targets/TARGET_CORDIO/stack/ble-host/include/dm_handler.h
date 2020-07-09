/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Interface to DM event handler.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef DM_HANDLER_H
#define DM_HANDLER_H

#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_EVENT
 *  \{ */

/** \name DM Event Handling
 * Message passing interface to DM from other tasks through WSF.
 */
/**@{*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  DM handler init function called during system initialization.
 *
 *  \param  handlerId  WSF handler ID for DM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmHandlerInit(wsfHandlerId_t handlerId);


/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for DM.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/**@}*/

/*! \} */    /* STACK_DM_API */

#ifdef __cplusplus
};
#endif

#endif /* DM_HANDLER_H */
