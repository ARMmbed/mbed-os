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
 * \brief Link layer controller scanning master interface file.
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

/* Utility */
bool_t LctrMstScanIsEnabled(void);
bool_t LctrMstScanIsPrivAddr(void);

/*! \} */    /* LL_LCTR_API_ADV_MST */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_MASTER_H */
