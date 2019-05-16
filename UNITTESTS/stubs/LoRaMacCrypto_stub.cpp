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

#include <stdlib.h>
#include <stdint.h>

#include "LoRaMacCrypto.h"
#include "system/lorawan_data_structures.h"

#include "LoRaMacCrypto_stub.h"

int LoRaMacCrypto_stub::int_table_idx_value = 0;
int LoRaMacCrypto_stub::int_table[20] = {};

LoRaMacCrypto::LoRaMacCrypto()
{
}

LoRaMacCrypto::~LoRaMacCrypto()
{
}

int LoRaMacCrypto::compute_mic(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t,
                               uint8_t dir, uint32_t, uint32_t *)
{
    return LoRaMacCrypto_stub::int_table[LoRaMacCrypto_stub::int_table_idx_value++];
}

int LoRaMacCrypto::encrypt_payload(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t,
                                   uint8_t, uint32_t, uint8_t *)
{
    return LoRaMacCrypto_stub::int_table[LoRaMacCrypto_stub::int_table_idx_value++];
}

int LoRaMacCrypto::decrypt_payload(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t,
                                   uint8_t, uint32_t, uint8_t *)
{
    return LoRaMacCrypto_stub::int_table[LoRaMacCrypto_stub::int_table_idx_value++];
}

int LoRaMacCrypto::compute_join_frame_mic(const uint8_t *, uint16_t, const uint8_t *, uint32_t, uint32_t *)
{
    return LoRaMacCrypto_stub::int_table[LoRaMacCrypto_stub::int_table_idx_value++];
}

int LoRaMacCrypto::decrypt_join_frame(const uint8_t *in, uint16_t size, const uint8_t *, uint32_t, uint8_t *out)
{
    memcpy(out, in, size);
    return LoRaMacCrypto_stub::int_table[LoRaMacCrypto_stub::int_table_idx_value++];
}

int LoRaMacCrypto::compute_skeys_for_join_frame(const uint8_t *, uint32_t, const uint8_t *, uint16_t,
                                                uint8_t *, uint8_t *)
{
    return LoRaMacCrypto_stub::int_table[LoRaMacCrypto_stub::int_table_idx_value++];
}
