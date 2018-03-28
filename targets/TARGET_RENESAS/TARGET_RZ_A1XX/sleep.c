/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
static volatile uint8_t wk_CPGSTBCR11;
static volatile uint8_t wk_CPGSTBCR12;
#if defined(TARGET_RZA1H)  || defined(TARGET_VK_RZ_A1H)
static volatile uint8_t wk_CPGSTBCR13;
#endif
static volatile uint8_t wk_CPGSTBREQ1;
static volatile uint8_t wk_CPGSTBREQ2;

typedef struct {
    volatile uint8_t * p_wk_stbcr;
    volatile uint8_t * p_stbcr;
    volatile uint8_t * p_stbreq;
    volatile uint8_t * p_stback;
    uint8_t mstp;
    uint8_t stbrq;
} module_stanby_t;

static const module_stanby_t module_stanby[] = {
    {&wk_CPGSTBCR6,  &CPGSTBCR6,  &CPGSTBREQ1, &CPGSTBACK1, CPG_STBCR6_BIT_MSTP61,   CPG_STBREQ1_BIT_STBRQ13}, /* JCU */
    {&wk_CPGSTBCR6,  &CPGSTBCR6,  &CPGSTBREQ1, &CPGSTBACK1, CPG_STBCR6_BIT_MSTP66,   CPG_STBREQ1_BIT_STBRQ10}, /* CEU */
    {&wk_CPGSTBCR6,  &CPGSTBCR8,  &CPGSTBREQ1, &CPGSTBACK1, CPG_STBCR8_BIT_MSTP82,   CPG_STBREQ1_BIT_STBRQ12}, /* EthernetAVB */
    {&wk_CPGSTBCR7,  &CPGSTBCR7,  &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR7_BIT_MSTP74,   CPG_STBREQ2_BIT_STBRQ26}, /* Ethernet */
    {&wk_CPGSTBCR8,  &CPGSTBCR8,  &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR8_BIT_MSTP83,   CPG_STBREQ2_BIT_STBRQ27}, /* MediaLB */
    {&wk_CPGSTBCR9,  &CPGSTBCR9,  &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR9_BIT_MSTP91,   CPG_STBREQ2_BIT_STBRQ25}, /* VDC5_0 */
#if defined(TARGET_RZA1H)  || defined(TARGET_VK_RZ_A1H)
    {&wk_CPGSTBCR8,  &CPGSTBCR8,  &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR8_BIT_MSTP85,   CPG_STBREQ2_BIT_STBRQ21}, /* IMR-LSD */
    {&wk_CPGSTBCR8,  &CPGSTBCR8,  &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR8_BIT_MSTP86,   CPG_STBREQ2_BIT_STBRQ22}, /* IMR-LS2_1 */
    {&wk_CPGSTBCR8,  &CPGSTBCR8,  &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR8_BIT_MSTP87,   CPG_STBREQ2_BIT_STBRQ23}, /* IMR-LS2_0 */
    {&wk_CPGSTBCR9,  &CPGSTBCR9,  &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR9_BIT_MSTP90,   CPG_STBREQ2_BIT_STBRQ24}, /* VDC5_1 */
    {&wk_CPGSTBCR10, &CPGSTBCR10, &CPGSTBREQ2, &CPGSTBACK2, CPG_STBCR10_BIT_MSTP100, CPG_STBREQ2_BIT_STBRQ20}, /* OpenVG */
#endif
    {0, 0, 0, 0, 0}  /* None */
};

static void module_standby_in(void) {
    volatile uint32_t cnt;
    volatile uint8_t dummy_8;
    const module_stanby_t * p_module = &module_stanby[0];

    while (p_module->p_wk_stbcr != 0) {
        if ((*p_module->p_wk_stbcr & p_module->mstp) == 0) {
            *p_module->p_stbreq |= p_module->stbrq;
            dummy_8             = *p_module->p_stbreq;
            for (cnt = 0; cnt < 1000; cnt++) {      // wait time
                if ((*p_module->p_stback & p_module->stbrq) != 0) {
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

static void module_standby_out(void) {
    volatile uint32_t cnt;
    volatile uint8_t dummy_8;
    const module_stanby_t * p_module = &module_stanby[0];

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

void hal_sleep(void) {
    // Transition to Sleep Mode
    __WFI();
}

void hal_deepsleep(void) {
    volatile uint8_t dummy_8;

    core_util_critical_section_enter();
    /* For powerdown the peripheral module, save current standby control register values(just in case) */
    wk_CPGSTBCR3  = CPGSTBCR3;
    wk_CPGSTBCR4  = CPGSTBCR4;
    wk_CPGSTBCR5  = CPGSTBCR5;
    wk_CPGSTBCR6  = CPGSTBCR6;
    wk_CPGSTBCR7  = CPGSTBCR7;
    wk_CPGSTBCR8  = CPGSTBCR8;
    wk_CPGSTBCR9  = CPGSTBCR9;
    wk_CPGSTBCR10 = CPGSTBCR10;
    wk_CPGSTBCR11 = CPGSTBCR11;
    wk_CPGSTBCR12 = CPGSTBCR12;
#if defined(TARGET_RZA1H)  || defined(TARGET_VK_RZ_A1H)
    wk_CPGSTBCR13 = CPGSTBCR13;
#endif

    /* MTU2 (for low power ticker) */
    CPGSTBCR3  |= ~(CPG_STBCR3_BIT_MSTP33);
    dummy_8    = CPGSTBCR3;
    CPGSTBCR4  = 0xFF;
    dummy_8    = CPGSTBCR4;
    CPGSTBCR5  = 0xFF;
    dummy_8    = CPGSTBCR5;
    /* Realtime Clock, JCU, CEU */
    CPGSTBCR6  |= ~(CPG_STBCR6_BIT_MSTP60 | CPG_STBCR6_BIT_MSTP61 | CPG_STBCR6_BIT_MSTP66);
    dummy_8    = CPGSTBCR6;
    /* Video Decoder0, Video Decoder1, Ethernet */
    CPGSTBCR7  |= ~(CPG_STBCR7_BIT_MSTP77 | CPG_STBCR7_BIT_MSTP76 | CPG_STBCR7_BIT_MSTP74);
    dummy_8    = CPGSTBCR7;
    /* EthernetAVB, MediaLB, IMR-LSD, IMR-LS2_1, IMR-LS2_0 */
    CPGSTBCR8  |= ~(CPG_STBCR8_BIT_MSTP82 | CPG_STBCR8_BIT_MSTP83 | CPG_STBCR8_BIT_MSTP85 | CPG_STBCR8_BIT_MSTP86 | CPG_STBCR8_BIT_MSTP87);
    dummy_8    = CPGSTBCR8;
    /* VDC5_1, SPI Multi I/O Bus Controller0 */
    CPGSTBCR9  |= ~(CPG_STBCR9_BIT_MSTP90 | CPG_STBCR9_BIT_MSTP93);
    dummy_8    = CPGSTBCR9;
    /* OpenVG */
    CPGSTBCR10 |= ~(CPG_STBCR10_BIT_MSTP100);
    dummy_8    = CPGSTBCR10;
    CPGSTBCR11 = 0xFF;
    dummy_8    = CPGSTBCR11;
    CPGSTBCR12 = 0xFF;
    dummy_8    = CPGSTBCR12;
#if defined(TARGET_RZA1H)  || defined(TARGET_VK_RZ_A1H)
    CPGSTBCR13 = 0xFF;
    dummy_8    = CPGSTBCR13;
#endif

    module_standby_in();

    // Transition to Sleep Mode
    __WFI();

    /* Revert standby control register values */
    CPGSTBCR3  = wk_CPGSTBCR3;
    dummy_8    = CPGSTBCR3;
    CPGSTBCR4  = wk_CPGSTBCR4;
    dummy_8    = CPGSTBCR4;
    CPGSTBCR5  = wk_CPGSTBCR5;
    dummy_8    = CPGSTBCR5;
    CPGSTBCR6  = wk_CPGSTBCR6;
    dummy_8    = CPGSTBCR6;
    CPGSTBCR7  = wk_CPGSTBCR7;
    dummy_8    = CPGSTBCR7;
    CPGSTBCR8  = wk_CPGSTBCR8;
    dummy_8    = CPGSTBCR8;
    CPGSTBCR9  = wk_CPGSTBCR9;
    dummy_8    = CPGSTBCR9;
    CPGSTBCR10 = wk_CPGSTBCR10;
    dummy_8    = CPGSTBCR10;
    CPGSTBCR11 = wk_CPGSTBCR11;
    dummy_8    = CPGSTBCR11;
    CPGSTBCR12 = wk_CPGSTBCR12;
    dummy_8    = CPGSTBCR12;
#if defined(TARGET_RZA1H)  || defined(TARGET_VK_RZ_A1H)
    CPGSTBCR13 = wk_CPGSTBCR13;
    dummy_8    = CPGSTBCR13;
#endif

    module_standby_out();
    core_util_critical_section_exit();

    (void)dummy_8;
}
#endif
