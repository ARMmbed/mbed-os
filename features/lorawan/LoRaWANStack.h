/**
 * \file      LoRaWANStack.h
 *
 * \brief     LoRaWAN stack layer implementation
 *
 * \copyright Revised BSD License, see LICENSE.TXT file include in the project
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \defgroup  LoRaWAN stack layer that controls MAC layer underneath
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LORAWANSTACK_H_
#define LORAWANSTACK_H_

#include <stdint.h>
#include "events/EventQueue.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"

#include "lorastack/mac/LoRaMac.h"
#include "system/LoRaWANTimer.h"
#include "system/lorawan_data_structures.h"
#include "LoRaRadio.h"

class LoRaWANStack: private mbed::NonCopyable<LoRaWANStack> {
private:
    /** End-device states.
     *
     */
    typedef enum device_states {
        DEVICE_STATE_NOT_INITIALIZED,
        DEVICE_STATE_INIT,
        DEVICE_STATE_JOINING,
        DEVICE_STATE_ABP_CONNECTING,
        DEVICE_STATE_JOINED,
        DEVICE_STATE_SEND,
        DEVICE_STATE_IDLE,
#if defined(LORAWAN_COMPLIANCE_TEST)
        DEVICE_STATE_COMPLIANCE_TEST,
#endif
        DEVICE_STATE_SHUTDOWN
    } device_states_t;

public:
    static LoRaWANStack& get_lorawan_stack();

    /** Binds radio driver to PHY layer.
     *
     * MAC layer is totally detached from the PHY layer so the stack layer
     * needs to play the role of an arbitrator. This API gets a radio driver
     * object from the application (via LoRaWANInterface), binds it to the PHY
     * layer and initialises radio callback handles which the radio driver will
     * use in order to report events.
     *
     * @param radio            LoRaRadio object, i.e., the radio driver
     *
     */
    void bind_radio_driver(LoRaRadio& radio);

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
    lorawan_status_t connect();

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
    lorawan_status_t connect(const lorawan_connect_t &connect);

    /** End device initialization.
     * @param queue            A pointer to an EventQueue passed from the application.
     * @return                 LORAWAN_STATUS_OK on success, a negative error code on failure.
     */
    lorawan_status_t initialize_mac_layer(events::EventQueue *queue);

    /** Sets all callbacks for the application.
     *
     * @param callbacks        A pointer to the structure carrying callbacks.
     * @return                 LORAWAN_STATUS_OK on success, a negative error code on failure.
     */
    lorawan_status_t set_lora_callbacks(lorawan_app_callbacks_t *callbacks);

    /** Adds channels to use.
     *
     * You can provide a list of channels with appropriate parameters filled
     * in. However, this list is not absolute. In some regions, a CF
     * list gets implemented by default, which means that the network can overwrite your channel
     * frequency settings right after receiving a Join Accept. You may try
     * to set up any channel or channels after that and if the channel requested
     * is already active, the request is silently ignored. A negative error
     * code is returned if there is any problem with parameters.
     *
     * You need to ensure that the base station nearby supports the channel or channels being added.
     *
     * If your list includes a default channel (a channel where Join Requests
     * are received) you cannot fully configure the channel parameters.
     * Either leave the channel settings to default or check your
     * corresponding PHY layer implementation. For example, LoRaPHYE868.
     *
     * @param  channel_plan     A list of channels or a single channel.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    lorawan_status_t add_channels(const lorawan_channelplan_t &channel_plan);

    /** Removes a channel from the list.
     *
     * @param channel_id        Index of the channel being removed
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    lorawan_status_t remove_a_channel(uint8_t channel_id);

    /** Removes a previously set channel plan.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    lorawan_status_t drop_channel_list();

    /** Gets a list of currently enabled channels .
     *
     * @param channel_plan      The channel plan structure to store final result.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    lorawan_status_t get_enabled_channels(lorawan_channelplan_t &channel_plan);

    /** Sets up a retry counter for confirmed messages.
     *
     * Valid only for confirmed messages. This API sets the number of times the
     * stack will retry a CONFIRMED message before giving up and reporting an
     * error.
     *
     * @param count             The number of retries for confirmed messages.
     *
     * @return                  LORAWAN_STATUS_OK or a negative error code.
     */
    lorawan_status_t set_confirmed_msg_retry(uint8_t count);

    /** Sets up the data rate.
     *
     * `set_datarate()` first verifies whether the data rate given is valid or not.
     * If it is valid, the system sets the given data rate to the channel.
     *
     * @param data_rate   The intended data rate, for example DR_0 or DR_1.
     *                    Note that the macro DR_* can mean different
     *                    things in different regions.
     *
     * @return            LORAWAN_STATUS_OK if everything goes well, otherwise
     *                    a negative error code.
     */
    lorawan_status_t set_channel_data_rate(uint8_t data_rate);

    /** Enables ADR.
     *
     * @param adr_enabled       0 ADR disabled, 1 ADR enabled.
     *
     * @return                  LORAWAN_STATUS_OK on success, a negative error
     *                          code on failure.
     */
    lorawan_status_t enable_adaptive_datarate(bool adr_enabled);

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
     * @param null_allowed      Internal use only. Needed for sending empty packet
     *                          having CONFIRMED bit on.
     *
     * @return                  The number of bytes sent, or
     *                          LORAWAN_STATUS_WOULD_BLOCK if another TX is
     *                          ongoing, or a negative error code on failure.
     */
    int16_t handle_tx(uint8_t port, const uint8_t* data,
                      uint16_t length, uint8_t flags, bool null_allowed = false);

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
     *                          message is being received, for example:
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
    int16_t handle_rx(const uint8_t port, uint8_t* data,
                      uint16_t length, uint8_t flags);

    /** Send Link Check Request MAC command.
     *
     *
     * This API schedules a Link Check Request command (LinkCheckReq) for the network
     * server and once the response, i.e., LinkCheckAns MAC command is received
     * from the Network Server, an event is generated.
     *
     * A callback function for the link check response must be set prior to using
     * this API, otherwise a LORAWAN_STATUS_PARAMETER_INVALID error is thrown.
     *
     * @return          LORAWAN_STATUS_OK on successfully queuing a request, or
     *                  a negative error code on failure.
     *
     */
    lorawan_status_t set_link_check_request();

    /** Removes link check request sticky MAC command.
     *
     * Any already queued request may still get entertained. However, no new
     * requests will be made.
     */
    void remove_link_check_request();

    /** Shuts down the LoRaWAN protocol.
     *
     * In response to the user call for disconnection, the stack shuts down itself.
     *
     * @return          LORAWAN_STATUS_DEVICE_OFF on successfully shutdown.
     */
    lorawan_status_t shutdown();

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
    lorawan_status_t set_device_class(const device_class_t& device_class);

private:
    LoRaWANStack();
    ~LoRaWANStack();

    /**
     * Checks if the user provided port is valid or not
     */
    bool is_port_valid(uint8_t port);

    /**
     * State machine for stack controller layer.
     */
    lorawan_status_t lora_state_machine(device_states_t new_state);

    /**
     * Callback function for MLME indication. Mac layer calls this function once
     * an MLME indication is received. This method translates Mac layer data
     * structure into stack layer data structure.
     */
    void mlme_indication_handler(loramac_mlme_indication_t *mlmeIndication);

    /**
     * Handles an MLME confirmation coming from the Mac layer and uses it to
     * update the state for example, a Join Accept triggers an MLME confirmation,
     * that eventually comes here and we take necessary steps accordingly.
     */
    void mlme_confirm_handler(loramac_mlme_confirm_t *mlme_confirm);

    /**
     * Handles an MCPS confirmation coming from the Mac layer in response to an
     * MCPS request. We take appropriate actions in response to the confirmation,
     * e.g., letting the application know that ack was not received in case of
     * a CONFIRMED message or scheduling error etc.
     */
    void mcps_confirm_handler(loramac_mcps_confirm_t *mcps_confirm);

    /**
     * Handles an MCPS indication coming from the Mac layer, e.g., once we
     * receive a packet from the Network Server, it is indicated to this handler
     * and consequently this handler posts an event to the application that
     * there is something available to read.
     */
    void mcps_indication_handler(loramac_mcps_indication_t *mcps_indication);

    /**
     * Sets up user application port
     */
    lorawan_status_t set_application_port(uint8_t port);

    /**
     * Handles connection internally
     */
    lorawan_status_t handle_connect(bool is_otaa);

private:

    LoRaMac _loramac;
    loramac_primitives_t LoRaMacPrimitives;

    device_states_t _device_current_state;
    lorawan_app_callbacks_t _callbacks;
    lorawan_session_t _lw_session;
    loramac_tx_message_t _tx_msg;
    loramac_rx_message_t _rx_msg;
    uint8_t _num_retry;
    uint8_t _app_port;
    bool _link_check_requested;

    events::EventQueue *_queue;

#if defined(LORAWAN_COMPLIANCE_TEST)

    /**
     * Used only for compliance testing
     */
    void compliance_test_handler(loramac_mcps_indication_t *mcps_indication);

    /**
     * Used only for compliance testing
     */
    lorawan_status_t send_compliance_test_frame_to_mac();

    compliance_test_t _compliance_test;
#endif
};

#endif /* LORAWANSTACK_H_ */
