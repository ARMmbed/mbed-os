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
 * \brief Link layer manager advertising slave interface file.
 */
/*************************************************************************************************/

#ifndef LMGR_API_ADV_SLAVE_H
#define LMGR_API_ADV_SLAVE_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_ADV_SLV
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising parameters. */
typedef struct
{
  uint32_t          advInterMin;        /*!< Minimum advertising interval. */
  uint32_t          advInterMax;        /*!< Maximum advertising interval. */
  uint8_t           advType;            /*!< Advertising type. */
  uint8_t           ownAddrType;        /*!< Address type used by this device. */
  uint8_t           peerAddrType;       /*!< Address type of peer device. Only used for directed advertising. */
  uint64_t          peerAddr;           /*!< Address of peer device. Only used for directed advertising. */
  uint8_t           advChanMap;         /*!< Advertising channel map. */
  uint8_t           advFiltPolicy;      /*!< Advertising filter policy. */
} lmgrAdvParam_t;

/*! \brief      Slave role device parameter definition. */
typedef struct
{
  uint32_t          advTermCntDown;     /*!< Advertising termination count down. */
  lmgrAdvParam_t    advParam;           /*!< Advertising parameters. */
  lmgrAdvbUser_t    advData;            /*!< Advertising host data buffer. */
  lmgrAdvbUser_t    scanRspData;        /*!< Scan response host data buffer. */
  uint8_t           ownAddrType;        /*!< Actual address type used by this device. */
  uint64_t          localRpa;           /*!< Local RPA used by this device. */
  bool_t            scanReportEna;      /*!< Scan report events enabled. */
  uint8_t           defTxPhyOpts;       /*!< Default Tx PHY options */
} lmgrSlvAdvCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrSlvAdvCtrlBlk_t lmgrSlvAdvCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrSlvInit(void);

/*! \} */    /* LL_LMGR_API_ADV_SLV */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_SLAVE_H */
