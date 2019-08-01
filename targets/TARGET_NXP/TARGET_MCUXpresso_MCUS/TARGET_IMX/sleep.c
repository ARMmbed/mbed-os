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
#include "fsl_clock.h"

extern void vPortPRE_SLEEP_PROCESSING(clock_mode_t powermode);
extern void vPortPOST_SLEEP_PROCESSING(clock_mode_t powermode);
extern void serial_wait_tx_complete(uint32_t uart_index);


void hal_sleep(void)
{
    vPortPRE_SLEEP_PROCESSING(kCLOCK_ModeWait);

    __DSB();
    __WFI();
    __ISB();

    vPortPOST_SLEEP_PROCESSING(kCLOCK_ModeWait);
}

void hal_deepsleep(void)
{
    vPortPRE_SLEEP_PROCESSING(kCLOCK_ModeStop);

    /* Wait till debug UART is done transmitting */
    serial_wait_tx_complete(STDIO_UART);

    __DSB();
    __WFI();
    __ISB();

    vPortPOST_SLEEP_PROCESSING(kCLOCK_ModeStop);
}

