/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller extended scanning master interface file.
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

#ifndef LCTR_API_ADV_MASTER_AE_H
#define LCTR_API_ADV_MASTER_AE_H

#include "lctr_api.h"
#include "lmgr_api_adv_master.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master scan and initiate task messages for \a LCTR_DISP_SCAN dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_EXT_SCAN_MSG_RESET               = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Scan events */
  LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE,    /*!< Scan enable API event. */
  LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE,   /*!< Scan disable API event. */
  LCTR_EXT_SCAN_MSG_TERMINATE,          /*!< Scan BOD terminated event. */
  LCTR_EXT_SCAN_MSG_TOTAL               /*!< Total number of scan events. */
};

/*! \brief      Scanner create sync task messages for \a LCTR_DISP_PER_CREATE_SYNC dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_CREATE_SYNC_MSG_RESET            = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Advertising events */
  LCTR_CREATE_SYNC_MSG_CREATE,          /*!< Create sync create API event. */
  LCTR_CREATE_SYNC_MSG_CANCEL,          /*!< Create sync cancel sync API event. */
  LCTR_CREATE_SYNC_MSG_DONE,            /*!< Create sync done event. */
  LCTR_CREATE_SYNC_MSG_TERMINATE,       /*!< Create sync scanning BOD terminate event. */
  LCTR_CREATE_SYNC_MSG_TOTAL            /*!< Total number of Create sync events. */

};

/*! \brief      Scanner periodic synchronous task messages for \a LCTR_DISP_PER_SCAN dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_PER_SCAN_MSG_RESET               = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Advertising events */
  LCTR_PER_SCAN_MSG_SYNC_ESTD,          /*!< Periodic scanning synchronous established  event. */
  LCTR_PER_SCAN_MSG_TERMINATE,          /*!< Periodic scanning terminate API event. */
  LCTR_PER_SCAN_MSG_TERMINATE_DONE,     /*!< Periodic scanning terminate done event. */
  LCTR_PER_SCAN_SUP_TIMEOUT,            /*!< Periodic synchronous supervision timeout event. */
  LCTR_PER_SCAN_MSG_TOTAL               /*!< Total number of periodic scanning events. */

};

/*! \brief      Scan PHYs. */
enum
{
  LCTR_SCAN_PHY_1M,                     /*!< LE 1M PHY scanner. */
  LCTR_INIT_PHY_2M,                     /*!< LE 2M PHY initiator (not valid for scanning). */
  LCTR_SCAN_PHY_CODED,                  /*!< LE Coded PHY scanner. */
  LCTR_SCAN_PHY_TOTAL,                  /*!< Total number of scanning PHYs. */
  LCTR_SCAN_PHY_ALL     = 0xFF          /*!< All PHY scanners. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended scan enable message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint8_t           filtDup;            /*!< Filter out duplicate advertisers. */
  uint32_t          durMs;              /*!< Duration in milliseconds. */
  uint32_t          perMs;              /*!< Period in milliseconds. */
} lctrExtScanEnableMsg_t;

/*! \brief      Extended initiate message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint8_t           filterPolicy;       /*!< Scan filter policy. */
  uint8_t           ownAddrType;        /*!< Address type used by this device. */
  uint8_t           peerAddrType;       /*!< Address type used for peer device. */
  uint64_t          peerAddr;           /*!< Address of peer device. */
  uint8_t           initPhys;           /*!< Initiating PHYs. */
} lctrExtInitiateMsg_t;

/*! \brief      Extended scan message data. */
typedef union
{
  lctrMsgHdr_t           hdr;           /*!< Message header. */
  lctrExtScanEnableMsg_t enable;        /*!< Extended scan enable message data. */
  lctrExtInitiateMsg_t   init;          /*!< Extended initiate message data. */
} LctrExtScanMsg_t;

/*! \brief      Periodic create sync message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint8_t           filterPolicy;       /*!< Filter Policy. */
  uint8_t           advSID;             /*!< Advertising SID. */
  uint8_t           advAddrType;        /*!< Advertiser Address Type. */
  uint64_t          advAddr;            /*!< Advertiser Address. */
  uint16_t          skip;               /*!< Skip. */
  uint16_t          syncTimeOut;        /*!< Synchronization Timeout. */
} lctrPerCreateSyncMsg_t;

/*! \brief      Periodic Advertising message data. */
typedef union
{
  lctrMsgHdr_t              hdr;            /*!< Message header. */
  lctrPerCreateSyncMsg_t    createSync;     /*!< Periodic create sync message data. */
} LctrPerScanMsg_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

/*! \brief      Periodic scan message data. */
extern LctrPerScanMsg_t *pLctrMstPerScanMsg;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstExtScanInit(void);
void LctrMstExtScanDefaults(void);
void LctrMstPerCreateSyncInit(void);
void LctrMstPerScanInit(void);

/* Status */
bool_t LctrMstExtScanValidateParam(void);

/* Control */
void LctrMstExtScanSetScanPhy(uint8_t scanPhy);
void LctrMstExtScanClearScanPhy(uint8_t scanPhy);
void LctrMstExtScanSetParam(uint8_t scanPhy, uint8_t ownAddrType, uint8_t scanFiltPolicy, const LlExtScanParam_t *pParam);
bool_t LctrMstPerIsSyncPending(void);
bool_t LctrMstPerIsSyncDisabled(void);
bool_t LctrMstPerIsSync(uint8_t advSID, uint8_t advAddrType, uint64_t advAddr);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_MASTER_AE_H */
