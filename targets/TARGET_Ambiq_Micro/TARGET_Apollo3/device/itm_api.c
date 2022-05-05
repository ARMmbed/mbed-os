/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if DEVICE_ITM

#include "hal/itm_api.h"
#include "cmsis.h"
#include "am_bsp.h"

#include <stdbool.h>

/* SWO frequency: 1000 kHz */

// As SWO has to be accessible everywhere, including ISRs, we can't easily
// communicate the dependency on clocks etc. to other components - so this
// function checks that things appear to be set up, and if not re-configures
// everything
void itm_init(void)
{
    am_bsp_itm_printf_enable();
}

#endif
