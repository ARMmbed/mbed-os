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

#define DEBUG_TICK 0 // Set to 1 to toggle a pin (see below which pin) at each tick

extern TIM_HandleTypeDef TimMasterHandle;

extern volatile uint32_t SlaveCounter;
extern volatile uint32_t oc_int_part;

volatile uint32_t PreviousVal = 0;

void us_ticker_irq_handler(void);

#if defined(TARGET_STM32F0)
void timer_update_irq_handler(void) {
#else
void timer_irq_handler(void)
{
#endif
    TimMasterHandle.Instance = TIM_MST;

    // Clear Update interrupt flag
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_UPDATE) == SET) {
        if (__HAL_TIM_GET_IT_SOURCE(&TimMasterHandle, TIM_IT_UPDATE) == SET) {
            __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_UPDATE);
            SlaveCounter++;
        }
    }

#if defined(TARGET_STM32F0)
} // end timer_update_irq_handler function
// Used for mbed timeout (channel 1) and HAL tick (channel 2)
void timer_oc_irq_handler(void)
{
    TimMasterHandle.Instance = TIM_MST;
#endif

    // Channel 1 for mbed timeout
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1) == SET) {
        if (__HAL_TIM_GET_IT_SOURCE(&TimMasterHandle, TIM_IT_CC1) == SET) {
            __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);

                if (oc_int_part > 0) {
                    oc_int_part--;
                } else {
                   us_ticker_irq_handler();
                }
        }
    }

    // Channel 2 for HAL tick
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC2) == SET) {

        if (__HAL_TIM_GET_IT_SOURCE(&TimMasterHandle, TIM_IT_CC2) == SET) {
            __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC2);
            uint32_t val = __HAL_TIM_GET_COUNTER(&TimMasterHandle);
            if ((val - PreviousVal) >= HAL_TICK_DELAY) {
                // Increment HAL variable
                HAL_IncTick();
                // Prepare next interrupt
                __HAL_TIM_SET_COMPARE(&TimMasterHandle, TIM_CHANNEL_2, val + HAL_TICK_DELAY);
                PreviousVal = val;
#if DEBUG_TICK > 0
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
#endif
            }
        }
    }
}

// Reconfigure the HAL tick using a standard timer instead of systick.
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

    //LL_TIM_EnableUpdateEvent(TimMasterHandle.Instance);

    // Configure output compare channel 1 for mbed timeout (enabled later when used)
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);

    // Configure output compare channel 2 for HAL tick
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_2);
    PreviousVal = __HAL_TIM_GET_COUNTER(&TimMasterHandle);
    __HAL_TIM_SET_COMPARE(&TimMasterHandle, TIM_CHANNEL_2, PreviousVal + HAL_TICK_DELAY);



    // Configure interrupts
    // Update interrupt used for 32-bit counter
    // Output compare channel 1 interrupt for mbed timeout
    // Output compare channel 2 interrupt for HAL tick
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

    // Enable interrupts
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_UPDATE); // For 32-bit counter
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC2); // For HAL tick

    // Enable timer
    HAL_TIM_Base_Start(&TimMasterHandle);

    // Freeze timer on stop/breakpoint
    // Define the FREEZE_TIMER_ON_DEBUG macro in mbed_app.json for example
#if !defined(NDEBUG) && defined(FREEZE_TIMER_ON_DEBUG) && defined(TIM_MST_DBGMCU_FREEZE)
    TIM_MST_DBGMCU_FREEZE;
#endif

#if DEBUG_TICK > 0
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

    return HAL_OK;
}

/* NOTE: must be called with interrupts disabled! */
void HAL_SuspendTick(void)
{
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC2);
}

/* NOTE: must be called with interrupts disabled! */
void HAL_ResumeTick(void)
{
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC2);
}

#endif // TIM_MST_16BIT
