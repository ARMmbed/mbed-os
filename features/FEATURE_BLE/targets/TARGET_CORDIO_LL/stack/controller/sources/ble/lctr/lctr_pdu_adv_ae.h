/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller extended advertising channel packet interface file.
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

#ifndef LCTR_PDU_ADV_AE_H
#define LCTR_PDU_ADV_AE_H

#include "lctr_pdu_adv.h"
#include "lctr_int_adv_slave_ae.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Advertising mode. */
enum
{
  LCTR_ADV_MODE_CONN_BIT    = (1 << 0), /*!< Connectable bit. */
  LCTR_ADV_MODE_SCAN_BIT    = (1 << 1)  /*!< Scannable bit. */
};

/*! \brief      Clock accuracy. */
enum
{
  LCTR_CLK_ACC_51_500_PPM   = 0,        /*!< Clock accuracy is between 51 and 500 PPM. */
  LCTR_CLK_ACC_0_50_PPM     = 1         /*!< Clock accuracy is between 0 and 51 PPM. */
};

/*! \brief      Channel offset units. */
enum
{
  LCTR_OFFS_UNITS_30_USEC   = 0,        /*!< Unit is 30 microseconds. */
  LCTR_OFFS_UNITS_300_USEC  = 1         /*!< Unit is 300 microseconds. */
};

#define LCTR_AUX_PTR_LEN    3           /*!< Size of AuxPtr field. */
#define LCTR_TX_POWER_LEN   1           /*!< Size of TxPower field. */
#define LCTR_SYNC_INFO_LEN  18          /*!< Size of SyncInfo field. */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended advertising channel PDU header. */
typedef struct
{
  uint8_t       extHdrLen;          /*!< Extended header length. */
  uint8_t       advMode;            /*!< Advertising mode. */
  uint8_t       extHdrFlags;        /*!< Extended header flags. */
  uint64_t      advAddr;            /*!< Advertiser's address. */
  uint64_t      tgtAddr;            /*!< Target address. */
  uint16_t      did;                /*!< Advertising data ID in ADI. */
  uint8_t       sid;                /*!< Advertising set ID in ADI. */
  int8_t        txPwr;              /*!< Tx Power. */
  uint8_t       acadLen;            /*!< ACAD length. */
  const uint8_t *pAcad;             /*!< Buffer pointer to ACAD. */
  const uint8_t *pAuxPtr;           /*!< Buffer pointer to AuxPtr field. */
  const uint8_t *pSyncInfo;         /*!< Buffer pointer to SyncInfo field. */
} lctrExtAdvHdr_t;

/*! \brief      Auxiliary Pointer. */
typedef struct
{
  uint8_t       auxChIdx;           /*!< Auxiliary channel index. */
  uint8_t       ca;                 /*!< Advertiser's clock accuracy. */
  uint8_t       offsetUnits;        /*!< Offset units. */
  uint16_t      auxOffset;          /*!< Auxiliary offset. */
  uint8_t       auxPhy;             /*!< Auxiliary PHY. */
} lctrAuxPtr_t;

/*! \brief      Sync info. */
typedef struct
{
  uint16_t      syncOffset;         /*!< Sync packet offset. */
  uint8_t       offsetUnits;        /*!< Offset units. */
  uint16_t      syncInter;          /*!< Sync interval. */
  uint64_t      chanMap;            /*!< Secondary channel map. */
  uint8_t       sca;                /*!< Sleep clock accuracy. */
  uint32_t      accAddr;            /*!< Access address. */
  uint32_t      crcInit;            /*!< CRC initialization value. */
  uint16_t      eventCounter;       /*!< Event counter. */
} lctrSyncInfo_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Pack */
uint8_t lctrPackAdvExtIndPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf, bool_t isPeriodic);
uint8_t lctrPackAuxAdvIndPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf, lctrAdvDataBuf_t *pAdvData, bool_t isPeriodic);
uint8_t lctrPackAuxScanRspPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf, bool_t isPeriodic);
uint8_t lctrPackAuxChainIndPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf, lctrAdvDataBuf_t *pAdvData, bool_t isPeriodic);
uint8_t lctrPackAuxConnRspPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf, bool_t isPeriodic);
void lctrPackAuxPtr(lctrAdvSet_t const *pAdvSet, uint32_t offsUsec, uint8_t chIdx, uint8_t *pAuxPtr);
uint8_t lctrPackLegacyAdvPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf);
uint8_t lctrPackLegacyScanRspPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf);
uint8_t lctrPackSyncIndPdu(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf, lctrAdvDataBuf_t *pAdvData, bool_t isPeriodic);

/* Unpack */
uint8_t lctrUnpackExtAdvHeader(lctrExtAdvHdr_t *pPdu, uint8_t *pNewFlags, const uint8_t *pBuf);
void lctrUnpackAuxPtr(lctrAuxPtr_t *pAuxPtr, const uint8_t *pBuf);
void lctrUnpackSyncInfo(lctrSyncInfo_t *pSyncInfo, const uint8_t *pBuf);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_PDU_ADV_AE_H */
