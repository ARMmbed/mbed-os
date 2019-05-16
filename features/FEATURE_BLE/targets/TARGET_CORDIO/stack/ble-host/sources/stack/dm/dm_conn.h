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
 *  \brief DM connection management module.
 */
/*************************************************************************************************/
#ifndef DM_CONN_H
#define DM_CONN_H

#include "dm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Uninitialized HCI handle */
#define DM_CONN_HCI_HANDLE_NONE             0xFFFF

/* Action set initializer */
#define DM_CONN_ACT_SET_INIT(n)             ((n) << 4)

/* Get action set ID from action */
#define DM_CONN_ACT_SET_ID(action)          ((action) >> 4)

/* Get action ID from action */
#define DM_CONN_ACT_ID(action)              ((action) & 0x0F)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! DM conn event handler messages for state machine */
enum
{
  /* messages from API */
  DM_CONN_MSG_API_OPEN = DM_MSG_START(DM_ID_CONN), /*!< Open Connection */
  DM_CONN_MSG_API_CLOSE,                           /*!< Close Connection */
  DM_CONN_MSG_API_ACCEPT,                          /*!< Accept Connection */
  DM_CONN_MSG_API_UPDATE_MASTER,                   /*!< Master Connection Parameter Update */
  DM_CONN_MSG_API_UPDATE_SLAVE,                    /*!< Slave Connecteion Parameter Update */

  /* messages from L2C */
  DM_CONN_MSG_L2C_UPDATE_IND,                      /*!< L2CAP Parameter update indication */
  DM_CONN_MSG_L2C_UPDATE_CNF,                      /*!< L2CAP Parameter update confirmation */

  /* messages from HCI */
  DM_CONN_MSG_HCI_LE_CONN_CMPL_FAIL,               /*!< HCI LE Connection Complete Failure Event */
  DM_CONN_MSG_HCI_LE_CONN_CMPL,                    /*!< HCI LE Connection Compelte Event */
  DM_CONN_MSG_HCI_DISCONNECT_CMPL,                 /*!< HCI Disconnection Complete Event */
  DM_CONN_MSG_HCI_LE_CONN_UPDATE_CMPL,             /*!< HCI LE Connection Update Complete Event */
  DM_CONN_MSG_HCI_LE_CREATE_CONN_CANCEL_CMD_CMPL,  /*!< HCI LE Create Connection Cancel Command Complet Event */

  /* other internal messages */
  DM_CONN_MSG_INT_UPDATE_TIMEOUT                   /*!< Internal Update Timeout */
};

/* Number of messages */
#define DM_CONN_NUM_MSGS    (DM_CONN_MSG_INT_UPDATE_TIMEOUT - DM_CONN_MSG_API_OPEN + 1)

/* DM conn event handler messages, non-state machine */
enum
{
  DM_CONN_MSG_API_FEAT  = DM_MSG_START(DM_ID_CONN_2),
  DM_CONN_MSG_API_READ_RSSI,
  DM_CONN_MSG_API_REM_CONN_PARAM_REQ_REPLY,
  DM_CONN_MSG_API_REM_CONN_PARAM_REQ_NEG_REPLY,
  DM_CONN_MSG_API_SET_DATA_LEN,
  DM_CONN_MSG_API_WRITE_AUTH_TO,

  /* messages from HCI */
  DM_CONN_MSG_HCI_READ_RSSI_CMPL,
  DM_CONN_MSG_HCI_FEAT_CMPL
};

/* State machine action function sets */
enum
{
  DM_CONN_ACT_SET_MAIN,
  DM_CONN_ACT_SET_MASTER,
  DM_CONN_ACT_SET_SLAVE,
  DM_CONN_NUM_ACT_SETS
};

/*! State machine actions */
enum
{
  DM_CONN_SM_ACT_NONE = DM_CONN_ACT_SET_INIT(DM_CONN_ACT_SET_MAIN),    /*!< No Action */
  DM_CONN_SM_ACT_CLOSE,                                                /*!< Process Connection Close */
  DM_CONN_SM_ACT_CONN_OPENED,                                          /*!< Procoess Connection Opened */
  DM_CONN_SM_ACT_CONN_FAILED,                                          /*!< Process Connection Failed */
  DM_CONN_SM_ACT_CONN_CLOSED,                                          /*!< Process Connection Closed */
  DM_CONN_SM_ACT_HCI_UPDATED,                                          /*!< Process HCI Connection Update */

  DM_CONN_SM_ACT_OPEN = DM_CONN_ACT_SET_INIT(DM_CONN_ACT_SET_MASTER),  /*!< Process Master Connection Open */
  DM_CONN_SM_ACT_CANCEL_OPEN,                                          /*!< Process Master Cancel Connection Open */
  DM_CONN_SM_ACT_UPDATE_MASTER,                                        /*!< Process Master Connection Parameter Update */
  DM_CONN_SM_ACT_L2C_UPDATE_IND,                                       /*!< Process Master L2CAP Connection Parameter Update Indication */

  DM_CONN_SM_ACT_ACCEPT = DM_CONN_ACT_SET_INIT(DM_CONN_ACT_SET_SLAVE), /*!< Process Slave Connection Accept */
  DM_CONN_SM_ACT_CANCEL_ACCEPT,                                        /*!< Process Slave Cancel Connection Accept */
  DM_CONN_SM_ACT_UPDATE_SLAVE,                                         /*!< Process Slave Connection Update */
  DM_CONN_SM_ACT_CONN_ACCEPTED,                                        /*!< Process Slave Connection Accepted */
  DM_CONN_SM_ACT_ACCEPT_FAILED,                                        /*!< Process Slave Connection Accept Failure */
  DM_CONN_SM_ACT_L2C_UPDATE_CNF                                        /*!< Process Slave L2CAP Connection Parameter Update Confirmation */
};

/*! State machine states */
enum
{
  DM_CONN_SM_ST_IDLE,          /*!< Idle State */
  DM_CONN_SM_ST_CONNECTING,    /*!< Connecting State */
  DM_CONN_SM_ST_ACCEPTING,     /*!< Accepting State */
  DM_CONN_SM_ST_CONNECTED,     /*!< Connected State */
  DM_CONN_SM_ST_DISCONNECTING, /*!< Disconnecting State */
  DM_CONN_SM_NUM_STATES
};

/* Data structure for DM_CONN_MSG_API_OPEN and DM_CONN_MSG_API_ACCEPT */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 initPhys;
  uint8_t                 advHandle;
  uint8_t                 advType;
  uint16_t                duration;
  uint8_t                 maxEaEvents;
  bdAddr_t                peerAddr;
  uint8_t                 addrType;
  uint8_t                 clientId;
} dmConnApiOpen_t;

/*! Data structure for DM_CONN_MSG_API_CLOSE */
typedef struct
{
  wsfMsgHdr_t              hdr;
  uint8_t                  reason;
  uint8_t                  clientId;
} dmConnApiClose_t;

/*! Data structure for DM_CONN_MSG_API_UPDATE_MASTER and SLAVE */
typedef struct
{
  wsfMsgHdr_t             hdr;
  hciConnSpec_t           connSpec;
} dmConnApiUpdate_t;

/*! Data structure for DM_CONN_MSG_L2C_UPDATE_IND */
typedef struct
{
  wsfMsgHdr_t             hdr;
  hciConnSpec_t           *pConnSpec;
  uint8_t                 identifier;
} dmConnL2cUpdateInd_t;

/*! Data structure for DM_CONN_MSG_L2C_UPDATE_CNF */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint16_t                result;
} dmConnL2cUpdateCnf_t;

/*! Union of all DM Conn state machine messages */
typedef union
{
  wsfMsgHdr_t               hdr;
  dmConnApiOpen_t           apiOpen;
  dmConnApiClose_t          apiClose;
  dmConnApiUpdate_t         apiUpdate;
  dmConnL2cUpdateInd_t      l2cUpdateInd;
  dmConnL2cUpdateCnf_t      l2cUpdateCnf;
  hciLeConnCmplEvt_t        hciLeConnCmpl;
  hciDisconnectCmplEvt_t    hciDisconnectCmpl;
  hciLeConnUpdateCmplEvt_t  hciLeConnUpdateCmpl;
} dmConnMsg_t;

/*! Data structure for DM_CONN_MSG_API_FEAT */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint16_t                handle;
  uint8_t                 type;
  uint8_t                 feature;
} dmConnApiFeat_t;

/*! Data structure for DM_CONN_MSG_API_READ_RSSI */
typedef struct
{
  wsfMsgHdr_t             hdr;
} dmConnApiReadRssi_t;

/*! Data structure for DM_CONN_MSG_API_REM_CONN_PARAM_REQ_REPLY */
typedef struct
{
  wsfMsgHdr_t             hdr;
  hciConnSpec_t           connSpec;
} dmConnApiRemConnParamReqReply_t;

/*! Data structure for DM_CONN_MSG_API_REM_CONN_PARAM_REQ_NEG_REPLY */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 reason;
} dmConnApiRemConnParamReqNegReply_t;

/*! Data structure for DM_CONN_MSG_API_SET_DATA_LEN */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint16_t                txOctets;
  uint16_t                txTime;
} dmConnApiSetDataLen_t;

/*! Data structure for DM_CONN_MSG_API_WRITE_AUTH_TO */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint16_t                timeout;
} dmConnApiWriteAuthPayloadTo_t;

/*! Union of all DM Conn 2 messages */
typedef union
{
  wsfMsgHdr_t                         hdr;
  dmConnApiFeat_t                     apiFeat;
  dmConnApiReadRssi_t                 apiReadRssi;
  dmConnApiRemConnParamReqReply_t     apiRemConnParamReqReply;
  dmConnApiRemConnParamReqNegReply_t  apiRemConnParamReqNegReply;
  dmConnApiSetDataLen_t               apiSetDataLen;
  dmConnApiWriteAuthPayloadTo_t       apiWriteAuthPayloadTo;
} dmConn2Msg_t;

/*! Connection control block */
typedef struct
{
  bdAddr_t        peerAddr;
  bdAddr_t        localAddr;
  uint16_t        handle;
  uint16_t        idleMask;
  dmConnId_t      connId;
  bool_t          updating;
  bool_t          usingLtk;
  uint8_t         peerAddrType;
  uint8_t         localAddrType;
  uint8_t         state;
  uint8_t         inUse;
  uint8_t         secLevel;
  uint8_t         tmpSecLevel;
  uint8_t         role;

  /* enhanced fields */
  bdAddr_t        localRpa;
  bdAddr_t        peerRpa;

  uint32_t        features;
  bool_t          featuresPresent;
} dmConnCcb_t;

/*! Action function */
typedef void (*dmConnAct_t)(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/*! Control block of the DM conn module */
typedef struct
{
  dmConnCcb_t     ccb[DM_CONN_MAX];
  dmCback_t       connCback[DM_CLIENT_ID_MAX];
  hciConnSpec_t   connSpec[DM_NUM_PHYS];
  uint16_t        scanInterval[DM_NUM_PHYS];
  uint16_t        scanWindow[DM_NUM_PHYS];
} dmConnCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! State machine action sets */
extern dmConnAct_t *dmConnActSet[DM_CONN_NUM_ACT_SETS];

/*! Control block */
extern dmConnCb_t dmConnCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* utility functions */
dmConnCcb_t *dmConnCcbAlloc(uint8_t *pAddr);
void dmConnCcbDealloc(dmConnCcb_t *pCcb);
dmConnCcb_t *dmConnCcbByHandle(uint16_t handle);
dmConnCcb_t *dmConnCcbByBdAddr(uint8_t *pAddr);
dmConnCcb_t *dmConnCcbById(dmConnId_t connId);
uint8_t dmConnNum(void);
dmConnId_t dmConnOpenAccept(uint8_t clientId, uint8_t initPhys, uint8_t advHandle, uint8_t advType,
                            uint16_t duration, uint8_t maxEaEvents, uint8_t addrType, uint8_t *pAddr,
                            uint8_t role);
void dmConnExecCback(dmConnMsg_t *pMsg);

/* component inteface */
void dmConnReset(void);
void dmConnMsgHandler(wsfMsgHdr_t *pMsg);
void dmConnHciHandler(hciEvt_t *pEvent);

/* component 2 inteface */
void dmConn2MsgHandler(wsfMsgHdr_t *pMsg);
void dmConn2HciHandler(hciEvt_t *pEvent);

/* state machine */
void dmConnSmExecute(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/* main action functions */
void dmConnSmActNone(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActClose(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActConnOpened(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActConnFailed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActConnClosed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActHciUpdated(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/* common master action functions */
void dmConnSmActCancelOpen(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActUpdateMaster(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActL2cUpdateInd(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/* legacy master action functions */
void dmConnSmActOpen(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/* extended master action functions */
void dmExtConnSmActOpen(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/* common slave action functions */
void dmConnSmActUpdateSlave(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActL2cUpdateCnf(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/* legacy slave action functions */
void dmConnSmActAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActCancelAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActConnAccepted(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmConnSmActAcceptFailed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

/* extended slave action functions */
void dmExtConnSmActAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmExtConnSmActCancelAccept(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmExtConnSmActConnAccepted(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);
void dmExtConnSmActAcceptFailed(dmConnCcb_t *pCcb, dmConnMsg_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* DM_CONN_H */
