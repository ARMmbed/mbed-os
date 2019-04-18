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
 * \brief Link layer controller master encryption packet implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_conn.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief  Unpack an encrypted request PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
static uint8_t lctrUnpackEncReqPdu(lctrEncReq_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_ENC_REQ_LEN;

  pBuf += 1;        /* skip opcode */

  memcpy(pPdu->rand, pBuf, sizeof(pPdu->rand));
  pBuf += sizeof(pPdu->rand);

  BSTREAM_TO_UINT16(pPdu->ediv, pBuf);

  memcpy(pPdu->skd_m, pBuf, sizeof(pPdu->skd_m));
  pBuf += sizeof(pPdu->skd_m);

  memcpy(pPdu->iv_m, pBuf, sizeof(pPdu->iv_m));
  /* pBuf += sizeof(pPdu->iv_m); */

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack an encrypted response PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
static uint8_t lctrUnpackEncRspPdu(lctrEncRsp_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_ENC_RSP_LEN;

  pBuf += 1;        /* skip opcode */
  memcpy(pPdu, pBuf, sizeof(*pPdu));

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Decode a LE-C channel buffer.
 *
 *  \param  pPdu    Destination unpacked PDU.
 *  \param  pBuf    Source packed packet buffer.
 *  \param  role    Role.
 *
 *  \return LL error code or success.
 */
/*************************************************************************************************/
uint8_t lctrDecodeEncPdu(lctrDataPdu_t *pPdu, const uint8_t *pBuf, uint8_t role)
{
  uint8_t result = LL_SUCCESS;

  pBuf += lctrUnpackDataPduHdr(&pPdu->hdr, pBuf);
  pPdu->opcode = *pBuf;

  switch (pPdu->opcode)
  {
    case LL_PDU_ENC_REQ:
      if (role == LL_ROLE_MASTER)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if (lctrUnpackEncReqPdu(&pPdu->pld.encReq, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      break;
    case LL_PDU_ENC_RSP:
      if (role == LL_ROLE_SLAVE)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if (lctrUnpackEncRspPdu(&pPdu->pld.encRsp, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      break;
    case LL_PDU_START_ENC_REQ:
      if (role == LL_ROLE_SLAVE)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ( pPdu->hdr.len != LL_START_ENC_LEN)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      break;
    case LL_PDU_PAUSE_ENC_REQ:
      if (role == LL_ROLE_MASTER)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ( pPdu->hdr.len != LL_PAUSE_ENC_LEN)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      break;
    case LL_PDU_START_ENC_RSP:
      if ( pPdu->hdr.len != LL_START_ENC_LEN)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      break;
    case LL_PDU_PAUSE_ENC_RSP:
      if ( pPdu->hdr.len != LL_PAUSE_ENC_LEN)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      break;
    case LL_PDU_PING_REQ:
    case LL_PDU_PING_RSP:
      if ((lmgrCb.features & LL_FEAT_LE_PING) == 0)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ( pPdu->hdr.len != LL_PING_PDU_LEN)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      break;
    default:
      result = LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      break;
  }

  return result;
}
