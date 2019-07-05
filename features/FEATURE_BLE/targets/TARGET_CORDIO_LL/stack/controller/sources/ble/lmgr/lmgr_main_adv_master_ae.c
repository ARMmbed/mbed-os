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
 * \brief Link manager extended scan implementation file.
 */
/*************************************************************************************************/

#include "lmgr_api_adv_master_ae.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/*************************************************************************************************/
/*!
 *  \brief      Send extended scan enable confirm.
 *
 *  \param      status      Confirm status.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendExtScanEnableCnf(uint8_t status)
{
  if ((lmgrCb.extScanEnaStatus == LL_SUCCESS) && (status != LL_SUCCESS))
  {
    /* Store first error. */
    lmgrCb.extScanEnaStatus = status;
  }

  if ((lmgrCb.extScanEnaDelayCnt == 0) ||   /* Suppress event. */
      (--lmgrCb.extScanEnaDelayCnt > 0))    /* Wait for last scanner. */
  {
    /* Delay until all enable confirms received. */
    return;
  }

  LlExtScanEnableCnf_t evt =
  {
    .hdr =
    {
      .event  = LL_EXT_SCAN_ENABLE_CNF,
      .status = lmgrCb.extScanEnaStatus
    },

    .status = lmgrCb.extScanEnaStatus
  };

  LL_TRACE_INFO1("### LlEvent ###  LL_EXT_SCAN_ENABLE_CNF, status=%u", lmgrCb.extScanEnaStatus);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send scan timeout indication.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendScanTimeoutInd(void)
{
  wsfMsgHdr_t evt =
  {
    .event  = LL_SCAN_TIMEOUT_IND,
    .status = LL_SUCCESS
  };

  LL_TRACE_INFO0("### LlEvent ###  LL_SCAN_TIMEOUT_IND");

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send extended advertising report indication.
 *
 *  \param      pEvt        Extended advertising report event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendExtAdvRptInd(LlExtAdvReportInd_t *pEvt)
{
  pEvt->hdr.param = 0;
  pEvt->hdr.event = LL_EXT_ADV_REPORT_IND;
  pEvt->hdr.status = LL_SUCCESS;

  LL_TRACE_INFO2("### LlEvent ###  LL_EXT_ADV_REPORT_IND, status=LL_SUCCESS, eventType=%u, SID=%u", pEvt->eventType, pEvt->advSID);

  LmgrSendEvent((LlEvt_t *)pEvt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send periodic advertising report event.
 *
 *  \param      pEvt        Periodic advertising report.
 *
 *  \return     TRUE if event was sent.
 */
/*************************************************************************************************/
void LmgrSendPerAdvRptInd(LlPerAdvReportInd_t *pEvt)
{
  pEvt->hdr.param = 0;
  pEvt->hdr.event = LL_PER_ADV_REPORT_IND;
  pEvt->hdr.status = LL_SUCCESS;

  LL_TRACE_INFO2("### LlEvent ###  LL_PER_ADV_REPORT_IND, handle=%u dataLen=%u ", pEvt->syncHandle, pEvt->len);

  LmgrSendEvent((LlEvt_t *)pEvt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send periodic advertising sync established event.
 *
 *  \param      status      Confirm status.
 *  \param      handle      Sync handle.
 *  \param      pEvt        Periodic advertising sync established event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendSyncEstInd(uint8_t status, uint16_t handle, lmgrPerAdvSyncEstdInd_t *pEvt)
{
  LlPerAdvSyncEstdCnf_t evt =
  {
    .hdr =
    {
      .event  = LL_PER_ADV_SYNC_ESTD_IND,
    },
    .status = status,
    .syncHandle = handle,
    .advSID = pEvt->advSID,
    .addrType = pEvt->addrType,
    .advPhy = pEvt->advPhy,
    .perAdvInterval = pEvt->advInterval,
    .advClkAccuracy = pEvt->advClkAccuracy
  };

  BdaCpy(evt.addr, pEvt->addr);

  LL_TRACE_INFO3("### LlEvent ###  LL_PER_ADV_SYNC_ESTD_IND, handle=%u status=%u advSID=%u", handle, status, pEvt->advSID);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send periodic advertising sync lost event.
 *
 *  \param      handle      Sync handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendSyncLostInd(uint16_t handle)
{
  LlPerAdvSyncLostInd_t evt =
  {
    .hdr =
    {
      .event  = LL_PER_ADV_SYNC_LOST_IND,
    },
    .syncHandle = handle,
  };

  LL_TRACE_INFO1("### LlEvent ###  LL_PER_ADV_SYNC_LOST_IND, handle=%u", handle);

  LmgrSendEvent((LlEvt_t *)&evt);
}
