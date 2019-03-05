/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Interface to ATT event handler.
 */
/*************************************************************************************************/
#ifndef ATT_HANDLER_H
#define ATT_HANDLER_H

#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_EVENT
 *  \{ */

/** \name ATT Event Handling
 * Message passing interface to ATT from other tasks through WSF.
 */
/**@{*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  ATT handler init function called during system initialization.
 *
 *  \param  handlerId  WSF handler ID for ATT.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttHandlerInit(wsfHandlerId_t handlerId);


/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for ATT.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/**@}*/

/*! \} */    /* STACK_EVENT */

#ifdef __cplusplus
};
#endif

#endif /* ATT_HANDLER_H */
