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
 * \brief Link layer (LL) slave parameter interface implementation file.
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lmgr_api_adv_slave.h"
#include "lctr_api_adv_slave.h"
#include "bb_ble_api.h"
#include "pal_bb_ble.h"
#include "pal_radio.h"
#include "bb_ble_api_op.h"
#include "util/bstream.h"
#include "wsf_assert.h"
#include "wsf_cs.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "bb_ble_api_reslist.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Set advertising transmit power.
 *
 *  \param      advTxPwr        Advertising transmit power level.
 *
 *  \return     None.
 *
 *  Set the advertising transmit power.
 */
/*************************************************************************************************/
void LlSetAdvTxPower(int8_t advTxPwr)
{
  LL_TRACE_INFO1("### LlApi ###  LlSetAdvTxPower, advTxPwr=%u", advTxPwr);

  lmgrCb.advTxPwr = advTxPwr;
}

/*************************************************************************************************/
/*!
 *  \brief      Get advertising transmit power.
 *
 *  \param      pAdvTxPwr       Advertising transmit power level.
 *
 *  \return     Status error code.
 *
 *  Return the advertising transmit power.
 */
/*************************************************************************************************/
uint8_t LlGetAdvTxPower(int8_t *pAdvTxPwr)
{
  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  WSF_ASSERT(pAdvTxPwr);
  *pAdvTxPwr = PalRadioGetActualTxPower(lmgrCb.advTxPwr, FALSE);

  LL_TRACE_INFO1("### LlApi ###  LlGetAdvTxPower, advTxPwr=%d", *pAdvTxPwr);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set advertising parameter.
 *
 *  \param      advIntervalMin  Minimum advertising interval.
 *  \param      advIntervalMax  Maximum advertising interval.
 *  \param      advType         Advertising type.
 *  \param      ownAddrType     Address type used by this device.
 *  \param      peerAddrType    Address type of peer device.  Only used for directed advertising.
 *  \param      pPeerAddr       Address of peer device.  Only used for directed advertising.
 *  \param      advChanMap      Advertising channel map.
 *  \param      advFiltPolicy   Advertising filter policy.
 *
 *  \return     Status error code.
 *
 *  Set advertising parameters.
 *
 *  \note       This function must only be called when advertising is disabled.
 */
/*************************************************************************************************/
uint8_t LlSetAdvParam(uint16_t advIntervalMin, uint16_t advIntervalMax, uint8_t advType,
                      uint8_t ownAddrType, uint8_t peerAddrType, const uint8_t *pPeerAddr,
                      uint8_t advChanMap, uint8_t advFiltPolicy)
{
  const uint16_t rangeMin = 0x0020;               /*     20 ms */
  const uint16_t rangeMax = 0x4000;         /* 10,240 ms */
  const uint8_t advTypeMax = LL_ADV_CONN_DIRECT_LOW_DUTY;
  const uint8_t peerAddrTypeMax = LL_ADDR_RANDOM;
  const uint8_t advFiltPolicyMax = LL_ADV_FILTER_WL_ONLY;
  const uint8_t ownAddrTypeMax = ((lmgrCb.features & LL_FEAT_PRIVACY) != 0) ? LL_ADDR_RANDOM_IDENTITY : LL_ADDR_RANDOM;

  LL_TRACE_INFO1("### LlApi ###  LlSetAdvParam, advType=%u", advType);

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if (lmgrCb.advEnabled)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
       /* For high-duty cycle directed advertising, advertising intervals are ignored. */
      (((advType != LL_ADV_CONN_DIRECT_HIGH_DUTY) &&
        ((advIntervalMin < rangeMin) || (advIntervalMax < advIntervalMin) || (rangeMax < advIntervalMax))) ||
       (advType > advTypeMax) ||
       (ownAddrType > ownAddrTypeMax) ||
       /* If directed advertising or advertiser is private, peer address should be valid. */
       (((advType == LL_ADV_CONN_DIRECT_HIGH_DUTY) || (advType == LL_ADV_CONN_DIRECT_LOW_DUTY) || ((ownAddrType & LL_ADDR_IDENTITY_BIT) != 0)) &&
        (!pPeerAddr || (peerAddrType > peerAddrTypeMax))) ||
       (advChanMap & ~LL_ADV_CHAN_ALL) || (advChanMap == 0) ||
       /* If directed advertising, filter policy is ignored. */
       ((advType != LL_ADV_CONN_DIRECT_HIGH_DUTY) && (advType != LL_ADV_CONN_DIRECT_LOW_DUTY) &&
        (advFiltPolicy > advFiltPolicyMax))))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  lctrAdvParamMsg_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.dispId = LCTR_DISP_ADV;
    pMsg->hdr.event = LCTR_ADV_MSG_PARAM_UPD;

    pMsg->param.advInterMin    = BB_BLE_TO_BB_TICKS(advIntervalMin);
    pMsg->param.advInterMax    = BB_BLE_TO_BB_TICKS(advIntervalMax);
    pMsg->param.advType        = advType;
    pMsg->param.ownAddrType    = ownAddrType;
    pMsg->param.peerAddrType   = peerAddrType;
    pMsg->param.advChanMap     = advChanMap;
    pMsg->param.advFiltPolicy  = advFiltPolicy;
    BSTREAM_TO_BDA64(pMsg->param.peerAddr, pPeerAddr);

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set advertising data.
 *
 *  \param      len     Data buffer length.
 *  \param      pData   Advertising data buffer.
 *
 *  \return     Status error code.
 *
 *  Set advertising data data.
 */
/*************************************************************************************************/
uint8_t LlSetAdvData(uint8_t len, const uint8_t *pData)
{
  WSF_CS_INIT(cs);

  LL_TRACE_INFO1("### LlApi ###  LlSetAdvData, len=%u", len);

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if (len > sizeof(lmgrSlvAdvCb.advData.buf))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  WSF_CS_ENTER(cs);
  memcpy(lmgrSlvAdvCb.advData.buf, pData, len);
  lmgrSlvAdvCb.advData.len = len;
  lmgrSlvAdvCb.advData.modified = TRUE;
  WSF_CS_EXIT(cs);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set scan response data.
 *
 *  \param      len     Data buffer length.
 *  \param      pData   Scan response data buffer.
 *
 *  \return     Status error code.
 *
 *  Set scan response data.
 */
/*************************************************************************************************/
uint8_t LlSetScanRespData(uint8_t len, const uint8_t *pData)
{
  WSF_CS_INIT(cs);

  LL_TRACE_INFO1("### LlApi ###  LlSetScanRespData, len=%u", len);

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if (len > sizeof(lmgrSlvAdvCb.scanRspData.buf))
  {
    LL_TRACE_WARN2("Invalid scan response buffer size, len=%u, maxLen=%u", len, sizeof(lmgrSlvAdvCb.scanRspData.buf));
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  WSF_CS_ENTER(cs);
  memcpy(lmgrSlvAdvCb.scanRspData.buf, pData, len);
  lmgrSlvAdvCb.scanRspData.len = len;
  lmgrSlvAdvCb.scanRspData.modified = TRUE;
  WSF_CS_EXIT(cs);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Advertising enable.
 *
 *  \param      enable          Set to TRUE to enable advertising, FALSE to disable advertising.
 *
 *  \return     None.
 *
 *  Enable or disable advertising.
 */
/*************************************************************************************************/
void LlAdvEnable(uint8_t enable)
{
  lctrMsgHdr_t *pMsg;

  LL_TRACE_INFO1("### LlApi ###  LlAdvEnable: enable=%u", enable);

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    LmgrSendAdvEnableCnf(LL_ERROR_CODE_CMD_DISALLOWED);
    return;
  }

  /* Disallow connectable advertising when no connections remain. */
  switch (lmgrSlvAdvCb.advParam.advType)
  {
    case LL_ADV_CONN_UNDIRECT:
    case LL_ADV_CONN_DIRECT_HIGH_DUTY:
    case LL_ADV_CONN_DIRECT_LOW_DUTY:
      if (lmgrCb.numConnEnabled == pLctrRtCfg->maxConn)
      {
        LmgrSendAdvEnableCnf(LL_ERROR_CODE_CMD_DISALLOWED);
        return;
      }
      break;
    default:
      break;
  }
  if ((LL_API_PARAM_CHECK == 1) &&
      (enable == TRUE) &&
      !LmgrIsAddressTypeAvailable(lmgrSlvAdvCb.advParam.ownAddrType))
  {
    if (lmgrSlvAdvCb.advParam.advType == LL_ADDR_RANDOM_IDENTITY)
    {
      if (bbBleIsPeerInResList(lmgrSlvAdvCb.advParam.peerAddrType, lmgrSlvAdvCb.advParam.peerAddr))
      {
        if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
        {
          pMsg->dispId = LCTR_DISP_ADV;
          pMsg->event = LCTR_ADV_MSG_START;

          WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
          return;
        }
      }
    }
    LL_TRACE_WARN1("Address type invalid or not available, ownAddrType=%u", lmgrSlvAdvCb.advParam.ownAddrType);
    LmgrSendAdvEnableCnf(LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS);
    return;
  }

  if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->dispId = LCTR_DISP_ADV;
    pMsg->event = enable ? LCTR_ADV_MSG_START : LCTR_ADV_MSG_STOP;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }
}

 /*************************************************************************************************/
 /*!
  *  \brief      Scan report enable.
  *
  *  \param      enable          Set to TRUE to enable scan reports, FALSE to disable scan reports.
  *
  *  \return     None.
  *
  *  Enable or disable reports about the scanners from which an advertiser receives scan requests.
  */
 /*************************************************************************************************/
 void LlScanReportEnable(uint8_t enable)
 {
   lmgrSlvAdvCb.scanReportEna = enable;
 }
