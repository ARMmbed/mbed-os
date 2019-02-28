/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Generic BLE baseband driver implementation file.
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

#include "bb_api.h"
#include "bb_ble_int.h"
#include "bb_ble_drv.h"
#include <string.h>

/**************************************************************************************************
  Globals
**************************************************************************************************/

bbBleCtrlBlk_t bbBleCb;               /*!< BB BLE control block. */

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of BLE protocol.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbBleStartBle(void)
{
  BbBleDrvEnable();
  BbBleDrvEnableDataWhitening(TRUE);
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of BLE protocol.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbBleStopBle(void)
{
  BbBleDrvDisable();
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of BLE DTM protocol.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbBleStartBleDtm(void)
{
  BbBleDrvEnable();
  BbBleDrvEnableDataWhitening(FALSE);
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of PRBS15 protocol.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbBleStartPrbs15(void)
{
  BbBleDrvEnable();
  BbBleDrvEnableDataWhitening(FALSE);
  BbBleDrvEnablePrbs15(TRUE);
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of PRBS15 protocol.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbBleStopPrbs15(void)
{
  BbBleDrvDisable();
  BbBleDrvEnablePrbs15(FALSE);
}

/*************************************************************************************************/
/*!
 *  \brief      Execute operation.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbBleExecOp(BbOpDesc_t *pBod)
{
  BbBleData_t * const pBle = pBod->prot.pBle;

  WSF_ASSERT(pBle->chan.opType < BB_BLE_OP_NUM);

  if (bbBleCb.opCbacks[pBle->chan.opType].execOpCback)
  {
    bbBleCb.opCbacks[pBle->chan.opType].execOpCback(pBod, pBle);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Cancel operation.
 *
 *  \param      pBod    Pointer to the BOD to cancel.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbBleCancelOp(BbOpDesc_t *pBod)
{
  BbBleData_t * const pBle = pBod->prot.pBle;

  WSF_ASSERT(pBle->chan.opType < BB_BLE_OP_NUM);

  if (bbBleCb.opCbacks[pBle->chan.opType].cancelOpCback)
  {
    bbBleCb.opCbacks[pBle->chan.opType].cancelOpCback(pBod, pBle);
  }
}
/*************************************************************************************************/
/*!
 *  \brief      Initialize the BLE BB.
 *
 *  \return     None.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void BbBleInit(void)
{
  BbBleDrvInit();
  BbRegisterProt(BB_PROT_BLE,     bbBleExecOp, bbBleCancelOp, bbBleStartBle,    bbBleStopBle);
  BbRegisterProt(BB_PROT_BLE_DTM, bbBleExecOp, bbBleCancelOp, bbBleStartBleDtm, bbBleStopBle);
  BbRegisterProt(BB_PROT_PRBS15,  NULL,        bbBleCancelOp, bbBleStartPrbs15, bbBleStopPrbs15);

  memset(&bbBleCb, 0, sizeof(bbBleCb));
}

/*************************************************************************************************/
/*!
 *  \brief      Register operation handlers.
 *
 *  \param      opType          Operation type.
 *  \param      execOpCback     Execute operation callback.
 *  \param      cancelOpCback   Cancel operation callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void bbBleRegisterOp(uint8_t opType, bbBleExecOpFn_t execOpCback, bbBleExecOpFn_t cancelOpCback)
{
  bbBleCb.opCbacks[opType].execOpCback   = execOpCback;
  bbBleCb.opCbacks[opType].cancelOpCback = cancelOpCback;
}
