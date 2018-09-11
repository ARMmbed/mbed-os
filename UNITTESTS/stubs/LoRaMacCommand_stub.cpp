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

Description: LoRa MAC layer implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )

Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#include "LoRaMacCommand.h"
#include "LoRaMac.h"


LoRaMacCommand::LoRaMacCommand()
{
}

void LoRaMacCommand::clear_command_buffer()
{
}

uint8_t LoRaMacCommand::get_mac_cmd_length() const
{
    return 0;
}

uint8_t *LoRaMacCommand::get_mac_commands_buffer()
{
    return NULL;
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
    return 0;
}

void LoRaMacCommand::clear_mac_commands_in_next_tx()
{
}

bool LoRaMacCommand::is_mac_command_in_next_tx() const
{
    return false;
}

void LoRaMacCommand::clear_sticky_mac_cmd()
{
}

bool LoRaMacCommand::has_sticky_mac_cmd() const
{
    return false;
}

lorawan_status_t LoRaMacCommand::process_mac_commands(const uint8_t *payload, uint8_t mac_index,
                                                      uint8_t commands_size, uint8_t snr,
                                                      loramac_mlme_confirm_t& mlme_conf,
                                                      lora_mac_system_params_t &mac_sys_params,
                                                      LoRaPHY &lora_phy)
{
    return LORAWAN_STATUS_OK;
}

bool LoRaMacCommand::is_sticky_mac_command_pending()
{
    return false;
}

int32_t LoRaMacCommand::cmd_buffer_remaining() const
{
    return 0;
}

void LoRaMacCommand::set_batterylevel_callback(mbed::Callback<uint8_t(void)> battery_level)
{
}

lorawan_status_t LoRaMacCommand::add_link_check_req()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_link_adr_ans(uint8_t status)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_duty_cycle_ans()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_rx_param_setup_ans(uint8_t status)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_dev_status_ans(uint8_t battery, uint8_t margin)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_new_channel_ans(uint8_t status)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_rx_timing_setup_ans()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_tx_param_setup_ans()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacCommand::add_dl_channel_ans(uint8_t status)
{
    return LORAWAN_STATUS_OK;
}
