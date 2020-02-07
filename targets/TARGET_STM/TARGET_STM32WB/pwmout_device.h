/* mbed Microcontroller Library
 * Copyright (c) 2019 STMicroelectronics
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

#ifndef MBED_PWMOUT_DEVICE_H
#define MBED_PWMOUT_DEVICE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEVICE_PWMOUT

typedef enum {
    PWMOUT_ON_APB1 = 0,
    PWMOUT_ON_APB2 = 1,
    PWMOUT_UNKNOWN = 2
} PwmoutApb;

/*  Structure to describe Timers to APB */
typedef struct pwm_apb_map {
    PWMName pwm;   // an index entry for each EXIT line
    PwmoutApb pwmoutApb;
} pwm_apb_map_t;

extern const pwm_apb_map_t pwm_apb_map_table[];

#endif // DEVICE_PWMOUT

#ifdef __cplusplus
}
#endif

#endif
