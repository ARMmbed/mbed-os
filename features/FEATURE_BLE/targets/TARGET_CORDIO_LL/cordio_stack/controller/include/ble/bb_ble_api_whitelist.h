/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE baseband whitelist interface file.
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

#ifndef BB_BLE_API_WHITELIST_H
#define BB_BLE_API_WHITELIST_H

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
 *  \brief      Check if address is white listed.
 *
 *  \param      randAddr    TRUE if random address, FALSE if public.
 *  \param      addr        Bluetooth device address.
 *
 *  \return     TRUE if white listed, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbBleWhiteListCheckAddr(bool_t randAddr, uint64_t addr);

/*************************************************************************************************/
/*!
 *  \brief      Get white list size.
 *
 *  \return     Total number of white list entries.
 *
 *  Read the white list capacity supported by the BB.
 */
/*************************************************************************************************/
uint8_t BbBleWhiteListGetSize(void);

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
void BbBleWhiteListClear(void);

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
bool_t BbBleWhiteListAdd(bool_t randAddr, uint64_t addr);

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
bool_t BbBleWhiteListRemove(bool_t randAddr, uint64_t addr);

/*************************************************************************************************/
/*!
 *  \brief      Add anonymous device to the white list.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleWhiteListAddAnonymous(void);

/*************************************************************************************************/
/*!
 *  \brief      Remove anonymous device from the white list.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleWhiteListRemoveAnonymous(void);

/*************************************************************************************************/
/*!
 *  \brief      Check if anonymous peer is allowed.
 *
 *  \return     TRUE if anonymous allowed, FALSE if disallowed.
 */
/*************************************************************************************************/
bool_t BbBleWhiteListIsAnonymousAllowed(void);

/*! \} */    /* BB_API_BLE_WL */

#ifdef __cplusplus
};
#endif

#endif /* BB_BLE_API_WHITELIST_H */
