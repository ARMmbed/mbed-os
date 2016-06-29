/*************************************************************************************************/
/*!
 *  \file   ble_init.c
 *
 *  \brief  BLE initialization.
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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
 */
/*************************************************************************************************/

#ifndef BLE_INIT_H
#define BLE_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************/
/*!
 *  \fn     BleInitStart
 *
 *  \brief  Set BLE initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BleInitStart(void);

#ifdef __cplusplus
}
#endif

#endif /* BLE_INIT_H */
