/* Copyright (c) 2013, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of Nordic Semiconductor ASA nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
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
 */

/* NOTE: Template files (including this one) are application specific and therefore expected to
   be copied into the application project folder prior to its use! */

#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "system_nrf51.h"

/*lint ++flb "Enter library region" */

#define __SYSTEM_CLOCK      (16000000UL)     /*!< nRF51 devices use a fixed System Clock Frequency of 16MHz */

static bool is_manual_peripheral_setup_needed(void);
static bool is_disabled_in_debug_needed(void);
static void init_clock(void);


#if defined ( __CC_ARM )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;
#elif defined ( __ICCARM__ )
    __root uint32_t SystemCoreClock = __SYSTEM_CLOCK;
#elif defined   ( __GNUC__ )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;
#endif

void SystemCoreClockUpdate(void)
{
    SystemCoreClock = __SYSTEM_CLOCK;
}

void SystemInit(void)
{
#if defined(TARGET_NRF_32MHZ_XTAL)
    /* For 32MHz HFCLK XTAL such as Taiyo Yuden
       Physically, tiny footprint XTAL oscillate higher freq. To make BLE modules smaller, some modules
       are using 32MHz XTAL.
       This code wriging the value 0xFFFFFF00 to the UICR (User Information Configuration Register)
       at address 0x10001008, to make nRF51 works with 32MHz system clock. This register will be overwritten
       by SoftDevice to 0xFFFFFFFF, the default value. Each hex files built with mbed classic online compiler
       contain SoftDevice, so that, this code run once just after the hex file will be flashed onto nRF51.
       After changing the value, nRF51 need to reboot. */
    if (*(uint32_t *)0x10001008 == 0xFFFFFFFF)
    {
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
        *(uint32_t *)0x10001008 = 0xFFFFFF00;
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
        NVIC_SystemReset();
        while (true){}
    }
#endif

    /* If desired, switch off the unused RAM to lower consumption by the use of RAMON register.
       It can also be done in the application main() function. */

    /* Prepare the peripherals for use as indicated by the PAN 26 "System: Manual setup is required
       to enable the use of peripherals" found at Product Anomaly document for your device found at
       https://www.nordicsemi.com/. The side effect of executing these instructions in the devices
       that do not need it is that the new peripherals in the second generation devices (LPCOMP for
       example) will not be available. */
    if (is_manual_peripheral_setup_needed())
    {
        *(uint32_t volatile *)0x40000504 = 0xC007FFDF;
        *(uint32_t volatile *)0x40006C18 = 0x00008000;
    }

    /* Disable PROTENSET registers under debug, as indicated by PAN 59 "MPU: Reset value of DISABLEINDEBUG
       register is incorrect" found at Product Anomaly document four your device found at
       https://www.nordicsemi.com/. There is no side effect of using these instruction if not needed. */
    if (is_disabled_in_debug_needed())
    {
        NRF_MPU->DISABLEINDEBUG = MPU_DISABLEINDEBUG_DISABLEINDEBUG_Disabled << MPU_DISABLEINDEBUG_DISABLEINDEBUG_Pos;
    }

    // Start the external 32khz crystal oscillator.
    init_clock();
}

void init_clock(void)
{
    /* For compatibility purpose, the default behaviour is to first attempt to initialise an
       external clock, and after a timeout, use the internal RC one. To avoid this wait, boards that
       don't have an external oscillator can set TARGET_NRF_LFCLK_RC directly. */
    uint32_t i = 0;
    const uint32_t polling_period = 200;
    const uint32_t timeout = 1000000;

#if defined(TARGET_NRF_LFCLK_RC)
    NRF_CLOCK->LFCLKSRC             = (CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos);
#else
    NRF_CLOCK->LFCLKSRC             = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
#endif
    NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
    NRF_CLOCK->TASKS_LFCLKSTART     = 1;

    /* Wait for the external oscillator to start up.
       nRF51822 product specification (8.1.5) gives a typical value of 300ms for external clock
       startup duration, and a maximum value of 1s. When using the internal RC source, typical delay
       will be 390µs, so we use a polling period of 200µs.

       We can't use us_ticker at this point, so we have to rely on a less precise method for
       measuring our timeout. Because of this, the actual timeout will be slightly longer than 1
       second, which isn't an issue at all, since this fallback should only be used as a safety net.
       */
    for (i = 0; i < (timeout / polling_period); i++) {
        if (NRF_CLOCK->EVENTS_LFCLKSTARTED != 0)
            return;
        nrf_delay_us(polling_period);
    }

    /* Fallback to internal clock. Belt and braces, since the internal clock is used by default
       whilst no external source is running. This is not only a sanity check, but it also allows
       code down the road (e.g. ble initialisation) to directly know which clock is used. */
    NRF_CLOCK->LFCLKSRC         = (CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0) {
        // Do nothing.
    }
}

static bool is_manual_peripheral_setup_needed(void)
{
    if ((((*(uint32_t *)0xF0000FE0) & 0x000000FF) == 0x1) && (((*(uint32_t *)0xF0000FE4) & 0x0000000F) == 0x0))
    {
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x00) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x10) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x30) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
    }

    return false;
}

static bool is_disabled_in_debug_needed(void)
{
    if ((((*(uint32_t *)0xF0000FE0) & 0x000000FF) == 0x1) && (((*(uint32_t *)0xF0000FE4) & 0x0000000F) == 0x0))
    {
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x40) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
    }

    return false;
}

/*lint --flb "Leave library region" */
