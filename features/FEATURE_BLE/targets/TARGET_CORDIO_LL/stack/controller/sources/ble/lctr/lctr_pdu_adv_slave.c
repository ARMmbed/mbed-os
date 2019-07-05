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
 * \brief Link layer controller slave advertising channel packet implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_adv_slave.h"
#include "util/bstream.h"
#include "wsf_trace.h"
#include <string.h>
#include <stdint.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack an advertising PDU header.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pHdr        Unpacked PDU header.
 *
 *  \return PDU header length.
 */
/*************************************************************************************************/
uint8_t lctrPackAdvbPduHdr(uint8_t *pBuf, const lctrAdvbPduHdr_t *pHdr)
{
  const uint8_t len = LL_ADV_HDR_LEN;

  uint16_t hdr = 0;

  hdr |= pHdr->pduType      << LCTR_ADV_HDR_PDU_TYPE_SHIFT;
  hdr |= pHdr->chSel        << LCTR_ADV_HDR_CH_SEL_SHIFT;
  hdr |= pHdr->txAddrRnd    << LCTR_ADV_HDR_TX_ADD_SHIFT;
  hdr |= pHdr->rxAddrRnd    << LCTR_ADV_HDR_RX_ADD_SHIFT;
  hdr |= pHdr->len          << LCTR_ADV_HDR_LEN_SHIFT;

  UINT16_TO_BSTREAM(pBuf, hdr);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a generic advertising PDU.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  advA        AdvA (local device address).
 *  \param  pAdvData    Advertising data buffer (ADVB-U).
 *
 *  \return PDU payload length.
 */
/*************************************************************************************************/
uint8_t lctrPackAdvPdu(uint8_t *pBuf, uint64_t advA, const lmgrAdvbUser_t *pAdvData)
{
  const uint8_t len = BDA_ADDR_LEN + pAdvData->len;

  BDA64_TO_BSTREAM(pBuf, advA);                 /* AdvA */
  memcpy(pBuf, pAdvData->buf, pAdvData->len);   /* AdvData */

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a connectable direct advertising indication PDU.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  advA        AdvA (local device address).
 *  \param  initA       InitA.
 *
 *  \return PDU payload length.
 */
/*************************************************************************************************/
uint8_t lctrPackConnDirectIndAdvPdu(uint8_t *pBuf, uint64_t advA, uint64_t initA)
{
  const uint8_t len = BDA_ADDR_LEN + BDA_ADDR_LEN;

  BDA64_TO_BSTREAM(pBuf, advA);                 /* AdvA */
  BDA64_TO_BSTREAM(pBuf, initA);                /* InitA */

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a scan request PDU.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pPdu        Unpacked packet.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrPackScanReqPdu(uint8_t *pBuf, const lctrScanReq_t *pPdu)
{
  const uint8_t len = LL_SCAN_REQ_PDU_LEN;

  BDA64_TO_BSTREAM(pBuf, pPdu->scanAddr);
  BDA64_TO_BSTREAM(pBuf, pPdu->advAddr);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a scan response PDU.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  advA        AdvA (local device address).
 *  \param  pRspData    Scan response data buffer (ADVB-U).
 *
 *  \return PDU payload length.
 */
/*************************************************************************************************/
uint8_t lctrPackScanRspPdu(uint8_t *pBuf, uint64_t advA, const lmgrAdvbUser_t *pRspData)
{
  /* Same as advertising PDU. */
  return lctrPackAdvPdu(pBuf, advA, pRspData);
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a connect indication PDU, addresses only.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pPdu        Unpacked packet.
 *
 *  \return PDU payload length.
 */
/*************************************************************************************************/
uint8_t lctrPackConnIndPduAddr(uint8_t *pBuf, const lctrConnInd_t *pPdu)
{
  const uint8_t len = 2 * sizeof(bdAddr_t);

  BDA64_TO_BSTREAM(pBuf, pPdu->initAddr);
  BDA64_TO_BSTREAM(pBuf, pPdu->advAddr);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a connect indication PDU.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pPdu        Unpacked packet.
 *
 *  \return PDU payload length.
 */
/*************************************************************************************************/
uint8_t lctrPackConnIndPdu(uint8_t *pBuf, const lctrConnInd_t *pPdu)
{
  const uint8_t len = LL_CONN_IND_PDU_LEN;

  uint8_t field8;

  pBuf += lctrPackConnIndPduAddr(pBuf, pPdu);

  uint16_t interval = pPdu->interval;
#if (LL_ENABLE_TESTER == TRUE)
  if (llTesterCb.connIntervalZero == TRUE)
  {
    interval = 0;
  }
#endif

  /* LLData */
  UINT32_TO_BSTREAM(pBuf, pPdu->accessAddr);
  UINT24_TO_BSTREAM(pBuf, pPdu->crcInit);
  UINT8_TO_BSTREAM (pBuf, pPdu->txWinSize);
  UINT16_TO_BSTREAM(pBuf, pPdu->txWinOffset);
  UINT16_TO_BSTREAM(pBuf, interval);
  UINT16_TO_BSTREAM(pBuf, pPdu->latency);
  UINT16_TO_BSTREAM(pBuf, pPdu->timeout);
  UINT40_TO_BSTREAM(pBuf, pPdu->chanMask);

  field8 = (pPdu->hopInc    << 0) |
           (pPdu->masterSca << 5);
  UINT8_TO_BSTREAM(pBuf, field8);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack an advertising channel PDU header.
 *
 *  \param  pHdr        Unpacked header.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU header length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackAdvbPduHdr(lctrAdvbPduHdr_t *pHdr, const uint8_t *pBuf)
{
  const uint8_t len = LL_ADV_HDR_LEN;

  uint16_t hdr;

  BSTREAM_TO_UINT16(hdr, pBuf);

  pHdr->pduType     = (hdr >> LCTR_ADV_HDR_PDU_TYPE_SHIFT) & 0x000F;
  pHdr->chSel       = (hdr >> LCTR_ADV_HDR_CH_SEL_SHIFT)   & 0x0001;
  pHdr->txAddrRnd   = (hdr >> LCTR_ADV_HDR_TX_ADD_SHIFT)   & 0x0001;
  pHdr->rxAddrRnd   = (hdr >> LCTR_ADV_HDR_RX_ADD_SHIFT)   & 0x0001;
  if (pLctrRtCfg->btVer >= LL_VER_BT_CORE_SPEC_5_0)
  {
    pHdr->len       = (hdr >> LCTR_ADV_HDR_LEN_SHIFT)      & LL_ADV_EXT_HDR_LEN_MSK;
  }
  else
  {
    pHdr->len       = (hdr >> LCTR_ADV_HDR_LEN_SHIFT)      & LL_ADV_HDR_LEN_MSK;
  }

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a scan request PDU.
 *
 *  \param  pPdu        Unpacked packet.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackScanReqPdu(lctrScanReq_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = 2 * sizeof(bdAddr_t);

  BSTREAM_TO_BDA64(pPdu->scanAddr, pBuf);
  BSTREAM_TO_BDA64(pPdu->advAddr, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a connect indication PDU.
 *
 *  \param  pPdu        Unpacked packet.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackConnIndPdu(lctrConnInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_CONN_IND_PDU_LEN;

  uint8_t field8;

  BSTREAM_TO_BDA64(pPdu->initAddr, pBuf);
  BSTREAM_TO_BDA64(pPdu->advAddr, pBuf);

  /* LLData */
  BSTREAM_TO_UINT32(pPdu->accessAddr, pBuf);
  BSTREAM_TO_UINT24(pPdu->crcInit, pBuf);
  BSTREAM_TO_UINT8 (pPdu->txWinSize, pBuf);
  BSTREAM_TO_UINT16(pPdu->txWinOffset, pBuf);
  BSTREAM_TO_UINT16(pPdu->interval, pBuf);
  BSTREAM_TO_UINT16(pPdu->latency, pBuf);
  BSTREAM_TO_UINT16(pPdu->timeout, pBuf);
  BSTREAM_TO_UINT40(pPdu->chanMask, pBuf);

  BSTREAM_TO_UINT8(field8, pBuf);
  pPdu->hopInc    = (field8 >> 0) & 0x1F;
  pPdu->masterSca = (field8 >> 5) & 0x07;

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Validate range of a connect indication PDU.
 *
 *  \param  pPdu        Unpacked packet.
 *
 *  \return PDU length.
 *
 *  Validation is limited to malfunction of receiving device. Some out of spec values are allowed
 *  to allow if safe operation of the system is possible.
 */
/*************************************************************************************************/
bool_t lctrValidateConnIndPdu(lctrConnInd_t *pPdu)
{
  bool_t result = TRUE;

  /* pPdu->initAddr     Allow all values */
  /* pPdu->advAddr      Allow all values */
  /* pPdu->accessAddr   Allow all values */
  /* pPdu->crcInit      Allow all values */

  /* connInterval: 7.5ms to 4.0s */
  if ((pPdu->interval < 6) || (pPdu->interval > 3200))
  {
    LL_TRACE_WARN1("Invalid CONN_IND field value interval=%u", pPdu->interval);
    result = FALSE;
  }

  /* transmitWindowSize: 1.25ms to 10ms */
  if (pPdu->txWinSize > 8)
  {
    LL_TRACE_WARN1("Invalid CONN_IND field value txWinSize=%u", pPdu->txWinSize);
    result = FALSE;
  }

  /* transmitWindowOffset: 0ms to connInterval */
  if (pPdu->txWinOffset > pPdu->interval)
  {
    LL_TRACE_WARN1("Invalid CONN_IND field value txWinOffset=%u", pPdu->txWinOffset);
    result = FALSE;
  }

  /* connSupervisionTimeout: range of 100 ms to 32.0s */
  if ((pPdu->timeout < 10) || (pPdu->timeout > 3200))
  {
    LL_TRACE_WARN1("Invalid CONN_IND field value timeout=%u", pPdu->timeout);
    result = FALSE;
  }

  /* ChM */
  if ((pPdu->chanMask & (UINT64_C(7) << LL_CHAN_ADV_MIN_IDX)) || (pPdu->chanMask == 0))
  {
    LL_TRACE_WARN0("Invalid CONN_IND field value chanMask");
    result = FALSE;
  }

  /* hopIncrement: 5 to 16 */
  if ((pPdu->hopInc < 5) || (pPdu->hopInc > 16))
  {
    LL_TRACE_WARN1("Invalid CONN_IND field value hopInc=%u", pPdu->hopInc);
    result = FALSE;
  }

  /* pPdu->latency      Allow all values */
  /* pPdu->masterSca    All 3 bits are valid */

  return result;
}
