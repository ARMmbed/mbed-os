/**
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *    (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * Description: LoRa MAC crypto implementation
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jäckle ( STACKFORCE )
 *
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdlib.h>
#include <stdint.h>

#include "LoRaMacCrypto.h"
#include "system/lorawan_data_structures.h"


LoRaMacCrypto::LoRaMacCrypto()
{
}

int LoRaMacCrypto::compute_mic(const uint8_t *, uint16_t , const uint8_t *, uint32_t, uint32_t,
                               uint8_t dir, uint32_t, uint32_t *)
{
    return 0;
}

int LoRaMacCrypto::encrypt_payload(const uint8_t *, uint16_t , const uint8_t *, uint32_t, uint32_t,
                                   uint8_t , uint32_t , uint8_t *)
{
    return 0;
}

int LoRaMacCrypto::decrypt_payload(const uint8_t *, uint16_t , const uint8_t *, uint32_t, uint32_t,
                                   uint8_t , uint32_t , uint8_t *)
{
    return 0;
}

int LoRaMacCrypto::compute_join_frame_mic(const uint8_t *, uint16_t , const uint8_t *, uint32_t, uint32_t *)
{
    return 0;
}

int LoRaMacCrypto::decrypt_join_frame(const uint8_t *, uint16_t , const uint8_t *, uint32_t, uint8_t *)
{
    return 0;
}

int LoRaMacCrypto::compute_skeys_for_join_frame(const uint8_t *, uint32_t, const uint8_t *, uint16_t ,
                                                uint8_t *, uint8_t *)
{
    return 0;
}

