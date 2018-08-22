/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef MBED_NFC_DEFINITIONS_H
#define MBED_NFC_DEFINITIONS_H

#include <stdint.h>
#include "stack/nfc_errors.h"

#include "acore/buffer.h"

namespace mbed {
namespace nfc {
struct nfc_rf_protocols_bitmask_t {
    uint8_t initiator_t1t : 1;
    uint8_t initiator_t2t : 1;
    uint8_t initiator_t3t : 1;
    uint8_t initiator_iso_dep : 1;
    uint8_t initiator_nfc_dep : 1;
    uint8_t initiator_t5t : 1;

    uint8_t target_t1t : 1;
    uint8_t target_t2t : 1;
    uint8_t target_t3t : 1;
    uint8_t target_iso_dep : 1;
    uint8_t target_nfc_dep : 1;
    uint8_t target_t5t : 1;
};

enum nfc_tag_type_t {
    nfc_tag_type_1,
    nfc_tag_type_2,
    nfc_tag_type_3,
    nfc_tag_type_4a,
    nfc_tag_type_4b,
    nfc_tag_type_5
};

} // namespace nfc
} // namespace mbed

#endif
