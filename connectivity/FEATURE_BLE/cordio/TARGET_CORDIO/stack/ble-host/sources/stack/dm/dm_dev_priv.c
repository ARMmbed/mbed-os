/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager device privacy module.
 *
 *  Copyright (c) 2009-2019 Arm Ltd. All Rights Reserved.
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

#include <string.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_timer.h"
#include "hci_api.h"
#include "dm_api.h"
#include "dm_dev.h"
#include "dm_main.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Structure for extended advertising */
typedef struct
{
  bool_t            configured;
  bool_t            connectable;
  bool_t            advertising;
} dmDevPrivExtAdv_t;

/* Control block for device privacy module */
typedef struct
{
  wsfTimer_t        addrTimer;
  bool_t            addrTimerStarted;
  uint8_t           prand[DM_PRIV_PRAND_LEN];
  uint16_t          changeInterval;
  bool_t            useResolvable;
  bool_t            addrInitialized;
  bdAddr_t          pendingAddr;
  bool_t            advertising;
  bool_t            scanning;
  bool_t            connecting;
  bool_t            connected;
  dmDevPrivExtAdv_t extAdv[DM_NUM_ADV_SETS];
} dmDevPrivCb_t;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* action function table */
static const dmDevPrivAct_t dmDevPrivAct[] =
{
  dmDevPrivActStart,
  dmDevPrivActStop,
  dmDevPrivActTimeout,
  dmDevPrivActAesCmpl,
  dmDevPrivActRpaStart,
  dmDevPrivActRpaStop,
  dmDevPrivActCtrl
};

/* Component function interface */
static const dmFcnIf_t dmDevPrivFcnIf =
{
  dmDevPrivReset,
  dmDevPrivHciHandler,
  dmDevPrivMsgHandler
};

/* control block */
static dmDevPrivCb_t dmDevPrivCb;

/*************************************************************************************************/
/*!
 *  \brief  Whether the legacy advertising or an advertising set using (non)connectable advertising
 *          is enabled.
 *
 *  \param  nonconnectable  Non-connectable advertising.
 *
 *  \return TRUE if advertising is enabled. FALSE, otherwise.
 */
/*************************************************************************************************/
static bool_t dmDevPrivAdvertising(bool_t nonconnectable)
{
  uint8_t i;

  /* if legacy advertising enabled */
  if (dmDevPrivCb.advertising)
  {
    return TRUE;
  }

  /* if (non)connectable extended advertising is enabled */
  for (i = 0; i < DM_NUM_ADV_SETS; i++)
  {
    if (dmDevPrivCb.extAdv[i].configured   &&
        dmDevPrivCb.extAdv[i].advertising  &&
        (dmDevPrivCb.extAdv[i].connectable || nonconnectable))
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Start the address generation timer.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmDevPrivTimerStart(void)
{
  /* if LL Privacy not enabled and device advertising, scanning or connected */
  if (!dmCb.llPrivEnabled && (dmDevPrivAdvertising(TRUE) || dmDevPrivCb.scanning || dmDevPrivCb.connected))
  {
    /* start address generation timer */
    dmDevPrivCb.addrTimerStarted = TRUE;
    dmDevPrivCb.addrTimer.msg.event = DM_DEV_PRIV_MSG_TIMEOUT;
    WsfTimerStartSec(&dmDevPrivCb.addrTimer, dmDevPrivCb.changeInterval);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Start resolvable address calculation.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmDevPrivAddrCalc(void)
{
  uint8_t buf[DM_PRIV_PLAINTEXT_LEN];

  /* get random number */
  SecRand(buf, DM_PRIV_PRAND_LEN);

  /* set address type in random number */
  buf[2] = (buf[2] & 0x3F) | DM_RAND_ADDR_RESOLV;

  /* pad buffer */
  memset(buf + DM_PRIV_PRAND_LEN, 0, (DM_PRIV_PLAINTEXT_LEN - DM_PRIV_PRAND_LEN));

  /* run calculation */
  SecAes(DmSecGetLocalIrk(), buf, dmCb.handlerId, 0, DM_DEV_PRIV_MSG_AES_CMPL);

  /* store random number */
  memcpy(dmDevPrivCb.prand, buf, DM_PRIV_PRAND_LEN);
}

/*************************************************************************************************/
/*!
 *  \brief  Set the RPA to be used by the local device.
 *
 *  \param  pAddr     New RPA.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmDevPrivSetRpa(uint8_t *pAddr)
{
  uint8_t i;

  /* set new RPA as local address */
  BdaCpy(dmCb.localAddr, pAddr);

  /* set RPA in device */
  HciLeSetRandAddrCmd(dmCb.localAddr);

  if (dmDevCb.advSetRandAddrCback)
  {
    for (i = 0; i < DM_NUM_ADV_SETS; i++)
    {
      if (dmDevPrivCb.extAdv[i].configured)
      {
        /* set RPA for advertising set */
        (*dmDevCb.advSetRandAddrCback)(i, dmCb.localAddr);
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the pending RPA to be used by the local device.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmDevPrivSetPendingRpa(void)
{
  /* if not advertising, scanning or initiating connection, and new RPA has been generated */
  if (!(dmDevPrivAdvertising(FALSE) || dmDevPrivCb.scanning || dmDevPrivCb.connecting) &&
      !BdaIsZeros(dmDevPrivCb.pendingAddr))
  {
    /* use pending RPA as local address */
    dmDevPrivSetRpa(dmDevPrivCb.pendingAddr);

    /* clear out pending RPA */
    memset(dmDevPrivCb.pendingAddr, 0, BDA_ADDR_LEN);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Start device privacy action function.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivActStart(dmDevPrivMsg_t *pMsg)
{
  /* initialize and store parameters */
  dmDevPrivCb.useResolvable = TRUE;
  dmDevPrivCb.changeInterval = pMsg->apiPrivStart.changeInterval;
  dmDevPrivCb.addrTimer.handlerId = dmCb.handlerId;

  /* set the local address type to random */
  DmAdvSetAddrType(DM_ADDR_RANDOM);
  DmScanSetAddrType(DM_ADDR_RANDOM);
  DmConnSetAddrType(DM_ADDR_RANDOM);

  /* start the address generation timer if applicable */
  if (dmDevPrivCb.changeInterval > 0)
  {
    /* start address generation timer */
    dmDevPrivTimerStart();

    /* if LL Privacy is supported */
    if (HciLlPrivacySupported())
    {
      /* Set LL resolvable private address timeout */
      DmPrivSetResolvablePrivateAddrTimeout(dmDevPrivCb.changeInterval);
    }
  }

  /* if private address has never been generated */
  if (dmDevPrivCb.addrInitialized == FALSE)
  {
    /* start address calculation */
    dmDevPrivAddrCalc();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Stop device privacy action function.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivActStop(dmDevPrivMsg_t *pMsg)
{
  /* stop address generation timer */
  dmDevPrivCb.addrTimerStarted = FALSE;
  WsfTimerStop(&dmDevPrivCb.addrTimer);

  dmDevPrivCb.useResolvable = FALSE;
  memset(dmDevPrivCb.pendingAddr, 0, BDA_ADDR_LEN);

  /* set the local address type to public */
  DmAdvSetAddrType(DM_ADDR_PUBLIC);
  DmScanSetAddrType(DM_ADDR_PUBLIC);
  DmConnSetAddrType(DM_ADDR_PUBLIC);

  /* if LL Privacy is supported */
  if (HciLlPrivacySupported())
  {
    /* remove all devices from resolving list and disable LL Privacy */
    DmPrivClearResList();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a private address generation timeout.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivActTimeout(dmDevPrivMsg_t *pMsg)
{
  /* if Host Privacy still enabled */
  if (dmDevPrivCb.useResolvable)
  {
    /* address generation timer has timed out */
    dmDevPrivCb.addrTimerStarted = FALSE;

    /* restart address generation timer */
    dmDevPrivTimerStart();

    /* start address calculation */
    dmDevPrivAddrCalc();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle AES calculation complete.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivActAesCmpl(dmDevPrivMsg_t *pMsg)
{
  bdAddr_t localAddr;

  if (dmDevPrivCb.useResolvable)
  {
    /* build and store address; hash is in ls bytes, random part in ms bytes */
    memcpy(localAddr, pMsg->aes.pCiphertext, DM_PRIV_HASH_LEN);
    memcpy(&localAddr[DM_PRIV_HASH_LEN], dmDevPrivCb.prand, DM_PRIV_PRAND_LEN);

    /* set generated address as random resolvable */
    DM_RAND_ADDR_SET(localAddr, DM_RAND_ADDR_RESOLV);

    /* if not advertising, scanning or initiating connection */
    if (!(dmDevPrivAdvertising(FALSE) || dmDevPrivCb.scanning || dmDevPrivCb.connecting))
    {
      /* use new RPA as local address */
      dmDevPrivSetRpa(localAddr);
    }
    else
    {
      /* save new RPA for later */
      BdaCpy(dmDevPrivCb.pendingAddr, localAddr);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a device privacy RPA start event.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivActRpaStart(dmDevPrivMsg_t *pMsg)
{
  switch (pMsg->hdr.param)
  {
    case DM_ADV_START_IND:
      /* advertising started */
      dmDevPrivCb.advertising = TRUE;
      break;

    case DM_ADV_SET_START_IND:
      /* extended advertising started */
      WSF_ASSERT(pMsg->privCtrl.advHandle < DM_NUM_ADV_SETS);

      dmDevPrivCb.extAdv[pMsg->privCtrl.advHandle].advertising = TRUE;
      break;

    case DM_SCAN_START_IND:
    case DM_EXT_SCAN_START_IND:
      /* scanning started */
      dmDevPrivCb.scanning = TRUE;
      break;

    case DM_CONN_OPEN_IND:
      /* connection opened */
      dmDevPrivCb.connected = TRUE;
      break;

    default:
      /* LL Privacy disabled */
      break;
  }

  /* if Host Privacy enabled and address generation timer not already running */
  if (dmDevPrivCb.useResolvable && !dmDevPrivCb.addrTimerStarted)
  {
    /* start address generation timer */
    dmDevPrivTimerStart();
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a device privacy RPA stop event.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivActRpaStop(dmDevPrivMsg_t *pMsg)
{
  switch (pMsg->hdr.param)
  {
    case DM_ADV_STOP_IND:
      /* advertising stopped */
      dmDevPrivCb.advertising = FALSE;

      /* use pending RPA */
      dmDevPrivSetPendingRpa();
      break;

    case DM_ADV_SET_STOP_IND:
      WSF_ASSERT(pMsg->privCtrl.advHandle < DM_NUM_ADV_SETS);

      /* extended advertising stopped */
      dmDevPrivCb.extAdv[pMsg->privCtrl.advHandle].advertising = FALSE;

      /* use pending RPA */
      dmDevPrivSetPendingRpa();
      break;

    case DM_SCAN_STOP_IND:
    case DM_EXT_SCAN_STOP_IND:
      /* scanning stopped */
      dmDevPrivCb.scanning = FALSE;

      /* use pending RPA */
      dmDevPrivSetPendingRpa();
      break;

    case DM_CONN_CLOSE_IND:
      /* connection closed */
      dmDevPrivCb.connected = FALSE;
      break;

    default:
      /* LL Privacy enabled see if address generation timer running */
      if (dmDevPrivCb.addrTimerStarted)
      {
        /* stop address generation timer (LL will generate RPAs) */
        dmDevPrivCb.addrTimerStarted = FALSE;
        WsfTimerStop(&dmDevPrivCb.addrTimer);
      }
      break;
  }

  /* let address generation timer timeout (if still running) and update address */
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a device privacy control event.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivActCtrl(dmDevPrivMsg_t *pMsg)
{
  switch (pMsg->hdr.param)
  {
    case DM_DEV_PRIV_MSG_CONN_INIT_START:
      /* connection initiation started */
      dmDevPrivCb.connecting = TRUE;
      break;

    case DM_DEV_PRIV_MSG_CONN_INIT_STOP:
      /* connection initiation stopped */
      dmDevPrivCb.connecting = FALSE;

      /* use pending RPA */
      dmDevPrivSetPendingRpa();
      break;

    case DM_DEV_PRIV_MSG_ADV_SET_ADD:
      /* advertising set created */
      WSF_ASSERT(pMsg->privCtrl.advHandle < DM_NUM_ADV_SETS);

      dmDevPrivCb.extAdv[pMsg->privCtrl.advHandle].configured = TRUE;
      dmDevPrivCb.extAdv[pMsg->privCtrl.advHandle].connectable = pMsg->privCtrl.connectable;

      /* if Host Privacy enabled and private address has been generated */
      if (dmDevPrivCb.useResolvable && dmDevPrivCb.addrInitialized)
      {
        WSF_ASSERT(dmDevCb.advSetRandAddrCback != NULL);

        /* set RPA for advertising set */
        (*dmDevCb.advSetRandAddrCback)(pMsg->privCtrl.advHandle, dmCb.localAddr);
      }
      break;

    case DM_DEV_PRIV_MSG_ADV_SET_REMOVE:
      /* advertising set removed */
      WSF_ASSERT(pMsg->privCtrl.advHandle < DM_NUM_ADV_SETS);

      /* clear advertising set */
      dmDevPrivCb.extAdv[pMsg->privCtrl.advHandle].configured = FALSE;
      dmDevPrivCb.extAdv[pMsg->privCtrl.advHandle].connectable = FALSE;
      dmDevPrivCb.extAdv[pMsg->privCtrl.advHandle].advertising = FALSE;
      break;

    case DM_DEV_PRIV_MSG_ADV_SETS_CLEAR:
      /* clear advertising sets */
      memset(dmDevPrivCb.extAdv, 0, sizeof(dmDevPrivCb.extAdv));
      break;

    default:
      /* unknown state message */
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM device priv HCI callback event handler.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivHciHandler(hciEvt_t *pEvent)
{
  /* if Host Privacy enabled */
  if (dmDevPrivCb.useResolvable)
  {
    /* handle incoming event */
    if (pEvent->hdr.event == HCI_LE_SET_RAND_ADDR_CMD_CMPL_CBACK_EVT)
    {
      dmAdvNewAddrIndEvt_t msg;

      msg.hdr.event = DM_ADV_NEW_ADDR_IND;
      msg.hdr.status = pEvent->hdr.status;
      BdaCpy(msg.addr, dmCb.localAddr);
      msg.firstTime = !dmDevPrivCb.addrInitialized;

      /* call callback */
      (*dmCb.cback)((dmEvt_t *) &msg);

      dmDevPrivCb.addrInitialized = TRUE;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  DM device privacy event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivMsgHandler(wsfMsgHdr_t *pMsg)
{
  /* execute action function */
  (*dmDevPrivAct[DM_MSG_MASK(pMsg->event)])((dmDevPrivMsg_t *) pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Reset the device privacy module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmDevPrivReset(void)
{
  if (dmDevPrivCb.useResolvable)
  {
    /* stop address generation timer */
    WsfTimerStop(&dmDevPrivCb.addrTimer);
  }

  /* initialize control block */
  memset(&dmDevPrivCb, 0, sizeof(dmDevPrivCb));
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize device privacy module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevPrivInit(void)
{
  WsfTaskLock();

  dmFcnIfTbl[DM_ID_DEV_PRIV] = (dmFcnIf_t *) &dmDevPrivFcnIf;

  /* initialize set advertising set random address callback */
  dmDevCb.advSetRandAddrCback = NULL;

  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Start using a private resolvable address.
 *
 *  \param  changeInterval  Interval between automatic address changes, in seconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevPrivStart(uint16_t changeInterval)
{
  dmDevPrivApiStart_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmDevPrivApiStart_t))) != NULL)
  {
    pMsg->hdr.event = DM_DEV_PRIV_MSG_API_START;
    pMsg->changeInterval = changeInterval;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Stop using a private resolvable address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmDevPrivStop(void)
{
  wsfMsgHdr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(wsfMsgHdr_t))) != NULL)
  {
    pMsg->event = DM_DEV_PRIV_MSG_API_STOP;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

