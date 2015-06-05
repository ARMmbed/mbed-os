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
//#include "system.h"
#include "mbed_assert.h"
#include "compiler.h"

//#include "core_cm0plus.h"
//#include "comp_sysctrl.h"

void system_clock_init(void)
{
 //   struct Sysctrl* syscontrol = 0x40000800UL;
 //   syscontrol->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET |
 //           SYSCTRL_INTFLAG_DFLLRDY;
	
}

void system_board_init(void)
{

}

 //called before main - implement here if board needs it ortherwise, let
 // the application override this if necessary
void mbed_sdk_init()
{
    system_clock_init();
    system_board_init();
 //   _system_events_init();
 //   _system_extint_init();
}
/***************************************************************/
