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
 *  \brief Security service implemented using HCI.
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
#include "hci_api.h"
#include "util/calc128.h"
#include "util/wstr.h"
#include "stack/platform/include/pal_crypto.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Security Control block */
secCb_t secCb;

/*************************************************************************************************/
/*!
 *  \brief  Callback for HCI encryption and random number events.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void secHciCback(hciEvt_t *pEvent)
{
  secQueueBuf_t *pBuf = NULL;
  wsfHandlerId_t handlerId = 0;

  /* Handle random number event. */
  switch (pEvent->hdr.event)
  {
  case HCI_LE_RAND_CMD_CMPL_CBACK_EVT:

    /* Copy new data to circular buffer of random data. */
    memcpy(&secCb.rand[HCI_RAND_LEN * secCb.randTop], pEvent->leRandCmdCmpl.randNum, HCI_RAND_LEN);
    secCb.randTop = (secCb.randTop >= SEC_HCI_RAND_MULT - 1) ? 0 : secCb.randTop + 1;
    break;

  case HCI_LE_ENCRYPT_CMD_CMPL_CBACK_EVT:
    pBuf = WsfMsgDeq(&secCb.aesEncQueue, &handlerId);

    WSF_ASSERT(pBuf != NULL);

    /* note: pBuf should never be NULL and is checked by assert above. */
    /* coverity[dereference] */
    if (pBuf->type == SEC_TYPE_CCM || pBuf->type == SEC_TYPE_CMAC || pBuf->type == SEC_TYPE_AES_REV)
    {
      WStrReverse(pEvent->leEncryptCmdCmpl.data, HCI_ENCRYPT_DATA_LEN);
    }
    break;

  case HCI_LE_READ_LOCAL_P256_PUB_KEY_CMPL_CBACK_EVT:
    pBuf = WsfMsgDeq(&secCb.pubKeyQueue, &handlerId);
    break;

  case HCI_LE_GENERATE_DHKEY_CMPL_CBACK_EVT:
    pBuf = WsfMsgDeq(&secCb.dhKeyQueue, &handlerId);
    break;

  default:
    break;
  }

  if (pBuf)
  {
    WSF_ASSERT(secCb.hciCbackTbl[pBuf->type]);
    secCb.hciCbackTbl[pBuf->type](pBuf, pEvent, handlerId);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the security service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecInit(void)
{
  WSF_QUEUE_INIT(&secCb.aesEncQueue);
  WSF_QUEUE_INIT(&secCb.pubKeyQueue);
  WSF_QUEUE_INIT(&secCb.dhKeyQueue);

  secCb.token = 0;

  /* Register callback with HCI */
  HciSecRegister(secHciCback);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the random number service.  This function should only be called once
 *          upon system initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecRandInit(void)
{
  int8_t i;

  /* get new random numbers */
  for (i=0; i<SEC_HCI_RAND_MULT; i++)
  {
    HciLeRandCmd();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function returns up to HCI_RAND_LEN * SEC_HCI_RAND_MULT bytes of random data to
 *          a buffer provided by the client.
 *
 *  \param  pRand       Pointer to returned random data.
 *  \param  randLen     Length of random data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecRand(uint8_t *pRand, uint8_t randLen)
{
  int8_t count = (randLen + HCI_RAND_LEN - 1) / HCI_RAND_LEN;
  uint8_t index = secCb.randBtm * HCI_RAND_LEN;

  WSF_ASSERT(randLen <= SEC_RAND_DATA_LEN);

  /* Copy from circular buffer of random data. */
  while (randLen--)
  {
    *pRand++ = secCb.rand[index];
    index = (index == SEC_RAND_DATA_LEN - 1) ? 0 : index + 1;
  }

  while (count--)
  {
    /* Request more random data. */
    HciLeRandCmd();

    /* Update copy index. */
    secCb.randBtm = (secCb.randBtm >= SEC_HCI_RAND_MULT - 1) ? 0 : secCb.randBtm + 1;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Queue callback and call LE encrypt given most significant byte ordered key and data.
 *
 *  \param  pKey        Pointer to key.
 *  \param  pText       Pointer to text to encrypt.
 *  \param  pBuf        Pointer to queue block.
 *  \param  handlerId   Handler ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecLeEncryptCmd(uint8_t *pKey, uint8_t *pText, void *pBuf, wsfHandlerId_t handlerId)
{
  uint8_t revKey[HCI_KEY_LEN];
  uint8_t revText[HCI_ENCRYPT_DATA_LEN];

  WStrReverseCpy(revKey, pKey, HCI_KEY_LEN);
  WStrReverseCpy(revText, pText, HCI_ENCRYPT_DATA_LEN);

  WsfMsgEnq(&secCb.aesEncQueue, handlerId, pBuf);
  HciLeEncryptCmd(revKey, revText);
}
