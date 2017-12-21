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
    // Pass mac_handlers to radio to the radio driver after
    // binding radio driver to PHY layer
    radio_events_t *events = stk_obj().bind_radio_driver(radio);
    radio.lock();
    radio.init_radio(events);
    radio.unlock();
}

LoRaWANInterface::~LoRaWANInterface()
{
}

lora_mac_status_t LoRaWANInterface::initialize(EventQueue *queue)
{
    if(!queue) {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    return stk_obj().initialize_mac_layer(queue);
}

lora_mac_status_t LoRaWANInterface::connect()
{
    // connection attempt without parameters.
    // System tries to look for configuration in mbed_lib.json that can be
    // overridden by mbed_app.json. However, if none of the json files are
    // available (highly unlikely), we still fallback to some default parameters.
    // Check lorawan_data_structure for fallback defaults.

    lorawan_connect_t connection_params;

    if (OVER_THE_AIR_ACTIVATION != 0) {
        static uint8_t dev_eui[] = LORAWAN_DEVICE_EUI;
        static uint8_t app_eui[] = LORAWAN_APPLICATION_EUI;
        static uint8_t app_key[] = LORAWAN_APPLICATION_KEY;
        /**
         *
         * OTAA join
         */
        connection_params.connect_type = LORAWAN_CONNECTION_OTAA;
        connection_params.connection_u.otaa.app_eui = app_eui;
        connection_params.connection_u.otaa.dev_eui = dev_eui;
        connection_params.connection_u.otaa.app_key = app_key;
        connection_params.connection_u.otaa.nb_trials = LORAWAN_NB_TRIALS;

        return connect(connection_params);
    } else {
        static uint8_t nwk_skey[] = LORAWAN_NWKSKEY;
        static uint8_t app_skey[] = LORAWAN_APPSKEY;
        static uint32_t dev_addr = LORAWAN_DEVICE_ADDRESS;
        static uint32_t nwk_id = (LORAWAN_DEVICE_ADDRESS & LORAWAN_NETWORK_ID_MASK);

        /**
         *
         * ABP connection
         */
        connection_params.connect_type = LORAWAN_CONNECTION_ABP;
        connection_params.connection_u.abp.nwk_id = nwk_id;
        connection_params.connection_u.abp.dev_addr = dev_addr;
        connection_params.connection_u.abp.nwk_skey = nwk_skey;
        connection_params.connection_u.abp.app_skey = app_skey;

        return connect(connection_params);
    }
}

lora_mac_status_t LoRaWANInterface::connect(const lorawan_connect_t &connect)
{
    lora_mac_status_t mac_status;

    if (connect.connect_type == LORAWAN_CONNECTION_OTAA) {
        mac_status = stk_obj().join_request_by_otaa(connect);
    } else if (connect.connect_type == LORAWAN_CONNECTION_ABP) {
        mac_status = stk_obj().activation_by_personalization(connect);
    } else {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }

    return mac_status;
}

lora_mac_status_t LoRaWANInterface::disconnect()
{
    stk_obj().shutdown();
    return LORA_MAC_STATUS_OK;
}

lora_mac_status_t LoRaWANInterface::set_datarate(uint8_t data_rate)
{
    return stk_obj().set_channel_data_rate(data_rate);
}

lora_mac_status_t LoRaWANInterface::set_confirmed_msg_retries(uint8_t count)
{
    return stk_obj().set_confirmed_msg_retry(count);
}

lora_mac_status_t LoRaWANInterface::enable_adaptive_datarate()
{
    return stk_obj().enable_adaptive_datarate(true);
}

lora_mac_status_t LoRaWANInterface::disable_adaptive_datarate()
{
    return stk_obj().enable_adaptive_datarate(false);
}

lora_mac_status_t LoRaWANInterface::set_channel_plan(const lora_channelplan_t &channel_plan)
{
    return stk_obj().add_channels(channel_plan);
}

lora_mac_status_t LoRaWANInterface::get_channel_plan(lora_channelplan_t &channel_plan)
{
    return stk_obj().get_enabled_channels(channel_plan);
}

lora_mac_status_t LoRaWANInterface::remove_channel(uint8_t id)
{
    return stk_obj().remove_a_channel(id);
}

lora_mac_status_t LoRaWANInterface::remove_channel_plan()
{
    return stk_obj().drop_channel_list();
}

int16_t LoRaWANInterface::send(uint8_t port, const uint8_t* data,
                               uint16_t length, int flags)
{
    if (data) {
        return stk_obj().handle_tx(port, data, length, flags);
    } else {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }
}

int16_t LoRaWANInterface::receive(uint8_t port, uint8_t* data, uint16_t length,
                                  int flags)
{
    if (data && length > 0) {
        return stk_obj().handle_rx(port, data, length, flags);
    } else {
        return LORA_MAC_STATUS_PARAMETER_INVALID;
    }
}

lora_mac_status_t LoRaWANInterface::add_app_callbacks(lorawan_app_callbacks_t *callbacks)
  {

     if (!callbacks || !callbacks->events) {
         // Event Callback is mandatory
         return LORA_MAC_STATUS_PARAMETER_INVALID;
     }

     stk_obj().set_lora_callbacks(callbacks);

     return LORA_MAC_STATUS_OK;
  }
