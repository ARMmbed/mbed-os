/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
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
#ifndef MBED_PIN_DEVICE_H
#define MBED_PIN_DEVICE_H

#include "cmsis.h"
#include "stm32f1xx_ll_gpio.h"

extern const uint32_t ll_pin_defines[16];

static inline void stm_pin_DisconnectDebug(PinName pin)
{
    // Enable AFIO clock
    __HAL_RCC_AFIO_CLK_ENABLE();

    // Disconnect JTAG-DP + SW-DP signals.
    // Warning: Need to reconnect under reset
    if ((pin == PA_13) || (pin == PA_14)) {
        __HAL_AFIO_REMAP_SWJ_DISABLE(); // JTAG-DP Disabled and SW-DP Disabled
    }
    if ((pin == PA_15) || (pin == PB_3) || (pin == PB_4)) {
        __HAL_AFIO_REMAP_SWJ_NOJTAG(); // JTAG-DP Disabled and SW-DP enabled
    }
}

/*  The AF selection of F1 family is specific compared to others */
static inline void stm_pin_SetAFPin(GPIO_TypeDef *gpio, PinName pin, uint32_t afnum)
{
    // Enable AFIO clock
    __HAL_RCC_AFIO_CLK_ENABLE();

    if (afnum > 0) {
        switch (afnum) {
        case 1: // Remap SPI1
            __HAL_AFIO_REMAP_SPI1_ENABLE();
            break;
        case 2: // Remap I2C1
            __HAL_AFIO_REMAP_I2C1_ENABLE();
            break;
        case 3: // Remap USART1
            __HAL_AFIO_REMAP_USART1_ENABLE();
            break;
        case 4: // Remap USART2
            __HAL_AFIO_REMAP_USART2_ENABLE();
            break;
        case 5: // Partial Remap USART3
            __HAL_AFIO_REMAP_USART3_PARTIAL();
            break;
        case 6: // Partial Remap TIM1
            __HAL_AFIO_REMAP_TIM1_PARTIAL();
            break;
        case 7: // Partial Remap TIM3
            __HAL_AFIO_REMAP_TIM3_PARTIAL();
            break;
        case 8: // Full Remap TIM2
            __HAL_AFIO_REMAP_TIM2_ENABLE();
            break;
        case 9: // Full Remap TIM3
            __HAL_AFIO_REMAP_TIM3_ENABLE();
            break;
#if defined(AFIO_MAPR_CAN_REMAP_REMAP1)
        case 10: // CAN_RX mapped to PB8, CAN_TX mapped to PB9
            __HAL_AFIO_REMAP_CAN1_2();
            break;
#endif
        default:
            break;
        }
    }
}

static inline void stm_pin_PullConfig(GPIO_TypeDef *gpio, uint32_t ll_pin, uint32_t pull_config)
{
    uint32_t function = LL_GPIO_GetPinMode(gpio, ll_pin);

    switch (pull_config) {
        case GPIO_PULLUP:
            if (function == LL_GPIO_MODE_FLOATING)
                LL_GPIO_SetPinMode(gpio, ll_pin, LL_GPIO_MODE_INPUT);
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_UP);
            break;
        case GPIO_PULLDOWN:
            if (function == LL_GPIO_MODE_FLOATING)
                LL_GPIO_SetPinMode(gpio, ll_pin, LL_GPIO_MODE_INPUT);
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_DOWN);
            break;
        default:
            /*  Input+NoPull = Floating for F1 family */
            if (function == LL_GPIO_MODE_INPUT)
                LL_GPIO_SetPinMode(gpio, ll_pin, LL_GPIO_MODE_FLOATING);
            break;
    }
}

#endif
