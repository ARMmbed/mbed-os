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
#include "util/bstream.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief   Maximum number of scanning PHYs. */
#define LHCI_MAX_SCAN_PHY   3

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Set advertising parameter command. */
typedef struct
{
  uint8_t ownAddrType;      /*!< Own address type. */
  uint8_t scanFiltPolicy;   /*!< Scan filter policy. */
  uint8_t scanPhys;         /*!< Scanning PHYs. */
  LlExtScanParam_t param[LHCI_MAX_SCAN_PHY];    /*!< Extended scan parameters for each PHY. */
} lhciSetExtScanParamCmd_t;

/*************************************************************************************************/
/*!
 *  \brief  Unpack a set scan parameter command packet.
 *
 *  \param  pCmd        Unpacked command structure.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void lhciUnpackSetExtScanParamCmd(lhciSetExtScanParamCmd_t *pCmd, const uint8_t *pBuf)
{
  const unsigned int maxBits = 8;

  BSTREAM_TO_UINT8(pCmd->ownAddrType, pBuf);
  BSTREAM_TO_UINT8(pCmd->scanFiltPolicy, pBuf);
  BSTREAM_TO_UINT8(pCmd->scanPhys, pBuf);

  unsigned int i;   /* scanPhy bit position */
  unsigned int j;   /* param array index */

  for (i = 0, j = 0; (i < maxBits) && (j < LHCI_MAX_SCAN_PHY); i++)
  {
    if (pCmd->scanPhys & (1 << i))
    {
      BSTREAM_TO_UINT8 (pCmd->param[j].scanType, pBuf);
      BSTREAM_TO_UINT16(pCmd->param[j].scanInterval, pBuf);
      BSTREAM_TO_UINT16(pCmd->param[j].scanWindow, pBuf);
      j++;
    }
  }
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
static void lhciMstAeSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen)
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

    case HCI_OPCODE_LE_SET_EXT_SCAN_PARAM:
    case HCI_OPCODE_LE_SET_EXT_SCAN_ENABLE:
    case HCI_OPCODE_LE_PER_ADV_CREATE_SYNC_CANCEL:
    case HCI_OPCODE_LE_PER_ADV_TERMINATE_SYNC:
    case HCI_OPCODE_LE_ADD_DEV_PER_ADV_LIST:
    case HCI_OPCODE_LE_REMOVE_DEV_PER_ADV_LIST:
    case HCI_OPCODE_LE_CLEAR_PER_ADV_LIST:
      lhciPackCmdCompleteEvtStatus(pBuf, status);
      break;
    case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
    {
      uint8_t listSize = 0;
      status = LlReadPeriodicAdvListSize(&listSize);
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT8_TO_BSTREAM(pBuf, listSize);
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
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciMstExtScanDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
#if LHCI_ENABLE_VS
  if (lhciMstExtScanVsStdDecodeCmdPkt(pHdr, pBuf))
  {
    return TRUE;
  }
#endif

  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;

  switch (pHdr->opCode)
  {
    /* --- LE Advertising Extensions --- */

    case HCI_OPCODE_LE_SET_EXT_SCAN_PARAM:
    {
      lhciSetExtScanParamCmd_t cmd;
      lhciUnpackSetExtScanParamCmd(&cmd, pBuf);
      status = LlSetExtScanParam(cmd.ownAddrType, cmd.scanFiltPolicy, cmd.scanPhys, cmd.param);
      paramLen = LHCI_LEN_LE_SET_EXT_SCAN_PARAM;
      break;
    }
    case HCI_OPCODE_LE_SET_EXT_SCAN_ENABLE:
    {
      uint8_t enable;
      uint8_t filterDup;
      uint16_t duration;
      uint16_t period;

      enable = *pBuf++;
      filterDup = *pBuf++;
      BSTREAM_TO_UINT16(duration, pBuf);
      BSTREAM_TO_UINT16(period, pBuf);

      LlExtScanEnable(enable, filterDup, duration, period);
      return TRUE;      /* LL event handler sends command completion */
    }
    case HCI_OPCODE_LE_PER_ADV_CREATE_SYNC:
    {
      LlPerAdvCreateSyncCmd_t cmd;
      BSTREAM_TO_UINT8(cmd.options, pBuf);
      BSTREAM_TO_UINT8(cmd.advSID, pBuf);
      BSTREAM_TO_UINT8(cmd.advAddrType, pBuf);
      cmd.pAdvAddr = pBuf;
      pBuf += BDA_ADDR_LEN;
      BSTREAM_TO_UINT16(cmd.skip, pBuf);
      BSTREAM_TO_UINT16(cmd.syncTimeOut, pBuf);
      pBuf++; /* Sync_CTE_Type (Unused) */
      status = LlPeriodicAdvCreateSync(&cmd);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }
    case HCI_OPCODE_LE_PER_ADV_CREATE_SYNC_CANCEL:
    {
      status = LlPeriodicAdvCreateSyncCancel();
      paramLen = LHCI_LEN_LE_PER_ADV_CREATE_SYNC_CANCEL;
      break;
    }
    case HCI_OPCODE_LE_PER_ADV_TERMINATE_SYNC:
    {
      uint16_t syncHandle;
      BSTREAM_TO_UINT16(syncHandle, pBuf);
      status = LlPeriodicAdvTerminateSync(syncHandle);
      paramLen = LHCI_LEN_LE_PER_ADV_TERMINATE_SYNC;
      break;
    }
    case HCI_OPCODE_LE_ADD_DEV_PER_ADV_LIST:
    {
      LlDevicePerAdvList_t cmd;
      BSTREAM_TO_UINT8(cmd.advAddrType, pBuf);
      cmd.pAdvAddr = pBuf;
      pBuf += BDA_ADDR_LEN;
      BSTREAM_TO_UINT8(cmd.advSID, pBuf);
      status = LlAddDeviceToPeriodicAdvList(&cmd);
      paramLen = LHCI_LEN_LE_ADD_DEV_PER_ADV_LIST;
      break;
    }
    case HCI_OPCODE_LE_REMOVE_DEV_PER_ADV_LIST:
    {
      LlDevicePerAdvList_t cmd;
      BSTREAM_TO_UINT8(cmd.advAddrType, pBuf);
      cmd.pAdvAddr = pBuf;
      pBuf += BDA_ADDR_LEN;
      BSTREAM_TO_UINT8(cmd.advSID, pBuf);
      status = LlRemoveDeviceFromPeriodicAdvList(&cmd);
      paramLen = LHCI_LEN_LE_REM_DEV_PER_ADV_LIST;
      break;
    }
    case HCI_OPCODE_LE_CLEAR_PER_ADV_LIST:
    {
      status = LlClearPeriodicAdvList();
      paramLen = LHCI_LEN_LE_CLEAR_PER_ADV_LIST;
      break;
    }
    case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
    {
      paramLen = LHCI_LEN_LE_READ_PER_ADV_LIST_SIZE;
      break;
    }
    /* --- default --- */

    default:
      return FALSE;
  }

  if (paramLen == LHCI_LEN_CMD_STATUS_EVT)
  {
    lhciSendCmdStatusEvt(pHdr, status);
  }
  else if (paramLen > 0)
  {
    lhciMstAeSendCmdCmplEvt(pHdr, status, paramLen);
  }

  return TRUE;
}
