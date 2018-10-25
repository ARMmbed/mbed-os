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

lorawan_status_t LoRaMacChannelPlan::set_plan(const lorawan_channelplan_t &plan)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::get_plan(lorawan_channelplan_t &plan,
                                              const channel_params_t *channel_list)
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

