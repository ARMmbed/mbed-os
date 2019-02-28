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
 *  \brief Controller HCI transport interface file.
 */
/*************************************************************************************************/

#ifndef CHCI_TR_H
#define CHCI_TR_H

#include "wsf_types.h"
#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Signal the completion of a message write.
 *
 *  \return None.
 *
 *  This routine is used for asynchronous write operations. When the driver has completed the
 *  use of the write buffer, this routine is called to free the buffer and release flow control.
 */
/*************************************************************************************************/
void chciTrSendComplete(void);

/*************************************************************************************************/
/*!
 *  \brief  Signal the completion of a message receive.
 *
 *  \param  prot    Protocol.
 *  \param  type    Message type.
 *  \param  pBuf    Message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrRecv(uint8_t prot, uint8_t type, uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief  Signal a hardware error.
 *
 *  \param  code    Error code.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrHwError(uint8_t code);

#ifdef __cplusplus
};
#endif

#endif /* CHCI_TR_H */
