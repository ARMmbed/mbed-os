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
 *  \brief ATT client optional write PDU processing functions.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "att_api.h"
#include "att_main.h"
#include "attc_main.h"

/*************************************************************************************************/
/*!
 *  \brief  Process received Prepare Write response packet.
 *
 *  \param  pCcb      Connection control block.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *  \param  pEvt      Pointer to callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcProcPrepWriteRsp(attcCcb_t *pCcb, uint16_t len, uint8_t *pPacket, attEvt_t *pEvt)
{
  /* if continuing */
  if (pCcb->outReq.hdr.status == ATTC_CONTINUING)
  {
    /* if no more data to send */
    if (pCcb->outReqParams.w.len == 0)
    {
      /* we're done */
      pCcb->outReq.hdr.status = ATTC_NOT_CONTINUING;
    }
  }

  /* adjust attribute value and its length (adjusted by ATT header length already) */
  pEvt->pValue += (ATT_PREP_WRITE_RSP_LEN - ATT_HDR_LEN);
  pEvt->valueLen -= (ATT_PREP_WRITE_RSP_LEN - ATT_HDR_LEN);
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Write Command.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcWriteCmd(dmConnId_t connId, uint16_t handle, uint16_t valueLen, uint8_t *pValue)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(ATT_WRITE_CMD_BUF_LEN + valueLen)) != NULL)
  {
    /* set length */
    pPkt->len = ATT_WRITE_CMD_LEN + valueLen;

    /* build packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_WRITE_CMD);
    UINT16_TO_BSTREAM(p, handle);
    memcpy(p, pValue, valueLen);

    /* send message */
    attcSendMsg(connId, handle, ATTC_MSG_API_WRITE_CMD, pPkt, FALSE);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Prepare Write Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  offset      Write attribute data starting at this offset.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *  \param  valueByRef  TRUE if pValue data is accessed by reference rather than copied.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcPrepareWriteReq(dmConnId_t connId, uint16_t handle, uint16_t offset, uint16_t valueLen,
                         uint8_t *pValue, bool_t valueByRef, bool_t continuing)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;
  uint16_t        bufLen;

  if (continuing && valueByRef)
  {
    bufLen = ATT_PREP_WRITE_REQ_BUF_LEN;
  }
  else
  {
    bufLen = ATT_PREP_WRITE_REQ_BUF_LEN + valueLen;
  }

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(bufLen)) != NULL)
  {
    /* set parameters */
    pPkt->w.len = valueLen;
    pPkt->w.offset = offset;

    /* build partial packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_PREP_WRITE_REQ);
    UINT16_TO_BSTREAM(p, handle);

    /* skip over offset field */
    p += sizeof(uint16_t);

    /* set value pointer and copy data to packet, if not valueByRef */
    if (continuing && valueByRef)
    {
      pPkt->w.pValue = pValue;
    }
    else
    {
      memcpy(p, pValue, valueLen);
      pPkt->w.pValue = p;
    }

    /* send message */
    attcSendMsg(connId, handle, ATTC_MSG_API_PREP_WRITE, pPkt, continuing);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Execute Write Request.
 *
 *  \param  connId    DM connection ID.
 *  \param  writeAll  TRUE to write all queued writes, FALSE to cancel all queued writes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcExecuteWriteReq(dmConnId_t connId, bool_t writeAll)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(ATT_EXEC_WRITE_REQ_BUF_LEN)) != NULL)
  {
    /* set length */
    pPkt->len = ATT_EXEC_WRITE_REQ_LEN;

    /* build packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_EXEC_WRITE_REQ);
    UINT8_TO_BSTREAM(p, writeAll);

    /* send message */
    attcSendMsg(connId, 0, ATTC_MSG_API_EXEC_WRITE, pPkt, FALSE);
  }
}

