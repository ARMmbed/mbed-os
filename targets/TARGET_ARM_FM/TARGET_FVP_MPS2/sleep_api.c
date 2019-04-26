
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
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


#include "sleep_api.h"
#include "us_ticker_api.h"

#include "PeripheralNames.h"
#define US_TICKER_COUNTER     CMSDK_DUALTIMER1
#define US_TICKER_INTERRUPT   CMSDK_DUALTIMER2
#define US_TICKER_TIMER_IRQn  DUALTIMER_IRQn


#if DEVICE_SLEEP

void hal_sleep(void)
{
    __WFI();
}

/* Since there is no power management function implemented in MPS2,
 * Also Deep Sleep mode mean to save power which is not practical on a simulator.
 * So mbed HAL Deep sleep is mocked by Sleep,
 * representing a "Waiting For Interrupt" state,
 * but disabling the Microsec ticker in addition */

void hal_deepsleep(void)
{
#if DEVICE_USTICKER
    uint32_t val = US_TICKER_COUNTER->TimerValue;
    US_TICKER_COUNTER->TimerControl &= ~CMSDK_DUALTIMER1_CTRL_EN_Msk;
    US_TICKER_INTERRUPT->TimerControl &= ~CMSDK_DUALTIMER2_CTRL_EN_Msk;
    US_TICKER_INTERRUPT->TimerControl &= ~CMSDK_DUALTIMER2_CTRL_INTEN_Msk;
#endif

    __WFI();

#if DEVICE_USTICKER
    US_TICKER_COUNTER->TimerLoad = val;
    US_TICKER_COUNTER->TimerControl |= CMSDK_DUALTIMER1_CTRL_EN_Msk;
    US_TICKER_INTERRUPT->TimerControl |= CMSDK_DUALTIMER1_CTRL_INTEN_Msk;
    US_TICKER_INTERRUPT->TimerControl |= CMSDK_DUALTIMER2_CTRL_EN_Msk;
#endif
}

#endif

/**@}*/
