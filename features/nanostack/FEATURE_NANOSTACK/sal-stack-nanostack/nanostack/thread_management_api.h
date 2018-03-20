/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
 * \file thread_management_api.h
 * \brief Public API for managing Thread network configuration.
 *
 * This interface enables modification and reading the Thread network parameters.
 *
 * Thread management can modify the following values in the Thread network:
 * * [Network Name] (Read/write)
 * * [Security Policy] (Read/write)
 * * [Steering Data] (Read/write)
 * * [Commissioning Data Timestamp] (Read/write)
 * * [Commissioning Credential] PSKc (Write only)
 * * [Network Master Key] (Read only when policy allows)
 * * [Network Key Sequence] (Read only when policy allows)
 * * [Network Mesh-Local ULA] (Read only)
 * * [Border Router Locator] (Read only)
 * * [Commissioner Session ID] (Read only)
 * * [XPANID] (Read only)
 * * [PANID] (Read only)
 * * [Channel] (Read only)
 */

#ifndef THREAD_MANAGEMENT_API_H_
#define THREAD_MANAGEMENT_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

#define TRACE_GROUP_THREAD_MANAGEMENT_API "TMaA"  /**< trace group definition */

/** Thread management state
 */
typedef enum {
    MANAGEMENT_STATE_REJECT,
    MANAGEMENT_STATE_PENDING,
    MANAGEMENT_STATE_ACCEPT
} management_state_e;

/** \brief Register Thread management interface
 *
 * \param interface_id Interface ID of the Thread network.
 *
 * \return A handle for the management interface.
 *
 */
int thread_management_register(int8_t interface_id);

/** \brief Unregister Thread management interface
 *
 * \param instance_id ID of the management session.
 *
 */
int thread_management_unregister(int8_t instance_id);

/** \brief Callback to display the result of management set command.
 *
 * This callback is called when the server responds to the management set command.
 * This can fail if the leader rejects the request or the request times out.
 *
 * \param interface_id The interface ID of the Thread network.
 * \param status Result of the request.
 *
 */
typedef int (management_set_response_cb)(int8_t interface_id, management_state_e status);

 /** \brief Set the Thread security policy
 *
 * \param instance_id The ID of the management session.
 * \param options Security policy options:
 *    bit 8 Out-of-band commissioning restricted.
 *    bit 7 Native commissioner restricted.
 * \param rotation_time Thread key rotation time in hours.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
int thread_management_set_security_policy(int8_t instance_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr);

/** \brief Set the steering data
 *
 * Steering data can be either:
 *  - Empty to decline joining.
 *  - Any length with all bits set to 0 to decline joining.
 *  - Any length with all bits 1 to allow anyone to join.
 *  - Bloom filter to guide which device can join.
 *
 * If a Bloom filter is present it can be any length between 1-16 bytes. The first bit of the Bloom
 * filter indicates whether to use 0 == EUI-64 or 1 == bottom 24 bits of EUI-64.
 *
 * \param instance_id The ID of the management session.
 * \param session_id The commissioning session ID that needs to be added.
 * \param steering_data_ptr A pointer to new steering data.
 * \param steering_data_len The length of the new steering data.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
int thread_management_set_steering_data(int8_t instance_id, uint16_t session_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr);

/** \brief Set the Thread commissioning data timestamp
 *
 * \param instance_id The ID of the management session.
 * \param time Upper 48 bits is the timestamp in seconds since the start of unix time, lower 16 bits are fractional portion of time. If the last bit is set to 1, the commissioner has accurate time.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
int thread_management_set_commissioning_data_timestamp(int8_t instance_id, uint64_t time, management_set_response_cb *cb_ptr);

/** \brief Callback for reading Thread management information
 *
 * Result message containing the Thread management TLV message.
 * This message can be parsed with thread_meshcop_lib.h.
 *
 * \param instance_id The ID of the management session.
 * \param status Result of the request.
 * \param response_message_ptr A meshcop TLV structure pointer containing the requested TLVs.
 * \param response_message_len The length of the message.
 *
 * \return 0 Success.
 * \return <0 Fail.
 */
typedef int (management_get_response_cb)(int8_t instance_id, management_state_e status, uint8_t *response_message_ptr, uint16_t response_message_len);

/** \brief Get Thread management fields.
 * Fetching Thread management field values from any device in the Thread network.
 *
 * \param instance_id Instance ID of the management session.
 * \param dst_addr Destination address; the address of a remote device from whome it is desirable to fetch  management information. If however, the address is not provided, a request is sent to the leader of the network for this purpose. If a native commissioner is used, the request for management information is sent to the border router.
 * \param uri_ptr The ASCII string for the URI. This string identifies the CoAP URI for the desired resource. For example, /c/mg identifies the management get information resource.
 * \param fields_ptr A pointer that points to an array of desirable MESHCOP TLVs. A list of such TLVs can be found in thread_meshcop_lib.h
 * \param fields_count Number of fields in the field pointer array (set of TLVs).
 * \param cb_ptr A callback function carrying the result of the operation.
 *
 * \return 0, Success.
 * \return <0 Fail.
 */
int thread_management_get(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *fields_ptr, uint8_t fields_count, management_get_response_cb *cb_ptr);

/** \brief Set Thread management fields
 *
 * Set Thread management field values to a device in Thread network.
 *
 * \param instance_id Instance ID of the management session.
 * \param dst_addr Destination address, the address of a remote device where it is desired to setup management information. If however, the address is not provided, a request is sent to leader of the network for this purpose. If a  native commissioner is being used, the rquest for setting up management information is sent to the Border router.
 * \param uri_ptr The ASCII string for the URI. This string identifies the CoAP URI for the desired resource, for example, /c/ms identifies the the management set information resource.
 * \param data_ptr A pointer to the desired set of TLVs. 
 * \param data_len count of the members (no. of TLVs) in the TLV set.
 * \param cb_ptr A callback function carrying the result of the operation.
 *
 * \return 0, Success
 * \return <0 Fail.
 *
 */
int thread_management_set(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *data_ptr, uint8_t data_len, management_set_response_cb *cb_ptr);

#ifdef __cplusplus
}
#endif
#endif /* THREAD_MANAGEMENT_API_H_ */
