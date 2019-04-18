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
 * \brief Arm Ltd. vendor specific HCI command module implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "hci_defs.h"
#include "bb_ble_api.h"
#include "ll_api.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Decode Arm Ltd. connection vendor specific HCI command packet.
 *
 *  \param  pHdr    Unpacked command header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing for vendor specific commands.
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciConnVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t evtParamLen = 1;      /* default is status field only */
  uint8_t peerMinUsedChan[LL_MAX_PHYS] = {2};

  /* Decode and consume command packet. */
  switch (pHdr->opCode)
  {
    /* --- extended device commands --- */

    case LHCI_OPCODE_VS_GET_ACL_TEST_REPORT:
      evtParamLen += sizeof(uint32_t) * 4;
      break;
    case LHCI_OPCODE_VS_ENA_ACL_SINK:
      lhciCb.recvAclSink = pBuf[0];
      break;
    case LHCI_OPCODE_VS_GENERATE_ACL:
    {
      uint16_t handle;
      uint16_t pktLen;
      uint8_t numPkts;

      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT16(pktLen, pBuf);
      numPkts = *pBuf++;

      lhciGenerateAcl(handle, pktLen, numPkts);
      break;
    }
    case LHCI_OPCODE_VS_ENA_AUTO_GEN_ACL:
      BSTREAM_TO_UINT16(lhciCb.genPktLen, pBuf);
      break;
    case LHCI_OPCODE_VS_SET_CONN_OP_FLAGS:
    {
      uint16_t handle;
      uint32_t flags;
      bool_t enable;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT32(flags, pBuf);
      BSTREAM_TO_UINT8 (enable, pBuf);

      status = LlSetConnOpFlags(handle, flags, enable);
      break;
    }
    case LHCI_OPCODE_VS_SET_CONN_TX_PWR:
    {
      uint16_t handle;
      int8_t level;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (level, pBuf);
      status = LlSetTxPowerLevel(handle, level);
      break;
    }

    case LHCI_OPCODE_VS_GET_CONN_STATS:
      evtParamLen += sizeof(BbBleDataPktStats_t);
      break;

    case LHCI_OPCODE_VS_SET_LOCAL_MIN_USED_CHAN:
    {
      uint8_t phys;
      int8_t  powerThreshold;
      uint8_t minUsedChan;

      BSTREAM_TO_UINT8(phys, pBuf);
      BSTREAM_TO_INT8(powerThreshold, pBuf);
      BSTREAM_TO_UINT8(minUsedChan, pBuf);

      status = LlSetLocalMinUsedChan(phys, powerThreshold, minUsedChan);
      break;
    }

    case LHCI_OPCODE_VS_GET_PEER_MIN_USED_CHAN:
    {
      uint16_t handle;

      BSTREAM_TO_UINT16(handle, pBuf);

      status = LlGetPeerMinUsedChan(handle, peerMinUsedChan);
      evtParamLen += sizeof(peerMinUsedChan);
      break;
    }

    /* --- default --- */

    default:
      return FALSE;       /* exit dispatcher routine */
  }

  uint8_t *pEvtBuf;

  /* Encode and send command complete event packet. */
  if ((pEvtBuf = lhciAllocCmdCmplEvt(evtParamLen, pHdr->opCode)) != NULL)
  {
    pBuf  = pEvtBuf;
    pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);

    switch (pHdr->opCode)
    {
      /* --- extended device commands --- */

      case LHCI_OPCODE_VS_GET_ACL_TEST_REPORT:
        UINT32_TO_BSTREAM(pBuf, lhciCb.recvAclPktCnt);
        UINT32_TO_BSTREAM(pBuf, lhciCb.recvAclOctetCnt);
        UINT32_TO_BSTREAM(pBuf, lhciCb.genPktCnt);
        UINT32_TO_BSTREAM(pBuf, lhciCb.genOctetCnt);
        break;

      case LHCI_OPCODE_VS_SET_CONN_OP_FLAGS:
        break;

      case LHCI_OPCODE_VS_SET_CONN_TX_PWR:
        /* no action */
        break;

      case LHCI_OPCODE_VS_GET_CONN_STATS:
      {
        BbBleDataPktStats_t stats;
        BbBleGetConnStats(&stats);
        memcpy(pBuf, (uint8_t *)&stats, sizeof(stats));
        break;
      }

      case LHCI_OPCODE_VS_SET_LOCAL_MIN_USED_CHAN:
        /* no action */
        break;

      case LHCI_OPCODE_VS_GET_PEER_MIN_USED_CHAN:
        memcpy(pBuf, peerMinUsedChan, sizeof(peerMinUsedChan));
        break;

      /* --- default --- */

      default:
        break;
    }

    lhciSendCmdCmplEvt(pEvtBuf);
  }

  return TRUE;

}
