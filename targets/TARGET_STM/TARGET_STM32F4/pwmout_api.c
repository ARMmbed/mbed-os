/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 *******************************************************************************
 */
#include "pwmout_api.h"

#if DEVICE_PWMOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

static TIM_HandleTypeDef TimHandle;

void pwmout_init(pwmout_t* obj, PinName pin)
{
    // Get the peripheral name from the pin and assign it to the object
    obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(obj->pwm != (PWMName)NC);

    // Get the functions (timer channel, (non)inverted) from the pin and assign it to the object
    uint32_t function = pinmap_function(pin, PinMap_PWM);
    MBED_ASSERT(function != (uint32_t)NC);
    obj->channel = STM_PIN_CHANNEL(function);
    obj->inverted = STM_PIN_INVERTED(function);

    // Enable TIM clock
#if defined(TIM1_BASE)
    if (obj->pwm == PWM_1) __HAL_RCC_TIM1_CLK_ENABLE();
#endif
#if defined(TIM2_BASE)
    if (obj->pwm == PWM_2) __HAL_RCC_TIM2_CLK_ENABLE();
#endif
#if defined(TIM3_BASE)
    if (obj->pwm == PWM_3) __HAL_RCC_TIM3_CLK_ENABLE();
#endif
#if defined(TIM4_BASE)
    if (obj->pwm == PWM_4) __HAL_RCC_TIM4_CLK_ENABLE();
#endif
#if defined(TIM5_BASE)
    if (obj->pwm == PWM_5) __HAL_RCC_TIM5_CLK_ENABLE();
#endif
#if defined(TIM8_BASE)
    if (obj->pwm == PWM_8) __HAL_RCC_TIM8_CLK_ENABLE();
#endif
#if defined(TIM9_BASE)
    if (obj->pwm == PWM_9) __HAL_RCC_TIM9_CLK_ENABLE();
#endif
#if defined(TIM10_BASE)
    if (obj->pwm == PWM_10) __HAL_RCC_TIM10_CLK_ENABLE();
#endif
#if defined(TIM11_BASE)
    if (obj->pwm == PWM_11) __HAL_RCC_TIM11_CLK_ENABLE();
#endif
#if defined(TIM12_BASE)
    if (obj->pwm == PWM_12) __HAL_RCC_TIM12_CLK_ENABLE();
#endif
#if defined(TIM13_BASE)
    if (obj->pwm == PWM_13) __HAL_RCC_TIM13_CLK_ENABLE();
#endif
#if defined(TIM14_BASE)
    if (obj->pwm == PWM_14) __HAL_RCC_TIM14_CLK_ENABLE();
#endif

    // Configure GPIO
    pinmap_pinout(pin, PinMap_PWM);

    obj->pin = pin;
    obj->period = 0;
    obj->pulse = 0;
    obj->prescaler = 1;

    pwmout_period_us(obj, 20000); // 20 ms per default
}

void pwmout_free(pwmout_t* obj)
{
    // Configure GPIO
    pin_function(obj->pin, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
}

void pwmout_write(pwmout_t* obj, float value)
{
    TIM_OC_InitTypeDef sConfig;
    int channel = 0;

    TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);

    if (value < (float)0.0) {
        value = 0.0;
    } else if (value > (float)1.0) {
        value = 1.0;
    }

    obj->pulse = (uint32_t)((float)obj->period * value);

    // Configure channels
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.Pulse        = obj->pulse / obj->prescaler;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    switch (obj->channel) {
        case 1:
            channel = TIM_CHANNEL_1;
            break;
        case 2:
            channel = TIM_CHANNEL_2;
            break;
        case 3:
            channel = TIM_CHANNEL_3;
            break;
        case 4:
            channel = TIM_CHANNEL_4;
            break;
        default:
            return;
    }

    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK) {
        error("Cannot initialize PWM\n");
    }

    if (obj->inverted) {
        HAL_TIMEx_PWMN_Start(&TimHandle, channel);
    } else {
        HAL_TIM_PWM_Start(&TimHandle, channel);
    }
}

float pwmout_read(pwmout_t* obj)
{
    float value = 0;
    if (obj->period > 0) {
        value = (float)(obj->pulse) / (float)(obj->period);
    }
    return ((value > (float)1.0) ? (float)(1.0) : (value));
}

void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

void pwmout_period_us(pwmout_t* obj, int us)
{
    TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    uint32_t PclkFreq;
    uint32_t APBxCLKDivider;
    float dc = pwmout_read(obj);

    __HAL_TIM_DISABLE(&TimHandle);

    // Get clock configuration
    // Note: PclkFreq contains here the Latency (not used after)
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &PclkFreq);

    // Get the PCLK and APBCLK divider related to the timer
    switch (obj->pwm) {
      
        // APB1 clock
#if defined(TIM2_BASE)
        case PWM_2:
#endif
#if defined(TIM3_BASE)
        case PWM_3:
#endif
#if defined(TIM4_BASE)
        case PWM_4:
#endif
#if defined(TIM5_BASE)
        case PWM_5:
#endif
#if defined(TIM12_BASE)
        case PWM_12:
#endif
#if defined(TIM13_BASE)
        case PWM_13:
#endif
#if defined(TIM14_BASE)
        case PWM_14:
#endif
            PclkFreq = HAL_RCC_GetPCLK1Freq();
            APBxCLKDivider = RCC_ClkInitStruct.APB1CLKDivider;
            break;
        
        // APB2 clock
#if defined(TIM1_BASE)
        case PWM_1:
#endif
#if defined(TIM8_BASE)
        case PWM_8:
#endif
#if defined(TIM9_BASE)
        case PWM_9:
#endif
#if defined(TIM10_BASE)
        case PWM_10:
#endif
#if defined(TIM11_BASE)
        case PWM_11:
#endif
            PclkFreq = HAL_RCC_GetPCLK2Freq();
            APBxCLKDivider = RCC_ClkInitStruct.APB2CLKDivider;
            break;
        default:
            return;
    }

    /* To make it simple, we use to possible prescaler values which lead to:
     * pwm unit = 1us, period/pulse can be from 1us to 65535us
     * or
     * pwm unit = 500us, period/pulse can be from 500us to ~32.76sec
     * Be careful that all the channels of a PWM shares the same prescaler
     */
    if (us >  0xFFFF) {
        obj->prescaler = 500;
    } else {
        obj->prescaler = 1;
    }

    // TIMxCLK = PCLKx when the APB prescaler = 1 else TIMxCLK = 2 * PCLKx
    if (APBxCLKDivider == RCC_HCLK_DIV1)
      TimHandle.Init.Prescaler   = (uint16_t)(((PclkFreq) / 1000000) * obj->prescaler) - 1; // 1 us tick
    else
      TimHandle.Init.Prescaler   = (uint16_t)(((PclkFreq * 2) / 1000000) * obj->prescaler) - 1; // 1 us tick

    if (TimHandle.Init.Prescaler > 0xFFFF)
        error("PWM: out of range prescaler");

    TimHandle.Init.Period        = (us - 1) / obj->prescaler;
    if (TimHandle.Init.Period > 0xFFFF)
        error("PWM: out of range period");

    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK) {
        error("Cannot initialize PWM\n");
    }

    // Save for future use
    obj->period = us;

    // Set duty cycle again
    pwmout_write(obj, dc);

    __HAL_TIM_ENABLE(&TimHandle);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us)
{
    float value = (float)us / (float)obj->period;
    pwmout_write(obj, value);
}

#endif
