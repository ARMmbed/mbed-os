/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include <string.h>
#include "nsdynmemLIB.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss_channel_stub.h"
#include "Service_Libs/fhss/fhss.h"
#include "Service_Libs/fhss/fhss_beacon.h"

uint8_t fhss_calculate_uc_index(uint8_t channel_index, uint16_t number_of_channels, uint8_t number_of_broadcast_channels)
{
    return 40;
}

uint8_t* fhss_beacon_encode_raw(uint8_t* buffer, const fhss_synchronization_beacon_payload_s* source)
{
    return NULL;
}

int fhss_beacon_update_payload(fhss_structure_t *fhss_structure, fhss_synchronization_beacon_payload_s *payload)
{
    return 0;
}

void fhss_beacon_decode(fhss_synchronization_beacon_payload_s* dest, const uint8_t* buffer, uint32_t elapsed_time, uint16_t number_of_channels)
{

}
