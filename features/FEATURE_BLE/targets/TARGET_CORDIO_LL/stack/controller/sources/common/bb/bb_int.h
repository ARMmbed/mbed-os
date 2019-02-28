/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal baseband interface file.
 *
 *  Copyright (c) 2016-2017 ARM Ltd. All Rights Reserved.
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

#ifndef BB_INT_H
#define BB_INT_H

#include "bb_api.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      BB control block. */
typedef struct
{
  BbOpDesc_t        *pOpInProgress;     /*!< Current BB operation in progress. */
  BbBodCompCback_t  bodCompCback;       /*!< BOD completion handler. */

  struct
  {
    BbBodCback_t    execOpCback;        /*!< Execute operation handler. */
    BbBodCback_t    cancelOpCback;      /*!< Cancel operation handler. */
    BbProtCback_t   startProtCback;     /*!< Start protocol handler. */
    BbProtCback_t   stopProtCback;      /*!< Stop protocol handler. */
    uint32_t        startCnt;           /*!< Start counter. */
  } prot[BB_PROT_NUM];                  /*!< Protocol callbacks. */

  uint8_t           protIdStarted;      /*!< Started protocol ID. */
  bool_t            protStarted;        /*!< Protocol started. */
  bool_t            termBod;            /*!< Terminate current BOD. */
} BbCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern BbCtrlBlk_t bbCb;
extern const BbRtCfg_t *pBbRtCfg;

#ifdef __cplusplus
};
#endif

#endif /* BB_INT_H */
