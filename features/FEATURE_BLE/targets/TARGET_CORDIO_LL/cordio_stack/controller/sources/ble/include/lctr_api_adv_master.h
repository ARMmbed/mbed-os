/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller scanning master interface file.
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

#ifndef LCTR_API_ADV_MASTER_H
#define LCTR_API_ADV_MASTER_H

#include "lctr_api.h"
#include "lmgr_api_adv_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_ADV_MST
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master scan task messages for \a LCTR_DISP_SCAN dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_SCAN_MSG_RESET                   = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Scan events */
  LCTR_SCAN_MSG_DISCOVER_ENABLE,        /*!< Scan enable API event. */
  LCTR_SCAN_MSG_DISCOVER_DISABLE,       /*!< Scan disable API event. */
  LCTR_SCAN_MSG_TERMINATE,              /*!< Scan BOD terminated event. */
  LCTR_SCAN_MSG_PARAM_UPD,              /*!< Scan parameter update event. */
  LCTR_SCAN_MSG_TOTAL                   /*!< Total number of scan events. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Scan parameter message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  lmgrScanParam_t   param;              /*!< Scan parameters. */
} lctrScanParamMsg_t;

/*! \brief      Scan enable message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  bool_t            filtDup;            /*!< Filter out duplicate advertisers. */
} lctrScanEnableMsg_t;

/*! \brief      Initiate message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  LlConnSpec_t      connSpec;           /*!< Connection spec. */
  lmgrScanParam_t   scanParam;          /*!< Scan parameters. */
  uint64_t          peerAddr;           /*!< Peer address. */
  uint8_t           peerAddrType;       /*!< Peer address type. */
} lctrInitiateMsg_t;

/*! \brief      Advertising report message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  LlAdvReportInd_t  rpt;                /*!< Report data. */
  uint32_t          hash;               /*!< Event data hash. */
} lctrAdvReportMsg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstScanInit(void);
void LctrMstScanDefaults(void);

/* Helpers */
void lctrAdvReportsInc(void);
void lctrAdvReportsDec(void);

/*! \} */    /* LL_LCTR_API_ADV_MST */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_MASTER_H */
