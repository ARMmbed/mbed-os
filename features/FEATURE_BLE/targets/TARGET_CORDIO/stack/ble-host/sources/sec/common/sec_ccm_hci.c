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
 *  \brief Counter with CBC-MAC (CCM) mode security - HCI AES.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_queue.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "sec_api.h"
#include "sec_main.h"
#include "wsf_buf.h"
#include "hci_api.h"
#include "util/calc128.h"
#include "util/wstr.h"

#ifndef SEC_CCM_CFG
#define SEC_CCM_CFG SEC_CCM_CFG_HCI
#endif

#if SEC_CCM_CFG == SEC_CCM_CFG_HCI

/**************************************************************************************************
  Constants
**************************************************************************************************/

/* State machine states */
enum
{
  SEC_CCM_STATE_XI_HDR,
  SEC_CCM_STATE_XI_MSG,
  SEC_CCM_STATE_S0,
  SEC_CCM_STATE_SI,
  SEC_CCM_STATE_MIC_COMPLETE,
};

/**************************************************************************************************
  External Variables
**************************************************************************************************/

/* Global security control block */
extern secCb_t secCb;

/*************************************************************************************************/
/*!
 *  \brief  Exclusive-or two 128-bit integers and return the result in pDst.
 *
 *  \param  pDst    Pointer to destination.
 *  \param  pSrc    Pointer to source.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmCalcXor(uint8_t *pDst, uint8_t *pSrc, uint8_t size)
{
  uint8_t i;

  for (i = 0; i < size; i++)
  {
    *pDst++ ^= *pSrc++;
  }
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmBlockEncrypt
 *
 *  \brief  Perform a 16-byte block encryption (HCI AES)
 *
 *  \param  pBuf    Security queue buffer containing CCM algorithm control block.
 *  \param  pText   Pointer to text to encrypt (16 bytes).
  *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmBlockEncrypt(secQueueBuf_t *pBuf, uint8_t *pText)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;
  SecLeEncryptCmd(pCcm->key, pText, pBuf, pCcm->handlerId);
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmGenX0
 *
 *  \brief  Generate X_0 := E(K, B_0)
 *
 *  \param  pBuf    Security queue buffer containing CCM algorithm control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmGenX0(secQueueBuf_t *pBuf)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;

  /* Scratch buffer contains nonce, add flags and message length */
  pCcm->scratch[0] = (SEC_CCM_L - 1) | (((pCcm->micLen - 2) / 2) << 3) | ((pCcm->clearLen > 0? 1:0) << 6);
  pCcm->scratch[SEC_BLOCK_LEN - 2] = pCcm->textLen >> 8;
  pCcm->scratch[SEC_BLOCK_LEN - 1] = pCcm->textLen & 0xFF;

  pCcm->state = pCcm->clearLen > 0 ? SEC_CCM_STATE_XI_HDR : SEC_CCM_STATE_XI_MSG;
  pCcm->position = 0;

  /* AES Operation */
  secCcmBlockEncrypt(pBuf, pCcm->scratch);
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmGenXiHdr
 *
 *  \brief  Generate X_i header if clear text (additional data) is present.
 *
 *  \param  pBuf      Security queue buffer containing CCM algorithm control block.
 *  \param  pPriorX   16 byte buffer containing X_i-1.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmGenXiHdr(secQueueBuf_t *pBuf, uint8_t *pPriorX)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;
  uint8_t b_i[SEC_BLOCK_LEN];
  uint16_t remaining;
  uint16_t offset = 0;

  if (pCcm->position == 0)
  {
    /* Copy additional data into working buffer */
    memcpy(pCcm->pWorking, pCcm->pClear, pCcm->clearLen);

    /* First two bytes of b_0 contain length of additional data */
    b_i[0] = pCcm->clearLen >> 8;
    b_i[1] = pCcm->clearLen & 0xFF;
    pCcm->position = offset = 2;
  }

  remaining = (int16_t) pCcm->clearLen - pCcm->position + 2;

  /* Copy additional to b_i */
  if (remaining >= SEC_BLOCK_LEN - offset)
  {
    memcpy(b_i + offset, pCcm->pClear + pCcm->position - 2, SEC_BLOCK_LEN - offset);
    pCcm->position += SEC_BLOCK_LEN - offset;

    if (remaining == SEC_BLOCK_LEN - offset)
    {
      pCcm->state = SEC_CCM_STATE_XI_MSG;
      pCcm->position = 0;
    }
  }
  else
  {
    memcpy(b_i + offset, pCcm->pClear + pCcm->position - 2, remaining);
    memset(b_i + offset + remaining, 0, SEC_BLOCK_LEN - remaining - offset);
    pCcm->state = SEC_CCM_STATE_XI_MSG;
    pCcm->position = 0;
  }

  /* X_i XOR B_i */
  Calc128Xor(b_i, pPriorX);

  /* AES Operation */
  secCcmBlockEncrypt(pBuf, b_i);
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmGenXiMsg
 *
 *  \brief  Generate X_i for the message text.
 *
 *  \param  pBuf      Security queue buffer containing CCM algorithm control block.
 *  \param  pPriorX   16 byte buffer containing X_i-1.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmGenXiMsg(secQueueBuf_t *pBuf, uint8_t *pPriorX)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;
  uint8_t b_i[SEC_BLOCK_LEN];
  uint16_t remaining = (int16_t) pCcm->textLen - pCcm->position;

  /* Copy data to b_i */
  if (remaining >= SEC_BLOCK_LEN)
  {
    Calc128Cpy(b_i, pCcm->pText + pCcm->position);
    pCcm->position += SEC_BLOCK_LEN;

    if (remaining == SEC_BLOCK_LEN)
    {
      pCcm->state = SEC_CCM_STATE_S0;
    }
  }
  else
  {
    memcpy(b_i, pCcm->pText + pCcm->position, remaining);
    memset(b_i + remaining, 0, SEC_BLOCK_LEN - remaining);
    pCcm->state = SEC_CCM_STATE_S0;
  }

  /* X_i XOR B_i */
  Calc128Xor(b_i, pPriorX);

  /* AES Operation */
  secCcmBlockEncrypt(pBuf, b_i);
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmGenS0
 *
 *  \brief  Generate S_0 := E(K, A_0)
 *
 *  \param  pBuf    Security queue buffer containing CCM algorithm control block.
 *  \param  x_n     16 byte buffer containing X_n (containing T).
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmGenS0(secQueueBuf_t *pBuf, uint8_t *x_n)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;
  uint16_t offset = pCcm->textLen + pCcm->clearLen;

  /* Copy T to working buffer */
  memcpy(pCcm->pWorking + offset, x_n, pCcm->micLen);

  /* Scratch buffer contains nonce, add flags and counter */
  pCcm->scratch[0] = (SEC_CCM_L - 1);
  pCcm->scratch[SEC_BLOCK_LEN - 2] = pCcm->scratch[SEC_BLOCK_LEN - 1] = 0;

  pCcm->state = SEC_CCM_STATE_MIC_COMPLETE;

  /* AES Operation */
  secCcmBlockEncrypt(pBuf, pCcm->scratch);
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmGenS1
 *
 *  \brief  Generate S_1 := E(K, A_1)
 *
 *  \param  pBuf    Security queue buffer containing CCM algorithm control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmGenS1(secQueueBuf_t *pBuf)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;

  /* Set counter. */
  pCcm->counter = 1;

  /* Scratch buffer contains nonce, add flags and counter. */
  pCcm->scratch[0] = (SEC_CCM_L - 1);
  pCcm->scratch[SEC_BLOCK_LEN - 2] = 0;
  pCcm->scratch[SEC_BLOCK_LEN - 1] = 1;

  /* Change state to S_i state. */
  pCcm->state = SEC_CCM_STATE_SI;

  /* AES Operation. */
  secCcmBlockEncrypt(pBuf, pCcm->scratch);
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmMicComplete
 *
 *  \brief  Called when MIC calculation is complete.
 *
 *  \param  pBuf    Security queue buffer containing CCM algorithm control block.
 *  \param  s_0     16 byte buffer containing S_0.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmMicComplete(secQueueBuf_t *pBuf, uint8_t *s_0)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;
  int16_t micOffset;

  /* MIC = s_0 XOR T (store in result buffer). */
  micOffset = pCcm->textLen + pCcm->clearLen;
  secCcmCalcXor(pCcm->pWorking + micOffset, s_0, pCcm->micLen);

  if (pCcm->operation == SEC_CCM_OP_ENCRYPT)
  {
    /* When encrypting, continue S_i calculations */
    secCcmGenS1(pBuf);
  }
  else
  {
    /* Decryption complete.  Send notification. */
    secCcmDecMsg_t *pMsg = (secCcmDecMsg_t *) &pBuf->msg;

    /* Verify MIC value */
    if (memcmp(pCcm->pRcvMic, pCcm->pWorking + micOffset, pCcm->micLen) == 0)
    {
      pMsg->pText = pCcm->pWorking + pCcm->clearLen;
      pMsg->textLen = pCcm->textLen;
      pMsg->success = TRUE;
    }
    else
    {
      pMsg->pText = NULL;
      pMsg->textLen = 0;
      pMsg->success = FALSE;
    }

    WsfMsgSend(pCcm->handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \fn     secCcmGenSi
 *
 *  \brief  Generate S_i := E(K, A_i)
 *
 *  \param  pBuf      Security queue buffer containing CCM algorithm control block.
 *  \param  pPriorS   16 byte buffer containing S_i-1.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secCcmGenSi(secQueueBuf_t *pBuf, uint8_t *pPriorS)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t*) pBuf->pCb;
  int16_t resultOffset = 0;
  uint16_t len;

  /* Determine length of XOR operation. */
  len = pCcm->textLen - ((pCcm->counter - 1) * SEC_BLOCK_LEN);
  len = len > SEC_BLOCK_LEN? SEC_BLOCK_LEN : len;

  /* m_i XOR s_i+1. */
  resultOffset = (pCcm->counter - 1) * SEC_BLOCK_LEN + pCcm->clearLen;

  secCcmCalcXor(pCcm->pWorking + resultOffset, pPriorS, (uint8_t) len);

  if (pCcm->counter * SEC_BLOCK_LEN >= pCcm->textLen)
  {
    if (pCcm->operation == SEC_CCM_OP_ENCRYPT)
    {
      /* Encription complete.  Send notification. */
      secCcmEncMsg_t *pMsg = (secCcmEncMsg_t *) &pBuf->msg;

      pMsg->pCiphertext = pCcm->pWorking;
      pMsg->textLen = pCcm->textLen + pCcm->clearLen + pCcm->micLen;
      WsfMsgSend(pCcm->handlerId, pMsg);
    }
    else
    {
      /* Set pText to point to the decrypted result in pWorking */
      pCcm->pText = pCcm->pWorking + pCcm->clearLen;

      /* Begin calculating the MIC */
      secCcmGenX0(pBuf);
    }
  }
  else
  {
    /* Update counter. */
    pCcm->counter++;
    pCcm->scratch[SEC_BLOCK_LEN - 2] = pCcm->counter  >> 8;
    pCcm->scratch[SEC_BLOCK_LEN - 1] = pCcm->counter  & 0xFF;

    /* AES Operation. */
    secCcmBlockEncrypt(pBuf, pCcm->scratch);
  }
}

/*************************************************************************************************/
/*!
 *  \fn     SecCcmBlockEncryptCmpl
 *
 *  \brief  Called when a block encryption operation completes.
 *
 *  \param  pParam          Pointer to security control block.
 *  \param  pCypherText     Pointer to encrypt result.
 *
 *  \return none.
 */
/*************************************************************************************************/
void SecCcmHciCback(secQueueBuf_t *pBuf, hciEvt_t *pEvent, wsfHandlerId_t handlerId)
{
  secCcmSecCb_t *pCcm = (secCcmSecCb_t *) pBuf->pCb;

  switch (pCcm->state)
  {
  case SEC_CCM_STATE_XI_HDR:
    secCcmGenXiHdr(pBuf, pEvent->leEncryptCmdCmpl.data);
    break;

  case SEC_CCM_STATE_XI_MSG:
    secCcmGenXiMsg(pBuf, pEvent->leEncryptCmdCmpl.data);
    break;

  case SEC_CCM_STATE_S0:
    secCcmGenS0(pBuf, pEvent->leEncryptCmdCmpl.data);
    break;

  case SEC_CCM_STATE_SI:
    secCcmGenSi(pBuf, pEvent->leEncryptCmdCmpl.data);
    break;

  case SEC_CCM_STATE_MIC_COMPLETE:
    secCcmMicComplete(pBuf, pEvent->leEncryptCmdCmpl.data);
    break;
  }
}

/*************************************************************************************************/
/*!
 *  \fn     SecCcmEnc
 *
 *  \brief  Execute the CCM-Mode encryption algorithm.
 *
 *  \param  pKey          Pointer to encryption key (SEC_CCM_KEY_LEN bytes).
 *  \param  pNonce        Pointer to nonce (SEC_CCM_NONCE_LEN bytes).
 *  \param  pPlainText    Pointer to text to encrypt.
 *  \param  textLen       Length of pPlainText in bytes.
 *  \param  pClear        Pointer to additional, unencrypted authentication text.
 *  \param  clearLen      Length of pClear in bytes.
 *  \param  micLen        Size of MIC in bytes (4, 8 or 16).
 *  \param  pResult       Buffer to hold result (returned in complete event).
 *  \param  handlerId     Task handler ID to receive complete event.
 *  \param  param         Optional parameter passed in complete event.
 *  \param  event         Event ID of complete event.

 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecCcmEnc(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pPlainText, uint16_t textLen,
                 uint8_t *pClear, uint16_t clearLen, uint8_t micLen, uint8_t *pResult,
                 wsfHandlerId_t handlerId, uint16_t param, uint8_t event)
{
  secQueueBuf_t *pBuf;
  uint16_t bufSize = sizeof(secQueueBuf_t) + sizeof(secCcmSecCb_t);

  WSF_ASSERT(clearLen < SEC_CCM_MAX_ADDITIONAL_LEN);

  if ((pBuf = WsfMsgAlloc(bufSize)) != NULL)
  {
    secCcmSecCb_t *pCcm = (secCcmSecCb_t *) (pBuf + 1);

    /* Setup queue buffer */
    pBuf->pCb = pCcm;
    pBuf->type = SEC_TYPE_CCM;

    pBuf->msg.hdr.status = secCb.token++;
    pBuf->msg.hdr.param = param;
    pBuf->msg.hdr.event = event;

    pCcm->handlerId = handlerId;

    pCcm->pText = pPlainText;
    pCcm->textLen = textLen;
    pCcm->pClear = pClear;
    pCcm->pWorking = pResult;
    pCcm->clearLen = clearLen;
    pCcm->micLen = micLen;
    pCcm->counter = 0;

    memcpy(pCcm->pWorking + clearLen, pPlainText, textLen);
    memcpy(&pCcm->scratch[1], pNonce, SEC_CCM_NONCE_LEN);
    Calc128Cpy(pCcm->key, (uint8_t *) pKey);

    pCcm->operation = SEC_CCM_OP_ENCRYPT;

    /* Begin encryption of text by generation of X_0 */
    secCcmGenX0(pBuf);

    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \fn     SecCcmDec
 *
 *  \brief  Execute the CCM-Mode verify and decrypt algorithm.
 *
 *  \param  pKey          Pointer to encryption key (SEC_CCM_KEY_LEN bytes).
 *  \param  pNonce        Pointer to nonce (SEC_CCM_NONCE_LEN bytes).
 *  \param  pCypherText   Pointer to text to decrypt.
 *  \param  textLen       Length of pCypherText in bytes.
 *  \param  pClear        Pointer to additional, unencrypted authentication text.
 *  \param  clearLen      Length of pClear in bytes.
 *  \param  pMic          Pointer to authentication digest.
 *  \param  micLen        Size of MIC in bytes (4, 8 or 16).
 *  \param  pResult       Buffer to hold result (returned in complete event).
 *  \param  handlerId     Task handler ID to receive complete event.
 *  \param  param         Optional parameter passed in complete event.
 *  \param  event         Event ID of complete event.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecCcmDec(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pCypherText, uint16_t textLen,
                 uint8_t *pClear, uint16_t clearLen, uint8_t *pMic, uint8_t micLen,
                 uint8_t *pResult, wsfHandlerId_t handlerId, uint16_t param, uint8_t event)
{
  secQueueBuf_t *pBuf;
  uint16_t bufSize = sizeof(secQueueBuf_t) + sizeof(secCcmSecCb_t);

  WSF_ASSERT(clearLen < SEC_CCM_MAX_ADDITIONAL_LEN);

  if ((pBuf = WsfMsgAlloc(bufSize)) != NULL)
  {
    secCcmSecCb_t *pCcm = (secCcmSecCb_t *) (pBuf + 1);

    /* Setup queue buffer */
    pBuf->pCb = pCcm;
    pBuf->type = SEC_TYPE_CCM;

    pBuf->msg.hdr.status = secCb.token++;
    pBuf->msg.hdr.param = param;
    pBuf->msg.hdr.event = event;

    pCcm->handlerId = handlerId;

    pCcm->pClear = pClear;
    pCcm->pRcvMic = pMic;
    pCcm->pWorking = pResult;
    pCcm->textLen = textLen;
    pCcm->clearLen = clearLen;
    pCcm->micLen = micLen;
    pCcm->counter = 0;

    /* Prepare the working buffer */
    memcpy(pCcm->pWorking, pClear, clearLen);
    memcpy(pCcm->pWorking + clearLen, pCypherText, textLen);
    memcpy(pCcm->pWorking + clearLen + textLen, pMic, micLen);

    memcpy(&pCcm->scratch[1], pNonce, SEC_CCM_NONCE_LEN);
    Calc128Cpy(pCcm->key, (uint8_t *) pKey);

    pCcm->operation = SEC_CCM_OP_DECRYPT;

    /* Begin decryption of text by generation of S_1 */
    secCcmGenS1(pBuf);

    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \fn     SecCcmInit
 *
 *  \brief  Called to initialize CCM-Mode security.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecCcmInit(void)
{
  secCb.hciCbackTbl[SEC_TYPE_CCM] = SecCcmHciCback;
}

#endif /* SEC_CCM_CFG */
