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
 *  \brief BLE protocol scheduler implementation file.
 */
/*************************************************************************************************/

/* Common internal definitions. */
#include "../../common/sch/sch_int.h"

#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "ll_defs.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Compute the duration in microseconds of an data BLE packet.
 *
 *  \param      phy     PHY.
 *  \param      len     Packet length.
 *
 *  \return     Time in microseconds for the packet to be transferred on the medium.
 *
 *  \note       This value includes preamble, access address, CRC and T_IFS.
 */
/*************************************************************************************************/
uint32_t SchBleCalcDataPktDurationUsec(uint8_t phy, uint16_t len)
{
  uint32_t duration = LL_BLE_TIFS_US;

  switch (phy)
  {
    case BB_PHY_BLE_1M:
      duration += LL_DATA_LEN_TO_TIME_1M(len);
      break;
    case BB_PHY_BLE_2M:
      duration += LL_DATA_LEN_TO_TIME_2M(len);
      break;
    case BB_PHY_BLE_CODED:
      duration += LL_DATA_LEN_TO_TIME_CODED_S8(len);
      break;
  }

  return duration;
}

/*************************************************************************************************/
/*!
 *  \brief      Compute the duration in microseconds of an advertising BLE packet.
 *
 *  \param      phy         PHY.
 *  \param      phyOptions  PHY Options.
 *  \param      len         Packet length.
 *
 *  \return     Time in microseconds for the packet to be transferred on the medium.
 *
 *  \note       This value includes preamble, access address, CRC and T_IFS.
 */
/*************************************************************************************************/
uint32_t SchBleCalcAdvPktDurationUsec(uint8_t phy, uint8_t phyOptions, uint16_t len)
{
  uint32_t usec;
  switch (phy)
  {
    case BB_PHY_BLE_1M:
    default:
      usec = (LL_PREAMBLE_LEN_1M + LL_AA_LEN + LL_CRC_LEN + len) * LL_BLE_US_PER_BYTE_1M;
      break;
    case BB_PHY_BLE_2M:
      usec = (LL_PREAMBLE_LEN_2M + LL_AA_LEN + LL_CRC_LEN + len) * LL_BLE_US_PER_BYTE_2M;
      break;
    case BB_PHY_BLE_CODED:
      usec  = (LL_PREAMBLE_LEN_CODED_BITS + (LL_AA_LEN * 8) + LL_CI_LEN_BITS + LL_TERM1_LEN_BITS ) * LL_BLE_US_PER_BIT_CODED_S8;
      usec += ((phyOptions == BB_PHY_OPTIONS_BLE_S2) ? LL_BLE_US_PER_BIT_CODED_S2 : LL_BLE_US_PER_BIT_CODED_S8) *
              ((len * 8) + (LL_CRC_LEN * 8) + LL_TERM2_LEN_BITS);
      break;
  }
  return usec;
}

/*************************************************************************************************/
/*!
 *  \brief      Compute the duration in microseconds of an auxiliary BLE packet.
 *
 *  \param      phy         PHY.
 *  \param      phyOptions  PHY Options.
 *  \param      len     Packet length.
 *
 *  \return     Time in microseconds for the packet to be transferred on the medium.
 *
 *  \note       This value includes preamble, access address, CRC and MAFS.
 */
/*************************************************************************************************/
uint32_t SchBleCalcAuxPktDurationUsec(uint8_t phy, uint8_t phyOptions, uint16_t len)
{
  return SchBleCalcAdvPktDurationUsec(phy, phyOptions, len) + LL_BLE_MAFS_US;
}

/*************************************************************************************************/
/*!
 *  \brief      Compute the duration of the given BOD.
 *
 *  \param      pBod    Operation to compute duration.
 *
 *  \return     None.
 *
 *  Compute all the actions time and assign the duration of the given BOD.
 */
/*************************************************************************************************/
void SchBleCalcAdvOpDuration(BbOpDesc_t *pBod)
{
  uint32_t usec;

  WSF_ASSERT(pBod->protId == BB_PROT_BLE);

  BbBleData_t * const pBle = pBod->prot.pBle;

  switch (pBle->chan.opType)
  {
    case BB_BLE_OP_MST_ADV_EVENT:
    {
      WSF_ASSERT(pBle->chan.txPhy != BB_PHY_BLE_2M);
      WSF_ASSERT(pBle->chan.rxPhy != BB_PHY_BLE_2M);

      BbBleMstAdvEvent_t * const pAdv = &pBle->op.mstAdv;
      switch (pBle->chan.rxPhy)
      {
        case BB_PHY_BLE_1M:
        default:
          usec = LL_ADVB_MAX_TIME_1M;
          break;
        case BB_PHY_BLE_CODED:
          /* Assume longest time, coded S8. */
          usec = LL_ADVB_MAX_TIME_S8;
          break;
      }

      if (pAdv->pTxReqBuf)
      {
        usec += LL_BLE_TIFS_US;
        /* Coded PHY doesn't have pTxReqBuf on primary channel. BB_PHY_OPTIONS_DEFAULT is OK. */
        usec += SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, BB_PHY_OPTIONS_DEFAULT, pBle->op.mstAdv.txReqLen);

        if (pAdv->pRxRspBuf)
        {
          usec += LL_BLE_TIFS_US;
          switch (pBle->chan.rxPhy)
          {
            case BB_PHY_BLE_1M:
            default:
              usec += LL_ADVB_MAX_TIME_1M;
              break;
            case BB_PHY_BLE_CODED:
              /* Assume longest time, coded S8. */
              usec += LL_ADVB_MAX_TIME_S8;
              break;
          }
        }
      }
      break;
    }
    case BB_BLE_OP_SLV_ADV_EVENT:
    {
      WSF_ASSERT(pBle->chan.txPhy != BB_PHY_BLE_2M);
      WSF_ASSERT(pBle->chan.rxPhy != BB_PHY_BLE_2M);

      BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;
      unsigned int numChan;

      numChan  = (pAdv->advChMap & (1 << 0)) ? 1 : 0;
      numChan += (pAdv->advChMap & (1 << 1)) ? 1 : 0;
      numChan += (pAdv->advChMap & (1 << 2)) ? 1 : 0;

      WSF_ASSERT(numChan > 0);

      usec  = numChan * SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, pBle->chan.initTxPhyOptions, pBle->op.slvAdv.txAdvLen);
      usec += (numChan - 1) * BbGetSchSetupDelayUs();

      if (pAdv->pRxReqBuf)
      {
        usec += LL_BLE_TIFS_US;
        switch (pBle->chan.rxPhy)
        {
          case BB_PHY_BLE_1M:
          default:
            usec += LL_ADVB_MAX_TIME_1M;
            break;
          case BB_PHY_BLE_CODED:
            /* Assume longest time, coded S8. */
            usec += LL_ADVB_MAX_TIME_S8;
            break;
        }
        if (pAdv->pTxRspBuf)
        {
          usec += LL_BLE_TIFS_US;
          /* Coded PHY doesn't have pTxRspBuf on primary channel. BB_PHY_OPTIONS_DEFAULT is OK. */
          usec += SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, BB_PHY_OPTIONS_DEFAULT, pBle->op.mstAdv.txReqLen);
        }
      }
      break;
    }
    case BB_BLE_OP_MST_AUX_ADV_EVENT:
    {
      BbBleMstAuxAdvEvent_t * const pAdv = &pBle->op.mstAuxAdv;

      if (pAdv->isInit == FALSE)
      {
        /* Scan due to discovery. */
        switch (pBle->chan.rxPhy)
        {
          case BB_PHY_BLE_1M:
          default:
            usec = LL_EXT_ADVB_MAX_TIME_1M;
            break;
          case BB_PHY_BLE_2M:
            usec = LL_EXT_ADVB_MAX_TIME_2M;
            break;
          case BB_PHY_BLE_CODED:
            /* Assume longest time, coded S8. */
            usec = LL_EXT_ADVB_MAX_TIME_S8;
            break;
        }

        if (pAdv->pTxAuxReqBuf)
        {
          usec += LL_BLE_TIFS_US;
          /*if TIFS has preference, it should use this value. Otherwise, it will assume longest time, codes S8. */
          usec += SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, (pBle->chan.tifsTxPhyOptions != BB_PHY_OPTIONS_DEFAULT) ? pBle->chan.tifsTxPhyOptions : BB_PHY_OPTIONS_BLE_S8, pAdv->txAuxReqLen);

          usec += LL_BLE_TIFS_US;
          switch (pBle->chan.rxPhy)
          {
            case BB_PHY_BLE_1M:
            default:
              usec += LL_EXT_ADVB_MAX_TIME_1M;
              break;
            case BB_PHY_BLE_2M:
              usec += LL_EXT_ADVB_MAX_TIME_2M;
              break;
            case BB_PHY_BLE_CODED:
              /* Assume longest time, coded S8. */
              usec += LL_EXT_ADVB_MAX_TIME_S8;
              break;
          }
        }
      }
      else
      {
        /* Scan due to initiation */
        switch (pBle->chan.rxPhy)
        {
          case BB_PHY_BLE_1M:
          default:
            usec = LL_EXT_ADVB_MAX_TIME_1M;
            break;
          case BB_PHY_BLE_2M:
            usec = LL_EXT_ADVB_MAX_TIME_2M;
            break;
          case BB_PHY_BLE_CODED:
            /* Assume longest time, coded S8. */
            usec = LL_EXT_ADVB_MAX_TIME_S8;
            break;
        }
        if (pAdv->pTxAuxReqBuf)
        {
          usec += LL_BLE_TIFS_US;
          /* Ff TIFS has preference, it should use this value. Otherwise, it will assume longest time, coded S8. */
          usec += SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, (pBle->chan.tifsTxPhyOptions != BB_PHY_OPTIONS_DEFAULT) ? pBle->chan.tifsTxPhyOptions : BB_PHY_OPTIONS_BLE_S8, LL_ADV_HDR_LEN + LL_CONN_IND_PDU_LEN);      /* aux_conn_req */
          usec += LL_BLE_TIFS_US;
          /* Assume longest time, coded S8. */
          usec += SchBleCalcAdvPktDurationUsec(pBle->chan.rxPhy, BB_PHY_OPTIONS_BLE_S8, LL_ADV_HDR_LEN + LL_CONN_RSP_PDU_LEN);  /* aux_conn_rsp */
        }
      }
      break;
    }
    case BB_BLE_OP_SLV_AUX_ADV_EVENT:
    {
      BbBleSlvAuxAdvEvent_t * const pAdv = &pBle->op.slvAuxAdv;

      usec = SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, pBle->chan.initTxPhyOptions, pAdv->txAuxAdvPdu[0].len +  pAdv->txAuxAdvPdu[1].len);

      if (pAdv->pRxAuxReqBuf)
      {
        usec += LL_BLE_TIFS_US;
        switch (pBle->chan.rxPhy)
        {
          case BB_PHY_BLE_1M:
          default:
            usec += LL_ADVB_MAX_TIME_1M;
            break;
          case BB_PHY_BLE_2M:
            usec += LL_ADVB_MAX_TIME_2M;
            break;
          case BB_PHY_BLE_CODED:
            /* Assume longest time, coded S8. */
            usec += LL_ADVB_MAX_TIME_S8;
            break;
        }

        usec += LL_BLE_TIFS_US;
        /* If TIFS has preference, it should use this value. Otherwise, it will assume longest time, coded S8. */
        usec += SchBleCalcAuxPktDurationUsec(pBle->chan.txPhy, (pBle->chan.tifsTxPhyOptions != BB_PHY_OPTIONS_DEFAULT) ? pBle->chan.tifsTxPhyOptions : BB_PHY_OPTIONS_BLE_S8, pAdv->txAuxRspPdu[0].len +  pAdv->txAuxRspPdu[1].len);
      }
      else
      {
        /* Use MAFS spacing instead of TIFS. */
        usec += LL_BLE_MAFS_US;
      }

      /* Do not reserve AUX_CHAIN_IND, transmission only if scheduler has opportunity. */
      break;
    }
    case BB_BLE_OP_SLV_PER_ADV_EVENT:
    {
      switch (pBle->chan.txPhy)
      {
        case BB_PHY_BLE_1M:
        default:
          usec = LL_EXT_ADVB_MAX_TIME_1M;
          break;
        case BB_PHY_BLE_2M:
          usec = LL_EXT_ADVB_MAX_TIME_2M;
          break;
        case BB_PHY_BLE_CODED:
          /* Assume longest time, coded S8. */
          usec = LL_EXT_ADVB_MAX_TIME_S8;
          break;
      }

      /* Do not reserve AUX_CHAIN_IND, transmission allowed only if scheduler has opportunity. */
      break;
    }
    case BB_BLE_OP_MST_PER_SCAN_EVENT:
    {
      switch (pBle->chan.rxPhy)
      {
        case BB_PHY_BLE_1M:
        default:
          usec = LL_EXT_ADVB_MAX_TIME_1M;
          break;
        case BB_PHY_BLE_2M:
          usec = LL_EXT_ADVB_MAX_TIME_2M;
          break;
        case BB_PHY_BLE_CODED:
          /* Assume longest time, coded S8. */
          usec = LL_EXT_ADVB_MAX_TIME_S8;
          break;
      }
      break;
    }

    default:
      usec = 0;
      break;
  }

  pBod->minDurUsec = usec;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the next BLE master connection due time.
 *
 *  \param      pDueTime    Due time return buffer.
 *
 *  \return     TRUE if dueTime is valid, FALSE otherwise.
 *
 *  Find the next master connection operation and return the due time.
 */
/*************************************************************************************************/
bool_t SchBleGetNextMstConnDueTime(uint32_t *pDueTime)
{
  BbOpDesc_t *pCur = schCb.pHead;

  while (pCur != NULL)
  {
    /* Only consider master connection events. */
    if ((pCur->protId == BB_PROT_BLE) &&
        (pCur->prot.pBle->chan.opType == BB_BLE_OP_MST_CONN_EVENT))
    {
      *pDueTime = pCur->due;
      return TRUE;
    }

    pCur = pCur->pNext;
  }

  return FALSE;
}
