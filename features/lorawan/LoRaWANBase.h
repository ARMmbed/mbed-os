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

#include "events/EventQueue.h"
#include "lorawan_types.h"

class LoRaWANBase {

public:
    /** Initialize the LoRa stack.
     *
     * You must call this before using the LoRa stack.
     *
     * @param queue A pointer to EventQueue provided by the application.
     *
     * @return         LORAWAN_STATUS_OK on success, a negative error code on
     *                 failure.
     */
    virtual lorawan_status_t initialize(events::EventQueue *queue) = 0;

    /** Connect OTAA or ABP by setup.
     *
     * Connect by Over The Air Activation or Activation By Personalization.
     * The connection type is selected at the setup.
     *
     * @return         LORAWAN_STATUS_OK on success, a negative error code on
     *                 failure.
     */
    virtual lorawan_status_t connect() = 0;

    /** Connect OTAA or ABP by parameters
     *
     * Connect by Over The Air Activation or Activation By Personalization.
     * The connection type is selected using the parameters.
     * You need to define the parameters in the main application.
     *
     * @param connect       Options how end-device will connect to gateway
     * @return              LORAWAN_STATUS_OK on success, negative error code
     *                      on failure
     */
    virtual lorawan_status_t connect(const lorawan_connect_t &connect) = 0;

    /** Disconnects the current session.
     *
     * @return         LORAWAN_STATUS_OK on success, a negative error code on failure.
     */
    virtual lorawan_status_t disconnect() = 0;

    /** Validate the connectivity with the network.
     *
     * Application may use this API to submit a request to the stack for
     * validation of its connectivity to a Network Server. Under the hood, this
     * API schedules a Link Check Request command (LinkCheckReq) for the network
     * server and once the response, i.e., LinkCheckAns MAC command is received
     * from the Network Server, user provided method is called.
     *
     * This API is usable only when the link check response is callback set by
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
    virtual lorawan_status_t add_link_check_request() = 0;

    /** Detaches Link Request MAC command.
     *
     * Removes sticky MAC command for link check request.
     */
    virtual void remove_link_check_request() = 0;

    /** Sets up a particular data rate of choice
     *
     * @param data_rate   Intended data rate e.g., DR_0, DR_1 etc.
     *                    Caution is advised as the macro DR_* can mean different
     *                    things while being in a different region.
     * @return            LORAWAN_STATUS_OK if everything goes well, otherwise
     *                    a negative error code.
     */
    virtual lorawan_status_t set_datarate(uint8_t data_rate) = 0;

    /** Enables adaptive data rate (ADR)
     *
     * Underlying LoRaPHY and LoRaMac layers handle the data rate automatically
     * for the user based upon radio conditions (network congestion).
     *
     * @return             LORAWAN_STATUS_OK on success, negative error code
     *                     on failure.
     */
    virtual lorawan_status_t enable_adaptive_datarate() = 0;

    /** Disables adaptive data rate
     *
     * When adaptive data rate (ADR) is disabled, user can either set a certain
     * data rate or the Mac layer will choose a default value.
     *
     * @return             LORAWAN_STATUS_OK on success, negative error code
     *                     on failure.
     */
    virtual lorawan_status_t disable_adaptive_datarate() = 0;

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
     * @return          LORAWAN_STATUS_OK or a negative error code
     */
    virtual lorawan_status_t set_confirmed_msg_retries(uint8_t count) = 0;

    /** Sets channel plan
     *
     * @param channel_plan  The defined channel plans to be set.
     * @return              0 on success, a negative error code on failure.
     */
    virtual lorawan_status_t set_channel_plan(const lorawan_channelplan_t &channel_plan) = 0;

    /** Gets the current channel plan.
     *
     * @param  channel_plan     The current channel information.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t get_channel_plan(lorawan_channelplan_t &channel_plan) = 0;

    /** Removes currently active channel plan
     *
     * Default channels (channels where Base Stations are listening) are not
     * allowed to be removed. So when a plan is abolished, only non-default
     * channels are removed.
     *
     * @return                  LORAWAN_STATUS_OK on success, negative error
     *                          code on failure
     */
    virtual lorawan_status_t remove_channel_plan() = 0;

    /** Removes a given single channel
     *
     * Default channels (channels where Base Stations are listening) are not
     * allowed to be removed.
     *
     * @param    index          The channel index
     *
     * @return                  LORAWAN_STATUS_OK on success, negative error
     *                          code on failure
     */
    virtual lorawan_status_t remove_channel(uint8_t index) = 0;

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
    virtual int16_t send(uint8_t port, const uint8_t *data,
                         uint16_t length, int flags) = 0;

    /** Receives a message from the Network Server on a specific port.
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
     *                             ii)  Number of bytes written to user buffer.
     *                             iii) LORAWAN_STATUS_WOULD_BLOCK if there is
     *                                  nothing available to read at the moment.
     *                             iv)  A negative error code on failure.
     */
    virtual int16_t receive(uint8_t port, uint8_t *data, uint16_t length, int flags) = 0;

    /** Receives a message from the Network Server from any port.
     *
     * @param data              A pointer to buffer where the received data will be
     *                          stored.
     *
     * @param length            The size of data in bytes
     *
     * @param port              Return the number of port to which message was received.
     *
     * @param flags             Return flags to determine what type of message was received.
     *                          MSG_UNCONFIRMED_FLAG = 0x01
     *                          MSG_CONFIRMED_FLAG = 0x02
     *                          MSG_MULTICAST_FLAG = 0x04
     *                          MSG_PROPRIETARY_FLAG = 0x08
     *
     * @return                  It could be one of these:
     *                             i)   0 if there is nothing else to read.
     *                             ii)  Number of bytes written to user buffer.
     *                             iii) LORAWAN_STATUS_WOULD_BLOCK if there is
     *                                  nothing available to read at the moment.
     *                             iv)  A negative error code on failure.
     */
    virtual int16_t receive(uint8_t *data, uint16_t length, uint8_t &port, int &flags) = 0;

    /** Add application callbacks to the stack.
     *
     * An example of using this API with a latch onto 'lorawan_events' could be:
     *
     * LoRaWANInterface lorawan(radio);
     * lorawan_app_callbacks_t cbs;
     * static void my_event_handler();
     *
     * int main()
     * {
     * lorawan.initialize();
     *  cbs.lorawan_events = mbed::callback(my_event_handler);
     *  lorawan.add_app_callbacks(&cbs);
     *  lorawan.connect();
     * }
     *
     * static void my_event_handler(lorawan_event_t event)
     * {
     *  switch(event) {
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
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    virtual lorawan_status_t add_app_callbacks(lorawan_app_callbacks_t *callbacks) = 0;

    /** Change device class
     *
     * Change current device class.
     *
     * @param    device_class   The device class
     *
     * @return                  LORAWAN_STATUS_OK on success,
     *                          LORAWAN_STATUS_UNSUPPORTED is requested class is not supported,
     *                          or other negative error code if request failed.
     */
    virtual lorawan_status_t set_device_class(device_class_t device_class) = 0;

    /** Get hold of TX meta-data
     *
     * Use this method to acquire any TX meta-data related to previous
     * transmission.
     * TX meta-data is only available right after the transmission is completed.
     * In other words, you can check for TX meta-data right after receiving the
     * TX_DONE event.
     *
     * @param    metadata    the inbound structure that will be filled if the meta-data
     *                       is available.
     *
     * @return               LORAWAN_STATUS_OK if the meta-data is available, otherwise
     *                       LORAWAN_STATUS_METADATA_NOT_AVAILABLE is returned.
     */
    virtual lorawan_status_t get_tx_metadata(lorawan_tx_metadata &metadata) = 0;

    /** Get hold of RX meta-data
     *
     * Use this method to acquire any RX meta-data related to current
     * reception.
     * RX meta-data is only available right after the reception is completed.
     * In other words, you can check for RX meta-data right after receiving the
     * RX_DONE event.
     *
     * @param    metadata    the inbound structure that will be filled if the meta-data
     *                       is available.
     *
     * @return               LORAWAN_STATUS_OK if the meta-data is available, otherwise
     *                       LORAWAN_STATUS_METADATA_NOT_AVAILABLE is returned.
     */
    virtual lorawan_status_t get_rx_metadata(lorawan_rx_metadata &metadata) = 0;

    /** Get hold of backoff time
     *
     * In the TX path, because of automatic duty cycling, the transmission is delayed
     * by a certain amount of time which is the backoff time. While the system schedules
     * application data to be sent, the application can inquire about how much time is
     * left in the actual transmission to happen.
     *
     * The system will provide you with a backoff time only if the application data is
     * in the TX pipe. If however, the event is already queued for the transmission, this
     * API returns a LORAWAN_STATUS_METADATA_NOT_AVAILABLE error code.
     *
     * @param    backoff    the inbound integer that will be carry the backoff time if it
     *                      is available.
     *
     * @return              LORAWAN_STATUS_OK if the meta-data regarding backoff is available,
     *                      otherwise LORAWAN_STATUS_METADATA_NOT_AVAILABLE is returned.
     *
     */
    virtual lorawan_status_t get_backoff_metadata(int &backoff) = 0;

    /** Cancel outgoing transmission
     *
     * This API is used to cancel any outstanding transmission in the TX pipe.
     * If an event for transmission is not already queued at the end of backoff timer,
     * the system can cancel the outstanding outgoing packet. Otherwise, the system is
     * busy sending and can't be held back.
     *
     * @return              LORAWAN_STATUS_OK if the sending is cancelled.
     *                      LORAWAN_STATUS_BUSY otherwise.
     *
     */
    virtual lorawan_status_t cancel_sending(void) = 0;
};

#endif /* LORAWAN_BASE_H_ */
