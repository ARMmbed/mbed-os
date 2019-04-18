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
 * \brief Link layer controller periodic sync transfer implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_adv_master_ae.h"
#include "lctr_int_adv_master.h"
#include "lctr_api_adv_master_ae.h"
#include "lctr_int_conn.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "bb_ble_api_reslist.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_math.h"
#include "util/bstream.h"

/**************************************************************************************************
  Globals
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Send sync info about periodic advertising to a connected device.
 *
 *  \param      connHandle        Connection handle.
 *  \param      serviceData       Service data provided by the host.
 *  \param      syncHandle        Periodic sync handle.
 *
 *  \return     Status error code.
 *
 *  Send sync info about about periodic advertising to a connected device.
 */
/*************************************************************************************************/
uint8_t LctrPeriodicAdvSyncTransfer(uint16_t connHandle, uint16_t serviceData, uint16_t syncHandle)
{
  lctrPerAdvSyncTrsf_t *pMsg;

  if (syncHandle > LL_SYNC_MAX_HANDLE)
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  if (syncHandle >= LL_MAX_PER_SCAN)
  {
    return LL_ERROR_CODE_UNKNOWN_ADV_ID;
  }

  if ((lctrMstPerScanTbl[syncHandle].enabled == FALSE) ||
      (lctrMstPerScanTbl[syncHandle].state != LCTR_PER_SCAN_STATE_SYNC_ESTD))
  {
    return LL_ERROR_CODE_UNKNOWN_ADV_ID;
  }

  if (connHandle >= pLctrRtCfg->maxConn)
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(connHandle);
  if (!pCtx->enabled)
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  /* Do not allow long control PDU if we do not know the remote feature explicitly. */
  if (((pCtx->usedFeatSet & LL_FEAT_PAST_RECIPIENT) == 0) || !pCtx->featExchFlag)
  {
    return LL_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE;
  }

  if ((pMsg = (lctrPerAdvSyncTrsf_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.handle = connHandle;
    pMsg->hdr.dispId = LCTR_DISP_CONN;
    pMsg->hdr.event  = LCTR_CONN_MSG_API_PER_ADV_SYNC_TRSF;
    pMsg->syncSource = LCTR_SYNC_SRC_SCAN;
    pMsg->syncHandle = syncHandle;
    pMsg->serviceData = serviceData;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Send sync info about periodic adv in an advertising set to a connected device.
 *
 *  \param      connHandle        Connection handle.
 *  \param      serviceData       Service data provided by the host.
 *  \param      advHandle         Handle to identify an advertising set.
 *
 *
 *  \return     Status error code.
 *
 *  Send sync info about periodic adv in an advertising set to a connected device.
 */
/*************************************************************************************************/
uint8_t LctrPeriodicAdvSetInfoTransfer(uint16_t connHandle, uint16_t serviceData, uint8_t advHandle)
{
  lctrPerAdvSyncTrsf_t *pMsg;
  lctrAdvSet_t *pAdvSet;

  if (connHandle >= pLctrRtCfg->maxConn)
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if ((pAdvSet = lctrFindAdvSet(advHandle)) == NULL)
  {
    return LL_ERROR_CODE_UNKNOWN_ADV_ID;
  }

  if (pAdvSet->perParam.perAdvEnabled != TRUE)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(connHandle);
  if (!pCtx->enabled)
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  /* Do not allow long control PDU if we do not know the remote feature explicitly. */
  if (((pCtx->usedFeatSet & LL_FEAT_PAST_RECIPIENT) == 0) || !pCtx->featExchFlag)
  {
    return LL_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE;
  }

  if ((pMsg = (lctrPerAdvSyncTrsf_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.handle = connHandle;
    pMsg->hdr.dispId = LCTR_DISP_CONN;
    pMsg->hdr.event  = LCTR_CONN_MSG_API_PER_ADV_SYNC_TRSF;
    pMsg->syncSource = LCTR_SYNC_SRC_BCST;
    pMsg->syncHandle = advHandle;
    pMsg->serviceData = serviceData;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set periodic advertising sync transfer parameters.
 *
 *  \param      connHandle        Connection handle.
 *  \param      mode              Periodic sync advertising sync transfer mode.
 *  \param      skip              The number of periodic advertising packets that can be skipped after a successful receive.
 *  \param      syncTimeout       Synchronization timeout for the periodic advertising.
 *  \param      cteType           Constant tone extension type(Used in AoD/AoA).
 *
 *  \return     Status error code.
 *
 *  Set periodic advertising sync transfer parameters.
 */
/*************************************************************************************************/
uint8_t LctrSetPeriodicAdvSyncTransParams(uint16_t connHandle, uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType)
{
  if (connHandle >= pLctrRtCfg->maxConn)
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  lctrConnCtx_t *pCtx = LCTR_GET_CONN_CTX(connHandle);
  if (!pCtx->enabled)
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  pCtx->syncMode = mode;
  pCtx->syncSkip = skip;
  pCtx->syncTimeout = syncTimeout;

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize lctr subsystem for PAST(Periodic advertising sync transfer).
 *
 *  \return     None.
 *
 *  This function initializes the lctr subsystem for PAST(Periodic advertising sync transfer).
 */
/*************************************************************************************************/
void LctrPastInit(void)
{
  lctrSendPerSyncFromScanFn = lctrSendPerSyncFromScan;
  lctrSendPerSyncFromBcstFn = lctrSendPerSyncFromBcst;
  lctrStorePeriodicSyncTrsfFn = lctrStorePeriodicSyncTrsf;
  lctrSendPeriodicSyncIndFn = lctrSendPeriodicSyncInd;
  lctrReceivePeriodicSyncIndFn = lctrReceivePeriodicSyncInd;
}
