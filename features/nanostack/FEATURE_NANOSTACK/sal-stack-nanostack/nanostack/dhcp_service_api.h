/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
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

#ifndef DHCP_SERVICE_API_H_
#define DHCP_SERVICE_API_H_

#include <ns_types.h>
/**
 * \file dhcp_service_api.h
 * \brief DHCP server connection interfaces
 *
 * \section dhcp-service DHCP Service Instance
 * - dhcp_service_init(), Initializes a DHCP service.
 * - dhcp_service_delete(), Removes the DHCP service.
 *
 * \section dhcp-msg DHCP Service Messages
 * - dhcp_service_send_req(), Sends out DHCP request messages.
 * - dhcp_service_send_resp(), Sends out DHCP response messages.
 *
 * \section dhcp-tim DHCP Service Timers (retry timers)
 * - dhcp_service_send_req(), Sends out DHCP request messages.
 * - dhcp_service_set_retry_timers(), Sets the retransmission parameters.
 * - dhcp_service_req_remove(), Stops retrying and retransmissions.
 * - dhcp_service_timer_tick(), Indicates if a timeout occurred.
 *
 */

/** Defines Debug Trace String for DHCP service */
#define DHCP_SERVICE_API_TRACE_STR "DHcS"

/*
 * Return values for callbacks
 */

/** Message belongs to someone else. */
#define RET_MSG_NOT_MINE 0
/** Message is handled. */
#define RET_MSG_ACCEPTED 1
/** Message is not the final one and needs to hold on a bit. */
#define RET_MSG_WAIT_ANOTHER -1
/** Message is unexpected or corrupted. */
#define RET_MSG_CORRUPTED -2

/** \name DHCP options */
///@{
#define TX_OPT_NONE                     0x00    /**< No options. */
#define TX_OPT_USE_SHORT_ADDR           0x01    /**< Use short addresses. */
#define TX_OPT_MULTICAST_HOP_LIMIT_64   0x02    /**< Use multicast hop limit of 64. */
///@}

typedef enum dhcp_instance_type
{
    DHCP_INSTANCE_CLIENT,
    DHCP_INSTANCE_SERVER
} dhcp_instance_type_e;

/**
 * \brief DHCP Service receive callback.
 *
 * When the DHCP service receives a DHCP message it will go through a list of registered DHCP services instances
 * until some instance acknowledges that the message belongs to it.
 * \param instance_id An instance of registered server.
 * \param msg_tr_id The message transaction ID.
 * \param msg_ptr An allocated message pointer. Should not deallocate unless RET_MSG_ACCEPTED returned (then responsibility of client).
 * \param msg_len The length of the message.
 *
 * Return values
 * \return RET_MSG_ACCEPTED - Message is handled.
 * \return RET_MSG_CORRUPTED - Message is corrupted.
 * \return RET_MSG_NOT_MINE - Message belongs to someone else.
 */

typedef int (dhcp_service_receive_req_cb)(uint16_t instance_id, uint32_t msg_tr_id, uint8_t msg_name, uint8_t *msg_ptr, uint16_t msg_len);

/**
 * \brief DHCP Service Message Response callback.
 *
 * When the DHCP service receives a response to a DHCP message, this callback receives it.
 *
 * \param instance_id An instance of a registered server.
 * \param ptr A pointer for the client object.
 * \param msg_ptr An allocated message pointer. Should not deallocate unless RET_MSG_ACCEPTED returned (then responsibility of client).
 * \param msg_len The length of the message.
 *
 * Return values
 * \return RET_MSG_ACCEPTED - Message is handled
 * \return RET_MSG_WAIT_ANOTHER - This message was not the last one for this transaction and a new reply is expected.
 */

typedef int (dhcp_service_receive_resp_cb)(uint16_t instance_id, void *ptr, uint8_t msg_name,  uint8_t *msg_ptr, uint16_t msg_len);


/**
 * \brief Initialize a new DHCP service instance.
 *
 * Creates and shares the socket for other DHCP services.
 *
 * \param interface_id Interface for the new DHCP instance.
 * \param instance_type The type of the new DHCP instance.
 * \param A callback function to receive DHCP messages.
 *
 * \return Instance ID that is used to identify the service.
 */

uint16_t dhcp_service_init(int8_t interface_id, dhcp_instance_type_e instance_type, dhcp_service_receive_req_cb *receive_req_cb);

/**
* \brief Deletes a server instance.
*
* Removes all data related to this instance.
*
* \param instance The instance ID of the registered server.
*/
void dhcp_service_delete(uint16_t instance);

/**
* \brief Sends a DHCP response message.
*
* \param msg_tr_id The message transaction ID.
* \param options Options for this request.
* \param msg_ptr An allocated message pointer. Should not deallocate unless RET_MSG_ACCEPTED returned (then responsibility of client).
* \param msg_len The length of the message.
*
* \return 0, if everything went fine.
* \return -1, if error occurred.
*/
int dhcp_service_send_resp(uint32_t msg_tr_id, uint8_t options, uint8_t *msg_ptr, uint16_t msg_len);


/**
 * \brief Sends DHCP request message.
 *
 * Service takes care of retransmissions.
 *
 * \param instance_id The instance ID of the registered server.
 * \param options Options for this request.
 * \param ptr A void pointer to the client object.
 * \param addr The address of the server.
 * \param msg_ptr An allocated message pointer. This pointer is the responsibility of the service after this call.
 * \param msg_len The length of the message.
 * \param receive_resp_cb Callback pointer
 *
 * \return Transaction ID of the DHCP transaction
 * \return 0, if error occurred.
 */
uint32_t dhcp_service_send_req(uint16_t instance_id, uint8_t options, void *ptr, const uint8_t addr[static 16], uint8_t *msg_ptr, uint16_t msg_len, dhcp_service_receive_resp_cb *receive_resp_cb);

/**
 * \brief Setting retransmission parameters.
 *
 * Sets the retransmission parameters for this transaction.
 *
 * \param msg_tr_id The message transaction ID.
 * \param timeout_init An initial timeout value.
 * \param timeout_max The maximum timeout value when initial timeout is doubled with every retry.
 * \param retrans_max The maximum number of retries after which an error is received.
 *
 */
void dhcp_service_set_retry_timers(uint32_t msg_tr_id, uint16_t timeout_init, uint16_t timeout_max, uint8_t retrans_max);

/**
 * \brief Stops transactions for a message (retransmissions).
 *
 * Clears off sending retransmissions for a particular message transaction by finding it via its message transaction ID.
 *
 * \param msg_tr_id The message transaction ID.
 *
 */
void dhcp_service_req_remove(uint32_t msg_tr_id);

/**
 * \brief Timer tick function for retransmissions.
 *
 * Retransmission timer ticks should be increased with 100ms interval, if necessary. One tick is one millisecond.
 *
 */
bool dhcp_service_timer_tick(uint16_t ticks);


#endif //DHCP_SERVICE_API_H_
