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

#include "LoRaWANStack.h"
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
bool LoRaWANStack::is_port_valid(uint8_t port, bool allow_port_0)
{
    //Application should not use reserved and illegal port numbers.
    if (port == 0) {
        return allow_port_0;
    } else {
        return true;
    }
}

lorawan_status_t LoRaWANStack::set_application_port(uint8_t port, bool allow_port_0)
{
    if (is_port_valid(port, allow_port_0)) {
        _app_port = port;
        return LORAWAN_STATUS_OK;
    }

    return LORAWAN_STATUS_PORT_INVALID;
}

/*****************************************************************************
 * Constructor and destructor                                                *
 ****************************************************************************/
LoRaWANStack::LoRaWANStack()
: _loramac(),
  _device_current_state(DEVICE_STATE_NOT_INITIALIZED), _num_retry(1),
  _app_port(INVALID_PORT), _link_check_requested(false), _queue(NULL)
{
#ifdef MBED_CONF_LORA_APP_PORT
    if (is_port_valid(MBED_CONF_LORA_APP_PORT)) {
        _app_port = MBED_CONF_LORA_APP_PORT;
    } else {
        tr_error("User defined port in .json is illegal.");
    }
#endif

     memset(&_lw_session, 0, sizeof(_lw_session));
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

void LoRaWANStack::bind_radio_driver(LoRaRadio& radio)
{
    _loramac.bind_radio_driver(radio);
}

lorawan_status_t LoRaWANStack::connect()
{
    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state) {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    lorawan_status_t status = _loramac.prepare_join(NULL, MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION);

    if (LORAWAN_STATUS_OK != status) {
        return status;
    }

    return handle_connect(MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION);
}

lorawan_status_t LoRaWANStack::connect(const lorawan_connect_t &connect)
{
    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state) {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    if (!(connect.connect_type == LORAWAN_CONNECTION_OTAA) &&
        !(connect.connect_type == LORAWAN_CONNECTION_ABP)) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }
    bool is_otaa = (connect.connect_type == LORAWAN_CONNECTION_OTAA);

    lorawan_status_t status = _loramac.prepare_join(&connect, is_otaa);

    if (LORAWAN_STATUS_OK != status) {
        return status;
    }

    return handle_connect(is_otaa);
}

lorawan_status_t LoRaWANStack::handle_connect(bool is_otaa)
{
    device_states_t new_state;

    if (is_otaa) {
        tr_debug("Initiating OTAA");

        // As mentioned in the comment above, in 1.0.2 spec, counters are always set
        // to zero for new connection. This section is common for both normal and
        // connection restore at this moment. Will change in future with 1.1 support.
        _lw_session.downlink_counter = 0;
        _lw_session.uplink_counter = 0;
        new_state = DEVICE_STATE_JOINING;
    } else {
        // If current state is SHUTDOWN, device may be trying to re-establish
        // communication. In case of ABP specification is meddled about frame counters.
        // It says to reset counters to zero but there is no mechanism to tell the
        // network server that the device was disconnected or restarted.
        // At the moment, this implementation does not support a non-volatile
        // memory storage.
        //_lw_session.downlink_counter; //Get from NVM
        //_lw_session.uplink_counter; //Get from NVM

        tr_debug("Initiating ABP");
        tr_debug("Frame Counters. UpCnt=%lu, DownCnt=%lu",
                 _lw_session.uplink_counter, _lw_session.downlink_counter);
        new_state = DEVICE_STATE_ABP_CONNECTING;
    }

    return lora_state_machine(new_state);
}

lorawan_status_t LoRaWANStack::initialize_mac_layer(EventQueue *queue)
{
    if(!queue) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    if (DEVICE_STATE_NOT_INITIALIZED != _device_current_state)
    {
        tr_debug("Initialized already");
        return LORAWAN_STATUS_OK;
    }

    tr_debug("Initializing MAC layer");
    _queue = queue;

    _loramac.initialize(&LoRaMacPrimitives, queue);

    // Reset counters to zero. Will change in future with 1.1 support.
    _lw_session.downlink_counter = 0;
    _lw_session.uplink_counter = 0;

    return lora_state_machine(DEVICE_STATE_INIT);
}

lorawan_status_t LoRaWANStack::set_confirmed_msg_retry(uint8_t count)
{
    if (count >= MAX_CONFIRMED_MSG_RETRIES) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    _num_retry = count;

    return LORAWAN_STATUS_OK;
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
        {
            // The MAC signals that we shall provide an uplink as soon as possible
#if (MBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE)
            tr_debug("mlme indication: sending empty uplink to port 0 to acknowledge MAC commands...");
            send_automatic_uplink_message(0);
#else
            send_event_to_application(UPLINK_REQUIRED);
#endif
            break;
        }
        default:
            break;
    }
}

lorawan_status_t LoRaWANStack::set_lora_callbacks(lorawan_app_callbacks_t *cbs)
{
    if (!cbs || !cbs->events) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    _callbacks.events = cbs->events;

    if (cbs->link_check_resp) {
        _callbacks.link_check_resp = cbs->link_check_resp;
    }

    if (cbs->battery_level) {
        _callbacks.battery_level = cbs->battery_level;
    }

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::add_channels(const lorawan_channelplan_t &channel_plan)
{
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
    _loramac.enable_adaptive_datarate(adr_enabled);
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::set_channel_data_rate(uint8_t data_rate)
{
    if (DEVICE_STATE_NOT_INITIALIZED == _device_current_state)
    {
        tr_error("Stack not initialized!");
        return LORAWAN_STATUS_NOT_INITIALIZED;
    }

    return _loramac.set_channel_data_rate(data_rate);
}

void LoRaWANStack::send_event_to_application(const lorawan_event_t event) const
{
    if (_callbacks.events) {
        const int ret = _queue->call(_callbacks.events, event);
        MBED_ASSERT(ret != 0);
        (void)ret;
    }
}

void LoRaWANStack::send_automatic_uplink_message(const uint8_t port)
{
    const int16_t ret = handle_tx(port, NULL, 0, MSG_CONFIRMED_FLAG, true, true);
    if (ret < 0) {
        send_event_to_application(AUTOMATIC_UPLINK_ERROR);
    }
}

int16_t LoRaWANStack::handle_tx(uint8_t port, const uint8_t* data,
                                uint16_t length, uint8_t flags,
                                bool null_allowed, bool allow_port_0)
{
    if (!null_allowed && !data) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }
    // add a link check request with normal data, until the application
    // explicitly removes it.
    if (_link_check_requested) {
        set_link_check_request();
    }

    if (!_lw_session.active) {
        return LORAWAN_STATUS_NO_ACTIVE_SESSIONS;
    }

    if(_loramac.tx_ongoing()) {
        return LORAWAN_STATUS_WOULD_BLOCK;
    }

#if defined(LORAWAN_COMPLIANCE_TEST)
    if (_compliance_test.running) {
        return LORAWAN_STATUS_COMPLIANCE_TEST_ON;
    }
#endif

    lorawan_status_t status;

    if (_loramac.nwk_joined() == false) {
        return LORAWAN_STATUS_NO_NETWORK_JOINED;
    }

    status = set_application_port(port, allow_port_0);

    if (status != LORAWAN_STATUS_OK) {
        tr_error("Illegal application port definition.");
        return status;
    }

    if (flags == 0 ||
           (flags & MSG_FLAG_MASK) == (MSG_CONFIRMED_FLAG|MSG_UNCONFIRMED_FLAG)) {
        tr_error("CONFIRMED and UNCONFIRMED are mutually exclusive for send()");
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    int16_t len = _loramac.prepare_ongoing_tx(port, data, length, flags, _num_retry);

    status = lora_state_machine(DEVICE_STATE_SEND);

    // send user the length of data which is scheduled now.
    // user should take care of the pending data.
    return (status == LORAWAN_STATUS_OK) ? len : (int16_t) status;
}

int convert_to_msg_flag(const mcps_type_t type)
{
    int msg_flag = MSG_UNCONFIRMED_FLAG;
    switch (type) {
        case MCPS_UNCONFIRMED:
            msg_flag = MSG_UNCONFIRMED_FLAG;
            break;

        case MCPS_CONFIRMED:
            msg_flag = MSG_CONFIRMED_FLAG;
            break;

        case MCPS_MULTICAST:
            msg_flag = MSG_MULTICAST_FLAG;
            break;

        case MCPS_PROPRIETARY:
            msg_flag = MSG_PROPRIETARY_FLAG;
            break;

        default:
            tr_error("Unknown message type!");
            MBED_ASSERT(0);
    }

    return msg_flag;
}

int16_t LoRaWANStack::handle_rx(uint8_t* data, uint16_t length, uint8_t& port, int& flags, bool validate_params)
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

    if (data == NULL || length == 0) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    int received_flags = convert_to_msg_flag(_rx_msg.msg.mcps_indication.type);
    if (validate_params) {
        // Check received message port and flags match with the ones requested by user
        received_flags &= MSG_FLAG_MASK;

        if (_rx_msg.msg.mcps_indication.port != port || !(flags & received_flags)) {
            return LORAWAN_STATUS_WOULD_BLOCK;
        }
    }

    // Report values back to user
    port = _rx_msg.msg.mcps_indication.port;
    flags = received_flags;

    const uint8_t *base_ptr = _rx_msg.msg.mcps_indication.buffer;
    uint16_t base_size = _rx_msg.msg.mcps_indication.buffer_size;
    bool read_complete = false;

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

void LoRaWANStack::mlme_confirm_handler(loramac_mlme_confirm_t *mlme_confirm)
{
    switch (mlme_confirm->req_type) {
        case MLME_JOIN:
            if (mlme_confirm->status == LORAMAC_EVENT_INFO_STATUS_OK) {
                if (lora_state_machine(DEVICE_STATE_JOINED) != LORAWAN_STATUS_OK) {
                    tr_error("Lora state machine did not return LORAWAN_STATUS_OK");
                }
            } else {
                if (lora_state_machine(DEVICE_STATE_IDLE) != LORAWAN_STATUS_IDLE) {
                    tr_error("Lora state machine did not return DEVICE_STATE_IDLE !");
                }

                send_event_to_application(JOIN_FAILURE);
            }
            break;
        case MLME_LINK_CHECK:
            if (mlme_confirm->status == LORAMAC_EVENT_INFO_STATUS_OK) {
#if defined(LORAWAN_COMPLIANCE_TEST)
                if (_compliance_test.running == true) {
                    _compliance_test.link_check = true;
                    _compliance_test.demod_margin = mlme_confirm->demod_margin;
                    _compliance_test.nb_gateways = mlme_confirm->nb_gateways;
                } else
#endif
                {
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

void LoRaWANStack::mcps_confirm_handler(loramac_mcps_confirm_t *mcps_confirm)
{
    if (mcps_confirm->status != LORAMAC_EVENT_INFO_STATUS_OK) {
        _loramac.reset_ongoing_tx();

        tr_error("mcps_confirm_handler: Error code = %d", mcps_confirm->status);

        if (mcps_confirm->status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT) {
            send_event_to_application(TX_TIMEOUT);
            return;
        } else if (mcps_confirm->status == LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT) {
            tr_debug("Did not receive Ack");
        }

        send_event_to_application(TX_ERROR);
        return;
    }

    if (mcps_confirm->req_type == MCPS_CONFIRMED &&
        mcps_confirm->ack_received) {
            tr_debug("Ack received.");
    }

    _lw_session.uplink_counter = mcps_confirm->ul_frame_counter;
    _loramac.set_tx_ongoing(false);
    send_event_to_application(TX_DONE);
}

void LoRaWANStack::mcps_indication_handler(loramac_mcps_indication_t *mcps_indication)
{
    if (mcps_indication->status != LORAMAC_EVENT_INFO_STATUS_OK) {
        tr_error("RX_ERROR: mcps_indication status = %d", mcps_indication->status);
        send_event_to_application(RX_ERROR);
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

    //TODO:
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

    if (!mcps_indication->is_data_recvd) {
        return;
    }

    switch (mcps_indication->port) {
        case 224: {
#if defined(LORAWAN_COMPLIANCE_TEST)
            tr_debug("Compliance test command received.");
            compliance_test_handler(mcps_indication);
#else
            tr_info("Compliance test disabled.");
#endif
            break;
        }
        default: {
            if (is_port_valid(mcps_indication->port) == true ||
                    mcps_indication->type == MCPS_PROPRIETARY) {
                // Valid message arrived.
                _rx_msg.type = LORAMAC_RX_MCPS_INDICATION;
                _rx_msg.msg.mcps_indication.buffer_size = mcps_indication->buffer_size;
                _rx_msg.msg.mcps_indication.port = mcps_indication->port;
                _rx_msg.msg.mcps_indication.buffer = mcps_indication->buffer;
                _rx_msg.msg.mcps_indication.type = mcps_indication->type;

                // Notify application about received frame..
                tr_debug("Received %d bytes", _rx_msg.msg.mcps_indication.buffer_size);
                _rx_msg.receive_ready = true;

                send_event_to_application(RX_DONE);

                /*
                 * If fPending bit is set we try to generate an empty packet
                 * with CONFIRMED flag set. We always set a CONFIRMED flag so
                 * that we could retry a certain number of times if the uplink
                 * failed for some reason
                 * or
                 * Class C and node received a confirmed message so we need to
                 * send an empty packet to acknowledge the message.
                 * This scenario is unspecified by LoRaWAN 1.0.2 specification,
                 * but version 1.1.0 says that network SHALL not send any new
                 * confirmed messages until ack has been sent
                 */
                if ((_loramac.get_device_class() != CLASS_C && mcps_indication->fpending_status) ||
                    (_loramac.get_device_class() == CLASS_C && mcps_indication->type == MCPS_CONFIRMED)) {
#if (MBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE)
                    tr_debug("Sending empty uplink message...");
                    send_automatic_uplink_message(mcps_indication->port);
#else
                    send_event_to_application(UPLINK_REQUIRED);
#endif
                }
            } else {
                // Invalid port, ports 0, 224 and 225-255 are reserved.
            }
            break;
        }
    }
}

lorawan_status_t LoRaWANStack::set_link_check_request()
{
    _link_check_requested = true;
    if (!_callbacks.link_check_resp) {
        tr_error("Must assign a callback function for link check request. ");
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    _loramac.setup_link_check_request();
    return LORAWAN_STATUS_OK;
}

void LoRaWANStack::remove_link_check_request()
{
    _link_check_requested = false;
}

lorawan_status_t LoRaWANStack::shutdown()
{
    return lora_state_machine(DEVICE_STATE_SHUTDOWN);
}

lorawan_status_t LoRaWANStack::set_device_class(const device_class_t& device_class)
{
    if (device_class == CLASS_B) {
        return LORAWAN_STATUS_UNSUPPORTED;
    }
    _loramac.set_device_class(device_class);
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::lora_state_machine(device_states_t new_state)
{
    lorawan_status_t status = LORAWAN_STATUS_DEVICE_OFF;

    _device_current_state = new_state;

    switch (_device_current_state) {
        case DEVICE_STATE_SHUTDOWN:
            /*
             * Remove channels
             * Radio will be put to sleep by the APIs underneath
             */
            drop_channel_list();
            _loramac.disconnect();

#if defined(LORAWAN_COMPLIANCE_TEST)
            _loramac.LoRaMacStopTxTimer();
#endif
            _loramac.set_nwk_joined(false);

            _loramac.reset_ongoing_tx(true);

            _rx_msg.msg.mcps_indication.buffer = NULL;
            _rx_msg.receive_ready = false;
            _rx_msg.prev_read_size = 0;
            _rx_msg.msg.mcps_indication.buffer_size = 0;

            _lw_session.active = false;

            tr_debug("LoRaWAN protocol has been shut down.");
            send_event_to_application(DISCONNECTED);
            status = LORAWAN_STATUS_DEVICE_OFF;
            break;
        case DEVICE_STATE_NOT_INITIALIZED:
            status = LORAWAN_STATUS_DEVICE_OFF;
            break;
        case DEVICE_STATE_INIT:
            status = LORAWAN_STATUS_OK;
            break;
        case DEVICE_STATE_JOINING:
            if (_lw_session.connect_type == LORAWAN_CONNECTION_OTAA) {
                tr_debug("Send Join-request..");

                status = _loramac.join(true);
                if (status != LORAWAN_STATUS_OK) {
                    return status;
                }

                return LORAWAN_STATUS_CONNECT_IN_PROGRESS;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        case DEVICE_STATE_JOINED:
            tr_debug("Join OK!");

            _lw_session.active = true;

            send_event_to_application(CONNECTED);
            status = LORAWAN_STATUS_OK;
            break;
        case DEVICE_STATE_ABP_CONNECTING:

            _loramac.join(false);

            tr_debug("ABP Connection OK!");

            status = LORAWAN_STATUS_OK;

            _lw_session.active = true;
            send_event_to_application(CONNECTED);
            break;
        case DEVICE_STATE_SEND:
            if (_loramac.tx_ongoing()) {
                status = LORAWAN_STATUS_OK;
            } else {
                _loramac.set_tx_ongoing(true);
                status = _loramac.send_ongoing_tx();

                switch (status) {
                    case LORAWAN_STATUS_OK:
                        tr_debug("Frame scheduled to TX..");
                        break;
                    case LORAWAN_STATUS_CRYPTO_FAIL:
                        tr_error("Crypto failed. Clearing TX buffers");
                        send_event_to_application(TX_CRYPTO_ERROR);
                        break;
                    default:
                        tr_error("Failure to schedule TX!");
                        send_event_to_application(TX_SCHEDULING_ERROR);
                        break;
                }
            }

            _device_current_state = DEVICE_STATE_IDLE;
            break;
        case DEVICE_STATE_IDLE:
            status = LORAWAN_STATUS_IDLE;
            break;
#if defined(LORAWAN_COMPLIANCE_TEST)
        case DEVICE_STATE_COMPLIANCE_TEST:
            tr_debug("Device is in compliance test mode.");

            _loramac.LoRaMacSetTxTimer(5000); //ms
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

#if defined(LORAWAN_COMPLIANCE_TEST)

lorawan_status_t LoRaWANStack::send_compliance_test_frame_to_mac()
{
    loramac_compliance_test_req_t test_req;

    //TODO: What if the port is not 224 ???
    if (_compliance_test.app_port == 224) {
        // Clear any normal message stuff before compliance test.
        memset(&test_req, 0, sizeof(test_req));

        if (_compliance_test.link_check == true) {
            _compliance_test.link_check = false;
            _compliance_test.state = 1;
            test_req.f_buffer_size = 3;
            test_req.f_buffer[0] = 5;
            test_req.f_buffer[1] = _compliance_test.demod_margin;
            test_req.f_buffer[2] = _compliance_test.nb_gateways;
        } else {
            switch (_compliance_test.state) {
            case 4:
                _compliance_test.state = 1;
                test_req.f_buffer_size = _compliance_test.app_data_size;
                test_req.f_buffer[0] = _compliance_test.app_data_buffer[0];
                for(uint8_t i = 1; i < MIN(_compliance_test.app_data_size, MBED_CONF_LORA_TX_MAX_SIZE); ++i) {
                    test_req.f_buffer[i] = _compliance_test.app_data_buffer[i];
                }
                break;
            case 1:
                test_req.f_buffer_size = 2;
                test_req.f_buffer[0] = _compliance_test.downlink_counter >> 8;
                test_req.f_buffer[1] = _compliance_test.downlink_counter;
                break;
            }
        }
    }

    //TODO: If port is not 224, this might not work!
    //Is there a test case where same _tx_msg's buffer would be used, when port is not 224???
    if (!_compliance_test.is_tx_confirmed) {
        test_req.type = MCPS_UNCONFIRMED;
        test_req.fport = _compliance_test.app_port;
        test_req.nb_trials = 1;
        test_req.data_rate = _loramac.get_default_tx_datarate();

        tr_info("Transmit unconfirmed compliance test frame %d bytes.", test_req.f_buffer_size);

        for (uint8_t i = 0; i < test_req.f_buffer_size; ++i) {
            tr_info("Byte %d, data is 0x%x", i+1, ((uint8_t*)test_req.f_buffer)[i]);
        }
    } else if (_compliance_test.is_tx_confirmed) {
        test_req.type = MCPS_CONFIRMED;
        test_req.fport = _compliance_test.app_port;
        test_req.nb_trials = _num_retry;
        test_req.data_rate = _loramac.get_default_tx_datarate();

        tr_info("Transmit confirmed compliance test frame %d bytes.", test_req.f_buffer_size);

        for (uint8_t i = 0; i < test_req.f_buffer_size; ++i) {
            tr_info("Byte %d, data is 0x%x", i+1, ((uint8_t*)test_req.f_buffer)[i]);
        }
    } else {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    return _loramac.test_request(&test_req);
}

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

            _loramac.enable_adaptive_datarate(true);

#if MBED_CONF_LORA_PHY      == 0
            _loramac.LoRaMacTestSetDutyCycleOn(false);
#endif
            //5000ms
            _loramac.LoRaMacSetTxTimer(5000);

            //TODO: Should we call lora_state_machine here instead of just setting the state?
            _device_current_state = DEVICE_STATE_COMPLIANCE_TEST;
//            lora_state_machine(DEVICE_STATE_COMPLIANCE_TEST);
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

            _loramac.enable_adaptive_datarate(MBED_CONF_LORA_ADR_ON);

#if MBED_CONF_LORA_PHY      == 0
            _loramac.LoRaMacTestSetDutyCycleOn(MBED_CONF_LORA_DUTY_CYCLE_ON);
#endif
            // Go to idle state after compliance test mode.
            tr_debug("Compliance test disabled.");
            _loramac.LoRaMacStopTxTimer();

            // Clear any compliance test message stuff before going back to normal operation.
            _loramac.reset_ongoing_tx();
            lora_state_machine(DEVICE_STATE_IDLE);
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
            _loramac.setup_link_check_request();
            break;
        case 6: // (ix)
            // Disable TestMode and revert back to normal operation
            _compliance_test.is_tx_confirmed = true;
            _compliance_test.app_port = MBED_CONF_LORA_APP_PORT;
            _compliance_test.app_data_size = LORAWAN_COMPLIANCE_TEST_DATA_SIZE;
            _compliance_test.downlink_counter = 0;
            _compliance_test.running = false;

            _loramac.enable_adaptive_datarate(MBED_CONF_LORA_ADR_ON);

#if MBED_CONF_LORA_PHY      == 0
            _loramac.LoRaMacTestSetDutyCycleOn(MBED_CONF_LORA_DUTY_CYCLE_ON);
#endif
            _loramac.join(true);
            break;
        case 7: // (x)
            if (mcps_indication->buffer_size == 3) {
                loramac_mlme_req_t mlme_req;
                mlme_req.type = MLME_TXCW;
                mlme_req.cw_tx_mode.timeout = (uint16_t)((mcps_indication->buffer[1] << 8) | mcps_indication->buffer[2]);
                _loramac.mlme_request(&mlme_req);
            } else if (mcps_indication->buffer_size == 7) {
                loramac_mlme_req_t mlme_req;
                mlme_req.type = MLME_TXCW_1;
                mlme_req.cw_tx_mode.timeout = (uint16_t)((mcps_indication->buffer[1] << 8) | mcps_indication->buffer[2]);
                mlme_req.cw_tx_mode.frequency = (uint32_t)((mcps_indication->buffer[3] << 16) | (mcps_indication->buffer[4] << 8)
                        | mcps_indication->buffer[5]) * 100;
                mlme_req.cw_tx_mode.power = mcps_indication->buffer[6];
                _loramac.mlme_request(&mlme_req);
            }
            _compliance_test.state = 1;
            break;
        }
    }
}
#endif

