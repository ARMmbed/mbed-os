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

#include "LoRaMac.h"
#include "lorastack/mac/LoRaMacMlme.h"

LoRaMacMlme::LoRaMacMlme()
: _lora_phy(NULL)
{
}

LoRaMacMlme::~LoRaMacMlme()
{
}

void LoRaMacMlme::reset_confirmation()
{
    memset((uint8_t*) &confirmation, 0, sizeof(confirmation));

    confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
}

loramac_mlme_confirm_t& LoRaMacMlme::get_confirmation()
{
    return confirmation;
}

loramac_mlme_indication_t& LoRaMacMlme::get_indication()
{
    return indication;
}

void LoRaMacMlme::activate_mlme_subsystem(LoRaPHY *phy)
{
    _lora_phy = phy;
}

void LoRaMacMlme::set_tx_continuous_wave(uint8_t channel, int8_t datarate, int8_t tx_power,
                                         float max_eirp, float antenna_gain, uint16_t timeout)
{
    cw_mode_params_t continuous_wave;

    continuous_wave.channel = channel;
    continuous_wave.datarate = datarate;
    continuous_wave.tx_power = tx_power;
    continuous_wave.max_eirp = max_eirp;
    continuous_wave.antenna_gain = antenna_gain;
    continuous_wave.timeout = timeout;

    _lora_phy->set_tx_cont_mode(&continuous_wave);
}
