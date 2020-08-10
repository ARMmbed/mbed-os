/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager scan module.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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

#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_assert.h"
#include "dm_api.h"
#include "dm_scan.h"
#include "dm_conn.h"
#include "dm_dev.h"
#include "dm_main.h"

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* action function table */
static const dmScanAct_t dmScanAct[] =
{
  dmScanActStart,
  dmScanActStop,
  dmScanActTimeout
};

/* Component function interface */
static const dmFcnIf_t dmScanFcnIf =
{
  dmScanReset,
  dmScanHciHandler,
  dmScanMsgHandler
};

/*************************************************************************************************/
/*!
 *  \brief  Start scanning action function.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmScanActStart(dmScanMsg_t *pMsg)
{
  uint8_t phyIdx = DmScanPhyToIdx(HCI_SCAN_PHY_LE_1M_BIT);

  if (dmScanCb.scanState == DM_SCAN_STATE_IDLE)
  {
    /* set scan parameters */
    HciLeSetScanParamCmd(pMsg->apiStart.scanType[phyIdx], dmScanCb.scanInterval[phyIdx],
                         dmScanCb.scanWindow[phyIdx], DmLlAddrType(dmCb.scanAddrType),
                         dmCb.scanFiltPolicy);

    /* initialize scan result filtering */
    if (pMsg->apiStart.mode == DM_DISC_MODE_LIMITED)
    {
      dmScanCb.discFilter = DM_FLAG_LE_LIMITED_DISC;
    }
    else if (pMsg->apiStart.mode == DM_DISC_MODE_GENERAL)
    {
      dmScanCb.discFilter = DM_FLAG_LE_LIMITED_DISC | DM_FLAG_LE_GENERAL_DISC;
    }
    else
    {
      dmScanCb.discFilter = 0;
    }
    dmScanCb.filterNextScanRsp = FALSE;

    /* enable scan */
    dmScanCb.scanDuration = pMsg->apiStart.duration;
    dmScanCb.scanState = DM_SCAN_STATE_STARTING;
    HciLeSetScanEnableCmd(TRUE, pMsg->apiStart.filterDup);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Stop scanning action function.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmScanActStop(dmScanMsg_t *pMsg)
{
  if (dmScanCb.scanState == DM_SCAN_STATE_SCANNING)
  {
    /* disable scan */
    dmScanCb.scanState = DM_SCAN_STATE_STOPPING;
    HciLeSetScanEnableCmd(FALSE, 0);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an scan timeout.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmScanActTimeout(dmScanMsg_t *pMsg)
{
  dmScanActStop(pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an advertising report event from HCI.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmScanActHciReport(hciEvt_t *pEvent)
{
  uint8_t *p;
  bool_t  filtered = FALSE;

  /* ignore if not scanning */
  if (dmScanCb.scanState == DM_SCAN_STATE_SCANNING)
  {
    /* if filtering results for limited or general discovery */
    if (dmScanCb.discFilter != 0)
    {
      /* if this is a scan response */
      if (pEvent->leAdvReport.eventType == DM_RPT_SCAN_RESPONSE)
      {
        /* check if filtering next scan response */
        if (dmScanCb.filterNextScanRsp)
        {
          filtered = TRUE;
          dmScanCb.filterNextScanRsp = FALSE;
        }
      }
      /* else it's an advertising response */
      else
      {
        /* find flags in advertising data */
        p = DmFindAdType(DM_ADV_TYPE_FLAGS, pEvent->leAdvReport.len, pEvent->leAdvReport.pData);
        if (p == NULL)
        {
          /* flags not found */
          filtered = TRUE;
          dmScanCb.filterNextScanRsp = TRUE;
        }
        /* else flags found; check them */
        else if ((p[DM_AD_DATA_IDX] & dmScanCb.discFilter) == 0)
        {
          /* flags do not match discovery mode */
          filtered = TRUE;
          dmScanCb.filterNextScanRsp = TRUE;
        }
      }
    }

    if (!filtered)
    {
      pEvent->hdr.event = DM_SCAN_REPORT_IND;
      (*dmCb.cback)((dmEvt_t *) pEvent);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Reset the scan module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmScanReset(void)
{
  wsfMsgHdr_t scanStop;

  /* if stopping scan or scanning */
  if ((dmScanCb.scanState == DM_SCAN_STATE_STOPPING) ||
      (dmScanCb.scanState == DM_SCAN_STATE_SCANNING))
  {
    /* stop scan timer */
    WsfTimerStop(&dmScanCb.scanTimer);

    /* generate scan stop event */
    scanStop.event = DM_SCAN_STOP_IND;
    scanStop.status = HCI_SUCCESS;

    /* call callback */
    (*dmCb.cback)((dmEvt_t *) &scanStop);

    dmScanCb.scanState = DM_SCAN_STATE_IDLE;
  }

  /* reset scan module */
  dmScanInit();
}

/*************************************************************************************************/
/*!
 *  \brief  DM scan HCI event handler.
 *
 *  \param  pEvent    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmScanHciHandler(hciEvt_t *pEvent)
{
  if (pEvent->hdr.event == HCI_LE_ADV_REPORT_CBACK_EVT)
  {
    dmScanActHciReport(pEvent);
  }
  else if (pEvent->hdr.event == HCI_LE_SCAN_ENABLE_CMD_CMPL_CBACK_EVT)
  {
    switch (dmScanCb.scanState)
    {
    case DM_SCAN_STATE_STARTING:
      if (pEvent->hdr.status == HCI_SUCCESS)
      {
        /* start scan timer if applicable */
        if (dmScanCb.scanDuration > 0)
        {
          dmScanCb.scanTimer.msg.event = DM_SCAN_MSG_TIMEOUT;
          WsfTimerStartMs(&dmScanCb.scanTimer, dmScanCb.scanDuration);
        }

        dmScanCb.scanState = DM_SCAN_STATE_SCANNING;
      }
      else
      {
        dmScanCb.scanState = DM_SCAN_STATE_IDLE;
      }

      pEvent->hdr.event = DM_SCAN_START_IND;
      break;

    case DM_SCAN_STATE_STOPPING:
      if (pEvent->hdr.status == HCI_SUCCESS)
      {
        /* stop scan timer */
        WsfTimerStop(&dmScanCb.scanTimer);
        dmScanCb.scanState = DM_SCAN_STATE_IDLE;
      }
      else
      {
        dmScanCb.scanState = DM_SCAN_STATE_SCANNING;
      }

      pEvent->hdr.event = DM_SCAN_STOP_IND;
      break;

    default:

      /* Should never happen */
      WSF_ASSERT(0);
      break;
    }

    /* pass scanning start/stop event to dev priv */
    dmDevPassEvtToDevPriv((pEvent->hdr.event == DM_SCAN_START_IND) ? \
                           DM_DEV_PRIV_MSG_RPA_START : DM_DEV_PRIV_MSG_RPA_STOP, pEvent->hdr.event,
                           0, 0);

    /* call callback */
    (*dmCb.cback)((dmEvt_t *) pEvent);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM scan event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmScanMsgHandler(wsfMsgHdr_t *pMsg)
{
  /* execute action function */
  (*dmScanAct[DM_MSG_MASK(pMsg->event)])((dmScanMsg_t *)pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM legacy scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmScanInit(void)
{
  WsfTaskLock();

  /* set function interface table */
  dmFcnIfTbl[DM_ID_SCAN] = (dmFcnIf_t *) &dmScanFcnIf;

  /* initialize scan module */
  dmScanInit();

  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Whether DM scanning is in legacy mode.
 *
 *  \return TRUE if DM scanning is in legacy mode. FALSE, otherwise.
 */
/*************************************************************************************************/
bool_t DmScanModeLeg(void)
{
  return (dmFcnIfTbl[DM_ID_SCAN] == (dmFcnIf_t *) &dmScanFcnIf) ? TRUE : FALSE;
}
