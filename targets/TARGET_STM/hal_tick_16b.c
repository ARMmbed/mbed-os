/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
#include "hal_tick.h"

// A 16-bit timer is used
#if TIM_MST_16BIT

extern TIM_HandleTypeDef TimMasterHandle;

volatile uint32_t PreviousVal = 0;

void us_ticker_irq_handler(void);

#if defined(TARGET_STM32F0)
void timer_update_irq_handler(void)
{
#else
void timer_irq_handler(void)
{
#endif
    TimMasterHandle.Instance = TIM_MST;

#if defined(TARGET_STM32F0)
} // end timer_update_irq_handler function

// Channel 1 used for mbed timeout
void timer_oc_irq_handler(void)
{
    TimMasterHandle.Instance = TIM_MST;
#endif

    // Channel 1 for mbed timeout
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1) == SET) {
        if (__HAL_TIM_GET_IT_SOURCE(&TimMasterHandle, TIM_IT_CC1) == SET) {
            __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
            us_ticker_irq_handler();
        }
    }
}

// Overwrite the default ST HAL function (defined as "weak") in order to configure an HW timer
// used for mbed timeouts only (not used for the Systick configuration).
// Additional notes:
// - The default ST HAL_InitTick function initializes the Systick to 1 ms and this is not correct for mbed
//   as the mbed Systick as to be configured to 1 us instead.
// - Furthermore the Systick is configured by mbed RTOS directly.
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    // Enable timer clock
    TIM_MST_RCC;

    // Reset timer
    TIM_MST_RESET_ON;
    TIM_MST_RESET_OFF;

    // Update the SystemCoreClock variable
    SystemCoreClockUpdate();

    // Configure time base
    TimMasterHandle.Instance = TIM_MST;
    TimMasterHandle.Init.Period        = 0xFFFF;
    TimMasterHandle.Init.Prescaler     = (uint32_t)(SystemCoreClock / 1000000) - 1; // 1 us tick
    TimMasterHandle.Init.ClockDivision = 0;
    TimMasterHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
#if !defined(TARGET_STM32L0) && !defined(TARGET_STM32L1)
    TimMasterHandle.Init.RepetitionCounter = 0;
#endif
#ifdef TIM_AUTORELOAD_PRELOAD_DISABLE
    TimMasterHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
#endif
    HAL_TIM_Base_Init(&TimMasterHandle);

    // Configure output compare channel 1 for mbed timeout (enabled later when used)
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);

    // Output compare channel 1 interrupt for mbed timeout
#if defined(TARGET_STM32F0)
    NVIC_SetVector(TIM_MST_UP_IRQ, (uint32_t)timer_update_irq_handler);
    NVIC_EnableIRQ(TIM_MST_UP_IRQ);
    NVIC_SetPriority(TIM_MST_UP_IRQ, 0);
    NVIC_SetVector(TIM_MST_OC_IRQ, (uint32_t)timer_oc_irq_handler);
    NVIC_EnableIRQ(TIM_MST_OC_IRQ);
    NVIC_SetPriority(TIM_MST_OC_IRQ, 1);
#else
    NVIC_SetVector(TIM_MST_IRQ, (uint32_t)timer_irq_handler);
    NVIC_EnableIRQ(TIM_MST_IRQ);
#endif

    // Enable timer
    HAL_TIM_Base_Start(&TimMasterHandle);

    // Freeze timer on stop/breakpoint
    // Define the FREEZE_TIMER_ON_DEBUG macro in mbed_app.json for example
#if !defined(NDEBUG) && defined(FREEZE_TIMER_ON_DEBUG) && defined(TIM_MST_DBGMCU_FREEZE)
    TIM_MST_DBGMCU_FREEZE;
#endif

    return HAL_OK;
}

#endif // TIM_MST_16BIT
