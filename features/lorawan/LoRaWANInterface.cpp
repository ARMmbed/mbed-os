/**
 * @file
 *
 * @brief      Implementation of LoRaWANBase
 *
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

#include "LoRaWANInterface.h"

using namespace events;

LoRaWANInterface::LoRaWANInterface(LoRaRadio &radio)
{
    _lw_stack.bind_radio_driver(radio);
}

LoRaWANInterface::~LoRaWANInterface()
{
}

lorawan_status_t LoRaWANInterface::initialize(EventQueue *queue)
{
    Lock lock(*this);
    return _lw_stack.initialize_mac_layer(queue);
}

lorawan_status_t LoRaWANInterface::connect()
{
    Lock lock(*this);
    return _lw_stack.connect();
}

lorawan_status_t LoRaWANInterface::connect(const lorawan_connect_t &connect)
{
    Lock lock(*this);
    return _lw_stack.connect(connect);
}

lorawan_status_t LoRaWANInterface::disconnect()
{
    Lock lock(*this);
    return _lw_stack.shutdown();
}

lorawan_status_t LoRaWANInterface::add_link_check_request()
{
    Lock lock(*this);
    return _lw_stack.set_link_check_request();
}

void LoRaWANInterface::remove_link_check_request()
{
    Lock lock(*this);
    _lw_stack.remove_link_check_request();
}

lorawan_status_t LoRaWANInterface::set_datarate(uint8_t data_rate)
{
    Lock lock(*this);
    return _lw_stack.set_channel_data_rate(data_rate);
}

lorawan_status_t LoRaWANInterface::set_confirmed_msg_retries(uint8_t count)
{
    Lock lock(*this);
    return _lw_stack.set_confirmed_msg_retry(count);
}

lorawan_status_t LoRaWANInterface::enable_adaptive_datarate()
{
    Lock lock(*this);
    return _lw_stack.enable_adaptive_datarate(true);
}

lorawan_status_t LoRaWANInterface::disable_adaptive_datarate()
{
    Lock lock(*this);
    return _lw_stack.enable_adaptive_datarate(false);
}

lorawan_status_t LoRaWANInterface::set_channel_plan(const lorawan_channelplan_t &channel_plan)
{
    Lock lock(*this);
    return _lw_stack.add_channels(channel_plan);
}

lorawan_status_t LoRaWANInterface::get_channel_plan(lorawan_channelplan_t &channel_plan)
{
    Lock lock(*this);
    return _lw_stack.get_enabled_channels(channel_plan);
}

lorawan_status_t LoRaWANInterface::remove_channel(uint8_t id)
{
    Lock lock(*this);
    return _lw_stack.remove_a_channel(id);
}

lorawan_status_t LoRaWANInterface::remove_channel_plan()
{
    Lock lock(*this);
    return _lw_stack.drop_channel_list();
}

int16_t LoRaWANInterface::send(uint8_t port, const uint8_t *data, uint16_t length, int flags)
{
    Lock lock(*this);
    return _lw_stack.handle_tx(port, data, length, flags);
}

lorawan_status_t LoRaWANInterface::cancel_sending(void)
{
    Lock lock(*this);
    return _lw_stack.stop_sending();
}

lorawan_status_t LoRaWANInterface::get_tx_metadata(lorawan_tx_metadata &metadata)
{
    Lock lock(*this);
    return _lw_stack.acquire_tx_metadata(metadata);
}

lorawan_status_t LoRaWANInterface::get_rx_metadata(lorawan_rx_metadata &metadata)
{
    Lock lock(*this);
    return _lw_stack.acquire_rx_metadata(metadata);
}

lorawan_status_t LoRaWANInterface::get_backoff_metadata(int &backoff)
{
    Lock lock(*this);
    return _lw_stack.acquire_backoff_metadata(backoff);
}

int16_t LoRaWANInterface::receive(uint8_t port, uint8_t *data, uint16_t length, int flags)
{
    Lock lock(*this);
    return _lw_stack.handle_rx(data, length, port, flags, true);
}

int16_t LoRaWANInterface::receive(uint8_t *data, uint16_t length, uint8_t &port, int &flags)
{
    Lock lock(*this);
    return _lw_stack.handle_rx(data, length, port, flags, false);
}

lorawan_status_t LoRaWANInterface::add_app_callbacks(lorawan_app_callbacks_t *callbacks)
{
    Lock lock(*this);
    return _lw_stack.set_lora_callbacks(callbacks);
}

lorawan_status_t LoRaWANInterface::set_device_class(const device_class_t device_class)
{
    Lock lock(*this);
    return _lw_stack.set_device_class(device_class);
}
