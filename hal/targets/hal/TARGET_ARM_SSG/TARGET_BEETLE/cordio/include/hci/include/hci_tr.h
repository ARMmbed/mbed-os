/*************************************************************************************************/
/*!
 *  \file   hci_tr.h
 *
 *  \brief  HCI transport interface.
 *
 *          $Date: 2015-06-12 07:19:18 -0400 (Fri, 12 Jun 2015) $
 *          $Revision: 3061 $
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*************************************************************************************************/
#ifndef HCI_TR_H
#define HCI_TR_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void hciTrSendAclData(void *pContext, uint8_t *pAclData);
void hciTrSendCmd(uint8_t *pCmdData);
bool_t hciTrInit(uint8_t port, uint32_t baudRate, bool_t flowControl);
void hciTrShutdown(void);

#ifdef __cplusplus
};
#endif

#endif /* HCI_TR_H */
