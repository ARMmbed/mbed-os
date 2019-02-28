/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL initialization implementation file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 *
 *
 *  Initialization conditional compilation are used to control LL initialization options.
 *  Define one or more of the following to enable roles and features.
 *
 *    - INIT_BROADCASTER (default)
 *    - INIT_OBSERVER
 *    - INIT_PERIPHERAL
 *    - INIT_CENTRAL
 *    - INIT_ENCRYPTED
 *
 *  \note   Each feature may require additional \ref LlRtCfg_t requirements.
 */
/*************************************************************************************************/

#ifndef LL_INIT_API_H
#define LL_INIT_API_H

#include "wsf_types.h"
#include "bb_api.h"
#include "ll_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_INIT_API
 *  \{
 */

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef INIT_BROADCASTER
/*! \brief  Initialize broadcaster feature. */
#define INIT_BROADCASTER
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Runtime configuration parameters. */
typedef struct
{
  const BbRtCfg_t   *pBbRtCfg;      /*!< BB runtime configuration. */
  const uint8_t     wlSizeCfg;      /*!< Whitelist size configuration. */
  const uint8_t     rlSizeCfg;      /*!< Resolving list size configuration. */
  const uint8_t     plSizeCfg;      /*!< Periodic list size configuration. */
  const LlRtCfg_t   *pLlRtCfg;      /*!< LL runtime configuration. */
  uint8_t           *pFreeMem;      /*!< Pointer to free memory, returns adjusted location of free memory. */
  uint32_t          freeMemAvail;   /*!< Amount of free memory available, returns adjusted amount of free memory. */
} LlInitRtCfg_t;

/**************************************************************************************************
  Functions Declarations
**************************************************************************************************/

/* System initializers. */
uint32_t LlInitStdInit(LlInitRtCfg_t *pCfg);
uint32_t LlInitExtInit(LlInitRtCfg_t *pCfg);
uint32_t LlInitControllerInit(LlInitRtCfg_t *pCfg);
uint32_t LlInitControllerExtInit(LlInitRtCfg_t *pCfg);

/* Intermediate initializers. */
uint32_t LlInitSetBbRtCfg(const BbRtCfg_t *pBbRtCfg, const uint8_t wlSizeCfg, const uint8_t rlSizeCfg,
                          const uint8_t plSizeCfg, uint8_t *pFreeMem, uint32_t freeMemAvail);
uint32_t LlInitSetLlRtCfg(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail);
void LlInitBbInit(void);
void LlInitBbAuxInit(void);
void LlInitSchInit(void);
void LlInitLlInit(bool_t initHandler);
void LlInitChciTrInit(void);
void LlInitLhciInit(void);
void LlMathSetSeed(const uint32_t *pSeed);

/*! \} */    /* LL_INIT_API */

#ifdef __cplusplus
};
#endif

#endif /* LL_INIT_API_H */
