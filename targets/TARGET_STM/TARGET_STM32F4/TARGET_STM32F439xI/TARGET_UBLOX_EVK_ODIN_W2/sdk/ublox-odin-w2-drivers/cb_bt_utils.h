/*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : Bluetooth utility
 * File        : cb_bt_utils.h
 *
 * Description : 
 *-------------------------------------------------------------------------*/

#ifndef _CB_BT_UTILS_H_
#define _CB_BT_UTILS_H_

#include "cb_comdefs.h"
#include "bt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * FUNCTIONS
 *=========================================================================
 */

/**
 * Compare two Bluetooth addresses
 *
 * @param addr1   Pointer to first address to compare
 * @param addr2   Pointer to second address to compare
 * @returns TRUE if equal otherwise FALSE
 */
extern cb_boolean cbBT_UTILS_cmpBdAddr(
    TBdAddr* addr1, 
    TBdAddr* addr2);

/**
 * Check if address in invalid i.e. {{0,0,0,0,0,0},BT_PUBLIC_ADDRESS}
 *
 * @param addr    Pointer to address to check
 * @returns TRUE if invalid otherwise FALSE
 */
extern cb_boolean cbBT_UTILS_isInvalidBdAddr(
    TBdAddr* addr);

/**
 * Set invalid address i.e. {{0,0,0,0,0,0},BT_PUBLIC_ADDRESS}
 *
 * @param addr    Pointer where to put the address
 */
extern void cbBT_UTILS_setInvalidBdAddr(
    TBdAddr* addr);

/**
 * Get invalid address
 *
 * @returns Pointer to the invalid address
 */
extern const TBdAddr* cbBT_UTILS_getInvalidBdAddr(void);

/**
 * Copy Bluetooth address
 *
 * @param dest   Pointer to destination address
 * @param src    Pointer to source address
 */
extern void cbBT_UTILS_cpyBdAddr(
    TBdAddr* dest, 
    TBdAddr* src);

#ifdef __cplusplus
}
#endif

#endif /* _CB_BT_UTILS_H_ */
