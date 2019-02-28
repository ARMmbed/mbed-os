/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief LL initialization for SoC configuration.
 */
/*************************************************************************************************/

#include "ll_init_api.h"
#include "bb_ble_api.h"
#include "sch_api.h"

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize BB.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitBbInit(void)
{
  BbInit();

  BbBleInit();

  #ifdef INIT_CENTRAL
    BbBleScanMasterInit();
    BbBleConnMasterInit();
  #else
    #ifdef INIT_OBSERVER
      BbBleScanMasterInit();
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    BbBleAdvSlaveInit();
    BbBleConnSlaveInit();
  #else
    #ifdef INIT_BROADCASTER
      BbBleAdvSlaveInit();
    #endif
  #endif

  BbBleTestInit();
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize scheduler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitSchInit(void)
{
  wsfHandlerId_t handlerId = WsfOsSetNextHandler(SchHandler);
  SchHandlerInit(handlerId);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL.
 *
 *  \param  initHandler     Initialize WSF handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitLlInit(bool_t initHandler)
{
  if (initHandler)
  {
    wsfHandlerId_t handlerId = WsfOsSetNextHandler(LlHandler);
    LlHandlerInit(handlerId);
  }
  /* else LlHandlerInit() is called by client */

  #ifdef INIT_CENTRAL
    LlScanMasterInit();
    LlInitMasterInit();
    #ifdef INIT_ENCRYPTED
      LlEncConnMasterInit();
    #else
      LlConnMasterInit();
    #endif
  #else
    #ifdef INIT_OBSERVER
      LlScanMasterInit();
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    LlAdvSlaveInit();
    #ifdef INIT_ENCRYPTED
      LlEncConnSlaveInit();
    #else
      LlConnSlaveInit();
    #endif
  #else
    #ifdef INIT_BROADCASTER
      LlAdvSlaveInit();
    #endif
  #endif

  #ifdef INIT_ENCRYPTED
    LlPrivInit();
    LlScInit();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief  Set BB runtime configuration.
 *
 *  \param  pBbRtCfg        BB runtime configuration (must be static).
 *  \param  wlSizeCfg       Whitelist size.
 *  \param  rlSizeCfg       Resolving list size.
 *  \param  plSizeCfg       Periodic list size.
 *  \param  pFreeMem        Free memory.
 *  \param  freeMemAvail    Amount of free memory in bytes.
 *
 *  \return Amount of free memory consumed.
 */
/*************************************************************************************************/
uint32_t LlInitSetBbRtCfg(const BbRtCfg_t *pBbRtCfg, const uint8_t wlSizeCfg, const uint8_t rlSizeCfg,
                          const uint8_t plSizeCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
{
  uint32_t memUsed;
  uint32_t totalMemUsed = 0;

  BbInitRunTimeCfg(pBbRtCfg);

  memUsed = BbBleInitWhiteList(wlSizeCfg, pFreeMem, freeMemAvail);
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  memUsed = BbBleInitPeriodicList(plSizeCfg, pFreeMem, freeMemAvail);
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  #ifdef INIT_ENCRYPTED
    memUsed = BbBleInitResolvingList(rlSizeCfg, pFreeMem, freeMemAvail);
    /* pFreeMem += memUsed; */
    /* freeMemAvail -= memUsed; */
    totalMemUsed += memUsed;
  #else
    BbBleInitResolvingList(0, NULL, 0);
  #endif

  return totalMemUsed;
}

/*************************************************************************************************/
/*!
 *  \brief  Set LL runtime configuration.
 *
 *  \param  pLlRtCfg        LL runtime configuration (must be static).
 *  \param  pFreeMem        Free memory.
 *  \param  freeMemAvail    Amount of free memory in bytes.
 *
 *  \return Amount of free memory consumed.
 */
/*************************************************************************************************/
uint32_t LlInitSetLlRtCfg(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
{
  uint32_t totalMemUsed = 0;

  LlInitRunTimeCfg(pLlRtCfg);

  #if defined (INIT_PERIPHERAL) || defined (INIT_CENTRAL)
    uint32_t memUsed;
    memUsed = LlInitConnMem(pFreeMem, freeMemAvail);
    /* pFreeMem += memUsed; */
    /* freeMemAvail -= memUsed; */
    totalMemUsed += memUsed;
  #endif

  return totalMemUsed;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize standard configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
uint32_t LlInitStdInit(LlInitRtCfg_t *pCfg)
{
  uint32_t memUsed;
  uint32_t totalMemUsed = 0;

  memUsed = LlInitSetBbRtCfg(pCfg->pBbRtCfg, pCfg->wlSizeCfg, pCfg->rlSizeCfg, pCfg->plSizeCfg,
                            pCfg->pFreeMem, pCfg->freeMemAvail);
  pCfg->pFreeMem += memUsed;
  pCfg->freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  memUsed = LlInitSetLlRtCfg(pCfg->pLlRtCfg, pCfg->pFreeMem, pCfg->freeMemAvail);
  pCfg->pFreeMem += memUsed;
  pCfg->freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  LlInitBbInit();
  LlInitSchInit();
  LlInitLlInit(TRUE);

  return totalMemUsed;
}
