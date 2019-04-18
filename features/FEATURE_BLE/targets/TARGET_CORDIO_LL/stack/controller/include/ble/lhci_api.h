/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Link layer HCI subsystem API.
 */
/*************************************************************************************************/

#ifndef LHCI_API_H
#define LHCI_API_H

#include "wsf_types.h"
#include "wsf_os.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      HCI header */
typedef struct
{
  uint16_t      opCode;                 /*!< OpCode command field. */
  uint8_t       len;                    /*!< Parameter length. */
} LhciHdr_t;

/*! \brief      Command handler call signature. */
typedef bool_t (*lhciCmdHandler_t)(LhciHdr_t *pHdr, uint8_t *pBuf);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LhciScanMasterInit(void);
void LhciExtScanMasterInit(void);
void LhciAdvSlaveInit(void);
void LhciExtAdvSlaveInit(void);
void LhciEncMasterInit(void);
void LhciEncSlaveInit(void);
void LhciPrivAdvInit(void);
void LhciPrivConnInit(void);
void LhciConnInit(void);
void LhciConnMasterInit(void);
void LhciExtConnMasterInit(void);
void LhciScInit(void);
void LhciPhyInit(void);
void LhciPastInit(void);
void LhciChannelSelection2Init(void);
void LhciCisMasterInit(void);
void LhciCisSlaveInit(void);
void LhciIsoInit(void);
void LhciVsExtInit(lhciCmdHandler_t decodeCmd);
void LhciHandlerInit(wsfHandlerId_t handlerId);
void LhciHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
void LhciIsoHandlerInit(wsfHandlerId_t handlerId);
void LhciIsoHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
void LhciSetDefaultHciSupCmd(uint8_t *pBuf);
#if (LL_ENABLE_TESTER)
void LhciTesterInit(void);
#endif

/* Command processing */
uint8_t LhciPackCmdStatusEvt(uint8_t *pBuf, uint8_t status, uint16_t opCode);
uint8_t LhciPackCmdCompleteEvt(uint8_t *pBuf, uint16_t opCode);
uint8_t LhciPackCmdCompleteEvtStatus(uint8_t *pBuf, uint8_t status);
uint8_t LhciPackVsEvt(uint8_t *pBuf, uint16_t vsEvtCode);
void LhciSendEvent(uint8_t *pBuf);
bool_t LhciIsEventPending(void);
uint8_t LhciPackEvtHdr(uint8_t *pBuf, uint8_t evtCode, uint8_t paramLen);

/* Event processing */
bool_t LhciVsEncodeTraceMsgEvtPkt(const uint8_t *pBuf, uint32_t len);

#ifdef __cplusplus
};
#endif

#endif /* LHCI_API_H */
