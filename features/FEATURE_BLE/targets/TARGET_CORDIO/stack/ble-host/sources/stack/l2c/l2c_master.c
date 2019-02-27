/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief L2CAP module for master operations.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "l2c_api.h"
#include "l2c_main.h"
#include "dm_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Master processing of received L2CAP signaling packets.
 *
 *  \param  handle    The connection handle.
 *  \param  l2cLen    The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void l2cMasterRxSignalingPkt(uint16_t handle, uint16_t l2cLen, uint8_t *pPacket)
{
  uint8_t       code;
  uint8_t       id;
  uint16_t      len;
  hciConnSpec_t connSpec;

  /* parse code, len, and identifier */
  pPacket += L2C_PAYLOAD_START;
  BSTREAM_TO_UINT8(code, pPacket);
  BSTREAM_TO_UINT8(id, pPacket);
  BSTREAM_TO_UINT16(len, pPacket);

  /* verify signaling length vs. l2c length
   * verify this is a conn param update rsp
   * verify parameter length
   */
  if ((l2cLen != (len + L2C_SIG_HDR_LEN)) ||
      (code != L2C_SIG_CONN_UPDATE_REQ) ||
      (len != L2C_SIG_CONN_UPDATE_REQ_LEN))
  {
    L2C_TRACE_WARN3("invalid msg code:%d len:%d l2cLen:%d", code, len, l2cLen);

    /* reject all unknown or invalid commands except command reject. */
    if (code != L2C_SIG_CMD_REJ)
    {
      l2cSendCmdReject(handle, id, L2C_REJ_NOT_UNDERSTOOD);
    }

    return;
  }

  /* parse parameters */
  BSTREAM_TO_UINT16(connSpec.connIntervalMin, pPacket);
  BSTREAM_TO_UINT16(connSpec.connIntervalMax, pPacket);
  BSTREAM_TO_UINT16(connSpec.connLatency, pPacket);
  BSTREAM_TO_UINT16(connSpec.supTimeout, pPacket);
  connSpec.minCeLen = 0;
  connSpec.maxCeLen = 0;

  /* check parameter range */
  if ((connSpec.connIntervalMin < HCI_CONN_INTERVAL_MIN) ||
      (connSpec.connIntervalMin > HCI_CONN_INTERVAL_MAX) ||
      (connSpec.connIntervalMin > connSpec.connIntervalMax) ||
      (connSpec.connIntervalMax < HCI_CONN_INTERVAL_MIN) ||
      (connSpec.connIntervalMax > HCI_CONN_INTERVAL_MAX) ||
      (connSpec.connLatency > HCI_CONN_LATENCY_MAX) ||
      (connSpec.supTimeout < HCI_SUP_TIMEOUT_MIN) ||
      (connSpec.supTimeout > HCI_SUP_TIMEOUT_MAX))
  {
    L2cDmConnUpdateRsp(id, handle, L2C_CONN_PARAM_REJECTED);
    return;
  }

  DmL2cConnUpdateInd(id, handle, &connSpec);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C for operation as a Bluetooth LE master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cMasterInit(void)
{
  l2cCb.masterRxSignalingPkt = l2cMasterRxSignalingPkt;
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to send an L2CAP connection update response.
 *
 *  \param  identifier  Identifier value previously passed from L2C to DM.
 *  \param  handle      The connection handle.
 *  \param  result      Connection update response result.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDmConnUpdateRsp(uint8_t identifier, uint16_t handle, uint16_t result)
{
  uint8_t *pPacket;
  uint8_t *p;

  /* allocate msg buffer */
  if ((pPacket = l2cMsgAlloc(L2C_SIG_PKT_BASE_LEN + L2C_SIG_CONN_UPDATE_RSP_LEN)) != NULL)
  {
    /* build message */
    p = pPacket + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, L2C_SIG_CONN_UPDATE_RSP);       /* command code */
    UINT8_TO_BSTREAM(p, identifier);                    /* identifier */
    UINT16_TO_BSTREAM(p, L2C_SIG_CONN_UPDATE_RSP_LEN);  /* parameter length */
    UINT16_TO_BSTREAM(p, result);                       /* result */

    /* send packet */
    L2cDataReq(L2C_CID_LE_SIGNALING, handle, (L2C_SIG_HDR_LEN + L2C_SIG_CONN_UPDATE_RSP_LEN), pPacket);
  }
}
