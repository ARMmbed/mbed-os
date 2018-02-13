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
#include "lorawan/LoRaWANStack.h"
#include "lorawan/LoRaRadio.h"
#include "lorawan/LoRaWANBase.h"

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
     * @param ev_queue A pointer to EventQueue provided by the application.
     *
     * @return         0 on success, a negative error code on failure.
     */
    virtual lorawan_status_t initialize(events::EventQueue *ev_queue) ;

    /** Connect OTAA or ABP using Mbed-OS config system
     *
     * Connect by Over The Air Activation or Activation By Personalization.
     * You need to configure the connection properly via the Mbed OS configuration
     * system.
     *
     * When connecting via OTAA, the return code for success (LORAWAN_STATUS_CONNECT_IN_PROGRESS) is negative.
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
     * @return         LORAWAN_STATUS_OK or LORAWAN_STATUS_CONNECT_IN_PROGRESS
     *                 on success, or a negative error code on failure.
     */
    virtual lorawan_status_t connect();

    /** Connect OTAA or ABP with parameters
     *
     * All connection parameters are chosen by the user and provided in the
     * data structure passed down.
     *
     * When connecting via OTAA, the return code for success (LORAWAN_STATUS_CONNECT_IN_PROGRESS) is negative.
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
     * @return        LORAWAN_STATUS_OK or LORAWAN_STATUS_CONNECT_IN_PROGRESS,
     *                a negative error code on failure.
     */
    virtual lorawan_status_t connect(const lorawan_connect_t &connect);

    /** Disconnect the current session.
     *
     * @return         LORAWAN_STATUS_DEVICE_OFF on successfully shutdown.
     */
    virtual lorawan_status_t disconnect();

    /** Validate the connectivity with the network.
     *
     * Application may use this API to submit a request to the stack for
     * validation of its connectivity to a Network Server. Under the hood, this
     * API schedules a Link Check Request command (LinkCheckReq) for the network
     * server and once the response, i.e., LinkCheckAns MAC command is received
     * from the Network Server, user provided method is called.
     *
     * One way to use this API may be the validation of connectivity after a long
     * deep sleep. Mbed LoRaWANStack piggy-backs the MAC commands with data
     * frame payload so the application needs to try sending something and the Network
     * Server may respond during the RX slots.
     *
     * This API is usable only when the 'link_check_resp' callback is set by
     * the application. See add_lora_app_callbacks API. If the above mentioned
     * callback is not set, a LORAWAN_STATUS_PARAMETER_INVALID error is thrown.
     *
     * First parameter to callback function is the demodulation margin and
     * the second parameter is the number of gateways that successfully received
     * the last request.
     *
     * A 'Link Check Request' MAC command remains set for every subsequent
     * transmission, until/unless application explicitly turns it off using
     * remove_link_check_request() API.
     *
     * @return          LORAWAN_STATUS_OK on successfully queuing a request, or
     *                  a negative error code on failure.
     *
     */
    virtual lorawan_status_t add_link_check_request();

    /** Removes link check request sticky MAC command.
     *
     * Any already queued request may still get entertained. However, no new
     * requests will be made.
     */
    virtual void remove_link_check_request();

    /** Sets up a particular data rate
     *
     * `set_datarate()` first verifies whether the data rate given is valid or not.
     * If it is valid, the system sets the given data rate to the channel.
     *
     * @param data_rate   The intended data rate, for example DR_0 or DR_1.
     *                    Please note, that the macro DR_* can mean different
     *                    things in different regions.
     * @return            LORAWAN_STATUS_OK if everything goes well, otherwise
     *                    a negative error code.
     */
    virtual lorawan_status_t set_datarate(uint8_t data_rate);

    /** Enables adaptive data rate (ADR).
     *
     * The underlying LoRaPHY and LoRaMac layers handle the data rate automatically
     * for the user, based upon the radio conditions (network congestion).
     *
     * @return          LORAWAN_STATUS_OK or negative error code otherwise.
     */
    virtual lorawan_status_t enable_adaptive_datarate();

    /** Disables adaptive data rate.
     *
     * When adaptive data rate (ADR) is disabled, you can either set a certain
     * data rate or the MAC layer selects a default value.
     *
     * @return          LORAWAN_STATUS_OK or negative error code otherwise.
     */
    virtual lorawan_status_t disable_adaptive_datarate();

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
     * @return          LORAWAN_STATUS_OK or a negative error code.
     */
    virtual lorawan_status_t set_confirmed_msg_retries(uint8_t count);

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
     * Please note that this API can also be used to add a single channel to the
     * existing channel plan.
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
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t set_channel_plan(const lorawan_channelplan_t &channel_plan);

    /** Gets the channel plans from the LoRa stack.
     *
     * Once you have selected a particular PHY layer, a set of channels
     * is automatically activated. Right after connecting, you can use this API
     * to see the current plan. Otherwise, this API returns the channel
     * plan that you have set using `set_channel_plan()`.
     *
     * @param  channel_plan     The current channel plan information.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t get_channel_plan(lorawan_channelplan_t &channel_plan);

    /** Removes an active channel plan.
     *
     * You cannot remove default channels (the channels the base stations are listening to).
     * When a plan is abolished, only the non-default channels are removed.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t remove_channel_plan();

    /** Removes a single channel.
     *
     * You cannot remove default channels (the channels the base stations are listening to).
     *
     * @param    index          The channel index.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t remove_channel(uint8_t index);

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
     *                          LORAWAN_STATUS_WOULD_BLOCK if another TX is
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
     *                             ii)  Number of bytes written to user buffer.
     *                             iii) LORAWAN_STATUS_WOULD_BLOCK if there is
     *                                  nothing available to read at the moment.
     *                             iv)  A negative error code on failure.
     */
    virtual int16_t receive(uint8_t port, uint8_t* data, uint16_t length,
                            int flags);

    /** Add application callbacks to the stack.
       *
       * 'lorawan_app_callbacks' is a structure that holds pointers to the application
       * provided methods which are needed to be called in response to certain
       * requests. The structure is default constructed to set all pointers to NULL.
       * So if the user does not provide the pointer, a response will not be posted.
       * However, the 'lorawan_events' callback is mandatory to be provided as it
       * contains essential events.
       *
       * Events that can be posted to user via 'lorawan_events' are:
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
       * Other responses to certain standard requests are an item for the future.
       * For example, a link check request could be sent whenever the device tries
       * to send a message and if the network server responds with a link check resposne,
       * the stack notifies the application be calling the appropriate method. For example,
       * 'link_check_resp' callback could be used to collect a response for a link check
       * request MAC command and the result is thus transported to the application
       * via callback function provided.
       *
       * As can be seen from declaration, mbed::Callback<void(uint8_t, uint8_t)> *link_check_resp)
       * carries two parameters. First one is Demodulation Margin and the second one
       * is number of gateways involved in the path to network server.
       *
       * An example of using this API with a latch onto 'lorawan_events' could be:
       *
       * LoRaWANInterface lorawan(radio);
       * lorawan_app_callbacks cbs;
       * static void my_event_handler();
       *
       * int main()
       * {
       * lorawan.initialize(&queue);
       *  cbs.events = mbed::callback(my_event_handler);
       *  lorawan.add_app_callbacks(&cbs);
       *  lorawan.connect();
       * }
       *
       * static void my_event_handler(lora_events_t events)
       * {
       *  switch(events) {
       *      case CONNECTED:
       *          //do something
       *          break;
       *      case DISCONNECTED:
       *          //do something
       *          break;
       *      case TX_DONE:
       *          //do something
       *          break;
       *      default:
       *          break;
       *  }
       * }
       *
       * @param callbacks         A pointer to the structure containing application
       *                          callbacks.
       */
    virtual lorawan_status_t add_app_callbacks(lorawan_app_callbacks_t *callbacks);

private:
    bool _link_check_requested;
};

#endif /* LORAWANINTERFACE_H_ */
