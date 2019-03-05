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
 *  \brief ATT client optional read PDU processing functions.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "att_api.h"
#include "att_main.h"
#include "attc_main.h"



/*************************************************************************************************/
/*!
 *  \brief  Process received Find By Type response packet.
 *
 *  \param  pCcb      Connection control block.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *  \param  pEvt      Pointer to callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcProcFindByTypeRsp(attcCcb_t *pCcb, uint16_t len, uint8_t *pPacket, attEvt_t *pEvt)
{
  uint8_t   *p;
  uint8_t   *pEnd;
  uint16_t  startHandle;
  uint16_t  endHandle;
  uint16_t  nextHandle;

  p = pPacket + L2C_PAYLOAD_START + ATT_HDR_LEN;
  pEnd = pPacket + L2C_PAYLOAD_START + len;

  /* get and verify all handles */
  nextHandle = pCcb->outReqParams.h.startHandle;
  while (p < pEnd)
  {
    /* get handle pair */
    BSTREAM_TO_UINT16(startHandle, p);
    BSTREAM_TO_UINT16(endHandle, p);

    /*
     * start handle of handle pair must be:
     *  not greater than end handle of handle pair
     *  not less than than start handle of request or end handle of previous handle pair
     *  not greater than end handle of request
     * and no additional handle pairs following end handle = 0xFFFF
     */
    if ((startHandle > endHandle) || (startHandle < nextHandle) ||
        (startHandle > pCcb->outReqParams.h.endHandle) || (nextHandle == 0))
    {
      pEvt->hdr.status = ATT_ERR_INVALID_RSP;
      break;
    }

    /* set next expected handle, with special case for max handle */
    if (endHandle == ATT_HANDLE_MAX)
    {
      nextHandle = 0;
    }
    else
    {
      nextHandle = endHandle + 1;
    }

    /* check for truncated response */
    if (p > pEnd)
    {
      pEvt->hdr.status = ATT_ERR_INVALID_RSP;
      break;
    }
  }

  /* if response was correct */
  if (pEvt->hdr.status == ATT_SUCCESS)
  {
    /* if continuing */
    if (pCcb->outReq.hdr.status == ATTC_CONTINUING)
    {
      /* if all handles read */
      if (nextHandle == 0 || nextHandle > pCcb->outReqParams.h.endHandle)
      {
        /* we're done */
        pCcb->outReq.hdr.status = ATTC_NOT_CONTINUING;
      }
      /* else set up for next request */
      else
      {
        pCcb->outReqParams.h.startHandle = nextHandle;
        pCcb->outReq.handle = nextHandle;
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process received Read Long response packet.
 *
 *  \param  pCcb      Connection control block.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *  \param  pEvt      Pointer to callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attcProcReadLongRsp(attcCcb_t *pCcb, uint16_t len, uint8_t *pPacket, attEvt_t *pEvt)
{
  /* if continuing */
  if (pCcb->outReq.hdr.status == ATTC_CONTINUING)
  {
    /* length of response is less than mtu */
    if (len < pCcb->pMainCcb->mtu)
    {
      /* we're done */
      pCcb->outReq.hdr.status = ATTC_NOT_CONTINUING;
    }
    /* else set up for next request */
    else
    {
      pCcb->outReqParams.o.offset += pEvt->valueLen;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Find By Type Value Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuid16      16-bit UUID to find.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcFindByTypeValueReq(dmConnId_t connId, uint16_t startHandle, uint16_t endHandle,
                            uint16_t uuid16, uint16_t valueLen, uint8_t *pValue, bool_t continuing)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(ATT_FIND_TYPE_REQ_BUF_LEN + valueLen)) != NULL)
  {
    /* set parameters */
    pPkt->len = ATT_FIND_TYPE_REQ_LEN + valueLen;
    pPkt->h.startHandle = startHandle;
    pPkt->h.endHandle = endHandle;

    /* build partial packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_FIND_TYPE_REQ);
    /* skip start and end handle fields */
    p += (2 * sizeof(uint16_t));
    UINT16_TO_BSTREAM(p, uuid16);
    memcpy(p, pValue, valueLen);

    /* send message */
    attcSendMsg(connId, startHandle, ATTC_MSG_API_FIND_BY_TYPE_VALUE, pPkt, continuing);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read By Type Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuidLen     Length of UUID (2 or 16).
 *  \param  pUuid       Pointer to UUID data.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadByTypeReq(dmConnId_t connId, uint16_t startHandle, uint16_t endHandle,
                       uint8_t uuidLen, uint8_t *pUuid, bool_t continuing)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(ATT_READ_TYPE_REQ_BUF_LEN + uuidLen)) != NULL)
  {
    /* set parameters */
    pPkt->len = ATT_READ_TYPE_REQ_LEN + uuidLen;
    pPkt->h.startHandle = startHandle;
    pPkt->h.endHandle = endHandle;

    /* build partial packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_READ_TYPE_REQ);
    /* skip start and end handle fields */
    p += (2 * sizeof(uint16_t));
    memcpy(p, pUuid, uuidLen);

    /* send message */
    attcSendMsg(connId, startHandle, ATTC_MSG_API_READ_BY_TYPE, pPkt, continuing);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      Attribute handle.
 *  \param  offset      Read attribute data starting at this offset.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadLongReq(dmConnId_t connId, uint16_t handle, uint16_t offset, bool_t continuing)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(ATT_READ_BLOB_REQ_BUF_LEN)) != NULL)
  {
    /* set parameters */
    pPkt->len = ATT_READ_BLOB_REQ_LEN;
    pPkt->o.offset = offset;

    /* build partial packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_READ_BLOB_REQ);
    UINT16_TO_BSTREAM(p, handle);

    /* send message */
    attcSendMsg(connId, handle, ATTC_MSG_API_READ_LONG, pPkt, continuing);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read Multiple Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  numHandles  Number of handles in attribute handle list.
 *  \param  pHandles    List of attribute handles.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadMultipleReq(dmConnId_t connId, uint8_t numHandles, uint16_t *pHandles)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;
  uint16_t        handle;

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(ATT_READ_MULT_REQ_BUF_LEN + (numHandles * sizeof(uint16_t)))) != NULL)
  {
    /* set length */
    pPkt->len = ATT_READ_MULT_REQ_LEN + (numHandles * sizeof(uint16_t));

    /* save first handle */
    handle = pHandles[0];

    /* build packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_READ_MULT_REQ);
    while (numHandles--)
    {
      UINT16_TO_BSTREAM(p, *pHandles);
      pHandles++;
    }

    /* send message */
    attcSendMsg(connId, handle, ATTC_MSG_API_READ_MULTIPLE, pPkt, FALSE);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read By Group Type Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuidLen     Length of UUID (2 or 16).
 *  \param  pUuid       Pointer to UUID data.
 *  \param  continuing  TRUE if ATTC continues sending requests until complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcReadByGroupTypeReq(dmConnId_t connId, uint16_t startHandle, uint16_t endHandle,
                            uint8_t uuidLen, uint8_t *pUuid, bool_t continuing)
{
  attcPktParam_t  *pPkt;
  uint8_t         *p;

  /* allocate packet and parameter buffer */
  if ((pPkt = attMsgAlloc(ATT_READ_GROUP_TYPE_REQ_BUF_LEN + uuidLen)) != NULL)
  {
    /* set parameters */
    pPkt->len = ATT_READ_GROUP_TYPE_REQ_LEN + uuidLen;
    pPkt->h.startHandle = startHandle;
    pPkt->h.endHandle = endHandle;

    /* build partial packet */
    p = (uint8_t *) pPkt + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_READ_GROUP_TYPE_REQ);
    /* skip start and end handle fields */
    p += (2 * sizeof(uint16_t));
    memcpy(p, pUuid, uuidLen);

    /* send message */
    attcSendMsg(connId, startHandle, ATTC_MSG_API_READ_BY_GROUP_TYPE, pPkt, continuing);
  }
}
