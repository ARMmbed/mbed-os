/*
 * Copyright (c) 2018, Pelion and affiliates.
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

#ifndef MLE_SERVICE_FRAME_COUNTER_TABLE_H_
#define MLE_SERVICE_FRAME_COUNTER_TABLE_H_

#include <ns_types.h>
/*
 * MLE neighbor security counter info entry for replay attack
 *
 */
typedef struct {
    uint32_t        mle_frame_counter;  /*!< Last used security frame counter */
    uint32_t        last_key_sequence;  /*!< Last used key sequence */
    bool        new_key_pending: 1; /*!< New Key is pending */
} mle_neighbor_security_counter_info_t;

//Internal use
mle_neighbor_security_counter_info_t *mle_service_counter_info_get(int8_t interface_id, uint8_t attribute_index);

#endif /* MLE_SERVICE_FRAME_COUNTER_TABLE_H_ */
