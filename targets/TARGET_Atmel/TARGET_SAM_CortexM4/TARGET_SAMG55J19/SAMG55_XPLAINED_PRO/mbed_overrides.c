/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "compiler.h"
#include "sysclk.h"

uint8_t g_sys_init = 0;

//called before main - implement here if board needs it ortherwise, let
// the application override this if necessary
//TODO: To be implemented by adding system init and board init
void mbed_sdk_init()
{
    if(g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }
}
/***************************************************************/
