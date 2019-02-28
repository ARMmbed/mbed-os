/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager advertising master interface file.
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
