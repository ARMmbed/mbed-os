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
#include "wsf_buf.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Decode slave extended advertising vendor specific HCI command packet.
 *
 *  \param  pHdr    Unpacked command header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing for vendor specific commands.
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvExtAdvVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t evtParamLen = 1;      /* default is status field only */

  /* Variables used for VS_GET_PER_CHAN_MAP. */
  uint16_t advHandle = 0;
  bool_t   isAdv = FALSE;

  /* Decode and consume command packet. */
  switch (pHdr->opCode)
  {
    /* --- extended device commands --- */

    case LHCI_OPCODE_VS_SET_AUX_DELAY:
    {
      uint8_t handle;
      uint32_t delayUsec;
      handle = *pBuf++;
      BSTREAM_TO_UINT32(delayUsec, pBuf);
      status = LlSetAuxOffsetDelay(handle, delayUsec);
      break;
    }
    case LHCI_OPCODE_VS_SET_EXT_ADV_FRAG_LEN:
      status = LlSetExtAdvDataFragLen(pBuf[0], pBuf[1]);
      break;
    case LHCI_OPCODE_VS_SET_EXT_ADV_PHY_OPTS:
      status = LlSetExtAdvTxPhyOptions(pBuf[0], pBuf[1], pBuf[2]);
      break;
    case LHCI_OPCODE_VS_GET_AUX_ADV_STATS:
      evtParamLen += sizeof(BbBleAuxAdvPktStats_t);
      break;
    case LHCI_OPCODE_VS_SET_EXT_ADV_DEF_PHY_OPTS:
    {
      uint8_t phyOptions;
      BSTREAM_TO_UINT8(phyOptions, pBuf);
      LlSetDefaultExtAdvTxPhyOptions(phyOptions);
      break;
    }
    case LHCI_OPCODE_VS_GET_PER_CHAN_MAP:
    {
      BSTREAM_TO_UINT16(advHandle, pBuf);
      BSTREAM_TO_UINT8(isAdv, pBuf);
      evtParamLen += HCI_CHAN_MAP_LEN;
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

      case LHCI_OPCODE_VS_GET_AUX_ADV_STATS:
      {
        BbBleAuxAdvPktStats_t stats;
        BbBleGetAuxAdvStats(&stats);
        memcpy(pBuf, (uint8_t *)&stats, sizeof(stats));
        break;
      }

      case LHCI_OPCODE_VS_GET_PER_CHAN_MAP:
      {
       /* Note: this function is also used by master. */
        uint64_t chM;
        chM = LlGetPeriodicChanMap(advHandle, isAdv);
        UINT40_TO_BSTREAM(pBuf, chM);
        break;
      }

      /* --- default --- */

      default:
        break;
    }

    lhciSendCmdCmplEvt(pEvtBuf);
  }

  return TRUE;
}
