/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM advertising module.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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
#ifndef DM_ADV_H
#define DM_ADV_H

#include "wsf_timer.h"
#include "sec_api.h"
#include "dm_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* DM adv event handler messages */
enum
{
  DM_ADV_MSG_API_CONFIG = DM_MSG_START(DM_ID_ADV),
  DM_ADV_MSG_API_SET_DATA,
  DM_ADV_MSG_API_START,
  DM_ADV_MSG_API_STOP,
  DM_ADV_MSG_API_REMOVE,
  DM_ADV_MSG_API_CLEAR,
  DM_ADV_MSG_API_SET_RAND_ADDR,
  DM_ADV_MSG_TIMEOUT
};

/* DM adv periodic event handler messages */
enum
{
  DM_ADV_PER_MSG_API_CONFIG = DM_MSG_START(DM_ID_ADV_PER),
  DM_ADV_PER_MSG_API_SET_DATA,
  DM_ADV_PER_MSG_API_START,
  DM_ADV_PER_MSG_API_STOP,
};

/* DM advertising states */
enum
{
  DM_ADV_STATE_IDLE,              /* idle */
  DM_ADV_STATE_ADVERTISING,       /* advertising */
  DM_ADV_STATE_STARTING_DIRECTED, /* starting high duty cycle directed advertising */
  DM_ADV_STATE_STARTING,          /* starting undirected or low duty cycle directed advertising */
  DM_ADV_STATE_STOPPING_DIRECTED, /* stopping high duty cycle directed advertising */
  DM_ADV_STATE_STOPPING,          /* stopping undirected or low duty cycle directed advertising */
  DM_ADV_STATE_REMOVING_SET,      /* removing advertising set */
  DM_ADV_STATE_CLEARING_SETS      /* clearing all advertising sets */
};

/* DM periodic advertising states */
enum
{
  DM_ADV_PER_STATE_IDLE,
  DM_ADV_PER_STATE_ADVERTISING,
  DM_ADV_PER_STATE_STARTING,
  DM_ADV_PER_STATE_STOPPING
};

/* Uninitialized HCI handle */
#define DM_ADV_HCI_HANDLE_NONE    0xFF

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Data structure for DM_ADV_MSG_API_CONFIG */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
  uint8_t                 advType;
  uint8_t                 peerAddrType;
  bdAddr_t                peerAddr;
  bool_t                  scanReqNotifEna;
} dmAdvApiConfig_t;

/* Data structure for DM_ADV_MSG_API_SET_DATA */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
  uint8_t                 op;
  uint8_t                 location;
  uint8_t                 len;
  uint8_t                 pData[];
} dmAdvApiSetData_t;

/* Data structure for DM_ADV_MSG_API_START */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 numSets;
  uint8_t                 advHandle[DM_NUM_ADV_SETS];
  uint16_t                duration[DM_NUM_ADV_SETS];
  uint8_t                 maxEaEvents[DM_NUM_ADV_SETS];
} dmAdvApiStart_t;

/* Data structure for DM_ADV_MSG_API_STOP */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 numSets;
  uint8_t                 advHandle[DM_NUM_ADV_SETS];
} dmAdvApiStop_t;

/* Data structure for DM_ADV_MSG_API_REMOVE */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
} dmAdvApiRemove_t;

/* Data structure for DM_ADV_MSG_API_SET_RAND_ADDR */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
  bdAddr_t                addr;
} dmAdvApiSetRandAddr_t;

/* Data structure for DM_ADV_PER_MSG_API_CONFIG */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
} dmAdvPerApiConfig_t;

/* Data structure for DM_ADV_PER_MSG_API_SET_DATA */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
  uint8_t                 op;
  uint8_t                 len;
  uint8_t                 pData[];
} dmAdvPerApiSetData_t;

/* Data structure for DM_ADV_PER_MSG_API_START */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
} dmAdvPerApiStart_t;

/* Data structure for DM_ADV_PER_MSG_API_STOP */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 advHandle;
} dmAdvPerApiStop_t;

/* Union of all adv messages */
typedef union
{
  wsfMsgHdr_t             hdr;
  dmAdvApiConfig_t        apiConfig;
  dmAdvApiSetData_t       apiSetData;
  dmAdvApiStart_t         apiStart;
  dmAdvApiStop_t          apiStop;
  dmAdvApiRemove_t        apiRemove;
  dmAdvApiSetRandAddr_t   apiSetRandAddr;
  dmAdvPerApiConfig_t     apiPerConfig;
  dmAdvPerApiSetData_t    apiPerSetData;
  dmAdvPerApiStart_t      apiPerStart;
  dmAdvPerApiStop_t       apiPerStop;
  secAes_t                aes;
} dmAdvMsg_t;

/* Action function */
typedef void (*dmAdvAct_t)(dmAdvMsg_t *pMsg);

/* Control block for advertising module */
typedef struct
{
  wsfTimer_t      advTimer;
  uint16_t        intervalMin[DM_NUM_ADV_SETS];
  uint16_t        intervalMax[DM_NUM_ADV_SETS];
  uint8_t         advType[DM_NUM_ADV_SETS];
  uint8_t         channelMap[DM_NUM_ADV_SETS];
  uint8_t         localAddrType;
  uint8_t         advState[DM_NUM_ADV_SETS];
  uint16_t        advDuration[DM_NUM_ADV_SETS];
  bool_t          advEnabled;
  bdAddr_t        peerAddr[DM_NUM_ADV_SETS];
  uint8_t         peerAddrType[DM_NUM_ADV_SETS];
} dmAdvCb_t;

extern dmAdvCb_t dmAdvCb;

/**************************************************************************************************
  Function declarations
**************************************************************************************************/

/* legacy adv component inteface */
void dmAdvMsgHandler(wsfMsgHdr_t *pMsg);
void dmAdvHciHandler(hciEvt_t *pEvent);
void dmAdvReset(void);

/* legacy adv action functions */
void dmAdvActConfig(dmAdvMsg_t *pMsg);
void dmAdvActSetData(dmAdvMsg_t *pMsg);
void dmAdvActStart(dmAdvMsg_t *pMsg);
void dmAdvActStop(dmAdvMsg_t *pMsg);
void dmAdvActRemoveSet(dmAdvMsg_t *pMsg);
void dmAdvActClearSets(dmAdvMsg_t *pMsg);
void dmAdvActSetRandAddr(dmAdvMsg_t *pMsg);
void dmAdvActTimeout(dmAdvMsg_t *pMsg);

/* extended adv component inteface */
void dmExtAdvMsgHandler(wsfMsgHdr_t *pMsg);
void dmExtAdvHciHandler(hciEvt_t *pEvent);
void dmExtAdvReset(void);

/* extended adv action functions */
void dmExtAdvActConfig(dmAdvMsg_t *pMsg);
void dmExtAdvActSetData(dmAdvMsg_t *pMsg);
void dmExtAdvActStart(dmAdvMsg_t *pMsg);
void dmExtAdvActStop(dmAdvMsg_t *pMsg);
void dmExtAdvActRemoveSet(dmAdvMsg_t *pMsg);
void dmExtAdvActClearSets(dmAdvMsg_t *pMsg);
void dmExtAdvActSetRandAddr(dmAdvMsg_t *pMsg);
void dmExtAdvActTimeout(dmAdvMsg_t *pMsg);

/* periodic adv action functions */
void dmPerAdvActConfig(dmAdvMsg_t *pMsg);
void dmPerAdvActSetData(dmAdvMsg_t *pMsg);
void dmPerAdvActStart(dmAdvMsg_t *pMsg);
void dmPerAdvActStop(dmAdvMsg_t *pMsg);

/* periodic adv component inteface */
void dmPerAdvMsgHandler(wsfMsgHdr_t *pMsg);
void dmPerAdvHciHandler(hciEvt_t *pEvent);
void dmPerAdvReset(void);

/* legacy adv directed advertising interface */
void dmAdvStartDirected(uint8_t advType, uint16_t duration, uint8_t addrType, uint8_t *pAddr);
void dmAdvStopDirected(void);
void dmAdvConnected(void);
void dmAdvConnectFailed(void);

/* extended adv directed advertising interface */
void dmExtAdvStartDirected(dmConnId_t connId, uint8_t advHandle, uint8_t advType,
                           uint16_t duration, uint8_t maxEaEvents, uint8_t addrType, uint8_t *pAddr);
void dmExtAdvStopDirected(dmConnId_t connId);
void dmExtAdvConnected(dmConnId_t connId);
void dmExtAdvConnectFailed(dmConnId_t connId);

/* adv utility functions */
void dmAdvInit(void);
void dmAdvCbInit(uint8_t advHandle);
void dmAdvGenConnCmpl(uint8_t advHandle, uint8_t status);

/* extended and periodic adv utility functions */
void dmExtAdvInit(void);
void dmPerAdvInit(void);
uint8_t dmPerAdvState(uint8_t advHandle);

#ifdef __cplusplus
};
#endif

#endif /* DM_ADV_H */
