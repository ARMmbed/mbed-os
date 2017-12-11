/**
 * Copyright (c) 2017, Arm Limited and affiliates.
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


#ifndef LORAWAN_BASE_H_
#define LORAWAN_BASE_H_

#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"

class LoRaWANBase {

public:
    /** Initialize the LoRa stack.
     *
     * You must call this before using the LoRa stack.
     *
     * @param queue A pointer to EventQueue provided by the application.
     *
     * @return         LORA_MAC_STATUS_OK on success, a negative error code on
     *                 failure.
     */
    virtual lora_mac_status_t initialize(events::EventQueue *queue) = 0;

    /** Connect OTAA or ABP by setup.
     *
     * Connect by Over The Air Activation or Activation By Personalization.
     * The connection type is selected at the setup.
     *
     * @return         LORA_MAC_STATUS_OK on success, a negative error code on
     *                 failure.
     */
    virtual lora_mac_status_t connect() = 0;

    /** Connect OTAA or ABP by parameters
     *
     * Connect by Over The Air Activation or Activation By Personalization.
     * The connection type is selected using the parameters.
     * You need to define the parameters in the main application.
     *
     * @param connect       Options how end-device will connect to gateway
     * @return              LORA_MAC_STATUS_OK on success, negative error code
     *                      on failure
     */
    virtual lora_mac_status_t connect(const lorawan_connect_t &connect) = 0;

    /** Disconnects the current session.
     *
     * @return         LORA_MAC_STATUS_OK on success, a negative error code on failure.
     */
    virtual lora_mac_status_t disconnect() = 0;

    /** Sets up a particular data rate of choice
     *
     * @param data_rate   Intended data rate e.g., DR_0, DR_1 etc.
     *                    Caution is advised as the macro DR_* can mean different
     *                    things while being in a different region.
     * @return            LORA_MAC_STATUS_OK if everything goes well, otherwise
     *                    a negative error code.
     */
    virtual lora_mac_status_t set_datarate(uint8_t data_rate) = 0;

    /** Enables adaptive data rate (ADR)
     *
     * Underlying LoRaPHY and LoRaMac layers handle the data rate automatically
     * for the user based upon radio conditions (network congestion).
     *
     * @return             LORA_MAC_STATUS_OK on success, negative error code
     *                     on failure.
     */
    virtual lora_mac_status_t enable_adaptive_datarate() = 0;

    /** Disables adaptive data rate
     *
     * When adaptive data rate (ADR) is disabled, user can either set a certain
     * data rate or the Mac layer will choose a default value.
     *
     * @return             LORA_MAC_STATUS_OK on success, negative error code
     *                     on failure.
     */
    virtual lora_mac_status_t disable_adaptive_datarate() = 0;

    /** Sets up retry counter for confirmed messages
     *
     * Valid only for confirmed messages.
     *
     * Number of trials to transmit the frame, if the LoRaMAC layer did not
     * receive an acknowledgment. The MAC performs a data-rate adaptation,
     * according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
     * to the table on page 64.
     *
     * Note, that if the number of trials is set to 1 or 2, the MAC will not decrease
     * the datarate, in case the LoRaMAC layer did not receive an acknowledgment.
     *
     * @param count     number of retries for confirmed messages
     *
     * @return          LORA_MAC_STATUS_OK or a negative error code
     */
    virtual lora_mac_status_t set_confirmed_msg_retries(uint8_t count) = 0;

    /** Sets channel plan
     *
     * @param channel_plan  The defined channel plans to be set.
     * @return              0 on success, a negative error code on failure.
     */
    virtual lora_mac_status_t set_channel_plan(const lora_channelplan_t &channel_plan) = 0;

    /** Gets the current channel plan.
     *
     * @param  channel_plan     The current channel information.
     *
     * @return                  LORA_MAC_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lora_mac_status_t get_channel_plan(lora_channelplan_t &channel_plan) = 0;

    /** Removes currently active channel plan
     *
     * Default channels (channels where Base Stations are listening) are not
     * allowed to be removed. So when a plan is abolished, only non-default
     * channels are removed.
     *
     * @return                  LORA_MAC_STATUS_OK on success, negative error
     *                          code on failure
     */
    virtual lora_mac_status_t remove_channel_plan() = 0;

    /** Removes a given single channel
     *
     * Default channels (channels where Base Stations are listening) are not
     * allowed to be removed.
     *
     * @param    index          The channel index
     *
     * @return                  LORA_MAC_STATUS_OK on success, negative error
     *                          code on failure
     */
    virtual lora_mac_status_t remove_channel(uint8_t index) = 0;

    /** Send message to gateway
     *
     * @param port              The application port number. Port numbers 0 and 224
     *                          are reserved, whereas port numbers from 1 to 223
     *                          (0x01 to 0xDF) are valid port numbers.
     *                          Anything out of this range is illegal.
     *
     * @param data              A pointer to the data being sent. The ownership of the
     *                          buffer is not transferred. The data is copied to the
     *                          internal buffers.
     *
     * @param length            The size of data in bytes.
     *
     * @param flags             A flag used to determine what type of
     *                          message is being sent, for example:
     *
     *                          MSG_UNCONFIRMED_FLAG = 0x01
     *                          MSG_CONFIRMED_FLAG = 0x02
     *                          MSG_MULTICAST_FLAG = 0x04
     *                          MSG_PROPRIETARY_FLAG = 0x08
     *                          MSG_MULTICAST_FLAG and MSG_PROPRIETARY_FLAG can be
     *                          used in conjunction with MSG_UNCONFIRMED_FLAG and
     *                          MSG_CONFIRMED_FLAG depending on the intended use.
     *
     *                          MSG_PROPRIETARY_FLAG|MSG_CONFIRMED_FLAG mask will set
     *                          a confirmed message flag for a proprietary message.
     *                          MSG_CONFIRMED_FLAG and MSG_UNCONFIRMED_FLAG are
     *                          mutually exclusive.
     *
     *
     * @return                  The number of bytes sent, or
     *                          LORA_MAC_STATUS_WOULD_BLOCK if another TX is
     *                          ongoing, or a negative error code on failure.
     */
    virtual int16_t send(uint8_t port, const uint8_t* data,
                         uint16_t length, int flags) = 0;

    /** Receives a message from the Network Server.
     *
     * @param port              The application port number. Port numbers 0 and 224
     *                          are reserved, whereas port numbers from 1 to 223
     *                          (0x01 to 0xDF) are valid port numbers.
     *                          Anything out of this range is illegal.
     *
     * @param data              A pointer to buffer where the received data will be
     *                          stored.
     *
     * @param length            The size of data in bytes.
     *
     * @param flags             A flag is used to determine what type of
     *                          message is being sent, for example:
     *
     *                          MSG_UNCONFIRMED_FLAG = 0x01,
     *                          MSG_CONFIRMED_FLAG = 0x02
     *                          MSG_MULTICAST_FLAG = 0x04,
     *                          MSG_PROPRIETARY_FLAG = 0x08
     *
     *                          MSG_MULTICAST_FLAG and MSG_PROPRIETARY_FLAG can be
     *                          used in conjunction with MSG_UNCONFIRMED_FLAG and
     *                          MSG_CONFIRMED_FLAG depending on the intended use.
     *
     *                          MSG_PROPRIETARY_FLAG|MSG_CONFIRMED_FLAG mask will set
     *                          a confirmed message flag for a proprietary message.
     *
     *                          MSG_CONFIRMED_FLAG and MSG_UNCONFIRMED_FLAG are
     *                          not mutually exclusive, i.e., the user can subscribe to
     *                          receive both CONFIRMED AND UNCONFIRMED messages at
     *                          the same time.
     *
     * @return                  It could be one of these:
     *                             i)   0 if there is nothing else to read.
     *                             ii)  Number of bytes still pending to read.
     *                             iii) LORA_MAC_STATUS_WOULD_BLOCK if there is
     *                                  nothing available to read at the moment.
     *                             iv)  A negative error code on failure.
     */
    virtual int16_t receive(uint8_t port, uint8_t* data, uint16_t length,
                            int flags) = 0;

    /** Callback handler.
     *
     * Events that can be posted to user:
     *
     * CONNECTED            - When the connection is complete
     * DISCONNECTED         - When the protocol is shut down in response to disconnect()
     * TX_DONE              - When a packet is sent
     * TX_TIMEOUT,          - When stack was unable to send packet in TX window
     * TX_ERROR,            - A general TX error
     * TX_CRYPTO_ERROR,     - If MIC fails, or any other crypto relted error
     * TX_SCHEDULING_ERROR, - When stack is unable to schedule packet
     * RX_DONE,             - When there is something to receive
     * RX_TIMEOUT,          - Not yet mapped
     * RX_ERROR             - A general RX error
     *
     * @param cb         A pointer to the callback function.
     */
    virtual void lora_event_callback(mbed::Callback<void(lora_events_t)> cb) = 0;
};

#endif /* LORAWAN_BASE_H_ */
