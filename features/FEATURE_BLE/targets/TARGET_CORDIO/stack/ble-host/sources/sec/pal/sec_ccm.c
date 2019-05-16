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
 *  \brief Counter with CBC-MAC (CCM) mode security - Native AES.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_buf.h"
#include "wsf_msg.h"
#include "sec_api.h"
#include "sec_main.h"

#ifndef SEC_CCM_CFG
#define SEC_CCM_CFG SEC_CCM_CFG_PLATFORM
#endif

#if SEC_CCM_CFG == SEC_CCM_CFG_PLATFORM

/**************************************************************************************************
  External Variables
**************************************************************************************************/

/* Global security control block */
extern secCb_t secCb;

/*************************************************************************************************/
/*!
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
  secCcmEncMsg_t *pCcmMsg;

  if ((pCcmMsg = WsfMsgAlloc(sizeof(secCcmEncMsg_t))) != NULL)
  {
    /* Encrypt. */
    PalCryptoCcmEnc(pKey, pNonce, pPlainText, textLen, pClear, clearLen, micLen, pResult,
                    handlerId, param, event);

    memcpy(pResult, pClear, clearLen);

    /* Send notification of encryption complete. */
    pCcmMsg->hdr.status = secCb.token++;
    pCcmMsg->hdr.param = param;
    pCcmMsg->hdr.event = event;
    pCcmMsg->pCiphertext = pResult;
    pCcmMsg->textLen = textLen + clearLen + micLen;
    WsfMsgSend(handlerId, pCcmMsg);

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
  secCcmDecMsg_t *pCcmMsg;

  if ((pCcmMsg = WsfMsgAlloc(sizeof(secCcmDecMsg_t))) != NULL)
  {
    /* Decrypt. */
    uint32_t error;

    error = PalCryptoCcmDec(pKey, pNonce, pCypherText, textLen, pClear, clearLen, pMic, micLen,
                            pResult, handlerId, param, event);

    /* Send notification of decryption complete. */
    pCcmMsg->hdr.status = secCb.token++;
    pCcmMsg->hdr.param = param;
    pCcmMsg->hdr.event = event;

    /* Compare MIC with computed MIC. */
    if (error)
    {
      /* MIC not authentic. */
      pCcmMsg->success = FALSE;
      pCcmMsg->pResult = NULL;
      pCcmMsg->pText = NULL;
      pCcmMsg->textLen = 0;
    }
    else
    {
      /* MIC authentic. */
      pCcmMsg->success = TRUE;
      pCcmMsg->pResult = pResult;
      pCcmMsg->pText = pResult;
      pCcmMsg->textLen = textLen;
    }

    WsfMsgSend(handlerId, pCcmMsg);

    return TRUE;
  }

  return  FALSE;

}

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize CCM-Mode security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecCcmInit()
{
  PalCryptoInit();

  secCb.hciCbackTbl[SEC_TYPE_CCM] = NULL;
}

#endif /* SEC_CCM_CFG */
