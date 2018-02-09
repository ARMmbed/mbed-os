/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRaWAN stack layer that controls both MAC and PHY underneath

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef LORAWANSTACK_H_
#define LORAWANSTACK_H_

#include <stdint.h>
#include "events/EventQueue.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "lorawan/system/LoRaWANTimer.h"
#include "lorastack/mac/LoRaMac.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "LoRaRadio.h"

#ifdef MBED_CONF_LORA_PHY
 #if MBED_CONF_LORA_PHY      == 0
  #include "lorawan/lorastack/phy/LoRaPHYEU868.h"
  #define LoRaPHY_region LoRaPHYEU868
 #elif MBED_CONF_LORA_PHY    == 1
  #include "lorawan/lorastack/phy/LoRaPHYAS923.h"
  #define LoRaPHY_region LoRaPHYAS923
 #elif MBED_CONF_LORA_PHY    == 2
  #include "lorawan/lorastack/phy/LoRaPHYAU915.h"
 #define LoRaPHY_region LoRaPHYAU915;
 #elif MBED_CONF_LORA_PHY    == 3
  #include "lorawan/lorastack/phy/LoRaPHYCN470.h"
  #define LoRaPHY_region LoRaPHYCN470
 #elif MBED_CONF_LORA_PHY    == 4
  #include "lorawan/lorastack/phy/LoRaPHYCN779.h"
  #define LoRaPHY_region LoRaPHYCN779
 #elif MBED_CONF_LORA_PHY    == 5
  #include "lorawan/lorastack/phy/LoRaPHYEU433.h"
  #define LoRaPHY_region LoRaPHYEU433
 #elif MBED_CONF_LORA_PHY    == 6
  #include "lorawan/lorastack/phy/LoRaPHYIN865.h"
  #define LoRaPHY_region LoRaPHYIN865
 #elif MBED_CONF_LORA_PHY    == 7
  #include "lorawan/lorastack/phy/LoRaPHYKR920.h"
  #define LoRaPHY_region LoRaPHYKR920
 #elif MBED_CONF_LORA_PHY    == 8
  #include "lorawan/lorastack/phy/LoRaPHYUS915.h"
  #define LoRaPHY_region LoRaPHYUS915
 #elif MBED_CONF_LORA_PHY    == 9
  #include "lorawan/lorastack/phy/LoRaPHYUS915Hybrid.h"
  #define LoRaPHY_region LoRaPHYUS915Hybrid
 #endif //MBED_CONF_LORA_PHY == VALUE
#else
 #error "Must set LoRa PHY layer parameters."
#endif //MBED_CONF_LORA_PHY

/**
 * A mask for the network ID.
 */
#define LORAWAN_NETWORK_ID_MASK                          ( uint32_t )0xFE000000

class LoRaWANStack: private mbed::NonCopyable<LoRaWANStack> {
public:
    static LoRaWANStack& get_lorawan_stack();

    /** Binds radio driver to PHY layer.
     *
     * MAC layer is totally detached from the PHY layer so the stack layer
     * needs to play the role of an arbitrator. This API gets a radio driver
     * object from the application (via LoRaWANInterface), binds it to the PHY
     * layer and returns MAC layer callback handles which the radio driver will
     * use in order to report events.
     *
     * @param radio            LoRaRadio object, i.e., the radio driver
     *
     * @return                 A list of callbacks from MAC layer that needs to
     *                         be passed to radio driver
     */
    radio_events_t *bind_radio_driver(LoRaRadio& radio);

    /** End device initialization.
     * @param queue            A pointer to an EventQueue passed from the application.
     * @return                 LORAWAN_STATUS_OK on success, a negative error code on failure.
     */
    lorawan_status_t initialize_mac_layer(events::EventQueue *queue);

    /** Sets all callbacks for the application.
     *
     * @param callbacks        A pointer to the structure carrying callbacks.
     */
    void set_lora_callbacks(lorawan_app_callbacks_t *callbacks);

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

    /** Commissions a LoRa device.
     *
     * @param commission_data   A structure representing all the commission
     *                          information.
     */
    void commission_device(const lorawan_dev_commission_t &commission_data);

    /** End device OTAA join.
     *
     * Based on the LoRaWAN standard 1.0.2.
     * Join the network using the Over The Air Activation (OTAA) procedure.
     *
     * @param  params           The `lorawan_connect_t` type structure.
     *
     * @return                  LORAWAN_STATUS_OK or
     *                          LORAWAN_STATUS_CONNECT_IN_PROGRESS on success,
     *                          or a negative error code on failure.
     */
    lorawan_status_t join_request_by_otaa(const lorawan_connect_t &params);

    /** End device ABP join.
     *
     * Based on the LoRaWAN standard 1.0.2.
     * Join the network using the Activation By Personalization (ABP) procedure.
     *
     * @param  params           The `lorawan_connect_t` type structure.
     *
     * @return                  LORAWAN_STATUS_OK or
     *                          LORAWAN_STATUS_CONNECT_IN_PROGRESS on success,
     *                          or a negative error code on failure.
     */
    lorawan_status_t activation_by_personalization(const lorawan_connect_t &params);

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
    int16_t handle_tx(uint8_t port, const uint8_t* data,
                      uint16_t length, uint8_t flags);

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

    /** Shuts down the LoRaWAN protocol.
     *
     * In response to the user call for disconnection, the stack shuts down itself.
     *
     * @return          LORAWAN_STATUS_DEVICE_OFF on successfully shutdown.
     */
    lorawan_status_t shutdown();

private:
    LoRaWANStack();
    ~LoRaWANStack();

    /**
     * Checks if the user provided port is valid or not
     */
    bool is_port_valid(uint8_t port);

    /**
     * State machine for stack controller layer.
     * Needs to be wriggled for every state change
     */
    lorawan_status_t lora_state_machine();

    /**
     * Sets the current state of the device.
     * Every call to set_device_state() should precede with
     * a call to lora_state_machine() in order to take the state change
     * in effect.
     */
    void set_device_state(device_states_t new_state);

    /**
     * Hands over the packet to Mac layer by posting an MCPS request.
     */
    lorawan_status_t send_frame_to_mac();

    /**
     * Callback function for MLME indication. Mac layer calls this function once
     * an MLME indication is received. This method translates Mac layer data
     * structure into stack layer data structure.
     */
    void mlme_indication_handler(loramac_mlme_indication_t *mlmeIndication);

    /**
     * Handles an MLME request coming from the upper layers and delegates
     * it to the Mac layer, for example, a Join request goes as an MLME request
     * to the Mac layer.
     */
    lorawan_status_t mlme_request_handler(loramac_mlme_req_t *mlme_request);

    /**
     * Handles an MLME confirmation coming from the Mac layer and uses it to
     * update the state for example, a Join Accept triggers an MLME confirmation,
     * that eventually comes here and we take necessary steps accordingly.
     */
    void mlme_confirm_handler(loramac_mlme_confirm_t *mlme_confirm);

    /**
     * Handles an MCPS request while attempting to hand over a packet from
     * upper layers to Mac layer. For example in response to send_frame_to_mac(),
     * an MCPS request is generated.
     */
    lorawan_status_t mcps_request_handler(loramac_mcps_req_t *mcps_request);

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
     * Sets a MIB request, i.e., update a particular parameter etc.
     */
    lorawan_status_t mib_set_request(loramac_mib_req_confirm_t *mib_set_params);

    /**
     * Requests the MIB to inquire about a particular parameter.
     */
    lorawan_status_t mib_get_request(loramac_mib_req_confirm_t *mib_get_params);

    /**
     * Sets up user application port
     */
    lorawan_status_t set_application_port(uint8_t port);

    /**
     * Helper function to figure out if the user defined data size is possible
     * to send or not. The allowed size for transmission depends on the current
     * data rate set for the channel. If its not possible to send user defined
     * packet size, this method returns the maximum possible size at the moment,
     * otherwise the user defined size is returned which means all of user data
     * can be sent.
     */
    uint16_t check_possible_tx_size(uint16_t size);

#if defined(LORAWAN_COMPLIANCE_TEST)
    /**
     * This function is used only for compliance testing
     */
    void prepare_special_tx_frame(uint8_t port);

    /**
     * Used only for compliance testing
     */
    void compliance_test_handler(loramac_mcps_indication_t *mcps_indication);

    /**
     * Used only for compliance testing
     */
    lorawan_status_t send_compliance_test_frame_to_mac();
#endif

    LoRaWANTimeHandler _lora_time;
    LoRaMac _loramac;
    LoRaPHY_region _lora_phy;
    loramac_primitives_t LoRaMacPrimitives;

#if defined(LORAWAN_COMPLIANCE_TEST)
    uint8_t compliance_test_buffer[MBED_CONF_LORA_TX_MAX_SIZE];
    compliance_test_t _compliance_test;
#endif

    device_states_t _device_current_state;
    lorawan_app_callbacks_t _callbacks;
    radio_events_t *_mac_handlers;
    lorawan_session_t _lw_session;
    loramac_tx_message_t _tx_msg;
    loramac_rx_message_t _rx_msg;
    uint8_t _app_port;
    uint8_t _num_retry;
    events::EventQueue *_queue;
    bool _duty_cycle_on;
};

#endif /* LORAWANSTACK_H_ */
