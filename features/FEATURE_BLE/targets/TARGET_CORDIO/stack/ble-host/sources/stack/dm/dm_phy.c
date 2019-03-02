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
 *  \brief DM PHY module.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "dm_api.h"
#include "dm_main.h"
#include "dm_conn.h"
#include "dm_phy.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* Component function interface */
static const dmFcnIf_t dmPhyFcnIf =
{
  dmEmptyReset,
  dmPhyHciHandler,
  (dmMsgHandler_t)dmEmptyHandler
};

/**************************************************************************************************
Local Functions
**************************************************************************************************/
static void dmPhyActDefPhySet(hciEvt_t *pEvent);
static void dmPhyActPhyRead(dmConnCcb_t *pCcb, hciEvt_t *pEvent);
static void dmPhyActPhyUpdate(dmConnCcb_t *pCcb, hciEvt_t *pEvent);

/*************************************************************************************************/
/*!
 *  \brief  DM PHY HCI event handler.
 *
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void dmPhyHciHandler(hciEvt_t *pEvent)
{
  dmConnCcb_t *pCcb;

  if (pEvent->hdr.event == HCI_LE_SET_DEF_PHY_CMD_CMPL_CBACK_EVT)
  {
    dmPhyActDefPhySet(pEvent);
  }
  /* look up ccb from conn handle */
  else if ((pCcb = dmConnCcbByHandle(pEvent->hdr.param)) != NULL)
  {
    /* handle incoming event */
    switch (pEvent->hdr.event)
    {
      case HCI_LE_READ_PHY_CMD_CMPL_CBACK_EVT:
        dmPhyActPhyRead(pCcb, pEvent);
        break;

      case HCI_LE_PHY_UPDATE_CMPL_CBACK_EVT:
        dmPhyActPhyUpdate(pCcb, pEvent);
        break;

      default:
        /* should never get here */
        break;
    }
  }
}

/*************************************************************************************************/
/*!
*  \brief  Handle a read PHY event from HCI.
*
*  \param  pCcb    Connection control block.
*  \param  pEvent  Pointer to HCI callback event structure.
*
*  \return None.
*/
/*************************************************************************************************/
static void dmPhyActPhyRead(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciLeReadPhyCmdCmplEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_PHY_READ_IND;
  evt.hdr.param = pCcb->connId;
  evt.status = evt.hdr.status = (uint8_t)pEvent->leReadPhyCmdCmpl.status;
  evt.handle = pCcb->handle;
  evt.txPhy = pEvent->leReadPhyCmdCmpl.txPhy;
  evt.rxPhy = pEvent->leReadPhyCmdCmpl.rxPhy;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
*  \brief  Handle a set default PHY event from HCI.
*
*  \param  pEvent  Pointer to HCI callback event structure.
*
*  \return None.
*/
/*************************************************************************************************/
static void dmPhyActDefPhySet(hciEvt_t *pEvent)
{
  hciLeSetDefPhyCmdCmplEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_PHY_SET_DEF_IND;
  evt.hdr.param = 0;
  evt.status = evt.hdr.status = (uint8_t)pEvent->leSetDefPhyCmdCmpl.status;
  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
*  \brief  Handle a PHY update event from HCI.
*
*  \param  pCcb    Connection control block.
*  \param  pEvent  Pointer to HCI callback event structure.
*
*  \return None.
*/
/*************************************************************************************************/
static void dmPhyActPhyUpdate(dmConnCcb_t *pCcb, hciEvt_t *pEvent)
{
  hciLePhyUpdateEvt_t evt;

  /* call callback */
  evt.hdr.event = DM_PHY_UPDATE_IND;
  evt.hdr.param = pCcb->connId;
  evt.status = evt.hdr.status = (uint8_t)pEvent->lePhyUpdate.status;
  evt.handle = pCcb->handle;
  evt.txPhy = pEvent->lePhyUpdate.txPhy;
  evt.rxPhy = pEvent->lePhyUpdate.rxPhy;

  (*dmConnCb.connCback[DM_CLIENT_ID_APP])((dmEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
*  \brief  Read the current transmitter PHY and receiver PHY for a given connection.
*
*  \param  connId      Connection identifier.
*
*  \return None.
*/
/*************************************************************************************************/
void DmReadPhy(dmConnId_t connId)
{
  dmConnCcb_t *pCcb;

  WsfTaskLock();
  /* look up ccb from conn id */
  pCcb = dmConnCcbById(connId);
  WsfTaskUnlock();

  /* if ccb found */
  if (pCcb != NULL)
  {
    HciLeReadPhyCmd(pCcb->handle);
  }
}

/*************************************************************************************************/
/*!
*  \brief  Set the preferred values for the transmitter PHY and receiver PHY for all subsequent
*          connections.
*
*  \param  allPhys     All PHYs preferences.
*  \param  txPhys      Preferred transmitter PHYs.
*  \param  rxPhys      Preferred receiver PHYs.
*
*  \return None.
*/
/*************************************************************************************************/
void DmSetDefaultPhy(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys)
{
  HciLeSetDefaultPhyCmd(allPhys, txPhys, rxPhys);
}

/*************************************************************************************************/
/*!
*  \brief  Set the PHY preferences for a given connection.
*
*  \param  connId      Connection identifier.
*  \param  allPhys     All PHYs preferences.
*  \param  txPhys      Preferred transmitter PHYs.
*  \param  rxPhys      Preferred receiver PHYs.
*  \param  phyOptions  PHY options.
*
*  \return None.
*/
/*************************************************************************************************/
void DmSetPhy(dmConnId_t connId, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
  dmConnCcb_t *pCcb;

  WsfTaskLock();
  /* look up ccb from conn id */
  pCcb = dmConnCcbById(connId);
  WsfTaskUnlock();

  /* if ccb found */
  if (pCcb != NULL)
  {
    HciLeSetPhyCmd(pCcb->handle, allPhys, txPhys, rxPhys, phyOptions);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize DM PHY.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DmPhyInit(void)
{
  dmFcnIfTbl[DM_ID_PHY] = (dmFcnIf_t *) &dmPhyFcnIf;
}
