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

#include "stdint.h"
#include "stdbool.h"
#include <cstddef>

#include "lorawan_types.h"

namespace LoRaPHY_stub {
extern LoRaRadio *radio;
extern uint8_t bool_counter;
extern bool bool_table[20];
extern uint32_t uint32_value;
extern uint8_t uint8_value;
extern int8_t int8_value;
extern int int_value;
extern double double_value;
extern uint16_t uint16_value;
extern lorawan_status_t lorawan_status_value;
extern channel_params_t *channel_params_ptr;
extern uint8_t linkAdrNbBytesParsed;
extern uint8_t ch_mask_value;
extern uint8_t adr_parse_count;
}
