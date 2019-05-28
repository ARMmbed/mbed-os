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
 * \brief LL initialization for SoC configuration.
 */
/*************************************************************************************************/

#include "ll_init_api.h"
#include "bb_ble_api.h"
#include "pal_bb_ble.h"
#include "pal_radio.h"
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

  #if (BT_VER >= LL_VER_BT_CORE_SPEC_5_0)
    #ifdef INIT_OBSERVER
      BbBleAuxScanMasterInit();
      BbBlePerScanMasterInit();
    #endif

    #ifdef INIT_BROADCASTER
      BbBleAuxAdvSlaveInit();
      PalRadioInitPathComp();
    #endif
  #endif

  #if (BT_VER >= LL_VER_BT_CORE_SPEC_MILAN)
    #ifdef INIT_CENTRAL
      BbBleCisMasterInit();
    #else
      #ifdef INIT_OBSERVER
      /* TODO BIS observer */
      #endif
    #endif

    #ifdef INIT_PERIPHERAL
      BbBleCisSlaveInit();
    #else
      #ifdef INIT_BROADCASTER
      /* TODO BIS broadcaster */
      #endif
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
 *  \return None.
 */
/*************************************************************************************************/
void LlInitLlInit(void)
{
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

  #if (BT_VER >= LL_VER_BT_CORE_SPEC_5_0)
    #ifdef INIT_CENTRAL
      LlExtScanMasterInit();
      LlExtInitMasterInit();
      LlPhyMasterInit();
      #if (BT_VER >= LL_VER_BT_CORE_SPEC_MILAN)
        LlCisMasterInit();
      #endif
    #else
      #ifdef INIT_OBSERVER
        LlExtScanMasterInit();
      #endif
    #endif

    #ifdef INIT_PERIPHERAL
      LlExtAdvSlaveInit();
      LlPhySlaveInit();
      #if (BT_VER >= LL_VER_BT_CORE_SPEC_MILAN)
        LlCisSlaveInit();
      #endif
    #else
      #ifdef INIT_BROADCASTER
        LlExtAdvSlaveInit();
      #endif
    #endif

    #if defined(INIT_PERIPHERAL) || defined(INIT_CENTRAL)
      LlChannelSelection2Init();
      #if (BT_VER >= LL_VER_BT_CORE_SPEC_5_1)
        LlPastInit();
      #endif
    #endif
  #endif

  /* Initialize handler after feature bits are set. */
  wsfHandlerId_t handlerId = WsfOsSetNextHandler(LlHandler);
  LlHandlerInit(handlerId);
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

  #if (BT_VER >= LL_VER_BT_CORE_SPEC_5_0)
    memUsed = BbBleInitPeriodicList(plSizeCfg, pFreeMem, freeMemAvail);
    pFreeMem += memUsed;
    freeMemAvail -= memUsed;
    totalMemUsed += memUsed;
  #endif

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

  #if defined (INIT_PERIPHERAL) || defined (INIT_CENTRAL) || (BT_VER >= LL_VER_BT_CORE_SPEC_5_0)
  uint32_t memUsed;
  #endif

  LlInitRunTimeCfg(pLlRtCfg);

  #if defined (INIT_PERIPHERAL) || defined (INIT_CENTRAL)
    memUsed = LlInitConnMem(pFreeMem, freeMemAvail);
    pFreeMem += memUsed;
    freeMemAvail -= memUsed;
    totalMemUsed += memUsed;
  #endif

  #if (BT_VER >= LL_VER_BT_CORE_SPEC_5_0)
    memUsed = LlInitExtScanMem(pFreeMem, freeMemAvail);
    pFreeMem += memUsed;
    freeMemAvail -= memUsed;
    totalMemUsed += memUsed;

    memUsed = LlInitExtAdvMem(pFreeMem, freeMemAvail);
    pFreeMem += memUsed;
    freeMemAvail -= memUsed;
    totalMemUsed += memUsed;
  #endif

  #if (BT_VER >= LL_VER_BT_CORE_SPEC_MILAN)
    memUsed = LlInitCisMem(pFreeMem, freeMemAvail);
    pFreeMem += memUsed;
    freeMemAvail -= memUsed;
    totalMemUsed += memUsed;

    memUsed = LlInitIsoMem(pFreeMem, freeMemAvail);
    /* pFreeMem += memUsed;
    freeMemAvail -= memUsed; */
    totalMemUsed += memUsed;
  #endif

  return totalMemUsed;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
uint32_t LlInit(LlInitRtCfg_t *pCfg)
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
  LlInitLlInit();

  return totalMemUsed;
}
