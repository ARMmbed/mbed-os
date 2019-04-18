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
 * \brief Link controller manager event implementation file.
 */
/*************************************************************************************************/

#include "bb_ble_api.h"
#include "lmgr_api.h"
#include "wsf_msg.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Send advertising enable confirm.
 *
 *  \param      status      Confirm status.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendAdvEnableCnf(uint8_t status)
{
  wsfMsgHdr_t evt =
  {
    .event  = LL_ADV_ENABLE_CNF,
    .status = status
  };

  LL_TRACE_INFO1("### LlEvent ###  LL_ADV_ENABLE_CNF, status=%u", status);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send scan enable confirm.
 *
 *  \param      status      Confirm status.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendScanEnableCnf(uint8_t status)
{
  wsfMsgHdr_t evt =
  {
    .event  = LL_SCAN_ENABLE_CNF,
    .status = status
  };

  LL_TRACE_INFO1("### LlEvent ###  LL_SCAN_ENABLE_CNF, status=%u", status);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send advertising set terminated indication.
 *
 *  \param      handle      Advertising handle.
 *  \param      status      Confirm status.
 *  \param      connHandle  Connection handle.
 *  \param      numEvents   Number of completed events.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrSendAdvSetTermInd(uint8_t handle, uint8_t status, uint16_t connHandle, uint8_t numEvents)
{
  LlAdvSetTermInd_t evt =
  {
    .hdr =
    {
      .event  = LL_ADV_SET_TERM_IND,
      .param  = handle,
      .status = status
    },

    .status = status,
    .advHandle = handle,
    .connHandle = connHandle,
    .numCmplAdvEvt = numEvents
  };

  LL_TRACE_INFO2("### LlEvent ###  LL_ADV_SET_TERM_IND, handle=%u, status=%u", handle, status);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Send event to registered handler.
 *
 *  \param      pEvt        Event data.
 *
 *  \return     TRUE if event was sent.
 */
/*************************************************************************************************/
bool_t LmgrSendEvent(LlEvt_t *pEvt)
{
  bool_t evtSent = FALSE;

  if (lmgrPersistCb.evtCback)
  {
    evtSent = lmgrPersistCb.evtCback(pEvt);
  }
  return evtSent;
}
