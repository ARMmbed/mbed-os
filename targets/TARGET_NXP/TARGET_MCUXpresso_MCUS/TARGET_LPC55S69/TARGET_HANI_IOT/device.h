/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#define NUMBER_OF_GPIO_INTS    8

#define LPADC_VREF_SOURCE      kLPADC_ReferenceVoltageAlt2
#define LPADC_DO_OFFSET_CALIBRATION false
#define LPADC_OFFSET_VALUE_A        10U
#define LPADC_OFFSET_VALUE_B        10U

#define APP_EXCLUDE_FROM_DEEPSLEEP (kPDRUNCFG_PD_DCDC | kPDRUNCFG_PD_FRO192M | kPDRUNCFG_PD_FRO32K)

/* Defines used by the sleep code */
#define LPC_CLOCK_INTERNAL_IRC BOARD_BootClockFRO12M()
#define LPC_CLOCK_RUN          BOARD_BootClockFROHF96M()

#define DEVICE_ID_LENGTH       24

#include "objects.h"

#endif
