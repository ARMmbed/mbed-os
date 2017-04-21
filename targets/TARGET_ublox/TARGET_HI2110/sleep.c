/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

#include "sleep_api.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * STATIC FUNCTIONS
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * MBED API CALLS
 * ----------------------------------------------------------------*/

void hal_sleep(void)
{
    __DSB();
    __WFI();
    __ISB();
}

void hal_deepsleep()
{
    hal_sleep();
}
