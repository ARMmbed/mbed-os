/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer controller common interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_API_H
#define LCTR_API_H

#include "ll_api.h"
#include "ll_defs.h"
#include "bb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Message dispatch handler types. */
enum
{
  LCTR_DISP_CONN_IND,                   /*!< Advertising channel connection indication dispatch handler type. */
  LCTR_DISP_CONN,                       /*!< Connection message dispatch handler type. */
  LCTR_DISP_SCAN,                       /*!< Scan message dispatch handler type. */
  LCTR_DISP_INIT,                       /*!< Initiate message dispatch handler type. */
  LCTR_DISP_ADV,                        /*!< Advertising message dispatch handler type. */
  LCTR_DISP_TEST,                       /*!< Test message dispatch handler type. */
  LCTR_DISP_PRIV,                       /*!< Privacy message dispatch handler type. */
  LCTR_DISP_EXT_SCAN,                   /*!< Extended Scan message dispatch handler type. */
  LCTR_DISP_EXT_ADV,                    /*!< Extended Advertising message dispatch handler type. */
  LCTR_DISP_EXT_INIT,                   /*!< Extended Initiate message dispatch handler type. */
  LCTR_DISP_PER_ADV,                    /*!< Periodic Advertising message dispatch handler type. */
  LCTR_DISP_PER_CREATE_SYNC,            /*!< Periodic Create Sync message dispatch handler type. */
  LCTR_DISP_TRANFER_SYNC,               /*!< Periodic Sync Transfer message dispatch handler type. */
  LCTR_DISP_PER_SCAN,                   /*!< Periodic Scanning message dispatch handler type. */
  LCTR_DISP_ACAD,                       /*!< ACAD message dispatch handler type (currently only used by slave). */
  LCTR_DISP_CIS,                        /*!< Connected Isochronous Stream dispatch handler type. */
  LCTR_DISP_BIG_BCST,                   /*!< Broadcast Isochronous Group broadcasting message dispatch handler type. */
  LCTR_DISP_BIG_SYNC,                   /*!< Broadcast Isochronous Group synchronization message dispatch handler type. */
  LCTR_DISP_TOTAL,                      /*!< Total number of dispatch handlers. */
  /* Special IDs */
  LCTR_DISP_FIRST_SM  = LCTR_DISP_CONN_IND+1,   /*!< First state machine. */
  LCTR_DISP_LAST_SM   = LCTR_DISP_TOTAL-1,      /*!< Last state machine. */
  LCTR_DISP_BCST      = 0xFF                    /*!< Broadcast message type. */
};

/*! \brief      Broadcast task messages. */
enum
{
  LCTR_MSG_RESET                        /*!< Reset API message. */
};

/*! \brief      Task event mask bit positions. */
enum
{
  LCTR_EVENT_RX_PENDING,                /*!< Receive data PDU pending. */
  LCTR_EVENT_TX_PENDING,                /*!< Transmit data PDU pending. */
  LCTR_EVENT_TX_COMPLETE,               /*!< Transmit data PDU completed. */
  LCTR_EVENT_CIS_TX_PENDING,            /*!< Transmit data PDU pending. */
  LCTR_EVENT_CIS_RX_PENDING,            /*!< Receive data PDU pending. */
  LCTR_EVENT_ISO_TX_COMPLETE,           /*!< Transmit ISO SDU completed. */
  LCTR_EVENT_RX_ADVB,                   /*!< Receive AdvB PDU completed. */
  LCTR_EVENT_RX_DIRECT_ADVB,            /*!< Receive direct AdvB PDU completed. */
  LCTR_EVENT_RX_SCAN_REQ,               /*!< Receive scan request PDU completed.  */
  LCTR_EVENT_SC_GENERATE_P256_KEY_PAIR, /*!< Generate P-256 public/private key pair. */
  LCTR_EVENT_SC_GENERATE_DHKEY,         /*!< Generate Diffie-Hellman key. */
  LCTR_EVENT_TOTAL                      /*!< Total number of event handlers. */
};

/*! \brief      Waiting host reply bitmask. */
enum
{
  LCTR_HOST_REPLY_CONN_PARAM_REQ    = (1 << 0),     /*!< Waiting for host to submit a connection parameter request reply. */
  LCTR_HOST_REPLY_LTK_REQ           = (1 << 1),     /*!< Waiting for host to submit a LTK request reply. */
  LCTR_HOST_REPLY_CIS_REQ           = (1 << 2),     /*!< Waiting for host to submit a CIS request reply. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Common message structure passed to event handler. */
typedef struct
{
  uint16_t          handle;             /*!< Handle. */
  uint8_t           dispId;             /*!< Dispatch ID. */
  uint8_t           event;              /*!< Event ID. */
} lctrMsgHdr_t;

/*! \brief      Channel map update message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint64_t          chanMap;            /*!< Channel map. */
} lctrChanMapUpdate_t;

/*! \brief      BIG created message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint8_t           bigHandle;          /*!< BIG handle. */
} lctrBigCreated_t;

/*! \brief      Connect request PDU. */
typedef struct
{
  uint64_t          initAddr;           /*!< Scanner's address. */
  uint64_t          advAddr;            /*!< Advertiser's address. */

  uint32_t          accessAddr;         /*!< Connection access address. */
  uint32_t          crcInit;            /*!< CRC initialization value. */
  uint64_t          chanMask;           /*!< Channel mask. */
  uint16_t          interval;           /*!< connInterval value. */
  uint16_t          latency;            /*!< connSlaveLatency value. */
  uint16_t          timeout;            /*!< connSupervisionTimeout value. */
  uint8_t           txWinSize;          /*!< transmitWindowSize value. */
  uint16_t          txWinOffset;        /*!< transmitWindowOffset value. */
  uint8_t           hopInc;             /*!< hopIncrement value. */
  uint8_t           masterSca;          /*!< Master sleep clock accuracy. */
} lctrConnInd_t;

/*! \brief      Connection establish. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  lctrConnInd_t     connInd;            /*!< Connection indication. */
  uint32_t          connIndEndTsUsec;   /*!< Connection indication packet end timestamp in microseconds. */
  uint8_t           peerIdAddrType;     /*!< Peer identity address type. */
  uint8_t           usedChSel;          /*!< Used channel selection. */
  uint8_t           phy;                /*!< PHY selection. */
  bool_t            sendAdvSetTerm;     /*!< Send Advertising Set Termination event. */
  uint8_t           numExtAdvEvents;    /*!< Number of completed extended advertising events. */
  bool_t            isAuxConnReq;       /*!< True if AUX_CON_REQ is received, False if CONN_IND is received. */
  uint64_t          peerIdAddr;         /*!< Peer identity address. */
  uint64_t          peerRpa;            /*!< Peer RPA. */
  uint64_t          localRpa;           /*!< Local RPA. */
} lctrConnEstablish_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief       Call signature for periodic enabled check function */
typedef bool_t (*LctrIsPerAdvEnabledFn_t)(uint8_t handle);

/*! \brief      Function pointer for periodic advertising enable check */
extern LctrIsPerAdvEnabledFn_t LctrPerAdvEnabled;

/*! \brief       Call signature to update CIS channel map */
typedef void (*LctrUpdateCisChanMapFn_t)(uint16_t aclHandle);

/*! \brief      Function pointer to update CIS channel map */
extern LctrUpdateCisChanMapFn_t LctrUpdateCisChanMapFn;

/*! \brief       Call signature for extended scan enabled check function. */
typedef bool_t (*LctrExtCheckFn_t)(uint8_t scanPhy);

/*! \brief      Function pointer for extended scan enable check. */
extern LctrExtCheckFn_t LctrMstExtScanEnabled;

/*! \brief      Function pointer for extended advertising init enable check. */
extern LctrExtCheckFn_t LctrMstExtInitEnabled;

/*! \brief      Call signature for periodic sync pending check. */
typedef bool_t (*LctrPerSyncPendFn_t)(void);

/*! \brief      Function pointer for periodic sync pending check. */
extern LctrPerSyncPendFn_t LctrMstPerSyncPending;

/*! \brief      Runtime configuration. */
extern const LlRtCfg_t *pLctrRtCfg;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
uint16_t LctrInitAdvSetMem(uint8_t *pFreeMem, uint32_t freeMemSize);
uint16_t LctrInitExtScanMem(uint8_t *pFreeMem, uint32_t freeMemSize);
uint16_t LctrInitConnMem(uint8_t *pFreeMem, uint32_t freeMemSize);
void LctrSetSupStates(void);

/* Task */
void LctrMsgDispatcher(lctrMsgHdr_t *pMsg);
void LctrEventHandler(uint8_t event);

/* Control */
uint8_t LctrSetChannelClass(uint64_t chanMap);

/*! \} */    /* LL_LCTR_API */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_H */
