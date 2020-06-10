/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Generic BLE baseband driver implementation file.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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

#include "bb_api.h"
#include "bb_ble_int.h"
#include "pal_bb_ble.h"
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
 */
/*************************************************************************************************/
static void bbBleStartBle(void)
{
  PalBbBleEnable();
  PalBbBleEnableDataWhitening(TRUE);
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of BLE protocol.
 */
/*************************************************************************************************/
static void bbBleStopBle(void)
{
  PalBbBleDisable();
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of BLE DTM protocol.
 */
/*************************************************************************************************/
static void bbBleStartBleDtm(void)
{
  PalBbBleEnable();
  PalBbBleEnableDataWhitening(FALSE);
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of PRBS15 protocol.
 */
/*************************************************************************************************/
static void bbBleStartPrbs15(void)
{
  PalBbBleEnable();
  PalBbBleEnableDataWhitening(FALSE);
  PalBbBleEnablePrbs15(TRUE);
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of PRBS15 protocol.
 */
/*************************************************************************************************/
static void bbBleStopPrbs15(void)
{
  PalBbBleDisable();
  PalBbBleEnablePrbs15(FALSE);
}

/*************************************************************************************************/
/*!
 *  \brief      Execute operation.
 *
 *  \param      pBod    Pointer to the BOD to execute.
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
 *  \brief      Low power operation.
 *
 */
/*************************************************************************************************/
static void bbBleLowPower(void)
{
  PalBbBleLowPower();
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BLE BB.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void BbBleInit(void)
{
  PalBbBleInit();
  BbRegisterProt(BB_PROT_BLE,     bbBleExecOp, bbBleCancelOp, bbBleStartBle,    bbBleStopBle);
  BbRegisterProt(BB_PROT_BLE_DTM, bbBleExecOp, bbBleCancelOp, bbBleStartBleDtm, bbBleStopBle);
  BbRegisterProt(BB_PROT_PRBS15,  NULL,        bbBleCancelOp, bbBleStartPrbs15, bbBleStopPrbs15);
  BbRegisterProtLowPower(BB_PROT_BLE, bbBleLowPower);

  memset(&bbBleCb, 0, sizeof(bbBleCb));
}

/*************************************************************************************************/
/*!
 *  \brief      Register operation handlers.
 *
 *  \param      opType          Operation type.
 *  \param      execOpCback     Execute operation callback.
 *  \param      cancelOpCback   Cancel operation callback.
 */
/*************************************************************************************************/
void bbBleRegisterOp(uint8_t opType, bbBleExecOpFn_t execOpCback, bbBleExecOpFn_t cancelOpCback)
{
  bbBleCb.opCbacks[opType].execOpCback   = execOpCback;
  bbBleCb.opCbacks[opType].cancelOpCback = cancelOpCback;
}
