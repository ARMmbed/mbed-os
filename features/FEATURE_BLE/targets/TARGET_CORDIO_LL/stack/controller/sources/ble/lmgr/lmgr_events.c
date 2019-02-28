/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link controller manager event implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
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
