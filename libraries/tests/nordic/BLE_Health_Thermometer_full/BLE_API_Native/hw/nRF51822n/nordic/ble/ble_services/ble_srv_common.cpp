/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/

#include "ble_srv_common.h"
#include <string.h>
#include "nordic_common.h"
#include "app_error.h"


uint8_t ble_srv_report_ref_encode(uint8_t *                    p_encoded_buffer,
                                  const ble_srv_report_ref_t * p_report_ref)
{
    uint8_t len = 0;
    
    p_encoded_buffer[len++] = p_report_ref->report_id;
    p_encoded_buffer[len++] = p_report_ref->report_type;
    
    APP_ERROR_CHECK_BOOL(len == BLE_SRV_ENCODED_REPORT_REF_LEN);
    return len;
}


void ble_srv_ascii_to_utf8(ble_srv_utf8_str_t * p_utf8, char * p_ascii)
{
    p_utf8->length = (uint16_t)strlen(p_ascii);
    p_utf8->p_str  = (uint8_t *)p_ascii;
}
