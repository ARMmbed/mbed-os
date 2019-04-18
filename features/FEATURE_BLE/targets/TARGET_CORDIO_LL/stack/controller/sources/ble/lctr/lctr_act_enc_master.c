/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Link layer controller master encryption action routines.
 */
/*************************************************************************************************/

#include "lctr_int_enc_master.h"
#include "lctr_int_enc_slave.h"
#include "lmgr_api.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Generate master encryption vectors.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrGenerateMstVectors(lctrConnCtx_t *pCtx)
{
  /* Store host parameters. */
  memcpy(pCtx->ltk, pLctrConnMsg->startEnc.key, sizeof(pCtx->ltk));
  memcpy(pCtx->rand, pLctrConnMsg->startEnc.rand, sizeof(pCtx->rand));
  pCtx->ediv = pLctrConnMsg->startEnc.diversifier;

  /* Generate master part of IV. */
  PalCryptoGenerateRandomNumber(pCtx->iv + LCTR_IV_M_OFFS, LL_IV_LEN / 2);

  /* Generate master part of SKD. */
  PalCryptoGenerateRandomNumber(pCtx->skd + LCTR_SKD_M_OFFS, LL_SKD_LEN / 2);
}

/*************************************************************************************************/
/*!
 *  \brief      Generate master encryption vectors.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrStoreSlvVectors(lctrConnCtx_t *pCtx)
{
  /* Store slave part. */
  memcpy(pCtx->skd + LCTR_SKD_S_OFFS, lctrDataPdu.pld.encRsp.skd_s, LL_SKD_LEN / 2);
  memcpy(pCtx->iv + LCTR_IV_S_OFFS, lctrDataPdu.pld.encRsp.iv_s, LL_IV_LEN / 2);
}

/*************************************************************************************************/
/*!
 *  \brief      Send encryption request PDU to peer.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSendEncReq(lctrConnCtx_t *pCtx)
{
  uint8_t *pPdu;

  if ((pPdu = lctrTxCtrlPduAlloc(LL_ENC_REQ_LEN)) != NULL)
  {
    uint8_t *pBuf = pPdu;

    /*** Assemble control PDU. ***/

    UINT8_TO_BSTREAM(pBuf, LL_PDU_ENC_REQ);

    memcpy(pBuf, pCtx->rand, LL_RAND_LEN);
    pBuf += LL_RAND_LEN;

    UINT16_TO_BSTREAM(pBuf, pCtx->ediv);

    memcpy(pBuf, pCtx->skd + LCTR_SKD_M_OFFS, LL_SKD_LEN / 2);
    pBuf += LL_SKD_LEN / 2;

    memcpy(pBuf, pCtx->iv + LCTR_IV_M_OFFS, LL_IV_LEN / 2);

    /*** Queue for transmit. ***/

    lctrTxCtrlPduQueue(pCtx, pPdu);
  }
}
