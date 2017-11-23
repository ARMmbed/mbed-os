/*
 *---------------------------------------------------------------------------
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
 * Component   : GATT
 * File        : cb_gatt_utils.h
 *
 * Description : Helper functions for GATT
 *
 *-------------------------------------------------------------------------*/

/**
 * @file cb_gatt_utils.h
 *
 * @brief Helper functions for GATT
 */

#ifndef _CB_GATT_UTILS_H_
#define _CB_GATT_UTILS_H_

#include "cb_comdefs.h"
#include "bt_types.h"
#include "cb_gatt.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * FUNCTIONS
 *=============================================================================
 */

/**
 * Returns a string representing the error code. NULL if the error code is 
 * not found.
 * @param errorCode     GATT error code
 */
cb_char* cbGATT_UTILS_getStringFromErrorCode(
    cbGATT_ErrorCode errorCode);

/**
 * Returns a string representing the GATT/ATT properties for a characteristic.
 * @param properties    Bitmap of properties see cbGATT_PROP_*
 */
cb_char* cbGATT_UTILS_getStringFromProperties(
    cb_uint8 properties);

/**
 * Returns a string representing the UUID. NULL if the UUID is not 
 * found.
 * @param pUuid     Pointer to 128 or 16-bit UUID
 */
cb_char* cbGATT_UTILS_getStringFromUuid(
    cbGATT_Uuid* pUuid);

/**
 * Returns a string representing the UUID as hex bytes.
 * @param pUuid     Pointer to 128 or 16-bit UUID
 * @param resultStr Allocated buffer to put hex string in.
 *                  Should fit 16*2+1 = 33 bytes
 */
cb_char* cbGATT_UTILS_getHexStringFromUuid(
    cbGATT_Uuid*    pUuid, 
    cb_char*        resultStr);

/**
 * Returns a string representing the data as hex bytes.
 * @param pData     Pointer to data
 * @param len       Length of data
 * @param resultStr Pointer to allocated buffer to put hex string in.
 *                  Should fit len*2+1 bytes
 */
cb_char* cbGATT_UTILS_dataToHex(
    cb_uint8*   pData, 
    cb_uint16   len, 
    cb_char*    resultStr);

/**
 * Reverse bytes
 * @param src           Pointer to data to reverse bytes for
 * @param nbrOfBytes    Length of src
 */
void cbGATT_UTILS_reverseBytes(
    cb_uint8*   src, 
    cb_uint16   nbrOfBytes);

#ifdef __cplusplus
}
#endif

#endif
