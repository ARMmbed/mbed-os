/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "cmsis.h"
#include "platform/mbed_wait_api.h"

// This implementation of the wait functions will be compiled only
// if the RTOS is not present.
#ifndef MBED_CONF_RTOS_PRESENT

#include "hal/us_ticker_api.h"

void wait(float s)
{
    wait_us(s * 1000000.0f);
}

void wait_ms(int ms)
{
    wait_us(ms * 1000);
}

void wait_us(int us)
{
    const ticker_data_t *const ticker = get_us_ticker_data();
    uint32_t start = ticker_read(ticker);
    while ((ticker_read(ticker) - start) < (uint32_t)us);
}

#endif // #ifndef MBED_CONF_RTOS_PRESENT

// This wait_ns is used by both RTOS and non-RTOS builds

#ifdef __CORTEX_M
#if (__CORTEX_M == 0 && !defined __CM0PLUS_REV) || __CORTEX_M == 1
// Cortex-M0 and Cortex-M1 take 7 cycles per iteration - SUBS = 1, 2xNOP = 2, BCS = 3
#define LOOP_SCALER 6000
#elif (__CORTEX_M == 0 && defined __CM0PLUS_REV) || __CORTEX_M == 3 || __CORTEX_M == 4 || \
      __CORTEX_M == 23 || __CORTEX_M == 33
// Cortex-M0+, M3, M4, M23 and M33 take 6 cycles per iteration - SUBS = 1, 3xNOP = 2, BCS = 2
// TODO - check M33
#define LOOP_SCALER 5000
#elif __CORTEX_M == 7
// Cortex-M7 manages to dual-issue for 2 cycles per iteration (SUB,NOP) = 1, (NOP,BCS) = 1
// (The NOPs were added to stabilise this - with just the SUB and BCS, it seems that the
// M7 sometimes takes 1 cycle, sometimes 2, possibly depending on alignment)
#define LOOP_SCALER 2000
#endif
#elif defined __CORTEX_A
#if __CORTEX_A == 9
// Cortex-A9 is dual-issue, so let's assume same performance as Cortex-M7.
// TODO - test.
#define LOOP_SCALER 2000
#endif
#endif

/* We only define the function if we've identified the CPU. If we haven't,
 * rather than a compile-time error, leave it undefined, rather than faulting
 * with an immediate #error. This leaves the door open to non-ARM
 * builds with or people providing substitutes for other CPUs, and only if
 * needed.
 */
#ifdef LOOP_SCALER

// *INDENT-OFF*
#ifdef __CC_ARM /* ARMC5 */
__asm static void delay_loop(uint32_t count)
{
1
  SUBS a1, a1, #1
  NOP
  NOP
  BCS  %BT1
  BX   lr
}
#elif defined (__ICCARM__)
static void delay_loop(uint32_t count)
{
  __asm volatile(
    "loop: \n"
    " SUBS %0, %0, #1 \n"
    " NOP\n"
    " NOP\n"
    " BCS.n  loop\n"
    : "+r" (count)
    :
    : "cc"
  );
}
#else // GCC or ARMC6
static void delay_loop(uint32_t count)
{
  __asm__ volatile (
    "%=:\n\t"
/* Only GCC insists on non-UAL assembly for Thumb v1 */
#if !defined(__ARMCC_VERSION) && defined(__thumb__) && !defined(__thumb2__)
    "SUB  %0, #1\n\t"
#else
    "SUBS %0, %0, #1\n\t"
#endif
    "NOP\n\t"
    "NOP\n\t"
    "BCS  %=b\n\t"
    : "+l" (count)
    :
    : "cc"
  );
}
#endif
// *INDENT-ON*

void wait_ns(unsigned int ns)
{
    uint32_t cycles_per_us = SystemCoreClock / 1000000;
    // Note that this very calculation, plus call overhead, will take multiple
    // cycles. Could well be 100ns on its own... So round down here, startup is
    // worth at least one loop iteration.
    uint32_t count = (cycles_per_us * ns) / LOOP_SCALER;

    delay_loop(count);
}
#endif // LOOP_SCALER
