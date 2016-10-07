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
 * Component   : OTP
 * File        : cb_otp.h
 *
 * Description : Support for One Time Programmable memory intended for
 *               storing production parameters such as mac addresses, trim
 *               values and product configuration. Writing to OTP memory shall 
 *               only be done in a production environment.
 *-------------------------------------------------------------------------*/

/**
 * @file cb_otp.h 
 * @ingroup platform
 */

#ifndef _CB_OTP_H_
#define _CB_OTP_H_

#include "cb_comdefs.h"
#include "cb_status.h"

#define cbOTP_MAX_SIZE          (30)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    cbOTP_MAC_BLUETOOTH = 1,
    cbOTP_MAC_WLAN,
    cbOTP_MAC_ETHERNET,
    cbOTP_MAC_FEATURE_INFO,
    cbOTP_MAC_DEBUG_UNIT,
    cbOTP_SERIAL_NUMBER,
    cbOTP_TYPE_CODE,
    cbOTP_RESERVED_UNUSED = 255
} cbOTP_Id;


/**
 * Read a OTP parameter
 * @param id    The id of the parameter to write
 * @param len   The length of the parameter to write
 * @param buf   Pointer to data to be written
 * @returns     The read length of the id is returned. If the read fails 0 is returned
 */
cb_uint32 cbOTP_read(cbOTP_Id id, cb_uint32 len, cb_uint8 *buf);

#ifdef __cplusplus
}
#endif

#endif
