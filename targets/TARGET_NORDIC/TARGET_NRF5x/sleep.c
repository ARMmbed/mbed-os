/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "cmsis.h"
#include "mbed_interface.h"
#include "mbed_nrf_sdk_adaptation.h"
#include "nrf_soc.h"
#include "nrf_timer.h"
#include "us_ticker.h"

#define FPU_EXCEPTION_MASK 0x0000009F

extern bool us_ticker_initialized;

void hal_sleep(void)
{
    // ensure debug is disconnected if semihost is enabled....

    // If the SoftDevice is enabled, its API must be used to go to sleep.
    if (mbed_softdevice_is_enabled()) {
        // SoftDevice documentation says this is required. This implies they
        // are using WFE, which sounds like bad news for us - we want WFI
        // semantics.
        SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

        // SoftDevice documentation suggests they are looking at pending IRQs
        // to decide when to return, without caring which we actually want
        // enabled. That would explain this logic, but the suggested implementation
        // below would not require this.
#if defined(NRF52)  || defined(NRF52840_XXAA)
        /* Clear exceptions and PendingIRQ from the FPU unit */
        __set_FPSCR(__get_FPSCR()  & ~(FPU_EXCEPTION_MASK));
        (void) __get_FPSCR();
        NVIC_ClearPendingIRQ(FPU_IRQn);
#endif

        // We can only hope that if they are using WFE, they have a reliable
        // mechanism. The "nosd" variants with just a lone WFE visible in some
        // directories are certainly not.
        //
        // I think two possible viable implementations are:
        //     static inline bool app_irq_pending(void)
        //     {
        //         return (NVIC->ICPR[0] & nrf_nvic_state.__irq_masks[0]) ||
        //                (NVIC->ICPR[1] & nrf_nvic_state.__irq_masks[1]);
        //     }
        //
        //     void sd_app_evt_wait(void)
        //     {
        //         // Function as WFI (only return when we have an IRQ pending)
        //         // or WFE (return when event flag set, maybe spuriously),
        //         // depending on cr_flag
        //         if (nrf_nvic_state.__cr_flag) {
        //             while (!app_irq_pending()) {
        //                 __DSB(); __WFE();  // relies on SEVONPEND
        //             }
        //         } else {
        //             __DSB(); __WFE();
        //         }
        //     }
        //
        // or, avoiding need for SEVONPEND:
        //
        //     void sd_app_evt_wait(void)
        //     {
        //         // Function as WFI or WFE, depending on cr_flag
        //         if (nrf_nvic_state.__cr_flag) {
        //             while (!app_irq_pending()) {
        //                 __disable_irq();
        //                 NVIC->ISER[0] = nrf_nvic_state.__irq_masks[0];
        //                 NVIC->ISER[1] = nrf_nvic_state.__irq_masks[1];
        //                 __DSB(); __WFI();
        //                 NVIC->ICER[0] = __NRF_NVIC_APP_IRQS_0;
        //                 NVIC->ICER[1] = __NRF_NVIC_APP_IRQS_1;
        //                 __enable_irq();
        //                 __ISB();
        //             }
        //         } else {
        //             __DSB(); __WFE();
        //         }
        //     }
        //
        sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
        sd_app_evt_wait();
    } else {
        NRF_POWER->TASKS_LOWPWR = 1;

        __DSB();
        __WFI();
    }
}

void hal_deepsleep(void)
{
    if (us_ticker_initialized) {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_STOP);
    }

    hal_sleep();

    if (us_ticker_initialized) {
        nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_START);
    }
    //   NRF_POWER->SYSTEMOFF=1;
}
