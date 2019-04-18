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
 * \brief Link manager extended advertising implementation file.
 */
/*************************************************************************************************/

#include "lmgr_api_adv_slave_ae.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief      Send extended advertising enable confirm.
 *
 *  \param      handle      Advertising handle.
 *  \param      status      Confirm status.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendExtAdvEnableCnf(uint8_t handle, uint8_t status)
{
  if ((lmgrCb.advSetEnaStatus == LL_SUCCESS) && (status != LL_SUCCESS))
  {
    /* Store first error. */
    lmgrCb.advSetEnaStatus = status;
  }

  WSF_ASSERT(lmgrCb.extAdvEnaDelayCnt > 0);
  if (--lmgrCb.extAdvEnaDelayCnt > 0)
  {
    /* Delay until all enable confirms received. */
    return;
  }

  LlExtAdvEnableCnf_t evt =
  {
    .hdr =
    {
      .event  = LL_EXT_ADV_ENABLE_CNF,
      .param  = handle,
      .status = lmgrCb.advSetEnaStatus
    },

    .handle = handle,
    .status = lmgrCb.advSetEnaStatus
  };

  LL_TRACE_INFO2("### LlEvent ###  LL_EXT_ADV_ENABLE_CNF, handle=%u, status=%u", handle, lmgrCb.advSetEnaStatus);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send extended advertising enable confirm.
 *
 *  \param      handle          Advertising handle.
 *  \param      scanAddrType    Scanner address type.
 *  \param      scanAddr        Scanner address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendScanReqReceivedInd(uint8_t handle, uint8_t scanAddrType, uint64_t scanAddr)
{
  LlScanReqRcvdInd_t evt =
  {
    .hdr =
    {
      .event  = LL_SCAN_REQ_RCVD_IND,
      .param  = handle,
      .status = LL_SUCCESS
    },

    .handle       = handle,
    .scanAddrType = scanAddrType
  };

  Bda64ToBstream(evt.scanAddr, scanAddr);

  LL_TRACE_INFO1("### LlEvent ###  LL_SCAN_REQ_RCVD_IND, handle=%u, status=LL_SUCCESS", handle);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send periodic advertising enable confirm.
 *
 *  \param      handle      Advertising handle.
 *  \param      status      Confirm status.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendPeriodicAdvEnableCnf(uint8_t handle, uint8_t status)
{
  LlPerAdvEnableCnf_t evt =
  {
    .hdr =
    {
      .event  = LL_PER_ADV_ENABLE_CNF,
      .param  = handle,
      .status = status
    },

    .handle = handle,
    .status = status
  };

  LL_TRACE_INFO2("### LlEvent ###  LL_PER_ADV_ENABLE_CNF, handle=%u, status=%u", handle, status);

  LmgrSendEvent((LlEvt_t *)&evt);
}
