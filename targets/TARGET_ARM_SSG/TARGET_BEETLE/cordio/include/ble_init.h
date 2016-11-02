/*************************************************************************************************/
/*!
 *  \file   ble_init.c
 *
 *  \brief  BLE initialization.
 *
 *  Copyright (c) 2015 ARM, Ltd., all rights reserved.
 *  ARM confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM, Ltd. prior
 *  to any use, copying or further distribution of this software.
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
