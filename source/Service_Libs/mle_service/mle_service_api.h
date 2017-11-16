/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#ifndef MLE_SERVICE_API_H_
#define MLE_SERVICE_API_H_

#include <ns_types.h>
#include "ns_list.h"
#include "ccmLIB.h"

#define MLE_STANDARD_RESPONSE_DELAY 10
#define MLE_HALF_SECOND_MAX_DELAY 5
#define MLE_NO_DELAY 0
#define MLE_TOKEN_DELAY 10
#define MLE_TOKEN_BUFFER_MAX_NBR 10
#define MLE_TOKEN_BUFFER_MIN_NBR -10

/**
 * \file mle_service_api.h
 * \brief Mle service API for multiple MLE protocol user.
 *
 * MLE service interface configuration API
 *
 * - mle_service_interface_register(), Register interface for first user service will open socket
 * - mle_service_interface_unregister(), Unregister service after last user call socket will be closed
 * - mle_service_interface_registeration_validate(), Validate has interface registered service already
 * - mle_service_interface_receiver_handler_update(), Update registered message handler to new one
 * - mle_service_interface_tx_queue_clean(), Free interface active queued messages
 * - mle_service_interface_token_bucket_settings_set(), Sets token bucket settings
 *
 * MLE Security configuration API
 *
 * - mle_service_security_init(), allocate security class for interface
 * - mle_service_security_set_security_key(), Set security key to security class
 * - mle_service_security_set_frame_counter(), Update security class frame counter
 * - mle_service_security_key_trig() Indicate mle security class to switch default key
 *
 * MLE Security class get API
 *
 * - mle_service_security_default_key_get(). Get default key pointer
 * - mle_service_security_default_key_id_get(), Get default key id
 * - mle_service_security_next_key_get(), Get secondary or pending key pointer
 * - mle_service_security_next_key_id_get(), Get secondary or pending key id
 * - mle_service_security_level_get(), Get configured security level
 *
 * MLE message Data flow API
 *
 * - mle_service_msg_allocate(), Allocate message buffer and set default security params based on configuration
 * - mle_service_msg_free(), Release allocated buffer
 * - mle_service_message_tail_get(), Get more space for message end
 * - mle_service_get_data_pointer(), Get messages data pointer DO NOT USE FOR WRITE
 * - mle_service_get_payload_length(), Get messages payload length
 * - mle_service_get_payload_start_point(), Get pointer to message payload start
 * - mle_service_update_length(), Update messages data length by given number of added bytes
 * - mle_service_update_length_by_ptr(), Update messages data length by updated data pointer
 * - mle_service_msg_update_security_params(), Change messages default setup
 * - mle_service_set_msg_response_true(), Mark message response status when you know that
 * - mle_service_check_msg_response(), Verify messages response status (typically called at timeout call back)
 * - mle_service_trasnaction_buffer_get_for_response(), Validate received response for interface request
 * - mle_service_check_msg_sent(), Check if message has been sent to socket
 *
 * Messages TX API
 * - mle_service_set_packet_callback(), Set messages timeout call back
 * - mle_service_get_msg_destination_address_pointer(), Get messages destination address pointer for verify or write
 * - mle_service_set_msg_destination_address(), Set messages IPv6 destination address
 * - mle_service_set_msg_timeout_parameters(), Update messages timeout and delay parameters
 * - mle_service_send_message(), Send builded message with given timeout parameters
 * - mle_service_reject_message_build(), Build and send MLE link reject message to given destination address
 * - mle_service_set_msg_token_bucket_priority(), Sets message a priority over token bucket restrictions
 *
 * Service Messages timeout API
 * - mle_service_timer_tick(), This need to be call periodically and give time in 100ms ticks
 *
 */

typedef struct mle_security_header {
    unsigned securityLevel:3;
    unsigned KeyIdMode:2;
    uint32_t frameCounter;
    uint8_t Keysource[8];
    uint8_t KeyIndex;
    bool invalid_frame_counter;
} mle_security_header_t;


typedef enum mle_security_event {
    MLE_SEC_MAX_FRAME_COUNTER_REACHED = 0, /*!< MLE security counter reach max 0xffffffff value when device can't use same key anymore */
    MLE_SEC_KEY_UPDATE_NOTIFY, /*!< MLE security have trigger new default key */
    MLE_SEC_UNKNOWN_KEY /*!< MLE security have detect unknown key id*/
} mle_security_event_t;

#define MLE_ALLOCATED_PORT 19788    /* Mesh Link Establishment - draft */

/*
 * When MLE service receives MLE message it will parse Generic MLE header
 *
 */
typedef struct {
    uint8_t message_type; /*!< MLE message type */
    uint8_t *packet_src_address; /*!< Packet Source address */
    uint8_t *packet_dst_address; /*!< Packet Destination address */
    uint8_t *data_ptr; /*!< Data pointer */
    uint16_t data_length; /*!< Data Length */
    uint16_t src_pan_id;  /*!< Link source Pan-id */
    uint16_t dst_pan_id; /*!< Link destination Pan-id */
    uint8_t lqi; /*!< Received Link quality*/
    int8_t dbm; /*!< Received RSSI */
} mle_message_t;

/*
 * MLE message timeout and re transmission parameters
 *
 */
typedef struct {
uint16_t timeout_init; /*!< Define start timeout in seconds */
uint16_t timeout_max; /*!< Define max timeout time in seconds */
uint8_t retrans_max; /*!< Define max packet TX count */
uint8_t delay; /*!< 100ms Ticks for random delay */
} mle_message_timeout_params_t;

/**
 *  MLE service message receiver handler call back function pointer
 *
 *  All service user need to register this function pointer.
 *
 *  \param interface_id define interface id for receiver
 *  \param mle_msg mle message structure
 *  \param security_headers indicate messages security parameters
 */
typedef void (mle_service_receive_cb)(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers);

/**
 *  MLE service security bypass message receiver handler call back function pointer
 *
 *  All service user need to register this function pointer.
 *
 *  \param interface_id define interface id for receiver
 *  \param mle_msg mle message structure
 */
typedef void (mle_service_receive_security_bypass_cb)(int8_t interface_id, mle_message_t *mle_msg);

/**
 *  MLE service security key request by 32-bit key source
 *
 *  All service user need to register this function pointer if they support 32-bit key source.
 *
 *  \param interface_id define interface id for receiver
 *  \param keyId key identifier
 *  \param keySequnce 32-bit key source
 */
typedef uint8_t * (mle_service_key_request_by_counter_cb)(int8_t interface_id, uint8_t keyId, uint32_t keySequnce);

/**
 *  MLE service security event notify
 *
 *  \param interface_id define interface id for receiver
 *  \param event
 *  \param keyId key identifier
 *
 */
typedef uint8_t * (mle_service_security_notify_cb)(int8_t interface_id, mle_security_event_t event, uint8_t keyId);

/**
 *  MLE service message timeout call back
 *
 *  This function will be called when msg timeout happen
 *
 *  \param interface_id define interface id for receiver
 *  \param msgId key identifier
 *  \param usedAllRetries false when
 *
 *  \return true Continue packet retry
 *  \return false Stop TX process and
 *
 */
typedef bool (mle_service_message_timeout_cb)(int8_t interface_id, uint16_t msgId, bool usedAllRetries);

/*
 * Initialise server instance.
 * Creates and shares socket for other mle services.
 *
 * \param interface_id Registered services interface Id.
 * \param receive_cb Message RX handler.
 * \param mac64 Interface 64-bit MAC.
 * \param challengeLength challenge length
 * Return values
 * 0, Register OK.
 */
int mle_service_interface_register(int8_t interface_id, mle_service_receive_cb *receive_cb, uint8_t *mac64, uint8_t challengeLength);

/*
* Deletes server instance.
* Removes all data related to this instance
*
* \param interface_id interface Id.
*
*/
void mle_service_interface_unregister(int8_t interface_id);

/*
* Validate is this interface register service already.
*
* \param interface_id interface Id.
*
* \return true when register ready
* \return false when register not ready
*
*/
bool mle_service_interface_registeration_validate(int8_t interface_id);

/*
* Update Interface packet handler.
*
* Usefully function to switch message handler based on state machine
*
* \param interface_id interface Id.
* \param receive_cb new message handler function pointer
*
* \return 0 register OK
* \return < 0 Register fail
*
*/
int mle_service_interface_receiver_handler_update(int8_t interface_id, mle_service_receive_cb *receive_cb);

/*
* Update Interface security by pass handler.
*
* Usefully function to switch message handler based on state machine
*
* \param interface_id interface Id.
* \param receive_cb new message handler function pointer
*
* \return 0 register OK
* \return < 0 Register fail
*
*/
int mle_service_interface_receiver_bypass_handler_update(int8_t interface_id, mle_service_receive_security_bypass_cb *receive_cb);

/**
* Delete and free all activated Transaction.
*
* \param interface_id interface Id.
*
*/
void mle_service_interface_tx_queue_clean(int8_t interface_id);

/**
* count all activated Transactions.
*
* \param interface_id interface Id.
*
* \return count of active buffers
*
*/

int mle_service_reset_frame_counters(int8_t interfaceId);

uint16_t mle_service_interface_tx_queue_size(int8_t interface_id);

/**
* Sets MLE token bucket settings
*
* Using of message tokens is optional.
*
* \param size Bucket size, 0 to 255, 0 means not in use
* \param rate Token rate, 0 to 255, 0 means not in use
* \param count, 0 to 255, 0 means not in use
*
* \return 0 Set is OK
* \return -1 Unknown interface id
* \return -2 Other error
*/
int mle_service_interface_token_bucket_settings_set(int8_t interface_id, uint8_t size, uint8_t rate, uint8_t count);

/*
 * MLE service interface security init.
 *
 * \param interface_id Id of registered server.
 * \param security_level valid values below:
 *  - AES_NO_SECURITY
 *  - AES_SECURITY_LEVEL_MIC32
 *  - AES_SECURITY_LEVEL_MIC64
 *  - AES_SECURITY_LEVEL_MIC128
 *  - AES_SECURITY_LEVEL_ENC
 *  - AES_SECURITY_LEVEL_ENC_MIC32
 *  - AES_SECURITY_LEVEL_ENC_MIC64
 *  - AES_SECURITY_LEVEL_ENC_MIC128
 *  \param security_frame_counter MLE security frame counter init value
 *  \param key_counter_req function pointer for request key by when key source is 32-bit
 *  \param notify function pointer for service notify security events
 *
 *  \return 0 Init OK
 *  \return -1 Unknown interface id
 *  \return -2 Un supported security level
 *
 */
int mle_service_security_init(int8_t interfaceId, uint8_t security_level,uint32_t security_frame_counter, mle_service_key_request_by_counter_cb * key_counter_req, mle_service_security_notify_cb *notify);

/*
 * SET MLE service interfac primary or pending primary security key.
 *
 * \param interface_id Id of registered server.
 * \param security_key pointer to 128-bit key
 * \param keyId key id.
 * \param primary true when set new primary and
 *
 */

bool mle_service_security_set_security_key(int8_t interfaceId, const uint8_t *security_key, uint8_t keyId, bool primary);

/*
 * SET MLE security frame counter
 *
 * \param interface_id Id of registered server.
 * \param frame_counter new frame counter
 *
 */
bool mle_service_security_set_frame_counter(int8_t interfaceId, uint32_t frame_counter);
/*
 * GET MLE security frame counter
 *
 * \param interface_id Id of registered server.
 *
 * \return Current frame counter
 *
 */
uint32_t mle_service_security_get_frame_counter(int8_t interfaceId);


/*
 * Notify MLE service interface for switch default security key.
 *
 * \param interface_id Id of registered server.
 * \param keyId Trigged key id.
 *
 */
bool mle_service_security_key_trig(int8_t interfaceId, uint8_t keyId);

/*
 * GET MLE security default key
 *
 * \param interface_id Id of registered server.
 *
 * \return > 0 Key pointer
 * \return NULL No key available
 */
uint8_t * mle_service_security_default_key_get(int8_t interfaceId);

/*
 * GET MLE security default key id
 *
 * \param interface_id Id of registered server.
 *
 * \return > 0 Key id
 * \return 0 not key id available or interface is not regostred
 */
uint8_t  mle_service_security_default_key_id_get(int8_t interfaceId);

/*
 * GET MLE security coming key pointer
 *
 * \param interface_id Id of registered server.
 *
 * \return > 0 Key id
 * \return 0 no available
 */
uint8_t * mle_service_security_next_key_get(int8_t interfaceId);

/**
 * GET MLE security coming key id
 *
 * \param interface_id Id of registered server.
 *
 * \return > 0 Pointer to key
 * \return NULL no available
 */
uint8_t  mle_service_security_next_key_id_get(int8_t interfaceId);

/*
 * GET MLE security security level
 *
 * \param interface_id Id of registered server.
 *
 * \return Security level
 */
uint8_t  mle_service_security_level_get(int8_t interfaceId);

/**
* Allocate MLE transaction buffer and set default security parameters to buffer.
*
* Configured security params from interface
*  * security level
*  * key id mode MAC_KEY_ID_MODE_IDX
* Service will automatically set next params
*  * Key id
*  * security frame counter
*
* \param interface_id interface Id.
* \param data_length Payload max length
* \param challengeAllocate True allocate challenge and write that to message automatically
* \param type message type
*
* \return > 0 Message id
* \return 0 Allocate fail
*
*/
uint16_t mle_service_msg_allocate(int8_t interface_id, uint16_t data_length, bool challengeAllocate, uint8_t type);

/**
* Free message.
*
* \param msgId buffer Id.
*
* \return  0 Free Valid
* \return -1 Unknown Id
*
*/
int mle_service_msg_free(uint16_t msgId);

/**
* Set new security level and Key id mode.
*
*
* \param msgId message id for update.
* \param security_level valid values below:
*  - AES_NO_SECURITY
*  - AES_SECURITY_LEVEL_MIC32
*  - AES_SECURITY_LEVEL_MIC64
*  - AES_SECURITY_LEVEL_MIC128
*  - AES_SECURITY_LEVEL_ENC
*  - AES_SECURITY_LEVEL_ENC_MIC32
*  - AES_SECURITY_LEVEL_ENC_MIC64
*  - AES_SECURITY_LEVEL_ENC_MIC128
*
* \param key_id_mode valid values below:
* - MAC_KEY_ID_MODE_IDX
* - MAC_KEY_ID_MODE_SRC4_IDX
*
* \return 0 Set OK
* \return -1 Unknown message id
* \return -2 Not supported parameters
*
*/
int mle_service_msg_update_security_params(uint16_t msgId, uint8_t security_level, uint8_t key_id_mode, uint32_t keysequence);

/**
* MLE message tail get for new payload.
*
* \param msgId message id.
* \param tail_length define messages new need tail length
*
* \return 0 tail allocated  OK
* \return -1 tail allocate fail
* \return -2 unknown message id
*
*/
int mle_service_message_tail_get(uint16_t msgId, uint16_t tail_length);

/**
* Get MLE message data pointer for write.
*
* \param msgId message id.
*
* \return >0 Valid pointer for write
* \return NULL Not valid message id
*
*/
uint8_t * mle_service_get_data_pointer(uint16_t msgId);

/**
* Get MLE message pointer to message start
*
* \param msgId message id.
*
* \return >0 Valid pointer for read or verify
* \return NULL Not valid message id
*
*/
uint8_t * mle_service_get_payload_start_point(uint16_t msgId);

/**
* Get MLE message payload length
*
* \param msgId message id.
*
* \return length of payload
*
*/
uint16_t  mle_service_get_payload_length(uint16_t msgId);

/**
* Set MLE message end point by new tail.
*
* \param msgId message id.
* \param tail_length how many new byte added to message end
*
* \return 0 Length Update OK
* \return -1 Unknown message id
* \return -2 buffer overflow
*
*/
int mle_service_update_length(uint16_t msgId, uint16_t tail_length);

/**
* Set MLE message end point by new data end pointer.
*
* \param msgId message id.
* \param data_end_ptr new message end pointer
*
* \return 0 Length Update OK
* \return -1 Unknown message id
* \return -2 buffer overflow
*
*/
int mle_service_update_length_by_ptr(uint16_t msgId, uint8_t *data_end_ptr);

/**
* Get MLE message destination address pointer for write purpose.
*
* \param msgId message id.
*
* \return >0 Pointer get OK
* \return NULL message id not valid
*
*/
uint8_t * mle_service_get_msg_destination_address_pointer(uint16_t msgId);


/**
* Set MLE message destination address
*
* \param msgId message id.
* \param dstAddress message destination address
*
* \return 0 Set OK
* \return -1 message id not valid
* \return -2 address pointer is NULL
*
*/
int mle_service_set_msg_destination_address(uint16_t msgId, const uint8_t *dstAddress);

/**
* Set Messages link PAN
*
* \param msgId message id.
* \param panid Link pan-id for this packet
*
* \return 0 Set OK
* \return -1 message id not valid
*
*/
int mle_service_set_msg_panid(uint16_t msgId, uint16_t panid);

/**
* Set messages timeout parameters.
*
* Messages timeout parameters define messages TX count and init timeout and max timeout values is messages delayed.
* Delayed message will affect random time between 100-900ms
*
* \param msgId Message Id.
* \param timeout_params messages transmission parameters
*/
int mle_service_set_msg_timeout_parameters(uint16_t msgId, mle_message_timeout_params_t *timeout_params);

int mle_service_set_msg_rf_channel(uint16_t msgId, uint8_t channel);

int mle_service_set_msg_link_layer_security_mode(uint16_t msgId, bool use_key_id_mode_2);


/**
* Set MLE message response received state
*
* \param msgId message id.
*
* \return 0 Set OK
* \return -1 Unknown Id
*
*/
int mle_service_set_msg_response_true(uint16_t msgId);

/**
* Check MLE message response received state
*
* Usefully function at message timeout call back to check response status
*
* \param msgId message id.
*
* \return true message has received response
* \return false no response or unknow message id
*
*/
bool mle_service_check_msg_response(uint16_t msgId);

/*
* MLE active  transaction buffer id get by challenge data.
*
* \param responseData
* \param responseData True allocate challenge and write that to message automatically
* \param msgId pointer where message id can be saved
*
* \return true Response is valid and tied to this msgId
* \return false No active Tx buffer available for this response
*
*/
bool mle_service_transaction_buffer_get_for_response(uint8_t *responseData, uint16_t length, uint16_t *msgId);

/**
* Check if message has been sent to socket.
*
* Function can be used whether message has been sent to socket or
* has been rejected by token bucket.
*
* \param msgId message id.
*
* \return true message has been sent
* \return false message has not been sent
*
*/
bool mle_service_check_msg_sent(uint16_t msgId);

/**
* MLE message timeout handler set.
*
* \param msgId message id.
* \param cb message timeout call back
*
* \return 0 Callback set OK
* \return < 0 unknown message id
*
*/
int mle_service_set_packet_callback(uint16_t msgId, mle_service_message_timeout_cb *cb);


/**
* Sends MLE message.
*
* \param msgId Message Id.
*/
int mle_service_send_message(uint16_t msgId);

/**
 * Timer tick function.
 *
 * should be called in 100ms intervals if more time passes before call amount in 100ms units.
 *
 */
bool mle_service_timer_tick(uint16_t ticks);

/**
* Build and send standard link reject message.
*
* \param msgId Message Id.
* \param dstIpv6 rejected link
* \param force_unsecure true force force disbale security
*/
int mle_service_reject_message_build(int8_t interfaceId, uint8_t *dstIpv6, bool force_unsecure);

/**
* Sets message a priority over token bucket restrictions.
*
* \param msgId Message Id.
*
* \return 0 priority set OK
* \return < 0 unknown message id
*/
int mle_service_set_msg_token_bucket_priority(uint16_t msgId);

/**
* Activates MLE Frame counter checking for the interface.
*
* \param value set true for check activation.
*
*/
void mle_service_set_frame_counter_check(bool value);

/**
* Set 802.15.4 security option for fragmented MLE messages
*
* \param value set true to force security for fragmented packets.
* \param value set false to use default security for fragmented packets.
*
*/
void mle_service_set_fragmented_msg_ll_security(bool value);

/**
* Commands MLE service to accept message with invalid MLE frame counter.
*
* \param value set true to accept invalid frame counters
*
*/
void mle_service_set_accept_invalid_frame_counter(bool value);

#endif /* MLE_SERVICE_API_H_ */
