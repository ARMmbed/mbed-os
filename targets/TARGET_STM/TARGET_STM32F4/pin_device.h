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

// when LL is available, below include can be used
// #include "stm32f4xx_ll_gpio.h"
// until then let's define locally the required functions
#define LL_GPIO_PIN_0                      GPIO_BSRR_BS_0 /*!< Select pin 0 */
#define LL_GPIO_PIN_1                      GPIO_BSRR_BS_1 /*!< Select pin 1 */
#define LL_GPIO_PIN_2                      GPIO_BSRR_BS_2 /*!< Select pin 2 */
#define LL_GPIO_PIN_3                      GPIO_BSRR_BS_3 /*!< Select pin 3 */
#define LL_GPIO_PIN_4                      GPIO_BSRR_BS_4 /*!< Select pin 4 */
#define LL_GPIO_PIN_5                      GPIO_BSRR_BS_5 /*!< Select pin 5 */
#define LL_GPIO_PIN_6                      GPIO_BSRR_BS_6 /*!< Select pin 6 */
#define LL_GPIO_PIN_7                      GPIO_BSRR_BS_7 /*!< Select pin 7 */
#define LL_GPIO_PIN_8                      GPIO_BSRR_BS_8 /*!< Select pin 8 */
#define LL_GPIO_PIN_9                      GPIO_BSRR_BS_9 /*!< Select pin 9 */
#define LL_GPIO_PIN_10                     GPIO_BSRR_BS_10 /*!< Select pin 10 */
#define LL_GPIO_PIN_11                     GPIO_BSRR_BS_11 /*!< Select pin 11 */
#define LL_GPIO_PIN_12                     GPIO_BSRR_BS_12 /*!< Select pin 12 */
#define LL_GPIO_PIN_13                     GPIO_BSRR_BS_13 /*!< Select pin 13 */
#define LL_GPIO_PIN_14                     GPIO_BSRR_BS_14 /*!< Select pin 14 */
#define LL_GPIO_PIN_15                     GPIO_BSRR_BS_15 /*!< Select pin 15 */

#define LL_GPIO_MODE_INPUT                 ((uint32_t)0x00000000U) /*!< Select input mode */
#define LL_GPIO_MODE_OUTPUT                GPIO_MODER_MODER0_0  /*!< Select output mode */
#define LL_GPIO_MODE_ALTERNATE             GPIO_MODER_MODER0_1  /*!< Select alternate function mode */
#define LL_GPIO_MODE_ANALOG                GPIO_MODER_MODER0    /*!< Select analog mode */

#define LL_GPIO_OUTPUT_PUSHPULL            ((uint32_t)0x00000000U) /*!< Select push-pull as output type */
#define LL_GPIO_OUTPUT_OPENDRAIN           GPIO_OTYPER_OT_0 /*!< Select open-drain as output type */

#define LL_GPIO_PULL_NO                    ((uint32_t)0x00000000U) /*!< Select I/O no pull */
#define LL_GPIO_PULL_UP                    GPIO_PUPDR_PUPDR0_0 /*!< Select I/O pull up */
#define LL_GPIO_PULL_DOWN                  GPIO_PUPDR_PUPDR0_1 /*!< Select I/O pull down */

#define LL_GPIO_SPEED_FREQ_LOW             ((uint32_t)0x00000000U) /*!< Select I/O low output speed    */
#define LL_GPIO_SPEED_FREQ_MEDIUM          GPIO_OSPEEDER_OSPEEDR0_0 /*!< Select I/O medium output speed */
#define LL_GPIO_SPEED_FREQ_HIGH            GPIO_OSPEEDER_OSPEEDR0_1 /*!< Select I/O fast output speed   */
#define LL_GPIO_SPEED_FREQ_VERY_HIGH       GPIO_OSPEEDER_OSPEEDR0   /*!< Select I/O high output speed   */

__STATIC_INLINE void LL_GPIO_SetAFPin_0_7(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Alternate)
{
    MODIFY_REG(GPIOx->AFR[0], (0xFU << (POSITION_VAL(Pin) * 4U)),
               (Alternate << (POSITION_VAL(Pin) * 4U)));
}
__STATIC_INLINE void LL_GPIO_SetAFPin_8_15(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Alternate)
{
    MODIFY_REG(GPIOx->AFR[1], (0xFU << (POSITION_VAL(Pin >> 8U) * 4U)),
               (Alternate << (POSITION_VAL(Pin >> 8U) * 4U)));
}
__STATIC_INLINE void LL_GPIO_SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
    MODIFY_REG(GPIOx->MODER, (GPIO_MODER_MODER0 << (POSITION_VAL(Pin) * 2U)), (Mode << (POSITION_VAL(Pin) * 2U)));
}
__STATIC_INLINE uint32_t LL_GPIO_GetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    return (uint32_t)(READ_BIT(GPIOx->MODER, ((Pin * Pin) * GPIO_MODER_MODER0)) / (Pin * Pin));
}
__STATIC_INLINE void LL_GPIO_SetPinPull(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Pull)
{
    MODIFY_REG(GPIOx->PUPDR, (GPIO_PUPDR_PUPDR0 << (POSITION_VAL(Pin) * 2U)), (Pull << (POSITION_VAL(Pin) * 2U)));
}
__STATIC_INLINE void LL_GPIO_SetPinOutputType(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint32_t OutputType)
{
    MODIFY_REG(GPIOx->OTYPER, PinMask, (PinMask * OutputType));
}
__STATIC_INLINE void LL_GPIO_SetPinSpeed(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t  Speed)
{
    MODIFY_REG(GPIOx->OSPEEDR, (GPIO_OSPEEDER_OSPEEDR0 << (POSITION_VAL(Pin) * 2U)),
               (Speed << (POSITION_VAL(Pin) * 2U)));
}
// Above lines shall be defined in LL when available

extern const uint32_t ll_pin_defines[16];

/* Family specific implementations */
static inline void stm_pin_DisconnectDebug(PinName pin)
{
    /* empty for now */
}

static inline void stm_pin_PullConfig(GPIO_TypeDef *gpio, uint32_t ll_pin, uint32_t pull_config)
{
    switch (pull_config) {
        case GPIO_PULLUP:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_UP);
            break;
        case GPIO_PULLDOWN:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_DOWN);
            break;
        default:
            LL_GPIO_SetPinPull(gpio, ll_pin, LL_GPIO_PULL_NO);
            break;
    }
}

static inline void stm_pin_SetAFPin(GPIO_TypeDef *gpio, PinName pin, uint32_t afnum)
{
    uint32_t ll_pin  = ll_pin_defines[STM_PIN(pin)];

    if (STM_PIN(pin) > 7) {
        LL_GPIO_SetAFPin_8_15(gpio, ll_pin, afnum);
    } else {
        LL_GPIO_SetAFPin_0_7(gpio, ll_pin, afnum);
    }
}

#endif
