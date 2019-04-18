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
 * \brief HCI command module implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "ll_defs.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Unpack a connection specification from a command packet.
 *
 *  \param  pConnSpec   Unpacked command structure.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Payload length.
 */
/*************************************************************************************************/
uint8_t lhciUnpackConnSpec(LlConnSpec_t *pConnSpec, const uint8_t *pBuf)
{
  const uint8_t len = 12;

  BSTREAM_TO_UINT16(pConnSpec->connIntervalMin, pBuf);
  BSTREAM_TO_UINT16(pConnSpec->connIntervalMax, pBuf);
  BSTREAM_TO_UINT16(pConnSpec->connLatency, pBuf);
  BSTREAM_TO_UINT16(pConnSpec->supTimeout, pBuf);
  BSTREAM_TO_UINT16(pConnSpec->minCeLen, pBuf);
  BSTREAM_TO_UINT16(pConnSpec->maxCeLen, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a read Tx power level or RSSI event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *  \param  level       Tx power level.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadPwrLevel(uint8_t *pBuf, uint8_t status, uint16_t handle, int8_t level)
{
  const uint8_t len = LHCI_LEN_READ_LOCAL_VER_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);
  UINT8_TO_BSTREAM (pBuf, level);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  pParam      Parameter buffer.
 *  \param  handle      Connection handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lhciConnSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t *pParam, uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *pEvtBuf;

  if ((pEvtBuf = lhciAllocCmdCmplEvt(paramLen, pCmdHdr->opCode)) == NULL)
  {
    return;
  }
  pBuf = pEvtBuf;

  switch (pCmdHdr->opCode)
  {
    /* --- command completion with status only parameter --- */

    case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
    case HCI_OPCODE_LE_SET_PER_ADV_RCV_ENABLE:
    case HCI_OPCODE_LE_SET_PAST_PARAM:
    case HCI_OPCODE_LE_SET_DEFAULT_PAST_PARAM:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;

    /* --- command completion with status and connHandle parameters --- */

    case HCI_OPCODE_LE_REM_CONN_PARAM_REP:
    case HCI_OPCODE_LE_REM_CONN_PARAM_NEG_REP:
    case HCI_OPCODE_LE_SET_DATA_LEN:
    case HCI_OPCODE_LE_PER_ADV_SYNC_TRANSFER:
    case HCI_OPCODE_LE_PER_ADV_SET_INFO_TRANSFER:
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT16_TO_BSTREAM(pBuf, handle);
      break;

    /* --- connection control --- */

    case HCI_OPCODE_LE_READ_DEF_DATA_LEN:
    {
      uint16_t initMaxTxLen;
      uint16_t initMaxTxTime;
      LlReadDefaultDataLen(&initMaxTxLen, &initMaxTxTime);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT16_TO_BSTREAM(pBuf, initMaxTxLen);
      UINT16_TO_BSTREAM(pBuf, initMaxTxTime);
      break;
    }

    case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
    {
      uint16_t maxTxLen;
      uint16_t maxTxTime;
      uint16_t maxRxLen;
      uint16_t maxRxTime;
      LlReadMaximumDataLen(&maxTxLen, &maxTxTime, &maxRxLen, &maxRxTime);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT16_TO_BSTREAM(pBuf, maxTxLen);
      UINT16_TO_BSTREAM(pBuf, maxTxTime);
      UINT16_TO_BSTREAM(pBuf, maxRxLen);
      UINT16_TO_BSTREAM(pBuf, maxRxTime);
      break;
    }

    /* --- status --- */

    case HCI_OPCODE_READ_TX_PWR_LVL:
    {
      int8_t level = 0;
      uint8_t type;
      BSTREAM_TO_UINT8(type, pParam);
      status = LlGetTxPowerLevel(handle, type, &level);
      lhciPackReadPwrLevel(pBuf, status, handle, level);
      break;
    }
    case HCI_OPCODE_READ_RSSI:
    {
      int8_t rssi;
      status = LlGetRssi(handle, &rssi);
      lhciPackReadPwrLevel(pBuf, status, handle, rssi);
      break;
    }

    /* --- remote device management --- */

    case HCI_OPCODE_LE_READ_CHAN_MAP:
      status = LlGetChannelMap(handle, pBuf + 3);         /* delay status and handle params; skip for now */
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status); /* now that status is known, write it */
      UINT16_TO_BSTREAM(pBuf, handle);                    /* and now write the handle */
      break;

    /* --- default --- */

    default:
      break;
  }

  lhciSendCmdCmplEvt(pEvtBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  Decode an HCI command packet.
 *
 *  \param  pHdr    Decoded packet header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing is organized by urgency then by frequency of command calls during normal
 *  operation. The following grouping is used:
 *      - connection control
 *      - connection status
 *      - local device management
 *      - remote device management
 *      - default
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciConnVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint8_t *pParam = NULL;
  uint16_t handle = 0;

  switch (pHdr->opCode)
  {
    /* --- connection control --- */

    case HCI_OPCODE_DISCONNECT:
    {
      uint8_t reason;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (reason, pBuf);
      status = LlDisconnect(handle, reason);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    case HCI_OPCODE_LE_CONN_UPDATE:
    {
      LlConnSpec_t connSpec;
      BSTREAM_TO_UINT16(handle, pBuf);
      lhciUnpackConnSpec(&connSpec, pBuf);
      status = LlConnUpdate(handle, &connSpec);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }
    case HCI_OPCODE_LE_REM_CONN_PARAM_REP:
    {
      LlConnSpec_t connSpec;
      BSTREAM_TO_UINT16(handle, pBuf);
      lhciUnpackConnSpec(&connSpec, pBuf);
      status = LlRemoteConnParamReqReply(handle, &connSpec);
      paramLen = LHCI_LEN_LE_REM_CONN_PARAM_REP;
      break;
    }
    case HCI_OPCODE_LE_REM_CONN_PARAM_NEG_REP:
    {
      uint8_t reason;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (reason, pBuf);
      status = LlRemoteConnParamReqNegReply(handle, reason);
      paramLen = LHCI_LEN_LE_REM_CONN_PARAM_NEG_REP;
      break;
    }

    case HCI_OPCODE_LE_SET_DATA_LEN:
    {
      uint16_t txLen;
      uint16_t txTime;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT16(txLen, pBuf);
      BSTREAM_TO_UINT16(txTime, pBuf);
      status = LlSetDataLen(handle, txLen, txTime);
      paramLen = LHCI_LEN_LE_SET_DATA_LEN;
      break;
    }

    /* --- connection status --- */

    case HCI_OPCODE_READ_TX_PWR_LVL:
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_READ_PWR_LVL_EVT;
      pParam = pBuf;
      break;

    case HCI_OPCODE_READ_RSSI:
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_READ_PWR_LVL_EVT;
      break;

    /* --- local device management --- */

    case HCI_OPCODE_LE_READ_DEF_DATA_LEN:
      paramLen = LHCI_LEN_LE_READ_DEF_DATA_LEN;
      break;
    case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
    {
      uint16_t txLen;
      uint16_t txTime;
      BSTREAM_TO_UINT16(txLen, pBuf);
      BSTREAM_TO_UINT16(txTime, pBuf);
      status = LlWriteDefaultDataLen(txLen, txTime);
      paramLen = LHCI_LEN_LE_WRITE_DEF_DATA_LEN;
      break;
    }
    case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
      paramLen = LHCI_LEN_LE_READ_MAX_DATA_LEN;
      break;

    /* --- remote device management --- */

    case HCI_OPCODE_LE_READ_REMOTE_FEAT:
      BSTREAM_TO_UINT16(handle, pBuf);
      status = LlReadRemoteFeat(handle);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    case HCI_OPCODE_LE_READ_CHAN_MAP:
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_LE_READ_CHAN_MAP_EVT;
      break;
    case HCI_OPCODE_READ_REMOTE_VER_INFO:
      BSTREAM_TO_UINT16(handle, pBuf);
      status = LlReadRemoteVerInfo(handle);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    case HCI_OPCODE_LE_REQUEST_PEER_SCA:
    {
      BSTREAM_TO_UINT16(handle, pBuf);
      status = LlRequestPeerSca(handle);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;

      break;
    }

    /* --- default --- */

    default:
      return FALSE;       /* exit dispatcher routine */
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }
  else
  {
    lhciConnSendCmdCmplEvt(pHdr, status, paramLen, pParam, handle);
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Generate ACL packets.
 *
 *  \param  handle      Connection handle.
 *  \param  pktLen      Packet length.
 *  \param  numPkts     Number of packets.
 *
 *  \return None.
 */
/*************************************************************************************************/
void lhciGenerateAcl(uint16_t handle, uint16_t pktLen, uint8_t numPkts)
{
  if (lhciCb.genEnaFlag)
  {
    /* Prevent re-entrance. */
    return;
  }

  lhciCb.genEnaFlag = TRUE;

  while (numPkts--)
  {
    /* Always use large data buffers. */
    unsigned int allocSize = HCI_ACL_HDR_LEN + BB_DATA_PLD_MAX_LEN + BB_DATA_PDU_TAILROOM;

    uint8_t *pAclBuf;
    if ((pAclBuf = (uint8_t*)WsfMsgAlloc(allocSize)) == NULL)
    {
      break;
    }

    uint8_t *pBuf = pAclBuf;
    UINT16_TO_BSTREAM(pBuf, handle & 0xFFF);   /* PB=0 and BC=0 */
    UINT16_TO_BSTREAM(pBuf, pktLen);
    memset(pBuf, lhciCb.genPldCnt++, pktLen);

    LlSendAclData(pAclBuf);
  }

  lhciCb.genEnaFlag = FALSE;
}
