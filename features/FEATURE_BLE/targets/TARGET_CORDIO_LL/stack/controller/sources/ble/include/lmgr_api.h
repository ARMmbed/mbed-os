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
 * \brief Link layer manager common interface file.
 */
/*************************************************************************************************/

#ifndef LMGR_API_H
#define LMGR_API_H

#include "ll_api.h"
#include "ll_defs.h"
#include "hci_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master scan modes. */
enum
{
  LMGR_SCAN_MODE_DISCOVER,              /*!< Scan enabled state. */
  LMGR_SCAN_MODE_INITIATE               /*!< Initiate enabled state. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising broadcast user data. */
typedef struct
{
  uint8_t           buf[LL_ADVBU_MAX_LEN];  /*!< ADVB-U buffer. */
  uint8_t           len;                    /*!< ADVB-U buffer length. */
  bool_t            modified;               /*!< Content modified by host. */
} lmgrAdvbUser_t;

/*! \brief      Link layer manager persistent control block (persists with resets). */
typedef struct
{
  llEvtCback_t      evtCback;           /*!< Event callback. */
  llAclCback_t      sendCompCback;      /*!< ACL data send complete callback. */
  llAclCback_t      recvPendCback;      /*!< ACL data receive pending callback. */
  wsfHandlerId_t    handlerId;          /*!< System event handler ID. */
  uint16_t          connCtxSize;        /*!< Size of the connection context. */
  uint16_t          advSetCtxSize;      /*!< Size of the advertising set context. */
  uint16_t          extScanCtxSize;     /*!< Size of the extended scanner context. */
  uint16_t          extInitCtxSize;     /*!< Size of the extended initiator context. */
  uint16_t          perScanCtxSize;     /*!< Size of the periodic scanning context. */
  uint16_t          cisCtxSize;         /*!< Size of the CIS context. */
  uint16_t          cigCtxSize;         /*!< Size of the CIG context. */
  uint64_t          featuresDefault;    /*!< Default supported features. */

  llIsoCback_t      sendIsoCompCback;   /*!< ISO data send complete callback. */
  llIsoCback_t      recvIsoPendCback;   /*!< ISO data receive pending callback. */

  /* Device parameters */
  uint64_t          bdAddr;             /*!< Public device address. */
  uint64_t          supStates;          /*!< Supported states. */
} lmgrPersistCtrlBlk_t;

/*! \brief      Link layer manager control block (cleared with resets). */
typedef struct
{
  /* Status */
  uint8_t           numConnEnabled;     /*!< Number of active connection. */
  bool_t            advEnabled;         /*!< Legacy advertising is enabled. */
  uint8_t           numExtAdvEnabled;   /*!< Number of extended advertising is enabled. */
  uint8_t           numScanEnabled;     /*!< Number of scan is enabled. */
  uint8_t           numInitEnabled;     /*!< Number of initiate is enabled. */
  uint8_t           scanMode;           /*!< Discover or initiating scan mode. */
  uint8_t           numWlFilterEnabled; /*!< Number of enabled whitelist filters. */
  uint8_t           numPlFilterEnabled; /*!< Number of enabled periodic filters. */
  bool_t            testEnabled;        /*!< Test is enabled. */
  uint8_t           resetDelayRefCnt;   /*!< Reset delay reference counter. */
  uint8_t           extAdvEnaDelayCnt;  /*!< Extended advertising enable event delay reference count. */
  uint8_t           advSetEnaStatus;    /*!< Last advertising set enable error status. */
  uint8_t           numExtScanPhys;     /*!< Number of extended scanning PHYs enabled. */
  uint8_t           extScanEnaDelayCnt; /*!< Extended scanning enable event delay reference count. */
  uint8_t           extScanEnaStatus;   /*!< Last extended scanning enable error status. */

  /* Device parameters */
  bool_t            bdAddrRndModAdv;    /*!< Modified random address (advertising). */
  bool_t            bdAddrRndModScan;   /*!< Modified random address (scan). */
  bool_t            bdAddrRndValid;     /*!< Random device address valid. */
  uint32_t          opModeFlags;        /*!< Operational mode flags. */
  uint64_t          bdAddrRnd;          /*!< Random device address. */
  uint64_t          features;           /*!< Used local supported features. */

  /* Operational parameters. */
  int8_t            advTxPwr;           /*!< Advertising Tx power. */
  bool_t            addrResEna;         /*!< Address resolution enabled. */
  bool_t            useLegacyCmds;      /*!< Use only legacy advertising, scan or initiate commands. */
  bool_t            useExtCmds;         /*!< Use only extended advertising, scan or initiate commands. */
  uint64_t          chanClass;          /*!< Channel class. */

  /* Sleep clock accuracy override value. */
  int8_t            scaMod;             /*!< SCA override value. */

  /* Power Class 1. */
  int8_t            powerThreshold[LL_MAX_PHYS];    /*!< Power threshold for each PHY. */
  uint8_t           localMinUsedChan[LL_MAX_PHYS];  /*!< Local minimum number of used channels for each PHY. */

  uint8_t           hciSupCommands[HCI_SUP_CMD_LEN]; /*!< Supported HCI commands bit mask. */
} lmgrCtrlBlk_t;

/*! \brief      Channel parameters. */
typedef struct
{
  /* Channel parameters */
  uint8_t       lastChanIdx;        /*!< Current channel index. */
  uint8_t       numUsedChan;        /*!< Number of used channels. */
  uint64_t      chanMask;           /*!< Channel mask. */
  uint8_t       chanRemapTbl[LL_CHAN_DATA_MAX_IDX + 1]; /*!< Channel remapping table. */

  uint8_t       usedChSel;          /*!< Used channel selection. */
  uint16_t      chIdentifier;       /*!< Channel identifier. */

  /* For subevent calculation only */
  uint16_t      prnLast;            /*!< Last used permutation. */
  uint8_t       subEvtIdx;          /*!< Subevent index. */
} lmgrChanParam_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrPersistCtrlBlk_t lmgrPersistCb;
extern lmgrCtrlBlk_t lmgrCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Reset */
void LmgrSetDefaults(void);
void LmgrIncResetRefCount(void);
void LmgrDecResetRefCount(void);

/* Whitelist */
void LmgrIncWhitelistRefCount(void);
void LmgrDecWhitelistRefCount(void);

/* Periodic List */
void LmgrIncPeriodiclistRefCount(void);
void LmgrDecPeriodiclistRefCount(void);

/* Status */
bool_t LmgrIsAddressTypeAvailable(uint8_t ownAddrType);
bool_t LmgrIsLegacyCommandAllowed(void);
bool_t LmgrIsExtCommandAllowed(void);

/* Utility */
void LmgrBuildRemapTable(lmgrChanParam_t *pChanParam);
uint8_t LmgrSelectNextChannel(lmgrChanParam_t *pChanParam, uint16_t eventCounter, uint16_t numSkip, bool_t calSubEvt);
uint32_t LmgrCalcWindowWideningUsec(uint32_t unsyncTimeUsec, uint32_t caPpm);
uint8_t LmgrSelectNextSubEvtChannel(lmgrChanParam_t *pChanParam);
uint8_t * LmgrReadHciSupCmd(void);

/* Event Messages */
void LmgrSendAdvEnableCnf(uint8_t status);
void LmgrSendScanEnableCnf(uint8_t status);
void LmgrSendAdvSetTermInd(uint8_t handle, uint8_t status, uint16_t connHandle, uint8_t numEvents);
bool_t LmgrSendEvent(LlEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief      Get operational mode flag.
 *
 *  \param      flag  Flag to check.
 *
 *  \return     TRUE if flag is set.
 *
 *  Get mode flag governing LL operations.
 */
/*************************************************************************************************/
static inline bool_t lmgrGetOpFlag(uint32_t flag)
{
  return (lmgrCb.opModeFlags & flag) ? TRUE : FALSE;
}

/*! \} */    /* LL_LMGR_API */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_H */
