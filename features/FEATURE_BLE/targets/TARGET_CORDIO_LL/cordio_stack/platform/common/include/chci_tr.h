/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI transport interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
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
