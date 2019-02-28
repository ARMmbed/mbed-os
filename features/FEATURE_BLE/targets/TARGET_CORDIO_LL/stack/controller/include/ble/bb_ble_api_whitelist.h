/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE baseband whitelist interface file.
 *
 *  Copyright (c) 2016-2017 ARM Ltd. All Rights Reserved.
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
