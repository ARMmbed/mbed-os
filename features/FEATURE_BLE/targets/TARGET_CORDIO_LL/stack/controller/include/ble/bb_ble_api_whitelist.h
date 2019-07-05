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
 * \brief BLE baseband whitelist interface file.
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
