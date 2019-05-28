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
 * \brief BLE baseband resolving list interface file.
 */
/*************************************************************************************************/

#ifndef BB_BLE_API_RESLIST_H
#define BB_BLE_API_RESLIST_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BB_API_BLE_RL
 *  \{ */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Resolvable address status. */
enum
{
  BB_BLE_RESLIST_STATUS_ID_ADDR_NOT_IN_LIST,  /*!< Identity address is not in list. */
  BB_BLE_RESLIST_STATUS_ZERO_IRK,             /*!< IRK is zero for address. */
  BB_BLE_RESLIST_STATUS_RES_ADDR_UNASSIGNED,  /*!< Resolvable address is unassigned. */
  BB_BLE_RESLIST_STATUS_RES_ADDR_ASSIGNED     /*!< Resolvable address is assigned. */
};

/*! \brief      Privacy modes. */
enum
{
  BB_BLE_RESLIST_PRIV_MODE_NETWORK = 0,       /*!< Network privacy mode. */
  BB_BLE_RESLIST_PRIV_MODE_DEVICE  = 1,       /*!< Device privacy mode. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Address resolution needed callback signature. */
typedef void (*bbBleResListAddrResNeeded_t)(uint64_t rpa, bool_t peer, uint8_t peerAddrType, uint64_t peerIdentityAddr);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Set address resolution needed callback.
 *
 *  \param      cback             Callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleResListSetAddrResNeededCback(bbBleResListAddrResNeeded_t cback);

/*************************************************************************************************/
/*!
 *  \brief      Get resolving list size.
 *
 *  \return     Total number of resolving list entries.
 *
 *  Get the resolving list capacity supported by the BB.
 */
/*************************************************************************************************/
uint8_t BbBleResListGetSize(void);

/*************************************************************************************************/
/*!
 *  \brief      Clear resolving list.
 *
 *  \return     None.
 *
 *  Clear all resolving list entries stored in the BB.
 */
/*************************************************************************************************/
void BbBleResListClear(void);

/*************************************************************************************************/
/*!
 *  \brief      Add device to resolving list.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pPeerIrk            Peer IRK.
 *  \param      pLocalIrk           Local IRK.
 *
 *  \return     TRUE if successful, FALSE if list full.
 *
 *  Add device to resolving list.
 */
/*************************************************************************************************/
bool_t BbBleResListAdd(uint8_t peerAddrType, uint64_t peerIdentityAddr, const uint8_t *pPeerIrk,
    const uint8_t *pLocalIrk);

/*************************************************************************************************/
/*!
 *  \brief      Remove device from resolving list.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     TRUE if successful, FALSE if address not in the list.
 *
 *  Remove device from resolving list.
 */
/*************************************************************************************************/
bool_t BbBleResListRemove(uint8_t peerAddrType, uint64_t peerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Set privacy mode of a device.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      privMode            Privacy mode.
 *
 *  \return     TRUE if successful, FALSE if address not in the list.
 *
 *  Set privacy mode of a device.
 */
/*************************************************************************************************/
bool_t BbBleResListSetPrivacyMode(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint8_t privMode);

/*************************************************************************************************/
/*!
 *  \brief      Get privacy mode of a device.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pPrivMode           Storage for privacy mode.
 *
 *  \return     TRUE if successful, FALSE if address not in the list.
 *
 *  Get privacy mode of a device.
 */
/*************************************************************************************************/
bool_t BbBleResListGetPrivacyMode(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint8_t *pPrivMode);

/*************************************************************************************************/
/*!
 *  \brief      Read peer resolvable address.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pRpa                Storage for peer resolvable private address
 *
 *  \return     TRUE if successful, FALSE if address not in list.
 *
 *  Get the peer resolvable private address that is currently being used for the peer identity
 *  address.
 */
/*************************************************************************************************/
bool_t BbBleResListReadPeer(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa);

/*************************************************************************************************/
/*!
 *  \brief      Read local resolvable address.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pRpa                Storage for peer resolvable private address
 *
 *  \return     TRUE if successful, FALSE if address not in list.
 *
 *  Get the local resolvable private address that is currently being used for the peer identity
 *  address.
 */
/*************************************************************************************************/
bool_t BbBleResListReadLocal(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa);

/*************************************************************************************************/
/*!
 *  \brief      Update local resolvable address.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pRpa                Storage for local resolvable private address
 *
 *  \return     TRUE if successful, FALSE if address not in list.
 *
 *  Update the local resolvable private address that is currently being used for the peer identity
 *  address.
 */
/*************************************************************************************************/
bool_t BbBleResListUpdateLocal(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa);

/*************************************************************************************************/
/*!
 *  \brief      Generate peer resolvable address.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pRpa                Storage for peer resolvable private address.
 *
 *  \return     TRUE if successful, FALSE if address not in list or peer IRK is zero.
 *
 *  Generate a peer resolvable address for a peer.
 */
/*************************************************************************************************/
bool_t BbBleResListGeneratePeer(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa);

/*************************************************************************************************/
/*!
 *  \brief      Check whether a peer address has been resolved.
 *
 *  \param      rpa                 Peer resolvable private address
 *  \param      pPeerAddrType       Storage for peer identity address type.
 *  \param      pPeerIdentityAddr   Storage for peer identity address.
 *
 *  \return     TRUE if successful, FALSE if address has not been resolved.
 *
 *  Check whether a peer address has been resolved.  Because of timing constraints, addresses should
 *  not be resolved in this function; instead, the resolution callback should be invoked to schedule
 *  the operation later.
 */
/*************************************************************************************************/
bool_t BbBleResListCheckResolvePeer(uint64_t rpa, uint8_t *pPeerAddrType, uint64_t *pPeerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Check whether a local address has been resolved.
 *
 *  \param      rpa                 Local resolvable private address
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     TRUE if successful, FALSE if address has not been resolved.
 *
 *  Check whether a local address has been resolved.  Because of timing constraints, addresses should
 *  not be resolved in this function; instead, the resolution callback should be invoked to schedule
 *  the operation later.
 */
/*************************************************************************************************/
bool_t BbBleResListCheckResolveLocal(uint64_t rpa, uint8_t peerAddrType, uint64_t peerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Check whether a local address has been resolved.
 *
 *  \param      rpa                 Local resolvable private address
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     TRUE if successful, FALSE if address has not been resolved.
 *
 */
/*************************************************************************************************/
bool_t BbBleResListIsLocalResolved(uint64_t rpa, uint8_t peerAddrType, uint64_t peerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Resolve peer resolvable address.
 *
 *  \param      rpa                 Peer resolvable private address
 *  \param      pPeerAddrType       Storage for peer identity address type.
 *  \param      pPeerIdentityAddr   Storage for peer identity address.
 *
 *  \return     TRUE if successful, FALSE if address could not be resolved.
 *
 *  Resolve a peer resolvable address.
 */
/*************************************************************************************************/
bool_t BbBleResListResolvePeer(uint64_t rpa, uint8_t *pPeerAddrType, uint64_t *pPeerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Resolve local resolvable address.
 *
 *  \param      rpa                 Local resolvable private address
 *  \param      pPeerAddrType       Storage for peer identity address type.
 *  \param      pPeerIdentityAddr   Storage for peer identity address.
 *
 *  \return     TRUE if successful, FALSE if address could not be resolved.
 *
 *  Resolve a local resolvable address.
 */
/*************************************************************************************************/
bool_t BbBleResListResolveLocal(uint64_t rpa, uint8_t *pPeerAddrType, uint64_t *pPeerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Get status of peer address in resolving list.
 *
 *  \param      peerAddrRand        TRUE if peer identity address is random.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     Peer address status.
 *
 *  Get the peer resolvable private address status
 */
/*************************************************************************************************/
uint8_t BbBleResListPeerStatus(bool_t peerAddrRand, uint64_t peerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Get status of local address in resolving list.
 *
 *  \param      peerAddrRand        TRUE if peer identity address is random.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     Local address status.
 *
 *  Get the peer resolvable private address status
 */
/*************************************************************************************************/
uint8_t BbBleResListLocalStatus(bool_t peerAddrRand, uint64_t peerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Handle timeout of local resolvable addresses.
 *
 *  \return     None.
 *
 *  A new local resolvable address will be generated for each entry in the resolving list.
 */
/*************************************************************************************************/
void BbBleResListHandleTimeout(void);

/*************************************************************************************************/
/*!
 *  \brief      Check if either local RPA or peer RPA is updated.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     TRUE if either local RPA or peer RPA is updated.
 *
 */
/*************************************************************************************************/
bool_t BbBleResListIsRpaUpd(uint8_t peerAddrType, uint64_t peerIdentityAddr);

/*************************************************************************************************/
/*!
 *  \brief      Check if peer identity is in the resolving list.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     TRUE if peer addr is in the resolving list.
 *
 */
/*************************************************************************************************/
bool_t bbBleIsPeerInResList(uint8_t peerAddrType, uint64_t peerIdentityAddr);

/*! \} */    /* BB_API_BLE_RL */

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_API_RESLIST_H */
