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

#if DEVICE_USTICKER

#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "us_ticker_data.h"
#include "us_ticker_defines.h"

TIM_HandleTypeDef TimMasterHandle;

uint32_t timer_cnt_reg;
uint32_t timer_ccr1_reg;
uint32_t timer_dier_reg;

const ticker_info_t *us_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,
        TIM_MST_BIT_WIDTH
    };
    return &info;
}

void us_ticker_irq_handler(void);

// ************************************ 16-bit timer ************************************
#if TIM_MST_BIT_WIDTH == 16

extern uint32_t prev_time;
extern uint32_t elapsed_time;

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

void timer_oc_irq_handler(void)
{
    TimMasterHandle.Instance = TIM_MST;
#endif
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1) == SET) {
        if (__HAL_TIM_GET_IT_SOURCE(&TimMasterHandle, TIM_IT_CC1) == SET) {
            __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
            us_ticker_irq_handler();
        }
    }
}

void init_16bit_timer(void)
{
    // Enable timer clock
    TIM_MST_RCC;

    // Reset timer
#if defined(DUAL_CORE)
    uint32_t timeout = HSEM_TIMEOUT;
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID) && (--timeout != 0)) {
    }
#endif /* DUAL_CORE */
    TIM_MST_RESET_ON;
    TIM_MST_RESET_OFF;
#if defined(DUAL_CORE)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */

    // Update the SystemCoreClock variable
    SystemCoreClockUpdate();

    // Configure time base
    TimMasterHandle.Instance           = TIM_MST;
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

    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);

    // Used by HAL_GetTick()
    prev_time = 0;
    elapsed_time = 0;
}

// ************************************ 32-bit timer ************************************
#else

void timer_irq_handler(void)
{
    TimMasterHandle.Instance = TIM_MST;
    if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1) == SET) {
        if (__HAL_TIM_GET_IT_SOURCE(&TimMasterHandle, TIM_IT_CC1) == SET) {
            __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
            us_ticker_irq_handler();
        }
    }
}

void init_32bit_timer(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    uint32_t PclkFreq;

    // Get clock configuration
    // Note: PclkFreq contains here the Latency (not used after)
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &PclkFreq);

    // Get timer clock value
#if TIM_MST_PCLK == 1
    PclkFreq = HAL_RCC_GetPCLK1Freq();
#else
    PclkFreq = HAL_RCC_GetPCLK2Freq();
#endif

    // Enable timer clock
    TIM_MST_RCC;

    // Reset timer
#if defined(DUAL_CORE)
    uint32_t timeout = HSEM_TIMEOUT;
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID) && (--timeout != 0)) {
    }
#endif /* DUAL_CORE */
    TIM_MST_RESET_ON;
    TIM_MST_RESET_OFF;
#if defined(DUAL_CORE)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */

    // Configure time base
    TimMasterHandle.Instance    = TIM_MST;
    TimMasterHandle.Init.Period = 0xFFFFFFFF;

    // TIMxCLK = PCLKx when the APB prescaler = 1 else TIMxCLK = 2 * PCLKx
#if TIM_MST_PCLK == 1
    if (RCC_ClkInitStruct.APB1CLKDivider == RCC_HCLK_DIV1) {
#else
    if (RCC_ClkInitStruct.APB2CLKDivider == RCC_HCLK_DIV1) {
#endif
        TimMasterHandle.Init.Prescaler   = (uint16_t)((PclkFreq) / 1000000) - 1; // 1 us tick
    } else {
        TimMasterHandle.Init.Prescaler   = (uint16_t)((PclkFreq * 2) / 1000000) - 1; // 1 us tick
    }

    TimMasterHandle.Init.ClockDivision     = 0;
    TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
#if !TARGET_STM32L1
    TimMasterHandle.Init.RepetitionCounter = 0;
#endif
#ifdef TIM_AUTORELOAD_PRELOAD_DISABLE
    TimMasterHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
#endif
    HAL_TIM_OC_Init(&TimMasterHandle);

    NVIC_SetVector(TIM_MST_IRQ, (uint32_t)timer_irq_handler);
    NVIC_EnableIRQ(TIM_MST_IRQ);

    // Channel 1 for mbed timeout
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);

    // Freeze timer on stop/breakpoint
    // Define the FREEZE_TIMER_ON_DEBUG macro in mbed_app.json for example
#if !defined(NDEBUG) && defined(FREEZE_TIMER_ON_DEBUG) && defined(TIM_MST_DBGMCU_FREEZE)
    TIM_MST_DBGMCU_FREEZE;
#endif

    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

#endif // 16-bit/32-bit timer

void us_ticker_init(void)
{
    // Timer is already initialized in HAL_InitTick()
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);
}

uint32_t (us_ticker_read)()
{
    /* Invoke the macro */
    return us_ticker_read();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    // NOTE: This function must be called with interrupts disabled to keep our
    //       timer interrupt setup atomic
    // Set new output compare value
    __HAL_TIM_SET_COMPARE(&TimMasterHandle, TIM_CHANNEL_1, (uint32_t)timestamp);
    // Ensure the compare event starts clear
    __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
    // Enable IT
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_fire_interrupt(void)
{
    __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
    LL_TIM_GenerateEvent_CC1(TimMasterHandle.Instance);
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

void us_ticker_disable_interrupt(void)
{
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

/* NOTE: must be called with interrupts disabled! */
void us_ticker_clear_interrupt(void)
{
    __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
}

void save_timer_ctx(void)
{
    timer_cnt_reg = __HAL_TIM_GET_COUNTER(&TimMasterHandle);
    timer_ccr1_reg = __HAL_TIM_GET_COMPARE(&TimMasterHandle, TIM_CHANNEL_1);
    timer_dier_reg = TIM_MST->DIER;
}

void restore_timer_ctx(void)
{
    __HAL_TIM_SET_COUNTER(&TimMasterHandle, timer_cnt_reg);
    __HAL_TIM_SET_COMPARE(&TimMasterHandle, TIM_CHANNEL_1, timer_ccr1_reg);
    TIM_MST->DIER = timer_dier_reg;
}

void us_ticker_free(void)
{
    HAL_TIM_OC_Stop(&TimMasterHandle, TIM_CHANNEL_1);
    us_ticker_disable_interrupt();
}

#endif /* DEVICE_USTICKER */
