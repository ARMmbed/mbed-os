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
 * \brief Generic BLE resolving list implementation file.
 */
/*************************************************************************************************/

#include "bb_api.h"
#include "bb_ble_api.h"
#include "bb_ble_api_reslist.h"
#include "bb_ble_api_pdufilt.h"
#include "stack/platform/include/pal_bb_ble.h"
#include "stack/platform/include/pal_crypto.h"
#include "wsf_assert.h"
#include "ll_math.h"
#include "util/bda.h"
#include "util/bstream.h"
#include "ll_defs.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! RPA high address word for invalid address. */
#define BB_BLE_RESLIST_RPA_INVALID   0

/*! \brief      Increment statistics counter. */
#define BB_INC_STAT(s)              s++

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Resolving list table entry. */
typedef struct
{
  uint8_t  peerIrk[LL_KEY_LEN];             /*!< Peer IRK. */
  uint8_t  localIrk[LL_KEY_LEN];            /*!< Local IRK. */
  uint64_t peerIdentityAddr;                /*!< Peer identity address. */
  uint64_t peerRpa;                         /*!< Peer resolvable private address. */
  uint64_t localRpa;                        /*!< Local resolvable private address. */
  uint64_t localRpaPeer;                    /*!< Local resolvable private address from peer. */
  bool_t   peerIrkZero;                     /*!< Indicates that peer IRK is zero. */
  bool_t   localIrkZero;                    /*!< Indicates that local IRK is zero. */
  bool_t   peerRpaGenerated;                /*!< Indicates that peer RPA was locally generated. */
  uint8_t  privMode;                        /*!< Privacy mode. */
  bool_t   isRpaUpd;                        /*!< TRUE if either lcaol RPA or peer RPA is updated. */
} bbBleResListEntry_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

static bbBleResListEntry_t  *pBbBleResListTbl;          /*!< Resolving list. */
static uint8_t              bbBleResListNumEntries;     /*!< Number of valid resolving list entries. */
static uint8_t              bbBleResListNumEntriesMax;  /*!< Maximum number of resolving list entries. */

/*! \brief      Device filter statistics. */
extern BbBlePduFiltStats_t  bbBlePduFiltStats;

/*! \brief      Address resolution needed callback. */
static bbBleResListAddrResNeeded_t bbBleResListAddrResNeededCback;

/*************************************************************************************************/
/*!
 *  \brief      Initialize resolving list.
 *
 *  \param      numEntries      Number of resolving list entries to provide.
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function initializes the resolving list.
 *
 *  \note       This function must be called once before initializing the LL.
 */
/*************************************************************************************************/
uint16_t BbBleInitResolvingList(uint8_t numEntries, uint8_t *pFreeMem, uint32_t freeMemSize)
{
  uint8_t *pAvailMem = pFreeMem;

  bbBleResListNumEntries    = 0;
  bbBleResListNumEntriesMax = 0;

  /* Allocate memory. */
  if (((uint32_t)pAvailMem) & 3)
  {
    /* Align to next word. */
    pAvailMem = (uint8_t *)(((uint32_t)pAvailMem & ~3) + sizeof(uint32_t));
  }
  pBbBleResListTbl = (bbBleResListEntry_t *)pAvailMem;
  pAvailMem  += sizeof(bbBleResListEntry_t) * numEntries;

  /* Check memory allocation. */
  if (((uint32_t)(pAvailMem - pFreeMem)) > freeMemSize)
  {
    WSF_ASSERT(FALSE);
    return 0;
  }

  bbBleResListNumEntriesMax = numEntries;
  return (pAvailMem - pFreeMem);
}

/*************************************************************************************************/
/*!
 *  \brief      Set address resolution needed callback.
 *
 *  \param      cback             Callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleResListSetAddrResNeededCback(bbBleResListAddrResNeeded_t cback)
{
  bbBleResListAddrResNeededCback = cback;
}

/*************************************************************************************************/
/*!
 *  \brief      Find entry in resolving list.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *
 *  \return     Pointer to entry or NULL if it does not exist.
 *
 *  Find entry in resolving list.
 */
/*************************************************************************************************/
static bbBleResListEntry_t *bbBleFindResolvingListEntry(uint8_t peerAddrType, uint64_t peerIdentityAddr)
{
  peerIdentityAddr |= (uint64_t)peerAddrType << 48;

  uint8_t i;

  for (i = 0; i < bbBleResListNumEntries; i++)
  {
    if (pBbBleResListTbl[i].peerIdentityAddr == peerIdentityAddr)
    {
      return &pBbBleResListTbl[i];
    }
  }

  return NULL;
}

/*************************************************************************************************/
/*!
 *  \brief      Generate a random address hash.
 *
 *  \param      pIrk            IRK.
 *  \param      r               Random value.
 *
 *  \return     Random address hash.
 *
 *  random address hash.
 */
/*************************************************************************************************/
static uint32_t bbGenerateHash(const uint8_t *pIrk, uint32_t r)
{
  uint8_t  rprime[LL_KEY_LEN];
  uint32_t hash;

  /* r' = padding | r */
  memset(rprime, 0, sizeof(rprime));
  rprime[0] = ((r >>  0) & 0xFF);
  rprime[1] = ((r >>  8) & 0xFF);
  rprime[2] = ((r >> 16) & 0xFF);

  /* r' = e(k, r') */
  PalCryptoAesEcb(pIrk, rprime, rprime);

  /* ah(k, r) = e(k, r') mod 2^24 */
  hash = (rprime[0] <<  0) |
         (rprime[1] <<  8) |
         (rprime[2] << 16);
  return hash;
}

/*************************************************************************************************/
/*!
 *  \brief      Generate a RPA.
 *
 *  \param      pIrk            IRK.
 *
 *  \return     RPA.
 *
 *  Generate a RPA.
 */
/*************************************************************************************************/
static uint64_t bbGenerateRpa(const uint8_t *pIrk)
{
  uint32_t prand;
  uint32_t hash;
  uint64_t rpa;

  PalCryptoGenerateRandomNumber((uint8_t *)&prand, (sizeof(uint32_t) / sizeof(uint8_t)));

  prand = (prand & 0x003FFFFF) | 0x00400000;
  hash  = bbGenerateHash(pIrk, prand);
  rpa   = ((uint64_t)prand << 24) | hash;
  return rpa;
}

/*************************************************************************************************/
/*!
 *  \brief      Verify a RPA was generated from an IRK.
 *
 *  \param      pIrk            IRK.
 *  \param      rpa             Resolvable private address to verify.
 *
 *  \return     TRUE if the RPA was generated from this IRK.
 *
 *  Verify a RPA was generated from an IRK.
 */
/*************************************************************************************************/
static bool_t bbVerifyRpa(const uint8_t *pIrk, uint64_t rpa)
{
  uint32_t prand;
  uint32_t hash;
  uint32_t localHash;

  prand     = (rpa >> 24) & UINT64_C(0xFFFFFF);
  hash      = (rpa >>  0) & UINT64_C(0xFFFFFF);
  localHash  = bbGenerateHash(pIrk, prand);
  return (hash == localHash);
}

/*************************************************************************************************/
/*!
 *  \brief      Get resolving list size.
 *
 *  \return     Total number of resolving list entries.
 *
 *  Get the resolving list capacity supported by the BB.
 */
/*************************************************************************************************/
uint8_t BbBleResListGetSize(void)
{
  return bbBleResListNumEntriesMax;
}

/*************************************************************************************************/
/*!
 *  \brief      Clear resolving list.
 *
 *  \return     None.
 *
 *  Clear all resolving list entries stored in the BB.
 */
/*************************************************************************************************/
void BbBleResListClear(void)
{
  bbBleResListNumEntries = 0;
}

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
    const uint8_t *pLocalIrk)
{
  bbBleResListEntry_t *pEntry;

  /* Check whether this entry already exists. */
  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry == NULL)
  {
    /* Otherwise add new entry. */
    if (bbBleResListNumEntries < bbBleResListNumEntriesMax)
    {
      pEntry = &pBbBleResListTbl[bbBleResListNumEntries];
      bbBleResListNumEntries++;
    }
  }

  /* Initialize entry. */
  if (pEntry != NULL)
  {
    uint8_t i;

    peerIdentityAddr |= ((uint64_t)peerAddrType << 48);
    pEntry->peerIdentityAddr = peerIdentityAddr;

    memcpy(pEntry->peerIrk, pPeerIrk, sizeof(pEntry->peerIrk));
    pEntry->peerIrkZero = TRUE;
    for (i = 0; i < sizeof(pEntry->peerIrk); i++)
    {
      if (pEntry->peerIrk[i] != 0)
      {
        pEntry->peerIrkZero = FALSE;
        break;
      }
    }

    memcpy(pEntry->localIrk, pLocalIrk, sizeof(pEntry->localIrk));
    pEntry->localIrkZero = TRUE;
    for (i = 0; i < sizeof(pEntry->localIrk); i++)
    {
      if (pEntry->localIrk[i] != 0)
      {
        pEntry->localIrkZero = FALSE;
        break;
      }
    }

    pEntry->peerRpa          = BB_BLE_RESLIST_RPA_INVALID;
    pEntry->localRpa         = BB_BLE_RESLIST_RPA_INVALID;
    pEntry->localRpaPeer     = BB_BLE_RESLIST_RPA_INVALID;
    pEntry->peerRpaGenerated = FALSE;
    pEntry->privMode         = BB_BLE_RESLIST_PRIV_MODE_NETWORK;

    /* Generate a local RPA now. */
    if (!pEntry->localIrkZero)
    {
      pEntry->localRpa = bbGenerateRpa(pEntry->localIrk);
    }

    return TRUE;
  }

  /* List full. */
  return FALSE;
}

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
bool_t BbBleResListRemove(uint8_t peerAddrType, uint64_t peerIdentityAddr)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    if (bbBleResListNumEntries > 1)
    {
      /* If there is more than one entry, move the last entry into this slot. */
      memcpy(pEntry, &pBbBleResListTbl[bbBleResListNumEntries - 1], sizeof(*pEntry));
    }
    bbBleResListNumEntries--;
    return TRUE;
  }

  return FALSE;
}

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
bool_t BbBleResListSetPrivacyMode(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint8_t privMode)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    pEntry->privMode = privMode;
    return TRUE;
  }

  return FALSE;
}

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
bool_t BbBleResListGetPrivacyMode(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint8_t *pPrivMode)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    *pPrivMode = pEntry->privMode;
    return TRUE;
  }

  return FALSE;
}

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
bool_t BbBleResListReadPeer(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    if (pEntry->peerRpa != BB_BLE_RESLIST_RPA_INVALID)
    {
      *pRpa = pEntry->peerRpa;
      return TRUE;
    }
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Read local resolvable address.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pRpa                Storage for local resolvable private address
 *
 *  \return     TRUE if successful, FALSE if address not in list.
 *
 *  Get the local resolvable private address that is currently being used for the peer identity
 *  address.
 */
/*************************************************************************************************/
bool_t BbBleResListReadLocal(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    if (pEntry->localRpa != BB_BLE_RESLIST_RPA_INVALID)
    {
      *pRpa = pEntry->localRpa;
      return TRUE;
    }
  }

  return FALSE;
}

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
bool_t BbBleResListUpdateLocal(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    pEntry->localRpa = *pRpa;
    return TRUE;
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Generate peer resolvable address.
 *
 *  \param      peerAddrType        Peer identity address type.
 *  \param      peerIdentityAddr    Peer identity address.
 *  \param      pRpa                Storage for peer resolvable private address
 *
 *  \return     TRUE if successful, FALSE if address not in list or peer IRK is zero.
 *
 *  Generate a peer resolvable address for a peer.
 */
/*************************************************************************************************/
bool_t BbBleResListGeneratePeer(uint8_t peerAddrType, uint64_t peerIdentityAddr, uint64_t *pRpa)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    /* There will never be a RPA if the IRK is zero. */
    if (pEntry->peerIrkZero)
    {
      return FALSE;
    }
    /* Generate a new RPA only if one is not generated locally. */
    if (!pEntry->peerRpaGenerated)
    {
      pEntry->peerRpa          = bbGenerateRpa(pEntry->peerIrk);
      pEntry->peerRpaGenerated = TRUE;
    }
    *pRpa = pEntry->peerRpa;
    return TRUE;
  }

  return FALSE;
}

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
bool_t BbBleResListCheckResolvePeer(uint64_t rpa, uint8_t *pPeerAddrType, uint64_t *pPeerIdentityAddr)
{
  uint8_t i;
  bool_t resCback = FALSE; /* Only call callback if we have a non-empty resolving list. */

  for (i = 0; i < bbBleResListNumEntries; i++)
  {
    bbBleResListEntry_t *pEntry = &pBbBleResListTbl[i];

    if (!pEntry->peerIrkZero)
    {
      /* Check whether this RPA is the identical to the cached value. */
      if (rpa == pEntry->peerRpa)
      {
        *pPeerAddrType     = (pEntry->peerIdentityAddr >> 48) & 0x1;
        *pPeerIdentityAddr = pEntry->peerIdentityAddr & UINT64_C(0xFFFFFFFFFFFF);
        return TRUE;
      }
      resCback = TRUE;
    }
  }

  if (resCback && bbBleResListAddrResNeededCback)
  {
    BB_INC_STAT(bbBlePduFiltStats.peerResAddrPend);
    bbBleResListAddrResNeededCback(rpa, TRUE, *pPeerAddrType, *pPeerIdentityAddr);
  }

  return FALSE;
}

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
bool_t BbBleResListCheckResolveLocal(uint64_t rpa, uint8_t peerAddrType, uint64_t peerIdentityAddr)
{
  bbBleResListEntry_t *pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);

  if (pEntry != NULL)
  {
    if (!pEntry->localIrkZero)
    {
      /* Check whether this RPA is the identical to the cached value. */
      if (rpa == pEntry->localRpaPeer)
      {
        return TRUE;
      }
      else
      {
        if (bbBleResListAddrResNeededCback)
        {
          BB_INC_STAT(bbBlePduFiltStats.localResAddrPend);
          bbBleResListAddrResNeededCback(rpa, FALSE, peerAddrType, peerIdentityAddr);
        }
      }
    }
  }

  return FALSE;
}

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
bool_t BbBleResListIsLocalResolved(uint64_t rpa, uint8_t peerAddrType, uint64_t peerIdentityAddr)
{
  bbBleResListEntry_t *pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);

  if (pEntry != NULL)
  {
    if (!pEntry->localIrkZero)
    {
      /* Check whether this RPA is the identical to the cached value. */
      if (rpa == pEntry->localRpaPeer)
      {
        return TRUE;
      }
    }
  }

  return FALSE;
}

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
bool_t BbBleResListResolvePeer(uint64_t rpa, uint8_t *pPeerAddrType, uint64_t *pPeerIdentityAddr)
{
  uint8_t i;

  for (i = 0; i < bbBleResListNumEntries; i++)
  {
    bbBleResListEntry_t *pEntry = &pBbBleResListTbl[i];

    if (!pEntry->peerIrkZero)
    {
      bool_t found = FALSE;

      /* Check whether this RPA is the identical to the cached value. */
      if (rpa == pEntry->peerRpa)
      {
        found = TRUE;
      }
      else if (bbVerifyRpa(pEntry->peerIrk, rpa))
      {
        /* Cache this RPA. */
        pEntry->peerRpa = rpa;
        pEntry->peerRpaGenerated = FALSE;
        found = TRUE;
        BB_INC_STAT(bbBlePduFiltStats.passPeerRpaVerify);
      }

      if (found)
      {
        *pPeerAddrType     = (pEntry->peerIdentityAddr >> 48) & 0x1;
        *pPeerIdentityAddr = pEntry->peerIdentityAddr & UINT64_C(0xFFFFFFFFFFFF);
        return TRUE;
      }
    }
  }

  BB_INC_STAT(bbBlePduFiltStats.failPeerRpaVerify);
  return FALSE;
}

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
bool_t BbBleResListResolveLocal(uint64_t rpa, uint8_t *pPeerAddrType, uint64_t *pPeerIdentityAddr)
{
  bbBleResListEntry_t *pEntry = bbBleFindResolvingListEntry(*pPeerAddrType, *pPeerIdentityAddr);

  if (pEntry != NULL)
  {
    if (!pEntry->localIrkZero)
    {
      /* Check whether this RPA is the identical to the cached value. */
      if (rpa == pEntry->localRpaPeer)
      {
        return TRUE;
      }
      else if (bbVerifyRpa(pEntry->localIrk, rpa))
      {
        pEntry->localRpaPeer = rpa;
        BB_INC_STAT(bbBlePduFiltStats.passLocalRpaVerify);
        return TRUE;
      }
    }
  }

  BB_INC_STAT(bbBlePduFiltStats.failLocalRpaVerify);
  return FALSE;
}

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
bool_t BbBleResListPeerStatus(bool_t peerAddrRand, uint64_t peerIdentityAddr)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrRand, peerIdentityAddr);
  if (pEntry != NULL)
  {
    if (pEntry->peerRpa != BB_BLE_RESLIST_RPA_INVALID)
    {
      return BB_BLE_RESLIST_STATUS_RES_ADDR_ASSIGNED;
    }
    else if (pEntry->peerIrkZero)
    {
      return BB_BLE_RESLIST_STATUS_ZERO_IRK;
    }
    else
    {
      return BB_BLE_RESLIST_STATUS_RES_ADDR_UNASSIGNED;
    }
  }

  return BB_BLE_RESLIST_STATUS_ID_ADDR_NOT_IN_LIST;
}

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
bool_t BbBleResListLocalStatus(bool_t peerAddrRand, uint64_t peerIdentityAddr)
{
  bbBleResListEntry_t *pEntry;

  pEntry = bbBleFindResolvingListEntry(peerAddrRand, peerIdentityAddr);
  if (pEntry != NULL)
  {
    if (pEntry->localRpa != BB_BLE_RESLIST_RPA_INVALID)
    {
      return BB_BLE_RESLIST_STATUS_RES_ADDR_ASSIGNED;
    }
    else if (pEntry->localIrkZero)
    {
      return BB_BLE_RESLIST_STATUS_ZERO_IRK;
    }
    else
    {
      return BB_BLE_RESLIST_STATUS_RES_ADDR_UNASSIGNED;
    }
  }

  return BB_BLE_RESLIST_STATUS_ID_ADDR_NOT_IN_LIST;
}

/*************************************************************************************************/
/*!
 *  \brief      Handle timeout of local resolvable addresses.
 *
 *  \return     None.
 *
 *  A new local resolvable address will be generated for each entry in the resolving list.
 */
/*************************************************************************************************/
void BbBleResListHandleTimeout(void)
{
  uint8_t i;

  for (i = 0; i < bbBleResListNumEntries; i++)
  {
    bbBleResListEntry_t *pEntry = &pBbBleResListTbl[i];

    if (!pEntry->localIrkZero)
    {
      pEntry->localRpa = bbGenerateRpa(pEntry->localIrk);
    }
    if (!pEntry->peerIrkZero && pEntry->peerRpaGenerated)
    {
      pEntry->peerRpa = bbGenerateRpa(pEntry->peerIrk);
    }

    pEntry->isRpaUpd = TRUE;
  }
}

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
bool_t BbBleResListIsRpaUpd(uint8_t peerAddrType, uint64_t peerIdentityAddr)
{
  bbBleResListEntry_t *pEntry;
  bool_t  result;

  pEntry = bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr);
  if (pEntry != NULL)
  {
    result = pEntry->isRpaUpd;
    pEntry->isRpaUpd = FALSE;
    return result;
  }

  return FALSE;
}

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
bool_t bbBleIsPeerInResList(uint8_t peerAddrType, uint64_t peerIdentityAddr)
{
  return (bbBleFindResolvingListEntry(peerAddrType, peerIdentityAddr) != NULL) ? TRUE : FALSE;
}
