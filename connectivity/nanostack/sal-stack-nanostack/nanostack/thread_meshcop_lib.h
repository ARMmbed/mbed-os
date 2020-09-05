/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file thread_meshcop_lib.h
 * \brief Public API to handle the Thread management framework message parsing and building.
 *
 */

#ifndef THREAD_MESHCOP_LIB_H_
#define THREAD_MESHCOP_LIB_H_

#include "ns_types.h"

/**
 * TLV Types used in thread network
 */
/**
 * Network configuration TLV
 */
/** Channel TLV. */
#define MESHCOP_TLV_CHANNEL                                0
/** PANID TLV. */
#define MESHCOP_TLV_PANID                                  1
/** Extended PANID TLV. */
#define MESHCOP_TLV_XPANID                                 2
/** Network Name TLV. */
#define MESHCOP_TLV_NETWORK_NAME                           3
/** PSKc TLV. */
#define MESHCOP_TLV_PSKC                                   4
/** Network Master key TLV. */
#define MESHCOP_TLV_NETWORK_MASTER_KEY                     5
/** Network Key Sequence TLV. */
#define MESHCOP_TLV_NETWORK_KEY_SEQUENCE                   6
/** Mesh Local Ula prefix TLV. */
#define MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA                 7

/**
 * Network management TLV
 */
/** Steering data TLV. */
#define MESHCOP_TLV_STEERING_DATA                          8
/** Border router locator TLV. */
#define MESHCOP_TLV_BORDER_ROUTER_LOCATOR                  9
/** Commissioner ID TLV. */
#define MESHCOP_TLV_COMMISSIONER_ID                        10
/** Commissioner session ID TLV. */
#define MESHCOP_TLV_COMMISSIONER_SESSION_ID                11
/** Security Policy TLV. */
#define MESHCOP_TLV_SECURITY_POLICY                        12
/** GET TLV. */
#define MESHCOP_TLV_GET                                    13
/** Active Timestamp TLV. */
#define MESHCOP_TLV_ACTIVE_TIME_STAMP                      14
/** Commissioner UDP port TLV. */
#define MESHCOP_TLV_COMMISSIONER_UDP_PORT                  15
/** state TLV. */
#define MESHCOP_TLV_STATE                                  16
/** Joiner UDP port TLV. */
#define MESHCOP_TLV_JOINER_UDP_PORT                        18

/**
 * Network management TLV specific bit defines
 */
#define MESHCOP_TLV_ACTIVE_TIME_STAMP_U_BIT                 0x01

/**
 * Relay message TLV
 */
/** Joiner Encapsulation TLV. */
#define MESHCOP_TLV_JOINER_ENCAPSULATION                   17
/** Joiner UDP port TLV. */
#define MESHCOP_TLV_JOINER_UDP_PORT                        18
/** Joiner IID TLV. */
#define MESHCOP_TLV_JOINER_IID                             19
/** Joiner Router locator TLV. */
#define MESHCOP_TLV_JOINER_ROUTER_LOCATOR                  20
/** Key Exchange Key TLV. */
#define MESHCOP_TLV_JOINER_ROUTER_KEK                      21

/**
 * Application provisioning TLVs
 */
/** Provisioning URL TLV. */
#define MESHCOP_TLV_PROVISIONING_URL                       32
/** Vendr Name TLV. */
#define MESHCOP_TLV_VENDOR_NAME                            33
/** Vendor Model TLV. */
#define MESHCOP_TLV_VENDOR_MODEL                           34
/** Vendor SW version TLV. */
#define MESHCOP_TLV_VENDOR_SW_VERSION                      35
/** Vendor Data TLV. */
#define MESHCOP_TLV_VENDOR_DATA                            36
/** Vendor Stack Version TLV. */
#define MESHCOP_TLV_VENDOR_STACK_VERSION                   37

/**
 * Tunneling
 */
/** UDP encapsulation TLV. */
#define MESHCOP_TLV_UDP_ENCAPSULATION                      48
/** IPv6 address TLV. */
#define MESHCOP_TLV_IPV6_ADDRESS                           49
/** TMF Forwarding Port TLV. */
#define MESHCOP_TLV_TMF_FORWARDING_PORT                    50

/**
 * Thread management interface TLVs
 */
/** Pending Timestamp TLV. */
#define MESHCOP_TLV_PENDING_TIMESTAMP                      51
/** Delay timer for pending configuration TLV. */
#define MESHCOP_TLV_DELAY_TIMER                            52
/** Channel mask TLV. */
#define MESHCOP_TLV_CHANNEL_MASK                           53
/** Count TLV. */
#define MESHCOP_TLV_COUNT                                  54
/** Period TLV. */
#define MESHCOP_TLV_PERIOD                                 55
/** Duration TLV. */
#define MESHCOP_TLV_DURATION                               56
/** Energy List TLV. */
#define MESHCOP_TLV_ENERGY_LIST                            57

/** Thread 1.2 */

/** Timeout TLV. */
#define MESHCOP_TLV_TIMEOUT                                58

/** Thread domain name TLV. */
#define MESHCOP_TLV_DOMAIN_NAME                            59

/** Domain prefix TLV TLV.*/
#define MESHCOP_TLV_DOMAIN_PREFIX                          60

/** AE Steering Data TLV. */
#define MESHCOP_TLV_AE_STEERING_DATA                       61

/** NMKP Steering Data TLV. */
#define MESHCOP_TLV_NMKP_STEERING_DATA                     62

/** Commissioner Token TLV. */
#define MESHCOP_TLV_COMM_TOKEN                             63

/** Commissioner Signature TLV. */
#define MESHCOP_TLV_COMM_SIGNATURE                         64

/** NMKP Steering Data TLV. */
#define MESHCOP_TLV_AE_PORT                                65

/** NMKP Steering Data TLV. */
#define MESHCOP_TLV_NMKP_PORT                              66

/** Registrar IPV6 address. */
#define MESHCOP_TLV_REGISTRAR_IPV6_ADDRESS                 68


/** Discovery Request TLV. */
#define MESHCOP_TLV_DISCOVERY_REQUEST                      128
/** Discovery response TLV. */
#define MESHCOP_TLV_DISCOVERY_RESPONSE                     129

/**
 * Write array TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param length length of the data that is written in TLV.
 * \param data array for TLV value.
 *
 * \return pointer value for writing the next TLV.
 */
uint8_t *thread_meshcop_tlv_data_write(uint8_t *ptr, const  uint8_t type, const  uint16_t length, const uint8_t *data);

/**
 * Write header.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param length length of the data that is written in TLV.
 *
 * \return pointer value for writing the remaining bytes of network data.
 */
uint8_t *thread_meshcop_tlv_data_write_header(uint8_t *ptr, uint8_t type, uint16_t length);

/**
 * Write 1 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 *
 * \return pointer value for writing the next TLV.
 */
uint8_t *thread_meshcop_tlv_data_write_uint8(uint8_t *ptr, const uint8_t type, const uint8_t data);

/**
 * Write 2 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 *
 * \return pointer value for writing the next TLV.
 */
uint8_t *thread_meshcop_tlv_data_write_uint16(uint8_t *ptr, const uint8_t type, const uint16_t data);

/**
 * Write 4 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 *
 * \return pointer value for writing the next TLV.
 */
uint8_t *thread_meshcop_tlv_data_write_uint32(uint8_t *ptr, const uint8_t type, const uint32_t data);

/**
 * Write 8 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 *
 * \return pointer value for writing the next TLV.
 */
uint8_t *thread_meshcop_tlv_data_write_uint64(uint8_t *ptr, const uint8_t type, const uint64_t data);

/**
 * Check if TLV exists in the message.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message.
 * \param type Type of TLV searched.
 *
 * \return true if TLV is found.
 * \return false if TLV does not exist.
 */
bool thread_meshcop_tlv_exist(const uint8_t *ptr, const uint16_t length, const uint8_t type);

/**
 * Find TLV from message.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message.
 * \param type Type of TLV searched.
 * \param result_ptr Pointer value is given as result if length is positive. Can be NULL which only searches for the length.
 *
 * \return The length of the TLV data found
 * \return 0 if TLV is empty or no TLV found.
 * \return negative value indicates corrupted message.
 */
uint16_t thread_meshcop_tlv_find(const uint8_t *ptr, const uint16_t length, const uint8_t type, uint8_t **result_ptr);

/**
 * Get length of the TLV.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message.
 *
 * \return The length of the TLV data found
 * \return 0 if TLV is empty.
 * \return negative value indicates corrupted message or no TLV present.
 */
int16_t thread_meshcop_tlv_length(const uint8_t *ptr, uint16_t length);

/**
 * Get length of the TLV including the type and length field.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message.
 *
 * \return The length of the TLV data found
 * \return 0 if TLV is empty.
 * \return negative value indicates corrupted message or no TLV present.
 */
int16_t thread_meshcop_tlv_length_required(const uint8_t *ptr, uint16_t length);

/**
 * Go through TLV list in order.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message. Length value modified to remaining value or 0 when no more TLVs present
 *
 * \return ptr to the next TLV
 * \return NULL if no TLV found.
 */
const uint8_t *thread_meshcop_tlv_get_next(const uint8_t *ptr, uint16_t *length);

/**
 * Validate that required TLVs are present in the buffer.
 *
 * \param ptr TLV message buffer.
 * \param length Length of the TLV message buffer.
 * \param required_tlv_ptr buffer where the IDs of TLVs that are must be present
 * \param required_tlv_len Amount of TLVs
 *
 * \return amount of TLVs present in the buffer.
 */
bool thread_meshcop_tlv_list_present(const uint8_t *ptr, uint16_t length, const uint8_t *required_tlv_ptr, uint8_t required_tlv_len);

/**
 * Get list of TLVS included in the buffer.
 *
 * \param ptr TLV message buffer.
 * \param length Length of the TLV message buffer.
 * \param result_ptr [OUT] buffer where the IDs of TLVs are written. can be NULL when counting amount of TLVs
 * \param result_len [OUT] Amount of TLVs written in buffer
 *
 * \return amount of TLVs present in the buffer.
 */
uint16_t thread_meshcop_tlv_list_generate(const uint8_t *ptr, uint16_t length, uint8_t *result_ptr, uint16_t *result_len);

/**
 * Remove TLV from list of TLVs.
 *
 * \param tlv_ptr pointer to TLV List buffer.
 * \param tlv_len Length of the TLV list buffer.
 * \param tlv_type TLV that is removed from the List
 *
 * \return amount of TLVs present in the buffer.
 */
uint16_t thread_meshcop_tlv_list_remove(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type);

/**
 * Check if specific type is available in list of TLVs.
 *
 * \param list_ptr Pointer to TLV list buffer.
 * \param list_len Length of the TLV list buffer.
 * \param tlv_type TLV type to be find from the list
 *
 * \return true if type exists, false if type does not exist.
 */
bool thread_meshcop_tlv_list_type_available(const uint8_t *list_ptr, uint16_t list_len, uint8_t tlv_type);

/**
 * Find next TLV from message.
 *
 * \param tlv_ba TLV message buffer.
 * \param tlv_ba_length Length of the TLV message buffer.
 * \param tlv_id ID of the TLV to be searched.
 * \param found_tlv [IN] Pointer value is given as result if length is > 0. Can be NULL which only searches for the length.
 * \                [OUT] Pointer to previous TLV found
 *
 * \return The length of the TLV data found and found_tlv updated to point beginning of value field. 0 if TLV is not found.
 */
uint16_t thread_meshcop_tlv_find_next(uint8_t *tlv_ba, uint16_t tlv_ba_length, uint8_t tlv_id, uint8_t **found_tlv);

/**
 * Read 1 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 *
 * \return pointer length of TLV 1 if success. 0 means TLV not found or does not have any data.
 * \return Any other value indicates that TLV is not as expected.
 */
uint8_t thread_meshcop_tlv_data_get_uint8(const uint8_t *ptr, const uint16_t length, const uint8_t type, uint8_t *data_ptr);

/**
 * Read 2 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 *
 * \return pointer length of TLV 2 if success. 0 means TLV not found or does not have any data.
 * \return Any other value indicates that TLV is not as expected.
 */
uint8_t thread_meshcop_tlv_data_get_uint16(const uint8_t *ptr, const uint16_t length, const uint8_t type, uint16_t *data_ptr);

/**
 * Read 4 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 *
 * \return pointer length of TLV 4 if success. 0 means TLV not found or does not have any data.
 * \return Any other value indicates that TLV is not as expected.
 */
uint8_t thread_meshcop_tlv_data_get_uint32(const uint8_t *ptr, const uint16_t length, const uint8_t type, uint32_t *data_ptr);

/**
 * Read 8 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 *
 * \return pointer length of TLV 4 if success. 0 means TLV not found or does not have any data.
 * \return Any other value indicates that TLV is not as expected.
 */
uint8_t thread_meshcop_tlv_data_get_uint64(const uint8_t *ptr, const uint16_t length, const uint8_t type, uint64_t *data_ptr);

#endif /* THREAD_MESHCOP_LIB_H_ */
