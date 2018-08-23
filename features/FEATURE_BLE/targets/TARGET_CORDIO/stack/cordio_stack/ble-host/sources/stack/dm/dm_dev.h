/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM local device management module.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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
#ifndef DM_DEV_H
#define DM_DEV_H

#include "dm_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* DM device event handler messages */
enum
{
  DM_DEV_MSG_API_RESET = DM_MSG_START(DM_ID_DEV)
};

/* DM device privacy event handler messages */
enum
{
  DM_DEV_PRIV_MSG_API_START = DM_MSG_START(DM_ID_DEV_PRIV),
  DM_DEV_PRIV_MSG_API_STOP,
  DM_DEV_PRIV_MSG_TIMEOUT,
  DM_DEV_PRIV_MSG_AES_CMPL,
  DM_DEV_PRIV_MSG_RPA_START,
  DM_DEV_PRIV_MSG_RPA_STOP,
  DM_DEV_PRIV_MSG_CTRL,
};

/* DM device privacy control messages */
enum
{
  DM_DEV_PRIV_MSG_CONN_INIT_START, /* connection initiation started */
  DM_DEV_PRIV_MSG_CONN_INIT_STOP,  /* connection initiation stopped */
  DM_DEV_PRIV_MSG_ADV_SET_ADD,     /* advertising set created */
  DM_DEV_PRIV_MSG_ADV_SET_REMOVE,  /* advertising set removed */
  DM_DEV_PRIV_MSG_ADV_SETS_CLEAR   /* advertising sets cleared */
};


/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Data structure for DM_DEV_PRIV_MSG_API_START */
typedef struct
{
  wsfMsgHdr_t                hdr;
  uint16_t                   changeInterval;
} dmDevPrivApiStart_t;

/* Data structure for DM_DEV_PRIV_MSG_CTRL */
typedef struct
{
  wsfMsgHdr_t                hdr;
  uint8_t                    advHandle;
  bool_t                     connectable;
} dmDevPrivCtrl_t;

/* Union of all dev priv messages */
typedef union
{
  wsfMsgHdr_t                hdr;
  dmDevPrivApiStart_t        apiPrivStart;
  dmDevPrivCtrl_t            privCtrl;
  secAes_t                   aes;
} dmDevPrivMsg_t;

/* Action function */
typedef void (*dmDevAct_t)(wsfMsgHdr_t *pMsg);
typedef void (*dmDevPrivAct_t)(dmDevPrivMsg_t *pMsg);

/* DM device set advertising set random address callback type */
typedef void (*dmDevAdvSetRandAddrCback_t)(uint8_t advHandle, const uint8_t *pAddr);

/* Main control block of the DM Dev subsystem */
typedef struct
{
  /* Set advertising set random address callback */
  dmDevAdvSetRandAddrCback_t advSetRandAddrCback;
} dmDevCb_t;

/**************************************************************************************************
  Global variables
**************************************************************************************************/

/* Component function interface */
extern const dmFcnIf_t dmDevFcnIf;

/* Control block */
extern dmDevCb_t dmDevCb;

/**************************************************************************************************
  Function declarations
**************************************************************************************************/

/* dev component inteface */
void dmDevActReset(wsfMsgHdr_t *pMsg);

/* dev action functions */
void dmDevMsgHandler(wsfMsgHdr_t *pMsg);
void dmDevHciHandler(hciEvt_t *pEvent);

/* dev priv component inteface */
void dmDevPrivMsgHandler(wsfMsgHdr_t *pMsg);
void dmDevPrivReset(void);

/* dev priv action functions */
void dmDevPrivActStart(dmDevPrivMsg_t *pMsg);
void dmDevPrivActStop(dmDevPrivMsg_t *pMsg);
void dmDevPrivActTimeout(dmDevPrivMsg_t *pMsg);
void dmDevPrivActAesCmpl(dmDevPrivMsg_t *pMsg);
void dmDevPrivActRpaStart(dmDevPrivMsg_t *pMsg);
void dmDevPrivActRpaStop(dmDevPrivMsg_t *pMsg);
void dmDevPrivActCtrl(dmDevPrivMsg_t *pMsg);

/* utility function */
void dmDevPassEvtToDevPriv(uint8_t event, uint8_t param, uint8_t advHandle, bool_t connectable);

#ifdef __cplusplus
};
#endif

#endif /* DM_DEV_H */
