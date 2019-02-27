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
#include "wsf_buf.h"
#include "hci_api.h"
#include "util/calc128.h"
#include "util/wstr.h"

#ifndef SEC_CMAC_CFG
#define SEC_CMAC_CFG SEC_CMAC_CFG_HCI
#endif

#if SEC_CMAC_CFG == SEC_CMAC_CFG_HCI

enum
{
  SEC_CMAC_STATE_SUBKEY,
  SEC_CMAC_STATE_BLOCK,
  SEC_CMAC_STATE_COMPLETE,
};

/**************************************************************************************************
  External Variables
**************************************************************************************************/

extern secCb_t secCb;

/*************************************************************************************************/
/*!
 *  \brief  Continue the execution of the CMAC algorithm over the next message block.
 *
 *  \param  pBuf    Security queue buffer containing CMAC algorithm control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCmacProcessBlock(secQueueBuf_t *pBuf)
{
  secCmacSecCb_t *pCmac = (secCmacSecCb_t*) pBuf->pCb;
  uint8_t text[SEC_BLOCK_LEN];
  uint8_t *pMn = pCmac->pPlainText + pCmac->position;
  int16_t remaining = (int16_t) pCmac->len - pCmac->position;

  /* Check for Last Block */
  if (remaining <= SEC_BLOCK_LEN)
  {
    memcpy(text, pMn, remaining);

    /* Pad the message if necessary */
    if (remaining != SEC_BLOCK_LEN)
    {
      memset(text + remaining, 0, SEC_BLOCK_LEN - remaining);
      text[remaining] = 0x80;
    }

    /* XOr the subkey */
    Calc128Xor(text, pCmac->subkey);
    pCmac->state = SEC_CMAC_STATE_COMPLETE;
  }
  else
  {
    /* Copy the block to the buffer */
    Calc128Cpy(text, pMn);
  }

  if (pCmac->position != 0)
  {
    /* Except for first block, XOr the previous AES calculation */
    Calc128Xor(text, pBuf->ciphertext);
  }

  pCmac->position += SEC_BLOCK_LEN;

  SecLeEncryptCmd(pCmac->key, text, pBuf, pCmac->handlerId);
}

/*************************************************************************************************/
/*!
 *  \brief  Step 1 to generate the subkey used in the CMAC algorithm.
 *
 *  \param  pBuf    Security queue buffer containing CMAC algorithm control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCmacGenSubkey1(secQueueBuf_t *pBuf)
{
  secCmacSecCb_t *pCmac = (secCmacSecCb_t*) pBuf->pCb;
  uint8_t buf[SEC_BLOCK_LEN];

  /* Perform aes on the key with a constant zero */
  memset(buf, 0, SEC_BLOCK_LEN);

  SecLeEncryptCmd(pCmac->key, buf, pBuf, pCmac->handlerId);
}

/*************************************************************************************************/
/*!
 *  \brief  Left shift a buffer of WSF_CMAC_KEY_LEN bytes by N bits.
 *
 *  \param  pBuf    Buffer to left shift.
 *  \param  shift   Number of bits to shift.
 *
 *  \return The overflow of the operaiton.
 */
/*************************************************************************************************/
static uint8_t secCmacKeyShift(uint8_t *pBuf, uint8_t shift)
{
  uint8_t overflow, i;
  uint8_t finalOverflow = pBuf[0] >> (8 - shift);

  for (i = 0; i < SEC_CMAC_KEY_LEN; i++)
  {
    /* store shifted bits for next byte */
    if (i < SEC_CMAC_KEY_LEN-1)
    {
      overflow = pBuf[i+1] >> (8 - shift);
    }
    else
    {
      overflow = 0;
    }

    /* shift byte and OR in shifted bits from previous byte */
    pBuf[i] = (pBuf[i] << shift) | overflow;
  }

  return finalOverflow;
}

/*************************************************************************************************/
/*!
 *  \brief  Complete generation of the subkey used in the CMAC algorithm.
 *
 *  \param  pBuf    Security queue buffer containing CMAC algorithm control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCmacGenSubkey2(secQueueBuf_t *pBuf)
{
  secCmacSecCb_t *pCmac = (secCmacSecCb_t*) pBuf->pCb;
  uint8_t overflow;

  /* Copy the result of the AES oepration */
  Calc128Cpy(pCmac->subkey, pBuf->ciphertext);

  /* Calculate the K1 subkey */
  overflow = secCmacKeyShift(pCmac->subkey, 1);

  if (overflow)
  {
    pCmac->subkey[SEC_BLOCK_LEN-1] ^= SEC_CMAC_RB;
  }

  if (pCmac->len % SEC_BLOCK_LEN != 0)
  {
    /* If the message len is not a multiple of SEC_BLOCK_LEN */
    /* Continue with generation of the K2 subkey based on the K1 key */
    overflow = secCmacKeyShift(pCmac->subkey, 1);

    if (overflow)
    {
      pCmac->subkey[SEC_BLOCK_LEN-1] ^= SEC_CMAC_RB;
    }
  }

  /* Begin CMAC calculation */
  pCmac->state = SEC_CMAC_STATE_BLOCK;
  secCmacProcessBlock(pBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  Send a message to the handler with CMAC result.
 *
 *  \param  pBuf    Security queue buffer containing CMAC algorithm control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCmacComplete(secQueueBuf_t *pBuf)
{
  /* CMAC is complete, copy and send result to handler */
  secCmacMsg_t *pMsg = (secCmacMsg_t *) &pBuf->msg;
  secCmacSecCb_t *pCmac = (secCmacSecCb_t *) pBuf->pCb;

  pMsg->pCiphertext = pBuf->ciphertext;
  pMsg->pPlainText = pCmac->pPlainText;

  WsfMsgSend(pCmac->handlerId, pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Callback for HCI encryption for CMAC operations.
 *
 *  \param  pBuf        Pointer to sec queue element.
 *  \param  pEvent      Pointer to HCI event.
 *  \param  handlerId   WSF handler ID.
 *
 *  \return none.
 */
/*************************************************************************************************/
void SecCmacHciCback(secQueueBuf_t *pBuf, hciEvt_t *pEvent, wsfHandlerId_t handlerId)
{
  secCmacSecCb_t *pCmac = (secCmacSecCb_t *) pBuf->pCb;

  if (pCmac)
  {
    Calc128Cpy(pBuf->ciphertext, pEvent->leEncryptCmdCmpl.data);

    switch (pCmac->state)
    {
    case SEC_CMAC_STATE_SUBKEY:
      secCmacGenSubkey2(pBuf);
      break;

    case SEC_CMAC_STATE_BLOCK:
      secCmacProcessBlock(pBuf);
      break;

    case SEC_CMAC_STATE_COMPLETE:
      secCmacComplete(pBuf);
      break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Execute the CMAC algorithm.
 *
 *  \param  pKey          Key used in CMAC operation.
 *  \param  pPlainText    Data to perform CMAC operation over
 *  \param  len           Size of pPlaintext in bytes.
 *  \param  handlerId     WSF handler ID for client.
 *  \param  param         Optional parameter sent to client's WSF handler.
 *  \param  event         Event for client's WSF handler.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecCmac(const uint8_t *pKey, uint8_t *pPlainText, uint16_t textLen, wsfHandlerId_t handlerId,
               uint16_t param, uint8_t event)
{
  secQueueBuf_t *pBuf;
  uint16_t bufSize = sizeof(secQueueBuf_t) + sizeof(secCmacSecCb_t);

  if ((pBuf = WsfMsgAlloc(bufSize)) != NULL)
  {
    secCmacSecCb_t *pCmacCb = (secCmacSecCb_t *) (pBuf + 1);

    /* Setup queue buffer */
    pBuf->pCb = pCmacCb;
    pBuf->type = SEC_TYPE_CMAC;

    pBuf->msg.hdr.status = secCb.token++;
    pBuf->msg.hdr.param = param;
    pBuf->msg.hdr.event = event;

    pCmacCb->pPlainText = pPlainText;

    pCmacCb->len = textLen;
    pCmacCb->position = 0;
    pCmacCb->handlerId = handlerId;
    pCmacCb->state = SEC_CMAC_STATE_SUBKEY;

    /* Copy key */
    Calc128Cpy(pCmacCb->key, (uint8_t *) pKey);

    /* Start the CMAC process by calculating the subkey */
    secCmacGenSubkey1(pBuf);

    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize CMAC security.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecCmacInit()
{
  secCb.hciCbackTbl[SEC_TYPE_CMAC] = SecCmacHciCback;
}

#endif /* SEC_CMAC_CFG */
