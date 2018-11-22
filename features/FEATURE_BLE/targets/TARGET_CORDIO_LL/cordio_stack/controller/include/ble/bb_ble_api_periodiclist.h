/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE baseband periodiclist interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef BB_BLE_API_PERIODICLIST_H
#define BB_BLE_API_PERIODICLIST_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BB_API_BLE_WL
 *  \{ */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Check if address is periodic listed.
 *
 *  \param      addrType    Address type.
 *  \param      addr        Bluetooth device address.
 *  \param      SID         Set ID.
 *
 *  \return     TRUE if white listed, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbBlePeriodicListCheckAddr(uint8_t addrType, uint64_t addr, uint8_t SID);

/*************************************************************************************************/
/*!
 *  \brief      Get periodic list size.
 *
 *  \return     Total number of periodic list entries.
 *
 *  Read the periodic list capacity supported by the BB.
 */
/*************************************************************************************************/
uint8_t BbBlePeriodicListGetSize(void);

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
void BbBlePeriodicListClear(void);

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
bool_t BbBlePeriodicListAdd(uint8_t addrType, uint64_t addr, uint8_t SID);

/*************************************************************************************************/
/*!
 *  \brief      Remove device from the periodic list.
 *
 *  \param      randAddr    TRUE if random address, FALSE if public.
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
bool_t BbBlePeriodicListRemove(bool_t randAddr, uint64_t addr, uint8_t SID);

/*! \} */    /* BB_API_BLE_WL */

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_API_WHITELIST_H */
