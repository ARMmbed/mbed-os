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
 *  \brief DM main module.
 */
/*************************************************************************************************/
#ifndef DM_MAIN_H
#define DM_MAIN_H

#include "util/bda.h"
#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* DM component IDs */
#define DM_ID_ADV                     0
#define DM_ID_DEV_PRIV                1
#define DM_ID_SCAN                    2
#define DM_ID_CONN                    3
#define DM_ID_CONN_2                  4
#define DM_ID_SEC                     5
#define DM_ID_PRIV                    6
#define DM_ID_DEV                     7
#define DM_ID_LESC                    8
#define DM_ID_PHY                     9
#define DM_ID_ADV_PER                 10
#define DM_ID_SYNC                    11
#define DM_ID_PAST                    12
#define DM_ID_CONN_CTE                13
#define DM_NUM_IDS                    14

/* Start of component message enumeration */
#define DM_MSG_START(id)              ((id) << 4)

/* Get the component ID from a message ID */
#define DM_ID_FROM_MSG(msg)           ((msg) >> 4)

/* Mask off the ID from the message ID */
#define DM_MSG_MASK(msg)              ((msg) & 0x0F)

/* Length of hash part of private resolvable address */
#define DM_PRIV_HASH_LEN              3

/* Length of random part of private resolvable address */
#define DM_PRIV_PRAND_LEN             3

/* Length of plaintext used for private resolvable address calculation */
#define DM_PRIV_PLAINTEXT_LEN         16

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* DM reset function type */
typedef void (*dmReset_t)(void);

/* DM message handling function types */
typedef void (*dmMsgHandler_t)(wsfMsgHdr_t *pMsg);
typedef void (*dmHciHandler_t)(hciEvt_t *pEvent);

/* DM component reset and handler function interface */
typedef struct
{
  dmReset_t           reset;
  dmHciHandler_t      hciHandler;
  dmMsgHandler_t      msgHandler;
} dmFcnIf_t;

/* Main control block of the DM subsystem */
typedef struct
{
  bdAddr_t            localAddr;
  dmCback_t           cback;
  wsfHandlerId_t      handlerId;
  uint8_t             connAddrType;
  uint8_t             advAddrType;
  uint8_t             scanAddrType;
  bool_t              resetting;

  /* Filter policies for Advertising, Scanning, Initiator and Synchronization */
  uint8_t             advFiltPolicy[DM_NUM_ADV_SETS];
  uint8_t             scanFiltPolicy;
  uint8_t             initFiltPolicy;

  /* Options (filter policies and periodic advertising report enablement) for Synchronization */
  uint8_t             syncOptions;

  /* LL Privacy */
  bool_t              llPrivEnabled;
} dmCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* Component function interface table indexed DM component ID */
extern dmFcnIf_t *dmFcnIfTbl[DM_NUM_IDS];

/* Control block */
extern dmCb_t dmCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void dmEmptyReset(void);
void dmEmptyHandler(wsfMsgHdr_t *pMsg);

/* utility functions */
uint8_t DmScanPhyToIdx(uint8_t scanPhy);
uint8_t DmInitPhyToIdx(uint8_t initPhy);
void dmDevPassHciEvtToConn(hciEvt_t *pEvent);

#ifdef __cplusplus
};
#endif

#endif /* DM_MAIN_H */
