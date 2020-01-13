/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "cmsis.h"
#include "os_tick.h"


#ifndef SYSTICK_IRQ_PRIORITY
#define SYSTICK_IRQ_PRIORITY    0xFFU
#endif

static uint8_t PendST;

/**
  * @brief  HAL Status structures definition
  */
#define  TICK_INT_PRIORITY            ((uint32_t)(1U<<__NVIC_PRIO_BITS) - 1U) /*!< tick interrupt priority (lowest by default)             */
/*  Warning: Must be set to higher priority for HAL_Delay()  */
/*  and HAL_GetTick() usage under interrupt context          */
/**
  * @brief  Initializes the System Timer and its interrupt, and starts the System Tick Timer.
  *         Counter is in free running mode to generate periodic interrupts.
  * @param  TicksNumb Specifies the ticks Number of ticks between two interrupts.
  * @retval status:  - 0  Function succeeded.
  *                  - 1  Function failed.
  */
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb)
{
    return SysTick_Config(TicksNumb);
}

/**
  * @brief This function configures the source of the time base:
  *        The time source is configured to have 1ms time base with a dedicated
  *        Tick interrupt priority.
  * @note This function is called  automatically at the beginning of program after
  *       reset by HAL_Init() or at any time when clock is reconfigured  by HAL_RCC_ClockConfig().
  * @note In the default implementation, SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals.
  *       Care must be taken if HAL_Delay() is called from a peripheral ISR process,
  *       The SysTick interrupt must have higher priority (numerically lower)
  *       than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
  *       The function is declared as __weak  to be overwritten  in case of other
  *       implementation  in user file.
  * @param TickPriority  Tick interrupt priority.
  * @retval HAL status
  */
#define SYSTICK_CLOCK (26000000)
#define SYSTICK_RELOAD (SYSTICK_CLOCK / 100) + 1 //10ms
int32_t OS_Tick_Setup(uint32_t freq, IRQHandler_t handler)
{
    uint32_t load;
    (void)handler;

    if (freq == 0U) {
        //lint -e{904} "Return statement before end of function"
        return (-1);
    }

    load = (SYSTICK_CLOCK / freq) - 1U;
    if (load > 0x00FFFFFFU) {
        //lint -e{904} "Return statement before end of function"
        return (-1);
    }

    // Set SysTick Interrupt Priority
#if   ((defined(__ARM_ARCH_8M_MAIN__) && (__ARM_ARCH_8M_MAIN__ != 0)) || \
       (defined(__CORTEX_M)           && (__CORTEX_M           == 7U)))
    SCB->SHPR[11] = SYSTICK_IRQ_PRIORITY;
#elif  (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
    SCB->SHPR[1] |= ((uint32_t)SYSTICK_IRQ_PRIORITY << 24);
#elif ((defined(__ARM_ARCH_7M__)      && (__ARM_ARCH_7M__      != 0)) || \
       (defined(__ARM_ARCH_7EM__)     && (__ARM_ARCH_7EM__     != 0)))
    SCB->SHP[11]  = SYSTICK_IRQ_PRIORITY;
#elif  (defined(__ARM_ARCH_6M__)      && (__ARM_ARCH_6M__      != 0))
    SCB->SHP[1]  |= ((uint32_t)SYSTICK_IRQ_PRIORITY << 24);
#else
#error "Unknown ARM Core!"
#endif
    /*
     override systick clock source
     NVIC_SYSTICK_CTRL_CLKSOURCE=1 : Use processor clock
     NVIC_SYSTICK_CTRL_CLKSOURCE=0 : Use OSC clock
    */

    SysTick->CTRL =  0x10003;//SysTick_CTRL_COUNTFLAG_Msk | /*SysTick_CTRL_CLKSOURCE_Msk |*/ SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD =  load;
//  SysTick->VAL  =  0U;

    PendST = 0U;

    return (0);
}

void OS_Tick_Enable(void)
{

}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /*Configure the SysTick to have interrupt in 1ms time basis*/
    HAL_SYSTICK_Config(SYSTICK_RELOAD);

    /*Configure the SysTick IRQ priority */
    //HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority ,0);

    /* Return function status */
    return HAL_OK;
}

#if defined ( __ICCARM__ )
#define S5JS100_BOOTMEM_BASE 0x00000000
extern uint32_t __vector_table;
extern uint32_t __Vectors_Size;
#elif defined (TOOLCHAIN_ARM_STD)/*__ARMCC_VERSION)*/
#define S5JS100_BOOTMEM_BASE 0x00000000
extern uint32_t __Vectors;
extern uint32_t __Vectors_Size;
#elif defined ( __GNUC__ )
extern uint32_t _v_start;
extern uint32_t __vector_table;
extern uint32_t __vector_table_end;
extern uint32_t __isr_vector[];
#endif
extern int cal_init(void);

/*
 * SystemCoreConfig(): Configure the System Core
 */
void SystemCoreConfig()
{

    const uint32_t *src;
    uint32_t *dest, size;

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    SCB->VTOR = (uint32_t) 0;
#endif

#if defined ( __ICCARM__ )
    src = &__vector_table;

    dest = (uint32_t *)S5JS100_BOOTMEM_BASE;
    size = (uint32_t)&__Vectors_Size;

    /* Move vector table to the right location. */
    for (src = &__vector_table, dest = (uint32_t *)S5JS100_BOOTMEM_BASE; dest < ((volatile unsigned int *)S5JS100_BOOTMEM_BASE + size);) {
        *dest++ = *src++;
        //*dest++ = *src++;
    }

#elif defined (TOOLCHAIN_ARM_STD)/*__ARMCC_VERSION)*/
    src = (uint32_t *)&__Vectors;

    dest = (uint32_t *)S5JS100_BOOTMEM_BASE;
    size = (uint32_t)&__Vectors_Size;

    /* Move vector table to the right location. */
    for (dest = (uint32_t *)S5JS100_BOOTMEM_BASE; (uint32_t)dest < ((uint32_t)S5JS100_BOOTMEM_BASE + size);) {
        *dest++ = *src++;
        //*dest++ = *src++;
    }
#elif defined ( __GNUC__ )
    src = __isr_vector;
    dest = &_v_start;
    size = (uint32_t)&__vector_table_end - (uint32_t)&__vector_table;

    /* Move vector table to the right location. */
    for (src = __isr_vector, dest = &_v_start; dest < (&_v_start + size);) {
        *dest++ = *src++;
    }
#endif

    cal_init();

    HAL_InitTick(TICK_INT_PRIORITY);
}

/* POWER MANAGEMENT */

/*
 * SystemPowerConfig(): Configures the System Power Modes
 */
void SystemPowerConfig()
{
}

/*
 * SystemPowerSuspend(): Enters in System Suspend
 */
void SystemPowerSuspend(power_mode_t mode)
{
    if (mode == POWER_MODE_DEEP_SLEEP) {
        /* Enable deepsleep */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        /* Ensure effect of last store takes effect */
        __DSB();
        /* Enter sleep mode */
        __WFI();
    } else {
        /* Enter sleep mode */
        __WFI();
    }
}

/*
 * SystemPowerResume(): Returns from System Suspend
 */
void SystemPowerResume(power_mode_t mode)
{
    if (mode == POWER_MODE_DEEP_SLEEP) {
        /* Disable sleeponexit */
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
        /* Ensure effect of last store takes effect */
        __DSB();
    }
}


/*
 * System config data storage functions
 * Reserved as the data is not strictly persistent
 */

