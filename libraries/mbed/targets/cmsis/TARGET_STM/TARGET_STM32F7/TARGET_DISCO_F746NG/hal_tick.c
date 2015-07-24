/**
  ******************************************************************************
  * @file    hal_tick.c
  * @author  MCD Application Team
  * @brief   Initialization of HAL tick
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "hal_tick.h"

// Enable it to measure the tick period on  ARDUINO-D2 pin (PG6)
#define DEBUG_TICK 0

TIM_HandleTypeDef TimMasterHandle;
uint32_t PreviousVal = 0;

void HAL_IncTick(void);
void us_ticker_irq_handler(void);

void timer_irq_handler(void) {
    // Channel 1 for mbed timeout
    if (__HAL_TIM_GET_ITSTATUS(&TimMasterHandle, TIM_IT_CC1) == SET) {
        __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC1);
        us_ticker_irq_handler();
    }

    // Channel 2 for HAL tick
    if (__HAL_TIM_GET_ITSTATUS(&TimMasterHandle, TIM_IT_CC2) == SET) {
        __HAL_TIM_CLEAR_IT(&TimMasterHandle, TIM_IT_CC2);
        uint32_t val = __HAL_TIM_GetCounter(&TimMasterHandle);
        if ((val - PreviousVal) >= HAL_TICK_DELAY) {
            // Increment HAL variable
            HAL_IncTick();
            // Prepare next interrupt
            __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_2, val + HAL_TICK_DELAY);
            PreviousVal = val;
#if DEBUG_TICK == 1
            HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_6);
#endif
        }
    }
}

// Reconfigure the HAL tick using a standard timer instead of systick.
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    // Enable timer clock
    TIM_MST_RCC;

    // Reset timer
    TIM_MST_RESET_ON;
    TIM_MST_RESET_OFF;

    // Just in case the system clock has been changed and the SystemCoreClock
    // variable not updated...
    SystemCoreClockUpdate();

    // Note on timer clock:
    // TIM5 input clock (TIM5CLK) is set to APB1 clock (PCLK1) x2,
    // since APB1 prescaler is equal to 4 (see system_stm32f7xx.c).
    // We have TIM5CLK = PCLK1*2 and PCLK1 = HCLK/4
    // So TIM5CLK = HCLK/2 = SystemCoreClock/2

    // Configure time base
    TimMasterHandle.Instance = TIM_MST;
    TimMasterHandle.Init.Period            = 0xFFFFFFFF;
    TimMasterHandle.Init.Prescaler         = (uint32_t)((SystemCoreClock / 2) / 1000000) - 1; // 1 us tick
    TimMasterHandle.Init.ClockDivision     = 0;
    TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimMasterHandle.Init.RepetitionCounter = 0;
    HAL_TIM_OC_Init(&TimMasterHandle);

    NVIC_SetVector(TIM_MST_IRQ, (uint32_t)timer_irq_handler);
    NVIC_EnableIRQ(TIM_MST_IRQ);

    // Channel 1 for mbed timeout
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1);

    // Channel 2 for HAL tick
    HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_2);
    PreviousVal = __HAL_TIM_GetCounter(&TimMasterHandle);
    __HAL_TIM_SetCompare(&TimMasterHandle, TIM_CHANNEL_2, PreviousVal + HAL_TICK_DELAY);
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC2);

#if DEBUG_TICK == 1
    __GPIOG_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
#endif

    return HAL_OK;
}

/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
