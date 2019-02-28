/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) slave control interface implementation file.
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

#include "lctr_api_conn.h"
#include "lmgr_api.h"
#include "wsf_msg.h"
#include "wsf_trace.h"

/*************************************************************************************************/
/*!
 *  \brief      Read remote features.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     Status error code.
 *
 *  Read the link layer features of the remote device.
 */
/*************************************************************************************************/
uint8_t LlReadRemoteFeat(uint16_t handle)
{
  lctrMsgHdr_t *pMsg;

  LL_TRACE_INFO1("### LlApi ###  LlReadRemoteFeat, handle=%u", handle);

  if (LctrIsProcActPended(handle, LCTR_CONN_MSG_API_REMOTE_FEATURE) == TRUE)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if ((pMsg = (lctrMsgHdr_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->handle = handle;
    pMsg->dispId = LCTR_DISP_CONN;
    pMsg->event  = LCTR_CONN_MSG_API_REMOTE_FEATURE;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}
