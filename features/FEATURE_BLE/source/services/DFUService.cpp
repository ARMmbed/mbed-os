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

#ifdef TARGET_NRF51822 /* DFU only supported on nrf51 platforms */

#include "ble/services/DFUService.h"

const uint8_t              DFUServiceBaseUUID[] = {
    0x00, 0x00, 0x00, 0x00, 0x12, 0x12, 0xEF, 0xDE,
    0x15, 0x23, 0x78, 0x5F, 0xEA, 0xBC, 0xD1, 0x23,
};
const uint16_t             DFUServiceShortUUID                      = 0x1530;
const uint16_t             DFUServiceControlCharacteristicShortUUID = 0x1531;
const uint16_t             DFUServicePacketCharacteristicShortUUID  = 0x1532;

const uint8_t              DFUServiceUUID[] = {
    0x00, 0x00, (uint8_t)(DFUServiceShortUUID >> 8), (uint8_t)(DFUServiceShortUUID & 0xFF), 0x12, 0x12, 0xEF, 0xDE,
    0x15, 0x23, 0x78, 0x5F, 0xEA, 0xBC, 0xD1, 0x23,
};
const uint8_t              DFUServiceControlCharacteristicUUID[] = {
    0x00, 0x00, (uint8_t)(DFUServiceControlCharacteristicShortUUID >> 8), (uint8_t)(DFUServiceControlCharacteristicShortUUID & 0xFF), 0x12, 0x12, 0xEF, 0xDE,
    0x15, 0x23, 0x78, 0x5F, 0xEA, 0xBC, 0xD1, 0x23,
};
const uint8_t              DFUServicePacketCharacteristicUUID[] = {
    0x00, 0x00, (uint8_t)(DFUServicePacketCharacteristicShortUUID >> 8), (uint8_t)(DFUServicePacketCharacteristicShortUUID & 0xFF), 0x12, 0x12, 0xEF, 0xDE,
    0x15, 0x23, 0x78, 0x5F, 0xEA, 0xBC, 0xD1, 0x23,
};

DFUService::ResetPrepare_t DFUService::handoverCallback = NULL;

#endif /* #ifdef TARGET_NRF51822 */