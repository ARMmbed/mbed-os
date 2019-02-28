/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer HCI subsystem API.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
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
void LhciChannelSelection2Init(void);
void LhciVsExtInit(lhciCmdHandler_t decodeCmd);
void LhciHandlerInit(wsfHandlerId_t handlerId);
void LhciHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
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
void LhciVsEncodeTraceMsgEvtPkt(uint8_t *pBuf, uint8_t len);

#ifdef __cplusplus
};
#endif

#endif /* LHCI_API_H */
