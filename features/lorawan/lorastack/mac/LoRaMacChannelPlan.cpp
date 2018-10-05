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
    _lora_phy = phy;
}

lorawan_status_t LoRaMacChannelPlan::set_plan(const lorawan_channelplan_t& plan)
{
    lorawan_status_t status;

    uint8_t max_num_channels;

    if (!_lora_phy->is_custom_channel_plan_supported()) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    max_num_channels = _lora_phy->get_max_nb_channels();

    // check if user is setting more channels than supported
    if (plan.nb_channels > max_num_channels) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        status = _lora_phy->add_channel(&plan.channels[i].ch_param, plan.channels[i].id);

        if (status != LORAWAN_STATUS_OK) {
            return status;
        }
    }

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::get_plan(lorawan_channelplan_t& plan,
                                              const channel_params_t* channel_list)
{
    uint8_t max_num_channels;
    uint16_t *channel_mask;
    uint8_t count = 0;

    if (!_lora_phy->is_custom_channel_plan_supported()) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    max_num_channels = _lora_phy->get_max_nb_channels();

    channel_mask = _lora_phy->get_channel_mask(false);

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip the channels which are not enabled
        if (_lora_phy->mask_bit_test(channel_mask, i) == 0) {
            continue;
        }

        // otherwise add them to the channel_plan struct
        plan.channels[count].id = i;
        plan.channels[count].ch_param.frequency = channel_list[i].frequency;
        plan.channels[count].ch_param.dr_range.value = channel_list[i].dr_range.value;
        plan.channels[count].ch_param.dr_range.fields.min = channel_list[i].dr_range.fields.min;
        plan.channels[count].ch_param.dr_range.fields.max = channel_list[i].dr_range.fields.max;
        plan.channels[count].ch_param.band = channel_list[i].band;
        plan.channels[count].ch_param.rx1_frequency = channel_list[i].rx1_frequency;
        count++;
    }

    plan.nb_channels = count;

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::remove_plan()
{
    lorawan_status_t status = LORAWAN_STATUS_OK;

    uint8_t max_num_channels;
    uint16_t *channel_mask;
    uint16_t *default_channel_mask;

    if (!_lora_phy->is_custom_channel_plan_supported()) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    max_num_channels = _lora_phy->get_max_nb_channels();

    channel_mask = _lora_phy->get_channel_mask(false);

    default_channel_mask = _lora_phy->get_channel_mask(true);

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip any default channels
        if (_lora_phy->mask_bit_test(default_channel_mask, i) != 0) {
            continue;
        }

        // skip any channels which are not currently enabled
        if (_lora_phy->mask_bit_test(channel_mask, i) == 0) {
            continue;
        }

        status = remove_single_channel(i);

        if (status != LORAWAN_STATUS_OK) {
            return status;
        }
    }

    return status;
}

lorawan_status_t LoRaMacChannelPlan::remove_single_channel(uint8_t channel_id)
{
    uint8_t max_num_channels;

    if (!_lora_phy->is_custom_channel_plan_supported()) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    max_num_channels = _lora_phy->get_max_nb_channels();

    // According to specification channel IDs start from 0 and last valid
    // channel ID is N-1 where N=MAX_NUM_CHANNELS.
    // So any ID which is larger or equal to the Max number of channels is invalid
    if (channel_id >= max_num_channels) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    if (_lora_phy->remove_channel(channel_id) == false) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    _lora_phy->put_radio_to_sleep();

    return LORAWAN_STATUS_OK;
}

