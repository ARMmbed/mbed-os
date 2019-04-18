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
 * \brief Link layer (LL) slave initialization implementation file.
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lctr_api.h"
#include "lmgr_api.h"
#include "pal_bb.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Runtime configuration. */
const LlRtCfg_t *pLctrRtCfg = NULL;

/*! \brief      Handler duration watermark in microseconds. */
static uint16_t llHandlerWatermarkUsec = 0;

/*************************************************************************************************/
/*!
 *  \brief      Get default runtime configuration values.
 *
 *  \param      pCfg            Pointer to runtime configuration parameters.
 *
 *  \return     None.
 *
 *  This function returns default value for the LL subsystem's runtime configurations.
 */
/*************************************************************************************************/
void LlGetDefaultRunTimeCfg(LlRtCfg_t *pCfg)
{
  /* Set minimum values and expect client to override. */
  const LlRtCfg_t defCfg =
  {
    /* Device */
    .compId             = LL_COMP_ID_ARM,
    .implRev            = LL_VER_NUM >> 16,
    .btVer              = LL_VER_BT_CORE_SPEC_4_2,
    /* Advertiser */
    .maxAdvSets         = 0,   /* Disable extended advertising. */
    .maxAdvReports      = 4,
    .maxExtAdvDataLen   = LL_EXT_ADVBU_MAX_LEN,
    .defExtAdvDataFrag  = 64,
    .auxDelayUsec       = 0,
    .auxPtrOffsetUsec   = 2,
    /* Scanner */
    .maxScanReqRcvdEvt  = 4,
    .maxExtScanDataLen  = LL_EXT_ADVBU_MAX_LEN,
    /* Connection */
    .maxConn            = 0,   /* Disable connections. */
    .numTxBufs          = 4,
    .numRxBufs          = 4,
    .maxAclLen          = 27,
    .defTxPwrLvl        = 0,
    .ceJitterUsec       = 0,
    /* ISO */
    .numIsoTxBuf        = 0,
    .numIsoRxBuf        = 0,
    .maxIsoBufLen       = 0,
    .maxIsoPduLen       = 0,
    .maxCis             = 0,  /* Disable CIS. */
    .subEvtSpaceDelay   = 0,
    /* DTM */
    .dtmRxSyncMs        = 10000,
    /* PHY */
    .phy2mSup           = FALSE,
    .phyCodedSup        = FALSE,
    .stableModIdxTxSup  = FALSE,
    .stableModIdxRxSup  = FALSE
  };

  *pCfg = defCfg;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize runtime configuration.
 *
 *  \param      pCfg            Pointer to runtime configuration parameters (data must be static).
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem's runtime configuration.
 *
 *  \note       This routine must be called only once before any other initialization routines.
 */
/*************************************************************************************************/
void LlInitRunTimeCfg(const LlRtCfg_t *pCfg)
{
  WSF_ASSERT(pLctrRtCfg == NULL);
  WSF_ASSERT(pCfg);

  WSF_ASSERT(pCfg->btVer >= LL_VER_BT_CORE_SPEC_4_0);
  WSF_ASSERT(pCfg->maxAdvReports > 0);
  WSF_ASSERT(pCfg->maxAdvSets <= LL_MAX_ADV_SETS);
  WSF_ASSERT((pCfg->maxAdvSets == 0) || (pCfg->maxExtAdvDataLen >= LL_ADVBU_MAX_LEN));
  WSF_ASSERT(pCfg->numTxBufs > 0);
  WSF_ASSERT(pCfg->numRxBufs > 0);
  WSF_ASSERT(pCfg->maxAclLen >= LL_MAX_DATA_LEN_MIN);
  WSF_ASSERT(pCfg->maxConn <= LL_MAX_CONN);
  WSF_ASSERT(pCfg->dtmRxSyncMs > 0);

  pLctrRtCfg = pCfg;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem with task handler.
 *
 *  \param      handlerId  WSF handler ID.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem.  It is called once upon system initialization.
 *  It must be called before any other function in the LL API is called.
 */
/*************************************************************************************************/
void LlHandlerInit(wsfHandlerId_t handlerId)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LL_TRACE_INFO0("LlHandlerInit: LL initialization completed");

  lmgrPersistCb.handlerId = handlerId;
  LctrSetSupStates();

  /* Setup default public address. */
  lmgrPersistCb.bdAddr = ((uint64_t)(pLctrRtCfg->compId) << 24) | 0x123456;

  /* Clear state. */
  lmgrCb.numConnEnabled = 0;
  lmgrCb.advEnabled = FALSE;
  lmgrCb.numExtAdvEnabled = 0;
  lmgrCb.numScanEnabled = 0;
  lmgrCb.numInitEnabled = 0;
  lmgrCb.numWlFilterEnabled = 0;
  lmgrCb.testEnabled = FALSE;
  lmgrCb.scaMod = 0;

  lmgrPersistCb.featuresDefault |= (pLctrRtCfg->phy2mSup) ? LL_FEAT_LE_2M_PHY : 0;
  lmgrPersistCb.featuresDefault |= (pLctrRtCfg->phyCodedSup) ? LL_FEAT_LE_CODED_PHY : 0;
  lmgrPersistCb.featuresDefault |= (pLctrRtCfg->stableModIdxTxSup) ? LL_FEAT_STABLE_MOD_IDX_TRANSMITTER : 0;
  lmgrPersistCb.featuresDefault |= (pLctrRtCfg->stableModIdxRxSup) ? LL_FEAT_STABLE_MOD_IDX_RECEIVER : 0;
  LmgrSetDefaults();

  LlTestInit();

  LL_TRACE_INFO1("    opModeFlags        = 0x%08x", lmgrCb.opModeFlags);
}

/*************************************************************************************************/
/*!
 *  \brief      LL message dispatch handler.
 *
 *  \param      event       WSF event.
 *  \param      pMsg        WSF message.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LlHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  bool_t startTimeValid;
  uint32_t startTime;
  uint32_t endTime;

  startTimeValid = PalBbGetTimestamp(&startTime);

  if (event != 0)
  {
    unsigned int eventBit = 0;
    do
    {
      if (event & (1 << eventBit))
      {
        LctrEventHandler(eventBit);
      }
    } while (++eventBit < LCTR_EVENT_TOTAL);
  }

  if (pMsg != NULL)
  {
    LctrMsgDispatcher((lctrMsgHdr_t *)pMsg);
  }

  if (startTimeValid &&
      PalBbGetTimestamp(&endTime))
  {
    uint32_t durUsec = BB_TICKS_TO_US(endTime - startTime);
    if (llHandlerWatermarkUsec < durUsec)
    {
      llHandlerWatermarkUsec = durUsec;
      LL_TRACE_INFO1("Raised watermark for LlHandler processing, time watermarkUsec=%u", durUsec);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Reset LL subsystem.
 *
 *  \return     None.
 *
 * Reset the LL subsystem.  All active connections are closed and all radio procedures such as
 * scanning or advertising are terminated.
 */
/*************************************************************************************************/
void LlReset(void)
{
  LL_TRACE_INFO0("### LlApi ###  LlReset");

  lctrMsgHdr_t *pMsg;

  /* Reset state machines. */
  if ((pMsg = WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->dispId = LCTR_DISP_BCST;
    pMsg->event = LCTR_MSG_RESET;
    pMsg->handle = 0xFF;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Register LL event handler.
 *
 *  \param      evtCback        Client callback function.
 *
 *  \return     None.
 *
 *  This function is called by a client to register for LL events.
 */
/*************************************************************************************************/
void LlEvtRegister(llEvtCback_t evtCback)
{
  lmgrPersistCb.evtCback = evtCback;
}

/*************************************************************************************************/
/*!
 *  \brief      Get advertising set context size.
 *
 *  \param      pMaxAdvSets     Buffer to return the maximum number of advertising sets.
 *  \param      pAdvSetCtxSize  Buffer to return the size in bytes of the advertising set context.
 *
 *  Return the advertising set context sizes.
 */
/*************************************************************************************************/
void LlGetAdvSetContextSize(uint8_t *pMaxAdvSets, uint16_t *pAdvSetCtxSize)
{
  if (pLctrRtCfg)
  {
    *pMaxAdvSets = pLctrRtCfg->maxAdvSets;
  }
  else
  {
    *pMaxAdvSets = 0;
  }

  *pAdvSetCtxSize = lmgrPersistCb.advSetCtxSize;
}

/*************************************************************************************************/
/*!
 *  \brief      Get connection context size.
 *
 *  \param      pMaxConn        Buffer to return the maximum number of connections.
 *  \param      pConnCtxSize    Buffer to return the size in bytes of the connection context.
 *
 *  Return the connection context sizes.
 */
/*************************************************************************************************/
void LlGetConnContextSize(uint8_t *pMaxConn, uint16_t *pConnCtxSize)
{
  if (pLctrRtCfg)
  {
    *pMaxConn = pLctrRtCfg->maxConn;
  }
  else
  {
    *pMaxConn = 0;
  }

  *pConnCtxSize = lmgrPersistCb.connCtxSize;
}

/*************************************************************************************************/
/*!
 *  \brief      Get extended scanner context size.
 *
 *  \param      pMaxPerScan     Buffer to return the maximum number of extended scanners.
 *  \param      pPerScanCtxSize Buffer to return the size in bytes of the extended scanner context.
 *
 *  Return the advertising set context sizes.
 */
/*************************************************************************************************/
void LlGetExtScanContextSize(uint8_t *pMaxExtScan, uint16_t *pExtScanCtxSize)
{
  *pMaxExtScan = LL_MAX_PHYS;

  *pExtScanCtxSize = lmgrPersistCb.extScanCtxSize;
}

/*************************************************************************************************/
/*!
 *  \brief      Get extended initiator context size.
 *
 *  \param      pMaxPerScan     Buffer to return the maximum number of extended initiators.
 *  \param      pPerScanCtxSize Buffer to return the size in bytes of the extended initiator context.
 *
 *  Return the advertising set context sizes.
 */
/*************************************************************************************************/
void LlGetExtInitContextSize(uint8_t *pMaxExtInit, uint16_t *pExtInitCtxSize)
{
  *pMaxExtInit = LL_MAX_PHYS;

  *pExtInitCtxSize = lmgrPersistCb.extInitCtxSize;
}

/*************************************************************************************************/
/*!
 *  \brief      Get periodic scan context size.
 *
 *  \param      pMaxPerScan     Buffer to return the maximum number of periodic scanners.
 *  \param      pPerScanCtxSize Buffer to return the size in bytes of the periodic scanner context.
 *
 *  Return the advertising set context sizes.
 */
/*************************************************************************************************/
void LlGetPerScanContextSize(uint8_t *pMaxPerScan, uint16_t *pPerScanCtxSize)
{
  *pMaxPerScan = LL_MAX_PER_SCAN;

  *pPerScanCtxSize = lmgrPersistCb.perScanCtxSize;
}

/*************************************************************************************************/
/*!
 *  \brief      Get CIG context size.
 *
 *  \param      pMaxCig         Buffer to return the maximum number of CIG.
 *  \param      pCigCtxSize     Buffer to return the size in bytes of the CIG context.
 *
 *  Return the connection context sizes.
 */
/*************************************************************************************************/
void LlGetCigContextSize(uint8_t *pMaxCig, uint16_t *pCigCtxSize)
{
  if (pLctrRtCfg)
  {
    *pMaxCig = pLctrRtCfg->maxCig;
  }
  else
  {
    *pMaxCig = 0;
  }

  *pCigCtxSize = lmgrPersistCb.cigCtxSize;
}

/*************************************************************************************************/
/*!
 *  \brief      Get CIS context size.
 *
 *  \param      pMaxCis         Buffer to return the maximum number of CIS.
 *  \param      pCisCtxSize     Buffer to return the size in bytes of the CIS context.
 *
 *  Return the connection context sizes.
 */
/*************************************************************************************************/
void LlGetCisContextSize(uint8_t *pMaxCis, uint16_t *pCisCtxSize)
{
  if (pLctrRtCfg)
  {
    *pMaxCis = pLctrRtCfg->maxCis;
  }
  else
  {
    *pMaxCis = 0;
  }

  *pCisCtxSize = lmgrPersistCb.cisCtxSize;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the LL handler watermark level.
 *
 *  \return     Watermark level in microseconds.
 */
/*************************************************************************************************/
uint16_t LlStatsGetHandlerWatermarkUsec(void)
{
  return llHandlerWatermarkUsec;
}
