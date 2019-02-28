/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller scanning master interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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
