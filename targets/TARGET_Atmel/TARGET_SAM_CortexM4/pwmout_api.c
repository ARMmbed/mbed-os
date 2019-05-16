/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "mbed_assert.h"
#include "pwmout_api.h"

#include "cmsis.h"
#include "tc.h"
#include "sysclk.h"
#include "PeripheralPins.h"

extern uint8_t g_sys_init;

/** Use TC Peripheral 0 **/
#define TC             TC0

static const uint32_t tc_prescalar[] = {
    TC_CMR_TCCLKS_TIMER_CLOCK1,  // MCK/2
    TC_CMR_TCCLKS_TIMER_CLOCK2,  // MCK/8
    TC_CMR_TCCLKS_TIMER_CLOCK3,  // MCK/32
    TC_CMR_TCCLKS_TIMER_CLOCK4,  // MCK/128
};
static const uint32_t tc_prescalar_divider[] = {
    2,  // MCK/2
    8,  // MCK/8
    32, // MCK/32
    128 // MCK/128
};

uint32_t getpwmchannelid (uint32_t channel)
{
    switch (channel) {
        case 0 :
            return ID_TC0;
        case 1 :
            return ID_TC1;
        case 2 :
            return ID_TC2;
        default :
            MBED_ASSERT(false);
            break;
    }
}

uint32_t getprescalarindex (uint16_t frequency)
{
    float time_period_ms;
    time_period_ms = (1.0 / (float)frequency) * 1000.0;
    if (time_period_ms <= 1.0) {
        return 0;
    } else if ((time_period_ms > 1.0) && (time_period_ms <= 4.0)) {
        return 1;
    } else if ((time_period_ms > 4.0) && (time_period_ms <= 16.0)) {
        return 2;
    } else {
        return 3;
    }
}

static void setregisterabc (pwmout_t* obj)
{
    uint32_t ra, rb, rc;
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Configure waveform frequency and duty cycle. */
    rc = (sysclk_get_peripheral_bus_hz(TC) /
          tc_prescalar_divider[obj->prescalarindex] )/
         obj->waveconfig.us_frequency;
    tc_write_rc(TC, obj->channel, rc);
    switch (obj->ioline) {
        case 0 :
            ra = (100 - obj->waveconfig.us_dutycycle) * rc / 100;
            if(ra <= 0) ra = 1; /*non zero value only*/
            tc_write_ra(TC, obj->channel, ra);
            break;
        case 1 :
            rb = (100 - obj->waveconfig.us_dutycycle) * rc / 100;
            if(rb <= 0) rb = 1; /*non zero value only*/
            tc_write_rb(TC, obj->channel, rb);
            break;
        default :
            MBED_ASSERT(false);
            break;
    }
}

void pwmout_inithw(pwmout_t* obj)
{
    uint32_t mode = 0;
    /* Configure the PMC to enable the TC module. */
    sysclk_enable_peripheral_clock(getpwmchannelid(obj->channel));
#if SAMG55
    /* Enable PCK output */
    pmc_disable_pck(PMC_PCK_3);
    pmc_switch_pck_to_mck(PMC_PCK_3, PMC_PCK_PRES_CLK_1);
    pmc_enable_pck(PMC_PCK_3);
#endif
    switch (obj->ioline) {
        case 0 :
            mode = TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR; /* RA Compare Effect: set */ /* RC Compare Effect: clear */
            break;
        case 1 :
            mode = TC_CMR_BCPB_SET | TC_CMR_BCPC_CLEAR | TC_CMR_ABETRG; /* RB Compare Effect: set */ /* RC Compare Effect: clear */ /*Change external event selection from TIOB*/
            break;
        default :
            MBED_ASSERT(false);
            break;
    }
    /* Disable TC TC_CHANNEL_WAVEFORM. */
    tc_stop(TC, obj->channel);
    /* Init TC to waveform mode. */
    tc_init(TC, obj->channel,
            /* Waveform Clock Selection */
            obj->waveconfig.ul_intclock
            | TC_CMR_WAVE /* Waveform mode is enabled */
            | TC_CMR_CPCTRG /* UP mode with automatic trigger on RC Compare */
            | mode
           );
}

/** Initialize PWM Module
 *
 * @param[in][out] obj  The PWM object to initialize
 * @return         void
 */
void pwmout_init(pwmout_t* obj, PinName pin)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    uint32_t ioline = NC;
    uint32_t channel = NC;

    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }
    if(pin != NC) {
        pin_function(pin, pinmap_find_function(pin, PinMap_PWM));
        ioport_disable_pin(pin);
    }
    obj->pin = pin;

    ioline = pinmap_find_function(pin, PinMap_PWM_IO_Line);  /*To find out which IO Line is associated with the pin and initialise accordingly*/ /*pinmap_find_function reused to find out iolin used*/
    MBED_ASSERT(ioline != NC);
    obj->ioline = ioline;

    channel = pinmap_find_peripheral(pin, PinMap_PWM_IO_Line); /* PinMap_PWM_IO_Line contains channel number and ioline to be used*/ /*pinmap_find_peripheral function reused to find out channel number*/
    MBED_ASSERT(channel != NC);
    obj->channel = channel;
    obj->waveconfig.us_frequency = 500;
    obj->waveconfig.us_dutycycle = 50;

    obj->prescalarindex = getprescalarindex(obj->waveconfig.us_frequency);
    obj->waveconfig.ul_intclock = tc_prescalar[obj->prescalarindex];
    pwmout_inithw(obj);

    /*Set the registers a,b,c*/
    setregisterabc(obj);

    /* Enable TC TC_CHANNEL_WAVEFORM. */
    tc_start(TC, channel);
}

/** Free the PWM Module
 *
 * @param[in] obj  The PWM object to free
 * @return    void
 */
void pwmout_free(pwmout_t* obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    tc_stop(TC, obj->channel);
}

/** Set the duty cycle of PWM Waveform
 *
 * @param[in] obj    The PWM object
 * @param[in] value  New duty cycle to be set
 * @return    void
 */
void pwmout_write(pwmout_t* obj, float value)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    if (value < 0.0f) {
        value = 0;
    } else if (value > 1.0f) {
        value = 1;
    }
    obj->waveconfig.us_dutycycle = (uint16_t)(value * 100);
    tc_stop(TC, obj->channel);

    /*Set the registers a,b,c*/
    setregisterabc(obj);

    /* Enable TC TC_CHANNEL_WAVEFORM. */
    tc_start(TC, obj->channel);
}

/** Get the duty cycle of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @return    Current duty cycle
 */
float pwmout_read(pwmout_t* obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    return (obj->waveconfig.us_dutycycle / 100.0);
}

/** Set the period of PWM Waveform
 *
 * @param[in] obj      The PWM object
 * @param[in] seconds  New period in seconds
 * @return           void
 */
void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

/** Set the period of PWM Waveform
 *
 * @param[in] obj    The PWM object
 * @param[in] value  New period in milliseconds
 * @return           void
 */
void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

/** Set the period of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @param[in] us   New period in microseconds
 * @return    void
 */
void pwmout_period_us(pwmout_t* obj, int us)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    float freq = ( 1.0 / us ) * 1000000.0;

    obj->waveconfig.us_frequency = (uint16_t) freq;
    obj->prescalarindex = getprescalarindex(obj->waveconfig.us_frequency);
    obj->waveconfig.ul_intclock = tc_prescalar[obj->prescalarindex];
    pwmout_inithw(obj);

    /*Set the registers a,b,c*/
    setregisterabc(obj);

    /* Enable TC TC_CHANNEL_WAVEFORM. */
    tc_start(TC, obj->channel);
}

/** Set the pulse width of PWM Waveform
 *
 * @param[in] obj      The PWM object
 * @param[in] seconds  New pulse width in seconds
 * @return    void
 */
void pwmout_pulsewidth(pwmout_t* obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

/** Set the pulse width of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @param[in] ms   New pulse width in milliseconds
 * @return    void
 */
void pwmout_pulsewidth_ms(pwmout_t* obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

/** Set the pulse width of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @param[in] us   New pulse width in microseconds
 * @return    void
 */
void pwmout_pulsewidth_us(pwmout_t* obj, int us)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    float new_duty = (us / 1000000.0) * (float)obj->waveconfig.us_frequency;
    pwmout_write(obj, new_duty);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
