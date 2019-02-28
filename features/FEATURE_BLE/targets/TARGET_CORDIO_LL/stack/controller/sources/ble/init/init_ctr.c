/*************************************************************************************************/
/*!
 *  \brief  LL initialization for controller configuration.
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
 *  \brief  Initialize controller HCI transport.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitChciTrInit(void)
{
  wsfHandlerId_t handlerId = WsfOsSetNextHandler(ChciTrHandler);
  ChciTrHandlerInit(handlerId);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL HCI.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlInitLhciInit(void)
{
  #ifdef INIT_CENTRAL
    LhciScanMasterInit();
    #ifdef INIT_ENCRYPTED
      LhciEncMasterInit();
    #endif
    LhciConnMasterInit();
  #else
    #ifdef INIT_OBSERVER
      LhciScanMasterInit();
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    LhciAdvSlaveInit();
    #ifdef INIT_ENCRYPTED
      LhciEncSlaveInit();
    #endif
    LhciConnInit();
  #else
    #ifdef INIT_BROADCASTER
      LhciAdvSlaveInit();
    #endif
  #endif

  #ifdef INIT_ENCRYPTED
    LhciPrivAdvInit();
    LhciPrivConnInit();
    LhciScInit();
  #endif

  wsfHandlerId_t handlerId = WsfOsSetNextHandler(LhciHandler);
  LhciHandlerInit(handlerId);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize standard controller configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
uint32_t LlInitControllerInit(LlInitRtCfg_t *pCfg)
{
  uint32_t totalMemUsed;

  totalMemUsed = LlInitStdInit(pCfg);

  LlInitChciTrInit();
  LlInitLhciInit();

  LhciVsExtInit(lhciVsExtDecodeCmdPkt);

  return totalMemUsed;
}
