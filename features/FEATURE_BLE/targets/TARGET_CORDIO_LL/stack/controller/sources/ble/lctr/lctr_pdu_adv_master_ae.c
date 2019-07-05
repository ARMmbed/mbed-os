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
 * \brief Link layer controller slave extended advertising operation builder implementation file.
 */
/*************************************************************************************************/

#include "lctr_pdu_adv_ae.h"
#include "lctr_pdu_adv.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief      Unpack Extended Advertising Header.
 *
 *  \param      pPdu        Unpacked extended advertising header.
 *  \param      pNewFlags   New extended header flags.
 *  \param      pBuf        Packed extended advertising buffer.
 *
 *  \return     Extended advertising header length.
 *
 *  \note       This routine does not clear unused field values to allow the client to have
 *              default values in pExtAdvHdr. Newer values will be updated.
 */
/*************************************************************************************************/
uint8_t lctrUnpackExtAdvHeader(lctrExtAdvHdr_t *pPdu, uint8_t *pNewFlags, const uint8_t *pBuf)
{
  const uint8_t *pStartBuf = pBuf;

  *pNewFlags = 0;

  uint8_t field8;
  BSTREAM_TO_UINT8(field8, pBuf);

  pPdu->extHdrLen = field8 & 0x3F;
  pPdu->advMode = field8 >> 6;

  /* Extended Header Flags only present if length >= 1. */
  if (pPdu->extHdrLen)
  {
    uint8_t extHdrFlags;
    BSTREAM_TO_UINT8(extHdrFlags, pBuf);
    *pNewFlags = extHdrFlags;
    pPdu->extHdrFlags |= extHdrFlags;

    if (extHdrFlags & LL_EXT_HDR_ADV_ADDR_BIT)
    {
      BSTREAM_TO_BDA64(pPdu->advAddr, pBuf);
    }

    if (extHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT)
    {
      BSTREAM_TO_BDA64(pPdu->tgtAddr, pBuf);
    }

    if (extHdrFlags & LL_EXT_HDR_CTE_INFO_BIT)
    {
      /* Skip CTEInfo byte. */
      pBuf++;
    }

    if (extHdrFlags & LL_EXT_HDR_ADI_BIT)
    {
      uint16_t adi;
      BSTREAM_TO_UINT16(adi, pBuf);
      pPdu->did = (adi >>  0) & 0x0FFF;
      pPdu->sid = (adi >> 12) & 0x000F;
    }

    if (extHdrFlags & LL_EXT_HDR_AUX_PTR_BIT)
    {
      pPdu->pAuxPtr = pBuf;
      pBuf += 3;
    }

    if (extHdrFlags & LL_EXT_HDR_SYNC_INFO_BIT)
    {
      pPdu->pSyncInfo = pBuf;
      pBuf += 18;
    }

    if (extHdrFlags & LL_EXT_HDR_TX_PWR_BIT)
    {
      BSTREAM_TO_UINT8(pPdu->txPwr, pBuf);
    }

    if ((pPdu->extHdrLen + 1) > (pBuf - pStartBuf))
    {
      pPdu->pAcad = pBuf;
      pPdu->acadLen = (pPdu->extHdrLen + 1) - (pBuf - pStartBuf);
      pBuf += pPdu->acadLen;
    }
    else
    {
      pPdu->pAcad = NULL;
      pPdu->acadLen = 0;
    }
  }
  return pBuf - pStartBuf;
}

/*************************************************************************************************/
/*!
 *  \brief      Unpack AuxPtr fields.
 *
 *  \param      pAuxPtr     Unpacked AuxPtr data.
 *  \param      pBuf        Packed AuxPtr data.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrUnpackAuxPtr(lctrAuxPtr_t *pAuxPtr, const uint8_t *pBuf)
{
  pAuxPtr->auxChIdx    = (pBuf[0] >> 0) & 0x3F;
  pAuxPtr->ca          = (pBuf[0] >> 6) & 0x01;
  pAuxPtr->offsetUnits = (pBuf[0] >> 7) & 0x01;
  pBuf++;

  uint16_t field16;
  BYTES_TO_UINT16(field16, pBuf);
  pAuxPtr->auxOffset   = (field16 >>  0) & 0x1FFF;
  pAuxPtr->auxPhy      = (field16 >> 13) & 0x0007;
}

/*************************************************************************************************/
/*!
 *  \brief      Unpack SyncInfo fields.
 *
 *  \param      pSyncInfo   Sync info.
 *  \param      pBuf        Buffer pointer to SyncInfo field.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrUnpackSyncInfo(lctrSyncInfo_t *pSyncInfo, const uint8_t *pBuf)
{
  uint16_t field16;
  BYTES_TO_UINT16(field16, pBuf);
  pSyncInfo->syncOffset   = (field16 >>  0) & 0x1FFF;
  pSyncInfo->offsetUnits  = (field16 >> 13) & 0x1;
  pSyncInfo->offsetAdjust = (field16 >> 14) & 0x1;
  pBuf +=        2;

  BSTREAM_TO_UINT16(pSyncInfo->syncInter, pBuf);

  uint64_t field64;
  BYTES_TO_UINT40(field64, pBuf);
  pSyncInfo->chanMap    = (field64 >>  0) & LL_CHAN_DATA_ALL;
  pSyncInfo->sca        = (field64 >>  37) & 0x07;
  pBuf+=5;

  BSTREAM_TO_UINT32(pSyncInfo->accAddr, pBuf);

  BSTREAM_TO_UINT24(pSyncInfo->crcInit, pBuf);

  BSTREAM_TO_UINT16(pSyncInfo->eventCounter, pBuf);
}
