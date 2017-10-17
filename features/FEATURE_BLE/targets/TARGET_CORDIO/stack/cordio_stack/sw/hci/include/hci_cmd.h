/*************************************************************************************************/
/*!
 *  \file   hci_cmd.h
 *
 *  \brief  HCI command module.
 *
 *          $Date: 2016-12-28 16:12:14 -0600 (Wed, 28 Dec 2016) $
 *          $Revision: 10805 $
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
#ifndef HCI_CMD_H
#define HCI_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void hciCmdSend(uint8_t *pData);
uint8_t *hciCmdAlloc(uint16_t opcode, uint16_t len);
void hciCmdInit(void);
void hciCmdTimeout(wsfMsgHdr_t *pMsg);
void hciCmdRecvCmpl(uint8_t numCmdPkts);

#ifdef __cplusplus
};
#endif

#endif /* HCI_CMD_H */
