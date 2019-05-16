/*************************************************************************************************/
/*!
 *  \brief  LL initialization for extended controller configuration.
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
#include "lhci_api.h"
#include "chci_api.h"

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief      Extended VS command decoder. */
extern bool_t lhciVsExtDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL HCI extended features.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitLhciExtInit(void)
{
  LhciVsExtInit(lhciVsExtDecodeCmdPkt);

  #ifdef INIT_CENTRAL
    LhciExtScanMasterInit();
    LhciExtConnMasterInit();
  #else
    #ifdef INIT_OBSERVER
      LhciExtScanMasterInit();
    #endif
  #endif

  #ifdef INIT_BROADCASTER
    LhciExtAdvSlaveInit();
  #endif

  #if defined(INIT_PERIPHERAL) || defined(INIT_CENTRAL)
    LhciChannelSelection2Init();
    LhciPhyInit();
  #endif
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize extended controller configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
uint32_t LlInitControllerExtInit(LlInitRtCfg_t *pCfg)
{
  uint32_t totalMemUsed;

  totalMemUsed = LlInitExtInit(pCfg);

  LlInitChciTrInit();
  LlInitLhciInit();
  LlInitLhciExtInit();

  return totalMemUsed;
}
