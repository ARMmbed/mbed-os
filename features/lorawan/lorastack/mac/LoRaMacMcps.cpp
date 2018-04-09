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
#include "lorastack/mac/LoRaMacMcps.h"

LoRaMacMcps::LoRaMacMcps()
{
}

LoRaMacMcps::~LoRaMacMcps()
{
}

void LoRaMacMcps::reset_confirmation()
{
    memset((uint8_t*) &confirmation, 0, sizeof(confirmation));

    confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
}

loramac_mcps_confirm_t& LoRaMacMcps::get_confirmation()
{
    return confirmation;
}

loramac_mcps_indication_t& LoRaMacMcps::get_indication()
{
    return indication;
}

void LoRaMacMcps::activate_mcps_subsystem()
{
}

