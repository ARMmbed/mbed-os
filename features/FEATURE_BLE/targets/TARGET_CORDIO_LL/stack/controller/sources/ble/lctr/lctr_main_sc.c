/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller secure connections implementation file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "lctr_int.h"
#include "lctr_api.h"
#include "lctr_api_sc.h"
#include "lmgr_api.h"
#include "lmgr_api_sc.h"
#include "wsf_assert.h"
#include "ll_math.h"
#include "wsf_trace.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Notify host of key generation.
 *
 *  \param      pPubKey     Public key.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrNotifyReadLocalP256PubKeyInd(const uint8_t *pPubKey)
{
  LlReadLocalP256PubKeyInd_t evt =
  {
    .hdr =
    {
      .param  = 0,
      .event  = LL_READ_LOCAL_P256_PUB_KEY_CMPL_IND,
      .status = LL_SUCCESS
    }
  };

  evt.status = LL_SUCCESS;
  memcpy(evt.pubKey_x, pPubKey, sizeof(evt.pubKey_x));
  memcpy(evt.pubKey_y, pPubKey + LL_ECC_KEY_LEN, sizeof(evt.pubKey_y));

  LL_TRACE_INFO0("### LlEvent ###  LL_READ_LOCAL_P256_PUB_KEY_CMPL_IND, status=LL_SUCCESS");

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host of key generation.
 *
 *  \param      pDhKey    Diffie-Hellman key.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrNotifyGenerateDhKeyInd(const uint8_t *pDhKey)
{
  LlGenerateDhKeyInd_t evt =
  {
    .hdr =
    {
      .param  = 0,
      .event  = LL_GENERATE_DHKEY_CMPL_IND,
      .status = LL_SUCCESS
    }
  };

  evt.status = LL_SUCCESS;
  memcpy(evt.dhKey, pDhKey, sizeof(evt.dhKey));

  LL_TRACE_INFO0("### LlEvent ###  LL_GENERATE_DHKEY_CMPL_IND, status=LL_SUCCESS");

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      P-256 key pair generation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrScGenerateP256KeyPairContinue(void)
{
  if (LlMathEccGenerateP256KeyPairContinue())
  {
    uint8_t pubKey[LL_ECC_KEY_LEN * 2];

    /* Set the newly-generated private key as the local key. */
    LlMathEccGenerateP256KeyPairComplete(pubKey, lmgrScCb.privKey);

    /* Notify host that the key was generated. */
    lctrNotifyReadLocalP256PubKeyInd(pubKey);

    lmgrScCb.eccOpActive = FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      DH Key generation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrScGenerateDhKeyContinue(void)
{
  if (LlMathEccGenerateDhKeyContinue())
  {
    uint8_t dhKey[LL_ECC_KEY_LEN];

    LlMathEccGenerateDhKeyComplete(dhKey);

    /* Notify host that the key was generated. */
    lctrNotifyGenerateDhKeyInd(dhKey);

    lmgrScCb.eccOpActive = FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Baseband driver ECC service callback.
 *
 *  \param      op      Operation to service.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrScBbDrvEccServiceCback(uint8_t op)
{
  switch (op)
  {
    case LL_MATH_ECC_OP_GENERATE_P256_KEY_PAIR:
      WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_SC_GENERATE_P256_KEY_PAIR));
      break;

    case LL_MATH_ECC_OP_GENERATE_DH_KEY:
      WsfSetEvent(lmgrPersistCb.handlerId, (1 << LCTR_EVENT_SC_GENERATE_DHKEY));
      break;

    default:
      WSF_ASSERT(FALSE);
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Generate a P-256 public/private key pair.
 *
 *  \return     Status error code.
 *
 *  Generate a P-256 public/private key pair.  If another ECC operation (P-256 key pair generation
 *  or Diffie-Hellman key generation) is ongoing, an error will be returned.
 */
/*************************************************************************************************/
uint8_t LctrGenerateP256KeyPair(void)
{
  /* Allow only one key pair generation at a time. */
  if (lmgrScCb.eccOpActive)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  /* Start operation. */
  lmgrScCb.eccOpActive = TRUE;
  if (lmgrScCb.privKeySet)
  {
    LlMathEccGenerateP256PublicKeyStart(lmgrScCb.privKey);
  }
  else
  {
    LlMathEccGenerateP256KeyPairStart();
  }

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Generate a Diffie-Hellman key.
 *
 *  \param      pPubKey     Public key.
 *  \param      pPrivKey    Private key.
 *
 *  \return     Status error code.
 *
 *  Generate a Diffie-Hellman key from a remote public key and the local private key.  If another
 *  ECC operation (P-256 key pair generation or Diffie-Hellman key generation) is ongoing, an error
 *  will be returned.
 */
/*************************************************************************************************/
uint8_t LctrGenerateDhKey(const uint8_t *pPubKey, const uint8_t *pPrivKey)
{
  /* Allow only one key pair generation at a time. */
  if (lmgrScCb.eccOpActive)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  /* Start operation. */
  lmgrScCb.eccOpActive = TRUE;
  LlMathEccGenerateDhKeyStart(pPubKey, pPrivKey);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer controller resources for secure connections.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrScInit(void)
{
  lctrEventHdlrTbl[LCTR_EVENT_SC_GENERATE_P256_KEY_PAIR] = lctrScGenerateP256KeyPairContinue;
  lctrEventHdlrTbl[LCTR_EVENT_SC_GENERATE_DHKEY] = lctrScGenerateDhKeyContinue;

  LlMathEccSetServiceCback(lctrScBbDrvEccServiceCback);
}
