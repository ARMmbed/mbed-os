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
 * \brief Generic BLE whitelist implementation file.
 */
/*************************************************************************************************/

#include "bb_ble_api.h"
#include "wsf_assert.h"
#include "util/bda.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Periodic list filter table entry. */
typedef struct
{
  uint64_t  addr;                           /*!< Address. */
  uint8_t   addrType;                       /*!< Address type. */
  uint8_t   SID;                            /*!< Set ID. */
} bbBlePeriodicListEntry_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

static bbBlePeriodicListEntry_t *pBbBlePeriodicListFilt;        /*!< Periodic list filter. */
static uint8_t                  bbBlePeriodicListNumEntries;    /*!< Number of valid periodic list entries. */
static uint8_t                  bbBlePeriodicListNumEntriesMax; /*!< Maximum number of periodic list entries. */

/*************************************************************************************************/
/*!
 *  \brief      Initialize periodic list.
 *
 *  \param      numEntries      Number of periodic list entries to provide.
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function initializes the white list.
 *
 *  \note       This function must be called once before initializing the LL.
 */
/*************************************************************************************************/
uint16_t BbBleInitPeriodicList(uint8_t numEntries, uint8_t *pFreeMem, uint32_t freeMemSize)
{
  uint8_t *pAvailMem = pFreeMem;

  bbBlePeriodicListNumEntries     = 0;
  bbBlePeriodicListNumEntriesMax  = 0;

  /* Allocate memory. */
  if (((uint32_t)pAvailMem) & 3)
  {
    /* Align to next word. */
    pAvailMem = (uint8_t *)(((uint32_t)pAvailMem & ~3) + sizeof(uint32_t));
  }
  pBbBlePeriodicListFilt = (bbBlePeriodicListEntry_t *)pAvailMem;
  pAvailMem   += sizeof(bbBlePeriodicListEntry_t) * numEntries;

  /* Check memory allocation. */
  if (((uint32_t)(pAvailMem - pFreeMem)) > freeMemSize)
  {
    WSF_ASSERT(FALSE);
    return 0;
  }

  bbBlePeriodicListNumEntriesMax = numEntries;
  return (pAvailMem - pFreeMem);
}

/*************************************************************************************************/
/*!
 *  \brief      Check if address is periodic listed.
 *
 *  \param      addrType    Address type.
 *  \param      addr        Bluetooth device address.
 *  \param      SID         Set ID.
 *
 *  \return     TRUE if periodic listed, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbBlePeriodicListCheckAddr(uint8_t addrType, uint64_t addr, uint8_t SID)
{
  for (uint8_t i = 0; i < bbBlePeriodicListNumEntries; i++)
  {
    if ((pBbBlePeriodicListFilt[i].addrType == addrType) &&
        (pBbBlePeriodicListFilt[i].addr == addr) &&
        (pBbBlePeriodicListFilt[i].SID == SID))
    {
      /* Peer is in periodic list, allow PDU to pass through the filter. */
      return TRUE;
    }
  }

  /* Peer is not in periodic list, filter out PDU. */
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Get periodic list size.
 *
 *  \return     Total number of periodic list entries.
 *
 *  Read the periodic list capacity supported by the BB.
 */
/*************************************************************************************************/
uint8_t BbBlePeriodicListGetSize(void)
{
  return bbBlePeriodicListNumEntriesMax;
}

/*************************************************************************************************/
/*!
 *  \brief      Clear all periodic list entries.
 *
 *  \return     None.
 *
 *  Clear all periodic list entries stored in the BB.
 *
 *  \note       No resource synchronization is required to modify the periodic list resource as
 *              the LL modifies the periodic list only when advertising events are disabled.
 */
/*************************************************************************************************/
void BbBlePeriodicListClear(void)
{
  bbBlePeriodicListNumEntries = 0;
}

/*************************************************************************************************/
/*!
 *  \brief      Add device to the periodic list.
 *
 *  \param      addrType    Address type.
 *  \param      addr        Bluetooth device address.
 *  \param      SID         Set ID.
 *
 *  \return     TRUE if successful, FALSE if list full.
 *
 *  Adds the given address to the periodic list stored in the BB.
 *
 *  \note       No resource synchronization is required to modify the periodic list resource as
 *              the LL modifies the periodic list only when advertising events are disabled.
 */
/*************************************************************************************************/
bool_t BbBlePeriodicListAdd(uint8_t addrType, uint64_t addr, uint8_t SID)
{
  if (bbBlePeriodicListNumEntries < bbBlePeriodicListNumEntriesMax)
  {
    pBbBlePeriodicListFilt[bbBlePeriodicListNumEntries].addrType = addrType;
    pBbBlePeriodicListFilt[bbBlePeriodicListNumEntries].addr = addr;
    pBbBlePeriodicListFilt[bbBlePeriodicListNumEntries].SID  = SID;

    bbBlePeriodicListNumEntries++;
    return TRUE;
  }

  /* List full. */
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Remove device from the periodic list.
 *
 *  \param      addrType    Address type.
 *  \param      addr        Bluetooth device address.
 *  \param      SID         Set ID.
 *
 *
 *  \return     TRUE if successful, FALSE if address not in the list.
 *
 *  Removes the given address from the periodic list stored in the BB.
 *
 *  \note       No resource synchronization is required to modify the white list resource as
 *              the LL modifies the white list only when advertising events are disabled.
 */
/*************************************************************************************************/
bool_t BbBlePeriodicListRemove(uint8_t addrType, uint64_t addr, uint8_t SID)
{
  for (uint8_t i = 0; i < bbBlePeriodicListNumEntries; i++)
  {
    if ((pBbBlePeriodicListFilt[i].addrType == addrType) &&
        (pBbBlePeriodicListFilt[i].addr == addr) &&
        (pBbBlePeriodicListFilt[i].SID == SID))
    {
      /* If there is more than one entry, move the last entry into this slot. */
      if ((bbBlePeriodicListNumEntries > 1) && (i != bbBlePeriodicListNumEntries - 1))
      {
        pBbBlePeriodicListFilt[i].addrType = pBbBlePeriodicListFilt[bbBlePeriodicListNumEntries - 1].addrType;
        pBbBlePeriodicListFilt[i].addr = pBbBlePeriodicListFilt[bbBlePeriodicListNumEntries - 1].addr;
        pBbBlePeriodicListFilt[i].SID = pBbBlePeriodicListFilt[bbBlePeriodicListNumEntries - 1].SID;
      }
      bbBlePeriodicListNumEntries--;
      return TRUE;
    }
  }

  return FALSE;
}
