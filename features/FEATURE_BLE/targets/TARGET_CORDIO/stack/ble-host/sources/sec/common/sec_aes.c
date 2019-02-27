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
 *  \brief AES and random number security service implemented using HCI.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_queue.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "sec_api.h"
#include "sec_main.h"
#include "hci_api.h"
#include "util/calc128.h"

/**************************************************************************************************
  External Variables
**************************************************************************************************/

extern secCb_t secCb;

/*************************************************************************************************/
/*!
 *  \brief  Returns the next token.
 *
 *  \return Token value.
 */
/*************************************************************************************************/
static uint8_t getNextToken()
{
  uint8_t token = secCb.token++;

  if (token == SEC_TOKEN_INVALID)
  {
    token = secCb.token++;
  }

  return token;
}

/*************************************************************************************************/
/*!
 *  \brief  Execute an AES calculation.  When the calculation completes, a WSF message will be
 *          sent to the specified handler.  This function returns a token value that
 *          the client can use to match calls to this function with messages.
 *
 *  \param  pKey        Pointer to 16 byte key.
 *  \param  pPlaintext  Pointer to 16 byte plaintext.
 *  \param  handlerId   WSF handler ID.
 *  \param  param       Client-defined parameter returned in message.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return Token value.
 */
/*************************************************************************************************/
uint8_t SecAes(uint8_t *pKey, uint8_t *pPlaintext, wsfHandlerId_t handlerId,
               uint16_t param, uint8_t event)
{
  secQueueBuf_t  *pBuf;

  /* allocate a buffer */
  if ((pBuf = WsfMsgAlloc(sizeof(secQueueBuf_t))) != NULL)
  {
    pBuf->msg.hdr.status = getNextToken();
    pBuf->msg.hdr.param = param;
    pBuf->msg.hdr.event = event;

    pBuf->type = SEC_TYPE_AES;

    /* queue buffer */
    WsfMsgEnq(&secCb.aesEncQueue, handlerId, pBuf);

    /* call HCI encrypt function */
    HciLeEncryptCmd(pKey, pPlaintext);

    return pBuf->msg.hdr.status;
  }

  return SEC_TOKEN_INVALID;
}

/*************************************************************************************************/
/*!
 *  \brief  Callback for HCI encryption for AES operations.
 *
 *  \param  pBuf        Pointer to sec queue element.
 *  \param  pEvent      Pointer to HCI event.
 *  \param  handlerId   WSF handler ID.
 *
 *  \return none.
 */
/*************************************************************************************************/
void SecAesHciCback(secQueueBuf_t *pBuf, hciEvt_t *pEvent, wsfHandlerId_t handlerId)
{
  secAes_t *pAes = (secAes_t *) &pBuf->msg;

  /* set encrypted data pointer and copy */
  pAes->pCiphertext = pBuf->ciphertext;
  Calc128Cpy(pAes->pCiphertext, pEvent->leEncryptCmdCmpl.data);

  /* send message */
  WsfMsgSend(handlerId, pAes);
}

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize AES secuirity.
 *
 *  \param  none.
 *
 *  \return none.
 */
/*************************************************************************************************/
void SecAesInit()
{
  secCb.hciCbackTbl[SEC_TYPE_AES] = SecAesHciCback;
}
