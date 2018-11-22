/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager advertising master interface file.
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

#ifndef LMGR_API_ADV_MASTER_H
#define LMGR_API_ADV_MASTER_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_ADV_MST
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Scan parameters. */
typedef struct
{
  uint16_t scanInterval;                /*!< Scan interval in BB ticks. */
  uint16_t scanWindow;                  /*!< Scan window duration in BB ticks. */
  uint8_t scanType;                     /*!< Advertising type. */
  uint8_t ownAddrType;                  /*!< Address type used by this device. */
  uint8_t scanFiltPolicy;               /*!< Scanning filter policy. */
} lmgrScanParam_t;

/*! \brief      Master role device parameter definition. */
typedef struct
{
  lmgrScanParam_t   scanParam;          /*!< Scan parameters. */
  uint64_t          chanClass;          /*!< Channel class. */
  uint8_t           scanChanMap;        /*!< Scan channel map. */
  uint8_t           numAdvReport;       /*!< Number of pending advertising reports. */
} lmgrMstScanCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrMstScanCtrlBlk_t lmgrMstScanCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrMstInit(void);

/*! \} */    /* LL_LMGR_API_ADV_MST */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_MASTER_H */
