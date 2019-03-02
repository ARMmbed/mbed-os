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
 *  \brief DM local device management module.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "dm_api.h"
#include "dm_dev.h"
#include "dm_main.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* action function table */
static const dmDevAct_t dmDevAct[] =
{
  dmDevActReset
};

/* Component function interface */
const dmFcnIf_t dmDevFcnIf =
{
  dmEmptyReset,
  dmDevHciHandler,
  dmDevMsgHandler
};

/* Control block */
dmDevCb_t dmDevCb;

/*************************************************************************************************/
/*!
 *  \brief  Reset action function.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
*/
/*************************************************************************************************/
void dmDevActReset(wsfMsgHdr_t *pMsg)
{
  uint8_t i;

  /* if DM not resetting */
  if (!dmCb.resetting)
  {
    /* set resetting state */
    dmCb.resetting = TRUE;

    /* for each DM component */
    for (i = 0; i < DM_NUM_IDS; i++)
    {
      /* call component's reset function */
      (*(dmFcnIfTbl[i]->reset))();
    }

    /* start HCI reset sequence */
    HciResetSequence();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a reset complete event from HCI.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmDevHciEvtReset(hciEvt_t *pEvent)
{
  /* reset resetting state */
  dmCb.resetting = FALSE;

  pEvent->hdr.event = DM_RESET_CMPL_IND;
  (*dmCb.cback)((dmEvt_t *) pEvent);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a vendor specific event from HCI.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmDevHciEvtVendorSpec(hciEvt_t *pEvent)
{
  pEvent->hdr.event = DM_VENDOR_SPEC_IND;
  (*dmCb.cback)((dmEvt_t *) pEvent);
}

/*************************************************************************************************/
/*!
 *  \brief  Handle an hardware error event from HCI.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmDevHciEvtHwError(hciEvt_t *pEvent)
{
  pEvent->hdr.event = DM_HW_ERROR_IND;
  (*dmCb.cback)((dmEvt_t *) pEvent);
}

/*************************************************************************************************/
/*!
 *  \brief  DM dev HCI event handler.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevHciHandler(hciEvt_t *pEvent)
{
  switch (pEvent->hdr.event)
  {
    case HCI_RESET_SEQ_CMPL_CBACK_EVT:
      dmDevHciEvtReset(pEvent);
      break;

    case HCI_VENDOR_SPEC_CBACK_EVT:
      dmDevHciEvtVendorSpec(pEvent);
      break;

    case HCI_HW_ERROR_CBACK_EVT:
      dmDevHciEvtHwError(pEvent);
      break;

    default:
      /* ignore event */
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM dev event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevMsgHandler(wsfMsgHdr_t *pMsg)
{
  /* execute action function */
  (*dmDevAct[DM_MSG_MASK(pMsg->event)])(pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Pass an event to the device privacy module.
 *
 *  \param  event        Device privacy event.
 *  \param  param        DM or Privacy event.
 *  \param  advHandle    Advertising handle.
 *  \param  connectable  TRUE if connectable extended advertising. FALSE, otherwise.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPassEvtToDevPriv(uint8_t event, uint8_t param, uint8_t advHandle, bool_t connectable)
{
  dmDevPrivMsg_t evt;

  DM_TRACE_INFO3("dmDevPassEvtToDevPriv: event: %d, param: %d, advHandle: %d", event, param, advHandle);

  /* build event */
  evt.hdr.event = event;
  evt.hdr.param = param;
  evt.privCtrl.advHandle = advHandle;
  evt.privCtrl.connectable = connectable;

  /* pass event to device privacy */
  (*(dmFcnIfTbl[DM_ID_DEV_PRIV]->msgHandler))((wsfMsgHdr_t *) &evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Pass a connection state change event to the Connection CTE module.
 *
 *  \param  state        Connection state.
 *  \param  connId       Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPassEvtToConnCte(uint8_t state, dmConnId_t connId)
{
  wsfMsgHdr_t evt;

  /* build event */
  evt.event = DM_CONN_CTE_MSG_STATE;
  evt.status = state;
  evt.param = connId;

  /* pass event to Connection CTE */
  (*(dmFcnIfTbl[DM_ID_CONN_CTE]->msgHandler))(&evt);
}

/*************************************************************************************************/
/*!
 *  \brief  Reset the device.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevReset(void)
{
  wsfMsgHdr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(wsfMsgHdr_t))) != NULL)
  {
    pMsg->event = DM_DEV_MSG_API_RESET;

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the random address to be used by the local device.
 *
 *  \param  pAddr     Random address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevSetRandAddr(uint8_t *pAddr)
{
  BdaCpy(dmCb.localAddr, pAddr);
  HciLeSetRandAddrCmd(pAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Add a peer device to the white list.  Note that this function cannot be called
 *          while advertising, scanning, or connecting with white list filtering active.
 *
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevWhiteListAdd(uint8_t addrType, uint8_t *pAddr)
{
  HciLeAddDevWhiteListCmd(addrType, pAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Remove a peer device from the white list.  Note that this function cannot be called
 *          while advertising, scanning, or connecting with white list filtering active.
 *
 *  \param  addrType  Address type.
 *  \param  pAddr     Peer device address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevWhiteListRemove(uint8_t addrType, uint8_t *pAddr)
{
  HciLeRemoveDevWhiteListCmd(addrType, pAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Clear the white list.  Note that this function cannot be called while
 *          advertising, scanning, or connecting with white list filtering active.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevWhiteListClear(void)
{
  HciLeClearWhiteListCmd();
}

/*************************************************************************************************/
/*!
 *  \brief  Set the Advertising, Scanning or Initiator filter policy.
 *
 *  \param  advHandle  Advertising handle (only applicable to advertising).
 *  \param  mode       Policy mode.
 *  \param  policy     Filter policy.
 *
 *  \return TRUE if the filter policy was successfully set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t dmDevSetFilterPolicy(uint8_t advHandle, uint8_t mode, uint8_t policy)
{
  bool_t policySet = FALSE;

  switch (mode)
  {
    case DM_FILT_POLICY_MODE_ADV:
      /* if Advertising filter policy is valid */
      if (policy <= HCI_ADV_FILT_ALL)
      {
        WSF_ASSERT(advHandle < DM_NUM_ADV_SETS);

        /* update the filter policy */
        dmCb.advFiltPolicy[advHandle] = policy;
        policySet = TRUE;
      }
      break;

    case DM_FILT_POLICY_MODE_SCAN:
      /* if Scanning filter policy is valid */
      if (policy <= HCI_FILT_WHITE_LIST_RES_INIT)
      {
        /* update the filter policy */
        dmCb.scanFiltPolicy = policy;
        policySet = TRUE;
      }
      break;

    case DM_FILT_POLICY_MODE_INIT:
      /* if Initiator filter policy is valid */
      if (policy <= HCI_FILT_WHITE_LIST)
      {
        /* update the filter policy */
        dmCb.initFiltPolicy = policy;
        policySet = TRUE;
      }
      break;

    case DM_FILT_POLICY_MODE_SYNC:
      /* if Synchronization filter policy is valid */
      if (policy <= HCI_FILT_PER_ADV_LIST)
      {
        /* clear the filter policy bit */
        dmCb.syncOptions &= ~HCI_OPTIONS_FILT_POLICY_BIT;

        /* set the filter policy bit */
        dmCb.syncOptions |= policy;
        policySet = TRUE;
      }
      break;

    default:
      /* invalid filter policy mode */
      break;
  }

  return policySet;
}

/*************************************************************************************************/
/*!
 *  \brief  Set the Advertising, Scanning or Initiator filter policy.
 *
 *  \param  mode     Policy mode.
 *  \param  policy   Filter policy.
 *
 *  \return TRUE if the filter policy was successfully set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t DmDevSetFilterPolicy(uint8_t mode, uint8_t policy)
{
  return dmDevSetFilterPolicy(DM_ADV_HANDLE_DEFAULT, mode, policy);
}

/*************************************************************************************************/
/*!
 *  \brief  Set the Advertising filter policy for the given advertising, Scanning or Initiator
 *          filter policy.
 *
 *  \param  advHandle  Advertising handle (only applicable to advertising).
 *  \param  mode       Policy mode.
 *  \param  policy     Filter policy.
 *
 *  \return TRUE if the filter policy was successfully set, FALSE otherwise.
*/
/*************************************************************************************************/
bool_t DmDevSetExtFilterPolicy(uint8_t advHandle, uint8_t mode, uint8_t policy)
{
  return dmDevSetFilterPolicy(advHandle, mode, policy);
}

/*************************************************************************************************/
/*!
 *  \brief  Vendor-specific controller initialization function.
 *
 *  \param  param    Vendor-specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevVsInit(uint8_t param)
{
  HciVsInit(param);
}
