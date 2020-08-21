/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Interface to SMP event handler.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
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
#ifndef SMP_HANDLER_H
#define SMP_HANDLER_H

#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_EVENT
 *  \{ */

/** \name SMP Event Handling
 * Message passing interface to SMP from other tasks through WSF.
 */
/**@{*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  SMP handler init function called during system initialization.
 *
 *  \param  handlerId  WSF handler ID for SMP.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpHandlerInit(wsfHandlerId_t handlerId);


/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for SMP.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/**@}*/

/*! \} */    /* STACK_SMP_API */

#ifdef __cplusplus
};
#endif

#endif /* SMP_HANDLER_H */
