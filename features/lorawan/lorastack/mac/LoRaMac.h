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

#include "lorawan/system/LoRaWANTimer.h"
#include "lorastack/phy/LoRaPHY.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "lorastack/mac/LoRaMacCommand.h"
#include "events/EventQueue.h"
#include "lorastack/mac/LoRaMacMlme.h"
#include "lorastack/mac/LoRaMacMcps.h"
#include "lorastack/mac/LoRaMacMib.h"
#include "lorastack/mac/LoRaMacChannelPlan.h"

class LoRaMac {

public:

    /**
     * Constructor
     */
    LoRaMac(LoRaWANTimeHandler &lora_time);

    /**
     * Destructor
     */
    ~LoRaMac();

    /**
     * @brief   LoRaMAC layer initialization
     *
     * @details In addition to the initialization of the LoRaMAC layer, this
     *          function initializes the callback primitives of the MCPS and
     *          MLME services. Every data field of \ref loramac_primitives_t must be
     *          set to a valid callback function.
     *
     * @param   primitives [in]   A pointer to the structure defining the LoRaMAC
     *                            event functions. Refer to \ref loramac_primitives_t.
     *
     * @param   phy [in]          A pointer to the selected PHY layer.
     *
     * @param   queue [in]        A pointer to the application provided EventQueue.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t initialize(loramac_primitives_t *primitives, LoRaPHY *phy,
                                events::EventQueue *queue);

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
     *          account and reports when the frame can be sent.
     *
     * @param   size     [in]    The size of the applicable payload to be sent next.
     * @param   tx_info  [out]   The structure \ref loramac_tx_info_t contains
     *                           information on the actual maximum payload possible
     *                           (according to the configured datarate or the next
     *                           datarate according to ADR), and the maximum frame
     *                           size, taking the scheduled MAC commands into account.
     *
     * @return  `lorawan_status_t` The status of the operation. When the parameters are
     *          not valid, the function returns \ref LORAWAN_STATUS_PARAMETER_INVALID.
     *          In case of a length error caused by the applicable payload in combination
     *          with the MAC commands, the function returns \ref LORAWAN_STATUS_LENGTH_ERROR.
     *          Please note that if the size of the MAC commands in the queue do
     *          not fit into the payload size on the related datarate, the LoRaMAC will
     *          omit the MAC commands.
     *          If the query is valid, and the LoRaMAC is able to send the frame,
     *          the function returns \ref LORAWAN_STATUS_OK.
     */
    lorawan_status_t query_tx_possible(uint8_t size, loramac_tx_info_t* tx_info);

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
    lorawan_status_t add_channel_plan(const lorawan_channelplan_t& plan);

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
    lorawan_status_t get_channel_plan(lorawan_channelplan_t& plan);

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

    /**
     * @brief   Get parameter values from MIB service.
     *
     * @details The MAC information base service to get the attributes of the LoRaMac layer.
     *
     *          The following code-snippet shows how to use the API to get the
     *          parameter `AdrEnable`, defined by the enumeration type
     *          \ref MIB_ADR.
     *
     * @code
     *
     * loramac_mib_req_confirm_t mib_get;
     * mib_get.type = MIB_ADR;
     *
     * if (mib_get_request_confirm(&mib_get) == LORAWAN_STATUS_OK) {
     *   // LoRaMAC updated the parameter mibParam.AdrEnable
     * }
     *
     * @endcode
     *
     * @param [in] mib_get    The MIB-GET request to perform. Refer to
     *                        \ref loramac_mib_req_confirm_t.
     *
     * @return  `lorawan_status_t` The status of the operation.
     *          The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t mib_get_request_confirm(loramac_mib_req_confirm_t *mib_get);

    /**
     * @brief   Set attributes for MAC layer using MIB service.
     *
     * @details The MAC information base service to set the attributes of the LoRaMac layer.
     *
     *          The following code-snippet shows how to use the API to set the
     *          parameter `adr_enable`, defined by the enumeration type
     *          \ref MIB_ADR.
     *
     * @code
     *
     * loramac_mib_req_confirm_t mib_set;
     * mib_set.Type = MIB_ADR;
     * mib_set.param.adr_enable = true;
     *
     * if (mib_set_request_confirm(&mib_set) == LORAWAN_STATUS_OK) {
     *   // LoRaMAC updated the parameter
     * }
     *
     * @endcode
     *
     * @param [in] mib_set    The MIB-SET request to perform. Refer to
     *                        \ref loramac_mib_req_confirm_t.
     *
     * @return  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_BUSY
     *          \ref LORAWAN_STATUS_SERVICE_UNKNOWN
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t mib_set_request_confirm(loramac_mib_req_confirm_t *mib_set);

    /**
     * @brief   Set forth an MLME request.
     *
     * @details The MAC layer management entity handles the management services. The
     *          following code-snippet shows how to use the API to perform a
     *          network join request.
     *
     * @code
     *
     * static uint8_t dev_eui[] =
     * {
     *   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
     * };
     * static uint8_t app_eui[] =
     * {
     *   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
     * };
     * static uint8_t app_key[] =
     * {
     *   0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
     *   0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
     * };
     *
     * loramac_mlme_req_t mlme_req;
     * mlme_req.Type = MLME_JOIN;
     * mlme_req.req.join.dev_eui = dev_eui;
     * mlme_req.req.join.app_eui = app_eui;
     * mlme_req.req.join.app_key = app_key;
     *
     * if (LoRaMacMlmeRequest(&mlme_req) == LORAWAN_STATUS_OK) {
     *   // Service started successfully. Waiting for the Mlme-Confirm event
     * }
     *
     * @endcode
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
     * loramac_mcps_req_t request;
     * request.type = MCPS_UNCONFIRMED;
     * request.req.unconfirmed.fport = 1;
     * request.req.unconfirmed.f_buffer = buffer;
     * request.req.unconfirmed.f_buffer_size = sizeof(buffer);
     *
     * if (mcps_request(&request) == LORAWAN_STATUS_OK) {
     *   // Service started successfully. Waiting for the MCPS-Confirm event
     * }
     *
     * @endcode
     *
     * @param [in] request    The MCPS request to perform.
     *                        Refer to \ref loramac_mcps_req_t.
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
    lorawan_status_t mcps_request(loramac_mcps_req_t *request);

    /**
     * @brief LoRaMAC layer provides its callback functions for
     *        PHY layer.
     *
     * @return Pointer to callback functions for radio events
     */
    radio_events_t *get_phy_event_handlers();

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
    lorawan_status_t send(loramac_mhdr_t *mac_hdr, uint8_t fport, void *fbuffer,
                          uint16_t fbuffer_size);

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
     * @brief Opens up a continuous RX 2 window. This is used for
     *        class c devices.
     */
    void open_continuous_rx2_window(void);

#if defined(LORAWAN_COMPLIANCE_TEST)
public: // Test interface

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
    lorawan_status_t LoRaMacSetTxTimer( uint32_t NextTxTime );

    /**
     * \brief   LoRaMAC stop tx timer.
     *
     * \details Stops the next tx timer.
     *
     * \retval  `lorawan_status_t` The status of the operation. The possible values are:
     *          \ref LORAWAN_STATUS_OK
     *          \ref LORAWAN_STATUS_PARAMETER_INVALID
     */
    lorawan_status_t LoRaMacStopTxTimer( );

    /**
     * \brief   Enabled or disables the reception windows
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] enable - Enabled or disables the reception windows
     */
    void LoRaMacTestRxWindowsOn( bool enable );

    /**
     * \brief   Enables the MIC field test
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] txPacketCounter - Fixed Tx packet counter value
     */
    void LoRaMacTestSetMic( uint16_t txPacketCounter );

    /**
     * \brief   Enabled or disables the duty cycle
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] enable - Enabled or disables the duty cycle
     */
    void LoRaMacTestSetDutyCycleOn( bool enable );

    /**
     * \brief   Sets the channel index
     *
     * \details This is a test function. It shall be used for testing purposes only.
     *          Changing this attribute may lead to a non-conformance LoRaMac operation.
     *
     * \param   [in] channel - Channel index
     */
    void LoRaMacTestSetChannel( uint8_t channel );

private:
    /**
     * Timer to handle the application data transmission duty cycle
     */
    timer_event_t tx_next_packet_timer;
#endif

private:
    /**
     * Function to be executed on Radio Tx Done event
     */
    void on_radio_tx_done(void);

    /**
     * This function prepares the MAC to abort the execution of function
     * on_radio_rx_done() in case of a reception error.
     */
    void prepare_rx_done_abort(void);

    /**
     * Function to be executed on Radio Rx Done event
     */
    void on_radio_rx_done(uint8_t *payload, uint16_t size, int16_t rssi,
                          int8_t snr);

    /**
     * Function executed on Radio Tx Timeout event
     */
    void on_radio_tx_timeout(void);

    /**
     * Function executed on Radio Rx error event
     */
    void on_radio_rx_error(void);

    /**
     * Function executed on Radio Rx Timeout event
     */
    void on_radio_rx_timeout(void);

    /**
     *Function executed on Resend Frame timer event.
     */
    void on_mac_state_check_timer_event(void);

    /**
     * Function executed on duty cycle delayed Tx  timer event
     */
    void on_tx_delayed_timer_event(void);

    /**
     * Function executed on first Rx window timer event
     */
    void on_rx_window1_timer_event(void);

    /**
     * Function executed on second Rx window timer event
     */
    void on_rx_window2_timer_event(void);

    /**
     * Function executed on AckTimeout timer event
     */
    void on_ack_timeout_timer_event(void);

    /**
     * Initializes and opens the reception window
     */
    void rx_window_setup(bool rx_continuous, uint32_t max_rx_window_time);

    /**
     * Validates if the payload fits into the frame, taking the datarate
     * into account.
     *
     * Please Refer to chapter 4.3.2 of the LoRaWAN specification, v1.0.2
     */
    bool validate_payload_length(uint8_t length, int8_t datarate, uint8_t fopts_len);

    /**
     * Prepares MAC frame on the behest of send() API.
     */
    lorawan_status_t prepare_frame(loramac_mhdr_t *mac_hdr,
                                   loramac_frame_ctrl_t *fctrl, uint8_t fport,
                                   void *fbuffer, uint16_t fbuffer_size);

    /**
     * Schedules a transmission on the behest of send() API.
     */
    lorawan_status_t schedule_tx(void);

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
     * Checks for Port validity.
     */
    bool is_fPort_allowed(uint8_t fPort);

    /**
     * Prototypes for ISR handlers
     */
    void handle_cad_done(bool cad);
    void handle_tx_done(void);
    void handle_rx_done(uint8_t *payload, uint16_t size, int16_t rssi,
                        int8_t snr);
    void handle_rx_error(void);
    void handle_rx_timeout(void);
    void handle_tx_timeout(void);
    void handle_fhss_change_channel(uint8_t cur_channel);
    void handle_rx1_timer_event(void);
    void handle_rx2_timer_event(void);
    void handle_ack_timeout(void);
    void handle_delayed_tx_timer_event(void);
    void handle_mac_state_check_timer_event(void);
    void handle_next_tx_timer_event(void);

private:
    /**
     * LoRa PHY layer object storage
     */
    LoRaPHY *lora_phy;

    /**
     * MAC command handle
     */
    LoRaMacCommand mac_commands;

    /**
     * MLME subsystem handle
     */
    LoRaMacMlme mlme;

    /**
     * MCPS subsystem handle
     */
    LoRaMacMcps mcps;

    /**
     * MCPS subsystem handle
     */
    LoRaMacMib mib;

    /**
     * Channel planning subsystem
     */
    LoRaMacChannelPlan channel_plan;

    /**
     * Timer subsystem handle
     */
    LoRaWANTimeHandler &_lora_time;

    /**
     * Central MAC layer data storage
     */
    loramac_protocol_params _params;

    /**
     * Radio event callback handlers for MAC
     */
    radio_events_t radio_events;

    /**
     * LoRaMac upper layer event functions
     */
    loramac_primitives_t *mac_primitives;

    /**
     * EventQueue object storage
     */
    events::EventQueue *ev_queue;
};

#endif // MBED_LORAWAN_MAC_H__
