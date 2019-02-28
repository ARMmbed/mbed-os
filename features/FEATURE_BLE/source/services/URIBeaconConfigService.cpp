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

#include "ble/services/URIBeaconConfigService.h"

#define UUID_URI_BEACON(FIRST, SECOND) {                         \
        0xee, 0x0c, FIRST, SECOND, 0x87, 0x86, 0x40, 0xba,       \
        0xab, 0x96, 0x99, 0xb9, 0x1a, 0xc9, 0x81, 0xd8,          \
}

const uint8_t UUID_URI_BEACON_SERVICE[UUID::LENGTH_OF_LONG_UUID]    = UUID_URI_BEACON(0x20, 0x80);
const uint8_t UUID_LOCK_STATE_CHAR[UUID::LENGTH_OF_LONG_UUID]       = UUID_URI_BEACON(0x20, 0x81);
const uint8_t UUID_LOCK_CHAR[UUID::LENGTH_OF_LONG_UUID]             = UUID_URI_BEACON(0x20, 0x82);
const uint8_t UUID_UNLOCK_CHAR[UUID::LENGTH_OF_LONG_UUID]           = UUID_URI_BEACON(0x20, 0x83);
const uint8_t UUID_URI_DATA_CHAR[UUID::LENGTH_OF_LONG_UUID]         = UUID_URI_BEACON(0x20, 0x84);
const uint8_t UUID_FLAGS_CHAR[UUID::LENGTH_OF_LONG_UUID]            = UUID_URI_BEACON(0x20, 0x85);
const uint8_t UUID_ADV_POWER_LEVELS_CHAR[UUID::LENGTH_OF_LONG_UUID] = UUID_URI_BEACON(0x20, 0x86);
const uint8_t UUID_TX_POWER_MODE_CHAR[UUID::LENGTH_OF_LONG_UUID]    = UUID_URI_BEACON(0x20, 0x87);
const uint8_t UUID_BEACON_PERIOD_CHAR[UUID::LENGTH_OF_LONG_UUID]    = UUID_URI_BEACON(0x20, 0x88);
const uint8_t UUID_RESET_CHAR[UUID::LENGTH_OF_LONG_UUID]            = UUID_URI_BEACON(0x20, 0x89);

const uint8_t BEACON_UUID[sizeof(UUID::ShortUUIDBytes_t)] = {0xD8, 0xFE};