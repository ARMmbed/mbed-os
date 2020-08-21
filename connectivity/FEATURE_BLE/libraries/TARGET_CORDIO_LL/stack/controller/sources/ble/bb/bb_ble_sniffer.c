/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  BB BLE Sniffer utility function definition file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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

#include "bb_ble_sniffer_api.h"
#include "wsf_types.h"
#include "pal_bb_ble.h"
#include "bb_api.h"
#include "ll_api.h"
#include "bb_ble_api_op.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "pal_bb.h"
#include "bb_ble_int.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! brief   Sniffer VS output packet function placeholder. */
static void bbSnifferOutputVendorSpec(BbBleSnifferPkt_t * pPktData);

/*! brief   Sniffer output table. */
bbSnifferFn_t bbSnifferOutTbl[BB_SNIFFER_OUTPUT_TOTAL_METHODS] =
{
  bbSnifferOutputVendorSpec                    /* BB_SNIFFER_OUTPUT_HCI_TOKEN */
};

/*! brief   Sniffer VS get packet function placeholder. */
static BbBleSnifferPkt_t * bbSnifferGetPktVendorSpec();

/*! brief   Sniffer get packet table. */
bbSnifferGetPktFn_t bbSnifferGetPktTbl[BB_SNIFFER_OUTPUT_TOTAL_METHODS] =
{
  bbSnifferGetPktVendorSpec                    /* BB_SNIFFER_OUTPUT_HCI_TOKEN */
};
/*! brief   Sniffer context. */
bbSnifferCtx_t bbSnifferCtx;

/**************************************************************************************************
  Packet retrival methods
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Retrive packet for processing.
 *
 *  \return     Pointer to packet to be packed.
 *
 */
/*************************************************************************************************/
static BbBleSnifferPkt_t * bbSnifferGetPktVendorSpec()
{
  BbBleSnifferHciCtx_t * pHci = &bbSnifferCtx.outputCtx.hci;

  if ((pHci->bufIdx == BB_SNIFFER_MAX_NUM_BUF) ||
      (bbSnifferCtx.enabled == FALSE))
  {
    return NULL;
  }

  return &pHci->pktBuf[pHci->bufIdx];
}


/**************************************************************************************************
  Output methods
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Finalize sniffer packet by incrementing index.
 *
 *  \param      pPktData    Packet to be pushed.
 *
 */
/*************************************************************************************************/
static void bbSnifferOutputVendorSpec(BbBleSnifferPkt_t * pPktData)
{
  BbBleSnifferHciCtx_t * pHci = &bbSnifferCtx.outputCtx.hci;

  pHci->bufIdx++;
}
/*************************************************************************************************/
/*!
 *  \brief      Master primary scan sniffer packet handler.
 *
 *  \param      pBod            Pointer to BOD.
 *  \param      pPktData        Pointer to sniffer packet.
 *
 *  Pack sniffer packet and call to output method function.
 */
/*************************************************************************************************/
void bbBleSnifferMstScanPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t * pPktData)
{
  BbBleMstAdvEvent_t * pScan = &pBod->prot.pBle->op.mstAdv;

  /* Pack channelization data. */
  memcpy(&pPktData->pktType.meta.chan, &pBod->prot.pBle->chan, sizeof(pPktData->pktType.meta.chan));

  /* Pack metadata. */
  pPktData->pktType.meta.timeStamp   =  bbSnifferCtx.packetCtr++;

  /* Pack header. */
  switch (pPktData->pktType.meta.state)
  {
    case BB_EVT_STATE_RX_ADV_IND:
      /* Handled in the Rx Callback. */
      break;

    case BB_EVT_STATE_TX_SCAN_OR_CONN_INIT:
      memcpy(pPktData->pktType.advPkt.hdr, pScan->pTxReqBuf, LL_ADV_HDR_LEN);
      break;

    case BB_EVT_STATE_RX_SCAN_OR_CONN_RSP:
      /* Handled in the Rx Callback. */
      break;

    default:
      break;
  }

  WSF_ASSERT(bbSnifferCtx.snifferOutCb);

  bbSnifferCtx.snifferOutCb(pPktData);
}

/*************************************************************************************************/
/*!
 *  \brief      Master auxiliary scan sniffer packet handler
 *
 *  \param      pBod            Pointer to BOD.
 *  \param      pPktData        Pointer to sniffer packet.
 *
 *  Pack sniffer packet and call to output method function.
 */
/*************************************************************************************************/
void bbBleSnifferMstAuxScanPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t *pPktData)
{
  BbBleMstAuxAdvEvent_t * pScan = &pBod->prot.pBle->op.mstAuxAdv;

  /* Pack channelization data. */
  memcpy(&pPktData->pktType.meta.chan, &pBod->prot.pBle->chan, sizeof(pPktData->pktType.meta.chan));

  /* Pack metadata. */
  pPktData->pktType.meta.timeStamp   =  bbSnifferCtx.packetCtr++;

  /* Pack header. */
  switch (pPktData->pktType.meta.state)
  {
    case BB_EVT_STATE_RX_ADV_IND:
      /* Handled in the Rx Callback. */
      break;

    case BB_EVT_STATE_TX_SCAN_OR_CONN_INIT:
      memcpy(pPktData->pktType.advPkt.hdr, pScan->pTxAuxReqBuf, LL_ADV_HDR_LEN);
      break;

    case BB_EVT_STATE_RX_SCAN_OR_CONN_RSP:
      /* Handled in the Rx Callback. */
      break;

    case BB_EVT_STATE_RX_CHAIN_IND:
      /* Handled in the Rx callback. */
      break;

    default:
      break;
  }

  WSF_ASSERT(bbSnifferCtx.snifferOutCb);

  bbSnifferCtx.snifferOutCb(pPktData);
}

/*************************************************************************************************/
/*!
 *  \brief      Master periodic scan sniffer packet handler.
 *
 *  \param      pBod            Pointer to BOD.
 *  \param      pPktData        Pointer to sniffer packet.
 *
 *  Pack sniffer packet and call to output method function.
 */
/*************************************************************************************************/
void bbBleSnifferMstPerScanPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t *pPktData)
{
  /* Pack channelization data. */
  memcpy(&pPktData->pktType.meta.chan, &pBod->prot.pBle->chan, sizeof(pPktData->pktType.meta.chan));

  /* Pack metadata. */
  pPktData->pktType.meta.timeStamp   =  bbSnifferCtx.packetCtr++;

  /* Pack header. */
  switch (pPktData->pktType.meta.state)
  {
    case BB_EVT_STATE_RX_ADV_IND:
      /* Handled in the Rx Callback. */
      break;

    case BB_EVT_STATE_RX_CHAIN_IND:
      /* Handled in the Rx callback. */
      break;

    default:
      break;
  }

  WSF_ASSERT(bbSnifferCtx.snifferOutCb);

  bbSnifferCtx.snifferOutCb(pPktData);
}

/*************************************************************************************************/
/*!
 *  \brief      Slave primary advertising sniffer packet handler.
 *
 *  \param      pBod            Pointer to BOD.
 *  \param      pPktData        Pointer to sniffer packet.
 *
 *  Pack sniffer packet and call to output method function.
 */
/*************************************************************************************************/
void bbBleSnifferSlvAdvPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t * pPktData)
{
  BbBleSlvAdvEvent_t * pAdv = &pBod->prot.pBle->op.slvAdv;

  /* Pack channelization data. */
  memcpy(&pPktData->pktType.meta.chan, &pBod->prot.pBle->chan, sizeof(pPktData->pktType.meta.chan));

  /* Pack saved channel index. */
  pPktData->pktType.meta.chan.chanIdx = bbSnifferCtx.chanIdx;

  /* Pack metadata. */
  pPktData->pktType.meta.timeStamp = bbSnifferCtx.packetCtr++;

  /* Pack header. */
  switch (pPktData->pktType.meta.state)
  {
    case BB_EVT_STATE_TX_ADV_IND:
      memcpy(pPktData->pktType.advPkt.hdr, pAdv->pTxAdvBuf, LL_ADV_HDR_LEN);
      break;

    case BB_EVT_STATE_RX_SCAN_OR_CONN_INIT:
      /* Handled in the Rx Callback. */
      break;

    case BB_EVT_STATE_TX_SCAN_OR_CONN_RSP:
      memcpy(pPktData->pktType.advPkt.hdr, pAdv->pTxRspBuf, LL_ADV_HDR_LEN);
      break;

    default:
      break;
  }

  WSF_ASSERT(bbSnifferCtx.snifferOutCb);

  bbSnifferCtx.snifferOutCb(pPktData);
}

/*************************************************************************************************/
/*!
 *  \brief      Slave auxiliary advertising sniffer packet handler.
 *
 *  \param      pBod            Pointer to BOD.
 *  \param      pPktData        Pointer to sniffer packet.
 *
 *  Pack sniffer packet and call to output method function.
 */
/*************************************************************************************************/
void bbBleSnifferSlvAuxAdvPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t * pPktData)
{
  BbBleSlvAuxAdvEvent_t * pAdv = &pBod->prot.pBle->op.slvAuxAdv;

  /* Pack channelization data. */
  memcpy(&pPktData->pktType.meta.chan, &pBod->prot.pBle->chan, sizeof(pPktData->pktType.meta.chan));

  /* Pack metadata. */
  pPktData->pktType.meta.timeStamp = bbSnifferCtx.packetCtr++;

  /* Pack header. */
  switch (pPktData->pktType.meta.state)
  {
    case BB_EVT_STATE_TX_ADV_IND:
      /* Handled in the Tx Callback. */
      break;

    case BB_EVT_STATE_RX_SCAN_OR_CONN_INIT:
      /* Handled in the Rx Callback. */
      break;

    case BB_EVT_STATE_TX_SCAN_OR_CONN_RSP:
      memcpy(pPktData->pktType.advPkt.hdr, pAdv->txAuxRspPdu[0].pBuf, LL_ADV_HDR_LEN);
      break;

    case BB_EVT_STATE_TX_CHAIN_IND:
      /* Handled in the Tx Callback. */
      break;

    default:
      break;
  }

  WSF_ASSERT(bbSnifferCtx.snifferOutCb);

  bbSnifferCtx.snifferOutCb(pPktData);
}

/*************************************************************************************************/
/*!
 *  \brief      Connection sniffer packet handler.
 *
 *  \param      pBod            Pointer to BOD.
 *  \param      pPktData        Pointer to sniffer packet.
 *
 *  Pack sniffer packet and call to output method function.
 */
/*************************************************************************************************/
void bbBleSnifferConnPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t * pPktData)
{
  /* BbBleMstConnEvent_t * pConn = &pBod->prot.pBle->op.mstConn; */

  /* Pack channelization data. */
  memcpy(&pPktData->pktType.meta.chan, &pBod->prot.pBle->chan, sizeof(pPktData->pktType.meta.chan));

  /* Pack metadata. */
  pPktData->pktType.meta.timeStamp = bbSnifferCtx.packetCtr++;

  /* Pack Header. */
  if (pPktData->pktType.meta.type == BB_SNIFF_PKT_TYPE_TX)
  {
    memcpy(pPktData->pktType.dataPkt.hdr, bbSnifferCtx.txBuf, LL_DATA_HDR_MAX_LEN);
  }
  else /* (type == BB_SNIFF_PKT_TYPE_RX) */
  {
    /* Header copy is done in the Rx comp callback. */
  }

  WSF_ASSERT(bbSnifferCtx.snifferOutCb);

  bbSnifferCtx.snifferOutCb(pPktData);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize packet sniffer
 *
 *  \param      outMethod     Output method.
 *  \param      enable        Enable.
 *
 *  \return     Status error code.
 *
 *  Initialize packet sniffer with specified output method.
 */
/*************************************************************************************************/
uint8_t BbBleInitSniffer(uint8_t outMethod, bool_t enable)
{
  memset(&bbSnifferCtx, 0, sizeof(bbSnifferCtx));

  if (enable == FALSE)
  {
    return LL_SUCCESS;
  }

  if (outMethod >= BB_SNIFFER_OUTPUT_TOTAL_METHODS)
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  bbSnifferCtx.snifferGetPktFn = bbSnifferGetPktTbl[outMethod];
  bbSnifferCtx.snifferOutCb = bbSnifferOutTbl[outMethod];
  bbSnifferCtx.enabled = enable;

  return LL_SUCCESS;
}

