/**
 * \file      LoRaMac.h
 *
 * \brief     LoRa MAC layer implementation
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
 * \defgroup  LORAMAC LoRa MAC layer implementation
 *            This module specifies the API implementation of the LoRaMAC layer.
 *            This is a placeholder for a detailed description of the LoRaMac
 *            layer and the supported features.
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef MBED_LORAWAN_MAC_H__
#define MBED_LORAWAN_MAC_H__

#include "events/EventQueue.h"

#include "lorastack/phy/loraphy_target.h"
#include "lorastack/phy/LoRaPHY.h"

#include "system/LoRaWANTimer.h"
#include "system/lorawan_data_structures.h"

#include "LoRaMacChannelPlan.h"
#include "LoRaMacCommand.h"
#include "LoRaMacCrypto.h"
#if MBED_CONF_RTOS_PRESENT
#include "rtos/Mutex.h"
#endif

#include "platform/ScopedLock.h"

class LoRaMac {

public:

    /**
     * Constructor
     */
    LoRaMac();

    /**
     * Destructor
     */
    ~LoRaMac();

    /**
     * @brief   LoRaMAC layer initialization
     *
     * @details Initializes the LoRaMAC layer,
     *
     *
     * @param   queue [in]        A pointer to the application provided EventQueue.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t initialize(events::EventQueue *queue);

    /**
     * @brief   Disconnect LoRaMac layer
     *
     * @details Cancels all outstanding requests and sets LoRaMac's
     *          internal state to idle.
     */
    void disconnect(void);

    /**
     * @brief   Queries the LoRaMAC whether it is possible to send the next frame with
     *          a given payload size. The LoRaMAC takes the scheduled MAC commands into
     *          account and returns corresponding value.
     *
     * @param   size     [in]    The size of the applicable payload to be sent next.
     *
     * @return  Size of the biggest packet that can be sent.
     *          Please note that if the size of the MAC commands in the queue do
     *          not fit into the payload size on the related datarate, the LoRaMAC will
     *          omit the MAC commands.
     */
    uint8_t get_max_possible_tx_size(uint8_t size);

    /**
     * @brief nwk_joined Checks if device has joined to network
     * @return True if joined to network, false otherwise
     */
    bool nwk_joined();

    /**
     * @brief set_nwk_joined This is used for ABP mode for which real joining does not happen
     * @param joined True if device has joined in network, false otherwise
     */
    void set_nwk_joined(bool joined);

    /**
     * @brief   Adds a channel plan to the system.
     *
     * @details Adds a whole channel plan or a single new channel if the plan
     *          contains only one channel and 'plan.nb_channels' is set to 1.
     *          Please note that this functionality is not available in all regions.
     *          Information on the allowed ranges is available at the
     *          LoRaWAN Regional Parameters V1.0.2rB.
     *
     * @param   plan [in]    A reference to application provided channel plan.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t add_channel_plan(const lorawan_channelplan_t &plan);

    /**
     * @brief   Removes a channel plan from the system.
     *
     * @details Removes the whole active channel plan except the 'Default Channels'.
     *          Please note that this functionality is not available in all regions.
     *          Information on the allowed ranges is available at the
     *          LoRaWAN Regional Parameters V1.0.2rB.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t remove_channel_plan();

    /**
     * @brief   Access active channel plan.
     *
     * @details Provides access to the current active channel plan.
     *
     * @param   plan [out]    A reference to application provided channel plan data
     *                        structure which will be filled in with active channel
     *                        plan.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t get_channel_plan(lorawan_channelplan_t &plan);

    /**
     * @brief   Remove a given channel from the active plan.
     *
     * @details Deactivates the given channel.
     *
     * @param id    Id of the channel.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t remove_single_channel(uint8_t id);

    /**
     * @brief   LoRaMAC multicast channel link service.
     *
     * @details Links a multicast channel into the linked list.
     *
     * @param [in] channel_param    The multicast channel parameters to link.
     *
     * @return  `lorawan_status_t` The  status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t multicast_channel_link(multicast_params_t *channel_param);

    /**
     * @brief   LoRaMAC multicast channel unlink service.
     *
     * @details Unlinks a multicast channel from the linked list.
     *
     * @param [in] channel_param    The multicast channel parameters to unlink.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t multicast_channel_unlink(multicast_params_t *channel_param);

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
    void bind_radio_driver(LoRaRadio &radio);

    /**
     * @brief Configures the events to trigger an MLME-Indication with
     *        a MLME type of MLME_SCHEDULE_UPLINK.
     */
    void set_mlme_schedule_ul_indication(void);

    /**
     * @brief Schedules the frame for sending.
     *
     * @details Prepares a full MAC frame and schedules it for physical
     *          transmission.
     *
     * @param [in] mac_hdr      MAC frame header field
     * @param [in] fport        Payload port
     * @param [in] fbuffer      MAC frame data buffer to be sent
     * @param [in] fbuffer_size MAC frame data buffer size
     *
     * @return status          Status of the operation. LORAWAN_STATUS_OK in case
     *                         of success and a negative error code in case of
     *                         failure.
     */
    lorawan_status_t send(loramac_mhdr_t *mac_hdr, const uint8_t fport,
                          const void *fbuffer, uint16_t fbuffer_size);

    /**
     * @brief Puts the system in continuous transmission mode
     *
     * @remark Uses the radio parameters set on the previous transmission.
     *
     * @param [in] timeout    Time in seconds while the radio is kept in continuous wave mode
     *
     * @return status          Status of the operation. LORAWAN_STATUS_OK in case
     *                         of success and a negative error code in case of
     *                         failure.
     */
    lorawan_status_t set_tx_continuous_wave(uint16_t timeout);

    /**
     * @brief Puts the system in continuous transmission mode
     *
     * @param [in] timeout     Time in seconds while the radio is kept in continuous wave mode
     * @param [in] frequency   RF frequency to be set.
     * @param [in] power       RF output power to be set.
     *
     * @return status          Status of the operation. LORAWAN_STATUS_OK in case
     *                         of success and a negative error code in case of
     *                         failure.
     */
    lorawan_status_t set_tx_continuous_wave1(uint16_t timeout, uint32_t frequency, uint8_t power);

    /**
     * @brief Resets MAC specific parameters to default
     */
    void reset_mac_parameters(void);

    /**
     * @brief get_default_tx_datarate Gets the default TX datarate
     * @return default TX datarate.
     */
    uint8_t get_default_tx_datarate();

    /**
     * @brief enable_adaptive_datarate Enables or disables adaptive datarate.
     * @param adr_enabled Flag indicating is adr enabled or disabled.
     */
    void enable_adaptive_datarate(bool adr_enabled);

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

    /**
     * @brief tx_ongoing Check whether a prepare is done or not.
     * @return True if prepare_ongoing_tx is called, false otherwise.
     */
    bool tx_ongoing();

    /**
     * @brief set_tx_ongoing Changes the ongoing status for prepared message.
     * @param ongoing The value indicating the status.
     */
    void set_tx_ongoing(bool ongoing);

    /**
     * @brief reset_ongoing_tx Resets _ongoing_tx_msg.
     * @param reset_pending If true resets pending size also.
     */
    void reset_ongoing_tx(bool reset_pending = false);

    /**
     * @brief prepare_ongoing_tx This will prepare (and override) ongoing_tx_msg.
     * @param port The application port number.
     * @param data A pointer to the data being sent. The ownership of the
     *             buffer is not transferred.
     * @param length The size of data in bytes.
     * @param flags A flag used to determine what type of
     *              message is being sent.
     * @param num_retries Number of retries for a confirmed type message
     * @return The number of bytes prepared for sending.
     */
    int16_t prepare_ongoing_tx(const uint8_t port, const uint8_t *data,
                               uint16_t length, uint8_t flags, uint8_t num_retries);

    /**
     * @brief send_ongoing_tx Sends the ongoing_tx_msg
     * @return LORAWAN_STATUS_OK or a negative error code on failure.
     */
    lorawan_status_t send_ongoing_tx(void);

    /**
     * @brief device_class Returns active device class
     * @return Device class in use.
     */
    device_class_t get_device_class() const;

    /**
     * @brief set_device_class Sets active device class.
     * @param device_class Device class to use.
     * @param ack_expiry_handler callback function to inform about ack expiry
     */
    void set_device_class(const device_class_t &device_class,
                          mbed::Callback<void(void)>ack_expiry_handler);

    /**
     * @brief opens a continuous RX2 window for Class C devices
     */
    void open_continuous_rx_window(void);

    /**
     * @brief setup_link_check_request Adds link check request command
     * to be put on next outgoing message (when it fits)
     */
    void setup_link_check_request();

    /**
     * @brief prepare_join prepares arguments to be ready for join() call.
     * @param params Join parameters to use, if NULL, the default will be used.
     * @param is_otaa True if joining is to be done using OTAA, false for ABP.
     *
     * @return LORAWAN_STATUS_OK or a negative error code on failure.
     */
    lorawan_status_t prepare_join(const lorawan_connect_t *params, bool is_otaa);

    /**
     * @brief join Joins the network.
     * @param is_otaa True if joining is to be done using OTAA, false for ABP.
     * @return LORAWAN_STATUS_OK or a negative error code on failure.
     */
    lorawan_status_t join(bool is_otaa);

    /**
     * MAC operations upon successful transmission
     */
    void on_radio_tx_done(lorawan_time_t timestamp);

    /**
     * MAC operations upon reception
     */
    void on_radio_rx_done(const uint8_t *const payload, uint16_t size,
                          int16_t rssi, int8_t snr);

    /**
     * MAC operations upon transmission timeout
     */
    void on_radio_tx_timeout(void);

    /**
     * MAC operations upon empty reception slots
     *
     * @param is_timeout false when radio encountered an error
     *                   true when the an RX slot went empty
     *
     * @return current RX slot
     */
    void on_radio_rx_timeout(bool is_timeout);

    /**
     * Handles retransmissions of Join requests if an Accept
     * was not received.
     *
     * @returns true if a retry will be made
     */
    bool continue_joining_process(void);

    /**
     * Checks if the CONFIRMED data can be sent again or not.
     */
    bool continue_sending_process(void);

    /**
     * Read-only access to MAC primitive blocks
     */
    const loramac_mcps_confirm_t *get_mcps_confirmation() const;
    const loramac_mcps_indication_t *get_mcps_indication() const;
    const loramac_mlme_confirm_t *get_mlme_confirmation() const;
    const loramac_mlme_indication_t *get_mlme_indication() const;

    /**
     * Post processing steps in response to actions carried out
     * by controller layer and Mac
     */
    void post_process_mcps_req(void);
    void post_process_mcps_ind(void);
    void post_process_mlme_request(void);
    void post_process_mlme_ind(void);

    /**
     * Set battery level query callback
     */
    void set_batterylevel_callback(mbed::Callback<uint8_t(void)> battery_level);

    /**
     * Returns the event ID of backoff timer.
     */
    int get_backoff_timer_event_id(void);

    /**
     * Clears out the TX pipe by discarding any outgoing message if the backoff
     * timer is still running.
     */
    lorawan_status_t clear_tx_pipe(void);

    /**
     * Gets the current time
     */
    lorawan_time_t get_current_time(void);

    /**
     * Gets the current receive slot
     */
    rx_slot_t get_current_slot(void);

    /**
     * These locks trample through to the upper layers and make
     * the stack thread safe.
     */
#if MBED_CONF_RTOS_PRESENT
    void lock(void)
    {
        osStatus status = _mutex.lock();
        MBED_ASSERT(status == osOK);
    }
    void unlock(void)
    {
        osStatus status = _mutex.unlock();
        MBED_ASSERT(status == osOK);
    }
#else
    void lock(void) { }
    void unlock(void) { }
#endif

private:
    typedef mbed::ScopedLock<LoRaMac> Lock;
#if MBED_CONF_RTOS_PRESENT
    rtos::Mutex _mutex;
#endif

    /**
     * Handles a Join Accept frame
     */
    void handle_join_accept_frame(const uint8_t *payload, uint16_t size);

    /**
     * Handles data frames
     */
    void handle_data_frame(const uint8_t *payload,  uint16_t size, uint8_t ptr_pos,
                           uint8_t msg_type, int16_t rssi, int8_t snr);

    /**
     * Send a Join Request
     */
    lorawan_status_t send_join_request();

    /**
     * Handles retransmissions
     */
    lorawan_status_t handle_retransmission();

    /**
     * Checks if the frame is valid
     */
    void check_frame_size(uint16_t size);

    /**
     * Performs MIC
     */
    bool message_integrity_check(const uint8_t *payload, uint16_t size,
                                 uint8_t *ptr_pos, uint32_t address,
                                 uint32_t *downlink_counter, const uint8_t *nwk_skey);

    /**
     * Decrypts and extracts data and MAC commands from the received encrypted
     * payload
     */
    void extract_data_and_mac_commands(const uint8_t *payload, uint16_t size,
                                       uint8_t fopts_len, uint8_t *nwk_skey,
                                       uint8_t *app_skey, uint32_t address,
                                       uint32_t downlink_frame_counter,
                                       int16_t rssi, int8_t snr);
    /**
     * Decrypts and extracts MAC commands from the received encrypted
     * payload if there is no data
     */
    void extract_mac_commands_only(const uint8_t *payload, int8_t snr, uint8_t fopts_len);

    /**
     * Callback function to be executed when the DC backoff timer expires
     */
    void on_backoff_timer_expiry(void);

    /**
     * At the end of an RX1 window timer, an RX1 window is opened using this method.
     */
    void open_rx1_window(void);

    /**
     * At the end of an RX2 window timer, an RX2 window is opened using this method.
     */
    void open_rx2_window(void);

    /**
     * A method to retry a CONFIRMED message after a particular time period
     * (ACK_TIMEOUT = TIME_IN_MS) if the ack was not received
     */
    void on_ack_timeout_timer_event(void);

    /*!
     * \brief Check if the OnAckTimeoutTimer has do be disabled. If so, the
     *        function disables it.
     *
     * \param [in] node_ack_requested Set to true, if the node has requested an ACK
     * \param [in] dev_class The device class
     * \param [in] ack_received Set to true, if the node has received an ACK
     * \param [in] ack_timeout_retries_counter Retries counter for confirmed uplinks
     * \param [in] ack_timeout_retries Maximum retries for confirmed uplinks
     */
    void check_to_disable_ack_timeout(bool node_ack_requested,
                                      device_class_t dev_class,
                                      bool ack_received,
                                      uint8_t ack_timeout_retries_counter,
                                      uint8_t ack_timeout_retries);

    /**
     * Validates if the payload fits into the frame, taking the datarate
     * into account.
     *
     * Please Refer to chapter 4.3.2 of the LoRaWAN specification, v1.0.2
     */
    bool validate_payload_length(uint16_t length, int8_t datarate, uint8_t fopts_len);

    /**
     * Prepares MAC frame on the behest of send() API.
     */
    lorawan_status_t prepare_frame(loramac_mhdr_t *mac_hdr,
                                   loramac_frame_ctrl_t *fctrl, const uint8_t fport,
                                   const void *fbuffer, uint16_t fbuffer_size);

    /**
     * Schedules a transmission on the behest of send() API.
     */
    lorawan_status_t schedule_tx();

    /**
     * Calculates the back-off time for the band of a channel.
     * Takes in the last used channel id as a parameter.
     */
    void calculate_backOff(uint8_t channel_id);

    /**
     * Hands over the MAC frame to PHY layer.
     */
    lorawan_status_t send_frame_on_channel(uint8_t channel);

    /**
     * Resets MAC primitive blocks
     */
    void reset_mcps_confirmation(void);
    void reset_mlme_confirmation(void);
    void reset_mcps_indication(void);

    /**
     * @brief set_tx_continuous_wave Puts the system in continuous transmission mode
     * @param [in] channel A Channel to use
     * @param [in] datarate A datarate to use
     * @param [in] tx_power A RF output power to use
     * @param [in] max_eirp A maximum possible EIRP to use
     * @param [in] antenna_gain Antenna gain to use
     * @param [in] timeout Time in seconds while the radio is kept in continuous wave mode
     */
    void set_tx_continuous_wave(uint8_t channel, int8_t datarate, int8_t tx_power,
                                float max_eirp, float antenna_gain, uint16_t timeout);

private:
    /**
     * Timer subsystem handle
     */
    LoRaWANTimeHandler _lora_time;

    /**
     * LoRa PHY layer object storage
     */
    LoRaPHY_region _lora_phy;

    /**
     * MAC command handle
     */
    LoRaMacCommand _mac_commands;

    /**
     * Channel planning subsystem
     */
    LoRaMacChannelPlan _channel_plan;

    /**
     * Crypto handling subsystem
     */
    LoRaMacCrypto _lora_crypto;

    /**
     * Central MAC layer data storage
     */
    loramac_protocol_params _params;

    /**
     * EventQueue object storage
     */
    events::EventQueue *_ev_queue;

    /**
     * Class C doesn't timeout in RX2 window as it is a continuous window.
     * We use this callback to inform the LoRaWANStack controller that the
     * system cannot do more retries.
     */
    mbed::Callback<void(void)> _ack_expiry_handler_for_class_c;

    /**
     * Structure to hold MCPS indication data.
     */
    loramac_mcps_indication_t _mcps_indication;

    /**
     * Structure to hold MCPS confirm data.
     */
    loramac_mcps_confirm_t _mcps_confirmation;

    /**
     * Structure to hold MLME indication data.
     */
    loramac_mlme_indication_t _mlme_indication;

    /**
     * Structure to hold MLME confirm data.
     */
    loramac_mlme_confirm_t _mlme_confirmation;

    loramac_tx_message_t _ongoing_tx_msg;

    bool _is_nwk_joined;

    bool _continuous_rx2_window_open;

    device_class_t _device_class;

#if defined(LORAWAN_COMPLIANCE_TEST)
public: // Test interface

    /**
     * @brief   Set forth an MLME request.
     *
     * @details The MAC layer management entity handles the management services.
     *
     * @param [in] request    The MLME request to perform.
     *                        Refer to \ref loramac_mlme_req_t.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     *          \ref LORAWAN_STATUS_NO_NETWORK_JOINED
     *          \ref LORAWAN_STATUS_LENGTH_ERROR
     *          \ref LORAWAN_STATUS_DEVICE_OFF
     */
    lorawan_status_t mlme_request(loramac_mlme_req_t *request);

    /**
     * @brief   Set forth an MCPS request.
     *
     * @details The MAC Common Part Sublayer handles the data services. The following
     *          code-snippet shows how to use the API to send an unconfirmed
     *          LoRaMAC frame.
     *
     * @code
     *
     * uint8_t buffer[] = {1, 2, 3};
     *
     * loramac_compliance_test_req_t request;
     * request.type = MCPS_UNCONFIRMED;
     * request.fport = 1;
     * request.f_buffer = buffer;
     * request.f_buffer_size = sizeof(buffer);
     *
     * if (test_request(&request) == LORAWAN_STATUS_OK) {
     *   // Service started successfully. Waiting for the MCPS-Confirm event
     * }
     *
     * @endcode
     *
     * @param [in] request    The test request to perform.
     *                        Refer to \ref loramac_compliance_test_req_t.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     *          \ref LORAWAN_STATUS_NO_NETWORK_JOINED
     *          \ref LORAWAN_STATUS_LENGTH_ERROR
     *          \ref LORAWAN_STATUS_DEVICE_OFF
     */
    lorawan_status_t test_request(loramac_compliance_test_req_t *request);

    /**
     * \brief   LoRaMAC set tx timer.
     *
     * \details Sets up a timer for next transmission (application specific timers).
     *
     * \param   [in] NextTxTime - Periodic time for next uplink.

     * \retval  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacSetTxTimer(uint32_t NextTxTime);

    /**
     * \brief   LoRaMAC stop tx timer.
     *
     * \details Stops the next tx timer.
     *
     * \retval  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacStopTxTimer();

    /**
     * \brief   Enabled or disables the reception windows
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] enable - Enabled or disables the reception windows
     */
    void LoRaMacTestRxWindowsOn(bool enable);

    /**
     * \brief   Enables the MIC field test
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] txPacketCounter - Fixed Tx packet counter value
     */
    void LoRaMacTestSetMic(uint16_t txPacketCounter);

    /**
     * \brief   Enabled or disables the duty cycle
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] enable - Enabled or disables the duty cycle
     */
    void LoRaMacTestSetDutyCycleOn(bool enable);

    /**
     * \brief   Sets the channel index
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] channel - Channel index
     */
    void LoRaMacTestSetChannel(uint8_t channel);

private:
    /**
     * Timer to handle the application data transmission duty cycle
     */
    timer_event_t tx_next_packet_timer;
#endif
};

#endif // MBED_LORAWAN_MAC_H__
