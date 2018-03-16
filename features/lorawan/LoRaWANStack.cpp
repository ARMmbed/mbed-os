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

using namespace mbed;
using namespace events;

#if defined(LORAWAN_COMPLIANCE_TEST)
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

lorawan_status_t LoRaWANStack::set_application_port(uint8_t port)
{
    if (is_port_valid(port)) {
        _app_port = port;
        return LORAWAN_STATUS_OK;
    }

    return LORAWAN_STATUS_PORT_INVALID;
}

/*****************************************************************************
 * Constructor and destructor                                                *
 ****************************************************************************/
LoRaWANStack::LoRaWANStack()
: _loramac(_lora_time), _lora_phy(_lora_time),
  _device_current_state(DEVICE_STATE_NOT_INITIALIZED), _mac_handlers(NULL),
  _num_retry(1), _queue(NULL), _duty_cycle_on(MBED_CONF_LORA_DUTY_CYCLE_ON)
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

     LoRaMacPrimitives.mcps_confirm     = callback(this, &LoRaWANStack::mcps_confirm_handler);
     LoRaMacPrimitives.mcps_indication  = callback(this, &LoRaWANStack::mcps_indication_handler);
     LoRaMacPrimitives.mlme_confirm     = callback(this, &LoRaWANStack::mlme_confirm_handler);
     LoRaMacPrimitives.mlme_indication  = callback(this, &LoRaWANStack::mlme_indication_handler);
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
    _mac_handlers = _loramac.get_phy_event_handlers();
    //  passes the reference to radio driver down to PHY layer
    _lora_phy.set_radio_instance(radio);
    return _mac_handlers;
}

lorawan_status_t LoRaWANStack::initialize_mac_layer(EventQueue *queue)
{
    if (DEVICE_STATE_NOT_INITIALIZED != _device_current_state)
    {
        tr_debug("Initialized already");
        return LORAWAN_STATUS_OK;
    }

    tr_debug("Initializing MAC layer");

    //store a pointer to Event Queue
    _queue = queue;

#if defined(LORAWAN_COMPLIANCE_TEST)
    _compliance_test.app_data_buffer = compliance_test_buffer;
#endif

    _lora_time.activate_timer_subsystem(queue);
    _loramac.initialize(&LoRaMacPrimitives, &_lora_phy, queue);

    loramac_mib_req_confirm_t mib_req;

    mib_req.type = MIB_ADR;
    mib_req.param.is_adr_enable = MBED_CONF_LORA_ADR_ON;
    mib_set_request(&mib_req);

    mib_req.type = MIB_PUBLIC_NETWORK;
    mib_req.param.enable_public_nwk = MBED_CONF_LORA_PUBLIC_NETWORK;
    mib_set_request(&mib_req);

    // Reset counters to zero. Will change in future with 1.1 support.
    _lw_session.downlink_counter = 0;
    _lw_session.uplink_counter = 0;

    // Start loRaWAN state machine.
    set_device_state(DEVICE_STATE_INIT);
    return lora_state_machine();
}

#if defined(LORAWAN_COMPLIANCE_TEST)
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
                for(uint8_t i = 1; i < MIN(_compliance_test.app_data_size, MBED_CONF_LORA_TX_MAX_SIZE); ++i) {
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
lorawan_status_t LoRaWANStack::send_compliance_test_frame_to_mac()
{
    loramac_mcps_req_t mcps_req;

    get_phy_params_t phy_params;
    phy_param_t default_datarate;
    phy_params.attribute = PHY_DEF_TX_DR;
    default_datarate = _lora_phy.get_phy_params(&phy_params);

    prepare_special_tx_frame(_compliance_test.app_port);

    if (!_compliance_test.is_tx_confirmed) {
        mcps_req.type = MCPS_UNCONFIRMED;
        mcps_req.req.unconfirmed.fport = _compliance_test.app_port;
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.unconfirmed.data_rate = default_datarate.value;

        tr_info("Transmit unconfirmed compliance test frame %d bytes.", mcps_req.f_buffer_size);

        for (uint8_t i = 0; i < mcps_req.f_buffer_size; ++i) {
            tr_info("Byte %d, data is 0x%x", i+1, ((uint8_t*)mcps_req.f_buffer)[i]);
        }
    } else if (_compliance_test.is_tx_confirmed) {
        mcps_req.type = MCPS_CONFIRMED;
        mcps_req.req.confirmed.fport = _compliance_test.app_port;
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.confirmed.nb_trials = _num_retry;
        mcps_req.req.confirmed.data_rate = default_datarate.value;

        tr_info("Transmit confirmed compliance test frame %d bytes.", mcps_req.f_buffer_size);

        for (uint8_t i = 0; i < mcps_req.f_buffer_size; ++i) {
            tr_info("Byte %d, data is 0x%x", i+1, ((uint8_t*)mcps_req.f_buffer)[i]);
        }
    } else {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    return mcps_request_handler(&mcps_req);
}
#endif

uint16_t LoRaWANStack::check_possible_tx_size(uint16_t size)
{
    loramac_tx_info_t tx_info;
    if (_loramac.query_tx_possible(size, &tx_info) == LORAWAN_STATUS_LENGTH_ERROR) {
        // Cannot transmit this much. Return how much data can be sent
        // at the moment
        return tx_info.max_possible_payload_size;
    }

    return tx_info.current_payload_size;
}

/** Hands over the frame to MAC layer
 *
 * \return          returns the state of the LoRa MAC
 */
lorawan_status_t LoRaWANStack::send_frame_to_mac()
{
    loramac_mcps_req_t mcps_req;
    lorawan_status_t status;
    loramac_mib_req_confirm_t mib_get_params;

    get_phy_params_t phy_params;
    phy_param_t default_datarate;
    phy_params.attribute = PHY_DEF_TX_DR;
    default_datarate = _lora_phy.get_phy_params(&phy_params);

    mcps_req.type = _tx_msg.type;

    if (MCPS_UNCONFIRMED == mcps_req.type) {
        mcps_req.req.unconfirmed.fport = _tx_msg.message_u.unconfirmed.fport;
        mcps_req.f_buffer = _tx_msg.f_buffer;

        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;

        mib_get_params.type = MIB_CHANNELS_DATARATE;
        if(mib_get_request(&mib_get_params) != LORAWAN_STATUS_OK) {
            tr_debug("Couldn't get MIB parameters: Using default data rate");
            mcps_req.req.unconfirmed.data_rate = default_datarate.value;
        } else {
            mcps_req.req.unconfirmed.data_rate = mib_get_params.param.channel_data_rate;
        }

    } else if (mcps_req.type == MCPS_CONFIRMED) {
        mcps_req.req.confirmed.fport = _tx_msg.message_u.confirmed.fport;
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;
        mcps_req.req.confirmed.nb_trials = _tx_msg.message_u.confirmed.nb_trials;

        mib_get_params.type = MIB_CHANNELS_DATARATE;
        if(mib_get_request(&mib_get_params) != LORAWAN_STATUS_OK) {
            tr_debug("Couldn't get MIB parameters: Using default data rate");
            mcps_req.req.confirmed.data_rate = default_datarate.value;
        } else {
            mcps_req.req.confirmed.data_rate = mib_get_params.param.channel_data_rate;
        }

    } else if ( mcps_req.type == MCPS_PROPRIETARY) {
        mcps_req.f_buffer = _tx_msg.f_buffer;
        mcps_req.f_buffer_size = _tx_msg.f_buffer_size;

        mib_get_params.type = MIB_CHANNELS_DATARATE;
        if(mib_get_request(&mib_get_params) != LORAWAN_STATUS_OK) {
            tr_debug("Couldn't get MIB parameters: Using default data rate");
            mcps_req.req.proprietary.data_rate = default_datarate.value;
        } else {
            mcps_req.req.proprietary.data_rate = mib_get_params.param.channel_data_rate;
        }

    } else {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    status = mcps_request_handler(&mcps_req);

    return status;
}

lorawan_status_t LoRaWANStack::set_confirmed_msg_retry(uint8_t count)
{
    if (count >= MAX_CONFIRMED_MSG_RETRIES) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    _num_retry = count;

    return LORAWAN_STATUS_OK;
}

void LoRaWANStack::set_device_state(device_states_t new_state)
{
    _device_current_state = new_state;
}

/*!
 * \brief   MLME-Indication event function
 *
 * \param   [IN] mlmeIndication - Pointer to the indication structure.
 */
void LoRaWANStack::mlme_indication_handler(loramac_mlme_indication_t *mlmeIndication)
{
    switch( mlmeIndication->indication_type )
    {
        case MLME_SCHEDULE_UPLINK:
        {// The MAC signals that we shall provide an uplink as soon as possible
            // TODO: Sending implementation missing and will be implemented using
            //       another task.
            //OnTxNextPacketTimerEvent( );
            break;
        }
        default:
            break;
    }
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

        if (cbs->battery_level) {
            _callbacks.battery_level = cbs->battery_level;
        }
    }
}

lorawan_status_t LoRaWANStack::add_channels(const lorawan_channelplan_t &channel_plan)
{
    // If device is not initialized, stop right away
    if (_device_current_state == DEVICE_STATE_NOT_INITIALIZED) {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    return _loramac.add_channel_plan(channel_plan);
}

lorawan_status_t LoRaWANStack::drop_channel_list()
{
    if (_device_current_state == DEVICE_STATE_NOT_INITIALIZED) {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    return _loramac.remove_channel_plan();
}

lorawan_status_t LoRaWANStack::remove_a_channel(uint8_t channel_id)
{
    if (_device_current_state == DEVICE_STATE_NOT_INITIALIZED )
    {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    return _loramac.remove_single_channel(channel_id);
}

lorawan_status_t LoRaWANStack::get_enabled_channels(lorawan_channelplan_t& channel_plan)
{
    if (_device_current_state == DEVICE_STATE_JOINING
            || _device_current_state == DEVICE_STATE_NOT_INITIALIZED
            || _device_current_state == DEVICE_STATE_INIT)
    {
        tr_error("Cannot get channel plan until Joined!");
        return LORAWAN_STATUS_BUSY;
    }

  return _loramac.get_channel_plan(channel_plan);
}

lorawan_status_t LoRaWANStack::enable_adaptive_datarate(bool adr_enabled)
{
    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state)
    {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    loramac_mib_req_confirm_t adr_mib_params;

    adr_mib_params.type = MIB_ADR;
    adr_mib_params.param.is_adr_enable = adr_enabled;

    return mib_set_request(&adr_mib_params);
}

lorawan_status_t LoRaWANStack::set_channel_data_rate(uint8_t data_rate)
{
    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state)
    {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    loramac_mib_req_confirm_t mib_params;
    mib_params.type = MIB_ADR;
    if (mib_get_request(&mib_params) != LORAWAN_STATUS_OK) {
        tr_error("Cannot set data rate. Please turn off ADR first.");
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    mib_params.type = MIB_CHANNELS_DATARATE;
    mib_params.param.channel_data_rate = data_rate;

    return mib_set_request(&mib_params);
}

void LoRaWANStack::commission_device(const lorawan_dev_commission_t &commission_data)
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
lorawan_status_t LoRaWANStack::join_request_by_otaa(const lorawan_connect_t &params)
{
    lorawan_dev_commission_t commission;

    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state)
    {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
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
lorawan_status_t LoRaWANStack::activation_by_personalization(const lorawan_connect_t &params)
{
    lorawan_status_t status;
    lorawan_dev_commission_t commission;

    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state) {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
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
        return LORAWAN_STATUS_NO_ACTIVE_SESSIONS;
    }

    if (_tx_msg.tx_ongoing) {
        return LORAWAN_STATUS_WOULD_BLOCK;
    }

    if (!data && length > 0) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

#if defined(LORAWAN_COMPLIANCE_TEST)
    if (_compliance_test.running) {
        return LORAWAN_STATUS_COMPLIANCE_TEST_ON;
    }
#endif

    loramac_mib_req_confirm_t mib_req;
    lorawan_status_t status;
    mib_req.type = MIB_NETWORK_JOINED;
    status = mib_get_request(&mib_req);

    if (status == LORAWAN_STATUS_OK) {
        if (mib_req.param.is_nwk_joined == false) {
            return LORAWAN_STATUS_NO_NETWORK_JOINED;
        }
    }

    status = set_application_port(port);

    if (status != LORAWAN_STATUS_OK) {
        tr_error("Illegal application port definition.");
        return status;
    }

    if (flags == 0
            || (flags & MSG_FLAG_MASK) == (MSG_CONFIRMED_FLAG|MSG_UNCONFIRMED_FLAG)) {
        tr_error("CONFIRMED and UNCONFIRMED are mutually exclusive for send()");
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    _tx_msg.port = port;

    uint16_t max_possible_size = check_possible_tx_size(length);

    if (max_possible_size > MBED_CONF_LORA_TX_MAX_SIZE) {
        // LORAWAN_APP_DATA_MAX_SIZE should at least be
        // either equal to or bigger than maximum possible
        // tx size because our tx message buffer takes its
        // length from that macro. Force maximum possible tx unit
        // to be equal to the buffer size user chose.
        max_possible_size = MBED_CONF_LORA_TX_MAX_SIZE;
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
        if (length > 0) {
            memcpy(_tx_msg.f_buffer, data, length);
        }
    }

    // Handles all unconfirmed messages, including proprietary and multicast
    if ((flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_FLAG
            || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_MULTICAST
            || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_PROPRIETARY) {

         _tx_msg.type = MCPS_UNCONFIRMED;
         _tx_msg.message_u.unconfirmed.fport = _app_port;
    }

    // Handles all confirmed messages, including proprietary and multicast
    if ((flags & MSG_FLAG_MASK) == MSG_CONFIRMED_FLAG
            || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_MULTICAST
            || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_PROPRIETARY) {

        _tx_msg.type = MCPS_CONFIRMED;
        _tx_msg.message_u.confirmed.fport = _app_port;
        _tx_msg.message_u.confirmed.nb_trials = _num_retry;
    }

    tr_info("RTS = %u bytes, PEND = %u", _tx_msg.f_buffer_size, _tx_msg.pending_size);
    set_device_state(DEVICE_STATE_SEND);
    status = lora_state_machine();

    // send user the length of data which is scheduled now.
    // user should take care of the pending data.
    return (status == LORAWAN_STATUS_OK) ? _tx_msg.f_buffer_size : (int16_t) status;
}

int16_t LoRaWANStack::handle_rx(const uint8_t port, uint8_t* data,
                                uint16_t length, uint8_t flags)
{
    if (!_lw_session.active) {
        return LORAWAN_STATUS_NO_ACTIVE_SESSIONS;
    }

    // No messages to read.
    if (!_rx_msg.receive_ready) {
        return LORAWAN_STATUS_WOULD_BLOCK;
    }

#if defined(LORAWAN_COMPLIANCE_TEST)
    if (_compliance_test.running) {
        return LORAWAN_STATUS_COMPLIANCE_TEST_ON;
    }
#endif

    if (data == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    uint8_t *base_ptr = _rx_msg.msg.mcps_indication.buffer;
    uint16_t base_size = _rx_msg.msg.mcps_indication.buffer_size;
    bool read_complete = false;

    if (_rx_msg.msg.mcps_indication.port != port) {
        // Nothing yet received for this particular port
        return LORAWAN_STATUS_WOULD_BLOCK;
    }

    // check if message received is a Confirmed message and user subscribed to it or not
    if (_rx_msg.msg.mcps_indication.type == MCPS_CONFIRMED
            && ((flags & MSG_FLAG_MASK) == MSG_CONFIRMED_FLAG
                    || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_MULTICAST
                    || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_PROPRIETARY)) {

        tr_debug("RX - Confirmed Message, flags=%d", flags);
    }

    // check if message received is a Unconfirmed message and user subscribed to it or not
    if (_rx_msg.msg.mcps_indication.type == MCPS_UNCONFIRMED
            && ((flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_FLAG
                    || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_MULTICAST
                    || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_PROPRIETARY)) {
        tr_debug("RX - Unconfirmed Message - flags=%d", flags);
    }

    // check the length of received message whether we can fit into user
    // buffer completely or not
    if (_rx_msg.msg.mcps_indication.buffer_size > length &&
            _rx_msg.prev_read_size == 0) {
        // we can't fit into user buffer. Invoke counter measures
        _rx_msg.pending_size = _rx_msg.msg.mcps_indication.buffer_size - length;
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
        memset(_rx_msg.msg.mcps_indication.buffer, 0, LORAMAC_PHY_MAXPAYLOAD);
        _rx_msg.receive_ready = false;
    }

    return base_size;
}

lorawan_status_t LoRaWANStack::mlme_request_handler(loramac_mlme_req_t *mlme_request)
{
    if (mlme_request == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    return _loramac.mlme_request(mlme_request);
}

/** MLME-Confirm event function
 *
 * \param mlme_confirm      Pointer to the confirm structure,
 *                          containing confirm attributes.
 */
void LoRaWANStack::mlme_confirm_handler(loramac_mlme_confirm_t *mlme_confirm)
{
    if (NULL == mlme_confirm) {
        tr_error("mlme_confirm: struct [in] is null!");
        MBED_ASSERT(0);
        return;
    }

    switch (mlme_confirm->req_type) {
        case MLME_JOIN:
            if (mlme_confirm->status == LORAMAC_EVENT_INFO_STATUS_OK) {
                // Status is OK, node has joined the network
                set_device_state(DEVICE_STATE_JOINED);
                if (lora_state_machine() != LORAWAN_STATUS_OK) {
                    tr_error("Lora state machine did not return LORAWAN_STATUS_OK");
                }
            } else {
                // Join attempt failed.
                set_device_state(DEVICE_STATE_IDLE);
                if (lora_state_machine() != LORAWAN_STATUS_IDLE) {
                    tr_error("Lora state machine did not return DEVICE_STATE_IDLE !");
                }

                if (_callbacks.events) {
                    const int ret = _queue->call(_callbacks.events, JOIN_FAILURE);
                    MBED_ASSERT(ret != 0);
                    (void)ret;
                }
            }
            break;
        case MLME_LINK_CHECK:
            if (mlme_confirm->status == LORAMAC_EVENT_INFO_STATUS_OK) {
                // Check DemodMargin
                // Check NbGateways
#if defined(LORAWAN_COMPLIANCE_TEST)
                if (_compliance_test.running == true) {
                    _compliance_test.link_check = true;
                    _compliance_test.demod_margin = mlme_confirm->demod_margin;
                    _compliance_test.nb_gateways = mlme_confirm->nb_gateways;
                } else
#endif
                {
                    // normal operation as oppose to compliance testing
                    if (_callbacks.link_check_resp) {
                        const int ret = _queue->call(_callbacks.link_check_resp,
                                                     mlme_confirm->demod_margin,
                                                     mlme_confirm->nb_gateways);
                        MBED_ASSERT(ret != 0);
                        (void)ret;
                    }
                }
            }
            break;
        default:
            break;
    }
}

lorawan_status_t LoRaWANStack::mcps_request_handler(loramac_mcps_req_t *mcps_request)
{
    if (mcps_request == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    return _loramac.mcps_request(mcps_request);
}

/** MCPS-Confirm event function
 *
 * \param mcps_confirm      Pointer to the confirm structure,
 *                          containing confirm attributes.
 */
void LoRaWANStack::mcps_confirm_handler(loramac_mcps_confirm_t *mcps_confirm)
{
    if (mcps_confirm == NULL) {
        tr_error("mcps_confirm: struct [in] is null!");
        MBED_ASSERT(0);
        return;
    }

    if (mcps_confirm->status != LORAMAC_EVENT_INFO_STATUS_OK) {
        // Couldn't schedule packet, ack not recieved in CONFIRMED case
        // or some other error happened. Discard buffer, unset the tx-ongoing
        // flag and let the application know
        _tx_msg.tx_ongoing = false;
        memset(_tx_msg.f_buffer, 0, MBED_CONF_LORA_TX_MAX_SIZE);
        _tx_msg.f_buffer_size = MBED_CONF_LORA_TX_MAX_SIZE;

        tr_error("mcps_confirm_handler: Error code = %d", mcps_confirm->status);

        // If sending timed out, we have a special event for that
        if (mcps_confirm->status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT) {
            if (_callbacks.events) {
                const int ret = _queue->call(_callbacks.events, TX_TIMEOUT);
                MBED_ASSERT(ret != 0);
                (void)ret;
            }
            return;
        } if (mcps_confirm->status == LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT) {
            tr_debug("Did not receive Ack");
        }

        // Otherwise send a general TX_ERROR event
        if (_callbacks.events) {
            const int ret = _queue->call(_callbacks.events, TX_ERROR);
            MBED_ASSERT(ret != 0);
            (void)ret;
        }
        return;
    }

    // If No errors encountered, let's proceed with the status.
    // CONFIRMED needs special handling because of acks
    if (mcps_confirm->req_type == MCPS_CONFIRMED) {
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
    _lw_session.uplink_counter = mcps_confirm->ul_frame_counter;
    _tx_msg.tx_ongoing = false;
     if (_callbacks.events) {
         const int ret = _queue->call(_callbacks.events, TX_DONE);
         MBED_ASSERT(ret != 0);
         (void)ret;
     }
}

/** MCPS-Indication event function
 *
 * \param mcps_indication   Pointer to the indication structure,
 *                          containing indication attributes.
 */
void LoRaWANStack::mcps_indication_handler(loramac_mcps_indication_t *mcps_indication)
{
    if (mcps_indication == NULL) {
        tr_error("mcps_indication: struct [in] is null.");
        return;
    }

    if (mcps_indication->status != LORAMAC_EVENT_INFO_STATUS_OK) {
        if (_callbacks.events) {
            const int ret = _queue->call(_callbacks.events, RX_ERROR);
            MBED_ASSERT(ret != 0);
            (void)ret;
        }
        return;
    }

    switch (mcps_indication->type) {
        case MCPS_UNCONFIRMED:
            break;
        case MCPS_CONFIRMED:
            break;
        case MCPS_PROPRIETARY:
            break;
        case MCPS_MULTICAST:
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

#if defined(LORAWAN_COMPLIANCE_TEST)
    if (_compliance_test.running == true) {
        _compliance_test.downlink_counter++;
    }
#endif

    if (mcps_indication->is_data_recvd == true) {
        switch (mcps_indication->port) {
        case 224:
#if defined(LORAWAN_COMPLIANCE_TEST)
            tr_debug("Compliance test command received.");
            compliance_test_handler(mcps_indication);
#else
            tr_debug("Compliance test disabled.");
#endif
            break;
        default:
            if (is_port_valid(mcps_indication->port) == true ||
                mcps_indication->type == MCPS_PROPRIETARY) {

                // Valid message arrived.
                _rx_msg.type = LORAMAC_RX_MCPS_INDICATION;
                _rx_msg.msg.mcps_indication.buffer_size = mcps_indication->buffer_size;
                _rx_msg.msg.mcps_indication.port = mcps_indication->port;

                // no copy, just set the pointer for the user
                _rx_msg.msg.mcps_indication.buffer =
                mcps_indication->buffer;

                // Notify application about received frame..
                tr_debug("Received %d bytes", _rx_msg.msg.mcps_indication.buffer_size);
                _rx_msg.receive_ready = true;

                if (_callbacks.events) {
                    const int ret = _queue->call(_callbacks.events, RX_DONE);
                    MBED_ASSERT(ret != 0);
                    (void)ret;
                }

                // If fPending bit is set we try to generate an empty packet
                // with CONFIRMED flag set. We always set a CONFIRMED flag so
                // that we could retry a certain number of times if the uplink
                // failed for some reason
                if (mcps_indication->fpending_status) {
                    handle_tx(mcps_indication->port, NULL, 0, MSG_CONFIRMED_FLAG);
                }
            } else {
                // Invalid port, ports 0, 224 and 225-255 are reserved.
            }
            break;
        }
    }
}

#if defined(LORAWAN_COMPLIANCE_TEST)
/** Compliance testing function
 *
 * \param mcps_indication   Pointer to the indication structure,
 *                          containing indication attributes.
 */
void LoRaWANStack::compliance_test_handler(loramac_mcps_indication_t *mcps_indication)
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

            loramac_mib_req_confirm_t mib_req;
            mib_req.type = MIB_ADR;
            mib_req.param.is_adr_enable = true;
            mib_set_request(&mib_req);

#if MBED_CONF_LORA_PHY      == 0
            _loramac.LoRaMacTestSetDutyCycleOn(false);
#endif
            //5000ms
            _loramac.LoRaMacSetTxTimer(5000);
            set_device_state(DEVICE_STATE_COMPLIANCE_TEST);
            tr_debug("Compliance test activated.");
        }
    } else {
        _compliance_test.state = mcps_indication->buffer[0];
        switch (_compliance_test.state) {
        case 0: // Check compliance test disable command (ii)
            _compliance_test.is_tx_confirmed = true;
            _compliance_test.app_port = MBED_CONF_LORA_APP_PORT;
            _compliance_test.app_data_size = LORAWAN_COMPLIANCE_TEST_DATA_SIZE;
            _compliance_test.downlink_counter = 0;
            _compliance_test.running = false;

            loramac_mib_req_confirm_t mib_req;
            mib_req.type = MIB_ADR;
            mib_req.param.is_adr_enable = MBED_CONF_LORA_ADR_ON;
            mib_set_request(&mib_req);
#if MBED_CONF_LORA_PHY      == 0
            _loramac.LoRaMacTestSetDutyCycleOn(MBED_CONF_LORA_DUTY_CYCLE_ON);
#endif
            // Go to idle state after compliance test mode.
            tr_debug("Compliance test disabled.");
            _loramac.LoRaMacStopTxTimer();

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
            loramac_mlme_req_t mlme_req;
            mlme_req.type = MLME_LINK_CHECK;
            mlme_request_handler(&mlme_req);
            break;
        case 6: // (ix)
            loramac_mlme_req_t mlme_request;
            loramac_mib_req_confirm_t mib_request;

            // Disable TestMode and revert back to normal operation
            _compliance_test.is_tx_confirmed = true;
            _compliance_test.app_port = MBED_CONF_LORA_APP_PORT;
            _compliance_test.app_data_size = LORAWAN_COMPLIANCE_TEST_DATA_SIZE;
            _compliance_test.downlink_counter = 0;
            _compliance_test.running = false;

            mib_request.type = MIB_ADR;
            mib_request.param.is_adr_enable = MBED_CONF_LORA_ADR_ON;
            mib_set_request(&mib_request);
#if MBED_CONF_LORA_PHY      == 0
            _loramac.LoRaMacTestSetDutyCycleOn(MBED_CONF_LORA_DUTY_CYCLE_ON);
#endif
            mlme_request.type = MLME_JOIN;
            mlme_request.req.join.dev_eui = _lw_session.connection.connection_u.otaa.dev_eui;
            mlme_request.req.join.app_eui = _lw_session.connection.connection_u.otaa.app_eui;
            mlme_request.req.join.app_key = _lw_session.connection.connection_u.otaa.app_key;
            mlme_request.req.join.nb_trials = _lw_session.connection.connection_u.otaa.nb_trials;
            mlme_request_handler(&mlme_request);
            break;
        case 7: // (x)
            if (mcps_indication->buffer_size == 3) {
                loramac_mlme_req_t mlme_req;
                mlme_req.type = MLME_TXCW;
                mlme_req.req.cw_tx_mode.timeout = (uint16_t)((mcps_indication->buffer[1] << 8) | mcps_indication->buffer[2]);
                mlme_request_handler(&mlme_req);
            } else if (mcps_indication->buffer_size == 7) {
                loramac_mlme_req_t mlme_req;
                mlme_req.type = MLME_TXCW_1;
                mlme_req.req.cw_tx_mode.timeout = (uint16_t)((mcps_indication->buffer[1] << 8) | mcps_indication->buffer[2]);
                mlme_req.req.cw_tx_mode.frequency = (uint32_t)((mcps_indication->buffer[3] << 16) | (mcps_indication->buffer[4] << 8)
                        | mcps_indication->buffer[5]) * 100;
                mlme_req.req.cw_tx_mode.power = mcps_indication->buffer[6];
                mlme_request_handler(&mlme_req);
            }
            _compliance_test.state = 1;
            break;
        }
    }
}
#endif

lorawan_status_t LoRaWANStack::mib_set_request(loramac_mib_req_confirm_t *mib_set_params)
{
    if (NULL == mib_set_params) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }
    return _loramac.mib_set_request_confirm(mib_set_params);
}

lorawan_status_t LoRaWANStack::mib_get_request(loramac_mib_req_confirm_t *mib_get_params)
{
    if(NULL == mib_get_params) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }
    return _loramac.mib_get_request_confirm(mib_get_params);
}

lorawan_status_t LoRaWANStack::set_link_check_request()
{
    if (!_callbacks.link_check_resp) {
        tr_error("Must assign a callback function for link check request. ");
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    loramac_mlme_req_t mlme_req;

    mlme_req.type = MLME_LINK_CHECK;
    return mlme_request_handler(&mlme_req);
}

lorawan_status_t LoRaWANStack::shutdown()
{
    set_device_state(DEVICE_STATE_SHUTDOWN);
    return lora_state_machine();
}

lorawan_status_t LoRaWANStack::lora_state_machine()
{
    loramac_mib_req_confirm_t mib_req;
    lorawan_status_t status = LORAWAN_STATUS_DEVICE_OFF;

    switch (_device_current_state) {
        case DEVICE_STATE_SHUTDOWN:
            /*
             * Remove channels
             * Radio will be put to sleep by the APIs underneath
             */
            drop_channel_list();

            // Shutdown LoRaMac
            _loramac.disconnect();

            // Stop sending messages and set joined status to false.
#if defined(LORAWAN_COMPLIANCE_TEST)
            _loramac.LoRaMacStopTxTimer();
#endif
            mib_req.type = MIB_NETWORK_JOINED;
            mib_req.param.is_nwk_joined = false;
            mib_set_request(&mib_req);

            // reset buffers to original state
            memset(_tx_msg.f_buffer, 0, MBED_CONF_LORA_TX_MAX_SIZE);
            _tx_msg.pending_size = 0;
            _tx_msg.f_buffer_size = 0;
            _tx_msg.tx_ongoing = false;
            _rx_msg.msg.mcps_indication.buffer = NULL;
            _rx_msg.receive_ready = false;
            _rx_msg.prev_read_size = 0;
            _rx_msg.msg.mcps_indication.buffer_size = 0;

            // disable the session
            _lw_session.active = false;

            tr_debug("LoRaWAN protocol has been shut down.");
            if (_callbacks.events) {
                const int ret = _queue->call(_callbacks.events, DISCONNECTED);
                MBED_ASSERT(ret != 0);
                (void)ret;
            }
            status = LORAWAN_STATUS_DEVICE_OFF;
            break;
        case DEVICE_STATE_NOT_INITIALIZED:
            // Device is disconnected.
            status = LORAWAN_STATUS_DEVICE_OFF;
            break;
        case DEVICE_STATE_INIT:
            status = LORAWAN_STATUS_OK;
            break;
        case DEVICE_STATE_JOINING:
            if (_lw_session.connection.connect_type == LORAWAN_CONNECTION_OTAA) {
                /*
                 * OTAA join
                 */
                tr_debug("Send Join-request..");
                loramac_mlme_req_t mlme_req;
                mlme_req.type = MLME_JOIN;

                mlme_req.req.join.dev_eui = _lw_session.connection.connection_u.otaa.dev_eui;
                mlme_req.req.join.app_eui = _lw_session.connection.connection_u.otaa.app_eui;
                mlme_req.req.join.app_key = _lw_session.connection.connection_u.otaa.app_key;
                mlme_req.req.join.nb_trials = _lw_session.connection.connection_u.otaa.nb_trials;

                // Send join request to server.
                status = mlme_request_handler(&mlme_req);
                if (status != LORAWAN_STATUS_OK) {
                    return status;
                }
                // Otherwise request was successful and OTAA connect is in
                //progress
                return LORAWAN_STATUS_CONNECT_IN_PROGRESS;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        case DEVICE_STATE_JOINED:
            tr_debug("Join OK!");
            // Session is now active
            _lw_session.active = true;
            // Tell the application that we are connected
            if (_callbacks.events) {
                const int ret = _queue->call(_callbacks.events, CONNECTED);
                MBED_ASSERT(ret != 0);
                (void)ret;
            }
            status = LORAWAN_STATUS_OK;
            break;
        case DEVICE_STATE_ABP_CONNECTING:
            /*
             * ABP connection
             */
            mib_req.type = MIB_NET_ID;
            mib_req.param.net_id = _lw_session.connection.connection_u.abp.nwk_id;
            mib_set_request(&mib_req);

            mib_req.type = MIB_DEV_ADDR;
            mib_req.param.dev_addr = _lw_session.connection.connection_u.abp.dev_addr;
            mib_set_request(&mib_req);

            mib_req.type = MIB_NWK_SKEY;
            mib_req.param.nwk_skey = _lw_session.connection.connection_u.abp.nwk_skey;
            mib_set_request(&mib_req);

            mib_req.type = MIB_APP_SKEY;
            mib_req.param.app_skey = _lw_session.connection.connection_u.abp.app_skey;
            mib_set_request(&mib_req);

            mib_req.type = MIB_NETWORK_JOINED;
            mib_req.param.is_nwk_joined = true;
            mib_set_request(&mib_req);
            tr_debug("ABP Connection OK!");
            // tell the application we are okay
            // if users provide wrong keys, it's their responsibility
            // there is no way to test ABP authentication success
            status = LORAWAN_STATUS_OK;
            // Session is now active
            _lw_session.active = true;
            if (_callbacks.events) {
                const int ret = _queue->call(_callbacks.events, CONNECTED);
                MBED_ASSERT(ret != 0);
                (void)ret;
            }
            break;
        case DEVICE_STATE_SEND:
            // If a transmission is ongoing, don't interrupt
            if (_tx_msg.tx_ongoing) {
                status = LORAWAN_STATUS_OK;
            } else {
                _tx_msg.tx_ongoing = true;
                status = send_frame_to_mac();

                switch (status) {
                    case LORAWAN_STATUS_OK:
                        tr_debug("Frame scheduled to TX..");
                        break;
                    case LORAWAN_STATUS_CRYPTO_FAIL:
                        tr_error("Crypto failed. Clearing TX buffers");
                        if (_callbacks.events) {
                            const int ret = _queue->call(_callbacks.events, TX_CRYPTO_ERROR);
                            MBED_ASSERT(ret != 0);
                            (void)ret;
                        }
                        break;
                    default:
                        tr_error("Failure to schedule TX!");
                        if (_callbacks.events) {
                            const int ret = _queue->call(_callbacks.events, TX_SCHEDULING_ERROR);
                            MBED_ASSERT(ret != 0);
                            (void)ret;
                        }
                        break;
                }
            }
            // otherwise all done, put device in idle state
            set_device_state(DEVICE_STATE_IDLE);
            break;
        case DEVICE_STATE_IDLE:
            //Do nothing
            status = LORAWAN_STATUS_IDLE;
            break;
#if defined(LORAWAN_COMPLIANCE_TEST)
        case DEVICE_STATE_COMPLIANCE_TEST:
            //Device is in compliance test mode
            tr_debug("Device is in compliance test mode.");

            //5000ms
            _loramac.LoRaMacSetTxTimer(5000);
            if (_compliance_test.running == true) {
                send_compliance_test_frame_to_mac();
            }
            status = LORAWAN_STATUS_COMPLIANCE_TEST_ON;
            break;
#endif
        default:
            status = LORAWAN_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return status;
}
