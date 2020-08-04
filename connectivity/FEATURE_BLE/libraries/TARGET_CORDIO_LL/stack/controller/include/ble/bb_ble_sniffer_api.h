/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Packet sniffer interface file.
 *
 *  Copyright (c) 2019 Arm Ltd. All Rights Reserved.
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

#ifndef BB_BLE_SNIFFER_API_H
#define BB_BLE_SNIFFER_API_H

#include "wsf_types.h"
#include "bb_api.h"
#include "pal_bb_ble.h"
#include "ll_defs.h"
#include "bb_ble_api.h"

#ifndef BB_SNIFFER_ENABLED
/*! \brief     Sniffer enabled for controller */
#define BB_SNIFFER_ENABLED          FALSE
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief   Maximum number of buffer. */
#define BB_SNIFFER_MAX_NUM_BUF 20

/*! \brief   Max packet size for a sniffer packet. */
#define BB_SNIFFER_MAX_PKT_SIZE 28

/*! \brief   Output methods of sniffer. */
enum
{
  BB_SNIFFER_OUTPUT_HCI_TOKEN,       /*!< Output over HCI through tokens. */
  BB_SNIFFER_OUTPUT_TOTAL_METHODS,   /*!< Total output methods. */

  BB_SNIFFER_OUTPUT_NULL_METHOD      /*!< Null output method. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief   Sniffer Packet types (Used to parse packet for sniffer). */
enum
{
  BB_SNIFF_PKT_TYPE_TX,   /*!< Transmit packets. */
  BB_SNIFF_PKT_TYPE_RX    /*!< Receive packets. */
};

/*! \brief   Sniffer packet metadata. */
typedef struct
{
  uint8_t           type;        /*!< TX/RX. */
  uint8_t           status;      /*!< TX/RX status. */
  uint8_t           state;       /*!< State of the operation. */
  uint32_t          timeStamp;   /*!< Timestamp. */
  uint8_t           rssi;        /*!< Signal power (TxPower, or RSSI). */
  PalBbBleChan_t    chan;        /*!< channelization params. */
} BbBleSnifferMeta_t;

/*! \brief   Sniffer data packet. */
typedef struct
{
  BbBleSnifferMeta_t    meta;                         /*!< Metadata header. */
  uint8_t               hdr[LL_DATA_HDR_MAX_LEN];     /*!< Header raw data. */
} BbBleDataSniffPkt_t;

/*! \brief   Sniffer advertisement packet. */
typedef struct
{
  BbBleSnifferMeta_t    meta;                         /*!< Metadata header. */
  uint8_t               hdr[LL_ADV_HDR_LEN];          /*!< Header raw data. */
} BbBleAdvSniffPkt_t;

/*! \brief      Sniffer generic packet. */
typedef struct
{
  union
  {
    BbBleSnifferMeta_t  meta;     /*!< Metadata. */
    BbBleDataSniffPkt_t dataPkt;  /*!< Data packet. */
    BbBleAdvSniffPkt_t  advPkt;   /*!< Advertising packet. */
  } pktType;                      /*!< Packet type. */
} BbBleSnifferPkt_t;

/*! \brief      Sniffer output call signature. */
typedef void (*bbSnifferFn_t)(BbBleSnifferPkt_t *pPkt);

/*! \brief      Sniffer get packet call signature. */
typedef BbBleSnifferPkt_t* (*bbSnifferGetPktFn_t)(void);

/*! \brief   HCI output context. */
typedef struct
{
  uint16_t               bufIdx;                          /*!< Current packet buffer index. */
  BbBleSnifferPkt_t      pktBuf[BB_SNIFFER_MAX_NUM_BUF];  /*!< Sotrage for packet buffer. */
} BbBleSnifferHciCtx_t;

/*! \brief   Sniffer context. */
typedef struct
{
  /* Control variables. */
  bool_t                 enabled;         /*!< Enable status of sniffer. */
  bbSnifferGetPktFn_t    snifferGetPktFn; /*!< Sniffer get packet function callback. */
  bbSnifferFn_t          snifferOutCb;    /*!< Sniffer output callback. */
  uint32_t               packetCtr;       /*!< Number of packets processed. */

  /* Temporary storage. */
  uint8_t                txBuf[LL_DATA_HDR_MAX_LEN];      /*!< Temporary storage for conn tx buf. */
  uint8_t                chanIdx;                         /*!< Temporary storage for adv channel index. */

  /* Output contexts. */
  union
  {
    BbBleSnifferHciCtx_t  hci;           /*!< HCI Output context. */
  } outputCtx;                           /*!< Output context union. */
} bbSnifferCtx_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern bbSnifferCtx_t bbSnifferCtx;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

uint8_t BbBleInitSniffer(uint8_t outMethod, bool_t enable);
void bbBleSnifferMstScanPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t *pPktData);
void bbBleSnifferMstAuxScanPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t *pPktData);
void bbBleSnifferSlvAdvPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t * pPktData);
void bbBleSnifferSlvAuxAdvPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t * pPktData);
void bbBleSnifferMstPerScanPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t *pPktData);
void bbBleSnifferConnPktHandler(BbOpDesc_t * pBod, BbBleSnifferPkt_t * pPktData);

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_TESTER_API_H */
