/* mbed Microcontroller Library
* Copyright (c) 2019 Uhuru Corporation
*
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
#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "pin_device.h"
#include "uhuru_raven_init.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * TABLES
 * ----------------------------------------------------------------*/
typedef struct {
    PinName pin;
    PinDirection dir;
    PinMode mode;
} PIN_DATA;

// pin init status
const PIN_DATA pin_init_status[] = {
    {PA_0 , PIN_INPUT , PullDown},   // PA_0
    {PA_1 , PIN_INPUT , PullDown},   // PA_1
    {PA_2 , PIN_INPUT , PullUp},     // PA_2
    {PA_3 , PIN_INPUT , PullDown},   // PA_3
    {PA_4 , PIN_INPUT , PullDown},   // PA_4
    {PA_5 , PIN_INPUT , PullDown},   // PA_5
    {PA_6 , PIN_INPUT , PullDown},   // PA_6
    {PA_7 , PIN_INPUT , PullDown},   // PA_7
    {PA_8 , PIN_INPUT , PullDown},   // PA_8
    {PA_9 , PIN_INPUT , PullDown},   // PA_9
    {PA_10 , PIN_INPUT , PullDown},  // PA_10
    {PA_11 , PIN_INPUT , PullDown},  // PA_11
    {PA_12 , PIN_INPUT , PullDown},  // PA_12
    {PA_15 , PIN_INPUT , PullDown},  // PA_15
    {PB_0 , PIN_INPUT , PullDown},   // PB_0
    {PB_1 , PIN_INPUT , PullDown},   // PB_1
    {PB_2 , PIN_INPUT , PullDown},   // PB_2
    {PB_4 , PIN_INPUT , PullDown},   // PB_4
    {PB_5 , PIN_INPUT , PullDown},   // PB_5
    {PB_6 , PIN_INPUT , PullDown},   // PB_6
    {PB_7 , PIN_INPUT , PullDown},   // PB_7
    {PB_8 , PIN_INPUT , PullUp},     // PB_8
    {PB_9 , PIN_INPUT , PullUp},     // PB_9
    {PB_10 , PIN_OUTPUT, PullUp},    // PB_10
    {PB_11 , PIN_INPUT , PullDown},  // PB_11
    {PB_12 , PIN_OUTPUT, PullDown},  // PB_12
    {PB_13 , PIN_OUTPUT, PullUp},    // PB_13
    {PB_14 , PIN_INPUT , PullUp},    // PB_14
    {PB_15 , PIN_INPUT , PullDown},  // PB_15
    {PC_0 , PIN_INPUT , PullDown},   // PC_0
    {PC_1 , PIN_INPUT , PullDown},   // PC_1
    {PC_2 , PIN_INPUT , PullDown},   // PC_2
    {PC_3 , PIN_INPUT , PullDown},   // PC_3
    {PC_4 , PIN_INPUT , PullDown},   // PC_4
    {PC_5 , PIN_INPUT , PullDown},   // PC_5
    {PC_6 , PIN_INPUT , PullDown},   // PC_6
    {PC_7 , PIN_INPUT , PullDown},   // PC_7
    {PC_8 , PIN_INPUT , PullDown},   // PC_8
    {PC_9 , PIN_INPUT , PullDown},   // PC_9
    {PC_10 , PIN_INPUT , PullDown},  // PC_10
    {PC_11 , PIN_INPUT , PullDown},  // PC_11
    {PC_12 , PIN_INPUT , PullDown},  // PC_12
    {PC_13 , PIN_INPUT , PullUp},    // PC_13
    {PD_0 , PIN_INPUT , PullDown},   // PD_0
    {PD_1 , PIN_INPUT , PullDown},   // PD_1
    {PD_2 , PIN_INPUT , PullDown},   // PD_2
    {PD_3 , PIN_INPUT , PullDown},   // PD_3
    {PD_4 , PIN_INPUT , PullDown},   // PD_4
    {PD_5 , PIN_INPUT , PullDown},   // PD_5
    {PD_6 , PIN_INPUT , PullDown},   // PD_6
    {PD_7 , PIN_INPUT , PullDown},   // PD_7
    {PD_8 , PIN_INPUT , PullUp},     // PD_8
    {PD_9 , PIN_INPUT , PullDown},   // PD_9
    {PD_10 , PIN_INPUT , PullDown},  // PD_10
    {PD_11 , PIN_INPUT , PullNone},  // PD_11
    {PD_12 , PIN_INPUT , PullUp},   // PD_12
    {PD_13 , PIN_INPUT , PullUp},    // PD_13
    {PD_14 , PIN_INPUT , PullNone},  // PD_14
    {PD_15 , PIN_INPUT , PullDown},  // PD_15
    {PE_0 , PIN_INPUT , PullNone},   // PE_0
    {PE_1 , PIN_INPUT , PullNone},   // PE_1
    {PE_2 , PIN_INPUT , PullDown},   // PE_2
    {PE_3 , PIN_INPUT , PullDown},   // PE_3
    {PE_4 , PIN_INPUT , PullDown},   // PE_4
    {PE_5 , PIN_INPUT , PullDown},   // PE_5
    {PE_6 , PIN_INPUT , PullDown},   // PE_6
    {PE_7 , PIN_INPUT , PullDown},   // PE_7
    {PE_8 , PIN_INPUT , PullDown},   // PE_8
    {PE_9 , PIN_INPUT , PullDown},   // PE_9
    {PE_10 , PIN_OUTPUT , PullUp},   // PE_10
    {PE_11 , PIN_OUTPUT , PullUp},   // PE_11
    {PE_12 , PIN_INPUT , PullDown},  // PE_12
    {PE_13 , PIN_INPUT , PullDown},  // PE_13
    {PE_14 , PIN_INPUT , PullDown},  // PE_14
    {PE_15 , PIN_OUTPUT , PullUp},   // PE_15
    {NC , 0 , 0},
};
/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * MBED API FUNCTIONS
 * ----------------------------------------------------------------*/

void uhuru_raven_init(void)
{
    for(int i= 0 ; pin_init_status[i].pin != NC ; i++){
        gpio_t gpio;

        if(pin_init_status[i].dir == PIN_INPUT){
            gpio_init_in_ex(&gpio , pin_init_status[i].pin , pin_init_status[i].mode);
        }
        else {
            if(pin_init_status[i].mode == PullUp){
                gpio_init_out_ex(&gpio , pin_init_status[i].pin , 1);
            }
            else{
                gpio_init_out_ex(&gpio , pin_init_status[i].pin , 0);
            }
        }
    }
}
