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
 * \brief Link layer controller data path implementation file.
 */
/*************************************************************************************************/

#include "lctr_api_conn_cs2.h"
#include "lctr_int_conn.h"

/*************************************************************************************************/
/*!
 *  \brief  Calculate permutation.
 *
 *  \param  v       Input value.
 *
 *  \return Result of permutation calculation.
 */
/*************************************************************************************************/
static inline uint32_t lctrCalcPerm(uint32_t v)
{
  uint32_t mask;

  /*          abcd efgh ijkl mnop */
  mask = 0x0F0F0F0F;
  v    = ((v << 8) & mask) | (v & ~mask);

  /*     efgh abcd mnop ijkl xxxx */
  mask = mask ^ (mask << 2);
  v    = ((v << 4) & mask) | (v & ~mask);

  /*  gh efcd abop mnkl ijxx xxxx */
  mask = mask ^ (mask << 1);
  v    = ((v << 2) & mask) | (v & ~mask);

  /* hgf edcb apon mlkj ixxx xxxx */
  return v >> 7;
}

/*************************************************************************************************/
/*!
 *  \brief  Calculate MAM (Multiply, Add, Modulo).
 *
 *  \param  a       Input value a.
 *  \param  b       Input value b.
 *
 *  \return Result of MAM.
 */
/*************************************************************************************************/
static inline uint16_t lctrCalcMAM(uint16_t a, uint16_t b)
{
  /* (17 x a + b) mod 2^16 */
  return ((17 * a) + b) & 0xFFFFF;
}

/*************************************************************************************************/
/*!
 *  \brief  Compute next channel index, algorithm #2.
 *
 *  \param  pCtx    Connection context.
 *  \param  numSkip Number of channels to skip.
 *
 *  \return Next data channel index.
 */
/*************************************************************************************************/
uint8_t lctrSelectNextChannel2(lctrConnCtx_t *pCtx, uint16_t numSkip)
{
  /* Ignore parameter since eventCounter is adjusted by client. */
  (void)numSkip;

  unsigned int prn;

  /* Pseudo random number */

  prn = pCtx->eventCounter ^ pCtx->chIdentifier;
  prn = lctrCalcPerm(prn);
  prn = lctrCalcMAM(prn, pCtx->chIdentifier);
  prn = lctrCalcPerm(prn);
  prn = lctrCalcMAM(prn, pCtx->chIdentifier);
  prn = lctrCalcPerm(prn);
  prn = lctrCalcMAM(prn, pCtx->chIdentifier);
  /* uint16_t prn_s = prn; */
  unsigned int prn_e = prn ^ pCtx->chIdentifier;

  /* unmappedChannel */

  uint16_t unmapChan = LL_MATH_MOD_37(prn_e & 0xFFFF);

  pCtx->lastChanIdx = unmapChan;

  /* remappingIndex */

  if (!((UINT64_C(1) << unmapChan) & pCtx->chanMask))
  {
    /* remappingIndex = (N * prn_e) / 2^16 */
    uint8_t remapIdx = (pCtx->numUsedChan * prn_e) >> 16;
    return pCtx->chanRemapTbl[remapIdx];
  }

  return unmapChan;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize the channel selection 2 resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrChannelSelection2Init(void)
{
  lctrChSelHdlr[LL_CH_SEL_2] = lctrSelectNextChannel2;

  /* Set supported features. */
  if (pLctrRtCfg->btVer >= LL_VER_BT_CORE_SPEC_5_0)
  {
    lmgrPersistCb.featuresDefault |= LL_FEAT_CH_SEL_2;
  }
}
