/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_PIN_DEVICE_H
#define MBED_PIN_DEVICE_H

#include "cmsis.h"
#include "stm32h7xx_ll_gpio.h"
#include "objects.h"

extern const uint32_t ll_pin_defines[16];

/* Family specific implementations */
static inline void stm_pin_DisconnectDebug(PinName pin)
{
    /* empty for now */
}

static inline void stm_pin_PullConfig(GPIO_TypeDef *gpio, uint32_t ll_pin, uint32_t pull_config)
{
#if defined(DUAL_CORE)
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_GPIO_SEMID)) {
    }
#endif /* DUAL_CORE */

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

#if defined(DUAL_CORE)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_GPIO_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */
}

static inline void stm_pin_SetAFPin(GPIO_TypeDef *gpio, PinName pin, uint32_t afnum)
{
    uint32_t ll_pin  = ll_pin_defines[STM_PIN(pin)];

#if defined(DUAL_CORE)
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_GPIO_SEMID)) {
    }
#endif /* DUAL_CORE */

    if (STM_PIN(pin) > 7) {
        LL_GPIO_SetAFPin_8_15(gpio, ll_pin, afnum);
    } else {
        LL_GPIO_SetAFPin_0_7(gpio, ll_pin, afnum);
    }

#if defined(DUAL_CORE)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_GPIO_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */
}

#endif
