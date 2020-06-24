/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM security module for LE Secure Connections.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "sec_api.h"
#include "wsf_buf.h"
#include "wsf_trace.h"
#include "util/calc128.h"
#include "smp_api.h"
#include "smp_sc_main.h"
#include "dm_api.h"
#include "dm_main.h"
#include "dm_conn.h"
#include "dm_sec.h"

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Component function interface */
static const dmFcnIf_t dmSecLescFcnIf =
{
  dmEmptyReset,
  (dmHciHandler_t) dmEmptyHandler,
  (dmMsgHandler_t) dmSecLescMsgHandler
};

/* OOB random value */
static uint8_t *dmSecOobRand;

/* Local device's ECC Key */
static secEccKey_t localEccKey;

/*************************************************************************************************/
/*!
 *  \brief  DM dev event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmSecLescMsgHandler(dmSecMsg_t *pMsg)
{
  if (pMsg->hdr.event == DM_SEC_MSG_ECC_KEY_CNF)
  {
    pMsg->hdr.event = DM_SEC_ECC_KEY_IND;
    (*dmCb.cback)((dmEvt_t *) pMsg);
  }
  else if (pMsg->hdr.event == DM_SEC_MSG_CALC_OOB_CNF)
  {
    dmSecOobCalcIndEvt_t oobEvt;
    secCmacMsg_t *pCmacMsg = (secCmacMsg_t *) pMsg;

    WsfBufFree(pCmacMsg->pPlainText);

    /* Notify the application of the local confirm and random values */
    oobEvt.hdr.event = DM_SEC_CALC_OOB_IND;
    oobEvt.hdr.status = HCI_SUCCESS;

    Calc128Cpy(oobEvt.confirm, ((secAes_t *) pMsg)->pCiphertext);
    Calc128Cpy(oobEvt.random, dmSecOobRand);

    WsfBufFree(dmSecOobRand);

    (*dmCb.cback)((dmEvt_t *) &oobEvt);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function sends keypress cmd messages to the peer.
 *
 *  \return none.
 */
/*************************************************************************************************/
void DmSecKeypressReq(dmConnId_t connId, uint8_t keypressType)
{
  smpDmKeypress_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(smpDmKeypress_t))) != NULL)
  {
    /* Execution an an SMP state machine event to send the keypress to the peer device */
    pMsg->keypress = keypressType;
    pMsg->hdr.param = connId;

    pMsg->hdr.event = SMP_MSG_API_USER_KEYPRESS;

    SmpDmMsgSend((smpDmMsg_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function records the peer random value and peer confirm value exchanged via
 *          out-of-band (OOB) methods.
 *
 *  \param  connId      ID of the connection
 *  \param  pCfg        OOB Configuration
 *
 *  \return Pointer to IRK.
 */
/*************************************************************************************************/
void DmSecSetOob(dmConnId_t connId, dmSecLescOobCfg_t *pCfg)
{
  /* Update the Security Manager control structure with random and confirm values from the peer */
  SmpScSetOobCfg(connId, pCfg);
}

/*************************************************************************************************/
/*!
 *  \brief  This function calcualtes the local random and confirm values used in LESC OOB pairing.
 *          The operation's result is posted as a DM_SEC_CALC_OOB_IND event to the application's DM
 *          callback handler.  The local rand and confirm values are exchanged with the peer via
 *          out-of-band (OOB) methods and passed into the DmSecSetOob after DM_CONN_OPEN_IND.
 *
 *  \param  pRand       Random value used in calculation.
 *  \param  pPubKeyX    X component of the local public key.
 *
 *  \return none.
 */
/*************************************************************************************************/
void DmSecCalcOobReq(uint8_t *pRand, uint8_t *pPubKeyX)
{
  uint8_t *pCmacText;
  dmSecOobCalcIndEvt_t pEvt;

  SMP_TRACE_256("DmSecCalcOobReq Key", pPubKeyX);
  SMP_TRACE_128("DmSecCalcOobReq Rand", pRand);

  if ((dmSecOobRand = WsfBufAlloc(SMP_RAND_LEN)) != NULL)
  {
    /* Store the random value */
    Calc128Cpy(dmSecOobRand, pRand);

    /* Cnf = f4(PKx, PKx, rand, 0x00) where f4(U, V, x, Z) = AES-CMACx (U || V || Z) */
    if ((pCmacText = (uint8_t*) WsfBufAlloc(SMP_F4_TEXT_LEN)) !=  NULL)
    {
      uint8_t *pCatBuf = pCmacText;

      /* Concatinate PKx, PKx, 0x00 */
      pCatBuf = SmpScCat(pCatBuf, pPubKeyX, SMP_PUB_KEY_LEN);
      pCatBuf = SmpScCat(pCatBuf, pPubKeyX, SMP_PUB_KEY_LEN);
      *pCatBuf = 0;

      /* Execute CMAC with rand as the key */
      if (SecCmac(dmSecOobRand, pCmacText, SMP_F4_TEXT_LEN, dmCb.handlerId, 0, DM_SEC_MSG_CALC_OOB_CNF))
      {
        return;
      }

      WsfBufFree(pCmacText);
    }

    WsfBufFree(dmSecOobRand);
  }

  /* Notify the application of a failure */
  memset(&pEvt, 0, sizeof(dmSecOobCalcIndEvt_t));

  pEvt.hdr.event = DM_SEC_CALC_OOB_IND;
  pEvt.hdr.status = HCI_ERR_MEMORY_EXCEEDED;

  (*dmCb.cback)((dmEvt_t *) &pEvt);
}

/*************************************************************************************************/
/*!
 *  \brief  This function generates an ECC key for use with LESC security.  When the calculation
 *          completes, the result is posted as a DM_SEC_ECC_KEY_IND event to the application's
 *          DM callback handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecGenerateEccKeyReq()
{
  SecEccGenKey(dmCb.handlerId, 0, DM_SEC_MSG_ECC_KEY_CNF);
}

/*************************************************************************************************/
/*!
 *  \brief  This function sets the ECC key for use with LESC security to standard debug keys values.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetDebugEccKey()
{
  const uint8_t privateKey[] = {0x3f, 0x49, 0xf6, 0xd4,  0xa3, 0xc5, 0x5f, 0x38,
                                0x74, 0xc9, 0xb3, 0xe3,  0xd2, 0x10, 0x3f, 0x50,
                                0x4a, 0xff, 0x60, 0x7b,  0xeb, 0x40, 0xb7, 0x99,
                                0x58, 0x99, 0xb8, 0xa6,  0xcd, 0x3c, 0x1a, 0xbd};

  const uint8_t publicKeyX[] = {0x20, 0xb0, 0x03, 0xd2,  0xf2, 0x97, 0xbe, 0x2c,
                                0x5e, 0x2c, 0x83, 0xa7,  0xe9, 0xf9, 0xa5, 0xb9,
                                0xef, 0xf4, 0x91, 0x11,  0xac, 0xf4, 0xfd, 0xdb,
                                0xcc, 0x03, 0x01, 0x48,  0x0e, 0x35, 0x9d, 0xe6};

  const uint8_t publicKeyY[] = {0xdc, 0x80, 0x9c, 0x49,  0x65, 0x2a, 0xeb, 0x6d,
                                0x63, 0x32, 0x9a, 0xbf,  0x5a, 0x52, 0x15, 0x5c,
                                0x76, 0x63, 0x45, 0xc2,  0x8f, 0xed, 0x30, 0x24,
                                0x74, 0x1c, 0x8e, 0xd0,  0x15, 0x89, 0xd2, 0x8b};

  SMP_TRACE_INFO0("Debug LESC Keys Enabled");

  memcpy(localEccKey.privKey, privateKey, SEC_ECC_KEY_LEN);
  memcpy(localEccKey.pubKey_x, publicKeyX, SEC_ECC_KEY_LEN);
  memcpy(localEccKey.pubKey_y, publicKeyY, SEC_ECC_KEY_LEN);
}

/*************************************************************************************************/
/*!
 *  \brief  This function sets the local ECC key for use with LESC security.  The key can be
 *          generated using DmSecGenerateEccKeyReq or the key could originate from an application
 *          specific source (e.g. prestored in non-volatile memory, generated with specialized
 *          hardware, etc).
 *
 *  \param  pKey        Pointer to local ECC key
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecSetEccKey(secEccKey_t *pKey)
{
  memcpy(&localEccKey, pKey, sizeof(secEccKey_t));
}

/*************************************************************************************************/
/*!
 *  \brief  This function gets the local ECC key for use with LESC security.
 *
 *  \return Pointer to local ECC key.
 */
/*************************************************************************************************/
secEccKey_t *DmSecGetEccKey(void)
{
  return &localEccKey;
}

/*************************************************************************************************/
/*!
 *  \brief  This function is called by the application in response to a DM_SEC_COMPARE_IND event.
 *          The valid parameter indicates if the compare value of the DM_SEC_COMPARE_IND was valid.
 *
 *  \param  connId      ID of the connection
 *  \param  valid       TRUE if compare value was valid
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecCompareRsp(dmConnId_t connId, bool_t valid)
{
  smpDmMsg_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(smpDmMsg_t))) != NULL)
  {
    /* Execution an an SMP state machine event to send the compare response */
    pMsg->hdr.param = connId;

    if (valid)
    {
      pMsg->hdr.event = SMP_MSG_API_USER_CONFIRM;
    }
    else
    {
      SmpScGetCancelMsgWithReattempt(connId, (wsfMsgHdr_t *) pMsg, SMP_ERR_NUMERIC_COMPARISON);
    }

    SmpDmMsgSend((smpDmMsg_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  This function returns the 6-digit compare value for the specified 128-bit confirm value.
 *
 *  \param  pConfirm    Pointer to 128-bit comfirm value.
 *
 *  \return Six-digit compare value.
 */
/*************************************************************************************************/
uint32_t DmSecGetCompareValue(uint8_t *pConfirm)
{
  uint32_t compare = ((uint32_t) pConfirm[15]        +
                     ((uint32_t) pConfirm[14] << 8)  +
                     ((uint32_t) pConfirm[13] << 16) +
                     ((uint32_t) pConfirm[12] << 24));

  /* return the least significant six digits */
  return compare % 1000000;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM LE Secure Connections security.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmSecLescInit(void)
{
  dmFcnIfTbl[DM_ID_LESC] = (dmFcnIf_t *) &dmSecLescFcnIf;
}
