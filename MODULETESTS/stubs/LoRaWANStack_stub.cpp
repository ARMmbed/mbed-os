/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include <string.h>
#include <stdlib.h>

#include "LoRaWANStack.h"

using namespace mbed;
using namespace events;

/*****************************************************************************
 * Constructor                                                               *
 ****************************************************************************/
LoRaWANStack::LoRaWANStack()
    : _loramac(),
      _device_current_state(DEVICE_STATE_NOT_INITIALIZED),
      _lw_session(),
      _tx_msg(),
      _rx_msg(),
      _tx_metadata(),
      _rx_metadata(),
      _num_retry(1),
      _ctrl_flags(0),
      _app_port(12),
      _link_check_requested(false),
      _automatic_uplink_ongoing(false),
      _queue(NULL)
{
}

void LoRaWANStack::bind_phy_and_radio_driver(LoRaRadio &radio, LoRaPHY &phy)
{
}

lorawan_status_t LoRaWANStack::initialize_mac_layer(EventQueue *queue)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::set_lora_callbacks(const lorawan_app_callbacks_t *callbacks)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::connect()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::connect(const lorawan_connect_t &connect)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::add_channels(const lorawan_channelplan_t &channel_plan)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::remove_a_channel(uint8_t channel_id)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::drop_channel_list()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::get_enabled_channels(lorawan_channelplan_t &channel_plan)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::set_confirmed_msg_retry(uint8_t count)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::set_channel_data_rate(uint8_t data_rate)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::enable_adaptive_datarate(bool adr_enabled)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::stop_sending(void)
{
    return LORAWAN_STATUS_OK;
}

int16_t LoRaWANStack::handle_tx(const uint8_t port, const uint8_t *data,
                                uint16_t length, uint8_t flags,
                                bool null_allowed, bool allow_port_0)
{
    return 0;
}

int16_t LoRaWANStack::handle_rx(uint8_t *data, uint16_t length, uint8_t &port, int &flags, bool validate_params)
{
    return 0;
}

lorawan_status_t LoRaWANStack::set_link_check_request()
{
    return LORAWAN_STATUS_OK;
}

void LoRaWANStack::remove_link_check_request()
{
}

lorawan_status_t LoRaWANStack::shutdown()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::set_device_class(const device_class_t &device_class)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t  LoRaWANStack::acquire_tx_metadata(lorawan_tx_metadata &tx_metadata)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::acquire_rx_metadata(lorawan_rx_metadata &metadata)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaWANStack::acquire_backoff_metadata(int &backoff)
{
    return LORAWAN_STATUS_OK;
}

/*****************************************************************************
 * Interrupt handlers                                                        *
 ****************************************************************************/
void LoRaWANStack::tx_interrupt_handler(void)
{
}

void LoRaWANStack::rx_interrupt_handler(const uint8_t *payload, uint16_t size,
                                        int16_t rssi, int8_t snr)
{
}

void LoRaWANStack::rx_error_interrupt_handler(void)
{
}

void LoRaWANStack::tx_timeout_interrupt_handler(void)
{
}

void LoRaWANStack::rx_timeout_interrupt_handler(void)
{
}

void LoRaWANStack::process_transmission_timeout()
{
}

void LoRaWANStack::process_transmission(void)
{
}

void post_process_tx_with_reception(void)
{

}

void post_process_tx_no_reception(void)
{

}

void LoRaWANStack::handle_scheduling_failure(void)
{
}

void LoRaWANStack::process_reception(const uint8_t *const payload, uint16_t size,
                                     int16_t rssi, int8_t snr)
{
}

void LoRaWANStack::process_reception_timeout(bool is_timeout)
{
}

void LoRaWANStack::make_tx_metadata_available(void)
{
}

void LoRaWANStack::make_rx_metadata_available(void)
{
}

bool LoRaWANStack::is_port_valid(const uint8_t port, bool allow_port_0)
{
    return true;
}

lorawan_status_t LoRaWANStack::set_application_port(const uint8_t port, bool allow_port_0)
{
    return LORAWAN_STATUS_OK;
}

void LoRaWANStack::state_machine_run_to_completion()
{
}

void LoRaWANStack::send_event_to_application(const lorawan_event_t event) const
{
}

void LoRaWANStack::send_automatic_uplink_message(const uint8_t port)
{
}

int LoRaWANStack::convert_to_msg_flag(const mcps_type_t type)
{
    return 0;
}

lorawan_status_t LoRaWANStack::handle_connect(bool is_otaa)
{
    return LORAWAN_STATUS_OK;
}

void LoRaWANStack::mlme_indication_handler()
{
}

void LoRaWANStack::mlme_confirm_handler()
{
}

void LoRaWANStack::mcps_confirm_handler()
{
}

void LoRaWANStack::mcps_indication_handler()
{
}

lorawan_status_t LoRaWANStack::state_controller(device_states_t new_state)
{
    return LORAWAN_STATUS_OK;
}

void LoRaWANStack::process_shutdown_state(lorawan_status_t &op_status)
{
}

void LoRaWANStack::process_status_check_state()
{
}

void LoRaWANStack::process_scheduling_state(lorawan_status_t &op_status)
{
}

void LoRaWANStack::process_joining_state(lorawan_status_t &op_status)
{
}

void LoRaWANStack::process_connected_state()
{
}

void LoRaWANStack::process_connecting_state(lorawan_status_t &op_status)
{
}

void LoRaWANStack::process_idle_state(lorawan_status_t &op_status)
{
}

void LoRaWANStack::process_uninitialized_state(lorawan_status_t &op_status)
{
}
