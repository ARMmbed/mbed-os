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
 * \brief BLE baseband interface file.
 */
/*************************************************************************************************/

#ifndef BB_BLE_API_OP_H
#define BB_BLE_API_OP_H

#include "bb_api.h"
#include "pal_bb_ble.h"
#include "bb_ble_api_pdufilt.h"
#include "ll_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BB_API_BLE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Convert BLE protocol ticks to BB ticks. */
#define BB_BLE_TO_BB_TICKS(n)       BB_US_TO_BB_TICKS((n) * LL_BLE_US_PER_TICK)

/*! \brief      Increment statistics counter. */
#define BB_INC_STAT(s)              s++

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Operation types. */
enum
{
  BB_BLE_OP_TEST_TX,                /*!< Continuous Tx test mode. */
  BB_BLE_OP_TEST_RX,                /*!< Continuous Rx test mode. */
  BB_BLE_OP_MST_ADV_EVENT,          /*!< Master advertising event. */
  BB_BLE_OP_SLV_ADV_EVENT,          /*!< Slave advertising event. */
  BB_BLE_OP_MST_CONN_EVENT,         /*!< Master connection event. */
  BB_BLE_OP_SLV_CONN_EVENT,         /*!< Slave connection event. */
  BB_BLE_OP_MST_AUX_ADV_EVENT,      /*!< Master auxiliary advertising event. */
  BB_BLE_OP_SLV_AUX_ADV_EVENT,      /*!< Slave auxiliary advertising event. */
  BB_BLE_OP_SLV_PER_ADV_EVENT,      /*!< Slave periodic advertising event. */
  BB_BLE_OP_MST_PER_SCAN_EVENT,     /*!< Master periodic scanning event. */
  BB_BLE_OP_MST_CIS_EVENT,          /*!< Master CIS event. */
  BB_BLE_OP_SLV_CIS_EVENT,          /*!< Slave CIS event. */
  BB_BLE_OP_NUM                     /*!< Total number of operations. */
};

/*! \brief      Maximum request PDU length (MAX(LL_SCAN_REQ_PDU_LEN, LL_CONN_IND_PDU_LEN)). */
#define BB_REQ_PDU_MAX_LEN          (LL_ADV_HDR_LEN + LL_CONN_IND_PDU_LEN)

/*! \brief      Guard time at the end of a scan window to the next BOD. Backoff one advertise data exchange. */
#define BB_SCAN_GUARD_US            (LL_ADV_PKT_MAX_USEC  + LL_BLE_TIFS_US + \
                                     LL_SCAN_REQ_MAX_USEC + LL_BLE_TIFS_US + \
                                     LL_SCAN_RSP_MAX_USEC + \
                                     BbGetSchSetupDelayUs())

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Pre-execute callback signature. */
typedef void (*BbBlePreExec_t)(BbOpDesc_t *pBod);

/*! \brief      Execute callback signature. */
typedef void (*BbBleExec_t)(BbOpDesc_t *pBod);

/*! \brief      Cancel callback signature. */
typedef void (*BbBleCancel_t)(BbOpDesc_t *pBod);

/*! \brief      Advertising PDU transmit setup call signature. */
typedef void (*BbBleTxAdvSetup_t)(BbOpDesc_t *pBod, uint32_t advTxTime);

/*! \brief      Chain indication PDU transmit setup call signature. */
typedef uint32_t (*BbBleTxAuxSetup_t)(BbOpDesc_t *pBod, bool_t isChainInd);

/*! \brief      Returns TRUE if an scan request/response required. */
typedef bool_t (*BbBleAdvComp_t)(BbOpDesc_t *pBod, const uint8_t *pBuf);

/*! \brief      Rx completion post processing call signature. */
typedef void (*BbBleAdvPost_t)(BbOpDesc_t *pBod, const uint8_t *pBuf);

/*! \brief      Chain indication PDU received call signature. */
typedef uint32_t (*BbBleRxChain_t)(BbOpDesc_t *pBod, const uint8_t *pBuf);

/*! \brief      Chain indication PDU received post call signature. */
typedef bool_t (*BbBleRxChainPost_t)(BbOpDesc_t *pBod, const uint8_t *pBuf);

/*! \brief      Data transmit completion callback signature. */
typedef void (*BbBleTxDataComp_t)(BbOpDesc_t *pBod, uint8_t status);

/*! \brief      Data receive completion callback signature. */
typedef void (*BbBleRxDataComp_t)(BbOpDesc_t *pBod, uint8_t *pBuf, uint8_t status);

/*! \brief      CIS check whether to continue current operation call signature. */
typedef uint32_t (*BbBleCisCheckContOp_t)(BbOpDesc_t *pBod, bool_t *pNewCisCtx);

/*! \brief      CIS post execute callback signature. */
typedef void (*BbBleCisPostExec_t)(BbOpDesc_t *pBod, uint8_t status);

/*! \brief      CIS data receive completion callback signature. */
typedef void (*BbBleCisRxDataComp_t)(BbOpDesc_t *pBod, uint8_t *pBuf, uint8_t status);

/*! \brief      Test completion callback signature. */
typedef bool_t (*BbBleTestComp_t)(BbOpDesc_t *pBod, uint8_t status);

/*! \brief      Periodic PDU Rx complete call signature. */
typedef uint32_t (*BbBlePerComp_t)(BbOpDesc_t *pBod, const uint8_t *pBuf, uint8_t status);

/*! \brief      Periodic PDU Rx complete post call signature. */
typedef bool_t (*BbBlePerPostComp_t)(BbOpDesc_t *pBod, const uint8_t *pBuf);

/*!
 * \brief       Master advertising event operation data (\ref BB_BLE_OP_MST_ADV_EVENT).
 *
 * \note        BB assumes maximum adversing and scan response payload is 39 bytes.
 */
typedef struct
{
  uint8_t                 *pRxAdvBuf;         /*!< Advertising buffer. */
  uint8_t                 *pTxReqBuf;         /*!< Scan request buffer. */
  uint8_t                 *pRxRspBuf;         /*!< Scan response buffer. */

  BbBlePreExec_t          preExecCback;       /*!< Pre-execute callback. */
  BbBleAdvComp_t          rxAdvCback;         /*!< Advertising completion callback. */
  BbBleAdvPost_t          rxAdvPostCback;     /*!< Advertising completion post processing callback. */
  BbBleAdvComp_t          txReqCback;         /*!< Scan request completion callback. */
  BbBleAdvComp_t          rxRspCback;         /*!< Scan response completion callback. */

  uint8_t                 txReqLen;           /*!< Scan request buffer length. */

  uint8_t                 scanChMap;          /*!< Scan channel map. */

  /* Return parameters. */
  int8_t                  advRssi;            /*!< RSSI of advertisement. */
  uint8_t                 advRxPhyOptions;    /*!< Rx PHY options. */
  uint32_t                advCrc;             /*!< CRC of advertisement. */
  uint32_t                advStartTs;         /*!< Start of advertising packet timestamp. */
  uint32_t                elapsedUsec;        /*!< Elapsed time of a single scan window in microseconds. */

  /* Filter results. */
  bbBlePduFiltResults_t   filtResults;        /*!< Results from PDU filtering. */
} BbBleMstAdvEvent_t;

/*!
 * \brief       Slave advertising event operation data (\ref BB_BLE_OP_SLV_ADV_EVENT).
 *
 * \note        BB assumes maximum scan request payload is 39 bytes.
 */
typedef struct
{
  uint8_t                 *pTxAdvBuf;         /*!< Advertising buffer. */
  uint8_t                 *pRxReqBuf;         /*!< Scan request buffer (must be size of BB_REQ_PDU_MAX_LEN). */
  uint8_t                 *pTxRspBuf;         /*!< Scan response buffer. */

  BbBleTxAdvSetup_t       txAdvSetupCback;    /*!< Adv PDU transmit setup callback. */
  BbBleAdvComp_t          rxReqCback;         /*!< Scan/Connect request receive completion callback. */
  BbBleAdvPost_t          rxReqPostCback;     /*!< Scan/Connect request receive post processing callback. */

  uint8_t                 txAdvLen;           /*!< Advertising buffer length. */
  uint8_t                 txRspLen;           /*!< Scan response buffer length. */

  uint8_t                 firstAdvChIdx;      /*!< first advertising channel index. */

  uint8_t                 advChMap;           /*!< Advertising channel map. */

  /* Return parameters. */
  uint32_t                reqStartTs;         /*!< Start of request packet timestamp. */

  /* Filter results. */
  bbBlePduFiltResults_t   filtResults;        /*!< Results from PDU filtering. */
} BbBleSlvAdvEvent_t;

/*! \brief       Master auxiliary advertising event operation data (\ref BB_BLE_OP_MST_AUX_ADV_EVENT). */
typedef struct
{
  uint8_t                 *pTxAuxReqBuf;      /*!< Scan request buffer. */

  BbBleAdvComp_t          rxAuxAdvCback;      /*!< Advertising completion callback. */
  BbBleAdvComp_t          rxAuxRspCback;      /*!< Scan response completion callback. */
  BbBleRxChain_t          rxAuxChainCback;    /*!< Chain completion callback. */
  BbBleRxChainPost_t      rxAuxChainPostCback;/*!< Chain completion post callback. */

  uint8_t                 txAuxReqLen;        /*!< Scan request buffer length. */

  uint32_t                rxSyncDelayUsec;    /*!< Receive timeout in microseconds. */
  bool_t                  isInit;             /*!< TRUE if the event is for an initiation. */

  /* Return parameters. */
  int8_t                  auxAdvRssi;         /*!< RSSI of advertisement. */
  uint8_t                 auxRxPhyOptions;    /*!< Rx PHY options. */
  uint32_t                auxAdvCrc;          /*!< CRC of advertisement. */
  uint32_t                auxStartTs;         /*!< Start of auxiliary advertising packet timestamp. */

  /* Filter results. */
  bbBlePduFiltResults_t   filtResults;        /*!< Results from PDU filtering. */
} BbBleMstAuxAdvEvent_t;

/*! \brief       Master periodic scanning event operation data (\ref BB_BLE_OP_MST_PER_SCAN_EVENT). */
typedef struct
{
  BbBlePerComp_t          rxPerAdvCback;      /*!< Periodic scanning completion callback. */
  BbBlePerPostComp_t      rxPerAdvPostCback;  /*!< Periodic scanning completion post callback. */
  uint32_t                rxSyncDelayUsec;    /*!< Receive timeout in microseconds. */

  /* Return parameters. */
  uint8_t                 rxStatus;           /*!< RX status. */
  int8_t                  perAdvRssi;         /*!< RSSI of advertisement. */
  uint8_t                 perRxPhyOptions;    /*!< Rx PHY options. */
  uint32_t                perAdvCrc;          /*!< CRC of advertisement. */
  uint32_t                perStartTs;         /*!< Start of periodic advertising packet timestamp. */
  bool_t                  perIsFirstTs;       /*!< True if it is the first timestamp for a serial of periodic packets. */

} BbBleMstPerScanEvent_t;

/*! \brief       Slave auxiliary advertising event operation data (\ref BB_BLE_OP_SLV_AUX_ADV_EVENT). */
typedef struct
{
  /* TODO BbBleSlvAuxAdvEvent_t hide buffer descriptors in BB layer. */
  PalBbBleTxBufDesc_t     txAuxAdvPdu[2];     /*!< Advertising PDU descriptor. */
  uint8_t                 *pRxAuxReqBuf;      /*!< Auxiliary request buffer (must be size of BB_REQ_PDU_MAX_LEN). */
  PalBbBleTxBufDesc_t     txAuxRspPdu[2];     /*!< Response PDU descriptor. */
  PalBbBleTxBufDesc_t     txAuxChainPdu[2];   /*!< Auxiliary chain PDU descriptor. */

  BbBleAdvComp_t          rxAuxReqCback;      /*!< Auxiliary request receive completion callback. */
  BbBleAdvPost_t          rxAuxReqPostCback;  /*!< Auxiliary scan/connect request receive post processing callback. */
  BbBleTxAuxSetup_t       txAuxSetupCback;    /*!< Auxiliary chain indication setup callback. */

  uint8_t                 auxAdvCh;           /*!< Advertising channel map. */

  /* Return parameters. */
  uint8_t                 auxRxPhyOptions;    /*!< Rx PHY options. */
  uint32_t                auxReqStartTs;      /*!< Start of request packet timestamp. */

  /* Filter results. */
  bbBlePduFiltResults_t   filtResults;        /*!< Results from PDU filtering. */
} BbBleSlvAuxAdvEvent_t;

/*! \brief      Connection event operation data (\ref BB_BLE_OP_MST_CONN_EVENT). */
typedef struct
{
  BbBleExec_t             execCback;          /*!< Execute callback. */
  BbBleCancel_t           cancelCback;        /*!< Cancel callback. */
  BbBleTxDataComp_t       txDataCback;        /*!< Transmit completion callback. */
  BbBleRxDataComp_t       rxDataCback;        /*!< Receive completion callback. */

  /* Return parameters. */
  int8_t                  rssi;               /*!< RSSI of the last received packet. */
  uint8_t                 rxPhyOptions;       /*!< Rx PHY options. */
} BbBleMstConnEvent_t;

/*! \brief      Connection event operation data (\ref BB_BLE_OP_SLV_CONN_EVENT). */
typedef struct
{
  uint32_t                rxSyncDelayUsec;    /*!< Receive timeout in microseconds. */
  BbBleExec_t             execCback;          /*!< Execute callback. */
  BbBleCancel_t           cancelCback;        /*!< Cancel callback. */
  BbBleTxDataComp_t       txDataCback;        /*!< Transmit completion callback. */
  BbBleRxDataComp_t       rxDataCback;        /*!< Receive completion callback. */

  /* Return parameters. */
  uint32_t                startTs;            /*!< Start timestamp of the first received packet. */
  int8_t                  rssi;               /*!< RSSI of the last received packet. */
  uint8_t                 rxPhyOptions;       /*!< Rx PHY options. */
} BbBleSlvConnEvent_t;

/*! \brief      CIS master event operation data (\ref BB_BLE_OP_MST_CIS_EVENT). */
typedef struct
{
  BbBleCisCheckContOp_t   checkContOpCback;   /*!< Check whether to continue current operation callback. */
  BbBleExec_t             execCback;          /*!< Execute callback. */
  BbBleExec_t             contExecCback;      /*!< Continue execute callback. */
  BbBleCisPostExec_t      postSubEvtCback;    /*!< Post subevent callback. */
  BbBleCancel_t           cancelCback;        /*!< Cancel callback. */
  BbBleTxDataComp_t       txDataCback;        /*!< Transmit completion callback. */
  BbBleCisRxDataComp_t    rxDataCback;        /*!< Receive completion callback. */
  /* Return parameters. */
  int8_t                  rssi;               /*!< RSSI of the last received packet. */
  uint8_t                 rxPhyOptions;       /*!< Rx PHY options. */
} BbBleMstCisEvent_t;

/*! \brief      CIS slave event operation data (\ref BB_BLE_OP_SLV_CIS_EVENT). */
typedef struct
{
  BbBleCisCheckContOp_t   checkContOpCback;   /*!< Check whether to continue current operation callback. */
  BbBleExec_t             execCback;          /*!< Execute callback. */
  BbBleExec_t             contExecCback;      /*!< Continue execute callback. */
  BbBleCisPostExec_t      postSubEvtCback;    /*!< Post subevent callback. */
  BbBleCancel_t           cancelCback;        /*!< Cancel callback. */
  BbBleTxDataComp_t       txDataCback;        /*!< Transmit completion callback. */
  BbBleRxDataComp_t       rxDataCback;        /*!< Receive completion callback. */

  /* Return parameters. */
  uint32_t                startTs;            /*!< Start timestamp of the first received packet. */
  int8_t                  rssi;               /*!< RSSI of the last received packet. */
  uint8_t                 rxPhyOptions;       /*!< Rx PHY options. */
  uint32_t                rxSyncDelayUsec;    /*!< Receive timeout in microseconds. */
} BbBleSlvCisEvent_t;

/*! \brief      Continuous transmit operation data (\ref BB_BLE_OP_TEST_TX). */
typedef struct
{
  BbBleTestComp_t         testCback;          /*!< Test callback. */

  uint8_t                 *pTxBuf;            /*!< Transmit data buffer. */
  uint16_t                txLen;              /*!< Transmit data buffer length. */
  uint16_t                pktInterUsec;       /*!< Transmit packet interval. */
} BbBleTestTx_t;

/*! \brief      Continuous receive operation data (\ref BB_BLE_OP_TEST_RX). */
typedef struct
{
  uint32_t                rxSyncDelayUsec;    /*!< Synchronization delay in microseconds. */
  BbBleTestComp_t         testCback;          /*!< Test callback. */

  uint8_t                 *pRxBuf;            /*!< Receive data buffer. */
  uint16_t                rxLen;              /*!< Receive data buffer length. */
} BbBleTestRx_t;

/*! \brief      Bluetooth Low Energy protocol specific operation parameters. */
typedef struct BbBleData_tag
{
  PalBbBleChan_t          chan;               /*!< Channelization parameters. */
  bbBlePduFiltParams_t    pduFilt;            /*!< PDU filter parameters. */

  union
  {
    BbBleMstAdvEvent_t    mstAdv;             /*!< Master advertising event data. */
    BbBleSlvAdvEvent_t    slvAdv;             /*!< Slave advertising event data. */
    BbBleMstAuxAdvEvent_t mstAuxAdv;          /*!< Master auxiliary advertising event data. */
    BbBleSlvAuxAdvEvent_t slvAuxAdv;          /*!< Slave auxiliary advertising event data. */
    BbBleSlvAuxAdvEvent_t slvPerAdv;          /*!< Slave periodic advertising event data. */
    BbBleMstConnEvent_t   mstConn;            /*!< Master connection event data. */
    BbBleSlvConnEvent_t   slvConn;            /*!< Slave connection event data. */
    BbBleMstPerScanEvent_t mstPerScan;        /*!< Master periodic scanning event data. */
    BbBleMstCisEvent_t    mstCis;             /*!< Master CIS event data. */
    BbBleSlvCisEvent_t    slvCis;             /*!< Slave CIS event data. */
    BbBleTestTx_t         testTx;             /*!< Transmit test data. */
    BbBleTestRx_t         testRx;             /*!< Receive test data. */
  } op;                                       /*!< Operation specific data. */
} BbBleData_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Transmit data PDU at next transmit slot.
 *
 *  \param      descs       Array of transmit buffer descriptor.
 *  \param      cnt         Number of descriptors.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the call context of
 *              \ref BbBleSlvConnEvent_t::rxDataCback callback routine.
 */
/*************************************************************************************************/
void BbBleTxData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Set receive data buffer for next receive slot.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Maximum length of data buffer.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the call context of
 *              \ref BbBleSlvConnEvent_t::rxDataCback callback routine.
 *
 *  \note       BB must always call the BbSlvConnEvent_t::rxDataCback callback routine of the
 *              currently executing BOD with the given buffer.
 */
/*************************************************************************************************/
void BbBleRxData(uint8_t *pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Transmit CIS PDU at next transmit slot.
 *
 *  \param      descs       Array of transmit buffer descriptor.
 *  \param      cnt         Number of descriptors.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the call context of
 *              \ref BbBleMstCisEvent_t::rxDataCback or \ref BbBleSlvCisEvent_t::rxDataCback
 *              callback routine.
 */
/*************************************************************************************************/
void BbBleCisTxData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Set receive data buffer for next receive slot.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Maximum length of data buffer.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the call context of
 *              \ref BbBleSlvCisEvent_t::rxDataCback callback routine.
 *
 *  \note       BB must always call the BbBleSlvCisEvent_t::rxDataCback callback routine of the
 *              currently executing BOD with the given buffer.
 */
/*************************************************************************************************/
void BbBleCisRxData(uint8_t *pBuf, uint16_t len);
/*! \} */    /* BB_API_BLE */

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_API_OP_H */
