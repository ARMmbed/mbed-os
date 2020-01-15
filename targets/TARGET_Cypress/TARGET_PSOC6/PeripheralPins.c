/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
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
#include "PeripheralPins.h"

static PinMap default_nc_pin_map = {NC, NC, 0};

void try_create_pin_map(PinMap **pin_map, const cyhal_resource_pin_mapping_t *resource_map, size_t resource_size)
{
    if (*pin_map == NULL) {
        if (resource_size == sizeof(cyhal_resource_pin_mapping_t) && resource_map[0].pin == NC) {
            // If there are no valid connection, then use default NC pin map.
            *pin_map = &default_nc_pin_map;
        } else {
            *pin_map = (PinMap *)malloc(resource_size + sizeof(PinMap));
            if (*pin_map == NULL) {
                // If malloc fails, then use default NC pin map.
                *pin_map = &default_nc_pin_map;
            } else {
                memcpy(*pin_map, resource_map, resource_size);
                (*pin_map)[resource_size / sizeof(PinMap)].pin = NC;
                (*pin_map)[resource_size / sizeof(PinMap)].peripheral = NC;
                (*pin_map)[resource_size / sizeof(PinMap)].function = 0;
            }
        }
    }
}

void try_create_combined_pin_map(PinMap **pin_map, const cyhal_resource_pin_mapping_t *resource_map0, size_t resource_size0, const cyhal_resource_pin_mapping_t *resource_map1, size_t resource_size1)
{
    /*
     * This functions is similar to try_create_pin_map except it create a PinMap from multple cyhal_resource_pin_mapping_t
     */
    if (*pin_map == NULL) {
        if (resource_size0 == sizeof(cyhal_resource_pin_mapping_t) && resource_map0[0].pin == NC) {
            resource_size0 = 0;
        }
        if (resource_size1 == sizeof(cyhal_resource_pin_mapping_t) && resource_map1[0].pin == NC) {
            resource_size1 = 0;
        }

        if (resource_size0 == 0 && resource_size1 == 0) {
            // If there are no valid connection, then use default NC pin map.
            *pin_map = &default_nc_pin_map;
        } else {
            *pin_map = (PinMap *)malloc(resource_size0 + resource_size1 + sizeof(PinMap));
            if (*pin_map == NULL) {
                // If malloc fails, then use default NC pin map.
                *pin_map = &default_nc_pin_map;
            } else {
                memcpy(*pin_map, resource_map0, resource_size0);
                memcpy(((void *)(*pin_map)) + resource_size0, resource_map1, resource_size1);
                (*pin_map)[(resource_size0 + resource_size1) / sizeof(PinMap)].pin = NC;
                (*pin_map)[(resource_size0 + resource_size1) / sizeof(PinMap)].peripheral = NC;
                (*pin_map)[(resource_size0 + resource_size1) / sizeof(PinMap)].function = 0;
            }
        }
    }
}
