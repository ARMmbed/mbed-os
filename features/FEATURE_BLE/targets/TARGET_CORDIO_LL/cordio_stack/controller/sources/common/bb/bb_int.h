/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal baseband interface file.
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
