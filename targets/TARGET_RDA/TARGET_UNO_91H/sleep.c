/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include "cmsis.h"
#include "mbed_interface.h"

#define DEFAULT_PD_BOOTJUMPADDR (0x18001004UL)

void hal_sleep(void)
{

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    // wait for interrupt
    __DSB();
    __WFI();
}

/*
* The mbed UNO_91H does not support the deepsleep mode
* as a debugger is connected to it (the mbed interface).
*
* We treat a deepsleep() as a normal sleep().
*/

void hal_deepsleep(void)
{
    uint32_t regval, gpio_num = 4;
    /* Set pd write_en */
    RDA_SCU->PWRCTRL |= (0x01UL << 8);
    regval = RDA_SCU->PWRCTRL | (0x01UL << 3); // gpio wakeup en
    regval &= ~(0x0FUL << 9);
    regval |=  (gpio_num << 9); // set gpio number
    RDA_SCU->PWRCTRL  = regval & ~(0x01UL << 14); // posedge
    RDA_SCU->BOOTJUMPADDRCFG = DEFAULT_PD_BOOTJUMPADDR;
    /* Clr pd write_en */
    RDA_SCU->PWRCTRL &= ~(0x01UL << 8);
    osDelay(1);
    rda_ccfg_ckrst();
    osDelay(1);
    RDA_SCU->PWRCTRL |= (0x01UL << 25); // goto low-power mode
}
