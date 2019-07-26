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
 * \brief Link layer (LL) slave control interface implementation file.
 */
/*************************************************************************************************/

#include "lctr_api_conn.h"
#include "lmgr_api.h"
#include "ll_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "stack/platform/include/pal_crypto.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Encrypt data.
 *
 *  \param      pKey            Encryption key.
 *  \param      pData           16 bytes of plain text data.
 *
 *  \return     Status error code.
 *
 *  Request the LL to encrypt a block of data in place.
 */
/*************************************************************************************************/
uint8_t LlEncrypt(uint8_t *pKey, uint8_t *pData)
{
  LL_TRACE_INFO0("### LlApi ###  LlEncrypt");

  PalCryptoAesEcb(pKey, pData, pData);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Reply to a LTK request.
 *
 *  \param      handle          Connection handle.
 *  \param      pKey            Pointer to new key.
 *
 *  \return     None.
 *
 *  Provide the requested LTK encryption key.  This function is only used when operating in
 *  slave mode.
 */
/*************************************************************************************************/
uint8_t LlLtkReqReply(uint16_t handle, const uint8_t *pKey)
{
  lctrLtkReply_t *pMsg;

  LL_TRACE_INFO1("### LlApi ###  LlLtkReqReply, handle=%u", handle);

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      ((LctrGetRole(handle) != LL_ROLE_SLAVE) ||
       !LctrIsWaitingForReply(handle, LCTR_HOST_REPLY_LTK_REQ)))
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((pMsg = (lctrLtkReply_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.handle = handle;
    pMsg->hdr.dispId = LCTR_DISP_CONN;
    pMsg->hdr.event  = LCTR_CONN_MSG_API_LTK_REPLY;

    memcpy(pMsg->key, pKey, sizeof(pMsg->key));

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Negative reply to a LTK request.
 *
 *  \param      handle          Connection handle.
 *
 *  \return     None.
 *
 *  Requested LTK encryption key not available.  This function is only used when operating in
 *  slave mode.
 */
/*************************************************************************************************/
uint8_t LlLtkReqNegReply(uint16_t handle)
{
  lctrMsgHdr_t *pMsg;

  LL_TRACE_INFO1("### LlApi ###  LlLtkReqNegReply, handle=%u", handle);

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      ((LctrGetRole(handle) != LL_ROLE_SLAVE) ||
       !LctrIsWaitingForReply(handle, LCTR_HOST_REPLY_LTK_REQ)))
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  if ((pMsg = (lctrMsgHdr_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->handle = handle;
    pMsg->dispId = LCTR_DISP_CONN;
    pMsg->event  = LCTR_CONN_MSG_API_LTK_NEG_REPLY;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Read authenticated payload timeout value.
 *
 *  \param      handle          Connection handle.
 *  \param      pTimeout        Pointer to timeout value in 10 millisecond units.
 *
 *  \return     Status error code.
 *
 *  Read authenticated payload timeout value for the given handle.
 */
/*************************************************************************************************/
uint8_t LlReadAuthPayloadTimeout(uint16_t handle, uint16_t *pTimeout)
{
  LL_TRACE_INFO1("### LlApi ###  LlReadAuthPayloadTimeout, handle=%u", handle);

  if ((LL_API_PARAM_CHECK == 1) &&
      ((handle >= pLctrRtCfg->maxConn) ||
      !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  uint32_t timeoutMs = LctrGetAuthPayloadTimeout(handle);
  /* Convert to 10-milliseconds units. */
  *pTimeout = LL_MATH_DIV_10(timeoutMs);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Write authenticated payload timeout value.
 *
 *  \param      handle          Connection handle.
 *  \param      timeout         New timeout value in 10 millisecond units.
 *
 *  \return     Status error code.
 *
 *  Write new authenticated payload timeout value for the given handle.
 */
/*************************************************************************************************/
uint8_t LlWriteAuthPayloadTimeout(uint16_t handle, uint16_t timeout)
{
  const uint16_t timeoutMin = 0x0001;       /*      10 ms */
  /* const uint16_t timeoutMax = 0xFFFF; */ /* 655,350 ms */

  LL_TRACE_INFO2("### LlApi ###  LlWriteAuthPayloadTimeout, handle=%u, timeout=%u", handle, timeout);

  if ((LL_API_PARAM_CHECK == 1) &&
      ((handle >= pLctrRtCfg->maxConn) ||
      !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
      (timeout < timeoutMin))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  /* Convert to milliseconds. */
  uint32_t timeoutMs = timeout * 10;

  if (!LctrSetAuthPayloadTimeout(handle, timeoutMs))
  {
    return LL_ERROR_CODE_INVALID_LMP_PARAMS;
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Get encryption mode used in a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      pMode           Storage for encryption mode.
 *
 *  \return     Status error code.
 *
 *  Get the encryption mode used by a connection.
 */
/*************************************************************************************************/
uint8_t LlGetEncMode(uint16_t handle, LlEncMode_t *pMode)
{
  LL_TRACE_INFO1("### LlApi ###  LlGetEncMode, handle=%u", handle);

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  LctrGetEncMode(handle, pMode);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set encryption mode used in a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      pMode           New encryption mode.
 *
 *  \return     Status error code.
 *
 *  Set the encryption mode used by a connection. Must be called before encryption is started or
 *  when encryption is paused.
 */
/*************************************************************************************************/
uint8_t LlSetEncMode(uint16_t handle, const LlEncMode_t *pMode)
{
  LL_TRACE_INFO1("### LlApi ###  LlSetEncMode, handle=%u", handle);

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  if (LctrSetEncMode(handle, pMode))
  {
    return LL_SUCCESS;
  }
  else
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }
}
