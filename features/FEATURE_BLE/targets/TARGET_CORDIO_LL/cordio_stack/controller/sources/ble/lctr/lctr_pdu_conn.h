/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller data channel packet interface file.
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

#ifndef LCTR_PDU_CONN_SLAVE_H
#define LCTR_PDU_CONN_SLAVE_H

#include "wsf_types.h"
#include "bb_ble_api.h"
#include "ll_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

#define LCTR_OFFSET_COUNT         6       /*!< Number of offsets in a connection parameter request. */

/*! \brief      Packet boundary flags. */
enum
{
  LCTR_PB_START_NON_AUTO_FLUSH  = 0,    /*!< Start of a non-automatically flushable L2CAP PDU. */
  LCTR_PB_CONT_FRAG             = 1,    /*!< Continuing fragment of Higher Layer Message. */
  LCTR_PB_START_AUTO_FLUSH      = 2,    /*!< Start of an automatically flushable L2CAP PDU. */
  /* N.B. next two enumerations intentionally use identical values. */
  LCTR_PB_COMPLETE_AUTO_FLUSH   = 3,    /*!< A complete L2CAP PDU automatically flushable (not allowed for LE-U). */
  LCTR_PB_VS_DATA               = 3     /*!< Vendor specific data. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ACL header. */
typedef struct
{
  uint16_t          connHandle;         /*!< Connection handle. */
  uint8_t           pktBound;           /*!< Packet boundary flag. */
  uint16_t          len;                /*!< Data length. */
} lctrAclHdr_t;

/*! \brief      Data channel PDU header. */
typedef struct
{
  uint8_t           llid;               /*!< PDU type. */
  uint8_t           nesn;               /*!< Next Expected Sequence Number. */
  uint8_t           sn;                 /*!< Sequence Number. */
  uint8_t           md;                 /*!< More Data. */
  uint8_t           len;                /*!< Payload length. */
} lctrDataPduHdr_t;

/*! \brief      Connection update indication PDU. */
typedef struct
{
  uint8_t           txWinSize;          /*!< transmitWindowSize value. */
  uint16_t          txWinOffset;        /*!< transmitWindowOffset value. */
  uint16_t          interval;           /*!< connInterval value. */
  uint16_t          latency;            /*!< connSlaveLatency value. */
  uint16_t          timeout;            /*!< connSupervisionTimeout value. */
  uint16_t          instant;            /*!< connInstant value. */
} lctrConnUpdInd_t;

/*! \brief      Connection update indication PDU. */
typedef struct
{
  uint64_t          chanMask;           /*!< Channel mask. */
  uint16_t          instant;            /*!< connInstant value. */
} lctrChanMapInd_t;

/*! \brief      Terminate indication control PDU. */
typedef struct
{
  uint8_t           errorCode;          /*!< Error code. */
} lctrTermInd_t;

/*! \brief      Encryption request control PDU. */
typedef struct
{
  uint8_t           rand[LL_RAND_LEN];  /*!< Random number. */
  uint16_t          ediv;               /*!< Encrypted diversifier. */
  uint8_t           skd_m[LL_SKD_LEN/2];/*!< Master's session key diversifier. */
  uint8_t           iv_m[LL_IV_LEN/2];  /*!< Master's initialization vector. */
} lctrEncReq_t;

/*! \brief      Encryption response control PDU. */
typedef struct
{
  uint8_t           skd_s[LL_SKD_LEN/2];/*!< Slave's session key diversifier. */
  uint8_t           iv_s[LL_IV_LEN/2];  /*!< Slave's initialization vector. */
} lctrEncRsp_t;

/*! \brief      Unknown response control PDU. */
typedef struct
{
  uint8_t           unknownType;        /*!< Unknown type. */
} lctrUnknownRsp_t;

/*! \brief      Feature request or response control PDU. */
typedef struct
{
  uint64_t          featSet;            /*!< Feature set. */
} lctrFeat_t;

/*! \brief      Connection parameter request/response values. */
typedef struct
{
  uint16_t          connIntervalMin;    /*!< Minimum connection interval. */
  uint16_t          connIntervalMax;    /*!< Maximum connection interval. */
  uint16_t          connLatency;        /*!< Connection latency. */
  uint16_t          supTimeout;         /*!< Supervision timeout. */
  uint8_t           prefPeriod;         /*!< Preferred periodicity. */
  uint16_t          refConnEvtCnt;      /*!< Reference connection event count. */
  uint16_t          offset[LCTR_OFFSET_COUNT];      /*!< Anchor point offset from \a refConnEvtCnt, first preference. */
} lctrConnParam_t;

/*! \brief      Version indication control PDU. */
typedef struct
{
  uint8_t           versNr;             /*!< Bluetooth controller specification version. */
  uint16_t          compId;             /*!< Company identifier. */
  uint16_t          subVersNr;          /*!< Sub-Version number. */
} lctrVerInd_t;

/*! \brief      Reject indication control PDU. */
typedef struct
{
  uint8_t           opcode;             /*!< Reject opcode. */
  uint8_t           reason;             /*!< Reject reason code. */
} lctrRejInd_t;

/*! \brief      Data length request or response PDU. */
typedef struct
{
  uint16_t          maxRxLen;            /*!< Maximum receive length. */
  uint16_t          maxRxTime;           /*!< Maximum receive time. */
  uint16_t          maxTxLen;            /*!< Maximum transmit length. */
  uint16_t          maxTxTime;           /*!< Maximum transmit time. */
} lctrDataLen_t;

/*! \brief      PHY request or response PDU. */
typedef struct
{
  uint8_t           txPhys;               /*!< Transmitter PHYs. */
  uint8_t           rxPhys;               /*!< Receiver PHYs. */
} lctrPhy_t;

/*! \brief      PHY update indication PDU. */
typedef struct
{
  uint8_t           masterToSlavePhy;     /*!< Master-to-slave PHY. */
  uint8_t           slaveToMasterPhy;     /*!< Slave-to-master PHY. */
  uint16_t          instant;              /*!< Instant. */
} lctrPhyUpdInd_t;

/*! \brief      Minimum number of used channels indication PDU. */
typedef struct
{
  uint8_t           phys;                 /*!< Bitmask for the affected PHYs. */
  uint8_t           minUsedChan;          /*!< Minimum number of used channels. */
} lctrMinUsedChanInd_t;

/*! \brief      Data channel control PDU. */
typedef struct
{
  lctrDataPduHdr_t  hdr;                /*!< Unpacked PDU header. */
  uint8_t opcode;                       /*!< Control PDU opcode. */

  union
  {
    lctrConnUpdInd_t connUpdInd;        /*!< Connection update indication. */
    lctrChanMapInd_t chanMapInd;        /*!< Channel map request. */
    lctrTermInd_t    termInd;           /*!< Terminate indication. */
    lctrEncReq_t     encReq;            /*!< Encryption request. */
    lctrEncRsp_t     encRsp;            /*!< Encryption response. */
    lctrUnknownRsp_t unknownRsp;        /*!< Unknown response. */
    lctrFeat_t       featReqRsp;        /*!< Feature request or response or slave feature request. */
    lctrVerInd_t     verInd;            /*!< Version indication. */
    lctrConnParam_t  connParamReqRsp;   /*!< Connection parameter request or response. */
    lctrRejInd_t     rejInd;            /*!< Reject indication. */
    lctrDataLen_t    lenReq;            /*!< Data length request. */
    lctrDataLen_t    lenRsp;            /*!< Data length response. */
    lctrPhy_t        phyReq;            /*!< PHY request. */
    lctrPhy_t        phyRsp;            /*!< PHY response. */
    lctrPhyUpdInd_t  phyUpdInd;         /*!< PHY update indication. */
    lctrMinUsedChanInd_t minUsedChanInd;/*!< Minimum number of used channels indication. */
  } pld;                                /*!< Unpacked PDU payload. */
} lctrDataPdu_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Pack */
uint8_t lctrPackAclHdr(uint8_t *pBuf, const lctrAclHdr_t *pHdr);
uint8_t lctrPackDataPduHdr(uint8_t *pBuf, const lctrDataPduHdr_t *pHdr);
uint8_t lctrPackConnUpdInd(uint8_t *pBuf, const lctrConnUpdInd_t *pPdu);

/* Unpack */
uint8_t lctrUnpackAclHdr(lctrAclHdr_t *pHdr, const uint8_t *pBuf);
uint8_t lctrUnpackDataPduHdr(lctrDataPduHdr_t *pHdr, const uint8_t *pBuf);
uint8_t lctrUnpackConnUpdateIndPdu(lctrConnUpdInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackChanMapIndPdu(lctrChanMapInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackTerminateIndPdu(lctrTermInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackUnknownRspPdu(lctrUnknownRsp_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackFeaturePdu(lctrFeat_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackRejectIndPdu(uint8_t *pReason, const uint8_t *pBuf);
uint8_t lctrUnpackVersionIndPdu(lctrVerInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackConnParamPdu(lctrConnParam_t *pConnParam, const uint8_t *pBuf);
uint8_t lctrUnpackRejectExtIndPdu(lctrRejInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackDataLengthPdu(lctrDataLen_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackPhyPdu(lctrPhy_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackPhyUpdateIndPdu(lctrPhyUpdInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackMinUsedChanIndPdu(lctrMinUsedChanInd_t *pPdu, const uint8_t *pBuf);

/* Decode */
uint8_t lctrDecodeCtrlPdu(lctrDataPdu_t *pPdu, const uint8_t *pBuf, uint8_t role);
uint8_t lctrDecodeEncPdu(lctrDataPdu_t *pPdu, const uint8_t *pBuf, uint8_t role);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_PDU_CONN_SLAVE_H */
