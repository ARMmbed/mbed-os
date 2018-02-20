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

#include "lorawan/LoRaWANInterface.h"

using namespace events;

inline LoRaWANStack& stk_obj()
{
    return LoRaWANStack::get_lorawan_stack();
}

LoRaWANInterface::LoRaWANInterface(LoRaRadio& radio)
{
    stk_obj().bind_radio_driver(radio);
}

LoRaWANInterface::~LoRaWANInterface()
{
}

lorawan_status_t LoRaWANInterface::initialize(EventQueue *queue)
{
    return stk_obj().initialize_mac_layer(queue);
}

lorawan_status_t LoRaWANInterface::connect()
{
    return stk_obj().connect();
}

lorawan_status_t LoRaWANInterface::connect(const lorawan_connect_t &connect)
{
    return stk_obj().connect(connect);
}

lorawan_status_t LoRaWANInterface::disconnect()
{
    return stk_obj().shutdown();
}

lorawan_status_t LoRaWANInterface::add_link_check_request()
{
    return stk_obj().set_link_check_request();
}

void LoRaWANInterface::remove_link_check_request()
{
    stk_obj().remove_link_check_request();
}

lorawan_status_t LoRaWANInterface::set_datarate(uint8_t data_rate)
{
    return stk_obj().set_channel_data_rate(data_rate);
}

lorawan_status_t LoRaWANInterface::set_confirmed_msg_retries(uint8_t count)
{
    return stk_obj().set_confirmed_msg_retry(count);
}

lorawan_status_t LoRaWANInterface::enable_adaptive_datarate()
{
    return stk_obj().enable_adaptive_datarate(true);
}

lorawan_status_t LoRaWANInterface::disable_adaptive_datarate()
{
    return stk_obj().enable_adaptive_datarate(false);
}

lorawan_status_t LoRaWANInterface::set_channel_plan(const lorawan_channelplan_t &channel_plan)
{
    return stk_obj().add_channels(channel_plan);
}

lorawan_status_t LoRaWANInterface::get_channel_plan(lorawan_channelplan_t &channel_plan)
{
    return stk_obj().get_enabled_channels(channel_plan);
}

lorawan_status_t LoRaWANInterface::remove_channel(uint8_t id)
{
    return stk_obj().remove_a_channel(id);
}

lorawan_status_t LoRaWANInterface::remove_channel_plan()
{
    return stk_obj().drop_channel_list();
}

int16_t LoRaWANInterface::send(uint8_t port, const uint8_t* data,
                               uint16_t length, int flags)
{
    return stk_obj().handle_tx(port, data, length, flags);

}

int16_t LoRaWANInterface::receive(uint8_t port, uint8_t* data, uint16_t length,
                                  int flags)
{
    return stk_obj().handle_rx(port, data, length, flags);
}

lorawan_status_t LoRaWANInterface::add_app_callbacks(lorawan_app_callbacks_t *callbacks)
{
    return stk_obj().set_lora_callbacks(callbacks);
}

lorawan_status_t LoRaWANInterface::set_device_class(const device_class_t device_class)
{
    return stk_obj().set_device_class(device_class);
}
