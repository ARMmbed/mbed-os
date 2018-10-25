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
#include "lorawan_data_structures.h"

namespace LoRaMac_stub {
extern bool bool_value;
extern int bool_false_counter;
extern int bool_true_counter;
extern int int_value;
extern uint8_t uint8_value;
extern rx_slot_t slot_value;
extern lorawan_status_t status_value;
extern loramac_mcps_confirm_t *mcps_conf_ptr;
extern loramac_mcps_indication_t *mcps_ind_ptr;
extern loramac_mlme_confirm_t *mlme_conf_ptr;
extern loramac_mlme_indication_t *mlme_ind_ptr;
extern device_class_t dev_class_value;
extern mbed::Callback<void(void)> _scheduling_failure_handler;
}
