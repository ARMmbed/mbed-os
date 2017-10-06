/*************************************************************************************************/
/*!
 *  \file   hci_core_ps.h
 *
 *  \brief  HCI core platform-specific interfaces for dual-chip.
 *
 *          $Date: 2016-12-28 16:12:14 -0600 (Wed, 28 Dec 2016) $
 *          $Revision: 10805 $
 *
 *  Copyright (c) 2013-2017 ARM Ltd., all rights reserved.
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
#ifndef HCI_CORE_PS_H
#define HCI_CORE_PS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void hciCoreResetSequence(uint8_t *pMsg);
void hciCoreNumCmplPkts(uint8_t *pMsg);
void hciCoreRecv(uint8_t msgType, uint8_t *pCoreRecvMsg);
uint8_t hciCoreVsCmdCmplRcvd(uint16_t opcode, uint8_t *pMsg, uint8_t len);

#ifdef __cplusplus
};
#endif

#endif /* HCI_CORE_PS_H */
