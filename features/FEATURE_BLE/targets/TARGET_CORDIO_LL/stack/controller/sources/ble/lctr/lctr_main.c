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
 * \brief Main link layer controller implementation file.
 */
/*************************************************************************************************/

#include "lctr_int.h"
#include "lmgr_api.h"
#include "sch_api_ble.h"
#include "bb_ble_api_reslist.h"
#include "bb_ble_api_whitelist.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/**************************************************************************************************
  Globals
**************************************************************************************************/

/*! \brief      Link layer controller reset handler lookup table. */
LctrResetHdlr_t lctrResetHdlrTbl[LCTR_DISP_TOTAL];

/*! \brief      Link layer controller message dispatcher lookup table. */
LctrMsgDisp_t   lctrMsgDispTbl[LCTR_DISP_TOTAL];

/*! \brief      Link layer task event handler lookup table. */
LctrEvtHdlr_t   lctrEventHdlrTbl[LCTR_EVENT_TOTAL];

/*! \brief      Event message which triggered the current execution context. */
lctrMsgHdr_t    *pLctrMsg;

/*! \brief      Reset enabled. */
static bool_t   lctrResetEnabled = FALSE;

/*************************************************************************************************/
/*!
 *  \brief      Notify host reset confirm.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrNotifyHostResetCnf(void)
{
  wsfMsgHdr_t evt =
  {
    .event  = LL_RESET_CNF,
    .status = LL_SUCCESS
  };

  LL_TRACE_INFO0("### LlEvent ###  LL_RESET_CNF, status=LL_SUCCESS");

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Set the supporting state bitmask.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LctrSetSupStates(void)
{
  const bool_t supAdv  = lctrMsgDispTbl[LCTR_DISP_ADV]  ? TRUE : FALSE;
  const bool_t supScan = lctrMsgDispTbl[LCTR_DISP_SCAN] ? TRUE : FALSE;
  const bool_t supInit = lctrMsgDispTbl[LCTR_DISP_INIT] ? TRUE : FALSE;
  const bool_t supConn = lctrMsgDispTbl[LCTR_DISP_CONN] ? TRUE : FALSE;

  lmgrPersistCb.supStates = 0;

  /* Non-connectable advertising is supported. */
  if (supAdv)
  {
    lmgrPersistCb.supStates |= LL_SUP_STATE_NON_CONN_ADV |
                               LL_SUP_STATE_SCAN_ADV;
  }

  /* Non-connectable advertising while scanning is supported. */
  if (supAdv && supScan)
  {
    lmgrPersistCb.supStates |= LL_SUP_STATE_NON_CONN_ADV_AND_PASS_SCAN |
                               LL_SUP_STATE_SCAN_ADV_AND_PASS_SCAN |
                               LL_SUP_STATE_NON_CONN_ADV_AND_ACT_SCAN |
                               LL_SUP_STATE_SCAN_ADV_AND_ACT_SCAN |
                               LL_SUP_STATE_LO_DUTY_DIR_ADV_AND_PASS_SCAN |
                               LL_SUP_STATE_LO_DUTY_DIR_ADV_AND_ACT_SCAN;
  }

  /* Connectable advertising and slave connection are supported. */
  if (supAdv && supConn)
  {
    lmgrPersistCb.supStates |= LL_SUP_STATE_CONN_ADV |
                               LL_SUP_STATE_HI_DUTY_DIR_ADV |
                               LL_SUP_STATE_LO_DUTY_DIR_ADV |
                               LL_SUP_STATE_CONN_SLV;
    if (pLctrRtCfg->maxConn > 1)
    {
      lmgrPersistCb.supStates |= LL_SUP_STATE_CONN_ADV_SLV |
                                 LL_SUP_STATE_HI_DUTY_DIR_ADV_SLV |
                                 LL_SUP_STATE_LO_DUTY_DIR_ADV_SLV;
    }
  }

  /* Scanning is supported. */
  if (supScan)
  {
    lmgrPersistCb.supStates |= LL_SUP_STATE_PASS_SCAN |
                               LL_SUP_STATE_ACT_SCAN;
  }

  /* Initiating and scanning while connected are supported. */
  if (supScan && supConn)
  {
    lmgrPersistCb.supStates |= LL_SUP_STATE_INIT |
                               LL_SUP_STATE_PASS_SCAN_MST |
                               LL_SUP_STATE_ACT_SCAN_MST |
                               LL_SUP_STATE_PASS_SCAN_SLV |
                               LL_SUP_STATE_ACT_SCAN_SLV;

    if (pLctrRtCfg->maxConn > 1)
    {
      lmgrPersistCb.supStates |= LL_SUP_STATE_INIT_MST;
    }

    if (supInit)
    {
      lmgrPersistCb.supStates |= LL_SUP_STATE_PASS_SCAN_AND_INIT |
                                 LL_SUP_STATE_ACT_SCAN_AND_INIT;
    }
  }

  /* Connectable advertising while scanning and advertising while initiating and advertising while connected are supported. */
  if (supAdv && supScan && supConn)
  {
    lmgrPersistCb.supStates |= LL_SUP_STATE_CONN_ADV_AND_PASS_SCAN |
                               LL_SUP_STATE_HI_DUTY_DIR_ADV_AND_PASS_SCAN |
                               LL_SUP_STATE_CONN_ADV_AND_ACT_SCAN |
                               LL_SUP_STATE_HI_DUTY_DIR_ADV_ACT_SCAN |
                               LL_SUP_STATE_NON_CONN_ADV_AND_INIT |
                               LL_SUP_STATE_SCAN_ADV_AND_INIT |
                               LL_SUP_STATE_NON_CONN_ADV_MST |
                               LL_SUP_STATE_SCAN_ADV_MST |
                               LL_SUP_STATE_NON_CONN_ADV_SLV |
                               LL_SUP_STATE_SCAN_ADV_SLV;

    if (pLctrRtCfg->maxConn > 1)
    {
      lmgrPersistCb.supStates |= LL_SUP_STATE_CONN_ADV_AND_INIT |
                                 LL_SUP_STATE_HI_DUTY_DIR_ADV_AND_INIT |
                                 LL_SUP_STATE_LO_DUTY_DIR_ADV_AND_INIT |
                                 LL_SUP_STATE_CONN_ADV_MST |
                                 LL_SUP_STATE_HI_DUTY_DIR_ADV_MST |
                                 LL_SUP_STATE_LO_DUTY_DIR_ADV_MST |
                                 LL_SUP_STATE_INIT_SLV;
    }
  }

  LL_TRACE_INFO1("    LCTR_DISP_CONN_IND = %c", lctrMsgDispTbl[LCTR_DISP_CONN_IND] ? 'Y' : 'N');
  LL_TRACE_INFO1("    LCTR_DISP_CONN     = %c", lctrMsgDispTbl[LCTR_DISP_CONN]     ? 'Y' : 'N');
  LL_TRACE_INFO1("    LCTR_DISP_SCAN     = %c", lctrMsgDispTbl[LCTR_DISP_SCAN]     ? 'Y' : 'N');
  LL_TRACE_INFO1("    LCTR_DISP_INIT     = %c", lctrMsgDispTbl[LCTR_DISP_INIT]     ? 'Y' : 'N');
  LL_TRACE_INFO1("    LCTR_DISP_ADV      = %c", lctrMsgDispTbl[LCTR_DISP_ADV]      ? 'Y' : 'N');
}

/*************************************************************************************************/
/*!
 *  \brief  Link layer controller message dispatch handler.
 *
 *  \param  pMsg        WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LctrMsgDispatcher(lctrMsgHdr_t *pMsg)
{
  uint8_t dispId = pMsg->dispId;

  pLctrMsg = pMsg;

  if (dispId != LCTR_DISP_BCST)
  {
    /*** Unicast message ***/

    WSF_ASSERT(dispId < LCTR_DISP_TOTAL);

    if (lctrMsgDispTbl[dispId])
    {
      lctrMsgDispTbl[dispId](pMsg);
    }
  }
  else
  {
    /*** Broadcast message ***/

    if (pMsg->event == LCTR_MSG_RESET)
    {
      lctrResetEnabled = TRUE;
    }

    for (dispId = LCTR_DISP_FIRST_SM; dispId <= LCTR_DISP_LAST_SM; dispId++)
    {
      if (lctrMsgDispTbl[dispId])
      {
        lctrMsgDispTbl[dispId](pMsg);
      }
    }
  }

  if (lctrResetEnabled && (lmgrCb.resetDelayRefCnt == 0))
  {
    /* Reset common/shared subsystems. */
    BbBleWhiteListClear();
    BbBleResListClear();
    LmgrSetDefaults();
    #if (LL_ENABLE_TESTER)
      LlTesterInit();
    #endif

    for (dispId = LCTR_DISP_FIRST_SM; dispId <= LCTR_DISP_LAST_SM; dispId++)
    {
      if (lctrResetHdlrTbl[dispId])
      {
        lctrResetHdlrTbl[dispId]();
      }
    }

    lctrResetEnabled = FALSE;
    lctrNotifyHostResetCnf();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Link layer controller task event handler.
 *
 *  \param  event       Event id.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LctrEventHandler(uint8_t event)
{
  WSF_ASSERT(event < LCTR_EVENT_TOTAL);

  if (lctrEventHdlrTbl[event])
  {
    lctrEventHdlrTbl[event]();
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host HW error indication.
 *
 *  \param      code        Error code.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrNotifyHostHwErrInd(uint8_t code)
{
  LlHwErrorInd_t evt =
  {
    .hdr =
    {
      .param        = 0,
      .event        = LL_ERROR_IND,
      .status       = code
    },

    .code           = code
  };

  LL_TRACE_INFO1("### LlEvent ###  LL_ERROR_IND, status=%u", code);

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Compute the window widening delay in microseconds.
 *
 *  \param  unsyncTimeUsec  Unsynchronized time in microseconds.
 *  \param  caPpm           Total clock accuracy.
 *
 *  \return Window widening delay in microseconds.
 */
/*************************************************************************************************/
uint32_t lctrCalcWindowWideningUsec(uint32_t unsyncTimeUsec, uint32_t caPpm)
{
  if (lmgrGetOpFlag(LL_OP_MODE_FLAG_ENA_WW))
  {
    /* Largest unsynchronized time is 1,996 seconds (interval=4s and latency=499) and
     * largest total accuracy is 1000 ppm. */
    /* coverity[overflow_before_widen] */
    uint64_t wwDlyUsec = LL_MATH_DIV_10E6(((uint64_t)unsyncTimeUsec * caPpm) +
                                          999999);     /* round up */

    /* Reduce to 32-bits and always round up to a sleep clock tick. */
    return wwDlyUsec + pLctrRtCfg->ceJitterUsec + LL_WW_RX_DEVIATION_USEC;
  }
  else
  {
    return 0;
  }
}
