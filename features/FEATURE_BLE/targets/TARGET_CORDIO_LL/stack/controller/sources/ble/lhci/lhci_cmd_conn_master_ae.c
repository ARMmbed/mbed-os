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
#include "util/bstream.h"
#include <string.h>

/*! \brief   Maximum number initiating of PHY. */
#define LHCI_MAX_INIT_PHY   2

/*************************************************************************************************/
/*!
 *  \brief  Unpack extended initiate parameter packet.
 *
 *  \param  pInitParam      Unpacked extended initiate parameter.
 *  \param  initScanParam   Unpacked extended initiate scan parameter table indexed by PHY.
 *  \param  connSpec        Unpacked connection specification table indexed by PHY.
 *  \param  pBuf            Packed packet buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
static uint8_t lhciUnpackExtInitParam(LlExtInitParam_t *pInitParam, LlExtInitScanParam_t initScanParam[], LlConnSpec_t connSpec[], const uint8_t *pBuf)
{
  const unsigned int maxBits = 8;

  const uint8_t *pBufStart = pBuf;

  BSTREAM_TO_UINT8(pInitParam->filterPolicy, pBuf);
  BSTREAM_TO_UINT8(pInitParam->ownAddrType, pBuf);
  BSTREAM_TO_UINT8(pInitParam->peerAddrType, pBuf);
  pInitParam->pPeerAddr = pBuf;
  pBuf += sizeof(bdAddr_t);
  BSTREAM_TO_UINT8(pInitParam->initPhys, pBuf);

  unsigned int i;   /* scanPhy bit position */
  unsigned int j;   /* paramPhys array index */

  for (i = 0, j = 0; (i < maxBits) && (j < LHCI_MAX_INIT_PHY); i++)
  {
    if (pInitParam->initPhys & (1 << i))
    {
      BSTREAM_TO_UINT16(initScanParam[j].scanInterval, pBuf);
      BSTREAM_TO_UINT16(initScanParam[j].scanWindow, pBuf);
      pBuf += lhciUnpackConnSpec(&connSpec[j], pBuf);
      j++;
    }
  }

  return pBuf - pBufStart;
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
 *      - default
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstExtConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciMstConnVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- connection control --- */

    case HCI_OPCODE_LE_EXT_CREATE_CONN:
    {
      LlExtInitParam_t initParam;
      LlExtInitScanParam_t initScanParam[LHCI_MAX_INIT_PHY];
      LlConnSpec_t connSpec[LHCI_MAX_INIT_PHY] = { {0} };

      lhciUnpackExtInitParam(&initParam, initScanParam, connSpec, pBuf);
      status = LlExtCreateConn(&initParam, initScanParam, connSpec);

      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }

    /* --- default --- */

    default:
      /* Fall back to legacy master connection handler. */
      return lhciMstConnDecodeCmdPkt(pHdr, pBuf);    /* exit dispatcher routine */
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }

  return TRUE;
}
