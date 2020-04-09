/* Copyright (c) 2009-2019 Arm Limited
 * Copyright (c) 2019-2020 Packetcraft, Inc.
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
 *  \brief SMP main module.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "wsf_buf.h"
#include "wsf_msg.h"
#include "wsf_math.h"
#include "util/bstream.h"
#include "util/calc128.h"
#include "dm_api.h"
#include "smp_api.h"
#include "smp_main.h"
#include "smp_handler.h"
#include "sec_api.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* SMP packet length table */
const uint8_t smpPktLenTbl[] =
{
  0,
  SMP_PAIR_REQ_LEN,
  SMP_PAIR_RSP_LEN,
  SMP_PAIR_CNF_LEN,
  SMP_PAIR_RAND_LEN,
  SMP_PAIR_FAIL_LEN,
  SMP_ENC_INFO_LEN,
  SMP_MASTER_ID_LEN,
  SMP_ID_INFO_LEN,
  SMP_ID_ADDR_INFO_LEN,
  SMP_SIGN_INFO_LEN,
  SMP_SECURITY_REQ_LEN,
  SMP_PUB_KEY_MSG_LEN,
  SMP_DHKEY_CHECK_MSG_LEN,
  SMP_KEYPRESS_MSG_LEN
};

/* Control block */
smpCb_t smpCb;

/*************************************************************************************************/
/*!
 *  \brief  L2C data callback for SMP.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void smpL2cDataCback(uint16_t handle, uint16_t len, uint8_t *pPacket)
{
  uint8_t       cmdCode;
  smpCcb_t      *pCcb;

  /* get connection control block for this handle, ignore packet if not found */
  if ((pCcb = smpCcbByHandle(handle)) == NULL)
  {
    return;
  }

  /* parse command code */
  cmdCode = *(pPacket + L2C_PAYLOAD_START);

  /* verify length and that command is the expected command or pairing failed */
  if ((cmdCode >= SMP_CMD_PAIR_REQ && cmdCode < SMP_CMD_MAX) &&
      (len == smpPktLenTbl[cmdCode]) &&
      ((cmdCode == pCcb->nextCmdCode) || (cmdCode == SMP_CMD_PAIR_FAIL)))
  {
    smpMsg_t msg;

    /* send to state machine */
    if (cmdCode == SMP_CMD_PAIR_FAIL)
    {
      msg.hdr.event = SMP_MSG_CMD_PAIRING_FAILED;
      msg.hdr.status = *(pPacket + L2C_PAYLOAD_START + SMP_HDR_LEN);
    }
    else
    {
      msg.hdr.event = SMP_MSG_CMD_PKT;
    }

    msg.hdr.param = pCcb->connId;
    msg.data.pPacket = pPacket;
    smpSmExecute(pCcb, &msg);
  }
  /* else ignore it */
  else
  {
    SMP_TRACE_WARN3("unexpected packet cmd:%d len:%d, expected:%d", cmdCode, len, pCcb->nextCmdCode);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  L2C control callback for SMP.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void smpL2cCtrlCback(wsfMsgHdr_t *pMsg)
{
  smpCcb_t      *pCcb;
  uint8_t       *pPkt;

  /* get connection control block */
  pCcb = smpCcbByConnId((dmConnId_t) pMsg->param);

  /* verify connection is open */
  if (pCcb->connId != DM_CONN_ID_NONE)
  {
    /* set flow */
    pCcb->flowDisabled = (pMsg->event == L2C_CTRL_FLOW_DISABLE_IND);

    /* if data flow enabled */
    if (!pCcb->flowDisabled)
    {
      /* if packet in qeueue */
      if (pCcb->pQueued != NULL)
      {
        /* send queued packet */
        pPkt = pCcb->pQueued;
        pCcb->pQueued = NULL;
        smpSendPkt(pCcb, pPkt);
      }

      /* if SMP state not idle */
      if (!smpStateIdle(pCcb))
      {
        /* trigger send of next key */
        pMsg->event = SMP_MSG_INT_SEND_NEXT_KEY;
        smpSmExecute(pCcb, (smpMsg_t *) pMsg);
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Called to resume attempts state if a disconnect and reconnect occured while in the
 *          attempts state.
 *
 *  \param  connId  Connection ID to prevent pairing.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void smpResumeAttemptsState(dmConnId_t connId)
{
  smpCcb_t *pCcb = smpCcbByConnId(connId);
  uint32_t timeMs = SmpDbGetPairingDisabledTime(connId);

  if (timeMs)
  {
    if (smpCb.lescSupported)
    {
      pCcb->state = DmConnRole(connId) == DM_ROLE_SLAVE? SMPR_SC_SM_ST_ATTEMPTS : SMPI_SC_SM_ST_ATTEMPTS;
    }
    else
    {
      pCcb->state = DmConnRole(connId) == DM_ROLE_SLAVE? SMPR_SM_ST_ATTEMPTS : SMPI_SM_ST_ATTEMPTS;
    }

    /* Start smp timer indicating the time to prevent pairing in the attempts state */
    pCcb->waitTimer.msg.event = SMP_MSG_INT_WI_TIMEOUT;
    WsfTimerStartMs(&pCcb->waitTimer, timeMs);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM connection callback for SMP.
 *
 *  \param  pDmEvt  DM callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void smpDmConnCback(dmEvt_t *pDmEvt)
{
  smpCcb_t      *pCcb;
  wsfMsgHdr_t   hdr;

  pCcb = smpCcbByConnId((dmConnId_t) pDmEvt->hdr.param);

  /* if new connection created */
  if (pDmEvt->hdr.event == DM_CONN_OPEN_IND)
  {
    /* set up state machine for master or slave */
    if (DmConnRole((dmConnId_t) pDmEvt->hdr.param) == DM_ROLE_MASTER)
    {
      pCcb->initiator = TRUE;
      pCcb->nextCmdCode = SMP_CMD_SECURITY_REQ;
    }
    else
    {
      pCcb->initiator = FALSE;
      pCcb->nextCmdCode = SMP_CMD_PAIR_REQ;
    }

    /* initialize control block */
    pCcb->handle = pDmEvt->connOpen.handle;
    pCcb->connId = (dmConnId_t) pDmEvt->hdr.param;
    pCcb->secReq = FALSE;
    pCcb->flowDisabled = FALSE;
    pCcb->attempts = SmpDbGetFailureCount((dmConnId_t) pDmEvt->hdr.param);
    pCcb->lastSentKey = 0;
    pCcb->state = 0;
    pCcb->keyReady = FALSE;

    /* Resume the attempts state if necessary */
    smpResumeAttemptsState((dmConnId_t) pDmEvt->hdr.param);
  }
  /* else if connection has been opened */
  else if (pCcb->connId != DM_CONN_ID_NONE)
  {
    /* handle close */
    if (pDmEvt->hdr.event == DM_CONN_CLOSE_IND)
    {
      /* store attempts count */
      SmpDbSetFailureCount((dmConnId_t) pDmEvt->hdr.param, pCcb->attempts);

      /* send to state machine */
      hdr.param = pDmEvt->hdr.param;
      hdr.event = SMP_MSG_DM_CONN_CLOSE;
      hdr.status = pDmEvt->connClose.reason + DM_SEC_HCI_ERR_BASE;
      smpSmExecute(pCcb, (smpMsg_t *) &hdr);

      /* clear conn ID after handling event */
      pCcb->connId = DM_CONN_ID_NONE;

      /* free queued packet buffer */
      if (pCcb->pQueued != NULL)
      {
        WsfMsgFree(pCcb->pQueued);
        pCcb->pQueued = NULL;
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Return the connection control block for the given handle.
 *
 *  \param  handle    The connection handle.
 *
 *  \return Pointer to connection control block or NULL if not found.
 */
/*************************************************************************************************/
smpCcb_t *smpCcbByHandle(uint16_t handle)
{
  dmConnId_t  connId;

  if ((connId = DmConnIdByHandle(handle)) != DM_CONN_ID_NONE)
  {
    return &smpCb.ccb[connId - 1];
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the connection control block for the connection ID.
 *
 *  \param  connId    Connection ID.
 *
 *  \return Pointer to connection control block.
 */
/*************************************************************************************************/
smpCcb_t *smpCcbByConnId(dmConnId_t connId)
{
  WSF_ASSERT((connId > 0) && (connId <= DM_CONN_MAX));

  return &smpCb.ccb[connId - 1];
}

/*************************************************************************************************/
/*!
 *  \brief  Perform the first part of SMP calculation C1.
 *
 *  \param  pCcb      Connection control block.
 *  \param  pKey      Encryption key parameter 'k'.
 *  \param  pRand     Random value 'r'.
 *
 *  \return None.
 */
/*************************************************************************************************/
void smpCalcC1Part1(smpCcb_t *pCcb, uint8_t *pKey, uint8_t *pRand)
{
  uint8_t   buf[HCI_ENCRYPT_DATA_LEN];
  uint8_t   *p;
  uint8_t   i;
  uint8_t   iAddrType;
  uint8_t   rAddrType;

  /* set initiator/responder address types */
  if (pCcb->initiator)
  {
    /* if local device's using RPA */
    if (!BdaIsZeros(DmConnLocalRpa(pCcb->connId)))
    {
      iAddrType = DM_ADDR_RANDOM;
    }
    else
    {
      iAddrType = DmConnLocalAddrType(pCcb->connId);
    }

    /* if peer device's using RPA */
    if (!BdaIsZeros(DmConnPeerRpa(pCcb->connId)))
    {
      rAddrType = DM_ADDR_RANDOM;
    }
    else
    {
      rAddrType = DmConnPeerAddrType(pCcb->connId);
    }
  }
  else
  {
    /* if peer device's using RPA */
    if (!BdaIsZeros(DmConnPeerRpa(pCcb->connId)))
    {
      iAddrType = DM_ADDR_RANDOM;
    }
    else
    {
      iAddrType = DmConnPeerAddrType(pCcb->connId);
    }

    /* if local device's using RPA */
    if (!BdaIsZeros(DmConnLocalRpa(pCcb->connId)))
    {
      rAddrType = DM_ADDR_RANDOM;
    }
    else
    {
      rAddrType = DmConnLocalAddrType(pCcb->connId);
    }
  }

  /* note all numbers contained in byte arrays are little endian */

  /* create parameter from xor of r and pres, preq, rat, and iat */
  p = buf;
  *p++ = iAddrType ^ *pRand++;
  *p++ = rAddrType ^ *pRand++;
  for (i = 0; i < SMP_PAIR_REQ_LEN; i++)
  {
    *p++ = pCcb->pairReq[i] ^ *pRand++;
  }
  for (i = 0; i < SMP_PAIR_RSP_LEN; i++)
  {
    *p++ = pCcb->pairRsp[i] ^ *pRand++;
  }

  /* encrypt */
  pCcb->token = SecAes(pKey, buf, smpCb.handlerId, pCcb->connId, SMP_MSG_WSF_AES_CMPL);

  if (pCcb->token == SEC_TOKEN_INVALID)
  {
    wsfMsgHdr_t     hdr;

    /* fail on invalid token */
    hdr.status = SMP_ERR_UNSPECIFIED;
    hdr.event = SMP_MSG_API_CANCEL_REQ;

    smpSmExecute(pCcb, (smpMsg_t *) &hdr);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform the second part of SMP calculation C1.
 *
 *  \param  pCcb      Connection control block.
 *  \param  pKey      Encryption key parameter 'k'.
 *  \param  pPart1    Result from part 1.
 *
 *  \return None.
 */
/*************************************************************************************************/
void smpCalcC1Part2(smpCcb_t *pCcb, uint8_t *pKey, uint8_t *pPart1)
{
  uint8_t   buf[HCI_ENCRYPT_DATA_LEN];
  uint8_t   *p;
  uint8_t   i;
  uint8_t   *pIaddr;
  uint8_t   *pRaddr;

  /* set initiator/responder addresss */
  if (pCcb->initiator)
  {
    /* use local device's RPA */
    pIaddr = DmConnLocalRpa(pCcb->connId);

    /* if local device's not using RPA */
    if (BdaIsZeros(pIaddr))
    {
      /* use local device's address */
      pIaddr = DmConnLocalAddr(pCcb->connId);
    }

    /* use peer device's RPA */
    pRaddr = DmConnPeerRpa(pCcb->connId);

    /* if peer device's not using RPA */
    if (BdaIsZeros(pRaddr))
    {
      /* use peer device's address */
      pRaddr = DmConnPeerAddr(pCcb->connId);
    }
  }
  else
  {
    /* use peer device's RPA */
    pIaddr = DmConnPeerRpa(pCcb->connId);

    /* if peer device's not using RPA */
    if (BdaIsZeros(pIaddr))
    {
      /* use peer device's address */
      pIaddr = DmConnPeerAddr(pCcb->connId);
    }

    /* use local device's RPA */
    pRaddr = DmConnLocalRpa(pCcb->connId);

    /* if local device's not using RPA */
    if (BdaIsZeros(pRaddr))
    {
      /* use local device's address */
      pRaddr = DmConnLocalAddr(pCcb->connId);
    }
  }

  /* note all numbers contained in byte arrays are little endian */

  /* create parameter from xor of part 1 result with ia, ra, and pad */
  p = buf;
  for (i = BDA_ADDR_LEN; i > 0; i--)
  {
    *p++ = *pRaddr++ ^ *pPart1++;
  }
  for (i = BDA_ADDR_LEN; i > 0; i--)
  {
    *p++ = *pIaddr++ ^ *pPart1++;
  }
  *p++ = *pPart1++;
  *p++ = *pPart1++;
  *p++ = *pPart1++;
  *p++ = *pPart1++;

  /* encrypt */
  pCcb->token = SecAes(pKey, buf, smpCb.handlerId, pCcb->connId, SMP_MSG_WSF_AES_CMPL);

  if (pCcb->token == SEC_TOKEN_INVALID)
  {
    wsfMsgHdr_t     hdr;

    /* fail on invalid token */
    hdr.status = SMP_ERR_UNSPECIFIED;
    hdr.event = SMP_MSG_API_CANCEL_REQ;

    smpSmExecute(pCcb, (smpMsg_t *) &hdr);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform calculation S1.
 *
 *  \param  pCcb      Connection control block.
 *  \param  pKey      Encryption key parameter 'k'.
 *  \param  pRand1    Random value 1.
 *  \param  pRand2    Random value 2.
 *
 *  \return None.
 */
/*************************************************************************************************/
void smpCalcS1(smpCcb_t *pCcb, uint8_t *pKey, uint8_t *pRand1, uint8_t *pRand2)
{
  uint8_t   buf[HCI_ENCRYPT_DATA_LEN];

  /* note all numbers contained in byte arrays are little endian */

  /* construct parameter r' from r1 and r2 */
  Calc128Cpy64(buf, pRand2);
  Calc128Cpy64(&buf[SMP_RAND8_LEN], pRand1);

  /* encrypt */
  pCcb->token = SecAes(pKey, buf, smpCb.handlerId, pCcb->connId, SMP_MSG_WSF_AES_CMPL);

  if (pCcb->token == SEC_TOKEN_INVALID)
  {
    wsfMsgHdr_t     hdr;

    /* fail on invalid token */
    hdr.status = SMP_ERR_UNSPECIFIED;
    hdr.event = SMP_MSG_API_CANCEL_REQ;

    smpSmExecute(pCcb, (smpMsg_t *) &hdr);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Generate LTK, EDIV, and RAND.
 *
 *  \param  pScr      Pointer to scratch buffer containing input and output data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void smpGenerateLtk(smpCcb_t *pCcb)
{
  uint8_t *p;
  smpScratch_t *pScr = pCcb->pScr;

  /* generated results are stored in scratch buffer */
  p = pScr->keyInd.keyData.ltk.key;

  /* generate LTK from random number */
  SecRand(p, pScr->keyInd.encKeyLen);
  p += pScr->keyInd.encKeyLen;

  /* set remaining key bytes to zero */
  memset(p, 0, (SMP_KEY_LEN - pScr->keyInd.encKeyLen));

  /* use existing random number stored in scratch buf b4 for EDIV and RAND */
  BYTES_TO_UINT16(pScr->keyInd.keyData.ltk.ediv, pScr->buf.b4);
  memcpy(pScr->keyInd.keyData.ltk.rand, &pScr->buf.b4[2], SMP_RAND8_LEN);

  /* pass key to app via DM */
  pScr->keyInd.type = DM_KEY_LOCAL_LTK;
  pScr->keyInd.secLevel = (pCcb->auth & SMP_AUTH_MITM_FLAG) ? DM_SEC_LEVEL_ENC_AUTH : DM_SEC_LEVEL_ENC;
  pScr->keyInd.hdr.event = DM_SEC_KEY_IND;
  DmSmpCbackExec((dmEvt_t *) &pScr->keyInd);
}

/*************************************************************************************************/
/*!
 *  \brief  Send an SMP command packet.
 *
 *  \param  pCcb      Connection control block.
 *  \param  pPkt      Buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void smpSendPkt(smpCcb_t *pCcb, uint8_t *pPkt)
{
  /* if flow disabled */
  if (pCcb->flowDisabled)
  {
    /* if packet already queued discard it and replace it with this new packet */
    if (pCcb->pQueued != NULL)
    {
      SMP_TRACE_WARN1("smpSendPkt packet discarded cmd:%d", pCcb->pQueued[L2C_PAYLOAD_START]);
      WsfMsgFree(pCcb->pQueued);
    }

    /* queue packet */
    pCcb->pQueued = pPkt;
  }
  /* else send it to L2CAP */
  else
  {
    L2cDataReq(L2C_CID_SMP, pCcb->handle, smpPktLenTbl[pPkt[L2C_PAYLOAD_START]], pPkt);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Check if SMP connection is in idle state.
 *
 *  \param  pCcb      Connection control block.
 *
 *  \return TRUE if in idle state, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t smpStateIdle(smpCcb_t *pCcb)
{
  return (pCcb->state == 0);
}

/*************************************************************************************************/
/*!
 *  \brief  Allocate an SMP data message buffer to be used for the SMP protocol messages.
 *
 *  \param  len   Message length in bytes.
 *
 *  \return Pointer to data message buffer or NULL if allocation failed.
 */
/*************************************************************************************************/
void *smpMsgAlloc(uint16_t len)
{
  return WsfMsgDataAlloc(len, HCI_TX_DATA_TAILROOM);
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to send a message to SMP.
 *
 *  \param  pMsg      Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDmMsgSend(smpDmMsg_t *pMsg)
{
  WsfMsgSend(smpCb.handlerId, pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to notify SMP of encrypted link status.
 *
 *  \param  pMsg    Pointer to HCI message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDmEncryptInd(wsfMsgHdr_t *pMsg)
{
  /* set event to SMP event type */
  pMsg->event = (pMsg->status == HCI_SUCCESS) ?
                 SMP_MSG_DM_ENCRYPT_CMPL : SMP_MSG_DM_ENCRYPT_FAILED;

  /* pass event to handler */
  SmpHandler(0, pMsg);
}

/*************************************************************************************************/
/*!
*  \brief  Calculate Secure Connections security level.
*          Note: calculation assumes Secure Connections was used.  This function cannot be used to
*          calculate a legacy pairing's security level.
*
*  \param  pCcb    Connection control block.
*
*  \return Security level.
*/
/*************************************************************************************************/
uint8_t smpGetScSecLevel(smpCcb_t *pCcb)
{
  uint8_t secLevel;

  if (pCcb->auth & SMP_AUTH_MITM_FLAG)
  {
    if (WSF_MIN(pCcb->pairReq[SMP_MAXKEY_POS], pCcb->pairRsp[SMP_MAXKEY_POS]) == SMP_KEY_SIZE_MAX)
    {
      secLevel = DM_SEC_LEVEL_ENC_LESC;
    }
    else
    {
      secLevel = DM_SEC_LEVEL_ENC_AUTH;
    }
  }
  else
  {
    secLevel = DM_SEC_LEVEL_ENC;
  }

  return secLevel;
}

/*************************************************************************************************/
/*!
 *  \brief  Check if LE Secure Connections is enabled on the connection.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return TRUE is Secure Connections is enabled, else FALSE
 */
/*************************************************************************************************/
bool_t SmpDmLescEnabled(dmConnId_t connId)
{
  smpCcb_t *pCcb = smpCcbByConnId(connId);

  if (pCcb == NULL || pCcb->pScCcb == NULL)
  {
    return FALSE;
  }

  return pCcb->pScCcb->lescEnabled;
}

/*************************************************************************************************/
/*!
 *  \brief  Return the STK for the given connection.
 *
 *  \param  connId    Connection identifier.
 *  \param  pSecLevel Returns the security level of pairing when STK was created.
 *
 *  \return Pointer to STK or NULL if not available.
 */
/*************************************************************************************************/
uint8_t *SmpDmGetStk(dmConnId_t connId, uint8_t *pSecLevel)
{
  smpCcb_t     *pCcb;

  /* get connection control block */
  pCcb = smpCcbByConnId(connId);

  if ((pCcb == NULL) || (pCcb->keyReady == FALSE))
  {
    return NULL;
  }

  if (smpCb.lescSupported && pCcb->pScCcb->lescEnabled && (pCcb->pScCcb->pLtk != NULL))
  {
    /* set security level */
    *pSecLevel = smpGetScSecLevel(pCcb);

    /* return buffer containing STK */
    return pCcb->pScCcb->pLtk->ltk_t;
  }
  else if (pCcb->pScr != NULL)
  {
    /* set security level */
    *pSecLevel = (pCcb->auth & SMP_AUTH_MITM_FLAG) ? DM_SEC_LEVEL_ENC_AUTH : DM_SEC_LEVEL_ENC;

    /* return buffer containing STK */
    return pCcb->pScr->buf.b3;
  }
  else
  {
    return NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Return the LTK for the given connection.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return Pointer to STK or NULL if not available.
 */
/*************************************************************************************************/
uint8_t *SmpDmGetLtk(dmConnId_t connId)
{
  smpCcb_t     *pCcb;

  /* get connection control block */
  pCcb = smpCcbByConnId(connId);

  if (smpCb.lescSupported)
  {
    /* return buffer containing STK */
    return pCcb->pScCcb->pLtk->ltk_t;
  }
  else
  {
    return NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  SMP handler init function called during system initialization.
 *
 *  \param  handlerID  WSF handler ID for SMP.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpHandlerInit(wsfHandlerId_t handlerId)
{
  uint8_t     i;
  smpCcb_t   *pCcb;

  /* store handler ID */
  smpCb.handlerId = handlerId;

  /* Initialize the SMP device database */
  SmpDbInit();

  /* Initialize control block CCBs */
  for (i = 0, pCcb = smpCb.ccb; i < DM_CONN_MAX; i++, pCcb++)
  {
    /* initialize response timer */
    pCcb->rspTimer.handlerId = handlerId;
    pCcb->rspTimer.msg.param = i + 1;  /* param stores the conn id */

    /* initialize wait interval timer */
    pCcb->waitTimer.handlerId = handlerId;
    pCcb->waitTimer.msg.param = i + 1;  /* param stores the conn id */
  }

  /* Register with L2C */
  L2cRegister(L2C_CID_SMP, smpL2cDataCback, smpL2cCtrlCback);

  /* Register with DM */
  DmConnRegister(DM_CLIENT_ID_SMP, smpDmConnCback);
}

/*************************************************************************************************/
/*!
 *
 *  \brief  WSF event handler for SMP.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  smpCcb_t     *pCcb;

  /* Handle message */
  if (pMsg != NULL)
  {
    if (pMsg->event == SMP_DB_SERVICE_IND)
    {
      SmpDbService();
    }
    else
    {
      if (pMsg->event == SMP_MSG_WSF_CMAC_CMPL)
      {
        secCmacMsg_t *pCmac = (secCmacMsg_t *) pMsg;

        /* Free the plain text buffer that was allocated and passed into SecCmac */
        if (pCmac->pPlainText)
        {
          WsfBufFree(pCmac->pPlainText);
        }
      }

      /* get connection control block */
      pCcb = smpCcbByConnId((dmConnId_t) pMsg->param);

      /* verify connection is open */
      if (pCcb->connId != DM_CONN_ID_NONE)
      {
        /* if AES result verify it is not stale */
        if (pMsg->event == SMP_MSG_WSF_AES_CMPL && pCcb->token != pMsg->status)
        {
          SMP_TRACE_WARN2("AES token mismatch: %d %d", pCcb->token, pMsg->status);
        }
        else
        {
          /* send to state machine */
          smpSmExecute(pCcb, (smpMsg_t *) pMsg);
        }
      }
    }
  }
  /* Handle events */
  else if (event)
  {

  }
}
