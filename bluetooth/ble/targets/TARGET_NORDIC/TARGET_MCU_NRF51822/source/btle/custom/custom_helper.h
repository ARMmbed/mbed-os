/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef _CUSTOM_HELPER_H_
#define _CUSTOM_HELPER_H_

#include "common/common.h"
#include "nrf_ble.h"
#include "ble/UUID.h"
#include "ble/GattCharacteristic.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t custom_add_uuid_base(uint8_t const *const p_uuid_base);
error_t custom_decode_uuid(uint8_t const *const p_uuid_base,
                           ble_uuid_t          *p_uuid);
ble_uuid_t custom_convert_to_nordic_uuid(const UUID &uuid);

error_t custom_add_in_characteristic(uint16_t                  service_handle,
                                     ble_uuid_t               *p_uuid,
                                     uint8_t                   properties,
                                     SecurityManager::SecurityMode_t requiredSecurity,
                                     uint8_t                  *p_data,
                                     uint16_t                  length,
                                     uint16_t                  max_length,
                                     bool                      has_variable_len,
                                     const uint8_t            *userDescriptionDescriptorValuePtr,
                                     uint16_t                  userDescriptionDescriptorValueLen,
                                     bool                      readAuthorization,
                                     bool                      writeAuthorization,
                                     ble_gatts_char_handles_t *p_char_handle);

error_t custom_add_in_descriptor(uint16_t                      char_handle,
                                     ble_uuid_t               *p_uuid,
                                     uint8_t                  *p_data,
                                     uint16_t                  length,
                                     uint16_t                  max_length,
                                     bool                      has_variable_len,
                                     uint16_t                 *p_desc_handle);

#ifdef __cplusplus
}
#endif

#endif // ifndef _CUSTOM_HELPER_H_
