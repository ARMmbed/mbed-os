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
 * \brief BLE protocol scheduler implementation file.
 */
/*************************************************************************************************/

/* Common internal definitions. */
#include "../../common/sch/sch_int.h"

#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "ll_defs.h"
#include "ll_math.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_math.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Estimated header length for AUX_ADV_IND with no chained packets. */
#define SCH_AUX_ADV_NO_CHAIN_HDR_LEN    (LL_ADV_HDR_LEN + 11)         /* Includes extended header len, Flags, AdvA, Txpower, ADI. */

/*! \brief      Estimated header length for AUX_ADV_IND with chained packets. */
#define SCH_AUX_ADV_W_CHAIN_HDR_LEN     (LL_ADV_HDR_LEN + 14)         /* Includes extended header len, Flags, AdvA, Txpower, ADI, AuxPtr. */

/*! \brief      Estimated header length for periodic advertisement with no chained packets. */
#define SCH_PER_ADV_NO_CHAIN_HDR_LEN    (LL_ADV_HDR_LEN + 3)          /* Includes extended header len, Flags, Txpower. */

/*! \brief      Estimated header length for periodic advertisement with chained packets. */
#define SCH_PER_ADV_W_CHAIN_HDR_LEN     (LL_ADV_HDR_LEN + 8)          /* Includes extended header len, Flags, Txpower, AuxPtr, ADI. */

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
              (((LL_CRC_LEN + len) * 8) + LL_TERM2_LEN_BITS);
      break;
  }
  return usec;
}

/*************************************************************************************************/
/*!
 *  \brief      Compute the duration of a periodic advertising event.
 *
 *  \param      txPhy       Advertising PHY being used.
 *  \param      fragLen     Fragmentation length.
 *  \param      addMafDelay MAF offset to add on top of MAF.
 *  \param      len         Data length to calculate.
 *  \param      worseCase   True if we want to calculate the worst case, false for predicted case.
 *  \param      phyOptions  Phy option to use when calculating coded timing.
 */
/*************************************************************************************************/
uint32_t SchBleCalcPerAdvDurationUsec(uint8_t txPhy, uint8_t fragLen, uint16_t addMafDelay, uint16_t len, bool_t worstCase, uint8_t phyOptions)
{
  static uint16_t perAdvHdrMaxLen = LL_ADV_HDR_LEN + LL_EXT_ADV_HDR_MAX_LEN;

  uint16_t headerLenNoChain = (worstCase == TRUE) ? perAdvHdrMaxLen : SCH_PER_ADV_NO_CHAIN_HDR_LEN;
  uint16_t headerLenChain = (worstCase == TRUE) ? perAdvHdrMaxLen : SCH_PER_ADV_W_CHAIN_HDR_LEN;

  uint16_t maxDataBytesNoChain;
  uint16_t maxDataBytesWithChain;
  if (fragLen == 0)
  {
    maxDataBytesNoChain = LL_EXT_ADVB_MAX_LEN - headerLenNoChain;
    maxDataBytesWithChain = LL_EXT_ADVB_MAX_LEN - headerLenChain;
  }
  else
  {
    maxDataBytesNoChain = fragLen;
    maxDataBytesWithChain = fragLen;
  }

  uint32_t usec = 0;

  if (len <= maxDataBytesNoChain)
  {
    usec = SchBleCalcAdvPktDurationUsec(txPhy, phyOptions, headerLenNoChain + len);
  }
  else
  {
    uint16_t remDataLen = len % maxDataBytesWithChain;
    uint16_t numFragWithAuxPtr = (len / maxDataBytesWithChain) - ((remDataLen) ? 0 : 1);

    usec = numFragWithAuxPtr * SchBleCalcAdvPktDurationUsec(txPhy, phyOptions, headerLenChain + maxDataBytesWithChain);
    usec += numFragWithAuxPtr * (LL_BLE_MAFS_US + addMafDelay);

    if (remDataLen)
    {
      usec += SchBleCalcAdvPktDurationUsec(txPhy, phyOptions, headerLenNoChain + remDataLen);
    }
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
 *  \param      fragLen Fragmentation length.
 *
 *  \return     None.
 *
 *  Compute all the actions time and assign the duration of the given BOD.
 */
/*************************************************************************************************/
void SchBleCalcAdvOpDuration(BbOpDesc_t *pBod, uint8_t fragLen)
{
  uint32_t usec = 0;

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
      uint32_t  pktDuration = SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, pBle->chan.initTxPhyOptions, pBle->op.slvAdv.txAdvLen);

      WSF_ASSERT(pBle->chan.txPhy != BB_PHY_BLE_2M);
      WSF_ASSERT(pBle->chan.rxPhy != BB_PHY_BLE_2M);

      BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;
      unsigned int numChan;

      numChan  = (pAdv->advChMap & (1 << 0)) ? 1 : 0;
      numChan += (pAdv->advChMap & (1 << 1)) ? 1 : 0;
      numChan += (pAdv->advChMap & (1 << 2)) ? 1 : 0;

      WSF_ASSERT(numChan > 0);

      usec = (numChan - 1) * SchBleGetAlignedAuxOffsUsec(pktDuration + BbGetSchSetupDelayUs());
      usec += pktDuration;   /* For the last channel. */

      if (pAdv->pRxReqBuf)
      {
        usec += (LL_BLE_TIFS_US * numChan);
        switch (pBle->chan.rxPhy)
        {
          case BB_PHY_BLE_1M:
          default:
            usec += (LL_ADVB_MAX_TIME_1M * numChan);
            break;
          case BB_PHY_BLE_CODED:
            /* Assume longest time, coded S8. */
            usec += (LL_ADVB_MAX_TIME_S8 * numChan);
            break;
        }
        if (pAdv->pTxRspBuf)
        {
          usec += (LL_BLE_TIFS_US * numChan);
          /* Coded PHY doesn't have pTxRspBuf on primary channel. BB_PHY_OPTIONS_DEFAULT is OK. */
          usec += (SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, BB_PHY_OPTIONS_DEFAULT, pBle->op.mstAdv.txReqLen) * numChan);
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
            /* Setting min & max duration differently to avoid conflict with other BOD's. */
            /* Min = 3.8ms for normal size(up to 50 bytes)      */
            /* Max = 17.4ms for maximum size(up to 255 bytes)   */
            /* When RX data size is larger than 50 bytes, it may stomp on the next high priority BOD's. */
            // TODO: We need HW support(Radio interrupt when packet header is received)  to properly fix the problem.
            usec = LL_EXT_ADVB_NORMAL_TIME_S8;
            pBod->maxDurUsec = LL_EXT_ADVB_MAX_TIME_S8;
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
      /* The calculation is estimate here because we do not have information how optional header bytes are going to be filled later. */
      uint16_t dataLen = 0;
      uint16_t extHeadLen = 0;
      uint16_t maxDataBytesNoChain = (fragLen == 0) ? (LL_EXT_ADVB_MAX_LEN - SCH_AUX_ADV_NO_CHAIN_HDR_LEN) : fragLen;
      uint16_t maxDataBytesWithChain = (fragLen == 0) ? (LL_EXT_ADVB_MAX_LEN - SCH_AUX_ADV_W_CHAIN_HDR_LEN) : fragLen;
      BbBleSlvAuxAdvEvent_t * const pAuxAdv = &pBle->op.slvAuxAdv;

      if (pAuxAdv->pRxAuxReqBuf)    /* Scannable advertising */
      {
        usec += SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, pBle->chan.initTxPhyOptions, SCH_AUX_ADV_NO_CHAIN_HDR_LEN);
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

        usec += LL_BLE_TIFS_US;
      }

      extHeadLen = WSF_MAX(pAuxAdv->txAuxAdvPdu[0].len, SCH_AUX_ADV_NO_CHAIN_HDR_LEN);

      if (pBod->pDataLen)
      {
        /* scan response data length if scannable advertising, ext advertising data length otherwise. */
        dataLen = *pBod->pDataLen;
      }

      if (dataLen <= maxDataBytesNoChain)
      {
        usec += SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, pBle->chan.initTxPhyOptions, extHeadLen + dataLen);
      }
      else
      {
        uint16_t remDataLen;
        uint16_t numFragWithAuxPtr = dataLen / maxDataBytesWithChain;

        usec += numFragWithAuxPtr * SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, pBle->chan.initTxPhyOptions, SCH_AUX_ADV_W_CHAIN_HDR_LEN + maxDataBytesWithChain);
        usec += numFragWithAuxPtr * LL_BLE_MAFS_US;

        remDataLen = dataLen - (numFragWithAuxPtr * maxDataBytesWithChain);
        if (remDataLen)
        {
          usec += SchBleCalcAdvPktDurationUsec(pBle->chan.txPhy, pBle->chan.initTxPhyOptions, SCH_AUX_ADV_NO_CHAIN_HDR_LEN + remDataLen);
        }
      }


      break;
    }
    case BB_BLE_OP_SLV_PER_ADV_EVENT:
    {
      /* The calculation for the periodic event is estimate here because we do not have information how optional header bytes are going to be filled later. */
      uint16_t dataLen = 0;
      if (pBod->pDataLen)
      {
        dataLen = *pBod->pDataLen;
      }

      usec = SchBleCalcPerAdvDurationUsec(pBle->chan.txPhy, 0, 0, dataLen, FALSE, pBle->chan.initTxPhyOptions);
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
          /* Setting min & max duration differently so that other BOD's can run after minDuration. */
          /* Min = 3.8ms for normal size(up to 50 bytes)      */
          /* Max = 17.4ms for maximum size(up to 255 bytes)   */
          /* When RX data size is larger than 50 bytes, it may stomp on the next high priority BOD's. */
          // TODO: We need HW support(Radio interrupt when packet header is received)  to properly fix the problem.
          usec = LL_EXT_ADVB_NORMAL_TIME_S8;
          pBod->maxDurUsec = LL_EXT_ADVB_MAX_TIME_S8;
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

/*************************************************************************************************/
/*!
 *  \brief  Get next aligned auxiliary offset in usec from the given aux offset.
 *
 *  \param  auxOffsUsec   Auxiliary offset in usec.
 *
 *  \return Aligned auxiliary offset in usec.
 */
/*************************************************************************************************/
uint32_t SchBleGetAlignedAuxOffsUsec(uint32_t auxOffsUsec)
{
  uint16_t auxOffset;
  uint32_t newAuxOffsetUsec;

  if (auxOffsUsec < LL_30_USEC_OFFS_MAX_USEC)
  {
    auxOffset = LL_MATH_DIV_30(auxOffsUsec);
    newAuxOffsetUsec = auxOffset * 30;

    if (newAuxOffsetUsec < auxOffsUsec)
    {
      newAuxOffsetUsec = newAuxOffsetUsec + 30;
    }
  }
  else
  {
    auxOffset = LL_MATH_DIV_300(auxOffsUsec);
    newAuxOffsetUsec = auxOffset * 300;

    if (newAuxOffsetUsec < auxOffsUsec)
    {
      newAuxOffsetUsec = newAuxOffsetUsec + 300;
    }
  }

  return newAuxOffsetUsec;
}
