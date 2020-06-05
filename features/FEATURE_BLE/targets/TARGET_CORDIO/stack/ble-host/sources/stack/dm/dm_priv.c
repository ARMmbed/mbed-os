/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Device manager privacy module.
 *
 *  Copyright (c) 2011-2019 Arm Ltd. All Rights Reserved.
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
#include "wsf_msg.h"
#include "sec_api.h"
#include "util/calc128.h"
#include "dm_api.h"
#include "dm_priv.h"
#include "dm_dev.h"
#include "dm_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* progress (dmPrivCb.inProgress) bitmask bits */
#define DM_PRIV_INPROGRESS_RES_ADDR              (1 << 0)  /* resolve address in progress */
#define DM_PRIV_INPROGRESS_GEN_ADDR              (1 << 1)  /* generate address in progress */

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Privacy action function table */
static const dmPrivAct_t dmPrivAct[] =
{
  dmPrivActResolveAddr,
  dmPrivActAddDevToResList,
  dmPrivActRemDevFromResList,
  dmPrivActClearResList,
  dmPrivActSetAddrResEnable,
  dmPrivActSetPrivacyMode,
  dmPrivActGenAddr
};

/* Privacy component function interface */
static const dmFcnIf_t dmPrivFcnIf =
{
  dmPrivReset,
  dmPrivHciHandler,
  dmPrivMsgHandler
};

/* Privacy AES action function table */
static const dmPrivAct_t dmPrivAesAct[] =
{
  dmPrivAesActResAddrAesCmpl,
  dmPrivAesActGenAddrAesCmpl
};

/* Privacy AES component function interface */
static const dmFcnIf_t dmPrivAesFcnIf =
{
  dmEmptyReset,
  (dmHciHandler_t) dmEmptyHandler,
  dmPrivAesMsgHandler
};

/* Control block */
dmPrivCb_t dmPrivCb;

/**************************************************************************************************
Local Functions
**************************************************************************************************/

static void dmPrivSetAddrResEnable(bool_t enable);

/*************************************************************************************************/
/*!
 *  \brief  Start address resolution procedure
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivActResolveAddr(dmPrivMsg_t *pMsg)
{
  uint8_t buf[DM_PRIV_PLAINTEXT_LEN];

  /* verify no resolution procedure currently in progress */
  if ((dmPrivCb.inProgress & DM_PRIV_INPROGRESS_RES_ADDR) == 0)
  {
    /* store hash */
    memcpy(dmPrivCb.hash, pMsg->apiResolveAddr.addr, DM_PRIV_HASH_LEN);

    /* copy random part of address with padding for address resolution calculation */
    memcpy(buf, &pMsg->apiResolveAddr.addr[3], DM_PRIV_PRAND_LEN);
    memset(buf + DM_PRIV_PRAND_LEN, 0, (DM_PRIV_PLAINTEXT_LEN - DM_PRIV_PRAND_LEN));

    /* set in progress */
    dmPrivCb.inProgress |= DM_PRIV_INPROGRESS_RES_ADDR;

    /* run calculation */
    SecAes(pMsg->apiResolveAddr.irk, buf, dmCb.handlerId,
           pMsg->hdr.param, DM_PRIV_MSG_RESOLVE_AES_CMPL);
  }
  else
  {
    /* call callback with error (note hdr.param is already set) */
    pMsg->hdr.status = HCI_ERR_MEMORY_EXCEEDED;
    pMsg->hdr.event = DM_PRIV_RESOLVED_ADDR_IND;
    (*dmCb.cback)((dmEvt_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Finish address resolution procedure upon completion of AES calculation.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivAesActResAddrAesCmpl(dmPrivMsg_t *pMsg)
{
  /* compare calculated value with hash */
  if (memcmp(dmPrivCb.hash, pMsg->aes.pCiphertext, DM_PRIV_HASH_LEN) == 0)
  {
    pMsg->hdr.status = HCI_SUCCESS;
  }
  else
  {
    pMsg->hdr.status = HCI_ERR_AUTH_FAILURE;
  }

  /* clear in progress */
  dmPrivCb.inProgress &= ~DM_PRIV_INPROGRESS_RES_ADDR;

  /* call client callback (note hdr.param is already set) */
  pMsg->hdr.event = DM_PRIV_RESOLVED_ADDR_IND;
  (*dmCb.cback)((dmEvt_t *) pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Add device to resolving list command.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivActAddDevToResList(dmPrivMsg_t *pMsg)
{
  dmPrivApiAddDevToResList_t *pDev = &pMsg->apiAddDevToResList;

  /* save whether asked to enable address resolution */
  dmPrivCb.enableLlPriv = pDev->enableLlPriv;

  /* save client-defined parameter for callback event */
  dmPrivCb.addDevToResListParam = pMsg->hdr.param;

  /* add device to resolving list */
  HciLeAddDeviceToResolvingListCmd(pDev->addrType, pDev->peerAddr, pDev->peerIrk, pDev->localIrk);
}

/*************************************************************************************************/
/*!
 *  \brief  Remove device from resolving list command.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivActRemDevFromResList(dmPrivMsg_t *pMsg)
{
  dmPrivApiRemDevFromResList_t *pDev = &pMsg->apiRemDevFromResList;

  /* save client-defined parameter for callback event */
  dmPrivCb.remDevFromResListParam = pMsg->hdr.param;

  /* remove device from resolving list */
  HciLeRemoveDeviceFromResolvingList(pDev->addrType, pDev->peerAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Clear resolving list command.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivActClearResList(dmPrivMsg_t *pMsg)
{
  /* clear resolving list */
  HciLeClearResolvingList();
}

/*************************************************************************************************/
/*!
 *  \brief  Set address resolution enable command.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivActSetAddrResEnable(dmPrivMsg_t *pMsg)
{
  dmPrivApiSetAddrResEnable_t *pAddrRes = &pMsg->apiSetAddrResEnable;

  /* enable or disable address resolution in LL */
  dmPrivSetAddrResEnable(pAddrRes->enable);
}

/*************************************************************************************************/
/*!
 *  \brief  Set privacy mode command.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivActSetPrivacyMode(dmPrivMsg_t *pMsg)
{
  dmPrivApiSetPrivacyMode_t *pPrivacyMode = &pMsg->apiSetPrivacyMode;

  /* set privacy mode */
  HciLeSetPrivacyModeCmd(pPrivacyMode->addrType, pPrivacyMode->peerAddr, pPrivacyMode->mode);
}

/*************************************************************************************************/
/*!
 *  \brief  Start address generation procedure.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivActGenAddr(dmPrivMsg_t *pMsg)
{
  if ((dmPrivCb.inProgress & DM_PRIV_INPROGRESS_GEN_ADDR) == 0)
  {
    /* get random number */
    SecRand(dmPrivCb.genAddrBuf, DM_PRIV_PRAND_LEN);

    /* set address type in random number */
    dmPrivCb.genAddrBuf[2] = (dmPrivCb.genAddrBuf[2] & 0x3F) | DM_RAND_ADDR_RESOLV;

    /* pad buffer */
    memset(dmPrivCb.genAddrBuf + DM_PRIV_PRAND_LEN, 0, (DM_PRIV_PLAINTEXT_LEN - DM_PRIV_PRAND_LEN));

    /* set in progress */
    dmPrivCb.inProgress |= DM_PRIV_INPROGRESS_GEN_ADDR;

    /* run calculation */
    SecAes(pMsg->apiGenerateAddr.irk, dmPrivCb.genAddrBuf, dmCb.handlerId,
           pMsg->hdr.param, DM_PRIV_MSG_GEN_ADDR_AES_CMPL);
  }
  else
  {
    /* call callback with error (note hdr.param is already set) */
    pMsg->hdr.status = HCI_ERR_MEMORY_EXCEEDED;
    pMsg->hdr.event = DM_PRIV_GENERATE_ADDR_IND;
    (*dmCb.cback)((dmEvt_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Finish generate RPA procedure upon completion of AES calculation.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivAesActGenAddrAesCmpl(dmPrivMsg_t *pMsg)
{
  dmPrivGenAddrIndEvt_t *pAddrEvt = (dmPrivGenAddrIndEvt_t*) pMsg;

  /* copy the hash and address to buffer */
  memcpy(pAddrEvt->addr, pMsg->aes.pCiphertext, DM_PRIV_HASH_LEN);
  memcpy(pAddrEvt->addr + DM_PRIV_HASH_LEN, dmPrivCb.genAddrBuf, DM_PRIV_PRAND_LEN);

  /* clear in progress */
  dmPrivCb.inProgress &= ~DM_PRIV_INPROGRESS_GEN_ADDR;

  /* call client callback */
  pAddrEvt->hdr.event = DM_PRIV_GENERATE_ADDR_IND;
  pMsg->hdr.status = HCI_SUCCESS;
  (*dmCb.cback)((dmEvt_t *) pAddrEvt);
}

/*************************************************************************************************/
/*!
 *  \brief  DM priv HCI callback event handler.
 *
 *  \param  pEvent  Pointer to HCI callback event structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivHciHandler(hciEvt_t *pEvent)
{
  /* handle incoming event */
  switch (pEvent->hdr.event)
  {
    case HCI_LE_ADD_DEV_TO_RES_LIST_CMD_CMPL_CBACK_EVT:
      pEvent->hdr.event = DM_PRIV_ADD_DEV_TO_RES_LIST_IND;
      pEvent->hdr.param = dmPrivCb.addDevToResListParam;

      /* if LE add device to resolving list command succeeded and been asked to enable address
       * resolution in LL and it's not enabled yet
       */
      if ((pEvent->hdr.status == HCI_SUCCESS) && dmPrivCb.enableLlPriv && !dmCb.llPrivEnabled)
      {
        /* enable address resolution in LL */
        dmPrivSetAddrResEnable(TRUE);
      }
      break;

    case HCI_LE_REM_DEV_FROM_RES_LIST_CMD_CMPL_CBACK_EVT:
      pEvent->hdr.event = DM_PRIV_REM_DEV_FROM_RES_LIST_IND;
      pEvent->hdr.param = dmPrivCb.remDevFromResListParam;
      break;

    case HCI_LE_CLEAR_RES_LIST_CMD_CMPL_CBACK_EVT:
      pEvent->hdr.event = DM_PRIV_CLEAR_RES_LIST_IND;

      /* if LE clear resolving list command succeeded and address resolution's enabled in LL */
      if ((pEvent->hdr.status == HCI_SUCCESS) && dmCb.llPrivEnabled)
      {
        /* disable address resolution in LL */
        dmPrivSetAddrResEnable(FALSE);
      }
      break;

    case HCI_LE_READ_PEER_RES_ADDR_CMD_CMPL_CBACK_EVT:
      pEvent->hdr.event = DM_PRIV_READ_PEER_RES_ADDR_IND;
      break;

    case HCI_LE_READ_LOCAL_RES_ADDR_CMD_CMPL_CBACK_EVT:
      pEvent->hdr.event = DM_PRIV_READ_LOCAL_RES_ADDR_IND;
      break;

    case HCI_LE_SET_ADDR_RES_ENABLE_CMD_CMPL_CBACK_EVT:
      pEvent->hdr.event = DM_PRIV_SET_ADDR_RES_ENABLE_IND;

      /* if LE set address resoultion enable command succeeded */
      if (pEvent->hdr.status == HCI_SUCCESS)
      {
        /* update LL Privacy Enabled flag */
        dmCb.llPrivEnabled = dmPrivCb.addrResEnable;

        /* pass LL Privacy enable/disable event to dev priv */
        dmDevPassEvtToDevPriv(dmCb.llPrivEnabled ? DM_DEV_PRIV_MSG_RPA_STOP : DM_DEV_PRIV_MSG_RPA_START,
                              dmCb.llPrivEnabled ? TRUE : FALSE, 0, 0);
      }
      break;

    default:
      /* should never get here */
      return;
  }

  /* call callback (note hdr.status is already set) */
  (*dmCb.cback)((dmEvt_t *)pEvent);
}

/*************************************************************************************************/
/*!
 *  \brief  Set address resolution enable command.
 *
 *  \param  enable   Set to TRUE to enable address resolution or FALSE to disable it.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void dmPrivSetAddrResEnable(bool_t enable)
{
  /* save input parameter */
  dmPrivCb.addrResEnable = enable;

  /* enable or disable address resolution in LL */
  HciLeSetAddrResolutionEnable(enable);
}

/*************************************************************************************************/
/*!
 *  \brief  DM priv event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivMsgHandler(wsfMsgHdr_t *pMsg)
{
  /* execute action function */
  (*dmPrivAct[DM_MSG_MASK(pMsg->event)])((dmPrivMsg_t *) pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Reset the privacy module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivReset(void)
{
  /* initialize control block */
  dmPrivCb.inProgress = 0;
  dmCb.llPrivEnabled = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  DM priv AES event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPrivAesMsgHandler(wsfMsgHdr_t *pMsg)
{
  /* execute action function */
  (*dmPrivAesAct[DM_MSG_MASK(pMsg->event)])((dmPrivMsg_t *) pMsg);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM privacy module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivInit(void)
{
  WsfTaskLock();

  dmFcnIfTbl[DM_ID_PRIV] = (dmFcnIf_t *) &dmPrivFcnIf;
  dmFcnIfTbl[DM_ID_PRIV_AES] = (dmFcnIf_t *) &dmPrivAesFcnIf;

  WsfTaskUnlock();
}

/*************************************************************************************************/
/*!
 *  \brief  Resolve a private resolvable address.  When complete the client's callback function
 *          is called with a DM_PRIV_RESOLVED_ADDR_IND event.  The client must wait to receive
 *          this event before executing this function again.
 *
 *  \param  pAddr     Peer device address.
 *  \param  pIrk      The peer's identity resolving key.
 *  \param  param     client-defined parameter returned with callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivResolveAddr(uint8_t *pAddr, uint8_t *pIrk, uint16_t param)
{
  dmPrivApiResolveAddr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmPrivApiResolveAddr_t))) != NULL)
  {
    pMsg->hdr.event = DM_PRIV_MSG_API_RESOLVE_ADDR;
    pMsg->hdr.param = param;
    Calc128Cpy(pMsg->irk, pIrk);
    BdaCpy(pMsg->addr, pAddr);
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Add device to resolving list.  When complete the client's callback function
 *          is called with a DM_PRIV_ADD_DEV_TO_RES_LIST_IND event.  The client must wait
 *          to receive this event before executing this function again.
 *
 *  \param  addrType      Peer identity address type.
 *  \param  pIdentityAddr Peer identity address.
 *  \param  pPeerIrk      The peer's identity resolving key.
 *  \param  pLocalIrk     The local identity resolving key.
 *  \param  enableLlPriv  Set to TRUE to enable address resolution in LL.
 *  \param  param         client-defined parameter returned with callback event.
 *
 *  \return None.
 *
 *  \Note   This command cannot be used when address resolution is enabled in the Controller and:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 *
 *  \Note   If the local or peer IRK associated with the peer Identity Address is all zeros then
 *          the Controller will use or accept the local or peer Identity Address respectively.
 *
 *  \Note   Parameter 'enableLlPriv' should be set to TRUE when the last device is being added
 *          to resolving list to enable address resolution in the Controller.
 */
/*************************************************************************************************/
void DmPrivAddDevToResList(uint8_t addrType, const uint8_t *pIdentityAddr, uint8_t *pPeerIrk,
                           uint8_t *pLocalIrk, bool_t enableLlPriv, uint16_t param)
{
  dmPrivApiAddDevToResList_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmPrivApiAddDevToResList_t))) != NULL)
  {
    pMsg->hdr.event = DM_PRIV_MSG_API_ADD_DEV_TO_RES_LIST;
    pMsg->hdr.param = param;
    pMsg->addrType = addrType;
    BdaCpy(pMsg->peerAddr, pIdentityAddr);
    Calc128Cpy(pMsg->peerIrk, pPeerIrk);
    Calc128Cpy(pMsg->localIrk, pLocalIrk);
    pMsg->enableLlPriv = enableLlPriv;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Remove device from resolving list.  When complete the client's callback function
 *          is called with a DM_PRIV_REM_DEV_FROM_RES_LIST_IND event.  The client must wait to
 *          receive this event before executing this function again.
 *
 *  \param  addrType      Peer identity address type.
 *  \param  pIdentityAddr Peer identity address.
 *  \param  param         client-defined parameter returned with callback event.
 *
 *  \return None.
 *
 *  \Note   This command cannot be used when address resolution is enabled in the Controller and:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 */
/*************************************************************************************************/
void DmPrivRemDevFromResList(uint8_t addrType, const uint8_t *pIdentityAddr, uint16_t param)
{
  dmPrivApiRemDevFromResList_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmPrivApiRemDevFromResList_t))) != NULL)
  {
    pMsg->hdr.event = DM_PRIV_MSG_API_REM_DEV_FROM_RES_LIST;
    pMsg->hdr.param = param;
    pMsg->addrType = addrType;
    BdaCpy(pMsg->peerAddr, pIdentityAddr);
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Clear resolving list.  When complete the client's callback function is called with a
 *          DM_PRIV_CLEAR_RES_LIST_IND event.  The client must wait to receive this event before
 *          executing this function again.
 *
 *  \return None.
 *
 *  \Note   This command cannot be used when address resolution is enabled in the Controller and:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 *
 *  \Note   Address resolution in the Controller will be disabled when resolving list's cleared
 *          successfully.
 */
/*************************************************************************************************/
void DmPrivClearResList(void)
{
  dmPrivMsg_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmPrivMsg_t))) != NULL)
  {
    pMsg->hdr.event = DM_PRIV_MSG_API_CLEAR_RES_LIST;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  HCI read peer resolvable address command.  When complete the client's callback
 *          function is called with a DM_PRIV_READ_PEER_RES_ADDR_IND event.  The client must
 *          wait to receive this event before executing this function again.
 *
 *  \param  addrType        Peer identity address type.
 *  \param  pIdentityAddr   Peer identity address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivReadPeerResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr)
{
  HciLeReadPeerResolvableAddr(addrType, pIdentityAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Read local resolvable address command.  When complete the client's callback
 *          function is called with a DM_PRIV_READ_LOCAL_RES_ADDR_IND event.  The client must
 *          wait to receive this event before executing this function again.
 *
 *  \param  addrType        Peer identity address type.
 *  \param  pIdentityAddr   Peer identity address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivReadLocalResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr)
{
  HciLeReadLocalResolvableAddr(addrType, pIdentityAddr);
}

/*************************************************************************************************/
/*!
 *  \brief  Enable or disable address resolution in LL.  When complete the client's callback
 *          function is called with a DM_PRIV_SET_ADDR_RES_ENABLE_IND event.  The client must
 *          wait to receive this event before executing this function again.
 *
 *  \param  enable   Set to TRUE to enable address resolution or FALSE to disable it.
 *
 *  \return None.
 *
 *  \Note   This command can be used at any time except when:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 */
/*************************************************************************************************/
void DmPrivSetAddrResEnable(bool_t enable)
{
  dmPrivApiSetAddrResEnable_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmPrivMsg_t))) != NULL)
  {
    pMsg->hdr.event = DM_PRIV_MSG_API_SET_ADDR_RES_ENABLE;
    pMsg->hdr.param = 0;
    pMsg->enable = enable;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set resolvable private address timeout command.
 *
 *  \param  rpaTimeout    Timeout measured in seconds.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivSetResolvablePrivateAddrTimeout(uint16_t rpaTimeout)
{
  HciLeSetResolvablePrivateAddrTimeout(rpaTimeout);
}

/*************************************************************************************************/
/*!
 *  \brief  Set privacy mode for a given entry in the resolving list.
 *
 *  \param  addrType      Peer identity address type.
 *  \param  pIdentityAddr Peer identity address.
 *  \param  mode          Privacy mode (by default, network privacy mode is used).
 *
 *  \return None.
 *
 *  \Note   This command can be used at any time except when:
 *          - Advertising (other than periodic advertising) is enabled,
 *          - Scanning is enabled, or
 *          - (Extended) Create connection or Create Sync command is outstanding.
 */
/*************************************************************************************************/
void DmPrivSetPrivacyMode(uint8_t addrType, const uint8_t *pIdentityAddr, uint8_t mode)
{
  dmPrivApiSetPrivacyMode_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmPrivApiSetPrivacyMode_t))) != NULL)
  {
    pMsg->hdr.event = DM_PRIV_MSG_API_SET_PRIVACY_MODE;
    pMsg->addrType = addrType;
    BdaCpy(pMsg->peerAddr, pIdentityAddr);
    pMsg->mode = mode;
    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Generate a Resolvable Private Address (RPA).
 *
 *  \param  pIrk      The identity resolving key.
 *  \param  param     Client-defined parameter returned with callback event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPrivGenerateAddr(uint8_t *pIrk, uint16_t param)
{
  dmPrivApiGenAddr_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(dmPrivApiGenAddr_t))) != NULL)
  {
    pMsg->hdr.event = DM_PRIV_MSG_API_GEN_ADDR;
    pMsg->hdr.param = param;
    Calc128Cpy(pMsg->irk, pIrk);

    WsfMsgSend(dmCb.handlerId, pMsg);
  }
}
