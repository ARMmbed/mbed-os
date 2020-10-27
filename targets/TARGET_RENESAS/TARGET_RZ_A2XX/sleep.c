/* mbed Microcontroller Library
 * Copyright (c) 2018-2020 ARM Limited
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
#include "cmsis.h"
#include "mbed_interface.h"
#include "mbed_critical.h"
#include "iodefine.h"

#if DEVICE_SLEEP
static volatile uint8_t wk_CPGSTBCR3;
static volatile uint8_t wk_CPGSTBCR4;
static volatile uint8_t wk_CPGSTBCR5;
static volatile uint8_t wk_CPGSTBCR6;
static volatile uint8_t wk_CPGSTBCR7;
static volatile uint8_t wk_CPGSTBCR8;
static volatile uint8_t wk_CPGSTBCR9;
static volatile uint8_t wk_CPGSTBCR10;

typedef struct {
    volatile uint8_t *p_wk_stbcr;
    volatile uint8_t *p_stbcr;
    volatile uint8_t *p_stbreq;
    volatile uint8_t *p_stback;
    uint8_t mstp;
    uint8_t stbrq;
} module_stanby_t;

static const module_stanby_t module_stanby[] = {
    {&wk_CPGSTBCR5,  &CPG.STBCR5.BYTE,  &CPG.STBREQ1.BYTE, &CPG.STBACK1.BYTE, 0x40,  0x01}, /* CEU */
    {&wk_CPGSTBCR5,  &CPG.STBCR5.BYTE,  &CPG.STBREQ1.BYTE, &CPG.STBACK1.BYTE, 0x02,  0x08}, /* JCU */
    {&wk_CPGSTBCR6,  &CPG.STBCR6.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x40,  0x80}, /* VIN */
    {&wk_CPGSTBCR6,  &CPG.STBCR6.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x20,  0x40}, /* ethernet0 */
    {&wk_CPGSTBCR6,  &CPG.STBCR6.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x10,  0x40}, /* ethernet1 */
    {&wk_CPGSTBCR6,  &CPG.STBCR6.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x08,  0x40}, /* ethernet PTP */
    {&wk_CPGSTBCR6,  &CPG.STBCR6.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x04,  0x40}, /* ethernet M */
    {&wk_CPGSTBCR6,  &CPG.STBCR6.BYTE,  &CPG.STBREQ3.BYTE, &CPG.STBACK3.BYTE, 0x02,  0x03}, /* USB host 0 */
    {&wk_CPGSTBCR6,  &CPG.STBCR6.BYTE,  &CPG.STBREQ3.BYTE, &CPG.STBACK3.BYTE, 0x01,  0x0C}, /* USB host 1 */
    {&wk_CPGSTBCR7,  &CPG.STBCR7.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x80,  0x08}, /* IMR */
    {&wk_CPGSTBCR7,  &CPG.STBCR7.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x40,  0x03}, /* DAVE-2D */
    {&wk_CPGSTBCR8,  &CPG.STBCR8.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x02,  0x20}, /* VDC-6 */
    {&wk_CPGSTBCR9,  &CPG.STBCR9.BYTE,  &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x01,  0x10}, /* DRP */
    {&wk_CPGSTBCR10, &CPG.STBCR10.BYTE, &CPG.STBREQ2.BYTE, &CPG.STBACK2.BYTE, 0x10,  0x04}, /* NAND */
    {&wk_CPGSTBCR10, &CPG.STBCR10.BYTE, &CPG.STBREQ3.BYTE, &CPG.STBACK3.BYTE, 0x08,  0x04}, /* SD host 0 */
    {&wk_CPGSTBCR10, &CPG.STBCR10.BYTE, &CPG.STBREQ3.BYTE, &CPG.STBACK3.BYTE, 0x02,  0x02}, /* SD host 1 */
    {0, 0, 0, 0, 0}  /* None */
};

/* Channel array defines of SCIF */
/*(Sample) value = SCIF[ channel ]->SCSMR; */
#define SCIFA_COUNT  (5)
#define SCIFA_ADDRESS_LIST \
{   /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */ \
    &SCIFA0, &SCIFA1, &SCIFA2, &SCIFA3, &SCIFA4 \
}   /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */ /* { } is for MISRA 19.4 */

/* End of channel array defines of SCIF */

static void module_standby_in(void)
{
    volatile uint32_t cnt;
    volatile uint8_t dummy_8;
    const module_stanby_t *p_module = &module_stanby[0];

    while (p_module->p_wk_stbcr != 0) {
        if ((*p_module->p_wk_stbcr & p_module->mstp) == 0) {
            *p_module->p_stbreq |= p_module->stbrq;
            dummy_8             = *p_module->p_stbreq;
            for (cnt = 0; cnt < 1000; cnt++) {      // wait time
                if ((*p_module->p_stback & p_module->stbrq) == p_module->stbrq) {
                    break;
                }
            }
            *p_module->p_stbcr |= p_module->mstp;
            dummy_8            = *p_module->p_stbcr;
        }
        p_module++;
    }
    (void)dummy_8;
}

static void module_standby_out(void)
{
    volatile uint32_t cnt;
    volatile uint8_t dummy_8;
    const module_stanby_t *p_module = &module_stanby[0];

    while (p_module->p_wk_stbcr != 0) {
        if ((*p_module->p_wk_stbcr & p_module->mstp) == 0) {
            *p_module->p_stbreq &= ~(p_module->stbrq);
            dummy_8             = *p_module->p_stbreq;
            for (cnt = 0; cnt < 1000; cnt++) {
                if ((*p_module->p_stback & p_module->stbrq) == 0) {
                    break;
                }
            }
        }
        p_module++;
    }
    (void)dummy_8;
}

void hal_sleep(void)
{
    // Transition to Sleep Mode
    __WFI();
}

void hal_deepsleep(void)
{
    volatile uint8_t dummy_8;

    /* Waits for the serial transmission to complete */
    volatile const struct st_scifa *SCIFA[SCIFA_COUNT] = SCIFA_ADDRESS_LIST;

    for (int uart = 0; uart < SCIFA_COUNT; uart++) {
        /* Is the power turned on? */
        if ((wk_CPGSTBCR4 & (1 << (7 - uart))) == 0) {
            /* Is transmission enabled? (TE = 1, TIE = 1) */
            if ((SCIFA[uart]->SCR.WORD & 0x00A0) == 0x00A0) {
                /* Waits for the transmission to complete (TEND = 1) */
                while ((SCIFA[uart]->FSR.WORD & 0x0040) == 0);
            }
        }
    }

    core_util_critical_section_enter();
    /* For powerdown the peripheral module, save current standby control register values(just in case) */
    wk_CPGSTBCR3  = CPG.STBCR3.BYTE;
    wk_CPGSTBCR4  = CPG.STBCR4.BYTE;
    wk_CPGSTBCR5  = CPG.STBCR5.BYTE;
    wk_CPGSTBCR6  = CPG.STBCR6.BYTE;
    wk_CPGSTBCR7  = CPG.STBCR7.BYTE;
    wk_CPGSTBCR8  = CPG.STBCR8.BYTE;
    wk_CPGSTBCR9  = CPG.STBCR9.BYTE;
    wk_CPGSTBCR10 = CPG.STBCR10.BYTE;

    /* non */
    CPG.STBCR3.BYTE  = 0xFF;
    dummy_8 = CPG.STBCR3.BYTE;
    /* non */
    CPG.STBCR4.BYTE  = 0xFF;
    dummy_8 = CPG.STBCR4.BYTE;
    /* Realtime Clock, CEU, JCU */
    CPG.STBCR5.BYTE  |= ~(0x4A);
    dummy_8 = CPG.STBCR5.BYTE;
    /* VIN, ethernet0, ethernet1, ethernet PTP, ethernet M, USB host 0, USB host 1 */
    CPG.STBCR6.BYTE  |= ~(0x7F);
    dummy_8 = CPG.STBCR6.BYTE;
    /* IMR, DAVE-2D */
    CPG.STBCR7.BYTE  |= ~(0xC0);
    dummy_8 = CPG.STBCR7.BYTE;
    /* VDC-6 */
    CPG.STBCR8.BYTE  |= ~(0x02);
    dummy_8 = CPG.STBCR8.BYTE;
    /* DRP */
    CPG.STBCR9.BYTE  |= ~(0x01);
    dummy_8 = CPG.STBCR9.BYTE;
    /* NAND, SD host 0, SD host 1 */
    CPG.STBCR10.BYTE |= ~(0x1A);
    dummy_8 = CPG.STBCR10.BYTE;

    module_standby_in();

    // Transition to Sleep Mode
    __WFI();

    /* Revert standby control register values */
    CPG.STBCR3.BYTE  = wk_CPGSTBCR3;
    dummy_8 = CPG.STBCR3.BYTE;
    CPG.STBCR4.BYTE  = wk_CPGSTBCR4;
    dummy_8 = CPG.STBCR4.BYTE;
    CPG.STBCR5.BYTE  = wk_CPGSTBCR5;
    dummy_8 = CPG.STBCR5.BYTE;
    CPG.STBCR6.BYTE  = wk_CPGSTBCR6;
    dummy_8 = CPG.STBCR6.BYTE;
    CPG.STBCR7.BYTE  = wk_CPGSTBCR7;
    dummy_8 = CPG.STBCR7.BYTE;
    CPG.STBCR8.BYTE  = wk_CPGSTBCR8;
    dummy_8 = CPG.STBCR8.BYTE;
    CPG.STBCR9.BYTE  = wk_CPGSTBCR9;
    dummy_8 = CPG.STBCR9.BYTE;
    CPG.STBCR10.BYTE = wk_CPGSTBCR10;
    dummy_8 = CPG.STBCR10.BYTE;

    module_standby_out();
    core_util_critical_section_exit();

    (void)dummy_8;
}
#endif
