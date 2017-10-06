/*************************************************************************************************/
/*!
 *  \file   hci_evt.h
 *
 *  \brief  HCI event module.
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
#ifndef HCI_EVT_H
#define HCI_EVT_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* HCI event statistics */
typedef struct
{
  uint16_t numDiscCmplEvt;
  uint16_t numEncChangeEvt;
  uint16_t numReadRemoteVerInfoCmpEvt;
  uint16_t numCmdCmplEvt;
  uint16_t numCmdStatusEvt;
  uint16_t numHwErrorEvt;
  uint16_t numCmplPktsEvt;
  uint16_t numDataBufOverflowEvt;
  uint16_t numEncKeyRefreshCmplEvt;
  uint16_t numLeMetaEvt;
  uint16_t numVendorSpecEvt;
  uint16_t numAuthToEvt;
} hciEvtStats_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void hciEvtProcessMsg(uint8_t *pEvt);
hciEvtStats_t *hciEvtGetStats(void);

#ifdef __cplusplus
};
#endif

#endif /* HCI_EVT_H */
