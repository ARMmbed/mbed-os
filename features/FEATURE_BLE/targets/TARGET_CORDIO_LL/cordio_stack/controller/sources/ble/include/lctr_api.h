/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer controller common interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
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
  LCTR_DISP_PER_SCAN,                   /*!< Periodic Scanning message dispatch handler type. */
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
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Common message structure passed to event handler. */
typedef struct
{
  uint16_t          handle;             /*!< Handle. */
  uint8_t           dispId;             /*!< Dispatch ID. */
  uint8_t           event;              /*!< PDU ID. */
} lctrMsgHdr_t;

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
  uint32_t          connIndEndTs;       /*!< Connection indication packet end timestamp. */
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

/* Runtime configuration. */
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

/*! \} */    /* LL_LCTR_API */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_H */
