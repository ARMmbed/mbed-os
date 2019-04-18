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
 * \brief Link layer (LL) master parameter interface implementation file.
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lctr_api_adv_master.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "bb_ble_api.h"

/*************************************************************************************************/
/*!
 *  \brief      Set scan channel map.
 *
 *  \param      chanMap         Scan channel map.
 *
 *  \return     Status error code.
 *
 *  Set scan channel map.
 *
 *  \note       This function must only be called when scanning is disabled.
 */
/*************************************************************************************************/
uint8_t LlSetSetScanChanMap(uint8_t chanMap)
{
  LL_TRACE_INFO1("### LlApi ###  LlSetSetScanChanMap, chanMap=0x%02x", chanMap);

  if (lmgrCb.numScanEnabled || lmgrCb.numInitEnabled)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      ((chanMap & ~LL_ADV_CHAN_ALL) || (chanMap == 0)))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  lmgrMstScanCb.scanChanMap = chanMap;

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set scan parameters.
 *
 *  \param      pParam          Scan parameters.
 *
 *  \return     Status error code.
 *
 *  Set scan parameters.
 *
 *  \note       This function must only be called when scanning is disabled.
 */
/*************************************************************************************************/
uint8_t LlSetScanParam(const LlScanParam_t *pParam)
{
  const uint16_t rangeMin = 0x0004;         /*      2.5 ms */
  const uint16_t rangeMax = 0x4000;         /* 10,240.0 ms */
  const uint8_t scanTypeMax = LL_SCAN_ACTIVE;
  const uint8_t scanFiltPolicyMax = ((lmgrCb.features & LL_FEAT_EXT_SCAN_FILT_POLICY) != 0) ? LL_SCAN_FILTER_WL_OR_RES_INIT : LL_SCAN_FILTER_WL_BIT;
  const uint8_t ownAddrTypeMax = ((lmgrCb.features & LL_FEAT_PRIVACY) != 0) ? LL_ADDR_RANDOM_IDENTITY : LL_ADDR_RANDOM;

  LL_TRACE_INFO1("### LlApi ###  LlSetScanParam, scanType=%u", pParam->scanType);

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if (lmgrCb.numScanEnabled || lmgrCb.numInitEnabled)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
     ((rangeMax < pParam->scanInterval) || (pParam->scanInterval < pParam->scanWindow) || (pParam->scanWindow < rangeMin) ||
      (pParam->scanType > scanTypeMax) ||
      (pParam->ownAddrType > ownAddrTypeMax) ||
      (pParam->scanFiltPolicy > scanFiltPolicyMax)))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  lctrScanParamMsg_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.dispId = LCTR_DISP_SCAN;
    pMsg->hdr.event = LCTR_SCAN_MSG_PARAM_UPD;

    pMsg->param.scanInterval   = pParam->scanInterval;
    pMsg->param.scanWindow     = pParam->scanWindow;
    pMsg->param.scanType       = pParam->scanType;
    pMsg->param.ownAddrType    = pParam->ownAddrType;
    pMsg->param.scanFiltPolicy = pParam->scanFiltPolicy;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Scan enable.
 *
 *  \param      enable          Set to TRUE to enable scanning, FALSE to disable scanning.
 *  \param      filterDup       Set to TRUE to filter duplicates.
 *
 *  \return     None.
 *
 *  Enable or disable scanning.  This function is only used when operating in master role.
 */
/*************************************************************************************************/
void LlScanEnable(uint8_t enable, uint8_t filterDup)
{
  lctrScanEnableMsg_t *pMsg;

  LL_TRACE_INFO2("### LlApi ###  LlScanEnable: enable=%u, filterDup=%u", enable, filterDup);

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    LmgrSendScanEnableCnf(LL_ERROR_CODE_CMD_DISALLOWED);
    return;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      (enable == TRUE) &&
      !LmgrIsAddressTypeAvailable(lmgrMstScanCb.scanParam.ownAddrType))
  {
    LL_TRACE_WARN1("Address type invalid or not available, ownAddrType=%u", lmgrMstScanCb.scanParam.ownAddrType);
    LmgrSendScanEnableCnf(LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS);
    return;
  }

  if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.dispId = LCTR_DISP_SCAN;
    pMsg->hdr.event = enable ? LCTR_SCAN_MSG_DISCOVER_ENABLE : LCTR_SCAN_MSG_DISCOVER_DISABLE;

    pMsg->filtDup = filterDup;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }
}
