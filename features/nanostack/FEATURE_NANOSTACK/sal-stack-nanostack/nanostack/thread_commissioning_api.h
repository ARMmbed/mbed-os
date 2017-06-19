/*
 * Copyright (c) 2015-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

/**
 * \file thread_commissioning_api.h
 * \brief Thread commissioning API.
 *
 * This is a public API used for creating a commissioning device. You can create an on-mesh commissioner
 * and a native commissioner.
 */

#ifndef THREAD_COMMISSIONING_API_H_
#define THREAD_COMMISSIONING_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

#define TRACE_GROUP_THREAD_COMMISSIONING_API "TCoA"

typedef enum {
    COMMISSIONING_STATE_ACCEPT,
    COMMISSIONING_STATE_PENDING,
    COMMISSIONING_STATE_REJECT,
    COMMISSIONING_STATE_NO_NETWORK
} commissioning_state_e;

/** \brief Commissioning petition response callback.
 *
 * \param interface_id Network interface ID. The request comes from this ID.
 * \param state State of the commissioning.
 *
 * \return 0 success, other values failure.
 */
typedef int (thread_commissioning_status_cb)(int8_t interface_id, uint16_t commissioner_session_id, commissioning_state_e state);


/** \brief Register the commissioner interface.
 *
 * If the network interface is up, the commissioner functionality is started within the Thread network.
 * If there is no interface, the network needs to be scanned first. When the network is found you can add an insecure commissioner,
 * attach to it and start using a different communication method with the border router.
 *
 * \param interface_id Interface ID where the request was made.
 * \param PSKc Pre-shared key between the commissioner and the Thread network.
 * \param PSKc_len The length of the PSKc.
 *
 * \return 0 success, other values failure.
 */
int thread_commissioning_register(int8_t interface_id, uint8_t PSKc[16]);

/** \brief Unregister the commissioner interface.
 *
 * This cleans up all the commissioner data from the device and disconnects it from the Thread network if an insecure commissioner was used.
 *
 * \param interface_id Network interface ID. The request comes from this ID.
 *
 * \return 0 success, other values failure.
 */
int thread_commissioning_unregister(int8_t interface_id);

/** \brief Start the commissioning petition.
 *
 * If the commissioner is insecure, you need to scan the networks and select the Thread network where you want to be a commissioner.
 *
 * \param interface_id Network interface ID. The request comes from this ID.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 * \return 0 Indicates success.
 *
 * \return -1 The client needs to scan the network to become an insecure commissioner.
 * \return Any other value indicates other failures.
 */
int thread_commissioning_petition_start(int8_t interface_id, char *commissioner_id_ptr, thread_commissioning_status_cb *status_cb_ptr);

/** \brief Send petition keep alive.
 *
 * This function must be called in 40 second intervals. TODO rethink if this should be automatic
 *
 * \param interface_id Network interface ID. The request comes from this ID.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 success, other values failure.
 */
int thread_commissioning_petition_keep_alive(int8_t interface_id, commissioning_state_e state);

/** \brief Callback received when a new device is completing the joining process.
 *
 * The message may include the following meshcop TLV fields:
 * * State TLV
 * * Vendor Name TLV
 * * Vendor Model TLV
 * * Vendor SW Version TLV
 * * Vendor Data TLV
 * * Vendor Stack
 * * Version TLV
 * * Provisioning URL TLV
 *
 * \param interface_id Network interface ID. The request comes from this ID.
 * \param EUI64 The client identifier.
 * \param message_ptr A message including the meshcop TLV set. This message can be parsed using thread_meshcop_lib.h.
 * \param message_len The length of the message.
 *
 * \return 0 Device accepted.
 * \return Any other value, device rejected.
 */
typedef int (thread_commissioning_joiner_finalisation_cb)(int8_t interface_id, uint8_t EUI64[8], uint8_t *message_ptr, uint16_t message_len);

/** \brief Add a device to commission to the Thread network.
 *
 *
 * \param interface_id Network interface ID. The request comes from this ID.
 * \param short_eui64 A boolean value indicating that short EUI version is used for bloom filter generation.
 * \param EUI64 A pointer to EUI64 buffer.
 * \param PSKd_ptr A pointer to PSKd buffer.
 * \param PSKd_len PSKd string length, current validity check is 1-32 bytes.
 * \param cb_ptr A callback function indicating the result of the operation. Can be NULL if no result code needed.
 *
 * \return 0 success, other values failure
 */
int thread_commissioning_device_add(int8_t interface_id, bool short_eui64, uint8_t EUI64[8], uint8_t *PSKd_ptr, uint8_t PSKd_len, thread_commissioning_joiner_finalisation_cb *joining_device_cb_ptr);

/** \brief Delete a device to commission to the Thread network.
 *
 *
 * \param interface_id Network interface ID. The request comes from this ID.
 * \param EUI64 A pointer to EUI64 buffer.
 *
 * \return 0 success, other values failure.
 */
int thread_commissioning_device_delete(int8_t interface_id, uint8_t EUI64[8]);

/** \brief Get next added device details.
 *
 * \param ptr A pointer for internal looping. First, use NULL pointer, after that use return pointer.
 * \param interface_id Network interface ID. The request comes from this ID.
 * \param short_eui64 A boolean value indicating that short EUI version is used for bloom filter generation. Can be NULL when no result wanted.
 * \param EUI64 A pointer to EUI64 buffer. Can be NULL when no result wanted.
 * \param PSKd_ptr A pointer to PSKd buffer. Can be NULL when no result wanted.
 *
 * \return >NULL for next iteration.
 * \return NULL when end of list.
 */
void *thread_commission_device_get_next(void *ptr, int8_t interface_id, bool *short_eui64, uint8_t EUI64[8], uint8_t PSKd[32], uint8_t *PSKd_len);

/** Interfaces needed for native commissioner.
 * current design:
 *  - The application configures the interface to scan available Thread networks to join
 *      - Time passes and the user wants to start scanning for native commissioner networks.
 *  - The application configures the interface to begin native commissioner interface scans.
 *  - The application selects a network to connect to.
 *  - The stack connects to that network -> interface UP event sent.
 *  - The application starts using Commissioning API to send COMM_PET.req message triggering a DTLS handshake.
 *      - Commission API queries the leader address and native info and uses the one that works.
 *
 *
 */

typedef struct thread_commissioning_link_configuration {
    uint8_t name[16]; /**< Name of the Thread network. utf8 string nul terminated if shorter than 16. */
    uint8_t extented_pan_id[8]; /**< Extended PAN ID. */
    uint16_t panId; /**< Network ID. */
    uint8_t Protocol_id; /**< Current protocol ID. */
    uint8_t version; /**< Current protocol version. */
    uint8_t rfChannel; /**< Current RF channel. */
} thread_commissioning_link_configuration_s;

/** \brief Native commissioner network scan result callback.
 *
 * This callback is called when networks that allow native commissioner to join are found.
 * Pointers are valid during this call.
 *
 * \param interface Interface ID of this Thread instance.
 *
 */
typedef void thread_commissioning_native_select_cb(int8_t interface_id, uint8_t count, thread_commissioning_link_configuration_s *link_ptr );

/** \brief Native commissioner network scan start.
 *
 * Starts the network scan mode to find networks where the device can become a native commissioner.
 * This stops the normal Thread joining process and informs the application of available networks.
 *
 * \param interface Interface ID of this Thread instance.
 *
 * \return 0 success, other values failure.
 */
int thread_commissioning_native_commissioner_start(int8_t interface_id, thread_commissioning_native_select_cb *cb_ptr);

/** \brief Native commissioner network scan stop.
 *
 * Stops the network scan mode and continues the normal joining process.
 *
 * \param interface Interface ID of this Thread instance.
 *
 * \return 0 success, other values failure.
 */
int thread_commissioning_native_commissioner_stop(int8_t interface_id);

/** \brief Native commissioner connect.
 *
 * Connects to a specific Thread network to become an active native commissioner.
 *
 * This function can be called in any time. When the network scan is completed, the available native commissioner networks are listed
 * using the callback.
 *
 * If the connection fails, network scan keeps looking for a new network. After a successful connection, the interface up event is sent.
 * TODO do we need backup timers or blacklist if PSKc fails. who is responsible for triggering new scans?
 *
 * Matching of thread network is made using Network name, Xpanid, panid, TODO channel?? or not? gives channel flexibility
 *
 * \param interface Interface ID of this Thread instance.
 *
 * \return 0 success, other values failure.
 */
int thread_commissioning_native_commissioner_connect(int8_t interface_id, thread_commissioning_link_configuration_s *link_ptr);

/**
 *\brief Get the address of the native commissioner parent and the commissioning port for the connection.
 *
 * \param interface_id Network interface ID.
 * \param address_ptr A pointer to address buffer (16 bytes) for the commission messages.
 * \param port Return the port for the commissioner.
 * \param PSKc_ptr A pointer to return buffer for the PSKc (16 bytes) of this network instance.
 *
 * \return 0, address OK.
 * \return <0 fail.
 */
int thread_commissioning_native_commissioner_get_connection_info(int8_t interface_id, uint8_t *address_ptr, uint16_t *port);

/**
 * \brief Get the management instance ID from the commissioner interface.
 *
 * \param interface_id Network interface ID.
 *
 * \return > 0 Instance ID.
 * \return <= 0 fail.
 */
int8_t thread_commissioning_get_management_id(int8_t interface_id);

#ifdef __cplusplus
}
#endif

#endif /* THREAD_COMMISSIONING_API_H_ */
