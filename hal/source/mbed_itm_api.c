/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if DEVICE_ITM

#include "hal/itm_api.h"
#include "cmsis.h"

#include <stdbool.h>

#ifndef ITM_STIM_FIFOREADY_Msk
#define ITM_STIM_FIFOREADY_Msk 1
#endif

#define ITM_ENABLE_WRITE 0xC5ACCE55

#define SWO_NRZ 0x02
#define SWO_STIMULUS_PORT 0x01

void mbed_itm_init(void)
{
    static bool do_init = true;

    if (do_init) {
        do_init = false;

        itm_init();

        /* Enable write access to ITM registers. */
        ITM->LAR  = ITM_ENABLE_WRITE;

        /* Trace Port Interface Selected Pin Protocol Register. */
        TPI->SPPR = (SWO_NRZ << TPI_SPPR_TXMODE_Pos);

        /* Trace Port Interface Formatter and Flush Control Register */
        TPI->FFCR = (1 << TPI_FFCR_TrigIn_Pos);

        /* Data Watchpoint and Trace Control Register */
        DWT->CTRL = (1 << DWT_CTRL_CYCTAP_Pos)       |
                    (0xF << DWT_CTRL_POSTINIT_Pos)   |
                    (0xF << DWT_CTRL_POSTPRESET_Pos) |
                    (1 << DWT_CTRL_CYCCNTENA_Pos);

        /* Trace Privilege Register.
         * Disable access to trace channel configuration from non-privileged mode.
         */
        ITM->TPR  = 0x0;

        /* Trace Control Register */
        ITM->TCR  = (1 << ITM_TCR_TraceBusID_Pos) |
                    (1 << ITM_TCR_DWTENA_Pos)     |
                    (1 << ITM_TCR_SYNCENA_Pos)    |
                    (1 << ITM_TCR_ITMENA_Pos);

        /* Trace Enable Register */
        ITM->TER = SWO_STIMULUS_PORT;
    }
}

static void itm_out8(uint32_t port, uint8_t data)
{
    /* Wait until port is available */
    while ((ITM->PORT[port].u32 & ITM_STIM_FIFOREADY_Msk) == 0) {
        __NOP();
    }

    /* write data to port */
    ITM->PORT[port].u8 = data;
}

static void itm_out32(uint32_t port, uint32_t data)
{
    /* Wait until port is available */
    while ((ITM->PORT[port].u32 & ITM_STIM_FIFOREADY_Msk) == 0) {
        __NOP();
    }

    /* write data to port */
    ITM->PORT[port].u32 = data;
}

uint32_t mbed_itm_send(uint32_t port, uint32_t data)
{
    /* Check if ITM and port is enabled */
    if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) &&      /* ITM enabled */
            ((ITM->TER & (1UL << port)) != 0UL)) {       /* ITM Port enabled */
        itm_out32(port, data);
    }

    return data;
}

void mbed_itm_send_block(uint32_t port, const void *data, size_t len)
{
    const char *ptr = data;

    /* Check if ITM and port is enabled */
    if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) &&      /* ITM enabled */
            ((ITM->TER & (1UL << port)) != 0UL)) {       /* ITM Port enabled */
        /* Output single byte at a time until data is aligned */
        while ((((uintptr_t) ptr) & 3) && len != 0) {
            itm_out8(port, *ptr++);
            len--;
        }

        /* Output bulk of data one word at a time */
        while (len >= 4) {
            itm_out32(port, *(const uint32_t *) ptr);
            ptr += 4;
            len -= 4;
        }

        /* Output any trailing bytes */
        while (len != 0) {
            itm_out8(port, *ptr++);
            len--;
        }
    }
}
#endif // DEVICE_ITM
