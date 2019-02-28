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
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Start encryption.
 *
 *  \param      handle          Connection handle.
 *  \param      pRand           Pointer to the random number.
 *  \param      diversifier     Diversifier value.
 *  \param      pKey            Pointer to the encryption key.
 *
 *  \return     Status error code.
 *
 *  Start or restart link layer encryption on a connection.  This function is only used when
 *  operating in master role.
 */
/*************************************************************************************************/
uint8_t LlStartEncryption(uint16_t handle, const uint8_t *pRand, uint16_t diversifier, const uint8_t *pKey)
{
  lctrStartEnc_t *pMsg;

  LL_TRACE_INFO1("### LlApi ###  LlStartEncryption, handle=%u", handle);

  if (LctrIsProcActPended(handle, LCTR_CONN_MSG_API_START_ENC) == TRUE)
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

  if ((pMsg = (lctrStartEnc_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.handle = handle;
    pMsg->hdr.dispId = LCTR_DISP_CONN;
    pMsg->hdr.event  = LCTR_CONN_MSG_API_START_ENC;

    memcpy(pMsg->rand, pRand, sizeof(pMsg->rand));
    pMsg->diversifier = diversifier;
    memcpy(pMsg->key, pKey, sizeof(pMsg->key));

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}
