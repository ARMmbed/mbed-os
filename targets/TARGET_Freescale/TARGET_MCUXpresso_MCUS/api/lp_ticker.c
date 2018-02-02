/* mbed Microcontroller Library
 * Copyright (c) 2016 - 2018 ARM Limited
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

#if DEVICE_LOWPOWERTIMER

#include "lp_ticker_api.h"
#include "fsl_rtc.h"
#include "fsl_lptmr.h"
#include "cmsis.h"
#include "rtc_api.h"

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        32768,        // 32kHz
           32         // 32 bit counter
    };
    return &info;
}

#define SEC_BITS (17)
#define SEC_SHIFT (15)
#define SEC_MASK ((1 << SEC_BITS) - 1)
#define TICKS_BITS (15)
#define TICKS_SHIFT (0)
#define TICKS_MASK ((1 << TICKS_BITS) - 1)

#define OSC32K_CLK_HZ (32768)
#define MAX_LPTMR_SLEEP ((1 << 16) - 1)

static bool lp_ticker_inited = false;
static timestamp_t lptmr_schedule = 0;

static void rtc_isr(void)
{
    uint32_t sr = RTC->SR;
    if (sr & RTC_SR_TOF_MASK) {
        /* Reset RTC to 0 so it keeps counting. */
        RTC_StopTimer(RTC);
        RTC->TSR = 0;
        RTC_StartTimer(RTC);
    } else if (sr & RTC_SR_TAF_MASK) {
        RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable);
        RTC->TAR = 0; /* Write clears the IRQ flag */

        /* Wait subsecond remainder. */
        const uint32_t now_ticks = lp_ticker_read();
        uint32_t delta_ticks =
                lptmr_schedule >= now_ticks ? lptmr_schedule - now_ticks : (uint32_t)((uint64_t) lptmr_schedule + 0xFFFFFFFF - now_ticks);

        lptmr_schedule = 0;

        if (delta_ticks == 0) {
            lp_ticker_irq_handler();
        } else {
            LPTMR_StopTimer(LPTMR0);
            LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
            LPTMR_SetTimerPeriod(LPTMR0, delta_ticks);
            LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
            LPTMR_StartTimer(LPTMR0);
        }

    } else if (sr & RTC_SR_TIF_MASK) {
        RTC_DisableInterrupts(RTC, kRTC_TimeOverflowInterruptEnable);
    }
}

static void lptmr_isr(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    LPTMR_StopTimer(LPTMR0);

    lp_ticker_irq_handler();
}

/** Initialize the low power ticker
 *
 */
void lp_ticker_init(void)
{
    lptmr_config_t lptmrConfig;

    if (!lp_ticker_inited) {

        /* Setup low resolution clock - RTC */
        if (!rtc_isenabled()) {
            rtc_init();
            RTC_StartTimer(RTC);
        }

        RTC->TAR = 0; /* Write clears the IRQ flag */
        NVIC_ClearPendingIRQ(RTC_IRQn);
        RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable | kRTC_SecondsInterruptEnable);
        NVIC_SetVector(RTC_IRQn, (uint32_t) rtc_isr);
        NVIC_EnableIRQ(RTC_IRQn);

        /* Setup high resolution clock - LPTMR */
        LPTMR_GetDefaultConfig(&lptmrConfig);

        /* Use 32kHz drive */
        CLOCK_SetXtal32Freq(OSC32K_CLK_HZ);
        lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_2;
        LPTMR_Init(LPTMR0, &lptmrConfig);
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        NVIC_ClearPendingIRQ(LPTMR0_IRQn);
        NVIC_SetVector(LPTMR0_IRQn, (uint32_t) lptmr_isr);
        EnableIRQ(LPTMR0_IRQn);

        lptmr_schedule = 0;

        lp_ticker_inited = true;
    } else {
        /* In case of re-init we need to disable lp ticker interrupt. */
        LPTMR_StopTimer(LPTMR0);

        RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable);
        RTC->TAR = 0; /* Write clears the IRQ flag */

        lptmr_schedule = 0;
    }
}

/** Read the current counter
 *
 * @return The current timer's counter value in ticks
 */
uint32_t lp_ticker_read(void)
{
    uint32_t count;
    uint32_t last_count;

    /* TPR is increments every 32.768 kHz clock cycle. The TSR increments when
     * bit 14 of the TPR transitions from a logic one (32768 ticks - 1 sec).
     * After that TPR starts counting from 0.
     *
     * count value is built as follows:
     * count[0 - 14] - ticks (RTC->TPR)
     * count[15 - 31] - seconds (RTC->TSR)
     */

    /* Loop until the same tick is read twice since this
     * is ripple counter on a different clock domain.
     */
    count = ((RTC->TSR << SEC_SHIFT) | (RTC->TPR & TICKS_MASK));
    do {
        last_count = count;
        count = ((RTC->TSR << SEC_SHIFT) | (RTC->TPR & TICKS_MASK));
    } while (last_count != count);

    return count;
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks to be set
 */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    lptmr_schedule = 0;

    /* We get here absolute interrupt time-stamp in ticks which takes into account counter overflow.
     * Since we use additional count-down timer to generate interrupt we need to calculate
     * load value based on time-stamp.
     */
    const uint32_t now_ticks = lp_ticker_read();
    uint32_t delta_ticks =
            timestamp >= now_ticks ? timestamp - now_ticks : (uint32_t)((uint64_t) timestamp + 0xFFFFFFFF - now_ticks);

    if (delta_ticks == 0) {
        /* The requested delay is less than the minimum resolution of this counter. */
        delta_ticks = 1;
    }

    if (delta_ticks > MAX_LPTMR_SLEEP) {
        /* Using RTC if wait time is over 16b (2s @32kHz). */
        uint32_t delay_sec = delta_ticks >> 15;

        RTC->TAR = RTC->TSR + delay_sec - 1;

        RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);

        /* Store absolute interrupt time-stamp value for further processing in
         * RTC interrupt handler (schedule remaining ticks using LPTMR). */
        lptmr_schedule = timestamp;
    } else {
        /* Below RTC resolution using LPTMR. */

        /* In case of re-schedule we need to disable RTC interrupt. */
        RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable);
        RTC->TAR = 0; /* Write clears the IRQ flag */

        /* When the LPTMR is enabled, the CMR can be altered only when CSR[TCF] is set. When
         * updating the CMR, the CMR must be written and CSR[TCF] must be cleared before the
         * LPTMR counter has incremented past the new LPTMR compare value.
         *
         * When TEN is clear, it resets the LPTMR internal logic, including the CNR and TCF.
         * When TEN is set, the LPTMR is enabled. While writing 1 to this field, CSR[5:1] must
         * not be altered.
         */
        LPTMR_StopTimer(LPTMR0);
        LPTMR_SetTimerPeriod(LPTMR0, delta_ticks);
        LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_StartTimer(LPTMR0);
    }
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(LPTMR0_IRQn);
}

/** Disable low power ticker interrupt
 *
 */
void lp_ticker_disable_interrupt(void)
{
    LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable);
}

/** Clear the low power ticker interrupt
 *
 */
void lp_ticker_clear_interrupt(void)
{
    RTC->TAR = 0; /* Write clears the IRQ flag */
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    lptmr_schedule = 0;
}

#endif /* DEVICE_LOWPOWERTIMER */
