/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  ATT server main module.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "wsf_buf.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include "att_api.h"
#include "att_main.h"
#include "atts_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* ATTS control block */

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

static void attsDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket);
static void attsConnCback(attCcb_t *pCcb, dmEvt_t *pDmEvt);
static void attsMsgCback(wsfMsgHdr_t *pMsg);
static void attsL2cCtrlCback(wsfMsgHdr_t *pMsg);

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Interface to ATT */
static const attFcnIf_t attsFcnIf =
{
  attsDataCback,
  attsL2cCtrlCback,
  (attMsgHandler_t) attsMsgCback,
  attsConnCback
};

/* Minimum PDU lengths, indexed by method */
static const uint8_t attsMinPduLen[] =
{
  0,                                  /* ATT_METHOD_ERR */
  ATT_MTU_REQ_LEN,                    /* ATT_METHOD_MTU */
  ATT_FIND_INFO_REQ_LEN,              /* ATT_METHOD_FIND_INFO */
  ATT_FIND_TYPE_REQ_LEN,              /* ATT_METHOD_FIND_TYPE */
  ATT_READ_TYPE_REQ_LEN,              /* ATT_METHOD_READ_TYPE */
  ATT_READ_REQ_LEN,                   /* ATT_METHOD_READ */
  ATT_READ_BLOB_REQ_LEN,              /* ATT_METHOD_READ_BLOB */
  ATT_READ_MULT_REQ_LEN + 4,          /* ATT_METHOD_READ_MULTIPLE */
  ATT_READ_GROUP_TYPE_REQ_LEN,        /* ATT_METHOD_READ_GROUP_TYPE */
  ATT_WRITE_REQ_LEN,                  /* ATT_METHOD_WRITE */
  ATT_WRITE_CMD_LEN,                  /* ATT_METHOD_WRITE_CMD */
  ATT_PREP_WRITE_REQ_LEN,             /* ATT_METHOD_PREPARE_WRITE */
  ATT_EXEC_WRITE_REQ_LEN,             /* ATT_METHOD_EXECUTE_WRITE */
  0,                                  /* ATT_METHOD_VALUE_NTF */
  0,                                  /* ATT_METHOD_VALUE_IND */
  ATT_VALUE_CNF_LEN,                  /* ATT_METHOD_VALUE_CNF */
  ATT_SIGNED_WRITE_CMD_LEN            /* ATT_METHOD_SIGNED_WRITE_CMD */
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* PDU processing function lookup table, indexed by method */
attsProcFcn_t attsProcFcnTbl[ATT_METHOD_SIGNED_WRITE_CMD+1] =
{
  NULL,                               /* ATT_METHOD_ERR */
  attsProcMtuReq,                     /* ATT_METHOD_MTU */
  attsProcFindInfoReq,                /* ATT_METHOD_FIND_INFO */
  attsProcFindTypeReq,                /* ATT_METHOD_FIND_TYPE */
  attsProcReadTypeReq,                /* ATT_METHOD_READ_TYPE */
  attsProcReadReq,                    /* ATT_METHOD_READ */
  attsProcReadBlobReq,                /* ATT_METHOD_READ_BLOB */
  attsProcReadMultReq,                /* ATT_METHOD_READ_MULT */
  attsProcReadGroupTypeReq,           /* ATT_METHOD_READ_GROUP_TYPE */
  attsProcWrite,                      /* ATT_METHOD_WRITE */
  attsProcWrite,                      /* ATT_METHOD_WRITE_CMD */
  attsProcPrepWriteReq,               /* ATT_METHOD_PREP_WRITE */
  attsProcExecWriteReq,               /* ATT_METHOD_EXEC_WRITE */
  NULL,                               /* ATT_METHOD_VALUE_NTF */
  NULL,                               /* ATT_METHOD_VALUE_IND */
  attsProcValueCnf,                   /* ATT_METHOD_VALUE_CNF */
  NULL                                /* ATT_METHOD_SIGNED_WRITE_CMD */
};

/* Control block */
attsCb_t attsCb;

/*************************************************************************************************/
/*!
 *  \brief  Data callback for ATTS.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attsDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  uint8_t       opcode;
  uint8_t       method;
  uint8_t       err;
  attsProcFcn_t procFcn;
  attCcb_t      *pCcb;

  /* get connection cb for this handle */
  if ((pCcb = attCcbByHandle(handle)) == NULL)
  {
    return;
  }

  /* parse opcode */
  opcode = *(pPacket + L2C_PAYLOAD_START);

  /* get method */
  if ((opcode <= ATT_PDU_WRITE_REQ) ||
      ((opcode >= ATT_PDU_PREP_WRITE_REQ) && (opcode <= ATT_PDU_VALUE_CNF)))
  {
    method = ATT_OPCODE_2_METHOD(opcode);
  }
  else if (opcode == ATT_PDU_WRITE_CMD)
  {
    method = ATT_METHOD_WRITE_CMD;
  }
  else if (opcode == ATT_PDU_SIGNED_WRITE_CMD)
  {
    method = ATT_METHOD_SIGNED_WRITE_CMD;
  }
  else
  {
    method = ATT_METHOD_ERR;
  }

  /* look up processing function */
  procFcn = attsProcFcnTbl[method];

#if defined(ATTS_ERROR_TEST) && (ATTS_ERROR_TEST == TRUE)
  if (attCb.errTest != ATT_SUCCESS)
  {
    uint16_t attHandle;
    BYTES_TO_UINT16(attHandle, pPacket + L2C_PAYLOAD_START + ATT_HDR_LEN);
    attsErrRsp(handle, opcode, attHandle, attCb.errTest);
    return;
  }
#endif

  /* if method is supported */
  if (procFcn != NULL)
  {
    /* verify length */
    if (len >= attsMinPduLen[method])
    {
      /* execute processing function */
      (*procFcn)(pCcb, len, pPacket);
      err = 0;
    }
    else
    {
      /* invalid PDU length */
      err = ATT_ERR_INVALID_PDU;
    }
  }
  else
  {
    /* PDU not supported */
    err = ATT_ERR_NOT_SUP;
  }

  /* if there's an error and an error response can be sent for this opcode */
  if (err && (opcode != ATT_PDU_MTU_REQ) && (opcode != ATT_PDU_VALUE_CNF) &&
      ((opcode & ATT_PDU_MASK_COMMAND) == 0))
  {
    attsErrRsp(handle, opcode, 0, err);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Connection callback for ATTS.
 *
 *  \param  pCcb    ATT control block.
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attsConnCback(attCcb_t *pCcb, dmEvt_t *pDmEvt)
{
  /* if connection closed */
  if (pDmEvt->hdr.event == DM_CONN_CLOSE_IND)
  {
    /* clear prepare write queue */
    attsClearPrepWrites(pCcb);

    /* stop service discovery idle timer, if running */
    if (DmConnCheckIdle(pCcb->connId) & DM_IDLE_ATTS_DISC)
    {
      WsfTimerStop(&pCcb->idleTimer);
    }
  }

  /* pass event to indication interface */
  (*attsCb.pInd->connCback)(pCcb, pDmEvt);
}

/*************************************************************************************************/
/*!
 *  \brief  WSF message handler callback for ATTS.
 *
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attsMsgCback(wsfMsgHdr_t *pMsg)
{
  /* handle service discovery idle timeout */
  if (pMsg->event == ATTS_MSG_IDLE_TIMEOUT)
  {
    /* set channel as idle */
    DmConnSetIdle((dmConnId_t) pMsg->param, DM_IDLE_ATTS_DISC, DM_CONN_IDLE);
  }
  /* pass event to indication interface */
  else if (pMsg->event <= ATTS_MSG_IND_TIMEOUT)
  {
    (*attsCb.pInd->msgCback)(pMsg);
  }
  /* pass event to signed data interface */
  else
  {
    (*attsCb.signMsgCback)(pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  L2C control callback for ATTS.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void attsL2cCtrlCback(wsfMsgHdr_t *pMsg)
{
  /* pass event to indication interface */
  (*attsCb.pInd->ctrlCback)(pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Send an error response PDU.
 *
 *  \param  handle    The connection handle.
 *  \param  opcode    Opcode of the request that generated this error.
 *  \param  attHandle Attribute handle in request, if applicable.
 *  \param  reason    Error reason.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attsErrRsp(uint16_t handle, uint8_t opcode, uint16_t attHandle, uint8_t reason)
{
  uint8_t *pBuf;
  uint8_t *p;

  /* allocate buffer */
  if ((pBuf = attMsgAlloc(L2C_PAYLOAD_START + ATT_ERR_RSP_LEN)) != NULL)
  {
    p = pBuf + L2C_PAYLOAD_START;
    UINT8_TO_BSTREAM(p, ATT_PDU_ERR_RSP);
    UINT8_TO_BSTREAM(p, opcode);
    UINT16_TO_BSTREAM(p, attHandle);
    UINT8_TO_BSTREAM(p, reason);

    L2cDataReq(L2C_CID_ATT, handle, ATT_ERR_RSP_LEN, pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Clear the prepared write queue.
 *
 *  \param  pCcb    ATT control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attsClearPrepWrites(attCcb_t *pCcb)
{
  void *pBuf;

  while ((pBuf = WsfQueueDeq(&pCcb->prepWriteQueue)) != NULL)
  {
    WsfBufFree(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set connection idle status to busy for service discovery.
 *
 *  \param  pCcb    ATT control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void attsDiscBusy(attCcb_t *pCcb)
{
  if (pAttCfg->discIdleTimeout > 0)
  {
    /* set channel as busy */
    DmConnSetIdle(pCcb->connId, DM_IDLE_ATTS_DISC, DM_CONN_BUSY);

    /* start service discovery idle timer */
    pCcb->idleTimer.handlerId = attCb.handlerId;
    pCcb->idleTimer.msg.event = ATTS_MSG_IDLE_TIMEOUT;
    pCcb->idleTimer.msg.param = pCcb->connId;
    WsfTimerStartSec(&pCcb->idleTimer, pAttCfg->discIdleTimeout);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize ATT server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsInit(void)
{
  /* Initialize control block */
  WSF_QUEUE_INIT(&attsCb.groupQueue);
  attsCb.pInd = &attFcnDefault;
  attsCb.signMsgCback = (attMsgHandler_t) attEmptyHandler;

  /* set up callback interfaces */
  attCb.pServer = &attsFcnIf;
}

/*************************************************************************************************/
/*!
 *  \brief  Register an authorization callback with the attribute server.
 *
 *  \param  cback  Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsAuthorRegister(attsAuthorCback_t cback)
{
  attsCb.authorCback = cback;
}

/*************************************************************************************************/
/*!
 *  \brief  Add an attribute group to the attribute server.
 *
 *  \param  pGroup    Pointer to an attribute group structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsAddGroup(attsGroup_t *pGroup)
{
  attsGroup_t   *pElem;
  attsGroup_t   *pPrev = NULL;

  /* task schedule lock */
  WsfTaskLock();

  pElem = (attsGroup_t *) attsCb.groupQueue.pHead;

  /* iterate over queue sorted by increasing handle value */
  while (pElem != NULL)
  {
    if (pGroup->startHandle < pElem->startHandle)
    {
      break;
    }
    pPrev = pElem;
    pElem = pElem->pNext;
  }

  /* insert new group */
  WsfQueueInsert(&attsCb.groupQueue, pGroup, pPrev);

  /* task schedule unlock */
  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Remove an attribute group from the attribute server.
 *
 *  \param  startHandle  Start handle of attribute group to be removed.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsRemoveGroup(uint16_t startHandle)
{
  attsGroup_t   *pElem;
  attsGroup_t   *pPrev = NULL;

  /* task schedule lock */
  WsfTaskLock();

  pElem = (attsGroup_t *) attsCb.groupQueue.pHead;

  /* find group in queue */
  while (pElem != NULL)
  {
    if (pElem->startHandle == startHandle)
    {
      break;
    }
    pPrev = pElem;
    pElem = pElem->pNext;
  }

  /* if group found remove from queue */
  if (pElem != NULL)
  {
    WsfQueueRemove(&attsCb.groupQueue, pElem, pPrev);
  }
  else
  {
    ATT_TRACE_WARN1("AttsRemoveGroup start handle not found: 0x%04x", startHandle);
  }

  /* task schedule unlock */
  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Set an attribute value in the attribute server.
 *
 *  \param  handle    Attribute handle.
 *  \param  valueLen  Attribute length.
 *  \param  pValue    Attribute value.
 *
 *  \return ATT_SUCCESS if successful otherwise error.
 */
/*************************************************************************************************/
uint8_t AttsSetAttr(uint16_t handle, uint16_t valueLen, uint8_t *pValue)
{
  attsAttr_t  *pAttr;
  attsGroup_t *pGroup;
  uint8_t     err = ATT_SUCCESS;

  WsfTaskLock();

  /* find attribute */
  if ((pAttr = attsFindByHandle(handle, &pGroup)) != NULL)
  {
    /* verify write length */
    if (valueLen > pAttr->maxLen)
    {
      err = ATT_ERR_LENGTH;
    }
    else
    {
      /* set attribute value */
      memcpy(pAttr->pValue, pValue, valueLen);

      /* set the length if variable length attribute */
      if ((pAttr->settings & ATTS_SET_VARIABLE_LEN) != 0)
      {
        *(pAttr->pLen) = valueLen;
      }
    }
  }
  /* else attribute not found */
  else
  {
    err = ATT_ERR_NOT_FOUND;
  }

  WsfTaskUnlock();

  return err;
}

/*************************************************************************************************/
/*!
 *  \brief  Get an attribute value in the attribute server.  Tasks should be locked before
 *          calling this function and remain locked until pLen and pValue are no longer used.
 *
 *  \param  handle    Attribute handle.
 *  \param  pLen      Returned attribute length pointer.
 *  \param  pValue    Returned attribute value pointer.
 *
 *  \return ATT_SUCCESS if successful or other error code if failure.
 */
/*************************************************************************************************/
uint8_t AttsGetAttr(uint16_t handle, uint16_t *pLen, uint8_t **pValue)
{
  attsAttr_t  *pAttr;
  attsGroup_t *pGroup;
  uint8_t     err = ATT_SUCCESS;

  /* find attribute */
  if ((pAttr = attsFindByHandle(handle, &pGroup)) != NULL)
  {
    /* set length and value pointers */
    *pLen = *(pAttr->pLen);
    *pValue = pAttr->pValue;
  }
  /* else attribute not found */
  else
  {
    err = ATT_ERR_NOT_FOUND;
  }

  return err;
}

/*************************************************************************************************/
/*!
 *  \brief  For testing purposes only.
 *
 *  \param  status    ATT status
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttsErrorTest(uint8_t status)
{
  attCb.errTest = status;
}
