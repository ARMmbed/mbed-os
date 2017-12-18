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

#include <string.h>
#include <stdlib.h>
#include "platform/Callback.h"
#include "events/EventQueue.h"
#include "lorawan/LoRaWANStack.h"
#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "LSTK"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#define tr_warn(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

#define INVALID_PORT                0xFF
#define MAX_CONFIRMED_MSG_RETRIES   255

#ifdef MBED_CONF_LORA_PHY
 #if MBED_CONF_LORA_PHY      == 0
  #include "lorawan/lorastack/phy/LoRaPHYEU868.h"
  static LoRaPHYEU868 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 1
  #include "lorawan/lorastack/phy/LoRaPHYAS923.h"
  static LoRaPHYAS923 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 2
  #include "lorawan/lorastack/phy/LoRaPHYAU915.h"
 static LoRaPHYAU915 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 3
  #include "lorawan/lorastack/phy/LoRaPHYCN470.h"
  static LoRaPHYCN470 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 4
  #include "lorawan/lorastack/phy/LoRaPHYCN779.h"
  static LoRaPHYCN779 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 5
  #include "lorawan/lorastack/phy/LoRaPHYEU433.h"
  static LoRaPHYEU433 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 6
  #include "lorawan/lorastack/phy/LoRaPHYIN865.h"
  static LoRaPHYIN865 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 7
  #include "lorawan/lorastack/phy/LoRaPHYKR920.h"
  static LoRaPHYKR920 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 8
  #include "lorawan/lorastack/phy/LoRaPHYUS915.h"
  static LoRaPHYUS915 lora_phy;
 #elif MBED_CONF_LORA_PHY    == 9
  #include "lorawan/lorastack/phy/LoRaPHYUS915Hybrid.h"
  static LoRaPHYUS915Hybrid lora_phy;
 #endif //MBED_CONF_LORA_PHY == VALUE
#else
 #error "Must set LoRa PHY layer parameters."
#endif //MBED_CONF_LORA_PHY

using namespace mbed;
using namespace events;

/**
 * Helper function prototypes
 */
static Mcps_t interpret_mcps_confirm_type(const lora_mac_mcps_t& local);
static Mib_t interpret_mib_req_confirm_type(const lora_mac_mib_t& mib_local);
static lora_mac_event_info_status_t interpret_event_info_type(const LoRaMacEventInfoStatus_t& remote);

#if MBED_CONF_LORA_PHY      == 0
#include "lorawan/lorastack/mac/LoRaMacTest.h"
#endif

/**
 *
 * User application data buffer size if compliance test is used
 */
#if (MBED_CONF_LORA_PHY == 0 || MBED_CONF_LORA_PHY == 4 || MBED_CONF_LORA_PHY == 6 || MBED_CONF_LORA_PHY == 7)
#define LORAWAN_COMPLIANCE_TEST_DATA_SIZE                  16
#elif (MBED_CONF_LORA_PHY == 1 || MBED_CONF_LORA_PHY == 2 || MBED_CONF_LORA_PHY == 8 || MBED_CONF_LORA_PHY == 9)
#define LORAWAN_COMPLIANCE_TEST_DATA_SIZE                  11
#else
#error "Must set LoRa PHY layer parameters."
#endif

/*****************************************************************************
 * Private Member Functions                                                  *
 ****************************************************************************/
bool LoRaWANStack::is_port_valid(uint8_t port)
{
    //Application should not use reserved and illegal port numbers.
    if (port >= 224 || port == 0) {
        return false;
    } else {
        return true;
    }
}

lora_mac_status_t LoRaWANStack::set_application_port(uint8_t port)
{
    if (is_port_valid(port)) {
        _app_port = port;
        return LORA_MAC_STATUS_OK;
    }

    return LORA_MAC_STATUS_PORT_INVALID;
}

/*****************************************************************************
 * Constructor and destructor                                                *
 ****************************************************************************/
LoRaWANStack::LoRaWANStack()
: _device_current_state(DEVICE_STATE_NOT_INITIALIZED), _mac_handlers(NULL),
  _num_retry(1), _queue(NULL), _duty_cycle_on(LORAWAN_DUTYCYCLE_ON)
{
#ifdef MBED_CONF_LORA_APP_PORT
    // is_port_valid() is not virtual, so we can call it in constructor
    if (is_port_valid(MBED_CONF_LORA_APP_PORT)) {
        _app_port = MBED_CONF_LORA_APP_PORT;
    } else {
        tr_error("User defined port in .json is illegal.");
        _app_port = INVALID_PORT;
    }

#else
    // initialize it to INVALID_PORT (255) an illegal port number.
    // user should set the port
     _app_port = INVALID_PORT;
#endif

     memset(&_lw_session, 0, sizeof(_lw_session));
     memset(&_tx_msg, 0, sizeof(_tx_msg));
     memset(&_rx_msg, 0, sizeof(_rx_msg));
}

LoRaWANStack::~LoRaWANStack()
{

}

/*****************************************************************************
 * Public member functions                                                   *
 ****************************************************************************/
LoRaWANStack& LoRaWANStack::get_lorawan_stack()
{
    static LoRaWANStack _lw_stack;
    return _lw_stack;
}

radio_events_t *LoRaWANStack::bind_radio_driver(LoRaRadio& radio)
{
    // Store pointer to callback routines inside MAC layer (non-IRQ safe)
    _mac_handlers = GetPhyEventHandlers();
    //  passes the reference to radio driver down to PHY layer
    lora_phy.set_radio_instance(radio);
    return _mac_handlers;
}
lora_mac_status_t LoRaWANStack::initialize_mac_layer(EventQueue *queue)
{
    if (DEVICE_STATE_NOT_INITIALIZED != _device_current_state)
    {
        tr_debug("Initialized already");
        return LORA_MAC_STATUS_OK;
    }

    static LoRaMacPrimitives_t LoRaMacPrimitives;
    static LoRaMacCallback_t LoRaMacCallbacks;
    static lora_mac_mib_request_confirm_t mib_req;
    static uint8_t compliance_test_buffer[LORAWAN_TX_MAX_SIZE];

    tr_debug("Initializing MAC layer");

    //store a pointer to Event Queue
    _queue = queue;

    // Allocate memory for compliance test
    _compliance_test.app_data_buffer = compliance_test_buffer;

    TimerTimeCounterInit( );
    LoRaMacPrimitives.MacMcpsConfirm = callback(this, &LoRaWANStack::mcps_confirm);
    LoRaMacPrimitives.MacMcpsIndication = callback(this, &LoRaWANStack::mcps_indication);
    LoRaMacPrimitives.MacMlmeConfirm = callback(this, &LoRaWANStack::mlme_confirm);
    LoRaMacInitialization(&LoRaMacPrimitives, &LoRaMacCallbacks, &lora_phy, queue);

    mib_req.type = LORA_MIB_ADR;
    mib_req.param.adr_enable = LORAWAN_ADR_ON;
    mib_set_request(&mib_req);

    mib_req.type = LORA_MIB_PUBLIC_NETWORK;
    mib_req.param.enable_public_network = LORAWAN_PUBLIC_NETWORK;
    mib_set_request(&mib_req);

    // Reset counters to zero. Will change in future with 1.1 support.
    _lw_session.downlink_counter = 0;
    _lw_session.uplink_counter = 0;

    // Start loRaWAN state machine.
    set_device_state(DEVICE_STATE_INIT);
    return lora_state_machine();
}

/**
 *
 * Prepares the upload message to reserved ports
 *
 * \param port              Application port
 */
void LoRaWANStack::prepare_special_tx_frame(uint8_t port)
{
    if (port == 224) {
        // Clear any normal message stuff before compliance test.
        memset(&_tx_msg, 0, sizeof(_tx_msg));

        if (_compliance_test.link_check == true) {
            _compliance_test.link_check = false;
            _compliance_test.state = 1;
            _tx_msg.f_buffer_size = 3;
            _tx_msg.f_buffer[0] = 5;
            _tx_msg.f_buffer[1] = _compliance_test.demod_margin;
            _tx_msg.f_buffer[2] = _compliance_test.nb_gateways;
        } else {
            switch (_compliance_test.state) {
            case 4:
                _compliance_test.state = 1;
                _tx_msg.f_buffer_size = _compliance_test.app_data_size;

                _tx_msg.f_buffer[0] = _compliance_test.app_data_buffer[0];
                for(uint8_t i = 1; i < MIN(_compliance_test.app_data_size, LORAWAN_TX_MAX_SIZE); ++i) {
                    _tx_msg.f_buffer[i] = _compliance_test.app_data_buffer[i];
                }
                break;
            case 1:
                _tx_msg.f_buffer_size = 2;
                _tx_msg.f_buffer[0] = _compliance_test.downlink_counter >> 8;
                _tx_msg.f_buffer[1] = _compliance_test.downlink_counter;
                break;
            }
        }
    }
}

/** Hands over the compliance test frame to MAC layer
 *
 * \return          returns the state of the LoRa MAC
 */
lora_mac_status_t LoRaWANStack::send_compliance_test_frame_to_mac()
{
    lora_mac_mcps_req_t mcps_req;

    GetPhyParams_t phy_params;
    PhyParam_t default_datarate;
    phy_params.Attribute = PHY_DEF_TX_DR;
    default_datarate = lora_phy.get_phy_params(&phy_params);

    prepare_special_tx_frame(_compliance_test.app_port);

    if (!_compliance_test.is_tx_confirmed) {
        mcps_req.type = LORA_MCPS_UNCONFIRMED;
        mcps_req.req.unconfirmed.f_port = _compliance_test.app_port;
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.unconfirmed.datarate = default_datarate.Value;

        tr_info("Transmit unconfirmed compliance test frame %d bytes.", mcps_req.f_buffer_size);

        for (uint8_t i = 0; i < mcps_req.f_buffer_size; ++i) {
            tr_info("Byte %d, data is 0x%x", i+1, ((uint8_t*)mcps_req.f_buffer)[i]);
        }
    } else if (_compliance_test.is_tx_confirmed) {
        mcps_req.type = LORA_MCPS_CONFIRMED;
        mcps_req.req.confirmed.f_port = _compliance_test.app_port;
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.confirmed.nb_trials = _num_retry;
        mcps_req.req.confirmed.datarate = default_datarate.Value;

        tr_info("Transmit confirmed compliance test frame %d bytes.", mcps_req.f_buffer_size);

        for (uint8_t i = 0; i < mcps_req.f_buffer_size; ++i) {
            tr_info("Byte %d, data is 0x%x", i+1, ((uint8_t*)mcps_req.f_buffer)[i]);
        }
    } else {
        return LORA_MAC_STATUS_SERVICE_UNKNOWN;
    }

    return mcps_request_handler(&mcps_req);
}

uint16_t LoRaWANStack::check_possible_tx_size(uint16_t size)
{
    LoRaMacTxInfo_t txInfo;
    if (LoRaMacQueryTxPossible(size, &txInfo) == LORAMAC_STATUS_LENGTH_ERROR) {
        // Cannot transmit this much. Return how much data can be sent
        // at the moment
        return txInfo.MaxPossiblePayload;
    }

    return txInfo.CurrentPayloadSize;
}

/** Hands over the frame to MAC layer
 *
 * \return          returns the state of the LoRa MAC
 */
lora_mac_status_t LoRaWANStack::send_frame_to_mac()
{
    lora_mac_mcps_req_t mcps_req;
    lora_mac_status_t status;
   // LoRaMacTxInfo_t txInfo;

    GetPhyParams_t phy_params;
    PhyParam_t default_datarate;
    phy_params.Attribute = PHY_DEF_TX_DR;
    default_datarate = lora_phy.get_phy_params(&phy_params);

    mcps_req.type = _tx_msg.type;

    if (LORA_MCPS_UNCONFIRMED == mcps_req.type) {
        mcps_req.req.unconfirmed.f_port = _tx_msg.message_u.unconfirmed.f_port;
        mcps_req.f_buffer = _tx_msg.f_buffer;

        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.unconfirmed.datarate = default_datarate.Value;

    } else if (LORA_MCPS_CONFIRMED == mcps_req.type) {
        mcps_req.req.confirmed.f_port = _tx_msg.message_u.confirmed.f_port;
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.confirmed.nb_trials = _tx_msg.message_u.confirmed.nb_trials;
        mcps_req.req.confirmed.datarate = default_datarate.Value;

    } else if (LORA_MCPS_PROPRIETARY == mcps_req.type) {
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.proprietary.datarate = default_datarate.Value;
    } else {
        return LORA_MAC_STATUS_SERVICE_UNKNOWN;
    }

    status = mcps_request_handler(&mcps_req);

    return status;
}

lora_mac_status_t LoRaWANStack::set_confirmed_msg_retry(uint8_t count)
{
    if (count >= MAX_CONFIRMED_MSG_RETRIES) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    _num_retry = count;

    return LORA_MAC_STATUS_OK;
}

void LoRaWANStack::set_device_state(device_states_t new_state)
{
    _device_current_state = new_state;
}

/** Wrapper function to MCPS-Confirm event function
 *
 * \param mcps_confirm      Pointer to the confirm structure,
 *                          containing confirm attributes.
 */
void LoRaWANStack::mcps_confirm(McpsConfirm_t *mcps_confirm)
{
    lora_mac_mcps_confirm_t lora_mcps_confirm;
    lora_mcps_confirm.ack_received = mcps_confirm->AckReceived;
    lora_mcps_confirm.nb_retries = mcps_confirm->NbRetries;
    lora_mcps_confirm.datarate = mcps_confirm->Datarate;
    lora_mcps_confirm.tx_power = mcps_confirm->TxPower;
    lora_mcps_confirm.uplink_counter = mcps_confirm->UpLinkCounter;
    lora_mcps_confirm.uplink_frequency = mcps_confirm->UpLinkFrequency;

    // Interprets from Mcps_t to lora_mac_mcps_t
    mcps_confirm->McpsRequest = interpret_mcps_confirm_type(lora_mcps_confirm.mcps_request);
    // Interprets from LoRaMacEventInfoStatus_t to lora_mac_event_info_status_t
    lora_mcps_confirm.status = interpret_event_info_type(mcps_confirm->Status);
    lora_mcps_confirm.tx_time_on_air = mcps_confirm->TxTimeOnAir;

    mcps_confirm_handler(&lora_mcps_confirm);
}

/** Wrapper function to MCPS-Indication event function
 *
 * \param mcps_indication   Pointer to the indication structure,
 *                          containing indication attributes.
 */
void LoRaWANStack::mcps_indication(McpsIndication_t *mcps_indication)
{
    lora_mac_mcps_indication_t lora_mcps_indication;

    lora_mcps_indication.ack_received = mcps_indication->AckReceived;
    memcpy(lora_mcps_indication.buffer,  mcps_indication->Buffer, mcps_indication->BufferSize);
    lora_mcps_indication.buffer_size = mcps_indication->BufferSize;
    lora_mcps_indication.downlink_counter = mcps_indication->DownLinkCounter;
    lora_mcps_indication.frame_pending = mcps_indication->FramePending;
    lora_mcps_indication.mcps_indication = (lora_mac_mcps_t)mcps_indication->McpsIndication;
    lora_mcps_indication.multicast = mcps_indication->Multicast;
    lora_mcps_indication.port = mcps_indication->Port;
    lora_mcps_indication.rssi = mcps_indication->Rssi;
    lora_mcps_indication.rx_data = mcps_indication->RxData;
    lora_mcps_indication.rx_datarate = mcps_indication->RxDatarate;
    lora_mcps_indication.rx_slot = mcps_indication->RxSlot;
    lora_mcps_indication.snr = mcps_indication->Snr;
    lora_mcps_indication.status = (lora_mac_event_info_status_t)mcps_indication->Status;

    mcps_indication_handler(&lora_mcps_indication);
}

/** Wrapper function to MLME-Confirm event function
 *
 * \param mlme_confirm      Pointer to the confirm structure,
 *                          containing confirm attributes.
 */
void LoRaWANStack::mlme_confirm(MlmeConfirm_t *mlme_confirm)
{
    lora_mac_mlme_confirm_t lora_mlme_confirm;

    lora_mlme_confirm.demod_margin = mlme_confirm->DemodMargin;
    lora_mlme_confirm.mlme_request = (lora_mac_mlme_t)mlme_confirm->MlmeRequest;
    lora_mlme_confirm.nb_gateways = mlme_confirm->NbGateways;
    lora_mlme_confirm.nb_retries = mlme_confirm->NbRetries;
    lora_mlme_confirm.status = (lora_mac_event_info_status_t)mlme_confirm->Status;
    lora_mlme_confirm.tx_time_on_air = mlme_confirm->TxTimeOnAir;

    mlme_confirm_handler(&lora_mlme_confirm);
}

void LoRaWANStack::set_lora_callbacks(lorawan_app_callbacks_t *cbs)
{

    if (cbs) {
        if (cbs->events) {
            _callbacks.events = cbs->events;
        }

        if (cbs->link_check_resp) {
            _callbacks.link_check_resp = cbs->link_check_resp;
        }
    }
}

lora_mac_status_t LoRaWANStack::add_channels(const lora_channelplan_t &channel_plan)
{
    // If device is not initialized, stop right away
    if (_device_current_state == DEVICE_STATE_NOT_INITIALIZED) {
        tr_error("Stack not initialized!");
        return LORA_MAC_STATUS_NOT_INITIALIZED;
    }

    ChannelParams_t mac_layer_ch_params;
    LoRaMacStatus_t status;

    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = lora_phy.get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // check if user is setting more channels than supported
    if (channel_plan.nb_channels > max_num_channels) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    for (uint8_t i = 0; i < channel_plan.nb_channels; i++) {
        mac_layer_ch_params.Band = channel_plan.channels[i].ch_param.band;
        mac_layer_ch_params.DrRange.Fields.Max = channel_plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max;
        mac_layer_ch_params.DrRange.Fields.Min = channel_plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min;
        mac_layer_ch_params.DrRange.Value = channel_plan.channels[i].ch_param.dr_range.value;
        mac_layer_ch_params.Frequency = channel_plan.channels[i].ch_param.frequency;
        mac_layer_ch_params.Rx1Frequency =channel_plan.channels[i].ch_param.rx1_frequency;

        status = LoRaMacChannelAdd(channel_plan.channels[i].id, mac_layer_ch_params);

        if (status != LORAMAC_STATUS_OK) {
            return error_type_converter(status);
        }
    }

    return LORA_MAC_STATUS_OK;
}

lora_mac_status_t LoRaWANStack::drop_channel_list()
{
    if (_device_current_state == DEVICE_STATE_NOT_INITIALIZED )
    {
        tr_error("Stack not initialized!");
        return LORA_MAC_STATUS_NOT_INITIALIZED;
    }

    lora_mac_status_t status = LORA_MAC_STATUS_OK;

    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_masks;
    uint16_t *default_channel_masks;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = lora_phy.get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // Now check the channel mask for enabled channels
    get_phy.Attribute = PHY_CHANNELS_MASK;
    phy_param = lora_phy.get_phy_params(&get_phy);
    channel_masks = phy_param.ChannelsMask;

    // Now check the channel mask for default channels
    get_phy.Attribute = PHY_CHANNELS_DEFAULT_MASK;
    phy_param = lora_phy.get_phy_params(&get_phy);
    default_channel_masks = phy_param.ChannelsMask;

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip any default channels
        if ((default_channel_masks[0] & (1U<<i)) != 0) {
            continue;
        }

        // skip any channels which are not currently enabled
        if ((channel_masks[0] & (1U<<i)) == 0) {
            continue;
        }

        status = error_type_converter(LoRaMacChannelRemove(i));

        if (status != LORA_MAC_STATUS_OK) {
            return status;
        }
    }

    return status;
}

lora_mac_status_t LoRaWANStack::remove_a_channel(uint8_t channel_id)
{
    if (_device_current_state == DEVICE_STATE_NOT_INITIALIZED )
    {
        tr_error("Stack not initialized!");
        return LORA_MAC_STATUS_NOT_INITIALIZED;
    }

    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_masks;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = lora_phy.get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // According to specification channel IDs start from 0 and last valid
    // channel ID is N-1 where N=MAX_NUM_CHANNELS.
    // So any ID which is larger or equal to the Max number of channels is invalid
    if (channel_id >= max_num_channels) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    // Now check the Default channel mask
    get_phy.Attribute = PHY_CHANNELS_DEFAULT_MASK;
    phy_param = lora_phy.get_phy_params(&get_phy);
    channel_masks = phy_param.ChannelsMask;

    // check if the channel ID give belongs to a default channel
    // Mostly the default channels are in the first mask if the region
    // have multiple channel masks for various sub-bands. So we check the first
    // mask only and return an error code if user sent a default channel id
    if ((channel_masks[0] & (1U << channel_id)) != 0) {
        tr_error("Not allowed to remove a Default Channel.");
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    return error_type_converter(LoRaMacChannelRemove(channel_id));
}

lora_mac_status_t LoRaWANStack::get_enabled_channels(lora_channelplan_t& channel_plan)
{
    if (_device_current_state == DEVICE_STATE_JOINING
            || _device_current_state == DEVICE_STATE_NOT_INITIALIZED
            || _device_current_state == DEVICE_STATE_INIT)
    {
        tr_error("Cannot get channel plan until Joined!");
        return LORA_MAC_STATUS_BUSY;
    }

    lora_mac_mib_request_confirm_t mib_params;

    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_masks;
    uint8_t count = 0;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = lora_phy.get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // Now check the Default channel mask
    get_phy.Attribute = PHY_CHANNELS_MASK;
    phy_param = lora_phy.get_phy_params(&get_phy);
    channel_masks = phy_param.ChannelsMask;

    // Request Mib to get channels
    memset(&mib_params, 0, sizeof(mib_params));
    mib_params.type = LORA_MIB_CHANNELS;
    mib_get_request(&mib_params);

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip the channels which are not enabled
        if ((channel_masks[0] & (1U << i)) == 0) {
            continue;
        }

        // otherwise add them to the channel_plan struct
        channel_plan.channels[count].id = i;
        channel_plan.channels[count].ch_param.frequency = mib_params.param.channel_list[i].frequency;
        channel_plan.channels[count].ch_param.dr_range.value = mib_params.param.channel_list[i].dr_range.value;
        channel_plan.channels[count].ch_param.dr_range.lora_mac_fields_s.min = mib_params.param.channel_list[i].dr_range.lora_mac_fields_s.min;
        channel_plan.channels[count].ch_param.dr_range.lora_mac_fields_s.max = mib_params.param.channel_list[i].dr_range.lora_mac_fields_s.max;
        channel_plan.channels[count].ch_param.band = mib_params.param.channel_list[i].band;
        channel_plan.channels[count].ch_param.rx1_frequency = mib_params.param.channel_list[i].rx1_frequency;
        count++;
    }

    channel_plan.nb_channels = count;

    return LORA_MAC_STATUS_OK;
}

lora_mac_status_t LoRaWANStack::enable_adaptive_datarate(bool adr_enabled)
{
    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state)
    {
        tr_error("Stack not initialized!");
        return LORA_MAC_STATUS_NOT_INITIALIZED;
    }

    lora_mac_mib_request_confirm_t adr_mib_params;

    adr_mib_params.type = LORA_MIB_ADR;
    adr_mib_params.param.adr_enable = adr_enabled;

    return mib_set_request(&adr_mib_params);
}

lora_mac_status_t LoRaWANStack::set_channel_data_rate(uint8_t data_rate)
{
    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state)
    {
        tr_error("Stack not initialized!");
        return LORA_MAC_STATUS_NOT_INITIALIZED;
    }

    lora_mac_mib_request_confirm_t mib_params;
    mib_params.type = LORA_MIB_ADR;
    if (mib_get_request(&mib_params) == true) {
        tr_error("Cannot set data rate. Please turn off ADR first.");
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    mib_params.type = LORA_MIB_CHANNELS_DATARATE;
    mib_params.param.channels_datarate = data_rate;

    return mib_set_request(&mib_params);
}

void LoRaWANStack::commission_device(const lora_dev_commission_t &commission_data)
{
    _lw_session.connection.connect_type = commission_data.connection.connect_type;
    _lw_session.downlink_counter = commission_data.downlink_counter;
    _lw_session.uplink_counter = commission_data.uplink_counter;

    if (commission_data.connection.connect_type == LORAWAN_CONNECTION_OTAA) {
        _lw_session.connection.connection_u.otaa.app_eui =
                commission_data.connection.connection_u.otaa.app_eui;
        _lw_session.connection.connection_u.otaa.app_key =
                commission_data.connection.connection_u.otaa.app_key;
        _lw_session.connection.connection_u.otaa.dev_eui =
                commission_data.connection.connection_u.otaa.dev_eui;
        _lw_session.connection.connection_u.otaa.nb_trials =
                commission_data.connection.connection_u.otaa.nb_trials;
    } else {
        _lw_session.connection.connection_u.abp.dev_addr =
                commission_data.connection.connection_u.abp.dev_addr;
        _lw_session.connection.connection_u.abp.nwk_skey =
                commission_data.connection.connection_u.abp.nwk_skey;
        _lw_session.connection.connection_u.abp.app_skey =
                commission_data.connection.connection_u.abp.app_skey;
    }
}

/**
 *
 * Join OTAA
 */
lora_mac_status_t LoRaWANStack::join_request_by_otaa(const lorawan_connect_t &params)
{
    lora_dev_commission_t commission;

    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state)
    {
        tr_error("Stack not initialized!");
        return LORA_MAC_STATUS_NOT_INITIALIZED;
    }

    tr_debug("Initiating OTAA");

    commission.connection.connect_type = LORAWAN_CONNECTION_OTAA;
    commission.connection.connection_u.otaa.dev_eui = params.connection_u.otaa.dev_eui;
    commission.connection.connection_u.otaa.app_eui = params.connection_u.otaa.app_eui;
    commission.connection.connection_u.otaa.app_key = params.connection_u.otaa.app_key;
    commission.connection.connection_u.otaa.nb_trials = params.connection_u.otaa.nb_trials;

    // As mentioned in the comment above, in 1.0.2 spec, counters are always set
    // to zero for new connection. This section is common for both normal and
    // connection restore at this moment. Will change in future with 1.1 support.
    commission.downlink_counter = 0;
    commission.uplink_counter = 0;

    commission_device(commission);
    set_device_state(DEVICE_STATE_JOINING);
    return lora_state_machine();
}

/**
 *
 * Connect ABP
 */
lora_mac_status_t LoRaWANStack::activation_by_personalization(const lorawan_connect_t &params)
{
    lora_mac_status_t status;
    lora_dev_commission_t commission;

    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state) {
        tr_error("Stack not initialized!");
        return LORA_MAC_STATUS_NOT_INITIALIZED;
    }

    tr_debug("Initiating ABP");

    commission.connection.connect_type = LORAWAN_CONNECTION_ABP;
    commission.connection.connection_u.abp.dev_addr = params.connection_u.abp.dev_addr;
    commission.connection.connection_u.abp.nwk_skey = params.connection_u.abp.nwk_skey;
    commission.connection.connection_u.abp.app_skey = params.connection_u.abp.app_skey;

    // If current state is SHUTDOWN, device may be trying to re-establish
    // communication. In case of ABP specification is meddled about frame counters.
    // It says to reset counters to zero but there is no mechanism to tell the
    // network server that the device was disconnected or restarted.
    // At the moment, this implementation does not support a non-volatile
    // memory storage, so we try a last ditch effort here to restore correct
    // frame counters. If that doesn't work, user must manually reset frame
    // counters on their network server.
    commission.downlink_counter = _lw_session.downlink_counter;
    commission.uplink_counter = _lw_session.uplink_counter;

    tr_debug("Frame Counters. UpCnt=%lu, DownCnt=%lu",
             commission.uplink_counter, commission.downlink_counter);

    commission_device(commission);

    set_device_state(DEVICE_STATE_ABP_CONNECTING);
    status = lora_state_machine();

    return status;
}

int16_t LoRaWANStack::handle_tx(uint8_t port, const uint8_t* data,
                                uint16_t length, uint8_t flags)
{
    if (!_lw_session.active) {
        return LORA_MAC_STATUS_NO_ACTIVE_SESSIONS;
    }

    if (_tx_msg.tx_ongoing) {
        return LORA_MAC_STATUS_WOULD_BLOCK;
    }

    if (_compliance_test.running) {
        return LORA_MAC_STATUS_COMPLIANCE_TEST_ON;
    }

    lora_mac_mib_request_confirm_t mib_req;
    lora_mac_status_t status;
    mib_req.type = LORA_MIB_NETWORK_JOINED;
    status = mib_get_request(&mib_req);

    if (status == LORA_MAC_STATUS_OK) {
        if (mib_req.param.is_network_joined == false) {
            return LORA_MAC_STATUS_NO_NETWORK_JOINED;
        }
    }

    status = set_application_port(port);

    if (status != LORA_MAC_STATUS_OK) {
        tr_error("Illegal application port definition.");
        return status;
    }

    if (flags == 0
            || (flags & MSG_FLAG_MASK) == (MSG_CONFIRMED_FLAG|MSG_UNCONFIRMED_FLAG)) {
        tr_error("CONFIRMED and UNCONFIRMED are mutually exclusive for send()");
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    _tx_msg.port = port;

    uint16_t max_possible_size = check_possible_tx_size(length);

    if (max_possible_size > LORAWAN_TX_MAX_SIZE) {
        // LORAWAN_APP_DATA_MAX_SIZE should at least be
        // either equal to or bigger than maximum possible
        // tx size because our tx message buffer takes its
        // length from that macro. Force maximum possible tx unit
        // to be equal to the buffer size user chose.
        max_possible_size = LORAWAN_TX_MAX_SIZE;
    }

    if (max_possible_size < length) {
        tr_info("Cannot transmit %d bytes. Possible TX Size is %d bytes",
                length, max_possible_size);

        _tx_msg.pending_size = length - max_possible_size;
        _tx_msg.f_buffer_size = max_possible_size;
        // copy user buffer upto the max_possible_size
        memcpy(_tx_msg.f_buffer, data, _tx_msg.f_buffer_size);
    } else {
        // Whole message can be sent at one time
        _tx_msg.f_buffer_size = length;
        _tx_msg.pending_size = 0;
        // copy user buffer upto the max_possible_size
        memcpy(_tx_msg.f_buffer, data, length);
    }

    // Handles all unconfirmed messages, including proprietary and multicast
    if ((flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_FLAG
            || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_MULTICAST
            || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_PROPRIETARY) {

         _tx_msg.type = LORA_MCPS_UNCONFIRMED;
         _tx_msg.message_u.unconfirmed.f_port = _app_port;
    }

    // Handles all confirmed messages, including proprietary and multicast
    if ((flags & MSG_FLAG_MASK) == MSG_CONFIRMED_FLAG
            || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_MULTICAST
            || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_PROPRIETARY) {

        _tx_msg.type = LORA_MCPS_CONFIRMED;
        _tx_msg.message_u.confirmed.f_port = _app_port;
        _tx_msg.message_u.confirmed.nb_trials = _num_retry;
    }

    tr_info("RTS = %u bytes, PEND = %u", _tx_msg.f_buffer_size, _tx_msg.pending_size);
    set_device_state(DEVICE_STATE_SEND);
    lora_state_machine();

    // send user the length of data which is scheduled now.
    // user should take care of the pending data.
    return _tx_msg.f_buffer_size;
}

int16_t LoRaWANStack::handle_rx(const uint8_t port, uint8_t* data,
                                uint16_t length, uint8_t flags)
{
    if (!_lw_session.active) {
        return LORA_MAC_STATUS_NO_ACTIVE_SESSIONS;
    }

    // No messages to read.
    if (!_rx_msg.receive_ready) {
        return LORA_MAC_STATUS_WOULD_BLOCK;
    }

    if (_compliance_test.running) {
        return LORA_MAC_STATUS_COMPLIANCE_TEST_ON;
    }

    if (data == NULL) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    uint8_t *base_ptr = _rx_msg.rx_message.mcps_indication.buffer;
    uint16_t base_size = _rx_msg.rx_message.mcps_indication.buffer_size;
    bool read_complete = false;

    if (_rx_msg.rx_message.mcps_indication.port != port) {
        // Nothing yet received for this particular port
        return LORA_MAC_STATUS_WOULD_BLOCK;
    }

    // check if message received is a Confirmed message and user subscribed to it or not
    if (_rx_msg.rx_message.mcps_indication.mcps_indication == LORA_MCPS_CONFIRMED
            && ((flags & MSG_FLAG_MASK) == MSG_CONFIRMED_FLAG
                    || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_MULTICAST
                    || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_PROPRIETARY)) {

        tr_debug("RX - Confirmed Message, flags=%d", flags);
    }

    // check if message received is a Unconfirmed message and user subscribed to it or not
    if (_rx_msg.rx_message.mcps_indication.mcps_indication == LORA_MCPS_UNCONFIRMED
            && ((flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_FLAG
                    || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_MULTICAST
                    || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_PROPRIETARY)) {
        tr_debug("RX - Unconfirmed Message - flags=%d", flags);
    }

    // check the length of received message whether we can fit into user
    // buffer completely or not
    if (_rx_msg.rx_message.mcps_indication.buffer_size > length &&
            _rx_msg.prev_read_size == 0) {
        // we can't fit into user buffer. Invoke counter measures
        _rx_msg.pending_size = _rx_msg.rx_message.mcps_indication.buffer_size - length;
        base_size = length;
        _rx_msg.prev_read_size = base_size;
        memcpy(data, base_ptr, base_size);
    } else if (_rx_msg.prev_read_size == 0) {
        _rx_msg.pending_size = 0;
        _rx_msg.prev_read_size = 0;
        memcpy(data, base_ptr, base_size);
        read_complete = true;
    }

    // If its the pending read then we should copy only the remaining part of
    // the buffer. Due to checks above, in case of a pending read, this block
    // will be the only one to get invoked
    if (_rx_msg.pending_size > 0 && _rx_msg.prev_read_size > 0) {
        memcpy(data, base_ptr+_rx_msg.prev_read_size, base_size);
    }

    // we are done handing over received buffer to user. check if there is
    // anything pending. If not, memset the buffer to zero and indicate
    // that no read is in progress
    if (read_complete) {
        memset(_rx_msg.rx_message.mcps_indication.buffer, 0, LORAMAC_PHY_MAXPAYLOAD);
        _rx_msg.receive_ready = false;
    }

    return base_size;
}

lora_mac_status_t LoRaWANStack::mlme_request_handler(lora_mac_mlme_req_t *mlme_request)
{
    MlmeReq_t request;

    if (NULL == mlme_request) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    request.Type = (Mlme_t)mlme_request->type;

    switch (mlme_request->type) {
        case LORA_MLME_JOIN:
            request.Req.Join.AppEui = mlme_request->req.join.app_eui;
            request.Req.Join.AppKey = mlme_request->req.join.app_key;
            request.Req.Join.DevEui = mlme_request->req.join.dev_eui;
            request.Req.Join.NbTrials = mlme_request->req.join.nb_trials;
            break;
        // This is handled in semtech stack. Only type value is needed.
        case LORA_MLME_LINK_CHECK:
            break;
        case LORA_MLME_TXCW:
            /* no break */
            /* Fall through */
        case LORA_MLME_TXCW_1:
            request.Req.TxCw.Frequency = mlme_request->req.tx_cw.frequency;
            request.Req.TxCw.Power = mlme_request->req.tx_cw.power;
            request.Req.TxCw.Timeout = mlme_request->req.tx_cw.timeout;
            break;
        default:
            return LORA_MAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return error_type_converter(LoRaMacMlmeRequest(&request));
}

/** MLME-Confirm event function
 *
 * \param mlme_confirm      Pointer to the confirm structure,
 *                          containing confirm attributes.
 */
void LoRaWANStack::mlme_confirm_handler(lora_mac_mlme_confirm_t *mlme_confirm)
{
    if (NULL == mlme_confirm) {
        return;
    }

    switch (mlme_confirm->mlme_request) {
        case LORA_MLME_JOIN:
            if (mlme_confirm->status == LORA_EVENT_INFO_STATUS_OK) {
                // Status is OK, node has joined the network
                set_device_state(DEVICE_STATE_JOINED);
                lora_state_machine();
            } else {
                // Join attempt failed.
                set_device_state(DEVICE_STATE_IDLE);
                lora_state_machine();

                if (_callbacks.events) {
                    _queue->call(_callbacks.events, JOIN_FAILURE);
                }
            }
            break;
        case LORA_MLME_LINK_CHECK:
            if (mlme_confirm->status == LORA_EVENT_INFO_STATUS_OK) {
                // Check DemodMargin
                // Check NbGateways
                if (_compliance_test.running == true) {
                    _compliance_test.link_check = true;
                    _compliance_test.demod_margin = mlme_confirm->demod_margin;
                    _compliance_test.nb_gateways = mlme_confirm->nb_gateways;
                }
            }
            break;
        default:
            return;
            break;
    }
}

lora_mac_status_t LoRaWANStack::mcps_request_handler(lora_mac_mcps_req_t *mcps_request)
{
    McpsReq_t request;

    if (NULL == mcps_request) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    request.Type = (Mcps_t)mcps_request->type;

    switch (mcps_request->type) {
        case LORA_MCPS_UNCONFIRMED:
            request.Req.Unconfirmed.Datarate = mcps_request->req.unconfirmed.datarate;
            request.Req.Unconfirmed.fBuffer = mcps_request->f_buffer;
            request.Req.Unconfirmed.fBufferSize = mcps_request->f_buffer_size;
            request.Req.Unconfirmed.fPort = mcps_request->req.unconfirmed.f_port;
            break;
        case LORA_MCPS_CONFIRMED:
            request.Req.Confirmed.Datarate = mcps_request->req.confirmed.datarate;
            request.Req.Confirmed.fBuffer = mcps_request->f_buffer;
            request.Req.Confirmed.fBufferSize = mcps_request->f_buffer_size;
            request.Req.Confirmed.fPort = mcps_request->req.confirmed.f_port;
            request.Req.Confirmed.NbTrials = mcps_request->req.confirmed.nb_trials;
            break;
        case LORA_MCPS_PROPRIETARY:
            request.Req.Proprietary.Datarate = mcps_request->req.proprietary.datarate;
            request.Req.Proprietary.fBuffer = mcps_request->f_buffer;
            request.Req.Proprietary.fBufferSize = mcps_request->f_buffer_size;
            break;
        default:
            return LORA_MAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return error_type_converter(LoRaMacMcpsRequest(&request));
}

/** MCPS-Confirm event function
 *
 * \param mcps_confirm      Pointer to the confirm structure,
 *                          containing confirm attributes.
 */
void LoRaWANStack::mcps_confirm_handler(lora_mac_mcps_confirm_t *mcps_confirm)
{
    if (mcps_confirm == NULL) {
        tr_error("mcps_confirm: struct [in] is null.");
        return;
    }

    if (mcps_confirm->status != LORA_EVENT_INFO_STATUS_OK) {
        // Couldn't schedule packet, ack not recieved in CONFIRMED case
        // or some other error happened. Discard buffer, unset the tx-ongoing
        // flag and let the application know
        _tx_msg.tx_ongoing = false;
        memset(_tx_msg.f_buffer, 0, LORAWAN_TX_MAX_SIZE);
        _tx_msg.f_buffer_size = LORAWAN_TX_MAX_SIZE;

        tr_error("mcps_confirm_handler: Error code = %d", mcps_confirm->status);

        // If sending timed out, we have a special event for that
        if (mcps_confirm->status == LORA_EVENT_INFO_STATUS_TX_TIMEOUT) {
            if (_callbacks.events) {
                _queue->call(_callbacks.events, TX_TIMEOUT);
            }
            return;
        } if (mcps_confirm->status == LORA_EVENT_INFO_STATUS_RX2_TIMEOUT) {
            tr_debug("Did not receive Ack");
        }

        // Otherwise send a general TX_ERROR event
        if (_callbacks.events) {
            _queue->call(_callbacks.events, TX_ERROR);
        }
        return;
    }

    // If No errors encountered, let's proceed with the status.
    // CONFIRMED needs special handling because of acks
    if (mcps_confirm->mcps_request == LORA_MCPS_CONFIRMED) {
        // In confirmed case, we need to check if we have received the Ack or not.
        // This is actually just being paranoid about ack because LoRaMac.cpp doesn't
        // call this callback until an ack is received.
        if (mcps_confirm->ack_received) {
            tr_debug("Ack received.");
        }
    }

    // This part is common to both CONFIRMED and UNCONFIRMED.
    // Tell the application about successful transmission and store
    // data rate plus frame counter.
    _lw_session.uplink_counter = mcps_confirm->uplink_counter;
    _tx_msg.tx_ongoing = false;
     if (_callbacks.events) {
         _queue->call(_callbacks.events, TX_DONE);
     }
}

/** MCPS-Indication event function
 *
 * \param mcps_indication   Pointer to the indication structure,
 *                          containing indication attributes.
 */
void LoRaWANStack::mcps_indication_handler(lora_mac_mcps_indication_t *mcps_indication)
{
    if (mcps_indication == NULL) {
        tr_error("mcps_indication: struct [in] is null.");
        return;
    }

    if (mcps_indication->status != LORA_EVENT_INFO_STATUS_OK) {
        if (_callbacks.events) {
            _queue->call(_callbacks.events, RX_ERROR);
        }
        return;
    }

    switch (mcps_indication->mcps_indication) {
        case LORA_MCPS_UNCONFIRMED:
            break;
        case LORA_MCPS_CONFIRMED:
            break;
        case LORA_MCPS_PROPRIETARY:
            break;
        case LORA_MCPS_MULTICAST:
            break;
        default:
            break;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot

    _lw_session.downlink_counter++;

    if (_compliance_test.running == true) {
        _compliance_test.downlink_counter++;
    }

    if (mcps_indication->rx_data == true) {
        switch (mcps_indication->port) {
        case 224:
            tr_debug("Compliance test command received.");
            compliance_test_handler(mcps_indication);
            break;
        default:
            if (is_port_valid(mcps_indication->port) == true ||
                    mcps_indication->mcps_indication == LORA_MCPS_PROPRIETARY) {

                // Valid message arrived.
                // Save message to buffer with session information.
                if (_rx_msg.rx_message.mcps_indication.buffer_size > LORAMAC_PHY_MAXPAYLOAD) {
                    // This may never happen as both radio and MAC are limited
                    // to the size 255 bytes
                    tr_debug("Cannot receive more than buffer capacity!");
                    if (_callbacks.events) {
                        _queue->call(_callbacks.events, RX_ERROR);
                    }
                    return;
                } else {
                    _rx_msg.type = LORAMAC_RX_MCPS_INDICATION;
                    _rx_msg.rx_message.mcps_indication.buffer_size = mcps_indication->buffer_size;
                    _rx_msg.rx_message.mcps_indication.port = mcps_indication->port;
                    // Copy message for user
                    memcpy(_rx_msg.rx_message.mcps_indication.buffer,
                           mcps_indication->buffer, mcps_indication->buffer_size);
                }

                // Notify application about received frame..
                tr_debug("Received %d bytes", _rx_msg.rx_message.mcps_indication.buffer_size);
                _rx_msg.receive_ready = true;
                if (_callbacks.events) {
                    _queue->call(_callbacks.events, RX_DONE);
                }
            } else {
                // Invalid port, ports 0, 224 and 225-255 are reserved.
            }
            break;
        }
    }
}

/** Compliance testing function
 *
 * \param mcps_indication   Pointer to the indication structure,
 *                          containing indication attributes.
 */
void LoRaWANStack::compliance_test_handler(lora_mac_mcps_indication_t *mcps_indication)
{
    if (_compliance_test.running == false) {
        // Check compliance test enable command (i)
        if ((mcps_indication->buffer_size == 4) &&
            (mcps_indication->buffer[0] == 0x01) &&
            (mcps_indication->buffer[1] == 0x01) &&
            (mcps_indication->buffer[2] == 0x01) &&
            (mcps_indication->buffer[3] == 0x01)) {
            _compliance_test.is_tx_confirmed = false;
            _compliance_test.app_port = 224;
            _compliance_test.app_data_size = 2;
            _compliance_test.downlink_counter = 0;
            _compliance_test.link_check = false;
            _compliance_test.demod_margin = 0;
            _compliance_test.nb_gateways = 0;
            _compliance_test.running = true;
            _compliance_test.state = 1;

            lora_mac_mib_request_confirm_t mib_req;
            mib_req.type = LORA_MIB_ADR;
            mib_req.param.adr_enable = true;
            mib_set_request(&mib_req);

#if MBED_CONF_LORA_PHY      == 0
            LoRaMacTestSetDutyCycleOn(false);
#endif
            //5000ms
            LoRaMacSetTxTimer(5000);
            set_device_state(DEVICE_STATE_COMPLIANCE_TEST);
            tr_debug("Compliance test activated.");
        }
    } else {
        _compliance_test.state = mcps_indication->buffer[0];
        switch (_compliance_test.state) {
        case 0: // Check compliance test disable command (ii)
            _compliance_test.is_tx_confirmed = true;
            _compliance_test.app_port = LORAWAN_APP_PORT;
            _compliance_test.app_data_size = LORAWAN_COMPLIANCE_TEST_DATA_SIZE;
            _compliance_test.downlink_counter = 0;
            _compliance_test.running = false;

            lora_mac_mib_request_confirm_t mib_req;
            mib_req.type = LORA_MIB_ADR;
            mib_req.param.adr_enable = LORAWAN_ADR_ON;
            mib_set_request(&mib_req);
#if MBED_CONF_LORA_PHY      == 0
            LoRaMacTestSetDutyCycleOn(LORAWAN_DUTYCYCLE_ON);
#endif
            // Go to idle state after compliance test mode.
            tr_debug("Compliance test disabled.");
            LoRaMacStopTxTimer();

            // Clear any compliance test message stuff before going back to normal operation.
            memset(&_tx_msg, 0, sizeof(_tx_msg));
            set_device_state(DEVICE_STATE_IDLE);
            lora_state_machine();
            break;
        case 1: // (iii, iv)
            _compliance_test.app_data_size = 2;
            break;
        case 2: // Enable confirmed messages (v)
            _compliance_test.is_tx_confirmed = true;
            _compliance_test.state = 1;
            break;
        case 3:  // Disable confirmed messages (vi)
            _compliance_test.is_tx_confirmed = false;
            _compliance_test.state = 1;
            break;
        case 4: // (vii)
            _compliance_test.app_data_size = mcps_indication->buffer_size;

            _compliance_test.app_data_buffer[0] = 4;
            for(uint8_t i = 1; i < MIN(_compliance_test.app_data_size, LORAMAC_PHY_MAXPAYLOAD); ++i) {
                _compliance_test.app_data_buffer[i] = mcps_indication->buffer[i] + 1;
            }

            send_compliance_test_frame_to_mac();
            break;
        case 5: // (viii)
            lora_mac_mlme_req_t mlme_req;
            mlme_req.type = LORA_MLME_LINK_CHECK;
            mlme_request_handler(&mlme_req);
            break;
        case 6: // (ix)
            lora_mac_mlme_req_t mlme_request;
            lora_mac_mib_request_confirm_t mib_request;

            // Disable TestMode and revert back to normal operation
            _compliance_test.is_tx_confirmed = true;
            _compliance_test.app_port = LORAWAN_APP_PORT;
            _compliance_test.app_data_size = LORAWAN_COMPLIANCE_TEST_DATA_SIZE;
            _compliance_test.downlink_counter = 0;
            _compliance_test.running = false;

            mib_request.type = LORA_MIB_ADR;
            mib_request.param.adr_enable = LORAWAN_ADR_ON;
            mib_set_request(&mib_request);
#if MBED_CONF_LORA_PHY      == 0
            LoRaMacTestSetDutyCycleOn(LORAWAN_DUTYCYCLE_ON);
#endif
            mlme_request.type = LORA_MLME_JOIN;
            mlme_request.req.join.dev_eui = _lw_session.connection.connection_u.otaa.dev_eui;
            mlme_request.req.join.app_eui = _lw_session.connection.connection_u.otaa.app_eui;
            mlme_request.req.join.app_key = _lw_session.connection.connection_u.otaa.app_key;
            mlme_request.req.join.nb_trials = _lw_session.connection.connection_u.otaa.nb_trials;
            mlme_request_handler(&mlme_request);
            break;
        case 7: // (x)
            if (mcps_indication->buffer_size == 3) {
                lora_mac_mlme_req_t mlme_req;
                mlme_req.type = LORA_MLME_TXCW;
                mlme_req.req.tx_cw.timeout = (uint16_t)((mcps_indication->buffer[1] << 8) | mcps_indication->buffer[2]);
                mlme_request_handler(&mlme_req);
            } else if (mcps_indication->buffer_size == 7) {
                lora_mac_mlme_req_t mlme_req;
                mlme_req.type = LORA_MLME_TXCW_1;
                mlme_req.req.tx_cw.timeout = (uint16_t)((mcps_indication->buffer[1] << 8) | mcps_indication->buffer[2]);
                mlme_req.req.tx_cw.frequency = (uint32_t)((mcps_indication->buffer[3] << 16) | (mcps_indication->buffer[4] << 8)
                        | mcps_indication->buffer[5]) * 100;
                mlme_req.req.tx_cw.power = mcps_indication->buffer[6];
                mlme_request_handler(&mlme_req);
            }
            _compliance_test.state = 1;
            break;
        }
    }
}

lora_mac_status_t LoRaWANStack::mib_set_request(lora_mac_mib_request_confirm_t *mib_set_params)
{
    MibRequestConfirm_t stack_mib_set;
    ChannelParams_t stack_channel_params;
    MulticastParams_t *stack_multicast_params = NULL;
    MulticastParams_t *head = NULL;
    lora_mac_status_t status;

    if (NULL == mib_set_params) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    // Interpreting from lora_mac_mib_t to Mib_t
    stack_mib_set.Type = interpret_mib_req_confirm_type(mib_set_params->type);

    switch (mib_set_params->type) {
        case LORA_MIB_DEVICE_CLASS:
            stack_mib_set.Param.Class = (DeviceClass_t)mib_set_params->param.lora_class;
            break;

        case LORA_MIB_NETWORK_JOINED:
            stack_mib_set.Param.IsNetworkJoined = mib_set_params->param.is_network_joined;
            break;

        case LORA_MIB_ADR:
            stack_mib_set.Param.AdrEnable = mib_set_params->param.adr_enable;
            break;

        case LORA_MIB_NET_ID:
            stack_mib_set.Param.NetID = mib_set_params->param.net_id;
            break;

        case LORA_MIB_DEV_ADDR:
            stack_mib_set.Param.DevAddr = mib_set_params->param.dev_addr;
            break;

        case LORA_MIB_NWK_SKEY:
            stack_mib_set.Param.NwkSKey = mib_set_params->param.nwk_skey;
            break;

        case LORA_MIB_APP_SKEY:
            stack_mib_set.Param.AppSKey = mib_set_params->param.app_skey;
            break;

        case LORA_MIB_PUBLIC_NETWORK:
            stack_mib_set.Param.EnablePublicNetwork = mib_set_params->param.enable_public_network;
            break;

        case LORA_MIB_REPEATER_SUPPORT:
            stack_mib_set.Param.EnableRepeaterSupport = mib_set_params->param.enable_repeater_support;
            break;

        case LORA_MIB_CHANNELS:
            stack_channel_params.Frequency = mib_set_params->param.channel_list->frequency;
            stack_channel_params.DrRange.Value = mib_set_params->param.channel_list->dr_range.value;
            stack_channel_params.DrRange.Fields.Min = mib_set_params->param.channel_list->dr_range.lora_mac_fields_s.min;
            stack_channel_params.DrRange.Fields.Max = mib_set_params->param.channel_list->dr_range.lora_mac_fields_s.max;
            stack_channel_params.Band = mib_set_params->param.channel_list->band;
            stack_channel_params.Rx1Frequency = mib_set_params->param.channel_list->rx1_frequency;
            stack_mib_set.Param.ChannelList = &stack_channel_params;
            break;

        case LORA_MIB_RX2_CHANNEL:
            stack_mib_set.Param.Rx2Channel.Frequency = mib_set_params->param.rx2_channel.frequency;
            stack_mib_set.Param.Rx2Channel.Datarate = mib_set_params->param.rx2_channel.datarate;
            break;

        case LORA_MIB_RX2_DEFAULT_CHANNEL:
            stack_mib_set.Param.Rx2DefaultChannel.Frequency = mib_set_params->param.rx2_default_channel.frequency;
            stack_mib_set.Param.Rx2DefaultChannel.Datarate = mib_set_params->param.rx2_default_channel.datarate;
            break;

        case LORA_MIB_CHANNELS_MASK:
            stack_mib_set.Param.ChannelsMask = mib_set_params->param.channels_mask;
           break;

        case LORA_MIB_CHANNELS_DEFAULT_MASK:
            stack_mib_set.Param.ChannelsDefaultMask = mib_set_params->param.channels_default_mask;
            break;

        case LORA_MIB_CHANNELS_NB_REP:
            stack_mib_set.Param.ChannelNbRep = mib_set_params->param.channel_nb_rep;
            break;

        case LORA_MIB_MAX_RX_WINDOW_DURATION:
            stack_mib_set.Param.MaxRxWindow = mib_set_params->param.max_rx_window;
            break;

        case LORA_MIB_RECEIVE_DELAY_1:
            stack_mib_set.Param.ReceiveDelay1 = mib_set_params->param.receive_delay1;
            break;

        case LORA_MIB_RECEIVE_DELAY_2:
            stack_mib_set.Param.ReceiveDelay2 = mib_set_params->param.receive_delay2;
            break;

        case LORA_MIB_JOIN_ACCEPT_DELAY_1:
            stack_mib_set.Param.JoinAcceptDelay1 = mib_set_params->param.join_accept_delay1;
            break;

        case LORA_MIB_JOIN_ACCEPT_DELAY_2:
            stack_mib_set.Param.JoinAcceptDelay2 = mib_set_params->param.join_accept_delay2;
            break;

        case LORA_MIB_CHANNELS_DEFAULT_DATARATE:
            stack_mib_set.Param.ChannelsDefaultDatarate = mib_set_params->param.channels_default_datarate;
            break;

        case LORA_MIB_CHANNELS_DATARATE:
            stack_mib_set.Param.ChannelsDatarate = mib_set_params->param.channels_datarate;
            break;

        case LORA_MIB_CHANNELS_TX_POWER:
            stack_mib_set.Param.ChannelsTxPower = mib_set_params->param.channels_tx_power;
            break;

        case LORA_MIB_CHANNELS_DEFAULT_TX_POWER:
            stack_mib_set.Param.ChannelsDefaultTxPower = mib_set_params->param.channels_default_tx_power;
            break;

        case LORA_MIB_UPLINK_COUNTER:
            stack_mib_set.Param.UpLinkCounter = mib_set_params->param.uplink_counter;
            break;

        case LORA_MIB_DOWNLINK_COUNTER:
            stack_mib_set.Param.DownLinkCounter = mib_set_params->param.downlink_counter;
            break;

        case LORA_MIB_MULTICAST_CHANNEL:
            /*
             * Parse multicast list (C++ linked list)
             */
            while (mib_set_params->param.multicast_list != NULL) {
                if (stack_multicast_params == NULL) {
                    stack_multicast_params = new MulticastParams_t;
                    head = stack_multicast_params;
                } else {
                    while (stack_multicast_params != NULL) {
                        stack_multicast_params = stack_multicast_params->Next;
                    }

                    stack_multicast_params = new MulticastParams_t;
                }

                stack_multicast_params->Address = mib_set_params->param.multicast_list->address;
                for (int i = 0; i < 16; i++) {
                    stack_multicast_params->NwkSKey[i] = mib_set_params->param.multicast_list->nwk_skey[i];
                    stack_multicast_params->AppSKey[i] = mib_set_params->param.multicast_list->app_skey[i];
                }

                stack_multicast_params->DownLinkCounter = mib_set_params->param.multicast_list->downlink_counter;
            }

            stack_mib_set.Param.MulticastList = head;
            break;

        case LORA_MIB_SYSTEM_MAX_RX_ERROR:
            stack_mib_set.Param.SystemMaxRxError = mib_set_params->param.system_max_rx_error;
            break;

        case LORA_MIB_MIN_RX_SYMBOLS:
            stack_mib_set.Param.MinRxSymbols = mib_set_params->param.min_rx_symbols;
            break;

        default:
            return LORA_MAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    /*
     * Set MIB data to LoRa stack
     */
    status = error_type_converter(LoRaMacMibSetRequestConfirm(&stack_mib_set));
    /*
     * Release memory if reserved by multicast list
     */
    if (NULL != head) {
        while (NULL != head) {
            delete head;
            head = NULL;
            head = stack_mib_set.Param.MulticastList->Next;
        }
        stack_mib_set.Param.MulticastList = NULL;
    }

    return status;
}

lora_mac_status_t LoRaWANStack::mib_get_request(lora_mac_mib_request_confirm_t *mib_get_params)
{
    MibRequestConfirm_t stack_mib_get;
    MulticastParams_t *origin_multicast_list = NULL;
    lora_mac_multicast_params_t *new_multicast_list = NULL;
    lora_mac_status_t mac_status = LORA_MAC_STATUS_OK;

    if(NULL == mib_get_params) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    // Interprets from lora_mac_mib_t to Mib_t
    stack_mib_get.Type = interpret_mib_req_confirm_type(mib_get_params->type);
    mac_status = error_type_converter(LoRaMacMibGetRequestConfirm(&stack_mib_get));

    if (LORA_MAC_STATUS_OK != mac_status) {
        return LORA_MAC_STATUS_SERVICE_UNKNOWN;
    }

    switch(mib_get_params->type) {
        case LORA_MIB_DEVICE_CLASS:
            mib_get_params->param.lora_class = (lora_mac_device_class_t)stack_mib_get.Param.Class;
            break;
        case LORA_MIB_NETWORK_JOINED:
            mib_get_params->param.is_network_joined = stack_mib_get.Param.IsNetworkJoined;
            break;
        case LORA_MIB_ADR:
            mib_get_params->param.adr_enable = stack_mib_get.Param.AdrEnable;
            break;
        case LORA_MIB_NET_ID:
            mib_get_params->param.net_id = stack_mib_get.Param.NetID;
            break;
        case LORA_MIB_DEV_ADDR:
            mib_get_params->param.dev_addr = stack_mib_get.Param.DevAddr;
            break;
        case LORA_MIB_NWK_SKEY:
            mib_get_params->param.nwk_skey = stack_mib_get.Param.NwkSKey;
            break;
        case LORA_MIB_APP_SKEY:
            mib_get_params->param.app_skey = stack_mib_get.Param.AppSKey;
            break;
        case LORA_MIB_PUBLIC_NETWORK:
            mib_get_params->param.enable_public_network = stack_mib_get.Param.EnablePublicNetwork;
            break;
        case LORA_MIB_REPEATER_SUPPORT:
            mib_get_params->param.enable_repeater_support = stack_mib_get.Param.EnableRepeaterSupport;
            break;
        case LORA_MIB_CHANNELS:
            mib_get_params->param.channel_list = (lora_mac_channel_params_t *) stack_mib_get.Param.ChannelList;
            break;
        case LORA_MIB_RX2_CHANNEL:
            mib_get_params->param.rx2_channel.datarate = stack_mib_get.Param.Rx2Channel.Datarate;
            mib_get_params->param.rx2_channel.frequency = stack_mib_get.Param.Rx2Channel.Frequency;
            break;
        case LORA_MIB_RX2_DEFAULT_CHANNEL:
            mib_get_params->param.rx2_default_channel.datarate = stack_mib_get.Param.Rx2DefaultChannel.Datarate;
            mib_get_params->param.rx2_default_channel.frequency = stack_mib_get.Param.Rx2DefaultChannel.Frequency;
            break;
        case LORA_MIB_CHANNELS_DEFAULT_MASK:
            mib_get_params->param.channels_default_mask = stack_mib_get.Param.ChannelsDefaultMask;
            break;
        case LORA_MIB_CHANNELS_MASK:
            mib_get_params->param.channels_mask = stack_mib_get.Param.ChannelsMask;
            break;
        case LORA_MIB_CHANNELS_NB_REP:
            mib_get_params->param.channel_nb_rep = stack_mib_get.Param.ChannelNbRep;
            break;
        case LORA_MIB_MAX_RX_WINDOW_DURATION:
            mib_get_params->param.max_rx_window = stack_mib_get.Param.MaxRxWindow;
            break;
        case LORA_MIB_RECEIVE_DELAY_1:
            mib_get_params->param.receive_delay1 = stack_mib_get.Param.ReceiveDelay1;
            break;
        case LORA_MIB_RECEIVE_DELAY_2:
            mib_get_params->param.receive_delay2 = stack_mib_get.Param.ReceiveDelay2;
            break;
        case LORA_MIB_JOIN_ACCEPT_DELAY_1:
            mib_get_params->param.join_accept_delay1 = stack_mib_get.Param.JoinAcceptDelay1;
            break;
        case LORA_MIB_JOIN_ACCEPT_DELAY_2:
            mib_get_params->param.join_accept_delay2 = stack_mib_get.Param.JoinAcceptDelay2;
            break;
        case LORA_MIB_CHANNELS_DEFAULT_DATARATE:
            mib_get_params->param.channels_default_datarate = stack_mib_get.Param.ChannelsDefaultDatarate;
            break;
        case LORA_MIB_CHANNELS_DATARATE:
            mib_get_params->param.channels_datarate = stack_mib_get.Param.ChannelsDatarate;
            break;
        case LORA_MIB_CHANNELS_DEFAULT_TX_POWER:
            mib_get_params->param.channels_default_tx_power = stack_mib_get.Param.ChannelsDefaultTxPower;
            break;
        case LORA_MIB_CHANNELS_TX_POWER:
            mib_get_params->param.channels_tx_power = stack_mib_get.Param.ChannelsTxPower;
            break;
        case LORA_MIB_UPLINK_COUNTER:
            mib_get_params->param.uplink_counter = stack_mib_get.Param.UpLinkCounter;
            break;
        case LORA_MIB_DOWNLINK_COUNTER:
            mib_get_params->param.downlink_counter = stack_mib_get.Param.DownLinkCounter;
            break;
        case LORA_MIB_MULTICAST_CHANNEL:
            /*
             * Parse multicast list (C++ linked list)
             */
            origin_multicast_list = stack_mib_get.Param.MulticastList;

            while (NULL != origin_multicast_list) {
                if (NULL == new_multicast_list) {
                    new_multicast_list = new lora_mac_multicast_params_t;
                    new_multicast_list->next = NULL;

                    mib_get_params->param.multicast_list = new_multicast_list;
                } else {
                    while (NULL != new_multicast_list) {
                        new_multicast_list = new_multicast_list->next;
                    }
                    new_multicast_list = new lora_mac_multicast_params_t;
                    new_multicast_list->next = NULL;
                }

                new_multicast_list->address = origin_multicast_list->Address;
                for (int i = 0; i < 16; ++i) {
                    new_multicast_list->nwk_skey[i] = origin_multicast_list->NwkSKey[i];
                    new_multicast_list->app_skey[i] = origin_multicast_list->AppSKey[i];
                }
                new_multicast_list->downlink_counter = origin_multicast_list->DownLinkCounter;

                origin_multicast_list = origin_multicast_list->Next;
            }
            break;
        case LORA_MIB_SYSTEM_MAX_RX_ERROR:
            mib_get_params->param.system_max_rx_error = stack_mib_get.Param.SystemMaxRxError;
            break;
        case LORA_MIB_MIN_RX_SYMBOLS:
            mib_get_params->param.min_rx_symbols = stack_mib_get.Param.MinRxSymbols;
            break;
        default:
            return LORA_MAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return mac_status;
}

lora_mac_status_t LoRaWANStack::error_type_converter(LoRaMacStatus_t type)
{
    switch (type) {
        case LORAMAC_STATUS_OK:
            return LORA_MAC_STATUS_OK;
            break;

        case LORAMAC_STATUS_BUSY:
            return LORA_MAC_STATUS_BUSY;
            break;

        case LORAMAC_STATUS_SERVICE_UNKNOWN:
            return LORA_MAC_STATUS_SERVICE_UNKNOWN;
            break;

        case LORAMAC_STATUS_PARAMETER_INVALID:
            return LORA_MAC_STATUS_PARAMETER_INVALID;
            break;

        case LORAMAC_STATUS_FREQUENCY_INVALID:
            return LORA_MAC_STATUS_FREQUENCY_INVALID;
            break;

        case LORAMAC_STATUS_DATARATE_INVALID:
            return LORA_MAC_STATUS_DATARATE_INVALID;
            break;

        case LORAMAC_STATUS_FREQ_AND_DR_INVALID:
            return LORA_MAC_STATUS_FREQ_AND_DR_INVALID;
            break;

        case LORAMAC_STATUS_NO_NETWORK_JOINED:
            return LORA_MAC_STATUS_NO_NETWORK_JOINED;
            break;

        case LORAMAC_STATUS_LENGTH_ERROR:
            return LORA_MAC_STATUS_LENGTH_ERROR;
            break;

        case LORAMAC_STATUS_DEVICE_OFF:
            return LORA_MAC_STATUS_DEVICE_OFF;
            break;

        case LORAMAC_STATUS_CRYPTO_FAIL:
            return LORA_MAC_STATUS_CRYPTO_FAIL;
            break;

        default:
            return LORA_MAC_STATUS_SERVICE_UNKNOWN;
            break;
    }
}

void LoRaWANStack::shutdown()
{
    set_device_state(DEVICE_STATE_SHUTDOWN);
    lora_state_machine();
}

lora_mac_status_t LoRaWANStack::lora_state_machine()
{
    lora_mac_mib_request_confirm_t mib_req;
    lora_mac_status_t status = LORA_MAC_STATUS_DEVICE_OFF;

    switch (_device_current_state) {
        case DEVICE_STATE_SHUTDOWN:
            /*
             * Remove channels
             * Radio will be put to sleep by the APIs underneath
             */
            drop_channel_list();

            // Stop sending messages and set joined status to false.
            LoRaMacStopTxTimer();
            mib_req.type = LORA_MIB_NETWORK_JOINED;
            mib_req.param.is_network_joined = false;
            mib_set_request(&mib_req);

            // reset buffers to original state
            memset(_tx_msg.f_buffer, 0, LORAWAN_TX_MAX_SIZE);
            _tx_msg.pending_size = 0;
            _tx_msg.f_buffer_size = 0;
            _tx_msg.tx_ongoing = false;
            memset(_rx_msg.rx_message.mcps_indication.buffer, 0, LORAMAC_PHY_MAXPAYLOAD);
            _rx_msg.receive_ready = false;
            _rx_msg.prev_read_size = 0;
            _rx_msg.rx_message.mcps_indication.buffer_size = 0;

            // disable the session
            _lw_session.active = false;

            tr_debug("LoRaWAN protocol has been shut down.");
            if (_callbacks.events) {
                _queue->call(_callbacks.events, DISCONNECTED);
            }
            status = LORA_MAC_STATUS_DEVICE_OFF;
            break;
        case DEVICE_STATE_NOT_INITIALIZED:
            // Device is disconnected.
            status = LORA_MAC_STATUS_DEVICE_OFF;
            break;
        case DEVICE_STATE_INIT:
            status = LORA_MAC_STATUS_OK;
            break;
        case DEVICE_STATE_JOINING:
            if (_lw_session.connection.connect_type == LORAWAN_CONNECTION_OTAA) {
                /*
                 * OTAA join
                 */
                tr_debug("Send Join-request..");
                lora_mac_mlme_req_t mlme_req;
                mlme_req.type = LORA_MLME_JOIN;

                mlme_req.req.join.dev_eui = _lw_session.connection.connection_u.otaa.dev_eui;
                mlme_req.req.join.app_eui = _lw_session.connection.connection_u.otaa.app_eui;
                mlme_req.req.join.app_key = _lw_session.connection.connection_u.otaa.app_key;
                mlme_req.req.join.nb_trials = _lw_session.connection.connection_u.otaa.nb_trials;

                // Send join request to server.
                status = mlme_request_handler(&mlme_req);
                if (status != LORA_MAC_STATUS_OK) {
                    return status;
                }
                // Otherwise request was successful and OTAA connect is in
                //progress
                return LORA_MAC_STATUS_CONNECT_IN_PROGRESS;
            } else {
                status = LORA_MAC_STATUS_PARAMETER_INVALID;
                break;
            }
            break;
        case DEVICE_STATE_JOINED:
            tr_debug("Join OK!");
            // Session is now active
            _lw_session.active = true;
            // Tell the application that we are connected
            if (_callbacks.events) {
                _queue->call(_callbacks.events, CONNECTED);
            }
            break;
        case DEVICE_STATE_ABP_CONNECTING:
            /*
             * ABP connection
             */
            mib_req.type = LORA_MIB_NET_ID;
            mib_req.param.net_id = _lw_session.connection.connection_u.abp.nwk_id;
            mib_set_request(&mib_req);

            mib_req.type = LORA_MIB_DEV_ADDR;
            mib_req.param.dev_addr = _lw_session.connection.connection_u.abp.dev_addr;
            mib_set_request(&mib_req);

            mib_req.type = LORA_MIB_NWK_SKEY;
            mib_req.param.nwk_skey = _lw_session.connection.connection_u.abp.nwk_skey;
            mib_set_request(&mib_req);

            mib_req.type = LORA_MIB_APP_SKEY;
            mib_req.param.app_skey = _lw_session.connection.connection_u.abp.app_skey;
            mib_set_request(&mib_req);

            mib_req.type = LORA_MIB_NETWORK_JOINED;
            mib_req.param.is_network_joined = true;
            mib_set_request(&mib_req);
            tr_debug("ABP Connection OK!");
            // tell the application we are okay
            // if users provide wrong keys, it's their responsibility
            // there is no way to test ABP authentication success
            status = LORA_MAC_STATUS_OK;
            // Session is now active
            _lw_session.active = true;
            if (_callbacks.events) {
                _queue->call(_callbacks.events, CONNECTED);
            }
            break;
        case DEVICE_STATE_SEND:
            // If a transmission is ongoing, don't interrupt
            if (_tx_msg.tx_ongoing) {
                status = LORA_MAC_STATUS_OK;
            } else {
                _tx_msg.tx_ongoing = true;
                status = send_frame_to_mac();

                switch (status) {
                    case LORA_MAC_STATUS_OK:
                        tr_debug("Frame scheduled to TX..");
                        break;
                    case LORA_MAC_STATUS_CRYPTO_FAIL:
                        tr_error("Crypto failed. Clearing TX buffers");
                        if (_callbacks.events) {
                            _queue->call(_callbacks.events, TX_CRYPTO_ERROR);
                        }
                        break;
                    default:
                        tr_error("Failure to schedule TX!");
                        if (_callbacks.events) {
                            _queue->call(_callbacks.events, TX_SCHEDULING_ERROR);
                        }
                        break;
                }
            }
            // otherwise all done, put device in idle state
            set_device_state(DEVICE_STATE_IDLE);
            break;
        case DEVICE_STATE_IDLE:
            //Do nothing
            status = LORA_MAC_STATUS_IDLE;
            break;
        case DEVICE_STATE_COMPLIANCE_TEST:
            //Device is in compliance test mode
            tr_debug("Device is in compliance test mode.");

            //5000ms
            LoRaMacSetTxTimer(5000);
            if (_compliance_test.running == true) {
                send_compliance_test_frame_to_mac();
            }
            status = LORA_MAC_STATUS_COMPLIANCE_TEST_ON;
            break;
        default:
            status = LORA_MAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return status;
}

static Mcps_t interpret_mcps_confirm_type(const lora_mac_mcps_t& local)
{
    switch (local) {
        case LORA_MCPS_UNCONFIRMED:
            return MCPS_UNCONFIRMED;
        case LORA_MCPS_CONFIRMED:
            return MCPS_CONFIRMED;
        case LORA_MCPS_MULTICAST:
            return MCPS_MULTICAST;
        case LORA_MCPS_PROPRIETARY:
            return MCPS_PROPRIETARY;
        default:
            MBED_ASSERT("Unknown Mcps_t");
    }

    // Never reaches here
    return MCPS_UNCONFIRMED;
}

static lora_mac_event_info_status_t interpret_event_info_type(const LoRaMacEventInfoStatus_t& remote)
{
    switch (remote) {
        case LORAMAC_EVENT_INFO_STATUS_OK:
            return LORA_EVENT_INFO_STATUS_OK;
        case LORAMAC_EVENT_INFO_STATUS_ERROR:
            return LORA_EVENT_INFO_STATUS_ERROR;
        case LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT:
            return LORA_EVENT_INFO_STATUS_TX_TIMEOUT;
        case LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT:
            return LORA_EVENT_INFO_STATUS_RX1_TIMEOUT;
        case LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT:
            return LORA_EVENT_INFO_STATUS_RX2_TIMEOUT;
        case LORAMAC_EVENT_INFO_STATUS_RX1_ERROR:
            return LORA_EVENT_INFO_STATUS_RX1_ERROR;
        case LORAMAC_EVENT_INFO_STATUS_RX2_ERROR:
            return LORA_EVENT_INFO_STATUS_RX2_ERROR;
        case LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL:
            return LORA_EVENT_INFO_STATUS_JOIN_FAIL;
        case LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED:
            return LORA_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
        case LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR:
            return LORA_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR;
        case LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS:
            return LORA_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS;
        case LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL:
            return LORA_EVENT_INFO_STATUS_ADDRESS_FAIL;
        case LORAMAC_EVENT_INFO_STATUS_MIC_FAIL:
            return LORA_EVENT_INFO_STATUS_MIC_FAIL;
        case LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL:
            return LORA_EVENT_INFO_STATUS_CRYPTO_FAIL;
        default:
            MBED_ASSERT("Unknown LoRaMacEventInfoStatus_t");
    }

    // Never reaches here actually
    return LORA_EVENT_INFO_STATUS_OK;
}

static Mib_t interpret_mib_req_confirm_type(const lora_mac_mib_t& local)
{
    switch (local) {
        case LORA_MIB_DEVICE_CLASS:
            return MIB_DEVICE_CLASS;
        case LORA_MIB_NETWORK_JOINED:
            return MIB_NETWORK_JOINED;
        case LORA_MIB_ADR:
            return MIB_ADR;
        case LORA_MIB_NET_ID:
            return MIB_NET_ID;
        case LORA_MIB_DEV_ADDR:
            return MIB_DEV_ADDR;
        case LORA_MIB_NWK_SKEY:
            return MIB_NWK_SKEY;
        case LORA_MIB_APP_SKEY:
            return MIB_APP_SKEY;
        case LORA_MIB_PUBLIC_NETWORK:
            return MIB_PUBLIC_NETWORK;
        case LORA_MIB_REPEATER_SUPPORT:
            return MIB_REPEATER_SUPPORT;
        case LORA_MIB_CHANNELS:
            return MIB_CHANNELS;
        case LORA_MIB_RX2_CHANNEL:
            return MIB_RX2_CHANNEL;
        case LORA_MIB_RX2_DEFAULT_CHANNEL:
            return MIB_RX2_DEFAULT_CHANNEL;
        case LORA_MIB_CHANNELS_MASK:
            return MIB_CHANNELS_MASK;
        case LORA_MIB_CHANNELS_DEFAULT_MASK:
            return MIB_CHANNELS_DEFAULT_MASK;
        case LORA_MIB_CHANNELS_NB_REP:
            return MIB_CHANNELS_NB_REP;
        case LORA_MIB_MAX_RX_WINDOW_DURATION:
            return MIB_MAX_RX_WINDOW_DURATION;
        case LORA_MIB_RECEIVE_DELAY_1:
            return MIB_RECEIVE_DELAY_1;
        case LORA_MIB_RECEIVE_DELAY_2:
            return MIB_RECEIVE_DELAY_2;
        case LORA_MIB_JOIN_ACCEPT_DELAY_1:
            return MIB_JOIN_ACCEPT_DELAY_1;
        case LORA_MIB_JOIN_ACCEPT_DELAY_2:
            return MIB_JOIN_ACCEPT_DELAY_2;
        case LORA_MIB_CHANNELS_DEFAULT_DATARATE:
            return MIB_CHANNELS_DEFAULT_DATARATE;
        case LORA_MIB_CHANNELS_DATARATE:
            return MIB_CHANNELS_DATARATE;
        case LORA_MIB_CHANNELS_TX_POWER:
            return MIB_CHANNELS_TX_POWER;
        case LORA_MIB_CHANNELS_DEFAULT_TX_POWER:
            return MIB_CHANNELS_DEFAULT_TX_POWER;
        case LORA_MIB_UPLINK_COUNTER:
            return MIB_UPLINK_COUNTER;
        case LORA_MIB_DOWNLINK_COUNTER:
            return MIB_DOWNLINK_COUNTER;
        case LORA_MIB_MULTICAST_CHANNEL:
            return MIB_MULTICAST_CHANNEL;
        case LORA_MIB_SYSTEM_MAX_RX_ERROR:
            return MIB_SYSTEM_MAX_RX_ERROR;
        case LORA_MIB_MIN_RX_SYMBOLS:
            return MIB_MIN_RX_SYMBOLS;
        default:
            MBED_ASSERT("Cannot Interpret Mib_t");
    }

    // Never actually reaches here
    return MIB_DEVICE_CLASS;
}

