/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  HCI command module implementation file.
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
 *  \brief  Pack a read enhanced Tx power level or RSSI event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  handle      Connection handle.
 *  \param  curPwr      Current Tx power level.
 *  \param  maxPwr      Maximum Tx power level.
 *  \param  phy         PHY read.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackEnhancedReadPwrLevel(uint8_t *pBuf, uint8_t status, uint16_t handle, int8_t curPwr, int8_t maxPwr, uint8_t phy)
{
  const uint8_t len = LHCI_LEN_LE_READ_ENH_TX_POWER_EVT;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT16_TO_BSTREAM(pBuf, handle);
  UINT8_TO_BSTREAM (pBuf, phy);
  UINT8_TO_BSTREAM (pBuf, curPwr);
  UINT8_TO_BSTREAM (pBuf, maxPwr);

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
 */
/*************************************************************************************************/
void lhciPclSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t *pParam, uint16_t handle)
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
    /* --- command completion with status and connHandle parameters --- */

    case HCI_OPCODE_LE_SET_TX_POWER_REPORT_ENABLE:
    case HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_PARAMS:
    case HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_ENABLE:
      pBuf += lhciPackCmdCompleteEvtStatus(pBuf, status);
      UINT16_TO_BSTREAM(pBuf, handle);
      break;

    /* --- status --- */
    case HCI_OPCODE_LE_READ_ENHANCED_TX_POWER:
    {
      int8_t curPwr = 0;
      int8_t maxPwr = 0;
      uint8_t phy;
      BSTREAM_TO_UINT8(phy, pParam);
      status = LlGetEnhTxPowerLevel(handle, phy, &curPwr, &maxPwr);
      lhciPackEnhancedReadPwrLevel(pBuf, status, handle, curPwr, maxPwr, phy);
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
 *      - connection control
 *      - connection status
 *      - local device management
 *      - remote device management
 *      - default
 *
 *  \return TRUE if opcode handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciPclDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
  uint8_t status = HCI_SUCCESS;
  uint8_t paramLen = 0;
  uint8_t *pParam = NULL;
  uint16_t handle = 0;

  switch (pHdr->opCode)
  {
    /* --- connection status --- */

    case HCI_OPCODE_LE_READ_ENHANCED_TX_POWER:
      BSTREAM_TO_UINT16(handle, pBuf);
      paramLen = LHCI_LEN_LE_READ_ENH_TX_POWER_EVT;
      pParam = pBuf;
      break;


    /* --- remote device management --- */
    case HCI_OPCODE_LE_READ_REMOTE_TX_POWER:
    {
      uint8_t phy;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (phy, pBuf);
      status = LlPowerCtrlReq(handle, 0, phy);
      paramLen = LHCI_LEN_CMD_STATUS_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_TX_POWER_REPORT_ENABLE:
    {
      uint8_t enableLocal;
      uint8_t enableRemote;
      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_UINT8 (enableLocal, pBuf);
      BSTREAM_TO_UINT8 (enableRemote, pBuf);
      status = LlSetTxPowerReporting(handle, enableLocal, enableRemote);
      paramLen = LHCI_LEN_LE_SET_TX_POWER_REPORT_EVT;
      break;
    }
    case HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_PARAMS:
    {
      int8_t highThreshold;
      int8_t highHysteresis;
      int8_t lowThreshold;
      int8_t lowHysteresis;
      uint16_t minTimeSpent;

      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_INT8  (highThreshold, pBuf);
      BSTREAM_TO_INT8  (highHysteresis, pBuf);
      BSTREAM_TO_INT8  (lowThreshold, pBuf);
      BSTREAM_TO_INT8  (lowHysteresis, pBuf);
      BSTREAM_TO_UINT16(minTimeSpent, pBuf);
      status = LlSetPathLossReportingParams(handle, highThreshold, highHysteresis, lowThreshold, lowHysteresis, minTimeSpent);
      paramLen = LHCI_LEN_LE_SET_PATH_LOSS_REPORTING_PARAMS;
      break;
    }
    case HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_ENABLE:
    {
      uint8_t enable;

      BSTREAM_TO_UINT16(handle, pBuf);
      BSTREAM_TO_INT8  (enable, pBuf);
      status = LlSetPathLossReportingEnable(handle, enable);
      paramLen = LHCI_LEN_LE_SET_PATH_LOSS_REPORTING_ENABLE;
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
    lhciPclSendCmdCmplEvt(pHdr, status, paramLen, pParam, handle);
  }

  return TRUE;
}
