/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#include "cmsis.h"
#include "systick_timer.h"

volatile uint32_t MyTicks;

/* Private Data SysTick */
static uint32_t clock;

/*
 * This Timer is written for MBED OS so the interrupt
 * is used to keep track of the overflow.
 */
#define SYSTICK_MAX_RELOAD    0xFFFFFFFF
#define SYSTICK_DIVIDER_US    (SystemCoreClock/1000000)

void SysTick_IRQ_Handler(void)
{
    MyTicks++;
#if 0
    printf("System Tick Interrupt: %u\n\r", MyTicks);
#endif
}

/*
 * SysTick_Initialize(): Initializes the SysTick timer
 */
void SysTick_Initialize(void)
{
    clock = SYSTICK_MAX_RELOAD;
#if 0
    printf("\n\rEnable System Tick Interrupt...\n\r");
#endif
    MyTicks=0;

    /* SysTick Reload Value Register */
    SysTick->LOAD = clock;

    /*
     * SysTick_CTRL_CLKSOURCE_Msk : Use core's clock
     * SysTick_CTRL_ENABLE_Msk : Enable SysTick
     * SysTick_CTRL_TICKINT_Msk : Active the SysTick interrupt on the NVIC
     */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
        | SysTick_CTRL_ENABLE_Msk
        | SysTick_CTRL_TICKINT_Msk;

    /* Register the Interrupt */
    NVIC_SetVector(SysTick_IRQn, (uint32_t)SysTick_IRQ_Handler);
    NVIC_EnableIRQ(SysTick_IRQn);
}

/*
 * SysTick_Disable(): Disables the SysTick timer
 */
void SysTick_Disable(void)
{
    /* Disable SysTick */
    SysTick->CTRL = 0;
}

/*
 * SysTick_Read(): Read SysTick Value
 * @return: the SysTick VALUE
 */
uint32_t SysTick_Read(void)
{
    return ((clock - (SysTick->VAL)) / SYSTICK_DIVIDER_US);
}

/*
 * SysTick_Overflow(): Read SysTick Overflow Value
 * @return: the SysTick Overflow VALUE
 */
uint32_t SysTick_Overflow(void)
{
    return MyTicks;
}
