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

#include "LoRaMacChannelPlan.h"

LoRaMacChannelPlan::LoRaMacChannelPlan() : _lora_phy(NULL)
{
}

LoRaMacChannelPlan::~LoRaMacChannelPlan()
{
}

void LoRaMacChannelPlan::activate_channelplan_subsystem(LoRaPHY *phy)
{
}

lorawan_status_t LoRaMacChannelPlan::set_plan(const lorawan_channelplan_t& plan)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::get_plan(lorawan_channelplan_t& plan,
                                              const channel_params_t* channel_list)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::remove_plan()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::remove_single_channel(uint8_t channel_id)
{
    return LORAWAN_STATUS_OK;
}

