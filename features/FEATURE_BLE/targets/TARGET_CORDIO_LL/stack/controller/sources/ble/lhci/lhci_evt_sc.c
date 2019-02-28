/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI event module implementation file.
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

#include "lhci_int.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack a read local P-256 public key complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Read local P-256 public key indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackReadLocalP256PubKeyCmplEvt(uint8_t *pBuf, const LlReadLocalP256PubKeyInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_READ_PUB_KEY_CMPL;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_READ_LOCAL_P256_PUB_KEY_CMPL_EVT);
  UINT8_TO_BSTREAM (pBuf, LL_SUCCESS);
  memcpy(pBuf, pEvt->pubKey_x, sizeof(pEvt->pubKey_x));
  pBuf += sizeof(pEvt->pubKey_x);
  memcpy(pBuf, pEvt->pubKey_y, sizeof(pEvt->pubKey_y));

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a generate Diffie-Hellman key complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        Generate Diffie-Hellman key indication data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static uint8_t lhciPackGenerateDhKeyCmplEvt(uint8_t *pBuf, const LlGenerateDhKeyInd_t *pEvt)
{
  const uint8_t len = HCI_LEN_LE_GEN_DHKEY_CMPL;

  UINT8_TO_BSTREAM (pBuf, HCI_LE_GENERATE_DHKEY_CMPL_EVT);
  UINT8_TO_BSTREAM (pBuf, LL_SUCCESS);
  memcpy(pBuf, pEvt->dhKey, sizeof(pEvt->dhKey));

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  LL secure connections data event handler.
 *
 *  \param  pEvt    Buffer containing LL event.
 *
 *  \return TRUE if event handled, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t lhciScEncodeEvtPkt(LlEvt_t *pEvt)
{
  uint8_t *pEvtBuf = NULL;

  switch (pEvt->hdr.event)
  {
    case LL_READ_LOCAL_P256_PUB_KEY_CMPL_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_READ_LOCAL_P256_PUB_KEY_CMPL) << LHCI_BYTE_TO_BITS(0))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_READ_PUB_KEY_CMPL)) != NULL)
        {
          lhciPackReadLocalP256PubKeyCmplEvt(pEvtBuf, &pEvt->readLocalP256PubKeyInd);
        }
      }
      break;
    case LL_GENERATE_DHKEY_CMPL_IND:
      if ((lhciCb.leEvtMsk & ((uint64_t)(HCI_EVT_MASK_LE_GENERATE_DHKEY_CMPL) << LHCI_BYTE_TO_BITS(1))) &&
         (lhciCb.evtMsk & ((uint64_t)(HCI_EVT_MASK_LE_META) << LHCI_BYTE_TO_BITS(7))))
      {
        if ((pEvtBuf = lhciAllocEvt(HCI_LE_META_EVT, HCI_LEN_LE_GEN_DHKEY_CMPL)) != NULL)
        {
          lhciPackGenerateDhKeyCmplEvt(pEvtBuf, &pEvt->generateDhKeyInd);
        }
      }
      break;

    default:
      break;
  }

  if (pEvtBuf)
  {
    lhciSendEvt(pEvtBuf);
    return TRUE;
  }

  return FALSE;
}
