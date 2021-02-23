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

#include "LoRaMacCommand.h"
#include "LoRaMac.h"

#include "LoRaMacCommand_stub.h"

lorawan_status_t LoRaMacCommand_stub::status_value = LORAWAN_STATUS_OK;
bool LoRaMacCommand_stub::bool_value = false;
uint8_t LoRaMacCommand_stub::uint8_value = 0;
int32_t LoRaMacCommand_stub::int32_value = 0;

LoRaMacCommand::LoRaMacCommand()
{
}

void LoRaMacCommand::clear_command_buffer()
{
}

uint8_t LoRaMacCommand::get_mac_cmd_length() const
{
    return LoRaMacCommand_stub::uint8_value;
}

uint8_t *LoRaMacCommand::get_mac_commands_buffer()
{
    return &LoRaMacCommand_stub::uint8_value;
}

void LoRaMacCommand::parse_mac_commands_to_repeat()
{
}


void LoRaMacCommand::clear_repeat_buffer()
{
}

void LoRaMacCommand::copy_repeat_commands_to_buffer()
{
}

uint8_t LoRaMacCommand::get_repeat_commands_length() const
{
    return LoRaMacCommand_stub::uint8_value;
}

void LoRaMacCommand::clear_sticky_mac_cmd()
{
}

bool LoRaMacCommand::has_sticky_mac_cmd() const
{
    return LoRaMacCommand_stub::bool_value;
}

lorawan_status_t LoRaMacCommand::process_mac_commands(const uint8_t *payload, uint8_t mac_index,
                                                      uint8_t commands_size, uint8_t snr,
                                                      loramac_mlme_confirm_t &mlme_conf,
                                                      lora_mac_system_params_t &mac_sys_params,
                                                      LoRaPHY &lora_phy)
{
    return LoRaMacCommand_stub::status_value;
}

int32_t LoRaMacCommand::cmd_buffer_remaining() const
{
    return LoRaMacCommand_stub::int32_value;
}

void LoRaMacCommand::set_batterylevel_callback(mbed::Callback<uint8_t(void)> battery_level)
{
}

lorawan_status_t LoRaMacCommand::add_link_check_req()
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_link_adr_ans(uint8_t status)
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_duty_cycle_ans()
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_rx_param_setup_ans(uint8_t status)
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_dev_status_ans(uint8_t battery, uint8_t margin)
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_new_channel_ans(uint8_t status)
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_rx_timing_setup_ans()
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_tx_param_setup_ans()
{
    return LoRaMacCommand_stub::status_value;
}

lorawan_status_t LoRaMacCommand::add_dl_channel_ans(uint8_t status)
{
    return LoRaMacCommand_stub::status_value;
}
