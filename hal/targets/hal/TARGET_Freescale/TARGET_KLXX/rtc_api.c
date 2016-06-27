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
#include "rtc_api.h"
#include "PeripheralPins.h"
#include "clk_freqs.h"

static void init(void) {
    // enable RTC clock
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

    // select RTC clock source
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
    
    // Enable external crystal source if clock source is 32KHz
    if (extosc_frequency()==32768) {
        SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(OSC32KCLK);
    }
    else{
        // If main clock is NOT 32KHz crystal, use external 32KHz clock source defined in PeripheralPins.c
        SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(PinMap_RTC[0].peripheral);
        pinmap_pinout(PinMap_RTC[0].pin, PinMap_RTC);        //Map RTC clk input (if not NC)
    }
}

void rtc_init(void) {
    init();

    // Configure the TSR. default value: 1
    RTC->TSR = 1;

    // Configure Time Compensation Register to calibrate RTC accuracy

    // dissable LRL lock
    RTC->LR &= ~RTC_LR_LRL_MASK;
    // RTC->TCR: RTC_TCR_CIR_MASK,RTC_TCR_CIR(x)=0,RTC_TCR_TCR(x)=0  Default no correction
    RTC->TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);
    /*
        RTC_TCR_CIR(x) sets the compensation interval in seconds from 1 to 256.
        0x05 will apply the compensation once every 4 seconds.

        RTC_TCR_TCR(x) sets the Register Overflow
        0x80 Time Prescaler Register overflows every 32896 clock cycles. (+128)
        ... ... RTC runs slower
        0xFF Time Prescaler Register overflows every 32769 clock cycles.
        0x00 Time Prescaler Register overflows every 32768 clock cycles, Default.
        0x01 Time Prescaler Register overflows every 32767 clock cycles.
        ... ... RTC runs faster
        0x7F Time Prescaler Register overflows every 32641 clock cycles. (-128)
    */
    // enable TCL lock
    RTC->LR |= RTC_LR_TCL_MASK;
    // enable LRL lock
    RTC->LR |= RTC_LR_LRL_MASK;

    // enable counter
    RTC->SR |= RTC_SR_TCE_MASK;
}

void rtc_free(void) {
    // [TODO]
}

/*
 * Little check routine to see if the RTC has been enabled
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void) {
    // even if the RTC module is enabled,
    // as we use RTC_CLKIN and an external clock,
    // we need to reconfigure the pins. That is why we
    // call init() if the rtc is enabled

    // if RTC not enabled return 0
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
    if ((RTC->SR & RTC_SR_TCE_MASK) == 0)
        return 0;

    init();
    return 1;
}

time_t rtc_read(void) {
    return RTC->TSR;
}

void rtc_write(time_t t) {
    // disable counter
    RTC->SR &= ~RTC_SR_TCE_MASK;

    // we do not write 0 into TSR
    // to avoid invalid time
    if (t == 0)
        t = 1;

    // write seconds
    RTC->TSR = t;

    // re-enable counter
    RTC->SR |= RTC_SR_TCE_MASK;
}
