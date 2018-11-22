/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller advertising channel packet interface file.
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

#ifndef LCTR_PDU_ADV_H
#define LCTR_PDU_ADV_H

#include "lmgr_api_adv_slave.h"
#include "lctr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define LCTR_ADV_HDR_PDU_TYPE_SHIFT 0   /*!< ADV header PDU type field shift. */
#define LCTR_ADV_HDR_CH_SEL_SHIFT   5   /*!< ADV header channel selection field shift. */
#define LCTR_ADV_HDR_TX_ADD_SHIFT   6   /*!< ADV header Tx Address type field shift. */
#define LCTR_ADV_HDR_RX_ADD_SHIFT   7   /*!< ADV header Rx Address type field shift. */
#define LCTR_ADV_HDR_LEN_SHIFT      8   /*!< ADV header length field shift. */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising channel PDU header. */
typedef struct
{
  uint8_t           pduType;            /*!< PDU type. */
  uint8_t           chSel;              /*!< Channel selection. */
  bool_t            txAddrRnd;          /*!< Tx address type is random. */
  bool_t            rxAddrRnd;          /*!< Rx address type is random. */
  uint8_t           len;                /*!< Payload length. */
} lctrAdvbPduHdr_t;

/*! \brief      Scan request PDU. */
typedef struct
{
  uint64_t          scanAddr;           /*!< Scanner's address. */
  uint64_t          advAddr;            /*!< Advertiser's address. */
} lctrScanReq_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Pack */
uint8_t lctrPackAdvbPduHdr(uint8_t *pBuf, const lctrAdvbPduHdr_t *pHdr);
uint8_t lctrPackAdvPdu(uint8_t *pBuf, uint64_t advA, const lmgrAdvbUser_t *pAdvData);
uint8_t lctrPackConnDirectIndAdvPdu(uint8_t *pBuf, uint64_t advA, uint64_t initA);
uint8_t lctrPackScanReqPdu(uint8_t *pBuf, const lctrScanReq_t *pPdu);
uint8_t lctrPackScanRspPdu(uint8_t *pBuf, uint64_t advA, const lmgrAdvbUser_t *pRspData);
uint8_t lctrPackConnIndPduAddr(uint8_t *pBuf, const lctrConnInd_t *pPdu);
uint8_t lctrPackConnIndPdu(uint8_t *pBuf, const lctrConnInd_t *pPdu);

/* Unpack */
uint8_t lctrUnpackAdvbPduHdr(lctrAdvbPduHdr_t *pHdr, const uint8_t *pBuf);
uint8_t lctrUnpackScanReqPdu(lctrScanReq_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackConnIndPdu(lctrConnInd_t *pPdu, const uint8_t *pBuf);

/* Validate */
bool_t lctrValidateConnIndPdu(lctrConnInd_t *pPdu);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_PDU_ADV_H */
