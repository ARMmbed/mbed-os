/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_NRF_SDK_ADAPTATION_H_
#define MBED_NRF_SDK_ADAPTATION_H_

#include <stdbool.h>
#include "nrf_sdh.h"

#ifdef SOFTDEVICE_PRESENT
static inline bool mbed_softdevice_is_enabled()
{
    return nrf_sdh_is_enabled();
}
#else
#define mbed_softdevice_is_enabled() false
#endif

#endif /* MBED_NRF_SDK_ADAPTATION_H_ */
