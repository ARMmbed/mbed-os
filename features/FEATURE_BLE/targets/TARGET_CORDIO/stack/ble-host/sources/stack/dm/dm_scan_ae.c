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
 *  \brief Device manager extended scan module.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "dm_api.h"
#include "dm_scan.h"
#include "dm_conn.h"
#include "dm_dev.h"
#include "dm_main.h"

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* extended scan action function table */
static const dmScanAct_t dmScanAct[] =
{
  dmExtScanActStart,
  dmExtScanActStop,
  dmExtScanActTimeout
};

/* extended scan component function interface */
static const dmFcnIf_t dmScanFcnIf =
{
  dmExtScanReset,
  dmExtScanHciHandler,
  dmExtScanMsgHandler
};

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* event being reported used for a legacy advertising PDU or a scan response */
#define DM_ADV_RPT_SCAN_RSP(evtType)    ((((evtType) == HCI_ADV_RPT_LEG_CONN_UNDIRECT_SCAN_RSP)  || \
                                          ((evtType) == HCI_ADV_RPT_LEG_SCAN_UNDIRECT_SCAN_RSP)) || \
                                         ((((evtType) & HCI_ADV_RPT_LEG_ADV_BIT) == 0)           && \
                                          ((evtType)  & HCI_ADV_RPT_SCAN_RSP_BIT)))

/* data status field of extended advertising report event type */
#define DM_ADV_RPT_DATA_STATUS(evtType) (((evtType) & HCI_ADV_RPT_DATA_STATUS_BITS) >> 5)

/*************************************************************************************************/
/*!
 *  \brief  Start extended scanning action function.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmExtScanActStart(dmScanMsg_t *pMsg)
{
  uint8_t i;
  uint8_t idx;
  uint8_t phyIdx;
  hciExtScanParam_t scanParam[DM_NUM_PHYS];

  if (dmScanCb.scanState == DM_SCAN_STATE_IDLE)
  {
    /* see advertising packets to be received on which PHY */
    for (i = 0, idx = 0; (i < 8) && (idx < DM_NUM_PHYS); i++)
    {
      if (pMsg->apiStart.scanPhys & (1 << i))
      {
        phyIdx = DmScanPhyToIdx(1 << i);

        /* set extended scan parameters for this PHY */
        scanParam[idx].scanType = pMsg->apiStart.scanType[idx];
        scanParam[idx].scanInterval = dmScanCb.scanInterval[phyIdx];
        scanParam[idx].scanWindow = dmScanCb.scanWindow[phyIdx];
        idx++;
      }
    }

    /* set extended scan parameters to be used on primary advertising channel */
    HciLeSetExtScanParamCmd(DmLlAddrType(dmCb.scanAddrType), dmCb.scanFiltPolicy,
                            pMsg->apiStart.scanPhys, scanParam);

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
    dmScanCb.scanState = DM_SCAN_STATE_STARTING;
    HciLeExtScanEnableCmd(TRUE, pMsg->apiStart.filterDup, (pMsg->apiStart.duration / 10), pMsg->apiStart.period);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Stop extended scanning action function.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmExtScanActStop(dmScanMsg_t *pMsg)
{
  if (dmScanCb.scanState == DM_SCAN_STATE_SCANNING)
  {
    /* disable scan */
    dmScanCb.scanState = DM_SCAN_STATE_STOPPING;
    HciLeExtScanEnableCmd(FALSE, 0, 0, 0);
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
void dmExtScanActTimeout(dmScanMsg_t *pMsg)
{
  /* empty */
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an extended advertising report event from HCI.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmExtScanActHciReport(hciEvt_t *pEvent)
{
  uint8_t       *p;
  static bool_t filtered = FALSE;
  static bool_t firstFrag = TRUE;

  /* ignore if not scanning */
  if (dmScanCb.scanState == DM_SCAN_STATE_SCANNING)
  {
    /* if filtering results for limited or general discovery, and first fragment */
    if ((dmScanCb.discFilter != 0) && firstFrag)
    {
      /* if this is a scan response */
      if (DM_ADV_RPT_SCAN_RSP(pEvent->leExtAdvReport.eventType))
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
        p = DmFindAdType(DM_ADV_TYPE_FLAGS, pEvent->leExtAdvReport.len, pEvent->leExtAdvReport.pData);
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

      firstFrag = FALSE;
    }

    if (!filtered)
    {
      pEvent->hdr.event = DM_EXT_SCAN_REPORT_IND;
      (*dmCb.cback)((dmEvt_t *) pEvent);
    }

    /* if filtering results for limited or general discovery, and no more fragmented data to come */
    if ((dmScanCb.discFilter != 0) &&
        (DM_ADV_RPT_DATA_STATUS(pEvent->leExtAdvReport.eventType) != HCI_ADV_RPT_DATA_INCMPL_MORE))
    {
      /* reset our flags */
      filtered = FALSE;
      firstFrag = TRUE;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM extended scan reset function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmExtScanReset(void)
{
  hciLeScanTimeoutEvt_t scanTimeout;

  /* if stopping scan or scanning */
  if ((dmScanCb.scanState == DM_SCAN_STATE_STOPPING) ||
      (dmScanCb.scanState == DM_SCAN_STATE_SCANNING))
  {
    /* generate HCI scan timeout event */
    scanTimeout.hdr.event = HCI_LE_SCAN_TIMEOUT_CBACK_EVT;
    scanTimeout.hdr.status = HCI_SUCCESS;

    /* handle the event */
    dmExtScanHciHandler((hciEvt_t *) &scanTimeout);
  }

  /* reset scan module */
  dmScanInit();
}

/*************************************************************************************************/
/*!
 *  \brief  DM extended scan HCI event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmExtScanHciHandler(hciEvt_t *pEvent)
{
  DM_TRACE_INFO2("dmExtScanHciHandler: event: %d state: %d", pEvent->hdr.event, dmScanCb.scanState);

  if (pEvent->hdr.event == HCI_LE_EXT_ADV_REPORT_CBACK_EVT)
  {
    dmExtScanActHciReport(pEvent);
  }
  else if (pEvent->hdr.event == HCI_LE_SCAN_TIMEOUT_CBACK_EVT)
  {
    dmScanCb.scanState = DM_SCAN_STATE_IDLE;

    /* pass scanning stop event to dev priv */
    dmDevPassEvtToDevPriv(DM_DEV_PRIV_MSG_RPA_STOP, DM_EXT_SCAN_STOP_IND, 0, 0);

    /* call callback */
    pEvent->hdr.event = DM_EXT_SCAN_STOP_IND;
    (*dmCb.cback)((dmEvt_t *) pEvent);
  }
  else if (pEvent->hdr.event == HCI_LE_EXT_SCAN_ENABLE_CMD_CMPL_CBACK_EVT)
  {
    switch (dmScanCb.scanState)
    {
    case DM_SCAN_STATE_STARTING:
      pEvent->hdr.event = DM_EXT_SCAN_START_IND;
      dmScanCb.scanState = pEvent->hdr.status == HCI_SUCCESS? DM_SCAN_STATE_SCANNING : DM_SCAN_STATE_IDLE;
      break;

    case DM_SCAN_STATE_STOPPING:
      pEvent->hdr.event = DM_EXT_SCAN_STOP_IND;
      dmScanCb.scanState = pEvent->hdr.status == HCI_SUCCESS? DM_SCAN_STATE_IDLE : DM_SCAN_STATE_SCANNING;
      break;

    default:
      /* Should never happen */
      WSF_ASSERT(0);
      break;
    }

    /* pass scanning start/stop event to dev priv */
    dmDevPassEvtToDevPriv((pEvent->hdr.event == DM_EXT_SCAN_START_IND) ? \
                           DM_DEV_PRIV_MSG_RPA_START : DM_DEV_PRIV_MSG_RPA_STOP, pEvent->hdr.event,
                           0, 0);

    /* call callback */
    (*dmCb.cback)((dmEvt_t *)pEvent);
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
void dmExtScanMsgHandler(wsfMsgHdr_t *pMsg)
{
  /* execute action function */
  (*dmScanAct[DM_MSG_MASK(pMsg->event)])((dmScanMsg_t *)pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM extended scanning.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmExtScanInit(void)
{
  WsfTaskLock();

  /* set function interface table */
  dmFcnIfTbl[DM_ID_SCAN] = (dmFcnIf_t *) &dmScanFcnIf;

  /* initialize scan/sync modules */
  dmScanInit();
  dmSyncInit();

  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Whether DM scanning is in extended mode.
 *
 *  \return TRUE if DM scanning is in extended mode. FALSE, otherwise.
 */
/*************************************************************************************************/
bool_t DmScanModeExt(void)
{
  return (dmFcnIfTbl[DM_ID_SCAN] == (dmFcnIf_t *) &dmScanFcnIf) ? TRUE : FALSE;
}
