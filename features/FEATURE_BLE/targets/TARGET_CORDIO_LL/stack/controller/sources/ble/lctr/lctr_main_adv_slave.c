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
 * \brief Link layer controller slave advertising operation builder implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_adv_slave.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "bb_ble_api.h"
#include "bb_ble_api_reslist.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_cs.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Globals
**************************************************************************************************/

/*! \brief      Advertising operational context. */
lctrSlvAdvCtx_t lctrSlvAdv;

/*! \brief      Assert BB meets advertising PDU requirements. */
WSF_CT_ASSERT((BB_FIXED_ADVB_PKT_LEN == 0) ||
              (BB_FIXED_ADVB_PKT_LEN >= LL_ADVB_MAX_LEN));

/**************************************************************************************************
  Internal functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Slave advertising reset handler.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrSlvAdvResetHandler(void)
{
  BbBleAdvSlaveInit();
  LctrSlvAdvDefaults();
  LmgrSlvInit();
}

/*************************************************************************************************/
/*!
 *  \brief      Slave advertising message dispatcher.
 *
 *  \param      pMsg    Pointer to message buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrSlvAdvDisp(LctrAdvMsg_t *pMsg)
{
  lctrSlvAdvExecuteSm(pMsg->hdr.event);
}

/*************************************************************************************************/
/*!
 *  \brief  Compute new access address.
 *
 *  \return New access address value.
 */
/*************************************************************************************************/
uint32_t lctrComputeAccessAddr(void)
{
  uint32_t accessAddr = LlMathRandNum();

  /*
   * The following code enforces a pattern to make sure the address meets all requirements
   * (including requirements for the LE coded PHY).  The pattern is
   *
   *   0byyyyyy1x 0xxxx1x0 xxxx1x0x xx11x0x1
   *
   * with 2^5 choices for the upper six bits.  This provides 2^5 * 2^16 = 2097152 variations.
   */

  /* Patterns for upper six bits.  The lower row contains complemented values of the upper row. */
  static const uint8_t upperSixBits[] =
  {
    /* 000010 000100 000101 000110 001000 001100 001101 001110 010000 010001 010011 010111 010110 011000 011100 011110 */
       0x08,  0x10,  0x14,  0x18,  0x20,  0x30,  0x34,  0x38,  0x40,  0x44,  0x4C,  0x5C,  0x58,  0x60,  0x70,  0x78,
    /* 111101 111011 111010 111001 110111 110011 110010 110001 101111 101110 101100 101000 101001 100111 100011 100001 */
       0xF4,  0xEC,  0xE8,  0xE4,  0xDC,  0xCC,  0xC8,  0xC4,  0xBC,  0xB8,  0xB0,  0xA0,  0xA4,  0x9C,  0x8C,  0x84
  };

  /* Set the upper six bits. */
  accessAddr  = (accessAddr & ~0xFC000000) | (upperSixBits[accessAddr >> 27] << 24);

  /* Set   ones  with the mask 0b00000010 00000100 00001000 00110001 */
  accessAddr |=  0x02040831;

  /* Clear zeros with the mask 0b00000000 10000001 00000010 00000100 */
  accessAddr &= ~0x00810204;

  return accessAddr;
}

/*************************************************************************************************/
/*!
 *  \brief      Scan request post-processing.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSlvRxScanReq(void)
{
  lctrSlvScanReport_t *pScanReport;
  wsfHandlerId_t handlerId;

  while ((pScanReport = WsfMsgDeq(&lctrSlvAdv.rxScanReqQ, &handlerId)) != NULL)
  {
    LlScanReportInd_t evt =
    {
        .hdr =
        {
          .param  = 0,
          .event  = LL_SCAN_REPORT_IND,
          .status = LL_SUCCESS
        },

        .peerAddr     = pScanReport->scanIdAddr,
        .peerAddrType = pScanReport->scanIdAddrType,
        .peerRpa      = 0
    };
    if ((evt.peerAddr     != pScanReport->scanAddr) ||
        (evt.peerAddrType != pScanReport->scanAddrType))
    {
      evt.peerAddrType |= LL_ADDR_IDENTITY_BIT;
      evt.peerRpa       = pScanReport->scanAddr;
    }

    LmgrSendEvent((LlEvt_t *)&evt);
    WsfMsgFree(pScanReport);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Choose Advertising Address.
 *
 *  \param  pBle            BLE data.
 *  \param  pPduHdr         PDU header.
 *  \param  ownAddrType     Own address type.
 *  \param  peerAddrType    Peer address type.
 *  \param  peerAddr        Peer address.
 *  \param  pAdvA           Storage for AdvA.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrChooseAdvA(BbBleData_t * const pBle, lctrAdvbPduHdr_t *pPduHdr,
                    uint8_t ownAddrType, uint8_t peerAddrType, uint64_t peerAddr,
                    uint64_t *pAdvA)
{
  /* Choose advertiser's address type. */
  lmgrSlvAdvCb.ownAddrType = ownAddrType & ~LL_ADDR_IDENTITY_BIT;  /* Fallback to non-RPA. */
  lmgrSlvAdvCb.localRpa = 0;

  /* Fallback onto non-RPA if local IRK is zero or peer address is not in resolving list. */
  if ((lmgrSlvAdvCb.ownAddrType == LL_ADDR_RANDOM_BIT) &&
      (lmgrCb.bdAddrRndValid))
  {
    pPduHdr->txAddrRnd = TRUE;
    *pAdvA = lmgrCb.bdAddrRnd;
    lmgrCb.bdAddrRndModAdv = FALSE;
  }
  else
  {
    pPduHdr->txAddrRnd = FALSE;
    *pAdvA = lmgrPersistCb.bdAddr;
  }

  /* Generate local RPA for advertisement. */
  if (ownAddrType & LL_ADDR_IDENTITY_BIT)
  {
    uint64_t localRpa;

    if (BbBleResListReadLocal(peerAddrType, peerAddr, &localRpa))
    {
      /* Actually using RPA.  Even though this is a RPA, any received local address will be strictly matched (never resolved). */
      pPduHdr->txAddrRnd = TRUE;
      *pAdvA = localRpa;
      lmgrSlvAdvCb.ownAddrType = ownAddrType;
      lmgrSlvAdvCb.localRpa    = localRpa;  /* Save local RPA for connection complete event. */
    }
  }

  /* Always match local address in PDU to advertiser's address. */
  pBle->pduFilt.localAddrMatch = *pAdvA;
  if (pPduHdr->txAddrRnd)
  {
    BB_BLE_PDU_FILT_SET_FLAG(&pBle->pduFilt, LOCAL_ADDR_MATCH_RAND);
  }
  BB_BLE_PDU_FILT_SET_FLAG(&pBle->pduFilt, LOCAL_ADDR_MATCH_ENA);

  /* Sometimes match peer address, but always match to command parameters. */
  pBle->pduFilt.peerAddrMatch = peerAddr;
  if (peerAddrType)
  {
    BB_BLE_PDU_FILT_SET_FLAG(&pBle->pduFilt, PEER_ADDR_MATCH_RAND);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Choose peer address.
 *
 *  \param  pBle            BLE data.
 *  \param  ownAddrType     Own address type.
 *  \param  peerAddrType    Peer address type.
 *  \param  peerAddr        Peer address.
 *  \param  pPeerRpa        Storage for peer RPA.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrChoosePeerAddr(BbBleData_t * const pBle, uint8_t ownAddrType,
                        uint8_t peerAddrType, uint64_t peerAddr, uint64_t *pPeerRpa)
{
  if (lmgrCb.addrResEna)
  {
    /* Attempt to generate RPA for peer. */
    if (ownAddrType & LL_ADDR_IDENTITY_BIT)
    {
      BbBleResListGeneratePeer(peerAddrType, peerAddr, pPeerRpa);
    }
    /* Resolve peer RPAs whether or not a RPA was generated for peer. */
    BB_BLE_PDU_FILT_SET_FLAG(&pBle->pduFilt, PEER_ADDR_RES_ENA);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Build advertising operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lctrSlvAdvBuildOp(void)
{
  lctrAdvbPduHdr_t pduHdr = { 0 };

  if ((lmgrCb.features & LL_FEAT_CH_SEL_2) &&
      ((lmgrSlvAdvCb.advParam.advType == LL_ADV_CONN_DIRECT_HIGH_DUTY) ||
       (lmgrSlvAdvCb.advParam.advType == LL_ADV_CONN_DIRECT_LOW_DUTY) ||
       (lmgrSlvAdvCb.advParam.advType == LL_ADV_CONN_UNDIRECT)))
  {
    pduHdr.chSel = LL_CH_SEL_2;
  }
  /* else use LL_CH_SEL_1 */

  /* Pre-resolve common structures for efficient access. */
  BbOpDesc_t * const pOp = &lctrSlvAdv.advBod;
  BbBleData_t * const pBle = &lctrSlvAdv.bleData;
  BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;

  memset(pOp, 0, sizeof(BbOpDesc_t));
  memset(pBle, 0, sizeof(BbBleData_t));

  /*** General Setup ***/

  pOp->reschPolicy = BB_RESCH_MOVEABLE_PREFERRED;
  pOp->protId = BB_PROT_BLE;
  pOp->prot.pBle = pBle;
  pOp->endCback = lctrSlvAdvEndOp;
  pOp->abortCback = lctrSlvAdvEndOp;

  /*** BLE General Setup ***/

  pBle->chan.opType = BB_BLE_OP_SLV_ADV_EVENT;

  /* pBle->chan.chanIdx = 0; */     /* overridden by BB */
  pBle->chan.txPower = lmgrCb.advTxPwr;
  pBle->chan.accAddr = LL_ADV_ACCESS_ADDR;
  pBle->chan.crcInit = LL_ADV_CRC_INIT;
  pBle->chan.rxPhy = BB_PHY_BLE_1M;
  pBle->chan.txPhy = BB_PHY_BLE_1M;

#if (LL_ENABLE_TESTER)
  pBle->chan.accAddrRx = llTesterCb.advAccessAddrRx ^ pBle->chan.accAddr;
  pBle->chan.accAddrTx = llTesterCb.advAccessAddrTx ^ pBle->chan.accAddr;
  pBle->chan.crcInitRx = llTesterCb.advCrcInitRx ^ pBle->chan.crcInit;
  pBle->chan.crcInitTx = llTesterCb.advCrcInitTx ^ pBle->chan.crcInit;
#endif

  /* Activate white list only for undirected advertisement. */
  switch (lmgrSlvAdvCb.advParam.advType)
  {
    case LL_ADV_CONN_DIRECT_HIGH_DUTY:
    case LL_ADV_CONN_DIRECT_LOW_DUTY:
      break;
    default:
      if (lmgrSlvAdvCb.advParam.advFiltPolicy & LL_ADV_FILTER_SCAN_WL_BIT)
      {
        pBle->pduFilt.wlPduTypeFilt |= (1 << LL_PDU_SCAN_REQ);
      }
      if (lmgrSlvAdvCb.advParam.advFiltPolicy & LL_ADV_FILTER_CONN_WL_BIT)
      {
        pBle->pduFilt.wlPduTypeFilt |= (1 << LL_PDU_CONNECT_IND);
      }
      break;
  }

  /*** BLE Advertising Setup: Tx advertising packet ***/

  pAdv->advChMap = lmgrSlvAdvCb.advParam.advChanMap;

  if (lmgrGetOpFlag(LL_OP_MODE_FLAG_ENA_ADV_CHAN_RAND))
  {
    pAdv->firstAdvChIdx = LlMathRandNum() % LL_NUM_CHAN_ADV;
  }
  else
  {
    pAdv->firstAdvChIdx = 0;
  }

  switch (lmgrSlvAdvCb.advParam.advType)
  {
    case LL_ADV_CONN_UNDIRECT:
      pduHdr.pduType = LL_PDU_ADV_IND;
      pBle->pduFilt.pduTypeFilt = (1 << LL_PDU_SCAN_REQ) | (1 << LL_PDU_CONNECT_IND);
      break;
    case LL_ADV_CONN_DIRECT_HIGH_DUTY:
      /* Advertising interval is determined by LL not host. */
      lmgrSlvAdvCb.advParam.advInterMin = 0;
      lmgrSlvAdvCb.advParam.advInterMax = BB_BLE_TO_BB_TICKS(LL_DIR_ADV_INTER_TICKS);
      lmgrSlvAdvCb.advTermCntDown = BB_BLE_TO_BB_TICKS(LL_DIR_ADV_DUR_TICKS);
      pAdv->firstAdvChIdx = 0;  /* High duty cycle always start from channel 37. */
      /* Fallthrough */
    case LL_ADV_CONN_DIRECT_LOW_DUTY:
      pduHdr.pduType = LL_PDU_ADV_DIRECT_IND;
      pBle->pduFilt.pduTypeFilt = (1 << LL_PDU_CONNECT_IND);
      break;
    case LL_ADV_SCAN_UNDIRECT:
      pduHdr.pduType = LL_PDU_ADV_SCAN_IND;
      pBle->pduFilt.pduTypeFilt = (1 << LL_PDU_SCAN_REQ);
      break;
    case LL_ADV_NONCONN_UNDIRECT:
      pduHdr.pduType = LL_PDU_ADV_NONCONN_IND;
      break;
    default:
      return;
  }

  /* Choose advertiser's address type. */
  uint64_t advA;
  lctrChooseAdvA(pBle, &pduHdr, lmgrSlvAdvCb.advParam.ownAddrType,
                 lmgrSlvAdvCb.advParam.peerAddrType, lmgrSlvAdvCb.advParam.peerAddr,
                 &advA);

  uint64_t peerRpa = 0;
  lctrChoosePeerAddr(pBle, lmgrSlvAdvCb.advParam.ownAddrType,
                     lmgrSlvAdvCb.advParam.peerAddrType, lmgrSlvAdvCb.advParam.peerAddr,
                     &peerRpa);

  uint8_t *pBuf = lctrSlvAdv.advBuf + LL_ADV_HDR_LEN;
  switch (lmgrSlvAdvCb.advParam.advType)
  {
    /* Pack using generic advertising PDU construction. */
    case LL_ADV_CONN_UNDIRECT:
    case LL_ADV_SCAN_UNDIRECT:
    case LL_ADV_NONCONN_UNDIRECT:
    default:
      pduHdr.len = lctrPackAdvPdu(pBuf, advA, &lmgrSlvAdvCb.advData);
      pBuf += pduHdr.len;
      lmgrSlvAdvCb.advData.modified = FALSE;
      break;

    /* Pack using directed advertising PDU construction. */
    case LL_ADV_CONN_DIRECT_HIGH_DUTY:
    case LL_ADV_CONN_DIRECT_LOW_DUTY:
    {
      uint8_t  initAType = lmgrSlvAdvCb.advParam.peerAddrType;
      uint64_t initA     = lmgrSlvAdvCb.advParam.peerAddr;
      if (peerRpa != 0)
      {
        initAType = TRUE;
        initA     = peerRpa;
      }

      pduHdr.rxAddrRnd = initAType;
      pduHdr.len       = lctrPackConnDirectIndAdvPdu(pBuf, advA, initA);
      pBuf += pduHdr.len;

      /* Always match peer address in PDU to initiator's address for directed advertising. */
      BB_BLE_PDU_FILT_SET_FLAG(&pBle->pduFilt, PEER_ADDR_MATCH_ENA);
      break;
    }
  }

  lctrPackAdvbPduHdr(lctrSlvAdv.advBuf, &pduHdr);

  pAdv->txAdvLen = pBuf - lctrSlvAdv.advBuf;
  pAdv->pTxAdvBuf = lctrSlvAdv.advBuf;

#if (LL_ENABLE_TESTER)
  if (llTesterCb.txAdvPduLen)
  {
    pAdv->txAdvLen = llTesterCb.txAdvPduLen;
    pAdv->pTxAdvBuf = llTesterCb.txAdvPdu;
  }
#endif

  /*** BLE Advertising Setup: Rx scan request/connection indication packet ***/

  switch (lmgrSlvAdvCb.advParam.advType)
  {
    case LL_ADV_CONN_UNDIRECT:
    case LL_ADV_CONN_DIRECT_HIGH_DUTY:
    case LL_ADV_CONN_DIRECT_LOW_DUTY:
    case LL_ADV_SCAN_UNDIRECT:
    {
      lctrMsgHdr_t *pMsg;

      if ((pMsg = WsfMsgAlloc(sizeof(lctrMsgHdr_t) + BB_REQ_PDU_MAX_LEN)) != NULL)
      {
        pAdv->pRxReqBuf = (uint8_t *)(pMsg + 1);      /* hide header */
        pAdv->rxReqCback = lctrSlvAdvHandler;
        pAdv->rxReqPostCback = lctrSlvAdvPostProcessHandler;
      }
      else
      {
        /* Attempt to obtain buffer on next advertising operation. */
        LL_TRACE_ERR0("Could not allocate response buffer");
        // TODO need OOM recovery
      }
      break;
    }
    case LL_ADV_NONCONN_UNDIRECT:
    default:
      pAdv->pRxReqBuf = NULL;
      break;
  }

  /*** BLE Advertising Setup: Tx scan response packet ***/

  switch (lmgrSlvAdvCb.advParam.advType)
  {
    case LL_ADV_CONN_UNDIRECT:
    case LL_ADV_SCAN_UNDIRECT:
    {
      pduHdr.pduType = LL_PDU_SCAN_RSP;
      pduHdr.len = LL_SCAN_PREFIX_LEN + lmgrSlvAdvCb.scanRspData.len;

      pBuf  = lctrSlvAdv.scanRspBuf;
      pBuf += lctrPackAdvbPduHdr(pBuf, &pduHdr);
      pBuf += lctrPackScanRspPdu(pBuf, advA, &lmgrSlvAdvCb.scanRspData);
      lmgrSlvAdvCb.scanRspData.modified = FALSE;

      pAdv->pTxRspBuf = lctrSlvAdv.scanRspBuf;
      pAdv->txRspLen = pBuf - lctrSlvAdv.scanRspBuf;

#if (LL_ENABLE_TESTER)
      if (llTesterCb.txScanRspPduLen)
      {
        pAdv->pTxRspBuf = llTesterCb.txScanRspPdu;
        pAdv->txRspLen = llTesterCb.txScanRspPduLen;
      }
#endif
      break;
    }
    case LL_ADV_CONN_DIRECT_HIGH_DUTY:
    case LL_ADV_CONN_DIRECT_LOW_DUTY:
    case LL_ADV_NONCONN_UNDIRECT:
    default:
      pAdv->pTxRspBuf = NULL;
      break;
  }

  /*** Commit operation ***/

  lctrSlvAdv.shutdown = FALSE;
  lctrSlvAdv.connIndRcvd = FALSE;
  SchBleCalcAdvOpDuration(pOp, 0);
  SchInsertNextAvailable(pOp);

  LL_TRACE_INFO1("### AdvEvent ###  Advertising enabled, due=%u", pOp->due);
}

/*************************************************************************************************/
/*!
 *  \brief      Cleanup resources on advertising operation termination.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSlvAdvCleanupOp(void)
{
  lctrMsgHdr_t *pMsg = (lctrMsgHdr_t *)lctrSlvAdv.bleData.op.slvAdv.pRxReqBuf;

  if (pMsg)
  {
    /* Recover header. */
    WsfMsgFree(pMsg - 1);
  }

  LL_TRACE_INFO0("### AdvEvent ###  Advertising disabled");
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer controller resources for advertising slave.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrSlvAdvInit(void)
{
  /* Add advertising reset handler. */
  lctrResetHdlrTbl[LCTR_DISP_ADV] = lctrSlvAdvResetHandler;

  /* Add advertising message dispatchers. */
  lctrMsgDispTbl[LCTR_DISP_ADV] = (LctrMsgDisp_t)lctrSlvAdvDisp;

  /* Add advertising event handlers. */
  lctrEventHdlrTbl[LCTR_EVENT_RX_SCAN_REQ] = lctrSlvRxScanReq;

  LctrSlvAdvDefaults();
}

/*************************************************************************************************/
/*!
 *  \brief      Set default values for advertising slave.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrSlvAdvDefaults(void)
{
  memset(&lctrSlvAdv, 0, sizeof(lctrSlvAdv));
}
