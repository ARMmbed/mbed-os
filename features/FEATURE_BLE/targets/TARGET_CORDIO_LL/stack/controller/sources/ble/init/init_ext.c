/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL initialization for extended SoC configuration.
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

#include "ll_init_api.h"
#include "bb_ble_api.h"

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize auxiliary BB operations.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitBbAuxInit(void)
{
  #ifdef INIT_OBSERVER
    BbBleAuxScanMasterInit();
    BbBlePerScanMasterInit();
  #endif

  #ifdef INIT_BROADCASTER
    BbBleAuxAdvSlaveInit();
    BbBleInitRfPathComp();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL extended features.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitLlExtInit(void)
{
  #ifdef INIT_CENTRAL
    LlExtScanMasterInit();
    LlExtInitMasterInit();
    LlPhyMasterInit();
  #else
    #ifdef INIT_OBSERVER
      LlExtScanMasterInit();
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    LlExtAdvSlaveInit();
    LlPhySlaveInit();
  #else
    #ifdef INIT_BROADCASTER
      LlExtAdvSlaveInit();
    #endif
  #endif

  #if defined(INIT_PERIPHERAL) || defined(INIT_CENTRAL)
    LlChannelSelection2Init();
  #endif
  /* Initialize handler after feature bits are set. */
  wsfHandlerId_t handlerId = WsfOsSetNextHandler(LlHandler);
  LlHandlerInit(handlerId);
}

/*************************************************************************************************/
/*!
 *  \brief  Set extended LL runtime configuration.
 *
 *  \param  pLlRtCfg        LL runtime configuration (must be static).
 *  \param  pFreeMem        Free memory.
 *  \param  freeMemAvail    Amount of free memory in bytes.
 *
 *  \return Amount of free memory consumed.
 */
/*************************************************************************************************/
uint32_t LlInitSetExtLlRtCfg(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
{
  uint32_t memUsed;
  uint32_t totalMemUsed = 0;

  memUsed = LlInitSetLlRtCfg(pLlRtCfg, pFreeMem, freeMemAvail);
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  memUsed = LlInitExtScanMem(pFreeMem, freeMemAvail);
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  memUsed = LlInitExtAdvMem(pFreeMem, freeMemAvail);
  /* pFreeMem += memUsed; */
  /* freeMemAvail -= memUsed; */
  totalMemUsed += memUsed;

  return totalMemUsed;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize extended configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
uint32_t LlInitExtInit(LlInitRtCfg_t *pCfg)
{
  uint32_t memUsed;
  uint32_t totalMemUsed = 0;

  memUsed = LlInitSetBbRtCfg(pCfg->pBbRtCfg, pCfg->wlSizeCfg, pCfg->rlSizeCfg, pCfg->plSizeCfg,
                            pCfg->pFreeMem, pCfg->freeMemAvail);
  pCfg->pFreeMem += memUsed;
  pCfg->freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  memUsed = LlInitSetExtLlRtCfg(pCfg->pLlRtCfg, pCfg->pFreeMem, pCfg->freeMemAvail);
  pCfg->pFreeMem += memUsed;
  pCfg->freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  LlInitBbInit();
  LlInitBbAuxInit();
  LlInitSchInit();
  LlInitLlInit(FALSE);
  LlInitLlExtInit();

  return totalMemUsed;
}
