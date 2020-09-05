/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM privacy module.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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
#ifndef DM_PRIV_H
#define DM_PRIV_H

#include "util/bda.h"
#include "wsf_os.h"
#include "wsf_timer.h"
#include "sec_api.h"
#include "dm_main.h"
#include "smp_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* DM privacy event handler messages */
enum
{
  DM_PRIV_MSG_API_RESOLVE_ADDR = DM_MSG_START(DM_ID_PRIV),
  DM_PRIV_MSG_API_ADD_DEV_TO_RES_LIST,
  DM_PRIV_MSG_API_REM_DEV_FROM_RES_LIST,
  DM_PRIV_MSG_API_CLEAR_RES_LIST,
  DM_PRIV_MSG_API_SET_ADDR_RES_ENABLE,
  DM_PRIV_MSG_API_SET_PRIVACY_MODE,
  DM_PRIV_MSG_API_GEN_ADDR
};

/* DM privacy AES event handler messages */
enum
{
  DM_PRIV_MSG_RESOLVE_AES_CMPL = DM_MSG_START(DM_ID_PRIV_AES),
  DM_PRIV_MSG_GEN_ADDR_AES_CMPL
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Data structure for DM_PRIV_MSG_API_RESOLVE_ADDR */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 irk[SMP_KEY_LEN];
  bdAddr_t                addr;
} dmPrivApiResolveAddr_t;

/* Data structure for DM_PRIV_MSG_API_GEN_ADDR */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 irk[SMP_KEY_LEN];
} dmPrivApiGenAddr_t;

/* Data structure for DM_PRIV_MSG_API_ADD_DEV_TO_RES_LIST */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 addrType;
  bdAddr_t                peerAddr;
  uint8_t                 peerIrk[SMP_KEY_LEN];
  uint8_t                 localIrk[SMP_KEY_LEN];
  bool_t                  enableLlPriv;
} dmPrivApiAddDevToResList_t;

/* Data structure for DM_PRIV_MSG_API_REM_DEV_FROM_RES_LIST */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 addrType;
  bdAddr_t                peerAddr;
} dmPrivApiRemDevFromResList_t;

/* Data structure for DM_PRIV_MSG_API_SET_ADDR_RES_ENABLE */
typedef struct
{
  wsfMsgHdr_t             hdr;
  bool_t                  enable;
} dmPrivApiSetAddrResEnable_t;

/* Data structure for DM_PRIV_MSG_API_SET_PRIVACY_MODE */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 addrType;
  bdAddr_t                peerAddr;
  uint8_t                 mode;
} dmPrivApiSetPrivacyMode_t;

/* Union of all privacy messages */
typedef union
{
  wsfMsgHdr_t                  hdr;
  dmPrivApiResolveAddr_t       apiResolveAddr;
  dmPrivApiAddDevToResList_t   apiAddDevToResList;
  dmPrivApiRemDevFromResList_t apiRemDevFromResList;
  dmPrivApiSetAddrResEnable_t  apiSetAddrResEnable;
  dmPrivApiSetPrivacyMode_t    apiSetPrivacyMode;
  dmPrivApiGenAddr_t           apiGenerateAddr;
  dmPrivGenAddrIndEvt_t        genAddrInd;
  secAes_t                     aes;
} dmPrivMsg_t;

/* Action function */
typedef void (*dmPrivAct_t)(dmPrivMsg_t *pMsg);

/* Control block for privacy module */
typedef struct
{
  uint8_t     hash[DM_PRIV_HASH_LEN];            /* Hash part of resolvable address */
  bool_t      inProgress;                        /* Address resolution in progress */
  uint16_t    addDevToResListParam;              /* 'Add device to resolving list' callback param */
  uint16_t    remDevFromResListParam;            /* 'Remove device from resolving list' callback param */
  bool_t      enableLlPriv;                      /* 'Add device to resolving list' input param */
  bool_t      addrResEnable;                     /* 'Set address resolution enable' input param */
  uint8_t     genAddrBuf[HCI_ENCRYPT_DATA_LEN];  /* Random value buffer for generating an RPA */
} dmPrivCb_t;

/**************************************************************************************************
  Function declarations
**************************************************************************************************/

/* Privacy component inteface */
void dmPrivMsgHandler(wsfMsgHdr_t *pMsg);
void dmPrivHciHandler(hciEvt_t *pEvent);
void dmPrivReset(void);

/* Privacy action functions */
void dmPrivActResolveAddr(dmPrivMsg_t *pMsg);
void dmPrivActAddDevToResList(dmPrivMsg_t *pMsg);
void dmPrivActRemDevFromResList(dmPrivMsg_t *pMsg);
void dmPrivActSetAddrResEnable(dmPrivMsg_t *pMsg);
void dmPrivActClearResList(dmPrivMsg_t *pMsg);
void dmPrivActSetPrivacyMode(dmPrivMsg_t *pMsg);
void dmPrivActGenAddr(dmPrivMsg_t *pMsg);

/* Privacy ASE component inteface */
void dmPrivAesMsgHandler(wsfMsgHdr_t *pMsg);

/* Privacy ASE action functions */
void dmPrivAesActResAddrAesCmpl(dmPrivMsg_t *pMsg);
void dmPrivAesActGenAddrAesCmpl(dmPrivMsg_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* DM_PRIV_H */
