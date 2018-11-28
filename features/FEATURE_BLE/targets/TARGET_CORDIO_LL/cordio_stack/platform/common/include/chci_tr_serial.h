/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI serial transport interface file.
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

#ifndef CHCI_TR_SERIAL_H
#define CHCI_TR_SERIAL_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************/
/*!
 *  \brief  Receive function.  Gets called by external code when bytes are received.
 *
 *  \param  pBuf   Pointer to buffer of incoming bytes.
 *  \param  len    Number of bytes in incoming buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrSerialRxIncoming(uint8_t *pBuf, uint8_t len);

#ifdef __cplusplus
};
#endif

#endif /* CHCI_TR_SERIAL_H */
