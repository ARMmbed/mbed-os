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
 *  \brief Link layer (LL) PHY features control interface implementation file.
 */
/*************************************************************************************************/

#include "lmgr_api.h"
#include "lctr_api_phy.h"
#include "lmgr_api_conn.h"
#include "lctr_api_conn.h"
#include "wsf_msg.h"
#include "wsf_trace.h"

/*************************************************************************************************/
/*!
 *  \brief      Read current transmitter PHY and receive PHY.
 *
 *  \param      handle            Connection handle.
 *  \param      pTxPhy            Storage for transmitter PHY.
 *  \param      pRxPhy            Storage for receiver PHY.
 *
 *  \return     Status error code.
 *
 *  Read current transmitter PHY and receive PHY.
 */
/*************************************************************************************************/
uint8_t LlReadPhy(uint16_t handle, uint8_t *pTxPhy, uint8_t *pRxPhy)
{
  LL_TRACE_INFO1("### LlApi ###  LlReadPhy, handle=%u", handle);

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  *pTxPhy = LctrGetTxPhy(handle);
  *pRxPhy = LctrGetRxPhy(handle);
  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Validate PHY preferences.
 *
 *  \param      allPhys           All PHYs preferences.
 *  \param      txPhys            Preferred transmitter PHYs.
 *  \param      rxPhys            Preferred receiver PHYs.
 *  \param      phyOptions        PHY options.
 *
 *  \return     TRUE if PHY preferences are valid.
 */
/*************************************************************************************************/
static bool_t llValidatePhyPreferences(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
  const uint8_t allPhysMask = LL_ALL_PHY_TX_PREFERENCE_BIT | LL_ALL_PHY_RX_PREFERENCE_BIT;
  const uint8_t phyMask = LL_PHYS_LE_1M_BIT | LL_PHYS_LE_2M_BIT | LL_PHYS_LE_CODED_BIT;

  if (((allPhys & ~allPhysMask) != 0) ||                                      /* no unknown all PHYs preferences */
      ((txPhys & ~phyMask) != 0) ||                                           /* no unknown Tx PHYs */
      ((rxPhys & ~phyMask) != 0) ||                                           /* no unknown Rx PHYs */
     (((allPhys & LL_ALL_PHY_TX_PREFERENCE_BIT) == 0) && (txPhys == 0)) ||    /* at least one Tx PHY if preference */
     (((allPhys & LL_ALL_PHY_RX_PREFERENCE_BIT) == 0) && (rxPhys == 0)) ||    /* at least one Rx PHY if preference */
       (phyOptions > LL_PHY_OPTIONS_S8_PREFERRED))                            /* no unknown PHY options */
  {
    return FALSE;
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Determine supported PHYs.
 *
 *  \return     Supported PHYs.
 */
/*************************************************************************************************/
static uint8_t lctrSuppPhys(void)
{
  uint8_t phys = LL_PHYS_LE_1M_BIT;
  if ((lmgrCb.features & LL_FEAT_LE_2M_PHY) != 0)
  {
    phys |= LL_PHYS_LE_2M_BIT;
  }
  if ((lmgrCb.features & LL_FEAT_LE_CODED_PHY) != 0)
  {
    phys |= LL_PHYS_LE_CODED_BIT;
  }
  return phys;
}

/*************************************************************************************************/
/*!
 *  \brief      Set default PHYs.
 *
 *  \param      allPhys           All PHYs preferences.
 *  \param      txPhys            Preferred transmitter PHYs.
 *  \param      rxPhys            Preferred receiver PHYs.
 *
 *  \return     Status error code.
 *
 *  Specify the preferred values for the transmitter PHY and receiver PHY to be used for all
 *  subsequent connections over the LE transport.
 */
/*************************************************************************************************/
uint8_t LlSetDefaultPhy(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys)
{
  LL_TRACE_INFO0("### LlApi ###  LlSetDefaultPhy");

  /* Ignore unsupported PHYs. */
  txPhys &= lctrSuppPhys();
  rxPhys &= lctrSuppPhys();

  if ((LL_API_PARAM_CHECK == 1) &&
     !llValidatePhyPreferences(allPhys, txPhys, rxPhys, LL_PHY_OPTIONS_NONE))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  /* Discard Tx or Rx PHYs value without preference. */
  if ((allPhys & LL_ALL_PHY_TX_PREFERENCE_BIT) != 0)
  {
    txPhys = LL_PHYS_NONE;
  }
  if ((allPhys & LL_ALL_PHY_RX_PREFERENCE_BIT) != 0)
  {
    rxPhys = LL_PHYS_NONE;
  }

  lmgrConnCb.allPhys = allPhys;
  lmgrConnCb.txPhys  = txPhys;
  lmgrConnCb.rxPhys  = rxPhys;

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set PHY for a connection.
 *
 *  \param      handle            Connection handle.
 *  \param      allPhys           All PHYs preferences.
 *  \param      txPhys            Preferred transmitter PHYs.
 *  \param      rxPhys            Preferred receiver PHYs.
 *  \param      phyOptions        PHY options.
 *
 *  \return     Status error code.
 *
 *  Set PHY preferences for a given connection.  The controller might not be able to make the
 *  change (e.g., because the peer does not support the requested PHY) or may decide that the
 *  current PHY is preferable.
 */
/*************************************************************************************************/
uint8_t LlSetPhy(uint16_t handle, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
  lctrPhyUpdate_t *pMsg;

  LL_TRACE_INFO1("### LlApi ###  LlSetPhy, handle=%u", handle);

  if (LctrIsProcActPended(handle, LCTR_CONN_MSG_API_PHY_UPDATE) == TRUE)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  /* Ignore unsupported PHYs. */
  txPhys &= lctrSuppPhys();
  rxPhys &= lctrSuppPhys();

  if ((LL_API_PARAM_CHECK == 1) &&
     !llValidatePhyPreferences(allPhys, txPhys, rxPhys, phyOptions))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  if ((LL_API_PARAM_CHECK == 1) &&
       ((handle >= pLctrRtCfg->maxConn) ||
       !LctrIsConnHandleEnabled(handle)))
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }

  /* Discard Tx or Rx PHYs value without preference. */
  if ((allPhys & LL_ALL_PHY_TX_PREFERENCE_BIT) != 0)
  {
    txPhys = LL_PHYS_NONE;
  }
  if ((allPhys & LL_ALL_PHY_RX_PREFERENCE_BIT) != 0)
  {
    rxPhys = LL_PHYS_NONE;
  }

  if ((pMsg = (lctrPhyUpdate_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.handle = handle;
    pMsg->hdr.dispId = LCTR_DISP_CONN;
    pMsg->hdr.event  = LCTR_CONN_MSG_API_PHY_UPDATE;
    pMsg->allPhys    = allPhys;
    pMsg->txPhys     = txPhys;
    pMsg->rxPhys     = rxPhys;
    pMsg->phyOptions = phyOptions;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  return LL_SUCCESS;
}
