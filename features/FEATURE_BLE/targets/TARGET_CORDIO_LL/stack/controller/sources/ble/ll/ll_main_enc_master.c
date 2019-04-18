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

  if (LctrIsCisEnabled(handle) == TRUE)
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
