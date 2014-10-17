/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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

// TIM5 cannot be used because already used by the us_ticker
static const PinMap PinMap_PWM[] = {
    /*
     * The lines below show all combinations to conect a port pin with a timer. Commented
     * lines are alternative possibilities not used per default. But they can be changed
     * manually instead of the suggested configuration. For example you can see that on 
     * PA_5 you can have a PWM using either Timer2/Channel1 or Timer8/Channel1N. Today I 
     * have decided to use Timer2/Channel1. But you can also notice that Timer2/Channel1 
     * is also used on PA_0. That means that today you cannot output two different PWM 
     * signals on PA_0 and PA_5 at the same time. If someone wants this, he will need to 
     * change the timer that is used on PA_5. This is why the other possibilities are
     * commented to make this change easier without looking deeply into the mcu datasheet.
     */
    {PA_0,  PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)}, // TIM2_CH1
    {PA_1,  PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)}, // TIM2_CH2
    {PA_2,  PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)}, // TIM2_CH3
//  {PA_2,  PWM_9, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9)}, // TIM9_CH1
    {PA_3,  PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)}, // TIM2_CH4
//  {PA_3,  PWM_9, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9)}, // TIM9_CH2
    {PA_5,  PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)}, // TIM2_CH1
//  {PA_5,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM8)}, // TIM8_CH1N
    {PA_6,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)}, // TIM3_CH1
    {PA_7,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)}, // TIM1_CH1N
//  {PA_7,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)}, // TIM3_CH2
//  {PA_7,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)}, // TIM8_CH1N
    {PA_8,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)}, // TIM1_CH1
    {PA_9,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)}, // TIM1_CH2
    {PA_10, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)}, // TIM1_CH3
    {PA_11, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)}, // TIM1_CH4
    {PA_15, PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)}, // TIM2_CH1

    {PB_0,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH2N
//  {PB_0,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH3
//  {PB_0,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH2N
    {PB_1,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH3N
//  {PB_1,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH4
//  {PB_1,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH3N
    {PB_3,  PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)},  // TIM2_CH2
    {PB_4,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH1
    {PB_5,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH2
    {PB_6,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH1
    {PB_7,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH2
    {PB_8,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH3
//  {PB_8,  PWM_10,STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM10)}, // TIM10_CH1
    {PB_9,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH4
//  {PB_9,  PWM_11,STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM11)}, // TIM11_CH1
    {PB_10, PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)},  // TIM2_CH3
    {PB_11, PWM_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2)},  // TIM2_CH4
    {PB_13, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH1N
    {PB_14, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH2N
//  {PB_14, PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH2N
    {PB_15, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH3N
//  {PB_15, PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM1_CH3N

    {PC_6,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH1
//  {PC_6,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH1
    {PC_7,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH2
//  {PC_7,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH2
    {PC_8,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH3
//  {PC_8,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH3
    {PC_9,  PWM_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3)},  // TIM3_CH4
//  {PC_9,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH4

    {PD_6,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH1
    {PD_7,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH2
    {PD_8,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH3
    {PD_9,  PWM_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4)},  // TIM4_CH4

    {PE_5,  PWM_9, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9)},  // TIM9_CH1
    {PE_6,  PWM_9, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM9)},  // TIM9_CH2

    {PE_8,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH1N
    {PE_9,  PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH1
    {PE_10, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH2N
    {PE_11, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH2
    {PE_12, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH3N
    {PE_13, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH3
    {PE_14, PWM_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1)},  // TIM1_CH4

    {PH_13, PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH1N
    {PH_14, PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH2N
    {PH_15, PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH3N

    {PI_2,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH4
    {PI_5,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH1
    {PI_6,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH2
    {PI_7,  PWM_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8)},  // TIM8_CH3

    {NC,    NC,    0}
};

static TIM_HandleTypeDef TimHandle;

void pwmout_init(pwmout_t* obj, PinName pin)
{
    // Get the peripheral name from the pin and assign it to the object
    obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);

    if (obj->pwm == (PWMName)NC) {
        error("PWM error: pinout mapping failed.");
    }

    // Enable TIM clock
    if (obj->pwm == PWM_1) __TIM1_CLK_ENABLE();
    if (obj->pwm == PWM_2) __TIM2_CLK_ENABLE();
    if (obj->pwm == PWM_3) __TIM3_CLK_ENABLE();
    if (obj->pwm == PWM_4) __TIM4_CLK_ENABLE();
    if (obj->pwm == PWM_9) __TIM9_CLK_ENABLE();
    if (obj->pwm == PWM_10) __TIM10_CLK_ENABLE();
    if (obj->pwm == PWM_11) __TIM11_CLK_ENABLE();

    // Configure GPIO
    pinmap_pinout(pin, PinMap_PWM);

    obj->pin = pin;
    obj->period = 0;
    obj->pulse = 0;

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
    int complementary_channel = 0;

    TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);

    if (value < (float)0.0) {
        value = 0.0;
    } else if (value > (float)1.0) {
        value = 1.0;
    }

    obj->pulse = (uint32_t)((float)obj->period * value);

    // Configure channels
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.Pulse        = obj->pulse;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    switch (obj->pin) {

        // Channels 1
        case PA_0:
        case PA_5:
        case PA_6:
        case PA_8:
        case PA_15:
        case PB_4:
        case PB_6:
        case PC_6:
            channel = TIM_CHANNEL_1;
            break;

        // Channels 1N
        case PA_7:
        case PB_13:
            channel = TIM_CHANNEL_1;
            complementary_channel = 1;
            break;

        // Channels 2
        case PA_1:
        case PA_9:
        case PB_3:
        case PB_5:
        case PB_7:
        case PC_7:
            channel = TIM_CHANNEL_2;
            break;

        // Channels 2N
        case PB_0:
        case PB_14:
            channel = TIM_CHANNEL_2;
            complementary_channel = 1;
            break;

        // Channels 3
        case PA_2:
        case PA_10:
        case PB_8:
        case PB_10:
        case PC_8:
            channel = TIM_CHANNEL_3;
            break;

        // Channels 3N
        case PB_1:
        case PB_15:
            channel = TIM_CHANNEL_3;
            complementary_channel = 1;
            break;

        // Channels 4
        case PA_3:
        case PA_11:
        case PB_9:
        case PC_9:
            channel = TIM_CHANNEL_4;
            break;

        default:
            return;
    }

    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel);
    if (complementary_channel) {
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

    float dc = pwmout_read(obj);

    __HAL_TIM_DISABLE(&TimHandle);

    // Update the SystemCoreClock variable
    SystemCoreClockUpdate();

    TimHandle.Init.Period        = us - 1;
    TimHandle.Init.Prescaler     = (uint16_t)(SystemCoreClock / 1000000) - 1; // 1 µs tick
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&TimHandle);

    // Set duty cycle again
    pwmout_write(obj, dc);

    // Save for future use
    obj->period = us;

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
