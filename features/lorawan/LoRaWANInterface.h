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

#ifndef LORAWANINTERFACE_H_
#define LORAWANINTERFACE_H_

#include "platform/Callback.h"
#include "netsocket/LoRaWANBase.h"
#include "lorawan/LoRaWANStack.h"
#include "netsocket/LoRaRadio.h"

class LoRaWANInterface: public LoRaWANBase {

public:

    /** Constructs a LoRaWANInterface using the LoRaWANStack instance underneath.
     *
     * Currently, LoRaWANStack is a singleton and you should only
     * construct a single instance of LoRaWANInterface.
     *
     */
    LoRaWANInterface(LoRaRadio& radio);
    virtual ~LoRaWANInterface();

    /** Initialize the LoRa stack.
     *
     * You must call this first to be able to use the LoRa stack.
     *
     * \return         0 on success, a negative error code on failure.
     */
    virtual lora_mac_status_t initialize();

    /** Connect OTAA or ABP using Mbed-OS config system
     *
     * Connect by Over The Air Activation or Activation By Personalization.
     * You need to configure the connection properly via the Mbed OS configuration
     * system.
     *
     * When connecting via OTAA, the return code for success (LORA_MAC_STATUS_CONNECT_IN_PROGRESS) is negative.
     * However, this is not a real error. It tells you that the connection is in progress and you will
     * be notified of the completion via an event. By default, after the Join Accept message
     * is received, base stations may provide the node with a CF-List that replaces
     * all user-configured channels except the Join/Default channels. A CF-List can
     * configure a maximum of five channels other than the default channels.
     *
     * In case of ABP, the CONNECTED event is posted before the call to `connect()` returns.
     * To configure more channels, we recommend that you use the `set_channel_plan()` API after the connection.
     * By default, the PHY layers configure only the mandatory Join channels. The retransmission back-off restrictions
     * on these channels are severe and you may experience long delays or even failures in the confirmed traffic.
     * If you add more channels, the aggregated duty cycle becomes much more relaxed as compared to the Join (default) channels only.
     *
     * **NOTES ON RECONNECTION:**
     * Currently, the Mbed OS LoRaWAN implementation does not support non-volatile
     * memory storage. Therefore, the state and frame counters cannot be restored after
     * a power cycle. However, if you use the `disconnect()` API to shut down the LoRaWAN
     * protocol, the state and frame counters are saved. Connecting again would try to
     * restore the previous session. According to the LoRaWAN 1.0.2 specification, the frame counters are always reset
     * to zero for OTAA and a new Join request lets the network server know
     * that the counters need a reset. The same is said about the ABP but there
     * is no way to convey this information to the network server. For a network
     * server, an ABP device is always connected. That's why storing the frame counters
     * is important, at least for ABP. That's why we try to restore frame counters from
     * session information after a disconnection.
     *
     * @return         LORA_MAC_STATUS_OK or LORA_MAC_STATUS_CONNECT_IN_PROGRESS
     *                 on success, or a negative error code on failure.
     */
    virtual lora_mac_status_t connect();

    /** Connect OTAA or ABP with parameters
     *
     * All connection parameters are chosen by the user and provided in the
     * data structure passed down.
     *
     * When connecting via OTAA, the return code for success (LORA_MAC_STATUS_CONNECT_IN_PROGRESS) is negative.
     * However, this is not a real error. It tells you that connection is in progress and you will
     * be notified of completion via an event. By default, after Join Accept message
     * is received, base stations may provide the node with a CF-List which replaces
     * all user-configured channels except the Join/Default channels. A CF-List can
     * configure a maximum of five channels other than the default channels.
     *
     * In case of ABP, the CONNECTED event is posted before the call to `connect()` returns.
     * To configure more channels, we recommend that you use the `set_channel_plan()` API after the connection.
     * By default, the PHY layers configure only the mandatory Join
     * channels. The retransmission back-off restrictions on these channels
     * are severe and you may experience long delays or even
     * failures in the confirmed traffic. If you add more channels, the aggregated duty
     * cycle becomes much more relaxed as compared to the Join (default) channels only.
     *
     * **NOTES ON RECONNECTION:**
     * Currently, the Mbed OS LoRaWAN implementation does not support non-volatile
     * memory storage. Therefore, the state and frame counters cannot be restored after
     * a power cycle. However, if you use the `disconnect()` API to shut down the LoRaWAN
     * protocol, the state and frame counters are saved. Connecting again would try to
     * restore the previous session. According to the LoRaWAN 1.0.2 specification, the frame counters are always reset
     * to zero for OTAA and a new Join request lets the network server know
     * that the counters need a reset. The same is said about the ABP but there
     * is no way to convey this information to the network server. For a network
     * server, an ABP device is always connected. That's why storing the frame counters
     * is important, at least for ABP. That's why we try to restore frame counters from
     * session information after a disconnection.
     *
     * @param connect  Options for an end device connection to the gateway.
     *
     * @return        LORA_MAC_STATUS_OK or LORA_MAC_STATUS_CONNECT_IN_PROGRESS,
     *                a negative error code on failure.
     */
    virtual lora_mac_status_t connect(const lorawan_connect_t &connect);

    /** Disconnect the current session.
     *
     * @return         LORA_MAC_STATUS_OK on success, a negative error code on
     *                 failure.
     */
    virtual lora_mac_status_t disconnect();

    /** Sets up a particular data rate
     *
     * `set_datarate()` first verifies whether the data rate given is valid or not.
     * If it is valid, the system sets the given data rate to the channel.
     *
     * @param data_rate   The intended data rate, for example DR_0 or DR_1.
     *                    Please note, that the macro DR_* can mean different
     *                    things in different regions.
     * @return            LORA_MAC_STATUS_OK if everything goes well, otherwise
     *                    a negative error code.
     */
    virtual lora_mac_status_t set_datarate(uint8_t data_rate);

    /** Enables adaptive data rate (ADR).
     *
     * The underlying LoRaPHY and LoRaMac layers handle the data rate automatically
     * for the user, based upon the radio conditions (network congestion).
     *
     * @return          LORA_MAC_STATUS_OK or negative error code otherwise.
     */
    virtual lora_mac_status_t enable_adaptive_datarate();

    /** Disables adaptive data rate.
     *
     * When adaptive data rate (ADR) is disabled, you can either set a certain
     * data rate or the MAC layer selects a default value.
     *
     * @return          LORA_MAC_STATUS_OK or negative error code otherwise.
     */
    virtual lora_mac_status_t disable_adaptive_datarate();

    /** Sets up the retry counter for confirmed messages.
     *
     * Valid for confirmed messages only.
     *
     * The number of trials to transmit the frame, if the LoRaMAC layer did not
     * receive an acknowledgment. The MAC performs a data rate adaptation as in
     * the LoRaWAN Specification V1.0.2, chapter 18.4, table on page 64.
     *
     * Note, that if number of retries is set to 1 or 2, MAC will not decrease
     * the datarate, if the LoRaMAC layer did not receive an acknowledgment.
     *
     * @param count     The number of retries for confirmed messages.
     *
     * @return          LORA_MAC_STATUS_OK or a negative error code.
     */
    virtual lora_mac_status_t set_confirmed_msg_retries(uint8_t count);

    /** Sets the channel plan.
     *
     * You can provide a list of channels with appropriate parameters filled
     * in. However, this list is not absolute. The stack applies a CF-List whenever
     * available, which means that the network can overwrite your channel
     * frequency settings right after Join Accept is received. You may try
     * to set up any channel or channels after that, and if the channel requested
     * is already active, the request is silently ignored. A negative error
     * code is returned if there is any problem with parameters.
     *
     * There is no reverse mechanism in the 1.0.2 specification for a node to request
     * a particular channel. Only the network server can initiate such a request.
     * You need to ensure that the corresponding base station supports the channel or channels being added.
     *
     * If your list includes a default channel (a channel where Join Requests
     * are received) you cannot fully configure the channel parameters.
     * Either leave the channel settings to default or check your
     * corresponding PHY layer implementation. For example, LoRaPHYE868.
     *
     * @param channel_plan      The channel plan to set.
     *
     * @return                  LORA_MAC_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lora_mac_status_t set_channel_plan(const lora_channelplan_t &channel_plan);

    /** Gets the channel plans from the LoRa stack.
     *
     * Once you have selected a particular PHY layer, a set of channels
     * is automatically activated. Right after connecting, you can use this API
     * to see the current plan. Otherwise, this API returns the channel
     * plan that you have set using `set_channel_plan()`.
     *
     * @param  channel_plan     The current channel plan information.
     *
     * @return                  LORA_MAC_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lora_mac_status_t get_channel_plan(lora_channelplan_t &channel_plan);

    /** Removes an active channel plan.
     *
     * You cannot remove default channels (the channels the base stations are listening to).
     * When a plan is abolished, only the non-default channels are removed.
     *
     * @return                  LORA_MAC_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lora_mac_status_t remove_channel_plan();

    /** Removes a single channel.
     *
     * You cannot remove default channels (the channels the base stations are listening to).
     *
     * @param    index          The channel index.
     *
     * @return                  LORA_MAC_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lora_mac_status_t remove_channel(uint8_t index);

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
    virtual int16_t send(uint8_t port, const uint8_t* data, uint16_t length,
                         int flags);

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
     * @param length            The size of data in bytes
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
                            int flags);

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
     * @param event_cb         A callback function for catching events from the stack.
     */
    virtual void lora_event_callback(mbed::Callback<void(lora_events_t)> event_cb);
};

#endif /* LORAWANINTERFACE_H_ */
