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
 *  \brief Link layer (LL) slave initialization implementation file.
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lctr_api.h"
#include "lmgr_api.h"
#include "bb_drv.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "wsf_msg.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Typical implementation revision number (LlRtCfg_t::implRev). */
#define LL_IMPL_REV             0x2303

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
    .implRev            = LL_IMPL_REV,
    .btVer              = LL_VER_BT_CORE_SPEC_4_2,
    /* Advertiser */
    .maxAdvSets         = 0,   /* Disable extended advertising. */
    .maxAdvReports      = 4,
    .maxExtAdvDataLen   = LL_EXT_ADVBU_MAX_LEN,
    .defExtAdvDataFrag  = 64,
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

  startTimeValid = BbDrvGetTimestamp(&startTime);

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
      BbDrvGetTimestamp(&endTime))
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
 *  \brief      Get the LL handler watermark level.
 *
 *  \return     Watermark level in microseconds.
 */
/*************************************************************************************************/
uint16_t LlStatsGetHandlerWatermarkUsec(void)
{
  return llHandlerWatermarkUsec;
}
