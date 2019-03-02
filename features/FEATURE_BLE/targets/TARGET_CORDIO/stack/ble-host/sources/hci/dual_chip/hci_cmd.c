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
 *  \brief HCI command module.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_queue.h"
#include "wsf_timer.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include "hci_cmd.h"
#include "hci_tr.h"
#include "hci_api.h"
#include "hci_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* HCI command timeout in seconds */
#define HCI_CMD_TIMEOUT           2

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Local control block type */
typedef struct
{
  wsfTimer_t      cmdTimer;       /* HCI command timeout timer */
  wsfQueue_t      cmdQueue;       /* HCI command queue */
  uint16_t        cmdOpcode;      /* Opcode of last HCI command sent */
  uint8_t         numCmdPkts;     /* Number of outstanding HCI commands that can be sent */
} hciCmdCb_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Local control block */
hciCmdCb_t hciCmdCb;

/*************************************************************************************************/
/*!
 *  \brief  Allocate an HCI command buffer and set the command header fields.
 *
 *  \param  opcode  Command opcode.
 *  \param  len     length of command parameters.
 *
 *  \return Pointer to WSF msg buffer.
 */
/*************************************************************************************************/
uint8_t *hciCmdAlloc(uint16_t opcode, uint16_t len)
{
  uint8_t   *p;

  /* allocate buffer */
  if ((p = WsfMsgAlloc(len + HCI_CMD_HDR_LEN)) != NULL)
  {
    /* set HCI command header */
    UINT16_TO_BSTREAM(p, opcode);
    UINT8_TO_BSTREAM(p, len);
    p -= HCI_CMD_HDR_LEN;
  }

  return p;
}

/*************************************************************************************************/
/*!
 *  \brief  Send an HCI command and service the HCI command queue.
 *
 *  \param  pData  Buffer containing HCI command to send or NULL.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCmdSend(uint8_t *pData)
{
  uint8_t         *p;
  wsfHandlerId_t  handlerId;

  /* queue command if present */
  if (pData != NULL)
  {
    /* queue data - message handler ID 'handerId' not used */
    WsfMsgEnq(&hciCmdCb.cmdQueue, 0, pData);
  }

  /* service the HCI command queue; first check if controller can accept any commands */
  if (hciCmdCb.numCmdPkts > 0)
  {
    /* if queue not empty */
    if ((p = WsfMsgDeq(&hciCmdCb.cmdQueue, &handlerId)) != NULL)
    {
      /* decrement controller command packet count */
      hciCmdCb.numCmdPkts--;

      /* store opcode of command we're sending */
      BYTES_TO_UINT16(hciCmdCb.cmdOpcode, p);

      /* start command timeout */
      WsfTimerStartSec(&hciCmdCb.cmdTimer, HCI_CMD_TIMEOUT);

      /* send command to transport */
      hciTrSendCmd(p);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the HCI cmd module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCmdInit(void)
{
  WSF_QUEUE_INIT(&hciCmdCb.cmdQueue);

  /* initialize numCmdPkts for special case of first command */
  hciCmdCb.numCmdPkts = 1;

  /* initialize timer */
  hciCmdCb.cmdTimer.msg.event = HCI_MSG_CMD_TIMEOUT;
  hciCmdCb.cmdTimer.handlerId = hciCb.handlerId;
}

/*************************************************************************************************/
/*!
 *  \brief  Process an HCI command timeout.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCmdTimeout(wsfMsgHdr_t *pMsg)
{
  HCI_TRACE_INFO0("hciCmdTimeout");
}

/*************************************************************************************************/
/*!
 *  \brief  Process an HCI Command Complete or Command Status event.
 *
 *  \param  numCmdPkts  Number of commands that can be sent to the controller.
 *
 *  \return None.
 */
/*************************************************************************************************/
void hciCmdRecvCmpl(uint8_t numCmdPkts)
{
  /* stop the command timeout timer */
  WsfTimerStop(&hciCmdCb.cmdTimer);

  /*
   * Set the number of commands that can be sent to the controller.  Setting this
   * to 1 rather than incrementing by numCmdPkts allows only one command at a time to
   * be sent to the controller and simplifies the code.
   */
  hciCmdCb.numCmdPkts = 1;

  /* send the next queued command */
  hciCmdSend(NULL);
}

/*************************************************************************************************/
/*!
 *  \brief  HCI disconnect command.
 */
/*************************************************************************************************/
void HciDisconnectCmd(uint16_t handle, uint8_t reason)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_DISCONNECT, HCI_LEN_DISCONNECT)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT8_TO_BSTREAM(p, reason);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE add device white list command.
 */
/*************************************************************************************************/
void HciLeAddDevWhiteListCmd(uint8_t addrType, uint8_t *pAddr)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ADD_DEV_WHITE_LIST, HCI_LEN_LE_ADD_DEV_WHITE_LIST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, addrType);
    BDA_TO_BSTREAM(p, pAddr);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE clear white list command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeClearWhiteListCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_CLEAR_WHITE_LIST, HCI_LEN_LE_CLEAR_WHITE_LIST)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI connection update command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeConnUpdateCmd(uint16_t handle, hciConnSpec_t *pConnSpec)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_CONN_UPDATE, HCI_LEN_LE_CONN_UPDATE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT16_TO_BSTREAM(p, pConnSpec->connIntervalMin);
    UINT16_TO_BSTREAM(p, pConnSpec->connIntervalMax);
    UINT16_TO_BSTREAM(p, pConnSpec->connLatency);
    UINT16_TO_BSTREAM(p, pConnSpec->supTimeout);
    UINT16_TO_BSTREAM(p, pConnSpec->minCeLen);
    UINT16_TO_BSTREAM(p, pConnSpec->maxCeLen);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE create connection command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeCreateConnCmd(uint16_t scanInterval, uint16_t scanWindow, uint8_t filterPolicy,
                        uint8_t peerAddrType, uint8_t *pPeerAddr, uint8_t ownAddrType,
                        hciConnSpec_t *pConnSpec)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_CREATE_CONN, HCI_LEN_LE_CREATE_CONN)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, scanInterval);
    UINT16_TO_BSTREAM(p, scanWindow);
    UINT8_TO_BSTREAM(p, filterPolicy);
    UINT8_TO_BSTREAM(p, peerAddrType);
    BDA_TO_BSTREAM(p, pPeerAddr);
    UINT8_TO_BSTREAM(p, ownAddrType);
    UINT16_TO_BSTREAM(p, pConnSpec->connIntervalMin);
    UINT16_TO_BSTREAM(p, pConnSpec->connIntervalMax);
    UINT16_TO_BSTREAM(p, pConnSpec->connLatency);
    UINT16_TO_BSTREAM(p, pConnSpec->supTimeout);
    UINT16_TO_BSTREAM(p, pConnSpec->minCeLen);
    UINT16_TO_BSTREAM(p, pConnSpec->maxCeLen);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE create connection cancel command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeCreateConnCancelCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_CREATE_CONN_CANCEL, HCI_LEN_LE_CREATE_CONN_CANCEL)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE remote connection parameter request reply command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeRemoteConnParamReqReply(uint16_t handle, uint16_t intervalMin, uint16_t intervalMax, uint16_t latency,
                                  uint16_t timeout, uint16_t minCeLen, uint16_t maxCeLen)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_REM_CONN_PARAM_REP, HCI_LEN_LE_REM_CONN_PARAM_REP)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT16_TO_BSTREAM(p, intervalMin);
    UINT16_TO_BSTREAM(p, intervalMax);
    UINT16_TO_BSTREAM(p, latency);
    UINT16_TO_BSTREAM(p, timeout);
    UINT16_TO_BSTREAM(p, minCeLen);
    UINT16_TO_BSTREAM(p, maxCeLen);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE remote connection parameter request negative reply command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeRemoteConnParamReqNegReply(uint16_t handle, uint8_t reason)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_REM_CONN_PARAM_NEG_REP, HCI_LEN_LE_REM_CONN_PARAM_NEG_REP)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT8_TO_BSTREAM(p, reason);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE set data len command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeSetDataLen(uint16_t handle, uint16_t txOctets, uint16_t txTime)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_DATA_LEN, HCI_LEN_LE_SET_DATA_LEN)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT16_TO_BSTREAM(p, txOctets);
    UINT16_TO_BSTREAM(p, txTime);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE read suggested default data len command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeReadDefDataLen(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_DEF_DATA_LEN, HCI_LEN_LE_READ_DEF_DATA_LEN)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE write suggested default data len command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeWriteDefDataLen(uint16_t suggestedMaxTxOctets, uint16_t suggestedMaxTxTime)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_WRITE_DEF_DATA_LEN, HCI_LEN_LE_WRITE_DEF_DATA_LEN)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, suggestedMaxTxOctets);
    UINT16_TO_BSTREAM(p, suggestedMaxTxTime);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE read local P-256 public key command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeReadLocalP256PubKey(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_LOCAL_P256_PUB_KEY, HCI_LEN_LE_READ_LOCAL_P256_PUB_KEY)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE generate DHKey command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeGenerateDHKey(uint8_t *pPubKeyX, uint8_t *pPubKeyY)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_GENERATE_DHKEY, HCI_LEN_LE_GENERATE_DHKEY)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pPubKeyX, HCI_DH_KEY_LEN);
    memcpy(p + HCI_DH_KEY_LEN, pPubKeyY, HCI_DH_KEY_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE Generate DH Key Version 2 command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeGenerateDHKeyV2(uint8_t *pPubKeyX, uint8_t *pPubKeyY, uint8_t keyType)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_GENERATE_DHKEY_V2, HCI_LEN_LE_GENERATE_DHKEY_V2)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pPubKeyX, HCI_DH_KEY_LEN);
    p += HCI_DH_KEY_LEN;
    memcpy(p, pPubKeyY, HCI_DH_KEY_LEN);
    p += HCI_DH_KEY_LEN;
    UINT8_TO_BSTREAM(p, keyType);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI LE read maximum data len command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciLeReadMaxDataLen(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_MAX_DATA_LEN, HCI_LEN_LE_READ_MAX_DATA_LEN)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE encrypt command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeEncryptCmd(uint8_t *pKey, uint8_t *pData)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ENCRYPT, HCI_LEN_LE_ENCRYPT)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pKey, HCI_KEY_LEN);
    p += HCI_KEY_LEN;
    memcpy(p, pData, HCI_ENCRYPT_DATA_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE long term key request negative reply command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeLtkReqNegReplCmd(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_LTK_REQ_NEG_REPL, HCI_LEN_LE_LTK_REQ_NEG_REPL)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE long term key request reply command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeLtkReqReplCmd(uint16_t handle, uint8_t *pKey)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_LTK_REQ_REPL, HCI_LEN_LE_LTK_REQ_REPL)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    memcpy(p, pKey, HCI_KEY_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE random command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeRandCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_RAND, HCI_LEN_LE_RAND)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read advertising TX power command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadAdvTXPowerCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_ADV_TX_POWER, HCI_LEN_LE_READ_ADV_TX_POWER)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read buffer size command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadBufSizeCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_BUF_SIZE, HCI_LEN_LE_READ_BUF_SIZE)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read channel map command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadChanMapCmd(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_CHAN_MAP, HCI_LEN_LE_READ_CHAN_MAP)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read local supported feautre command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadLocalSupFeatCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT, HCI_LEN_LE_READ_LOCAL_SUP_FEAT)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read remote feature command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadRemoteFeatCmd(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_REMOTE_FEAT, HCI_LEN_LE_READ_REMOTE_FEAT)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read supported states command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadSupStatesCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_SUP_STATES, HCI_LEN_LE_READ_SUP_STATES)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE read white list size command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadWhiteListSizeCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_WHITE_LIST_SIZE, HCI_LEN_LE_READ_WHITE_LIST_SIZE)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE remove device white list command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeRemoveDevWhiteListCmd(uint8_t addrType, uint8_t *pAddr)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_REMOVE_DEV_WHITE_LIST, HCI_LEN_LE_REMOVE_DEV_WHITE_LIST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, addrType);
    BDA_TO_BSTREAM(p, pAddr);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE set advanced enable command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetAdvEnableCmd(uint8_t enable)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_ADV_ENABLE, HCI_LEN_LE_SET_ADV_ENABLE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, enable);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE set advertising data command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetAdvDataCmd(uint8_t len, uint8_t *pData)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_ADV_DATA, HCI_LEN_LE_SET_ADV_DATA)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, len);
    memcpy(p, pData, len);
    p += len;
    memset(p, 0, (HCI_ADV_DATA_LEN - len));
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE set advertising parameters command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetAdvParamCmd(uint16_t advIntervalMin, uint16_t advIntervalMax, uint8_t advType,
                         uint8_t ownAddrType, uint8_t peerAddrType, uint8_t *pPeerAddr,
                         uint8_t advChanMap, uint8_t advFiltPolicy)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_ADV_PARAM, HCI_LEN_LE_SET_ADV_PARAM)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, advIntervalMin);
    UINT16_TO_BSTREAM(p, advIntervalMax);
    UINT8_TO_BSTREAM(p, advType);
    UINT8_TO_BSTREAM(p, ownAddrType);
    UINT8_TO_BSTREAM(p, peerAddrType);
    if (pPeerAddr != NULL)
    {
      BDA_TO_BSTREAM(p, pPeerAddr);
    }
    else
    {
      p = BdaClr(p);
    }

    /* Note: p cannot equal NULL here. */
    /* coverity[dereference] */
    UINT8_TO_BSTREAM(p, advChanMap);
    UINT8_TO_BSTREAM(p, advFiltPolicy);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE set event mask command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetEventMaskCmd(uint8_t *pLeEventMask)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_EVENT_MASK, HCI_LEN_LE_SET_EVENT_MASK)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pLeEventMask, HCI_LE_EVT_MASK_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI set host channel class command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetHostChanClassCmd(uint8_t *pChanMap)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_HOST_CHAN_CLASS, HCI_LEN_LE_SET_HOST_CHAN_CLASS)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pChanMap, HCI_CHAN_MAP_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE set random address command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetRandAddrCmd(uint8_t *pAddr)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_RAND_ADDR, HCI_LEN_LE_SET_RAND_ADDR)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    BDA_TO_BSTREAM(p, pAddr);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE set scan enable command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetScanEnableCmd(uint8_t enable, uint8_t filterDup)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_SCAN_ENABLE, HCI_LEN_LE_SET_SCAN_ENABLE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, enable);
    UINT8_TO_BSTREAM(p, filterDup);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI set scan parameters command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetScanParamCmd(uint8_t scanType, uint16_t scanInterval, uint16_t scanWindow,
                          uint8_t ownAddrType, uint8_t scanFiltPolicy)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_SCAN_PARAM, HCI_LEN_LE_SET_SCAN_PARAM)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, scanType);
    UINT16_TO_BSTREAM(p, scanInterval);
    UINT16_TO_BSTREAM(p, scanWindow);
    UINT8_TO_BSTREAM(p, ownAddrType);
    UINT8_TO_BSTREAM(p, scanFiltPolicy);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE set scan response data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetScanRespDataCmd(uint8_t len, uint8_t *pData)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_SCAN_RESP_DATA, HCI_LEN_LE_SET_SCAN_RESP_DATA)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, len);
    memcpy(p, pData, len);
    p += len;
    memset(p, 0, (HCI_SCAN_DATA_LEN - len));
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI LE start encryption command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeStartEncryptionCmd(uint16_t handle, uint8_t *pRand, uint16_t diversifier, uint8_t *pKey)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_START_ENCRYPTION, HCI_LEN_LE_START_ENCRYPTION)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    memcpy(p, pRand, HCI_RAND_LEN);
    p += HCI_RAND_LEN;
    UINT16_TO_BSTREAM(p, diversifier);
    memcpy(p, pKey, HCI_KEY_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read BD address command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciReadBdAddrCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_BD_ADDR, HCI_LEN_READ_BD_ADDR)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read buffer size command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciReadBufSizeCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_BUF_SIZE, HCI_LEN_READ_BUF_SIZE)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read local supported feature command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciReadLocalSupFeatCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_LOCAL_SUP_FEAT, HCI_LEN_READ_LOCAL_SUP_FEAT)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read local version info command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciReadLocalVerInfoCmd(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_LOCAL_VER_INFO, HCI_LEN_READ_LOCAL_VER_INFO)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read remote version info command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciReadRemoteVerInfoCmd(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_REMOTE_VER_INFO, HCI_LEN_READ_REMOTE_VER_INFO)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read RSSI command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciReadRssiCmd(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_RSSI, HCI_LEN_READ_RSSI)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read Tx power level command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciReadTxPwrLvlCmd(uint16_t handle, uint8_t type)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_TX_PWR_LVL, HCI_LEN_READ_TX_PWR_LVL)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT8_TO_BSTREAM(p, type);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI reset command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciResetCmd(void)
{
  uint8_t *pBuf;

  /* initialize numCmdPkts for special case of reset command */
  hciCmdCb.numCmdPkts = 1;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_RESET, HCI_LEN_RESET)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI set event mask command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciSetEventMaskCmd(uint8_t *pEventMask)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_SET_EVENT_MASK, HCI_LEN_SET_EVENT_MASK)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pEventMask, HCI_EVT_MASK_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI set event mask page 2 command.
*
*  \return None.
*/
/*************************************************************************************************/
void HciSetEventMaskPage2Cmd(uint8_t *pEventMask)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_SET_EVENT_MASK_PAGE2, HCI_LEN_SET_EVENT_MASK_PAGE2)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pEventMask, HCI_EVT_MASK_PAGE_2_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI read authenticated payload timeout command.
*
*  \param  handle    Connection handle.
*
*  \return None.
*/
/*************************************************************************************************/
void HciReadAuthPayloadTimeout(uint16_t handle)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_READ_AUTH_PAYLOAD_TO, HCI_LEN_READ_AUTH_PAYLOAD_TO)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
*  \brief  HCI write authenticated payload timeout command.
*
*  \param  handle    Connection handle.
*  \param  timeout   Timeout value.
*
*  \return None.
*/
/*************************************************************************************************/
void HciWriteAuthPayloadTimeout(uint16_t handle, uint16_t timeout)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_WRITE_AUTH_PAYLOAD_TO, HCI_LEN_WRITE_AUTH_PAYLOAD_TO)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, handle);
    UINT16_TO_BSTREAM(p, timeout);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI add device to resolving list command.
 *
 *  \param  peerAddrType        Peer identity address type.
 *  \param  pPeerIdentityAddr   Peer identity address.
 *  \param  pPeerIrk            Peer IRK.
 *  \param  pLocalIrk           Local IRK.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeAddDeviceToResolvingListCmd(uint8_t peerAddrType, const uint8_t *pPeerIdentityAddr,
                                      const uint8_t *pPeerIrk, const uint8_t *pLocalIrk)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_ADD_DEV_RES_LIST, HCI_LEN_LE_ADD_DEV_RES_LIST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, peerAddrType);
    BDA_TO_BSTREAM(p, pPeerIdentityAddr);
    memcpy(p, pPeerIrk, HCI_KEY_LEN);
    p += HCI_KEY_LEN;
    memcpy(p, pLocalIrk, HCI_KEY_LEN);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI remove device from resolving list command.
 *
 *  \param  peerAddrType        Peer identity address type.
 *  \param  pPeerIdentityAddr   Peer identity address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeRemoveDeviceFromResolvingList(uint8_t peerAddrType, const uint8_t *pPeerIdentityAddr)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_REMOVE_DEV_RES_LIST, HCI_LEN_LE_REMOVE_DEV_RES_LIST)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, peerAddrType);
    BDA_TO_BSTREAM(p, pPeerIdentityAddr);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI clear resolving list command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeClearResolvingList(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_CLEAR_RES_LIST, HCI_LEN_LE_CLEAR_RES_LIST)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read resolving list command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadResolvingListSize(void)
{
  uint8_t *pBuf;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_RES_LIST_SIZE, HCI_LEN_LE_READ_RES_LIST_SIZE)) != NULL)
  {
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read peer resolvable address command.
 *
 *  \param  addrType        Peer identity address type.
 *  \param  pIdentityAddr   Peer identity address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadPeerResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_PEER_RES_ADDR, HCI_LEN_LE_READ_PEER_RES_ADDR)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, addrType);
    BDA_TO_BSTREAM(p, pIdentityAddr);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read local resolvable address command.
 *
 *  \param  addrType        Peer identity address type.
 *  \param  pIdentityAddr   Peer identity address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeReadLocalResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_READ_LOCAL_RES_ADDR, HCI_LEN_LE_READ_LOCAL_RES_ADDR)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, addrType);
    BDA_TO_BSTREAM(p, pIdentityAddr);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI enable or disable address resolution command.
 *
 *  \param  enable      Set to TRUE to enable address resolution or FALSE to disable address
 *                      resolution.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetAddrResolutionEnable(uint8_t enable)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_ADDR_RES_ENABLE, HCI_LEN_LE_SET_ADDR_RES_ENABLE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, enable);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI set resolvable private address timeout command.
 *
 *  \param  rpaTimeout    Timeout measured in seconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciLeSetResolvablePrivateAddrTimeout(uint16_t rpaTimeout)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_RES_PRIV_ADDR_TO, HCI_LEN_LE_SET_RES_PRIV_ADDR_TO)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT16_TO_BSTREAM(p, rpaTimeout);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      HCI LE set privacy mode command.
 *
 *  \param      peerAddrType    Peer identity address type.
 *  \param      pPeerAddr       Peer identity address.
 *  \param      mode            Privacy mode.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void HciLeSetPrivacyModeCmd(uint8_t addrType, uint8_t *pAddr, uint8_t mode)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(HCI_OPCODE_LE_SET_PRIVACY_MODE, HCI_LEN_LE_SET_PRIVACY_MODE)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    UINT8_TO_BSTREAM(p, addrType);
    BDA_TO_BSTREAM(p, pAddr);
    UINT8_TO_BSTREAM(p, mode);
    hciCmdSend(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI vencor specific command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HciVendorSpecificCmd(uint16_t opcode, uint8_t len, uint8_t *pData)
{
  uint8_t *pBuf;
  uint8_t *p;

  if ((pBuf = hciCmdAlloc(opcode, len)) != NULL)
  {
    p = pBuf + HCI_CMD_HDR_LEN;
    memcpy(p, pData, len);
    hciCmdSend(pBuf);
  }
}
