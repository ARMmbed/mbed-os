/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief Link layer tester interface file.
 */
/*************************************************************************************************/

#ifndef LL_TESTER_API_H
#define LL_TESTER_API_H

#include "wsf_types.h"
#include "ll_api.h"
#include "lctr_api.h"

/* Access internal definitions. */
#include "../../sources/ble/lctr/lctr_pdu_conn.h"

/* Require compile time directive. */
#if (LL_ENABLE_TESTER != TRUE)
#error "LL_ENABLE_TESTER compilation directive must be set to 1."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Tester acknowledgement mode. */
enum
{
  LL_TESTER_ACK_MODE_NORMAL        = 0, /*!< Ack packets according to normal acknowledgement/flow control scheme. */
  LL_TESTER_ACK_MODE_NO_RX_ACK     = 1, /*!< Do not acknowledge Rx packets. */
  LL_TESTER_ACK_MODE_IGNORE_TX_ACK = 2  /*!< Ignore acknowledgements of Tx packets. */
};

#define LL_TESTER_TRIGGER_NONEMPTY 0xFE /*!< Trigger only on non-empty packets. */
#define LL_TESTER_TRIGGER_ALWAYS   0xFF /*!< Always trigger. */

#define LL_TESTER_ADVB_MAX_LEN      LL_ADVB_MAX_LEN + 4     /*!< Maximum allowed ADVB length. */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Link layer tester control block */
typedef struct
{
  /* Advertising channel override values. */
  /* Place here to 32-bit align. */
  uint8_t           txAdvPdu[LL_TESTER_ADVB_MAX_LEN];       /*!< Advertising PDU override buffer. */
  uint32_t          advAccessAddrRx;    /*!< Advertising access address override (Rx). */
  uint32_t          advAccessAddrTx;    /*!< Advertising access address override (Tx). */
  /* Place here to 32-bit align. */
  uint8_t           txScanReqPdu[LL_TESTER_ADVB_MAX_LEN];   /*!< Scan request PDU override buffer. */
  uint32_t          advCrcInitRx;       /*!< Advertising CRC value override (Rx). */
  uint32_t          advCrcInitTx;       /*!< Advertising CRC value override (Tx). */
  /* Place here to 32-bit align. */
  uint8_t           txScanRspPdu[LL_TESTER_ADVB_MAX_LEN];   /*!< Scan response PDU override buffer. */
  bool_t            txAdvPduLen;        /*!< Advertising PDU override buffer length, 0 to disable. */
  bool_t            txScanReqPduLen;    /*!< Scan request PDU override buffer length, 0 to disable. */
  bool_t            txScanRspPduLen;    /*!< Scan response PDU override buffer length, 0 to disable. */

  /* Extended advertising channel override values. */
  uint32_t          auxAccessAddrRx;    /*!< Auxiliary advertising access address override (Rx). */
  uint32_t          auxAccessAddrTx;    /*!< Auxiliary advertising access address override (Tx). */
  uint32_t          auxCrcInitRx;       /*!< Advertising CRC value override (Rx). */
  uint32_t          auxCrcInitTx;       /*!< Advertising CRC value override (Tx). */
  struct
  {
    uint8_t         pduMatch;           /*!< PDU type to override. */
    uint8_t         len;                /*!< Length of override buffer. */
    uint8_t         buf[LL_CONN_IND_PDU_LEN];
                                        /*!< Override request buffer. */
  } auxReq;                             /*!< Auxiliary request buffer override parameters. */
  struct
  {
    uint16_t        pduMatchMask;       /*!< Enable override bitmask of PDU types. */
    uint8_t         modifyMask;         /*!< Enable extended header field override bitmask. */
    uint64_t        AdvA;               /*!< AdvA override value. */
    uint64_t        TargetA;            /*!< TargetA override value. */
    uint8_t         SuppInfo;           /*!< SuppInfo override value. */
    uint16_t        ADI;                /*!< ADI override value. */
    int8_t          TxPower;            /*!< TxPower override value. */
  } extHdr;                             /*!< Extended header override values. */

  /* Data channel override values. */
  uint32_t          dataAccessAddrRx;   /*!< Data channel access address override (Rx). */
  uint32_t          dataAccessAddrTx;   /*!< Data channel access address override (Tx). */
  uint32_t          dataCrcInitRx;      /*!< Data channel CRC value override (Rx). */
  uint32_t          dataCrcInitTx;      /*!< Data channel CRC value override (Tx). */

  /* Connection parameter override values. */
  bool_t            connIndEnabled;     /*!< Connection indication override packet enabled. */
  lctrConnInd_t     connInd;            /*!< Connection indication override packet. */
  bool_t            connUpdIndEnabled;  /*!< Connection update indication override packet enabled. */
  lctrConnUpdInd_t  connUpdInd;         /*!< Connection update indication override packet. */
  bool_t            connParamReqEnabled;/*!< Connection parameter request override packet enabled. */
  lctrConnParam_t   connParamReq;       /*!< Connection parameter request override packet. */

  /* Data length override values. */
  bool_t            dataLenReqEnabled;  /*!< Local Data PDU parameters enabled. */
  lctrDataLen_t     dataLenReq;         /*!< Local Data PDU parameters. */

  uint32_t          connIntervalUs;     /*!< Connection interval override. */
  uint16_t          eventCounterOffset; /*!< Event counter offset value. */

  uint32_t          txLlcpFilter;       /*!< Filter for LLCP Tx. */
  uint32_t          rxLlcpFilter;       /*!< Filter for LLCP Rx. */
  bool_t            dataTriggerEnabled; /*!< Trigger is enabled. */
  uint8_t           dataTriggerPdu;     /*!< PDU on which to trigger data. */
  bool_t            dataTriggerAfter;   /*!< Data sent after PDU. */

  /* Packet override values. */
  uint32_t          pktMic;             /*!< Packet MIC override XOR value. */
  uint8_t           pktLlId;            /*!< Packet LLID override XOR value. */

  /* LLCP handling. */
  bool_t            llcpForwardEnabled; /*!< LLCP packet to host enable. */
  bool_t            llcpLlcpIntercept;  /*!< LLCP intercept enable. */

  /* Acknowledgement/flow control. */
  uint8_t           ackMode;            /*!< Acknowledgement mode. */
  uint8_t           ackTrigger;         /*!< Rx datum or condition on which to trigger special ack handling. */
} llTesterCtrlBlock_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern llTesterCtrlBlock_t  llTesterCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup LL_TESTER_API_INIT
 *  \{ */

void LlTesterInit(void);

/*  \} */

/*! \addtogroup LL_TESTER_API_ADV_CHAN
 *  \{ */

/* Advertising channel override */
void LlTesterSetAdvAccessAddr(uint32_t accessAddr, bool_t forRx);
void LlTesterSetAdvCrcInit(uint32_t crcInit, bool_t forRx);

/* Advertising data exchange */
uint8_t LlTesterSetTxAdvPdu(uint8_t *pBuf, uint8_t len);
uint8_t LlTesterSetTxScanReqPdu(uint8_t *pBuf, uint8_t len);
uint8_t LlTesterSetTxScanRspPdu(uint8_t *pBuf, uint8_t len);

/* Extended advertising override */
void LlTesterSetAuxAccessAddr(uint32_t accessAddr, bool_t forRx);
void LlTesterSetAuxCrcInit(uint32_t crcInit, bool_t forRx);
void LlTesterSetTxAuxReqPdu(uint8_t pduMatch, uint8_t *pBuf, uint8_t len);
void LlTesterSetExtAdvHdrFields(uint16_t pduMatchMask, uint8_t modifyMask,
                                uint8_t *pAdvA, uint8_t *pTargetA, uint8_t SuppInfo, uint16_t ADI, int8_t TxPower);

/*  \} */

/*! \addtogroup LL_TESTER_API_DATA_CHAN
 *  \{ */

/* Data channel override */
void LlTesterSetDataAccessAddr(uint32_t accessAddr, bool_t forRx);
uint8_t LlTesterGetDataAccessAddr(uint16_t handle, uint32_t *pAccessAddr);
void LlTesterSetDataCrcInit(uint32_t crcInit, bool_t forRx);
uint8_t LlTesterGetDataCrcInit(uint16_t handle, uint32_t *pCrcInit);

/* Data control override */
void LlTesterSetConnInd(uint32_t accessAddr, uint32_t crcInit, uint8_t txWinSize, uint16_t txWinOffset,
                        uint16_t interval, uint16_t latency, uint16_t timeout, uint64_t chanMask,
                        uint8_t hopInc, uint8_t masterSca);
void LlTesterAdjConnInterval(uint32_t intervalUs);

/* Data packet override */
void LlTesterSetPktLlId(uint8_t id);
void LlTesterSetPktMic(uint32_t mic);

/* Acknowledgement/flow control override */
void LlTesterSetAckMode(uint8_t ackMode, uint8_t ackTrigger);

/* Data exchange */
uint8_t LlTesterTxDataPdu(uint16_t handle, uint8_t *pBuf, uint8_t len);

/*  \} */

/*! \addtogroup LL_TESTER_API_LLCP
 *  \{ */

void LlTesterForwardLlcpToHost(bool_t enable, bool_t intercept);
void LlTesterSetEventCounterOffset(uint16_t offset);
uint8_t LlTesterSendConnUpdInd(uint16_t handle,
                               uint8_t txWinSize, uint16_t txWinOffset,
                               uint16_t interval, uint16_t latency, uint16_t timeout);
uint8_t LlTesterSendConnParamReq(uint16_t handle,
                                 uint16_t connIntervalMin, uint16_t connIntervalMax,
                                 uint16_t connLatency, uint16_t supTimeout, uint8_t prefPeriod,
                                 uint16_t refConnEvtCnt, uint16_t *pOffset);
uint8_t LlTesterSendDataLen(uint16_t handle,
                            uint16_t rxLen, uint16_t rxTime,
                            uint16_t txLen, uint16_t txTime);
uint8_t LlTesterSendPhyReq(uint16_t handle, uint8_t txPhys, uint8_t rxPhys);
uint8_t LlTesterSendPhyUpdateInd(uint16_t handle, uint8_t mToSPhy, uint8_t sToMPhy);
void LlTesterEnableRxFlowControl(bool_t enable);
void LlTesterSetTxLlcpFilter(uint32_t filter);
void LlTesterSetRxLlcpFilter(uint32_t filter);
void LlTesterSetDataTrigger(uint8_t pdu, bool_t enable, bool_t after);

/*  \} */

/*! \addtogroup LL_TESTER_API_BB
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief      Adjust Tx TIFS timing value.
 *
 *  \param      adjNs       Adjustment value in nanoseconds.
 *
 *  \return     None.
 *
 *  Adjust the TIFS timing of transmit by the given signed value of timer ticks.
 *  If adjustment value is out of range, maximum allowed value is used.
 */
/*************************************************************************************************/
void BbTesterAdjTxTifsNs(int16_t adjNs);

/*************************************************************************************************/
/*!
 *  \brief      Trigger channel modifications on matching Tx packet header.
 *
 *  \param      hdrMask     Header mask.
 *  \param      hdrValue    Match value.
 *
 *  \return     None.
 *
 *  Modify the transmit channel parameters of a packet only when the Tx packet header matches
 *  the given parameters. This applies to the modification parameter provided by the following
 *  routines:
 *
 *      - \ref BbTesterSetInvalidCrcInit()
 *      - \ref BbTesterSetInvalidAccessAddress()
 */
/*************************************************************************************************/
void BbTesterSetModifyTxPktTrigger(uint16_t hdrMask, uint16_t hdrValue);

/*************************************************************************************************/
/*!
 *  \brief      Invalidate CRC initialization value.
 *
 *  \param      chanMask    Invalidate channel mask.
 *  \param      adjMask     Number of adjustments (0 to disable).
 *  \param      forRx       TRUE for Rx, FALSE for Tx.
 *
 *  \return     None.
 *
 *  Force the receiver to receive a packet with CRC error if the receive channel is in
 *  \b chanMask while stepping through the invalid pattern in \b invalidMask.
 */
/*************************************************************************************************/
void BbTesterSetInvalidCrcInit(uint64_t chanMask, uint32_t adjMask, bool_t forRx);

/*************************************************************************************************/
/*!
 *  \brief      Invalidate access address value.
 *
 *  \param      chanMask    Invalidate channel mask.
 *  \param      invalidMask Enable mask for invalidating access address (0 to disable).
 *  \param      forRx       TRUE for Rx, FALSE for Tx.
 *
 *  \return     None.
 *
 *  Force the receiver to receive a miss a packet if the receive channel is in
 *  \b chanMask while stepping through the invalid pattern in \b invalidMask.
 */
/*************************************************************************************************/
void BbTesterSetInvalidAccessAddress(uint64_t chanMask, uint32_t invalidMask, bool_t forRx);

/*************************************************************************************************/
/*!
 *  \brief      Enable bypassing PDU filtering.
 *
 *  \param      enable      If TRUE enable bypass PDU filtering, FALSE otherwise.
 *
 *  \return     None.
 *
 *  Enable bypassing PDU filtering.
 */
/*************************************************************************************************/
void BbTesterEnablePduFilterBypass(bool_t enable);

/*  \} */

#ifdef __cplusplus
};
#endif

#endif /* LL_TESTER_API_H */
