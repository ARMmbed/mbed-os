/**
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "LoRaRadio.h"
#include <math.h>


int32_t LoRaRadio::lora_time_on_air(uint16_t preamble_length, uint8_t datarate,
                                    uint32_t bandwidth, uint8_t coding_rate,
                                    bool crc_on, bool fixed_len, uint8_t pkt_len)
{
    uint32_t air_time;
    bool low_datarate_optimize = false;

    switch (bandwidth) {
        case 125000:
            low_datarate_optimize = (datarate == 11) || (datarate == 12);
            break;
        case 250000:
            low_datarate_optimize = (datarate == 12);
            break;
        case 500000:
            break;
        default:
            return -1;
    }

    // Symbol rate : time for one symbol (secs)
    float rs = (float)bandwidth / (1 << datarate);
    float ts = 1 / rs;
    // time of preamble
    float t_preamble = (preamble_length + 4.25f) * ts;
    // Symbol length of payload and time
    float tmp = ceil((8 * pkt_len - 4 * datarate + 28
                      + 16 * (crc_on ? 1 : 0)
                      - (fixed_len ? 20 : 0))
                     / (float)(4 * (datarate - ((low_datarate_optimize > 0) ? 2 : 0))))
                * (coding_rate + 4);

    float n_payload = 8 + ((tmp > 0) ? tmp : 0);
    float t_payload = n_payload * ts;
    // Time on air
    float t_on_air = t_preamble + t_payload;
    // return ms secs
    air_time = floor(t_on_air * 1000 + 0.999f);
    return air_time;
}

