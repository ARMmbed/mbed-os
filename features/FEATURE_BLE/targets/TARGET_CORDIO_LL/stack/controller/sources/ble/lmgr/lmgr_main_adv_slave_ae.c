/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link manager extended advertising implementation file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
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
