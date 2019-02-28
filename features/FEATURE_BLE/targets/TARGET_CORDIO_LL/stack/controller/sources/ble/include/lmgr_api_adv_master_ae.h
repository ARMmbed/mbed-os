/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager advertising extension interface file.
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

#ifndef LMGR_API_ADV_MASTER_AE_H
#define LMGR_API_ADV_MASTER_AE_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Periodic advertising sync established event */
typedef struct
{
  uint8_t       advSID;         /*!< Advertising SID. */
  uint8_t       addrType;       /*!< Address type. */
  bdAddr_t      addr;           /*!< Address. */
  uint8_t       advPhy;         /*!< Advertising PHY. */
  uint16_t      advInterval;    /*!< Advertising interval. */
  uint8_t       advClkAccuracy; /*!< Advertising clock accuracy. */
} lmgrPerAdvSyncEstdInd_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Events */
void LmgrSendExtScanEnableCnf(uint8_t status);
void LmgrSendScanTimeoutInd(void);
void LmgrSendExtAdvRptInd(LlExtAdvReportInd_t *pEvt);
void LmgrSendPerAdvRptInd(LlPerAdvReportInd_t *pEvt);
void LmgrSendSyncEstInd(uint8_t status, uint16_t handle, lmgrPerAdvSyncEstdInd_t *pEvt);
void LmgrSendSyncLostInd(uint16_t handle);

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_MASTER_AE_H */
