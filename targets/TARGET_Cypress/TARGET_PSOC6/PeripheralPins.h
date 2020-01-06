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

#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "pinmap.h"
#include "PeripheralNames.h"

/**
 * This functions creates a PinMap[] from the cypress cyhal_resource_pin_mapping_t[] which has different format.
 * The cypress cyhal_resource_mapping_t has use a fixed size array with no trailing NC terminating array element.
 * @param[in,out] pin_map        the address of the static pin_map array to try to create
 * @param[in]     resource_map   the Cypress hal resource pin mapping
 * @param[in]     resource_size  the total size in bytes of resource_map
 */
void try_create_pin_map(PinMap **pin_map, const cyhal_resource_pin_mapping_t* resource_map, size_t resource_size);

/**
 * This functions creates a PinMap[] from two cypress cyhal_resource_pin_mapping_t[] which has different format.
 * The cypress cyhal_resource_mapping_t has use a fixed size array with no trailing NC terminating array element.
 * @param[in,out] pin_map         the address of the static pin_map array to try to create
 * @param[in]     resource_map0   the first Cypress hal resource pin mapping
 * @param[in]     resource_size0  the total size in bytes of in the first resource_map
 * @param[in]     resource_map1   the second Cypress hal resource pin mapping
 * @param[in]     resource_size1  the total size in bytes of in the second resource_map
 */
void try_create_combined_pin_map(PinMap **pin_map, const cyhal_resource_pin_mapping_t* resource_map0, size_t resource_size0, const cyhal_resource_pin_mapping_t* resource_map1, size_t resource_size1);

#endif
