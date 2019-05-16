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
 *  \brief Link layer controller slave data channel packet implementation file.
 */
/*************************************************************************************************/

#include "ll_math.h"
#include "lctr_int_conn.h"
#include "util/bstream.h"
#include "hci_defs.h"
#include "wsf_math.h"
#include <string.h>
#include <stdint.h>

/*************************************************************************************************/
/*!
 *  \brief  Check whether the channel map mask is valid.
 *
 *  \param  chanMask    Channel map mask.
 *
 *  \return TRUE if channel map is valid, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t lctrIsChanMaskValid(uint64_t chanMask)
{
  /* Valid channels are between bit 0 and bit 36. */
  if (chanMask & (~LL_CHAN_DATA_ALL))
  {
    return FALSE;
  }

  /* The minimum number of used channels shall be 2. */
  if (LlMathGetNumBitsSet(chanMask) < LL_MIN_NUM_CHAN_DATA)
  {
    return FALSE;
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Check whether the connection update indication parameters are valid.
 *
 *  \param  pParam    Connection update parameter.
 *
 *  \return TRUE if parameters are valid, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t lctrIsConnUpdateParamValid(lctrConnUpdInd_t *pParam)
{
  /* In the range of 7.5ms to 4s. */
  if ((pParam->interval < LL_MIN_CONN_INTERVAL) || (pParam->interval > LL_MAX_CONN_INTERVAL))
  {
    return FALSE;
  }

  /* In the range of 1.25ms to the lesser of 10ms and (connInterval - 1.25ms). */
  if ((pParam->txWinSize < LL_MIN_TX_WIN_SIZE) ||
      (pParam->txWinSize > WSF_MIN(LL_MAX_TX_WIN_SIZE, (pParam->interval - 1))))
  {
    return FALSE;
  }

  /* In the range of 0ms to the connInterval. */
  if (pParam->txWinOffset > (pParam->interval))
  {
    return FALSE;
  }

  /* In the range of 100ms to 32000ms. And it shall be larger than (1 + connSlaveLatency) * connInterval * 2. */
  if ((pParam->timeout < LL_MIN_SUP_TIMEOUT) || (pParam->timeout > LL_MAX_SUP_TIMEOUT) ||
      ((pParam->timeout << 2) <= ((1 + pParam->latency) * pParam->interval)))
  {
    return FALSE;
  }

  /* In the range of 0 to the ((connSupervisionTimeout / (connInterval*2)) - 1), checked above already.
   * And it shall be less than 500. */
  if (pParam->latency > LL_MAX_CONN_LATENCY)
  {
    return FALSE;
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Check whether the set minimum number of used channels parameters are valid.
 *
 *  \param  pParam    Set minimum number of used channels parameter.
 *
 *  \return TRUE if parameters are valid, FALSE otherwise.
 */
/*************************************************************************************************/
static inline bool_t lctrIsSetMinUsedChanParamValid(lctrMinUsedChanInd_t *pParam)
{
  /* At least one bit shall be set. */
  if (pParam->phys == 0)
  {
    return FALSE;
  }

  /* The minimum number of used channels shall be between 2 - 37. */
  if ((pParam->minUsedChan < LL_MIN_NUM_CHAN_DATA) ||
      (pParam->minUsedChan > LL_MAX_NUM_CHAN_DATA))
  {
    return FALSE;
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack an ACL header.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pHdr        Unpacked packet header.
 *
 *  \return Header length.
 */
/*************************************************************************************************/
uint8_t lctrPackAclHdr(uint8_t *pBuf, const lctrAclHdr_t *pHdr)
{
  const uint8_t len = sizeof(uint32_t);

  uint16_t flags = 0;

  flags |= (pHdr->connHandle) <<  0;
  flags |= (pHdr->pktBound  ) << 12;
  UINT16_TO_BSTREAM(pBuf, flags);

  UINT16_TO_BSTREAM(pBuf, pHdr->len);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack an data channel PDU header.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pHdr        Unpacked PDU header.
 *
 *  \return Header length.
 */
/*************************************************************************************************/
uint8_t lctrPackDataPduHdr(uint8_t *pBuf, const lctrDataPduHdr_t *pHdr)
{
  const uint8_t len = LL_DATA_HDR_LEN;

  uint16_t hdr = 0;

  hdr |= (pHdr->llid    ) << 0;
  hdr |= (pHdr->nesn & 1) << 2;
  hdr |= (pHdr->sn   & 1) << 3;
  hdr |= (pHdr->md   & 1) << 4;
  hdr |= (pHdr->len     ) << 8;
  UINT16_TO_BSTREAM(pBuf, hdr);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a connection update request PDU.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pPdu        Unpacked control data PDU.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrPackConnUpdInd(uint8_t *pBuf, const lctrConnUpdInd_t *pPdu)
{
  const uint8_t len = LL_CONN_UPD_IND_PDU_LEN;

  UINT8_TO_BSTREAM (pBuf, LL_PDU_CONN_UPDATE_IND);
  UINT8_TO_BSTREAM (pBuf, pPdu->txWinSize);
  UINT16_TO_BSTREAM(pBuf, pPdu->txWinOffset);
  UINT16_TO_BSTREAM(pBuf, pPdu->interval);
  UINT16_TO_BSTREAM(pBuf, pPdu->latency);
  UINT16_TO_BSTREAM(pBuf, pPdu->timeout);
  UINT16_TO_BSTREAM(pBuf, pPdu->instant);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack an ACL header.
 *
 *  \param  pHdr        Unpacked packet header.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Header length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackAclHdr(lctrAclHdr_t *pHdr, const uint8_t *pBuf)
{
  const uint8_t len = HCI_ACL_HDR_LEN;

  uint16_t flags;

  BSTREAM_TO_UINT16(flags, pBuf);
  pHdr->connHandle = (flags >>  0) & 0x0FFF;
  pHdr->pktBound   = (flags >> 12) & 0x0003;

  BSTREAM_TO_UINT16(pHdr->len, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a data channel PDU header.
 *
 *  \param  pHdr        Unpacked header.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return Header length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackDataPduHdr(lctrDataPduHdr_t *pHdr, const uint8_t *pBuf)
{
  const uint8_t len = LL_DATA_HDR_LEN;

  uint16_t hdr;

  BSTREAM_TO_UINT16(hdr, pBuf);

  pHdr->llid = (hdr >> 0) & LL_DATA_HDR_LLID_MSK;
  pHdr->nesn = (hdr >> 2) & 0x0001;
  pHdr->sn   = (hdr >> 3) & 0x0001;
  pHdr->md   = (hdr >> 4) & 0x0001;
  pHdr->len  = (hdr >> 8) & LL_DATA_HDR_LEN_MSK;

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a connection update request PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackConnUpdateIndPdu(lctrConnUpdInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_CONN_UPD_IND_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8 (pPdu->txWinSize, pBuf);
  BSTREAM_TO_UINT16(pPdu->txWinOffset, pBuf);
  BSTREAM_TO_UINT16(pPdu->interval, pBuf);
  BSTREAM_TO_UINT16(pPdu->latency, pBuf);
  BSTREAM_TO_UINT16(pPdu->timeout, pBuf);
  BSTREAM_TO_UINT16(pPdu->instant, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a connection update request PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackChanMapIndPdu(lctrChanMapInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_CHAN_MAP_IND_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT40(pPdu->chanMask, pBuf);
  BSTREAM_TO_UINT16(pPdu->instant, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a terminate indication PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackTerminateIndPdu(lctrTermInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_TERMINATE_IND_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8(pPdu->errorCode, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a unknown response PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackUnknownRspPdu(lctrUnknownRsp_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_UNKNOWN_RSP_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8(pPdu->unknownType, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a feature request or response PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackFeaturePdu(lctrFeat_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_FEATURE_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT64(pPdu->featSet, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a reject indication PDU.
 *
 *  \param  pReason     Reason code.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackRejectIndPdu(uint8_t *pReason, const uint8_t *pBuf)
{
  const uint8_t len = LL_REJECT_IND_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8(*pReason, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a version indication PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackVersionIndPdu(lctrVerInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_VERSION_IND_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8(pPdu->versNr, pBuf);
  BSTREAM_TO_UINT16(pPdu->compId, pBuf);
  BSTREAM_TO_UINT16(pPdu->subVersNr, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack an extended reject indication PDU.
 *
 *  \param  pPdu        Unpacked control data PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackRejectExtIndPdu(lctrRejInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_REJECT_EXT_IND_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8(pPdu->opcode, pBuf);
  BSTREAM_TO_UINT8(pPdu->reason, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a data length request or response PDU.
 *
 *  \param  pPdu        Unpacked data length PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackDataLengthPdu(lctrDataLen_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_DATA_LEN_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT16(pPdu->maxRxLen, pBuf);
  BSTREAM_TO_UINT16(pPdu->maxRxTime, pBuf);
  BSTREAM_TO_UINT16(pPdu->maxTxLen, pBuf);
  BSTREAM_TO_UINT16(pPdu->maxTxTime, pBuf);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a connection parameter request or response PDU.
 *
 *  \param  pConnParam  Connection parameter request/response values.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackConnParamPdu(lctrConnParam_t *pConnParam, const uint8_t *pBuf)
{
  const uint8_t len = LL_CONN_PARAM_PDU_LEN;

  unsigned int i;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT16(pConnParam->connIntervalMin, pBuf);
  BSTREAM_TO_UINT16(pConnParam->connIntervalMax, pBuf);
  BSTREAM_TO_UINT16(pConnParam->connLatency, pBuf);
  BSTREAM_TO_UINT16(pConnParam->supTimeout, pBuf);

  BSTREAM_TO_UINT8 (pConnParam->prefPeriod, pBuf);
  BSTREAM_TO_UINT16(pConnParam->refConnEvtCnt, pBuf);

  for (i = 0; i < LCTR_OFFSET_COUNT; i++)
  {
    BSTREAM_TO_UINT16(pConnParam->offset[i], pBuf);
  }

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a PHY request or response PDU.
 *
 *  \param  pPdu        Unpacked PHY PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackPhyPdu(lctrPhy_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_PHY_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8(pPdu->txPhys, pBuf);
  BSTREAM_TO_UINT8(pPdu->rxPhys, pBuf);

  /* Mask out the RFU bits for PHYs. */
  pPdu->txPhys &= LL_ALL_PHYS_MSK;
  pPdu->rxPhys &= LL_ALL_PHYS_MSK;

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a PHY update indication PDU.
 *
 *  \param  pPdu        Unpacked PHY update indication PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackPhyUpdateIndPdu(lctrPhyUpdInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_PHY_UPD_IND_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8 (pPdu->masterToSlavePhy, pBuf);
  BSTREAM_TO_UINT8 (pPdu->slaveToMasterPhy, pBuf);
  BSTREAM_TO_UINT16(pPdu->instant, pBuf);

  /* Mask out the RFU bits for PHYs. */
  pPdu->masterToSlavePhy &= LL_ALL_PHYS_MSK;
  pPdu->slaveToMasterPhy &= LL_ALL_PHYS_MSK;

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Unpack a set minimum number of used channels indication PDU.
 *
 *  \param  pPdu        Unpack a set minimum number of used channels indication PDU.
 *  \param  pBuf        Packed packet buffer.
 *
 *  \return PDU length.
 */
/*************************************************************************************************/
uint8_t lctrUnpackMinUsedChanIndPdu(lctrMinUsedChanInd_t *pPdu, const uint8_t *pBuf)
{
  const uint8_t len = LL_MIN_USED_CHAN_PDU_LEN;

  pBuf += 1;        /* skip opcode */
  BSTREAM_TO_UINT8 (pPdu->phys, pBuf);
  BSTREAM_TO_UINT8 (pPdu->minUsedChan, pBuf);

  /* Mask out the RFU bits for PHYs. */
  pPdu->phys &= LL_ALL_PHYS_MSK;

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Decode an LE-C channel buffer.
 *
 *  \param  pPdu    Destination unpacked PDU.
 *  \param  pBuf    Source packed packet buffer.
 *  \param  role    Role.
 *
 *  \return LL error code or success.
 */
/*************************************************************************************************/
uint8_t lctrDecodeCtrlPdu(lctrDataPdu_t *pPdu, const uint8_t *pBuf, uint8_t role)
{
  uint8_t result = LL_SUCCESS;

  pBuf += lctrUnpackDataPduHdr(&pPdu->hdr, pBuf);
  pPdu->opcode = *pBuf;

  switch (pPdu->opcode)
  {
    case LL_PDU_CONN_UPDATE_IND:
      if (role == LL_ROLE_MASTER)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ((lctrUnpackConnUpdateIndPdu(&pPdu->pld.connUpdInd, pBuf) != pPdu->hdr.len) ||
          (lctrIsConnUpdateParamValid(&pPdu->pld.connUpdInd) == FALSE))
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_CONN_PARAM_RSP:
      if (role == LL_ROLE_SLAVE)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      /* no break */
    case LL_PDU_CONN_PARAM_REQ:
      if ((lmgrCb.features & LL_FEAT_CONN_PARAM_REQ_PROC) == 0)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if (lctrUnpackConnParamPdu(&pPdu->pld.connParamReqRsp, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_CHANNEL_MAP_IND:
      if (role == LL_ROLE_MASTER)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ((lctrUnpackChanMapIndPdu(&pPdu->pld.chanMapInd, pBuf) != pPdu->hdr.len) ||
          (lctrIsChanMaskValid(pPdu->pld.chanMapInd.chanMask) == FALSE))
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_LENGTH_REQ:
    case LL_PDU_LENGTH_RSP:
      if (lctrUnpackDataLengthPdu(&pPdu->pld.lenRsp, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_TERMINATE_IND:
      if (lctrUnpackTerminateIndPdu(&pPdu->pld.termInd, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_UNKNOWN_RSP:
      if (lctrUnpackUnknownRspPdu(&pPdu->pld.unknownRsp, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_FEATURE_REQ:
      if (role == LL_ROLE_MASTER)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      /* no break */
    case LL_PDU_FEATURE_RSP:
      if (lctrUnpackFeaturePdu(&pPdu->pld.featReqRsp, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_SLV_FEATURE_REQ:
      if (role == LL_ROLE_SLAVE)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ((lmgrCb.features & LL_FEAT_SLV_INIT_FEAT_EXCH) == 0)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if (lctrUnpackFeaturePdu(&pPdu->pld.featReqRsp, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_VERSION_IND:
      if (lctrUnpackVersionIndPdu(&pPdu->pld.verInd, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_REJECT_IND:
      pPdu->pld.rejInd.opcode = LL_PDU_UNSPECIFIED;
      if (lctrUnpackRejectIndPdu(&pPdu->pld.rejInd.reason, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_REJECT_EXT_IND:
      if ((lmgrCb.features & LL_FEAT_EXT_REJECT_IND) == 0)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if (lctrUnpackRejectExtIndPdu(&pPdu->pld.rejInd, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_PHY_RSP:
      if (role == LL_ROLE_SLAVE)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      /* no break */
    case LL_PDU_PHY_REQ:
      if ((lmgrCb.features & (LL_FEAT_LE_2M_PHY | LL_FEAT_LE_CODED_PHY)) == 0)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if (lctrUnpackPhyPdu(&pPdu->pld.phyReq, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_PHY_UPDATE_IND:
      if (role == LL_ROLE_MASTER)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ((lmgrCb.features & (LL_FEAT_LE_2M_PHY | LL_FEAT_LE_CODED_PHY)) == 0)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if (lctrUnpackPhyUpdateIndPdu(&pPdu->pld.phyUpdInd, pBuf) != pPdu->hdr.len)
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;
    case LL_PDU_MIN_USED_CHAN_IND:
      if (role == LL_ROLE_SLAVE)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ((lmgrCb.features & LL_FEAT_MIN_NUM_USED_CHAN) == 0)
      {
        return LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      }
      if ((lctrUnpackMinUsedChanIndPdu(&pPdu->pld.minUsedChanInd, pBuf) != pPdu->hdr.len) ||
          (lctrIsSetMinUsedChanParamValid(&pPdu->pld.minUsedChanInd) == FALSE))
      {
        return LL_ERROR_CODE_INVALID_LMP_PARAMS;
      }
      break;

    default:
      result = LL_ERROR_CODE_UNKNOWN_LMP_PDU;
      break;
  }

  return result;
}
