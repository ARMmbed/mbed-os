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

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Set advertising parameter command. */
typedef struct
{
  uint16_t      advIntervalMin;         /*!< Minimum advertising interval. */
  uint16_t      advIntervalMax;         /*!< Maximum advertising interval. */
  uint8_t       advType;                /*!< Advertising type. */
  uint8_t       ownAddrType;            /*!< Address type used by this device. */
  uint8_t       directAddrType;         /*!< Address type of peer device.  Only used for directed advertising. */
  bdAddr_t      directAddr;             /*!< Address of peer device.  Only used for directed advertising. */
  uint8_t       advChanMap;             /*!< Advertising channel map. */
  uint8_t       advFiltPolicy;          /*!< Advertising filter policy. */
} lhciSetAdvParamCmd_t;

/*************************************************************************************************/
/*!
 *  \brief  Unpack a set advertising parameter command packet.
 *
 *  \param  pCmd        Unpacked command structure.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Payload length.
 */
/*************************************************************************************************/
static uint8_t lhciUnpackSetAdvParamCmd(lhciSetAdvParamCmd_t *pCmd, const uint8_t *pBuf)
{
  const uint8_t len = 15;

  BSTREAM_TO_UINT16(pCmd->advIntervalMin, pBuf);
  BSTREAM_TO_UINT16(pCmd->advIntervalMax, pBuf);
  BSTREAM_TO_UINT8 (pCmd->advType, pBuf);
  BSTREAM_TO_UINT8 (pCmd->ownAddrType, pBuf);
  BSTREAM_TO_UINT8 (pCmd->directAddrType, pBuf);
  memcpy(pCmd->directAddr, pBuf, sizeof(pCmd->directAddr));
  pBuf += sizeof(pCmd->directAddr);
  BSTREAM_TO_UINT8 (pCmd->advChanMap, pBuf);
  BSTREAM_TO_UINT8 (pCmd->advFiltPolicy, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Build and send a command complete event packet.
 *
 *  \param  pCmdHdr     Command HCI header.
 *  \param  status      Status value.
 *  \param  paramLen    Parameter length of the command status event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciSlvAdvSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen)
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

    case HCI_OPCODE_LE_SET_ADV_PARAM:
    case HCI_OPCODE_LE_SET_ADV_DATA:
    case HCI_OPCODE_LE_SET_SCAN_RESP_DATA:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;

    /* --- advertising control --- */

    case HCI_OPCODE_LE_READ_ADV_TX_POWER:
    {
      int8_t advTxPwr = 0;
      status = LlGetAdvTxPower(&advTxPwr);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT8_TO_BSTREAM(pBuf, advTxPwr);
      break;
    }

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
 *      - status
 *      - advertising control
 *      - default/unused
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciSlvAdvDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciSlvAdvVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- status --- */

    case HCI_OPCODE_LE_READ_ADV_TX_POWER:
      paramLen = LHCI_LEN_LE_READ_ADV_TX_POWER_EVT;
      break;

    /* --- advertising control --- */

    case HCI_OPCODE_LE_SET_ADV_DATA:
      /* coverity[check_return] */
      status = LlSetAdvData(pBuf[0], pBuf + 1);
      paramLen = LHCI_LEN_LE_SET_ADV_DATA_EVT;
      break;
    case HCI_OPCODE_LE_SET_SCAN_RESP_DATA:
      /* coverity[check_return] */
      status = LlSetScanRespData(pBuf[0], pBuf + 1);
      paramLen = LHCI_LEN_LE_SET_SCAN_RESP_DATA_EVT;
      break;
    case HCI_OPCODE_LE_SET_ADV_PARAM:
    {
      lhciSetAdvParamCmd_t cmd;
      lhciUnpackSetAdvParamCmd(&cmd, pBuf);
      /* coverity[check_return] */
      status = LlSetAdvParam(cmd.advIntervalMin, cmd.advIntervalMax, cmd.advType,
                             cmd.ownAddrType, cmd.directAddrType, cmd.directAddr,
                             cmd.advChanMap, cmd.advFiltPolicy);
      paramLen = LHCI_LEN_LE_SET_ADV_PARAM_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_ADV_ENABLE:
      LlAdvEnable(pBuf[0]);
      return TRUE;       /* LL event handler sends command completion */

    /* --- default --- */

    default:
      return FALSE;       /* exit dispatcher routine */
  }

  lhciSlvAdvSendCmdCmplEvt(pHdr, status, paramLen);

  return TRUE;
}
