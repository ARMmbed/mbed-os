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
#include "wsf_math.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *  \param  handle      Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciSlvExtAdvSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t handle)
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
    /* --- LE Extended Advertising --- */

    case HCI_OPCODE_LE_SET_EXT_ADV_PARAM:
    {
      int8_t selTxPwr;
      LlGetExtAdvTxPowerLevel(handle, &selTxPwr);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT8_TO_BSTREAM(pBuf, selTxPwr);
      break;
    }
    case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
    {
      uint16_t maxLen = 31;
      status = LlReadMaxAdvDataLen(&maxLen);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT16_TO_BSTREAM(pBuf, maxLen);
      break;
    }
    case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
    {
      uint8_t numSets = 1;
      status = LlReadNumSupAdvSets(&numSets);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT8_TO_BSTREAM(pBuf, numSets);
      break;
    }

    /* --- command completion with status only parameter --- */

    case HCI_OPCODE_LE_SET_ADV_SET_RAND_ADDR:
    case HCI_OPCODE_LE_SET_EXT_ADV_DATA:
    case HCI_OPCODE_LE_SET_EXT_SCAN_RESP_DATA:
    case HCI_OPCODE_LE_SET_EXT_ADV_ENABLE:
    case HCI_OPCODE_LE_REMOVE_ADV_SET:
    case HCI_OPCODE_LE_CLEAR_ADV_SETS:
    case HCI_OPCODE_LE_SET_PER_ADV_PARAM:
    case HCI_OPCODE_LE_SET_PER_ADV_DATA:
    case HCI_OPCODE_LE_SET_PER_ADV_ENABLE:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
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
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvExtAdvDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciSlvExtAdvVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint8_t handle = 0xFF;

  switch (pHdr->opCode)
  {
    /* --- LE Extended Advertising --- */

    case HCI_OPCODE_LE_SET_ADV_SET_RAND_ADDR:
    {
      handle = pBuf[0];
      uint8_t *pAddr = &pBuf[1];
      status = LlSetAdvSetRandAddr(handle, pAddr);
      paramLen = LHCI_LEN_LE_SET_EXT_SCAN_PARAM;
      break;
    }
    case HCI_OPCODE_LE_SET_EXT_ADV_PARAM:
    {
      LlExtAdvParam_t extAdvParam;
      BSTREAM_TO_UINT8 (handle, pBuf);
      BSTREAM_TO_UINT16(extAdvParam.advEventProp, pBuf);
      BSTREAM_TO_UINT24(extAdvParam.priAdvInterMin, pBuf);
      BSTREAM_TO_UINT24(extAdvParam.priAdvInterMax, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.priAdvChanMap, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.ownAddrType, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.peerAddrType, pBuf);
      extAdvParam.pPeerAddr = pBuf;
      pBuf += BDA_ADDR_LEN;
      BSTREAM_TO_UINT8 (extAdvParam.advFiltPolicy, pBuf);
      BSTREAM_TO_INT8  (extAdvParam.advTxPwr, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.priAdvPhy, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.secAdvMaxSkip, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.secAdvPhy, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.advSID, pBuf);
      BSTREAM_TO_UINT8 (extAdvParam.scanReqNotifEna, pBuf);
      status = LlSetExtAdvParam(handle, &extAdvParam);
      /* pBuf += */ lhciPackCmdCompleteEvtStatus(pBuf, status);
      paramLen = LHCI_LEN_LE_SET_EXT_ADV_PARAM;
      break;
    }
    case HCI_OPCODE_LE_SET_EXT_ADV_DATA:
    {
      status = LlSetExtAdvData(pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf + 4);
      paramLen = LHCI_LEN_LE_SET_EXT_ADV_DATA;
      break;
    }
    case HCI_OPCODE_LE_SET_EXT_SCAN_RESP_DATA:
    {
      status = LlSetExtScanRespData(pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf + 4);
      paramLen = LHCI_LEN_LE_SET_EXT_SCAN_RESP_DATA;
      break;
    }
    case HCI_OPCODE_LE_SET_EXT_ADV_ENABLE:
    {
      uint8_t enable;
      uint8_t numOfSets;
      BSTREAM_TO_UINT8(enable, pBuf);
      BSTREAM_TO_UINT8(numOfSets, pBuf);
      LlExtAdvEnableParam_t enaParam[LL_MAX_ADV_SETS];
      for (unsigned int i = 0; i < WSF_MIN(numOfSets, LL_MAX_ADV_SETS); i++)
      {
        BSTREAM_TO_UINT8 (enaParam[i].handle, pBuf);
        BSTREAM_TO_UINT16(enaParam[i].duration, pBuf);
        BSTREAM_TO_UINT8 (enaParam[i].numEvents, pBuf);
      }
      LlExtAdvEnable(enable, numOfSets, enaParam);
      /* Send command complete once LL finishes processing the command. */
      return TRUE;
    }
    case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
    {
      paramLen = LHCI_LEN_LE_READ_MAX_ADV_DATA_LEN;
      break;
    }
    case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
    {
      paramLen = LHCI_LEN_LE_READ_NUM_OF_SUP_ADV_SETS;
      break;
    }
    case HCI_OPCODE_LE_REMOVE_ADV_SET:
    {
      status = LlRemoveAdvSet(pBuf[0]);
      paramLen = LHCI_LEN_LE_REMOVE_ADV_SET;
      break;
    }
    case HCI_OPCODE_LE_CLEAR_ADV_SETS:
    {
      status = LlClearAdvSets();
      paramLen = LHCI_LEN_LE_CLEAR_ADV_SETS;
      break;
    }

    /* --- LE Periodic Advertising --- */

    case HCI_OPCODE_LE_SET_PER_ADV_PARAM:
    {
      LlPerAdvParam_t perAdvParam;
      BSTREAM_TO_UINT8 (handle, pBuf);
      BSTREAM_TO_UINT16(perAdvParam.perAdvInterMin, pBuf);
      BSTREAM_TO_UINT16(perAdvParam.perAdvInterMax, pBuf);
      BSTREAM_TO_UINT16(perAdvParam.perAdvProp, pBuf);
      status = LlSetPeriodicAdvParam(handle, &perAdvParam);
      paramLen = LHCI_LEN_LE_SET_PER_ADV_PARAM;
      break;
    }
    case HCI_OPCODE_LE_SET_PER_ADV_DATA:
    {
      status = LlSetPeriodicAdvData(pBuf[0], pBuf[1], pBuf[2], pBuf + 3);
      paramLen = LHCI_LEN_LE_SET_PER_ADV_DATA;
      break;
    }
    case HCI_OPCODE_LE_SET_PER_ADV_ENABLE:
    {
      uint8_t enable;
      BSTREAM_TO_UINT8(enable, pBuf);
      BSTREAM_TO_UINT8(handle, pBuf);
      LlSetPeriodicAdvEnable(handle, enable);
      /* Send command complete once LL finishes processing the command. */
      return TRUE;
    }

    /* --- default --- */

    default:
      return FALSE;
  }

  if (paramLen > 0)
  {
    lhciSlvExtAdvSendCmdCmplEvt(pHdr, status, paramLen, handle);
  }

  return TRUE;
}
