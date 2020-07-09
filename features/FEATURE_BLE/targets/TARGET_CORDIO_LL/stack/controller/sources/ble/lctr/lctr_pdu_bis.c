/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller BIS packet implementation file.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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

#include "lctr_pdu_iso.h"
#include "util/bstream.h"
#include "hci_defs.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Pack a BIS Data PDU header.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pHdr        Unpacked PDU header.
 *
 *  \return Header length.
 */
/*************************************************************************************************/
uint8_t lctrBisPackDataPduHdr(uint8_t *pBuf, const lctrBisDataPduHdr_t *pHdr)
{
  const uint8_t len = LL_DATA_HDR_LEN;

  uint16_t hdr = 0;

  hdr |= (pHdr->llid    ) << 0;
  hdr |= (pHdr->cssn & 7) << 2;
  hdr |= (pHdr->cstf    ) << 5;
  hdr |= (pHdr->len     ) << 8;
  UINT16_TO_BSTREAM(pBuf, hdr);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a BIG Channel Map indication.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  chanMap     Channel map.
 *  \param  instance    Instance.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t lctrBisPackBigChannelMapInd(uint8_t *pBuf, uint64_t chanMap, uint16_t instance)
{
  const uint8_t len = sizeof(uint8_t) + LL_BIG_CHAN_MAP_IND_PDU_LEN;

  UINT8_TO_BSTREAM (pBuf, LL_BIG_OPCODE_CHAN_MAP_IND);
  UINT40_TO_BSTREAM(pBuf, chanMap);
  UINT16_TO_BSTREAM(pBuf, instance);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a BIG Terminate indication.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  reason      Reason code.
 *  \param  instance    Instance.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t lctrBisPackBigTerminateInd(uint8_t *pBuf, uint8_t reason, uint16_t instance)
{
  const uint8_t len = sizeof(uint8_t) + LL_BIG_TERMINATE_IND_PDU_LEN;

  UINT8_TO_BSTREAM (pBuf, LL_BIG_OPCODE_BIG_TERM_IND);
  UINT8_TO_BSTREAM (pBuf, reason);
  UINT16_TO_BSTREAM(pBuf, instance);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a BIS Data PDU header.
 *
 *  \param  pHdr        Unpacked header.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Header length.
 */
/*************************************************************************************************/
uint8_t lctrBisUnpackDataPduHdr(lctrBisDataPduHdr_t *pHdr, const uint8_t *pBuf)
{
  const uint8_t len = LL_DATA_HDR_LEN;

  uint16_t hdr;

  BSTREAM_TO_UINT16(hdr, pBuf);

  pHdr->llid = (hdr >> 0) & 0x03;
  pHdr->cssn = (hdr >> 2) & 0x07;
  pHdr->cstf = (hdr >> 5) & 0x01;
  pHdr->len  = (hdr >> 8);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a BIG Channel Map indication.
 *
 *  \param  pChanMap     Channel map return buffer.
 *  \param  pInstance    Instance return buffer.
 *  \param  pBuf         Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrBisUnpackBigChannelMapInd(uint64_t *pChanMap, uint16_t *pInstance, const uint8_t *pBuf)
{
  const uint8_t len = sizeof(uint8_t) + LL_BIG_CHAN_MAP_IND_PDU_LEN;

  uint64_t chanMap;
  uint16_t instance;

  pBuf++;   /* Opcode */
  BSTREAM_TO_UINT40(chanMap, pBuf);
  BSTREAM_TO_UINT16(instance, pBuf);

  *pChanMap = chanMap;
  *pInstance = instance;

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a BIG Terminate indication.
 *
 *  \param  pReason     Channel map return buffer.
 *  \param  pInstance   Instance return buffer.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrBisUnpackBigTerminateInd(uint8_t *pReason, uint16_t *pInstance, const uint8_t *pBuf)
{
  const uint8_t len = sizeof(uint8_t) + LL_BIG_TERMINATE_IND_PDU_LEN;

  pBuf++;   /* Opcode */
  BSTREAM_TO_UINT8 (*pReason, pBuf);
  BSTREAM_TO_UINT16(*pInstance, pBuf);

  return len;
}
