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
#include "ll_math.h"
#include "lctr_api_adv_master.h"
#include "lctr_api_init_master.h"
#include "lctr_api_conn.h"
#include "lmgr_api_adv_master.h"
#include "bb_ble_api.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Set connection's channel map.
 *
 *  \param      handle          Connection handle.
 *  \param      pChanMap        Channel map.
 *
 *  \return     Status error code.
 *
 *  Set the channel map of a connection.
 */
/*************************************************************************************************/
uint8_t LlSetChannelMap(uint16_t handle, const uint8_t *pChanMap)
{
  lctrChanMapUpdate_t *pMsg;
  uint64_t chanMap;

  LL_TRACE_INFO1("### LlApi ###  LlSetChannelMap, handle=%u", handle);

  if (LctrIsProcActPended(handle, LCTR_CONN_MSG_API_CHAN_MAP_UPDATE) == TRUE)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      (LctrGetRole(handle) != LL_ROLE_MASTER))
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  BSTREAM_TO_UINT64(chanMap, pChanMap);
  chanMap &= lmgrCb.chanClass;

  if ((LL_API_PARAM_CHECK == 1) &&
      (LlMathGetNumBitsSet(chanMap) < 2))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  if ((pMsg = (lctrChanMapUpdate_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.handle = handle;
    pMsg->hdr.dispId = LCTR_DISP_CONN;
    pMsg->hdr.event  = LCTR_CONN_MSG_API_CHAN_MAP_UPDATE;

    pMsg->chanMap = chanMap;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Create connection.
 *
 *  \param      pInitParam      Initiating parameters.
 *  \param      pConnSpec       Connection specification.
 *
 *  \return     Status error code.
 *
 *  Create a connection to the specified peer address with the specified connection parameters.
 *  This function is only when operating in master role.
 */
/*************************************************************************************************/
uint8_t LlCreateConn(const LlInitParam_t *pInitParam, const LlConnSpec_t *pConnSpec)
{
  lctrInitiateMsg_t *pMsg;
  const uint16_t scanRangeMin = 0x0004;
  const uint16_t scanRangeMax = 0x4000;
  const uint8_t filterPolicyMax = LL_SCAN_FILTER_WL_BIT;
  const uint8_t addrTypeMax = ((lmgrCb.features & LL_FEAT_PRIVACY) != 0) ? LL_ADDR_RANDOM_IDENTITY : LL_ADDR_RANDOM;

  LL_TRACE_INFO2("### LlApi ###  LlCreateConn: scanInterval=%u, scanWindow=%u", pInitParam->scanInterval, pInitParam->scanWindow);
  LL_TRACE_INFO2("                             connIntervalMin=%u, connIntervalMax=%u", pConnSpec->connIntervalMin, pConnSpec->connIntervalMax);
  LL_TRACE_INFO2("                             connLatency=%u, supTimeout=%u", pConnSpec->connLatency, pConnSpec->supTimeout);

  WSF_ASSERT(pInitParam->pPeerAddr);    /* not NULL */
  WSF_ASSERT(pConnSpec);    /* not NULL */

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsLegacyCommandAllowed())
  {
    LL_TRACE_WARN0("Extended Advertising/Scanning operation enabled; legacy commands not available");
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((pInitParam->scanInterval > scanRangeMax) || (pInitParam->scanWindow > pInitParam->scanInterval) || (scanRangeMin > pInitParam->scanWindow) ||
      (pInitParam->filterPolicy > filterPolicyMax) ||
      ((pInitParam->filterPolicy == LL_SCAN_FILTER_NONE) && (pInitParam->peerAddrType > addrTypeMax)) ||
      (pInitParam->ownAddrType > addrTypeMax) ||
      (LctrValidateConnSpec(pConnSpec) != LL_SUCCESS))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      !LmgrIsAddressTypeAvailable(pInitParam->ownAddrType))
  {
    LL_TRACE_WARN1("Address type invalid or not available, ownAddrType=%u", pInitParam->ownAddrType);
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.dispId = LCTR_DISP_INIT;
    pMsg->hdr.event = LCTR_INIT_MSG_INITIATE;

    pMsg->connSpec = *pConnSpec;

    pMsg->scanParam.scanInterval = pInitParam->scanInterval;
    pMsg->scanParam.scanWindow = pInitParam->scanWindow;
    /* pMsg->scanParam.scanType = LL_SCAN_ACTIVE; */    /* not used */
    pMsg->scanParam.ownAddrType = pInitParam->ownAddrType;
    pMsg->scanParam.scanFiltPolicy = pInitParam->filterPolicy;

    pMsg->peerAddrType = pInitParam->peerAddrType;
    pMsg->peerAddr = BstreamToBda64(pInitParam->pPeerAddr);

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Cancel a create connection operation.
 *
 *  \return     None.
 *
 *  Cancel a connection before it is established.  This function is only used when operating
 *  in master role. This command is used to cancel extended and legacy create connection.
 */
/*************************************************************************************************/
void LlCreateConnCancel(void)
{
  LL_TRACE_INFO0("### LlApi ###  LlCreateConnCancel");

  lctrMsgHdr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    if (lmgrCb.useExtCmds)
    {
      pMsg->dispId = LCTR_DISP_EXT_INIT;
    }
    else
    {
      pMsg->dispId = LCTR_DISP_INIT;
    }
    /* LCTR_EXT_INIT_MSG_INITIATE_CANCEL and LCTR_INIT_MSG_INITIATE_CANCEL shall be aligned. */
    pMsg->event = LCTR_INIT_MSG_INITIATE_CANCEL;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }
}
