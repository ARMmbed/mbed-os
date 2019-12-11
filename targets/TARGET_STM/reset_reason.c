/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "reset_reason_api.h"

#ifdef DEVICE_RESET_REASON

#include "device.h"

reset_reason_t hal_reset_reason_get(void)
{
#ifdef RCC_FLAG_LPWRRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)) {
        return RESET_REASON_WAKE_LOW_POWER;
    }
#endif

#ifdef RCC_FLAG_LPWR1RST
    if ((__HAL_RCC_GET_FLAG(RCC_FLAG_LPWR1RST)) || (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWR2RST))) {
        return RESET_REASON_WAKE_LOW_POWER;
    }
#endif

#ifdef RCC_FLAG_WWDGRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) {
        return RESET_REASON_WATCHDOG;
    }
#endif

#ifdef RCC_FLAG_WWDG1RST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDG1RST)) {
        return RESET_REASON_WATCHDOG;
    }
#endif

#ifdef RCC_FLAG_IWDGRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        return RESET_REASON_WATCHDOG;
    }
#endif

#ifdef RCC_FLAG_IWDG1RST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST)) {
        return RESET_REASON_WATCHDOG;
    }
#endif

#ifdef RCC_FLAG_SFTRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
        return RESET_REASON_SOFTWARE;
    }
#endif

#ifdef RCC_FLAG_PORRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
        return RESET_REASON_POWER_ON;
    }
#endif

#ifdef RCC_FLAG_BORRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST)) {
        return RESET_REASON_BROWN_OUT;
    }
#endif

#ifdef RCC_FLAG_PINRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
        return RESET_REASON_PIN_RESET;
    }
#endif

    return RESET_REASON_UNKNOWN;
}


uint32_t hal_reset_reason_get_raw(void)
{
#if TARGET_STM32H7
    return RCC->RSR;
#else /* TARGET_STM32H7 */
    return RCC->CSR;
#endif /* TARGET_STM32H7 */
}


void hal_reset_reason_clear(void)
{
    __HAL_RCC_CLEAR_RESET_FLAGS();
}

#endif // DEVICE_RESET_REASON
