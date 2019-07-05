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
 * \brief Generic baseband driver implementation file.
 */
/*************************************************************************************************/

#include <string.h>
#include "bb_api.h"
#include "bb_int.h"
#include "pal_bb.h"

/**************************************************************************************************
  Globals
**************************************************************************************************/

BbCtrlBlk_t bbCb;                       /*!< BB control block. */
const BbRtCfg_t *pBbRtCfg = NULL;       /*!< Runtime configuration. */

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize runtime configuration.
 *
 *  \param      pCfg        Pointer to runtime configuration parameters (data must be static).
 *
 *  \return     None.
 *
 *  This function initializes the BB subsystem's runtime configuration.
 *
 *  \note       This routine must be called only once before any other initialization routine.
 */
/*************************************************************************************************/
void BbInitRunTimeCfg(const BbRtCfg_t *pCfg)
{
  WSF_ASSERT(pBbRtCfg == NULL);
  WSF_ASSERT(pCfg);

  WSF_ASSERT(pCfg->clkPpm >= 20);
  WSF_ASSERT(pCfg->rfSetupDelayUs > 0);
  WSF_ASSERT(pCfg->maxScanPeriodMs > 0);
  WSF_ASSERT(pCfg->schSetupDelayUs > 0);

  pBbRtCfg = pCfg;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BB.
 *
 *  \return     None.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
void BbInit(void)
{
  WSF_ASSERT(pBbRtCfg);

  PalBbInit();

  memset(&bbCb, 0, sizeof(bbCb));
}

/*************************************************************************************************/
/*!
 *  \brief      Register operation completion handler.
 *
 *  \param      eventCback  Event callback.
 *
 *  \return     None.
 *
 *  Register operation completion handler.
 */
/*************************************************************************************************/
void BbRegister(BbBodCompCback_t eventCback)
{
  bbCb.bodCompCback = eventCback;
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbProtStart(uint8_t protId)
{
  WSF_ASSERT(bbCb.prot[protId].startProtCback != NULL);

  /* Enable protocol-specific BB. */
  bbCb.prot[protId].startProtCback();

  /* Protocol now started. */
  bbCb.protStarted   = TRUE;
  bbCb.protIdStarted = protId;
  PalBbSetProtId(protId);
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 *
 *  Enable BB and start processing the list of BODs.  This routine may be called several times, if
 *  a protocol layers has several simultaneously-enabled operations.  However, \ref BbStop() must
 *  be called an equal number of time to disable the baseband.
 */
/*************************************************************************************************/
void BbStart(uint8_t protId)
{
  WSF_ASSERT(protId < BB_PROT_NUM);

  if (!bbCb.protStarted)
  {
    /* Enable generic BB. */
    PalBbEnable();

    /* Enable protocol-specific BB. */
    bbProtStart(protId);
  }

  bbCb.prot[protId].startCnt++;
}

/*************************************************************************************************/
/*!
 *  \brief      Stop BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbProtStop(uint8_t protId)
{
  WSF_ASSERT(bbCb.prot[protId].stopProtCback != NULL);

  /* No protocol started. */
  bbCb.protStarted = FALSE;

  /* Disable protocol-specific BB. */
  bbCb.prot[protId].stopProtCback();
}

/*************************************************************************************************/
/*!
 *  \brief      Stop BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  Disable BB processing of BODs.
 *
 *  \note       For any particular protocol, calls to \ref BbStart() and \ref BbStop() must be
 *              balanced to ensure that the hardware is disabled if and only if appropriate.
 */
/*************************************************************************************************/
void BbStop(uint8_t protId)
{
  WSF_ASSERT(protId < BB_PROT_NUM);
  WSF_ASSERT(bbCb.prot[protId].startCnt > 0);

  bbCb.prot[protId].startCnt--;

  if (bbCb.protStarted && (bbCb.protIdStarted == protId) && (bbCb.prot[protId].startCnt == 0))
  {
    /* Disable protocol-specific BB. */
    bbProtStop(protId);

    /* Disable generic BB. */
    PalBbDisable();
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Execute BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *
 *  \return     None.
 *
 *  Execute the protocol specific BOD handler.
 */
/*************************************************************************************************/
void BbExecuteBod(BbOpDesc_t *pBod)
{
  WSF_ASSERT(pBod);

  WSF_ASSERT(pBod->protId < BB_PROT_NUM);
  /* TODO: Removed this assert as it spuriously seems to be taken. */
  /* WSF_ASSERT(!bbCb.pOpInProgress); */
  bbCb.pOpInProgress = pBod;
  bbCb.termBod = FALSE;

  /* Enable generic BB. */
  if (!bbCb.protStarted)
  {
    PalBbEnable();
  }

  /* Switch protocols if necessary. */
  if (bbCb.protStarted && (bbCb.protIdStarted != pBod->protId))
  {
    /* Disable protocol-specific BB. */
    bbProtStop(bbCb.protIdStarted);   /* sets bbCb.protStarted = FALSE */
  }
  if (!bbCb.protStarted)
  {
    /* Enable protocol-specific BB. */
    bbProtStart(pBod->protId);
  }

  if (bbCb.prot[pBod->protId].execOpCback != NULL)
  {
    bbCb.prot[pBod->protId].execOpCback(pBod);
  }

  if (bbCb.termBod)
  {
    bbCb.pOpInProgress = NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Cancel current executing BOD.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbCancelBod(void)
{
  if (bbCb.pOpInProgress)
  {
    BbOpDesc_t * const pBod = bbCb.pOpInProgress;

    WSF_ASSERT(pBod->protId < BB_PROT_NUM);
    if (bbCb.prot[pBod->protId].cancelOpCback != NULL)
    {
      bbCb.prot[pBod->protId].cancelOpCback(pBod);
    }

    bbCb.pOpInProgress = NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Get the currently-executing BOD.
 *
 *  \return     Currently-executing BOD.
 */
/*************************************************************************************************/
BbOpDesc_t *BbGetCurrentBod(void)
{
  return bbCb.pOpInProgress;
}

/*************************************************************************************************/
/*!
 *  \brief      Set termination flag of current executing BOD.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the execution context of the
 *              current executing BOD, typically in the related ISRs. In the end, termination
 *              flag will help to decide if BbTerminateBod() should be called.
 */
/*************************************************************************************************/
void BbSetBodTerminateFlag(void)
{
  if (bbCb.pOpInProgress)
  {
    bbCb.termBod = TRUE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Get termination state of current executing BOD.
 *
 *  \return     TRUE if termination flag set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbGetBodTerminateFlag(void)
{
  return bbCb.termBod;
}

/*************************************************************************************************/
/*!
 *  \brief      Terminate a BOD immediately.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the execution context of the
 *              current executing BOD, typically in the related ISRs.
 */
/*************************************************************************************************/
void BbTerminateBod(void)
{
  WSF_ASSERT(bbCb.bodCompCback);

  bbCb.pOpInProgress = NULL;
  bbCb.termBod = TRUE;
  bbCb.bodCompCback();
}

/*************************************************************************************************/
/*!
 *  \brief      Get BB clock accuracy.
 *
 *  \return     Clock accuracy in part per million.
 *
 *  Returns the current BB clock accuracy.
 */
/*************************************************************************************************/
uint16_t BbGetClockAccuracy(void)
{
  return pBbRtCfg->clkPpm;
}

/*************************************************************************************************/
/*!
 *  \brief      Get scheduler setup delay.
 *
 *  \return     Scheduler setup delay in microseconds.
 *
 *  Returns the scheduler setup delay.
 */
/*************************************************************************************************/
uint16_t BbGetSchSetupDelayUs(void)
{
  return pBbRtCfg->schSetupDelayUs;
}

/*************************************************************************************************/
/*!
 *  \brief      Returns the ID of the active protocol.
 *
 *  \return     Protocol operation in progress.
 */
/*************************************************************************************************/
uint8_t BbGetActiveProtocol(void)
{
  return bbCb.protIdStarted;
}

/*************************************************************************************************/
/*!
 *  \brief      Register protocol handlers.
 *
 *  \param      protId          Protocol ID.
 *  \param      execOpCback     Execute operation callback.
 *  \param      cancelOpCback   Cancel operation callback.
 *  \param      startProtCback  Start protocol callback.
 *  \param      stopProtCback   Stop protocol callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbRegisterProt(uint8_t protId, BbBodCback_t execOpCback, BbBodCback_t cancelOpCback,
    BbProtCback_t startProtCback, BbProtCback_t stopProtCback)
{
  WSF_ASSERT(protId < BB_PROT_NUM);
  WSF_ASSERT(startProtCback != NULL);
  WSF_ASSERT(stopProtCback != NULL);

  bbCb.prot[protId].execOpCback    = execOpCback;
  bbCb.prot[protId].cancelOpCback  = cancelOpCback;
  bbCb.prot[protId].startProtCback = startProtCback;
  bbCb.prot[protId].stopProtCback  = stopProtCback;
}
