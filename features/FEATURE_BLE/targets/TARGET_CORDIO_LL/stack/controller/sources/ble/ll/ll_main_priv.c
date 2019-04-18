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
 * \brief Link layer (LL) privacy control interface implementation file.
 */
/*************************************************************************************************/

#include "bb_ble_api_reslist.h"
#include "lmgr_api.h"
#include "lmgr_api_priv.h"
#include "lctr_api_priv.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief      Add device to resolving list.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      pPeerIdentityAddr   Peer identity address.
 *  \param      pPeerIrk            Peer IRK.
 *  \param      pLocalIrk           Local IRK.
 *
 *  \return     Status error code.
 *
 *  Add device to resolving list.
 */
/*************************************************************************************************/
uint8_t LlAddDeviceToResolvingList(uint8_t peerAddrType, const uint8_t *pPeerIdentityAddr, const uint8_t *pPeerIrk, const uint8_t *pLocalIrk)
{
  LL_TRACE_INFO0("### LlApi ###  LlAddDeviceToResolvingList");

  if ((LL_API_PARAM_CHECK == 1) &&
      (peerAddrType > LL_ADDR_RANDOM))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }
  if ((lmgrCb.advEnabled || lmgrCb.numExtAdvEnabled || lmgrCb.numScanEnabled || lmgrCb.numInitEnabled) && lmgrCb.addrResEna)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }
  uint64_t peerIdentityAddr = BstreamToBda64(pPeerIdentityAddr);
  if (BbBleResListAdd(peerAddrType, peerIdentityAddr, pPeerIrk, pLocalIrk))
  {
    return LL_SUCCESS;
  }
  else
  {
    return LL_ERROR_CODE_MEM_CAP_EXCEEDED;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Remove device from resolving list.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      pPeerIdentityAddr   Peer identity address.
 *
 *  \return     Status error code.
 *
 *  Remove device from resolving list.
 */
/*************************************************************************************************/
uint8_t LlRemoveDeviceFromResolvingList(uint8_t peerAddrType, const uint8_t *pPeerIdentityAddr)
{
  LL_TRACE_INFO0("### LlApi ###  LlRemoveDeviceFromResolvingList");

  if ((LL_API_PARAM_CHECK == 1) &&
      (peerAddrType > LL_ADDR_RANDOM))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }
  if ((lmgrCb.advEnabled || lmgrCb.numExtAdvEnabled || lmgrCb.numScanEnabled || lmgrCb.numInitEnabled) && lmgrCb.addrResEna)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }
  uint64_t peerIdentityAddr = BstreamToBda64(pPeerIdentityAddr);
  if (BbBleResListRemove(peerAddrType, peerIdentityAddr))
  {
    return LL_SUCCESS;
  }
  else
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Clear resolving list.
 *
 *  \return     Status error code.
 *
 *  Clear resolving list.
 */
/*************************************************************************************************/
uint8_t LlClearResolvingList(void)
{
  LL_TRACE_INFO0("### LlApi ###  LlClearResolvingList");

  if ((lmgrCb.advEnabled || lmgrCb.numExtAdvEnabled || lmgrCb.numScanEnabled || lmgrCb.numInitEnabled) && lmgrCb.addrResEna)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  BbBleResListClear();

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Read resolving list size.
 *
 *  \param      pSize             Storage for resolving list size.
 *
 *  \return     Status error code.
 *
 *  Read number of address translation entries that can be stored in the resolving list.
 */
/*************************************************************************************************/
uint8_t LlReadResolvingListSize(uint8_t *pSize)
{
  LL_TRACE_INFO0("### LlApi ###  LlReadResolvingListSize");

  *pSize = BbBleResListGetSize();

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Read peer resolvable address.
 *
 *  \param      addrType        Peer identity address type.
 *  \param      pIdentityAddr   Peer identity address.
 *  \param      pRpa            Storage for peer resolvable private address
 *
 *  \return     Status error code.
 *
 *  Get the peer resolvable private address that is currently being used for the peer identity
 *  address.
 */
/*************************************************************************************************/
uint8_t LlReadPeerResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr, uint8_t *pRpa)
{
  LL_TRACE_INFO0("### LlApi ###  LlReadPeerResolvableAddr");

  if ((LL_API_PARAM_CHECK == 1) &&
      (addrType > LL_ADDR_RANDOM))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }
  uint64_t identityAddr = BstreamToBda64(pIdentityAddr);
  uint64_t rpa = 0;
  if (BbBleResListReadPeer(addrType, identityAddr, &rpa))
  {
    Bda64ToBstream(pRpa, rpa);
    return LL_SUCCESS;
  }
  else
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Read local resolvable address.
 *
 *  \param      addrType        Peer identity address type.
 *  \param      pIdentityAddr   Peer identity address.
 *  \param      pRpa            Storage for peer resolvable private address
 *
 *  \return     Status error code.
 *
 *  Get the local resolvable private address that is currently being used for the peer identity
 *  address.
 */
/*************************************************************************************************/
uint8_t LlReadLocalResolvableAddr(uint8_t addrType, const uint8_t *pIdentityAddr, uint8_t *pRpa)
{
  LL_TRACE_INFO0("### LlApi ###  LlReadLocalResolvableAddr");

  if ((LL_API_PARAM_CHECK == 1) &&
      (addrType > LL_ADDR_RANDOM))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }
  uint64_t identityAddr = BstreamToBda64(pIdentityAddr);
  uint64_t rpa = 0;
  if (BbBleResListReadLocal(addrType, identityAddr, &rpa))
  {
    Bda64ToBstream(pRpa, rpa);
    return LL_SUCCESS;
  }
  else
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Enable or disable address resolution.
 *
 *  \param      enable      Set to TRUE to enable address resolution or FALSE to disable address
 *                          resolution.
 *
 *  \return     Status error code.
 *
 *  Enable or disable address resolution so that received local or peer resolvable private
 *  addresses are resolved.
 */
/*************************************************************************************************/
uint8_t LlSetAddrResolutionEnable(uint8_t enable)
{
  LL_TRACE_INFO0("### LlApi ###  LlSetAddrResolutionEnable");

  if ((LL_API_PARAM_CHECK == 1) &&
      (enable > 1))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }
  if (lmgrCb.advEnabled || lmgrCb.numExtAdvEnabled || lmgrCb.numScanEnabled || lmgrCb.numInitEnabled)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  lmgrCb.addrResEna = enable;

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set resolvable private address timeout.
 *
 *  \param      rpaTimeout    Timeout measured in seconds.
 *
 *  \return     Status error code.
 *
 *  Set the time period between automatic generation of new resolvable private addresses.
 */
/*************************************************************************************************/
uint8_t LlSetResolvablePrivateAddrTimeout(uint16_t rpaTimeout)
{
  LL_TRACE_INFO0("### LlApi ###  LlSetResolvablePrivateAddrTimeout");

  const uint16_t rpaTimeoutMin = 1;
  const uint16_t rpaTimeoutMax = 0xA1B8;

  if ((LL_API_PARAM_CHECK == 1) &&
      ((rpaTimeout < rpaTimeoutMin) || (rpaTimeout > rpaTimeoutMax)))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  LctrPrivSetResPrivAddrTimeout(rpaTimeout);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Set privacy mode.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      pPeerIdentityAddr   Peer identity address.
 *  \param      privMode            Privacy mode.
 *
 *  \return     Status error code.
 *
 *  Allow the host to specify the privacy mode to be used for a given entry on the resolving list.
 */
/*************************************************************************************************/
uint8_t LlSetPrivacyMode(uint8_t peerAddrType, const uint8_t *pPeerIdentityAddr, uint8_t privMode)
{
  LL_TRACE_INFO0("### LlApi ###  LlSetPrivacyMode");

  if ((LL_API_PARAM_CHECK == 1) &&
      ((peerAddrType > LL_ADDR_RANDOM) || (privMode > LL_PRIV_MODE_DEVICE)))
  {
    return LL_ERROR_CODE_INVALID_HCI_CMD_PARAMS;
  }

  if ((lmgrCb.advEnabled || lmgrCb.numExtAdvEnabled || lmgrCb.numScanEnabled || lmgrCb.numInitEnabled) && lmgrCb.addrResEna)
  {
    return LL_ERROR_CODE_CMD_DISALLOWED;
  }

  uint64_t peerIdentityAddr = BstreamToBda64(pPeerIdentityAddr);
  if (BbBleResListSetPrivacyMode(peerAddrType, peerIdentityAddr, privMode))
  {
    return LL_SUCCESS;
  }
  else
  {
    return LL_ERROR_CODE_UNKNOWN_CONN_ID;
  }
}
