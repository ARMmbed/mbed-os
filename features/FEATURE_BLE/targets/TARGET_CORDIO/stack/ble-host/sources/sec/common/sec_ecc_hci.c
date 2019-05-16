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
 *  \brief WSF Security ECC implementation using HCI.
 */
/*************************************************************************************************/

#include <stdlib.h>
#include <time.h>
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

#ifndef SEC_ECC_CFG
#define SEC_ECC_CFG SEC_ECC_CFG_HCI
#endif

#if SEC_ECC_CFG == SEC_ECC_CFG_HCI

/**************************************************************************************************
External Variables
**************************************************************************************************/

extern secCb_t secCb;

/**************************************************************************************************
Local Variables
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Callback for HCI encryption for ECC operations.
 *
 *  \param  pBuf        Pointer to sec queue element.
 *  \param  pEvent      Pointer to HCI event.
 *  \param  handlerId   WSF handler ID.
 *
 *  \return none.
 */
/*************************************************************************************************/
void SecEccHciCback(secQueueBuf_t *pBuf, hciEvt_t *pEvent, wsfHandlerId_t handlerId)
{
  secEccMsg_t *pMsg = (secEccMsg_t *) &pBuf->msg;

  if (pEvent->hdr.event == HCI_LE_READ_LOCAL_P256_PUB_KEY_CMPL_CBACK_EVT)
  {
    /* Reverse copy the public key (to big endian) */
    WStrReverseCpy(pMsg->data.key.pubKey_x, pEvent->leP256.key, SEC_ECC_KEY_LEN);
    WStrReverseCpy(pMsg->data.key.pubKey_y, pEvent->leP256.key + SEC_ECC_KEY_LEN, SEC_ECC_KEY_LEN);

    /* Send shared secret to handler */
    pMsg->hdr.status = pEvent->leP256.status;
    WsfMsgSend(handlerId, pMsg);
  }
  else if (pEvent->hdr.event == HCI_LE_GENERATE_DHKEY_CMPL_CBACK_EVT)
  {
    /* Reverse copy the DH key (to big endian) */
    WStrReverseCpy(pMsg->data.sharedSecret.secret, pEvent->leGenDHKey.key, SEC_ECC_KEY_LEN);

    /* Send shared secret to handler */
    pMsg->hdr.status = pEvent->leGenDHKey.status;
    WsfMsgSend(handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Generate an ECC key.
 *
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecEccGenKey(wsfHandlerId_t handlerId, uint16_t param, uint8_t event)
{
  secQueueBuf_t *pBuf;
  uint16_t bufSize = sizeof(secQueueBuf_t) + sizeof(secEccMsg_t);

  if ((pBuf = WsfMsgAlloc(bufSize)) != NULL)
  {
    /* Record the event and parameter for use in the HCI response */
    pBuf->msg.hdr.param = param;
    pBuf->msg.hdr.event = event;
    pBuf->type = SEC_TYPE_DH;

    /* queue buffer */
    WsfMsgEnq(&secCb.pubKeyQueue, handlerId, pBuf);

    /* Request the local public key via HCI */
    HciLeReadLocalP256PubKey();

    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Generate an ECC key.
 *
 *  \param  pKey        ECC Key structure.
 *  \param  handlerId   WSF handler ID for client.
 *  \param  param       Optional parameter sent to client's WSF handler.
 *  \param  event       Event for client's WSF handler.
 *
 *  \return TRUE if successful, else FALSE.
 */
/*************************************************************************************************/
bool_t SecEccGenSharedSecret(secEccKey_t *pKey, wsfHandlerId_t handlerId, uint16_t param, uint8_t event)
{
  secQueueBuf_t *pBuf;
  uint16_t bufSize = sizeof(secQueueBuf_t) + sizeof(secEccMsg_t);

  if ((pBuf = WsfMsgAlloc(bufSize)) != NULL)
  {
    uint8_t pubKeyX[SEC_ECC_KEY_LEN];
    uint8_t pubKeyY[SEC_ECC_KEY_LEN];

    /* Record the event and parameter for use in the HCI response */
    pBuf->msg.hdr.param = param;
    pBuf->msg.hdr.event = event;
    pBuf->type = SEC_TYPE_DH;

    /* queue buffer */
    WsfMsgEnq(&secCb.dhKeyQueue, handlerId, pBuf);

    /* Reverse keys (to little endian) */
    WStrReverseCpy(pubKeyX, pKey->pubKey_x, SEC_ECC_KEY_LEN);
    WStrReverseCpy(pubKeyY, pKey->pubKey_y, SEC_ECC_KEY_LEN);

    /* Request the DH Key via HCI */
    HciLeGenerateDHKey(pubKeyX, pubKeyY);

    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Called to initialize ECC security.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecEccInit()
{
  secCb.hciCbackTbl[SEC_TYPE_DH] = SecEccHciCback;
}

#endif /* SEC_ECC_CFG */
