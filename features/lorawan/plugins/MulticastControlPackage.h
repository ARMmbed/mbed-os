/**
 * Copyright (c) 2019, Arm Limited and affiliates.
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

/** @addtogroup MulticastControlPackage
 * LoRaWAN Remote Multicast Setup Protocol
 *  @{
 */

#ifndef MBED_OS_FEATURES_LORAWAN_PLUGINS_MULTICASTCONTROLPACKAGE_H_
#define MBED_OS_FEATURES_LORAWAN_PLUGINS_MULTICASTCONTROLPACKAGE_H_

#include "lorawan_data_structures.h"

/**
 * MAX_CTRL_CMD_BUFFER_SIZE defines the maximum size needed for in-bound and
 * out-bound buffers. If all the srv_mcast_ctrl_cmds are combined to form one
 * packet, it will have the length equal to 75 bytes.
 */
#define MAX_CTRL_CMD_BUFFER_SIZE    75

/**
 * Multicast control command identifiers as defined in Remote Multicast Setup
 * Specification v1.0.0.
 */
typedef enum {
    MCAST_PACKAGE_VERSION           = 0x00,
    MCAST_GROUP_STATUS              = 0x01,
    MCAST_GROUP_SETUP               = 0x02,
    MCAST_GROUP_DELETE              = 0x03,
    MCAST_GROUP_CLASS_C_SESSION     = 0x04,
    MCAST_GROUP_CLASS_B_SESSION     = 0x05
} mcast_ctrl_cmds;

/**
 * Multicast session parameters
 */
typedef struct {
    /**
     * Multicast Group ID [0:3]. An end device can handle at max. 4 simultaneous
     * multicast sessions.
     */
    uint8_t mcast_group_id;

    /**
     * Device class associated with the session.
     */
    uint8_t mcast_session_class;

    /**
     * Length of time in seconds for which the session will remain active.
     */
    uint8_t session_timeout;

    /**
     * Data rate associated with the session
     */
    uint8_t data_rate;

    /**
     * Storage for Multicast Group Key, will be used to derive Multicast
     * network session key and Multicast application session key.
     */
    uint8_t mcast_key[16];

    /**
     * Storage for Multicast network session key.
     */
    uint8_t mcast_nwk_skey[16];

    /**
     * Storage for Multicast application session key.
     */
    uint8_t mcast_app_skey[16];

    /**
     * Multicast address assigned [4 bytes]. This address is common to all the
     * devices in this particular Multicast group identified by mcast_group_id.
     */
    uint32_t mcast_addr;

    /**
     * Time in seconds after which the session will commence
     */
    uint32_t session_time;

    /**
     * Downlink frequency used for the session
     */
    uint32_t dl_freq;

    /**
     * Frame counter, keeps track of Multicast downlinks for this group.
     */
    uint32_t mcast_fcnt;

    /**
     * A device may get added to an already active Multicast group. In such a
     * case the device must reject any downlink with frame counter less than
     * min_mcast_fcnt.
     */
    uint32_t min_mcast_fcnt;

    /**
     * Specifies the life-time the session. The device accept only
     * frames with counter less than max_mcast_fcnt and greater than min_mcast_fcnt.
     */
    uint32_t max_mcast_fcnt;
} lorawan_mcast_session_t;

/**
 * Multicast session context.
 */
typedef struct {
    lorawan_mcast_session_t mcast_session[MBED_CONF_LORA_MAX_MULTICAST_SESSIONS];
    uint8_t nb_active_sessions;
} lorawan_mcast_ctx_t;

/**
 * Contains response to the in-bound Multicast control commands.
 */
typedef struct {
    uint8_t *data;
    uint16_t size;
} mcast_ctrl_response_t;

/**
 * Multicast controller callbacks are a way to gather/provide information on-demand
 * in response to certain control commands, e.g., when a 'MCAST_GROUP_CLASS_C_SESSION'
 * setup request is received, the plug-in will use these callbacks to inquire or
 * notify the application as per case.
 */
typedef struct {
    /**
     * 'switch_class' callback tells the application which device class it should
     * switch to along-with the exact time when to switch and associated parameters
     * like, life-time, channel frequency and data rate.
     */
    mbed::Callback<void(uint8_t device_class,
                        uint32_t time_to_switch,
                        uint8_t life_time,
                        uint8_t dr,
                        uint32_t dl_freq)> switch_class;

    /**
     * 'check_params_validity' callback asks the application to help it with
     * verifying channel frequency and data rate associated with a session.
     * The application can use the public API 'verify_multicast_freq_and_dr(...)'
     * and feed in the response.
     */
    mbed::Callback<lorawan_status_t(uint8_t dr, uint32_t dl_freq)> check_params_validity;
} mcast_controller_cbs_t;

/** Multicast control plug-in
 * MulticastControlPackage is an implementation of LoRaWAN Remote Multicast Setup
 * Specification v1.0.0. It is presented as a plug-in to be used
 * with Mbed LoRaWAN stack.
 */
class MulticastControlPackage {
public:
    /**
     * CTOR
     */
    MulticastControlPackage();

    /**
     * DTOR
     */
    ~MulticastControlPackage();

    /** Activate the Multicast control package
     *
     * Multicast control package must be properly activated before use.
     *
     * @param gen_app_key    For LoRaWAN v1.0.X this is a pre-provisioned new
     *                       root key. For LoRaWAN v1.1.X gen_app_key is the AppKey.
     * @param key_len        Size of the key in bytes
     * @return LORAWAN_STATUS_OK if successful,
     *         LORAWAN_STATUS_PARAMETER_INVALID or
     *         LORAWAN_STATUS_CRYPTO_FAILURE in case of failure.
     */
    lorawan_status_t activate_multicast_control_package(const uint8_t *gen_app_key, uint32_t key_len);

    /** Parse and prepare response for the Multicast control commands
     *
     * When the application receives a message on Multicast control port 200, it
     * should trickle it down to the Multicast control package using this API for
     * interpretation of the commands and generation of appropriate responses.
     * All the uplink and downlink traffic on the Multicast control port 200 is
     * actually unicast. The specification forbids use of Multicast downlinks at
     * port 200. Alongwith the Multicast control commands payload and associated
     * size, the application provides access to the Multicast register maintained
     * by the LoRaWAN network stack and controller callbacks.
     *
     * By default the plug-in blocks overriding an existing Multicast session
     * by the network. However, application can choose to allow the network override
     * of locally created or previously activated sessions.
     *
     * @param payload          A pointer to the control command payload
     * @param size             Size of the payload buffer
     * @param mcast_register   A pointer to the Multicast register (allowed to be modified)
     * @param cbs              A pointer to 'mcast_controller_cbs_t' containing callbacks
     *                         which will come in handy while switching session or verifying
     *                         session parameters
     * @param network_override Defaults to false which prevents network to override existing
     *                         Multicast sessions. Set to true if the network is allowed to
     *                         override the sessions.
     *
     * @return A pointer to 'mcast_ctrl_response_t'which contains pointer to out-bound
     *         buffer and it's size. The ownership of the buffer is not transferred and
     *         the application should take a copy of the buffer if it is expecting a downlink
     *         before transmission of the response, e.g., in case Class C is active.
     */
    mcast_ctrl_response_t *parse(const uint8_t *payload, uint16_t size,
                                 lorawan_mcast_register_t *mcast_register,
                                 mcast_controller_cbs_t *cbs,
                                 bool network_override=false);

private:
    uint8_t _inbound_buf[MAX_CTRL_CMD_BUFFER_SIZE];
    uint8_t _outbound_buf[MAX_CTRL_CMD_BUFFER_SIZE];
    uint8_t _mc_kekey[16];
    lorawan_mcast_ctx_t _sessions_ctx;

    /**
     * AES computation context variable
     */
    mbedtls_aes_context aes_ctx;

    mcast_ctrl_response_t _resp;

    int mcast_crypt(const uint8_t *buffer, uint16_t size,
              const uint8_t *key, uint32_t key_length, uint8_t *dec_buffer, uint8_t mode=MBEDTLS_AES_ENCRYPT);
};

#endif /* MBED_OS_FEATURES_LORAWAN_PLUGINS_MULTICASTCONTROLPACKAGE_H_ */
/** @}*/
