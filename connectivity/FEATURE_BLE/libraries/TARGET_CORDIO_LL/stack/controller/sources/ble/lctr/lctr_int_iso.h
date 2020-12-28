/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connection interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef LCTR_INT_ISO_H
#define LCTR_INT_ISO_H

#include "lctr_api_iso.h"
#include "lctr_int.h"
#include "lctr_int_conn.h"
#include "lctr_pdu_iso.h"
#include "lmgr_api_iso.h"
#include "ll_defs.h"
#include "wsf_cs.h"
#include "pal_codec.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define LCTR_ISO_DATA_PDU_FC_OFFSET     0       /*!< Flow control fields data PDU offset. */
#define LCTR_ISO_DATA_PDU_LEN_OFFSET    1       /*!< Length field CIS or BIS Data PDU offset. */

/*! \brief      First CIS handle (follows ACL handles). */
#define LCTR_FIRST_CIS_HANDLE           (pLctrRtCfg->maxConn)

/*! \brief      First BIS handle (follows CIS handles). */
#define LCTR_FIRST_BIS_HANDLE           (pLctrRtCfg->maxConn + pLctrRtCfg->maxCis)

/*! \brief      Fragment header maximum length. */
#define LCTR_ISO_FRAG_HDR_MAX_LEN       (LL_ISO_DATA_HDR_LEN + LL_ISO_SEG_HDR_LEN + LL_ISO_SEG_TO_LEN)

/*! \brief      Fragment trailer maximum length. */
#define LCTR_ISO_FRAG_TRL_MAX_LEN       LL_DATA_MIC_LEN

/*! \brief      SDU offset to start of the SDU data buffer. */
#define LCTR_ISO_SDU_DATA_START_OFFSET  HCI_ISO_HDR_LEN + HCI_ISO_DL_MAX_LEN

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief      Transmit buffer descriptor. */
typedef struct
{
  uint8_t *pIsoSdu;                     /*!< ISO buffer. */
  uint8_t *pPduBuf;                     /*!< PDU data buffer. */
  uint16_t isoLen;                      /*!< ISO SDU length. */
  uint8_t  fragLen;                     /*!< Fragmentation length. */
  uint8_t  fragCnt;                     /*!< Current fragmentation Tx count. */
  struct
  {
    uint8_t hdrLen;                     /*!< Data PDU header length. */
    uint8_t trlLen;                     /*!< Data PDU trailer length. */
    uint8_t hdr[LCTR_ISO_FRAG_HDR_MAX_LEN];   /*!< Data PDU header. */
    uint8_t trl[LCTR_ISO_FRAG_TRL_MAX_LEN];   /*!< Data PDU trailer (i.e., MIC). */
  } frag[];                             /*!< Fragmented Data PDU packet data. */
} lctrIsoTxBufDesc_t;

/*! \brief      Start stream call signature. */
typedef bool_t (*lctrCodecStartStream)(uint16_t id, PalCodecSreamParam_t *pParam);

/*! \brief      Stop stream call signature. */
typedef void (*lctrCodecStopStream)(uint16_t id);

/*! \brief      Stream in data call signature. */
typedef uint16_t (*lctrCodecStreamIn)(uint16_t id, uint8_t *pBuf, uint16_t len, uint32_t *pPktCtr);

/*! \brief      Stream out data call signature. */
typedef void (*lctrCodecStreamOut)(uint16_t id, const uint8_t *pBuf, uint16_t len, uint32_t pktCtr);

/*! \brief      Codec event handlers. */
typedef struct
{
  lctrCodecStartStream  start;          /*!< Start stream. */
  lctrCodecStopStream   stop;           /*!< Stop stream. */
  lctrCodecStreamIn     in;             /*!< Stream data input. */
  lctrCodecStreamOut    out;            /*!< Stream data output. */
} lctrCodecHandlers_t;

/*! \brief      Codec event handlers. */
extern lctrCodecHandlers_t lctrCodecHdlr;

/*! \brief      Transmit ISOAL Context. */
typedef struct
{
  wsfQueue_t        pendingSduQ;        /*!< ISO pending SDU queue. */
  uint8_t           pendQueueSize;      /*!< Pending queue size. */
  uint16_t          sduOffset;          /*!< Offset of incomplete SDU. */
  uint8_t           compSdu;            /*!< Number of completed SDUs for current ISO event. */
} lctrIsoalTxCtx_t;

/*! \brief      Receive ISOAL Context. */
typedef struct
{
  uint8_t           *pPendSduBuf;       /*!< Pointer to incomplete SDU buffer. */
  uint16_t          rxSduOffset;        /*!< Offset of SDU currently being assembled. */
  bool_t            pduFlushed;         /*!< Most recent PDU was flushed due to failed RXs. */
  uint8_t           rxState;            /*!< RX state. */
  uint16_t          packetSequence;     /*!< Packet sequence number. */

  union
  {
    struct
    {
      uint32_t      timeOffset;         /*!< Time offset of an SDU. */
    } framed;                           /*!< Framed specific data. */

    struct
    {
      wsfQueue_t    pendSduQ;           /*!< Pending PDU fragments. */
      uint16_t      curLen;             /*!< Current length of SDU being received. */
      uint8_t       ps;                 /*!< Packet status. */
    } unframed;                         /*!< Unframed specific data. */
  } data;                               /*!< Framing-specific data. */
} lctrIsoalRxCtx_t;

/*! \brief      Input datapath context. */
typedef struct
{
  LlIsoDataPath_t   id;                 /*!< Input data path ID. */
} lctrInDataPathCtx_t;                  /*!< Input datapath configuration. */

/*! \brief      Output datapath context. */
typedef struct
{
  LlIsoDataPath_t   id;                 /*!< Output data path ID. */

  union
  {
    struct
    {
      wsfQueue_t    rxDataQ;            /*!< Receive data pending queue. */
      uint8_t       numRxPend;          /*!< Number of messages pending in the RX queue. */
    } hci;                              /*!< HCI data path configuration. */
  } cfg;                                /*!< Datapath-specific configuration parameters. */
} lctrOutDataPathCtx_t;                 /*!< Output datapath configuration. */

/*! \brief      Datapath context. */
typedef union
{
  lctrInDataPathCtx_t  in;       /*!< Input context. */
  lctrOutDataPathCtx_t out;      /*!< Output context. */
} lctrDataPathCtx_t;             /*!< Datapath context collection. */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Task event handler */
void lctrIsoTxCompletedHandler(void);
void lctrCisRxPendingHandler(void);
void lctrNotifyHostIsoEventComplete(uint8_t handle, uint32_t evtCtr);

/* ISO data path */
lctrIsoTxBufDesc_t *lctrAllocIsoTxBufDesc(void);
void lctrFreeIsoTxBufDesc(lctrIsoTxBufDesc_t *pDesc);
uint8_t lctrSetupIsoDataPath( LlIsoSetupDataPath_t *pSetupDataPath, lctrDataPathCtx_t *pDataPathCtx);
void lctrIsoSendCodecSdu(uint16_t id);
bool_t lctrIsoRxConnEnq(lctrOutDataPathCtx_t *pOutDataPathCtx, uint16_t handle, uint8_t *pBuf);
void lctrIsoOutDataPathClear(lctrOutDataPathCtx_t *pOutCtx);
void lctrIsoalRxDataPathClear(lctrIsoalRxCtx_t *pRxCtx, uint8_t framing);
void lctrIsoOutDataPathSetup(lctrOutDataPathCtx_t *pOutCtx);
uint8_t *lctrIsoRxConnDeq(lctrOutDataPathCtx_t *pOutCtx);
bool_t lctrIsoUnframedRxSduPendQueue(lctrIsoalRxCtx_t *pRxCtx, uint8_t *pSdu, uint16_t handle,
                                     uint16_t dataLen, uint8_t llid);

/* ISO Test mode. */
uint8_t *lctrGenerateIsoTestData(uint16_t handle, LlIsoPldType_t pldType, uint16_t maxSdu, uint32_t pktCtr);
void lctrValidateIsoTestData(uint8_t *pPld, uint8_t actLen, LlIsoTestCtrs_t *pRxStats, uint8_t pldType, uint16_t expMaxSdu, uint32_t expPldCtr);
/* ISOAL Utility. */
uint8_t lctrAssembleTxFramedPdu(lctrIsoalTxCtx_t *pIsoalTxCtx, uint8_t *pPduBuf, uint16_t maxPduLen);
uint8_t lctrAssembleRxFramedSdu(lctrIsoalRxCtx_t *pIsoalRxCtx, wsfQueue_t *pRxQueue,
                                uint16_t handle, uint8_t *pIsoBuf, uint8_t len);
uint8_t *lctrTxIsoDataPduAlloc(void);

/*************************************************************************************************/
/*!
 *  \brief  Increment available Tx data buffers.
 */
/*************************************************************************************************/
static inline void lctrIsoSduTxIncAvailBuf(void)
{
  WSF_CS_INIT();

  WSF_CS_ENTER();
  lmgrIsoCb.availTxBuf++;
  WSF_CS_EXIT();
}

/*************************************************************************************************/
/*!
 *  \brief  Decrement available Tx data buffers.
 */
/*************************************************************************************************/
static inline void lctrIsoSduTxDecAvailBuf(void)
{
  WSF_CS_INIT();

  WSF_CS_ENTER();
  lmgrIsoCb.availTxBuf--;
  WSF_CS_EXIT();
}

/*************************************************************************************************/
/*!
 *  \brief  Increment available Rx data buffers.
 *
 *  \param  numBufs     Number of additional buffers available.
 */
/*************************************************************************************************/
static inline void lctrIsoDataRxIncAvailBuf(uint8_t numBufs)
{
  WSF_CS_INIT();

  WSF_CS_ENTER();
  lmgrIsoCb.availRxBuf += numBufs;
  WSF_CS_EXIT();
}

/*************************************************************************************************/
/*!
 *  \brief  Decrement available Rx data buffers.
 */
/*************************************************************************************************/
static inline void lctrIsoDataRxDecAvailBuf(void)
{
  WSF_CS_INIT();

  WSF_CS_ENTER();
  lmgrIsoCb.availRxBuf--;
  WSF_CS_EXIT();
}

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ISO_H */
