/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Platform interface file: ble-specific.
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

#ifndef PLATFORM_BLE_API_H
#define PLATFORM_BLE_API_H

#include "wsf_types.h"

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief  LL configuration. */
typedef struct
{
  /* Advertiser */
  uint8_t  maxAdvSets;              /*!< Maximum number of advertising sets. */
  uint8_t  maxAdvReports;           /*!< Maximum number of pending legacy or extended advertising reports. */
  uint16_t maxExtAdvDataLen;        /*!< Maximum extended advertising data size. */
  uint8_t  defExtAdvDataFragLen;    /*!< Default extended advertising data fragmentation size. */
  uint32_t auxDelayUsec;            /*!< Additional Auxiliary Offset delay above T_MAFS in microseconds. */
  /* Scanner */
  uint8_t  maxScanReqRcvdEvt;       /*!< Maximum scan request received events. */
  uint16_t maxExtScanDataLen;       /*!< Maximum extended scan data size. */
  /* Connection */
  uint8_t  maxConn;                 /*!< Maximum number of connections. */
  uint8_t  numTxBufs;               /*!< Default number of transmit buffers. */
  uint8_t  numRxBufs;               /*!< Default number of receive buffers. */
  uint16_t maxAclLen;               /*!< Maximum ACL buffer size. */
  int8_t   defTxPwrLvl;             /*!< Default Tx power level for connections. */
  uint8_t  ceJitterUsec;            /*!< Allowable CE jitter on a slave (account for master's sleep clock resolution). */
  /* DTM */
  uint16_t dtmRxSyncMs;             /*!< DTM Rx synchronization window in milliseconds. */
} PlatformLlCfg_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Configuration */
/*************************************************************************************************/
/*!
 *  \brief      Get PHY feature configuration.
 *
 *  \param      pPhy2mSup             2M PHY supported.
 *  \param      pPhyCodedSup          Coded PHY supported.
 *  \param      pStableModIdxTxSup    Tx stable modulation index supported.
 *  \param      pStableModIdxRxSup    Rx stable modulation index supported.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PlatformGetBlePhyFeatures(bool_t *pPhy2mSup, bool_t *pPhyCodedSup,
                               bool_t *pStableModIdxTxSup, bool_t *pStableModIdxRxSup);

/*************************************************************************************************/
/*!
 *  \brief      Load LL advertising configuration.
 *
 *  \param      pCfg                Return configuration values.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PlatformLoadLlConfig(PlatformLlCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief      Load device Bluetooth device address.
 *
 *  \param      pDevAddr    Bluetooth device address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PlatformLoadBdAddress(uint8_t *pDevAddr);

#endif /* PLATFORM_BLE_API_H */
