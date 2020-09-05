/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal baseband interface file.
 *
 *  Copyright (c) 2016-2018 ARM Ltd. All Rights Reserved.
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

#ifndef BB_INT_H
#define BB_INT_H

#include "bb_api.h"
#include "pal_bb.h"
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
    BbLowPowerCback_t lowPowerOpCback;  /*!<  Low power operation handler. */
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
