/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager common interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef LMGR_API_H
#define LMGR_API_H

#include "ll_api.h"
#include "ll_defs.h"

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
  uint16_t          perScanCtxSize;     /*!< Size of the periodic scanning context. */
  uint64_t          featuresDefault;    /*!< Default supported features. */

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

  /* Power Class 1. */
  int8_t            powerThreshold[LL_MAX_PHYS];    /*!< Power threshold for each PHY. */
  uint8_t           localMinUsedChan[LL_MAX_PHYS];  /*!< Local minimum number of used channels for each PHY. */
} lmgrCtrlBlk_t;

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

/* Event Messages */
void LmgrSendAdvEnableCnf(uint8_t status);
void LmgrSendScanEnableCnf(uint8_t status);
void LmgrSendAdvSetTermInd(uint8_t handle, uint8_t status, uint16_t connHandle, uint8_t numEvents);
bool_t LmgrSendEvent(LlEvt_t *pEvt);

/*! \} */    /* LL_LMGR_API */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_H */
