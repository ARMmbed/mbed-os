/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Generic BLE whitelist implementation file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
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

/*! \brief      White list filter table entry. */
typedef struct
{
  uint64_t addr;                            /*!< Address. */
} bbBleWhiteListEntry_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

static bbBleWhiteListEntry_t  *pBbBleWhiteListFilt;         /*!< White list filter. */
static uint8_t                bbBleWhiteListNumEntries;     /*!< Number of valid white list entries. */
static uint8_t                bbBleWhiteListNumEntriesMax;  /*!< Maximum number of white list entries. */
static bool_t                 bbBleWhiteListAllowAnonymous; /*!< Allow anonymous peer address. */

/*************************************************************************************************/
/*!
 *  \brief      Initialize white list.
 *
 *  \param      numEntries      Number of white list entries to provide.
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
uint16_t BbBleInitWhiteList(uint8_t numEntries, uint8_t *pFreeMem, uint32_t freeMemSize)
{
  uint8_t *pAvailMem = pFreeMem;

  bbBleWhiteListNumEntries     = 0;
  bbBleWhiteListNumEntriesMax  = 0;
  bbBleWhiteListAllowAnonymous = FALSE;

  /* Allocate memory. */
  if (((uint32_t)pAvailMem) & 3)
  {
    /* Align to next word. */
    pAvailMem = (uint8_t *)(((uint32_t)pAvailMem & ~3) + sizeof(uint32_t));
  }
  pBbBleWhiteListFilt = (bbBleWhiteListEntry_t *)pAvailMem;
  pAvailMem   += sizeof(bbBleWhiteListEntry_t) * numEntries;

  /* Check memory allocation. */
  if (((uint32_t)(pAvailMem - pFreeMem)) > freeMemSize)
  {
    WSF_ASSERT(FALSE);
    return 0;
  }

  bbBleWhiteListNumEntriesMax = numEntries;
  return (pAvailMem - pFreeMem);
}

/*************************************************************************************************/
/*!
 *  \brief      Check if address is white listed.
 *
 *  \param      randAddr    TRUE if random address, FALSE if public.
 *  \param      addr        Bluetooth device address.
 *
 *  \return     TRUE if white listed, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbBleWhiteListCheckAddr(bool_t randAddr, uint64_t addr)
{
  addr |= (uint64_t)randAddr << 48;

  uint8_t i;
  for (i = 0; i < bbBleWhiteListNumEntries; i++)
  {
    if (addr == pBbBleWhiteListFilt[i].addr)
    {
      /* Peer is in white list, allow PDU to pass through the filter. */
      return TRUE;
    }
  }

  /* Peer is not in white list, filter out PDU. */
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Get white list size.
 *
 *  \return     Total number of white list entries.
 *
 *  Read the white list capacity supported by the BB.
 */
/*************************************************************************************************/
uint8_t BbBleWhiteListGetSize(void)
{
  return bbBleWhiteListNumEntriesMax;
}

/*************************************************************************************************/
/*!
 *  \brief      Clear all white list entries.
 *
 *  \return     None.
 *
 *  Clear all white list entries stored in the BB.
 *
 *  \note       No resource synchronization is required to modify the white list resource as
 *              the LL modifies the white list only when advertising events are disabled.
 */
/*************************************************************************************************/
void BbBleWhiteListClear(void)
{
  bbBleWhiteListNumEntries = 0;
  bbBleWhiteListAllowAnonymous = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Add device to the white list.
 *
 *  \param      randAddr    TRUE if random address, FALSE if public.
 *  \param      addr        Bluetooth device address.
 *
 *  \return     TRUE if successful, FALSE if list full.
 *
 *  Adds the given address to the white list stored in the BB.
 *
 *  \note       No resource synchronization is required to modify the white list resource as
 *              the LL modifies the white list only when advertising events are disabled.
 */
/*************************************************************************************************/
bool_t BbBleWhiteListAdd(bool_t randAddr, uint64_t addr)
{
  if (bbBleWhiteListNumEntries < bbBleWhiteListNumEntriesMax)
  {
    addr |= (uint64_t)randAddr << 48;
    pBbBleWhiteListFilt[bbBleWhiteListNumEntries].addr = addr;

    bbBleWhiteListNumEntries++;
    return TRUE;
  }

  /* List full. */
  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Remove device from the white list.
 *
 *  \param      randAddr    TRUE if random address, FALSE if public.
 *  \param      addr        Bluetooth device address.
 *
 *  \return     TRUE if successful, FALSE if address not in the list.
 *
 *  Removes the given address from the white list stored in the BB.
 *
 *  \note       No resource synchronization is required to modify the white list resource as
 *              the LL modifies the white list only when advertising events are disabled.
 */
/*************************************************************************************************/
bool_t BbBleWhiteListRemove(bool_t randAddr, uint64_t addr)
{
  addr |= (uint64_t)randAddr << 48;

  uint8_t i;

  for (i = 0; i < bbBleWhiteListNumEntries; i++)
  {
    if (pBbBleWhiteListFilt[i].addr == addr)
    {
      /* If there is more than one entry, move the last entry into this slot. */
      if (bbBleWhiteListNumEntries > 1)
      {
        pBbBleWhiteListFilt[i].addr = pBbBleWhiteListFilt[bbBleWhiteListNumEntries - 1].addr;
      }
      bbBleWhiteListNumEntries--;
      return TRUE;
    }
  }

  return FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Add anonymous device to the white list.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleWhiteListAddAnonymous(void)
{
  bbBleWhiteListAllowAnonymous = TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Remove anonymous device from the white list.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleWhiteListRemoveAnonymous(void)
{
  bbBleWhiteListAllowAnonymous = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Get allow anonymous peers.
 *
 *  \return     TRUE if anonymous allowed, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbBleWhiteListIsAnonymousAllowed(void)
{
  return bbBleWhiteListAllowAnonymous;
}
