/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file WHD utilities
 *
 * Utilities to help do specialized (not general purpose) WHD specific things
 */
#include "whd_chip.h"
#include "whd_events_int.h"
#include "whd_types_int.h"

#ifndef INCLUDED_WHD_UTILS_H_
#define INCLUDED_WHD_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Searches for a specific WiFi Information Element in a byte array
 *
 * Traverse a string of 1-byte tag/1-byte length/variable-length value
 * triples, returning a pointer to the substring whose first element
 * matches tag
 *
 * @note : This function has been copied directly from the standard Broadcom host driver file wl/exe/wlu.c
 *
 *
 * @param tlv_buf : The byte array containing the Information Elements (IEs)
 * @param buflen  : The length of the tlv_buf byte array
 * @param key     : The Information Element tag to search for
 *
 * @return    NULL : if no matching Information Element was found
 *            Non-Null : Pointer to the start of the matching Information Element
 */

whd_tlv8_header_t *whd_parse_tlvs(const whd_tlv8_header_t *tlv_buf, uint32_t buflen, dot11_ie_id_t key);

/** Checks if a WiFi Information Element is a WPA entry
 *
 * Is this body of this tlvs entry a WPA entry? If
 * not update the tlvs buffer pointer/length
 *
 * @note : This function has been copied directly from the standard Broadcom host driver file wl/exe/wlu.c
 *
 * @param wpaie    : The byte array containing the Information Element (IE)
 * @param tlvs     : The larger IE array to be updated if not a WPA IE
 * @param tlvs_len : The current length of larger IE array
 *
 * @return    WHD_TRUE  : if IE matches the WPA OUI (Organizationally Unique Identifier) and its type = 1
 *            WHD_FALSE : otherwise
 */
whd_bool_t whd_is_wpa_ie(vendor_specific_ie_header_t *wpaie, whd_tlv8_header_t **tlvs, uint32_t *tlvs_len);

/** Searches for a specific WiFi Information Element in a byte array
 *
 * Traverse a string of 1-byte tag/1-byte length/variable-length value
 * triples, returning a pointer to the substring whose first element
 * matches tag
 *
 * @note : This function has been copied directly from the standard Broadcom host driver file wl/exe/wlu.c
 *
 *
 * @param tlv_buf : The byte array containing the Information Elements (IEs)
 * @param buflen  : The length of the tlv_buf byte array
 * @param key     : The Information Element tag to search for
 *
 * @return    NULL : if no matching Information Element was found
 *            Non-Null : Pointer to the start of the matching Information Element
 */
whd_tlv8_header_t *whd_parse_dot11_tlvs(const whd_tlv8_header_t *tlv_buf, uint32_t buflen, dot11_ie_id_t key);

/******************************************************
*             Debug helper functionality
******************************************************/
#ifdef WPRINT_ENABLE_WHD_DEBUG
const char *whd_event_to_string(whd_event_num_t var);
char *whd_ssid_to_string(uint8_t *value, uint8_t length, char *ssid_buf, uint8_t ssid_buf_len);
const char *whd_status_to_string(whd_event_status_t status);
const char *whd_reason_to_string(whd_event_reason_t reason);
char *whd_ether_ntoa(const uint8_t *ea, char *buf, uint8_t buf_len);
#endif /* ifdef WPRINT_ENABLE_WHD_DEBUG */

/**
 ******************************************************************************
 * Prints partial details of a scan result on a single line
 * @param[in] record :  A pointer to the whd_scan_result_t record
 *
 */
extern void whd_print_scan_result(whd_scan_result_t *record);

/**
 ******************************************************************************
 * Convert a security bitmap to string
 * @param[in] security :  security of type whd_security_t
 * @param[in] out_str :  a character array to store output
 * @param[in] out_str_len :  length of out_str char array
 *
 */
extern void whd_convert_security_type_to_string(whd_security_t security, char *out_str, uint16_t out_str_len);

/*!
 ******************************************************************************
 * Convert an IOCTL to a string.
 *
 * @param[in] cmd  The ioct_log value.
 * @param[out] ioctl_str The string value after conversion.
 * @param[out] ioctl_str_len The string length of the IOCTL string.
 *
 * @result
 */
void whd_ioctl_to_string(uint32_t cmd, char *ioctl_str, uint16_t ioctl_str_len);

/*!
 ******************************************************************************
 * Convert event, status and reason value coming from firmware to string.
 *
 * @param[in] cmd  The event value in numeric form.
 * @param[in] flag  The status value in numeric form.
 * @param[in] reason  The reson value in numeric form.
 * @param[out] ioctl_str  The string representation of event, status and reason.
 * @param[out] ioctl_str_len  The str_len of ioctl_str.
 *
 * @result
 */
void whd_event_info_to_string(uint32_t cmd, uint16_t flag, uint32_t reason, char *ioctl_str, uint16_t ioctl_str_len);

/*!
 ******************************************************************************
 * Prints Hexdump and ASCII dump for data passed as argument.
 *
 * @param[in] data  The data which has to be converted into hex and ascii format.
 * @param[in] data_len The length of data.
 *
 * @result
 */
void whd_hexdump(uint8_t *data, uint32_t data_len);

extern wl_chanspec_t whd_channel_to_wl_band(whd_driver_t whd_driver, uint32_t channel);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif

