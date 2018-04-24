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

/***************************************************************** Includes **/
#include "pinmap.h"
#include "PeripheralPins.h"
#include "xmc4_helper.h"
#include "mbed_assert.h"
#include "pwmout_api.h"
#include "xmc_gpio.h"
#include "xmc_ccu8.h"
#include "xmc_ccu4.h"
#include "xmc_scu.h"

/****************************************************************** Defines **/
#define CCU_MODULE_4                    4
#define CCU_MODULE_8                    8

/******************************************************************* Macros **/
#define GET_CCU_MODULE(x)               ((x & 0x0000F000) >> 12)
#define GET_CCU_MODULE_NBR(x)           ((x & 0x00000F00) >> 8)
#define GET_CCU_SLICE(x)                (x & 0x0000000F)

/****************************************************************** Globals **/

/* Slice compare configuration CCU8 */
XMC_CCU8_SLICE_COMPARE_CONFIG_t slice_cfg_ccu8 =
{
    .timer_mode = (uint32_t)XMC_CCU8_SLICE_TIMER_COUNT_MODE_EA,
    .monoshot = XMC_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT,
    .shadow_xfer_clear = 0U,
    .dither_timer_period = 0U,
    .dither_duty_cycle = 0U,
    .prescaler_mode = (uint32_t)XMC_CCU8_SLICE_PRESCALER_MODE_NORMAL,
    .mcm_ch1_enable = false,
    .mcm_ch2_enable = false,
    .passive_level_out0 = (uint32_t)XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
    .passive_level_out1 = (uint32_t)XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
    .passive_level_out2 = (uint32_t)XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
    .passive_level_out3 = (uint32_t)XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
    .asymmetric_pwm = 0U,
    .invert_out0 = 0U,
    .invert_out1 = 0U,
    .invert_out2 = 0U,
    .invert_out3 = 0U,
    .prescaler_initval = XMC_CCU8_SLICE_PRESCALER_128,
    .float_limit = 0U,
    .dither_limit = 0U,
    .timer_concatenation = 0U,
};

/* Slice compare configuration CCU4 */
XMC_CCU4_SLICE_COMPARE_CONFIG_t slice_cfg_ccu4 =
{
    .timer_mode = (uint32_t)XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
    .monoshot = (uint32_t)XMC_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT,
    .shadow_xfer_clear = 0U,
    .dither_timer_period = 0U,
    .dither_duty_cycle = 0U,
    .prescaler_mode = (uint32_t)XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
    .prescaler_initval = XMC_CCU4_SLICE_PRESCALER_128,
    .float_limit = 0U,
    .passive_level = (uint32_t)XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
    .dither_limit = 0U,
    .timer_concatenation = 0U,
    .mcm_enable = 0U
};

/* Pin configuration for PWM generation */
XMC_GPIO_CONFIG_t pwm_gpio_cfg =
{
    .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT3,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
};

/* Prescaler values for CCU Module */
const uint32_t ccu_prescaler[] = {
    XMC_CCU8_SLICE_PRESCALER_1,
    XMC_CCU8_SLICE_PRESCALER_2,
    XMC_CCU8_SLICE_PRESCALER_4,
    XMC_CCU8_SLICE_PRESCALER_8,
    XMC_CCU8_SLICE_PRESCALER_16,
    XMC_CCU8_SLICE_PRESCALER_32,
    XMC_CCU8_SLICE_PRESCALER_64,
    XMC_CCU8_SLICE_PRESCALER_128,
    XMC_CCU8_SLICE_PRESCALER_256,
    XMC_CCU8_SLICE_PRESCALER_512,
    XMC_CCU8_SLICE_PRESCALER_1024,
    XMC_CCU8_SLICE_PRESCALER_2048,
    XMC_CCU8_SLICE_PRESCALER_4096,
    XMC_CCU8_SLICE_PRESCALER_8192,
    XMC_CCU8_SLICE_PRESCALER_16384,
    XMC_CCU8_SLICE_PRESCALER_32768
};

/******************************************************* Internal Functions **/

/** Set the period of PWM object (will not update the waveform)
 *
 * @param[in] obj        The PWM object whose period is to be updated
 * @param[in] period_us  Period in microseconds
 * @return    void
 */
static void pwmout_set_period(pwmout_t* obj, int period_us)
{
    uint8_t i;
    uint32_t div_freq;
    double us_per_cycle;
    uint64_t max_period = 0;
    uint32_t us_period = period_us;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    for (i=0; i<sizeof(ccu_prescaler); i++) {
        div_freq = obj->ccu_freq >> ccu_prescaler[i];
        if (!div_freq) break;
        us_per_cycle = 1000000.0 / div_freq;
        max_period = us_per_cycle * 65535;
        if (max_period >= us_period) {
            obj->prescaler = (XMC_CCU8_SLICE_PRESCALER_t)ccu_prescaler[i];
            obj->period = us_period / us_per_cycle;
            obj->us_per_cycle = us_per_cycle;
            break;
        }
    }
}

/* @internal Initialize Timer module for PWM generation
 *
 * @param obj The pwmout object to initialize
 */
static void pwm_hw_init(pwmout_t *obj) {

    uint8_t alt_function;
    port_type_t *port;

    switch(obj->ccu_module) {
        case CCU_MODULE_4:
            /* Init CCU4x module */
            XMC_CCU4_Init((XMC_CCU4_MODULE_t *)obj->module_ptr, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
            /* Start the prescaler */
            XMC_CCU4_StartPrescaler((XMC_CCU4_MODULE_t *)obj->module_ptr);
            /* Ensure fCCU reaches CCU80 */
            XMC_CCU4_SetModuleClock((XMC_CCU4_MODULE_t *)obj->module_ptr, XMC_CCU4_CLOCK_SCU);

            /* Init Timer slice */
            XMC_CCU4_SLICE_CompareInit((XMC_CCU4_SLICE_t *)obj->slice_ptr, &slice_cfg_ccu4);

            /*Initializes the GPIO*/
            alt_function = (uint8_t)pinmap_function(obj->pin, PinMap_PWM);
            pwm_gpio_cfg.mode = (0x80UL | (alt_function << PORT0_IOCR0_PC0_Pos));
            port = (port_type_t *)(PORT0_BASE + (uint32_t)((obj->pin & 0xF0) << 4));
            XMC_GPIO_Init((XMC_GPIO_PORT_t *)port, obj->pin & 0x0F, &pwm_gpio_cfg);

            /* Enable clock to slice */
            XMC_CCU4_EnableClock((XMC_CCU4_MODULE_t *)obj->module_ptr, obj->slice_nbr);
            /* Start the timer & set default */
            pwmout_period_ms(obj, 20);
            pwmout_write(obj, 0.50f);
            break;
        case CCU_MODULE_8:
            /* Init CCU4x module */
            XMC_CCU8_Init((XMC_CCU8_MODULE_t *)obj->module_ptr, XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
            /* Start the prescaler */
            XMC_CCU8_StartPrescaler((XMC_CCU8_MODULE_t *)obj->module_ptr);
            /* Ensure fCCU reaches CCU80 */
            XMC_CCU8_SetModuleClock((XMC_CCU8_MODULE_t *)obj->module_ptr, XMC_CCU8_CLOCK_SCU);

            /* Init Timer slice */
            slice_cfg_ccu8.slice_status = (uint32_t)obj->channel_nbr;
            XMC_CCU8_SLICE_CompareInit((XMC_CCU8_SLICE_t *)obj->slice_ptr, &slice_cfg_ccu8);

            /*Initializes the GPIO*/
            alt_function = (uint8_t)pinmap_function(obj->pin, PinMap_PWM);
            pwm_gpio_cfg.mode = (0x80UL | (alt_function << PORT0_IOCR0_PC0_Pos));
            port = (port_type_t *)(PORT0_BASE + (uint32_t)((obj->pin & 0xF0) << 4));
            XMC_GPIO_Init((XMC_GPIO_PORT_t *)port, obj->pin & 0x0F, &pwm_gpio_cfg);

            /* Enable clock to slice */
            XMC_CCU8_EnableClock((XMC_CCU8_MODULE_t *)obj->module_ptr, obj->slice_nbr);

            /* Start the timer & set default */
            pwmout_period_ms(obj, 20);
            pwmout_write(obj, 0.50f);
            break;
    }
}

/**************************************************************** Functions **/

/** Initialize the pwm out peripheral and configure the pin
 *
 * @param obj The pwmout object to initialize
 * @param pin The pwmout pin to initialize
 */
void pwmout_init(pwmout_t *obj, PinName pin) {

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Get PWM Unit from Pin */
    uint32_t pwm_module = pinmap_peripheral(pin, PinMap_PWM);
    if ((uint32_t)NC == pwm_module) {
        /* Pin not supported */
        return;
    }

    pin_io_function_t *pwm_function = get_pin_function(pin);

    /* Set object parameters */
    obj->pin = pin;
    obj->ccu_freq = XMC_SCU_CLOCK_GetCcuClockFrequency();
    switch(GET_CCU_MODULE(pwm_module)) {
        case CCU_MODULE_4:
            obj->ccu_module = CCU_MODULE_4;
            switch(GET_CCU_MODULE_NBR(pwm_module)) {
                case 0:
                    obj->module_ptr = CCU40;
                    obj->slice_nbr = GET_CCU_SLICE(pwm_module);
                    obj->slice_ptr = (uint32_t *)(CCU40_CC40_BASE + (obj->slice_nbr * 0x100UL));
                    break;
                case 1:
                    obj->module_ptr = CCU41;
                    obj->slice_nbr = GET_CCU_SLICE(pwm_module);
                    obj->slice_ptr = (uint32_t *)(CCU41_CC40_BASE + (obj->slice_nbr * 0x100UL));
                    break;
                case 2:
                    obj->module_ptr = CCU42;
                    obj->slice_nbr = GET_CCU_SLICE(pwm_module);
                    obj->slice_ptr = (uint32_t *)(CCU42_CC40_BASE + (obj->slice_nbr * 0x100UL));
                    break;
                case 3:
                    obj->module_ptr = CCU43;
                    obj->slice_nbr = GET_CCU_SLICE(pwm_module);
                    obj->slice_ptr = (uint32_t *)(CCU43_CC40_BASE + (obj->slice_nbr * 0x100UL));
                    break;
            }
            break;
        case CCU_MODULE_8:
            obj->ccu_module = CCU_MODULE_8;
            switch(GET_CCU_MODULE_NBR(pwm_module)) {
                case 0:
                    obj->module_ptr = CCU80;
                    obj->slice_nbr = GET_CCU_SLICE(pwm_module);
                    obj->channel_nbr = pwm_function->pwm_ccu8_channel - 1;
                    obj->slice_ptr = (uint32_t *)(CCU80_CC80_BASE + (obj->slice_nbr * 0x100UL));
                    break;
                case 1:
                    obj->module_ptr = CCU81;
                    obj->slice_nbr = GET_CCU_SLICE(pwm_module);
                    obj->channel_nbr = pwm_function->pwm_ccu8_channel - 1;
                    obj->slice_ptr = (uint32_t *)(CCU81_CC80_BASE + (obj->slice_nbr * 0x100UL));
                    break;
            }
            break;
    }

    /* Initialize HW PWM */
    pwm_hw_init(obj);
}

/** Deinitialize the pwmout object
 *
 * @param obj The pwmout object
 */
void pwmout_free(pwmout_t *obj) {

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Stop CCU */
    switch(obj->ccu_module) {
        case CCU_MODULE_4:
            XMC_CCU4_SLICE_StopTimer((XMC_CCU4_SLICE_t *)obj->slice_ptr);
            break;
        case CCU_MODULE_8:
            XMC_CCU8_SLICE_StopTimer((XMC_CCU8_SLICE_t *)obj->slice_ptr);
            break;
    }
}

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * Value 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * @param obj     The pwmout object
 * @param percent The floating-point percentage number
 */
void pwmout_write(pwmout_t *obj, float percent) {

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    if (percent < 0.0f) {
        percent = 0;
    } else if (percent > 1.0f) {
        percent = 1;
    }

    uint16_t cmp_val = obj->period - (obj->period * percent);
    obj->duty_cycle = percent;

    /* Update changes */
    switch(obj->ccu_module) {
        case CCU_MODULE_4:
            /* Set duty cycle */
            XMC_CCU4_SLICE_SetTimerCompareMatch((XMC_CCU4_SLICE_t *)obj->slice_ptr, cmp_val);
            XMC_CCU4_EnableShadowTransfer((XMC_CCU4_MODULE_t *)obj->module_ptr, (uint32_t)(0x1UL << (obj->slice_nbr)*4));
            break;
        case CCU_MODULE_8:
            /* Set duty cycle */
            XMC_CCU8_SLICE_SetTimerCompareMatch((XMC_CCU8_SLICE_t *)obj->slice_ptr, (uint32_t)obj->channel_nbr, cmp_val);
            XMC_CCU8_EnableShadowTransfer((XMC_CCU8_MODULE_t *)obj->module_ptr, (uint32_t)(0x1UL << (obj->slice_nbr)*4));
            break;
    }
}

/** Read the current float-point output duty-cycle
 *
 * @param obj The pwmout object
 * @return A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj) {

    return obj->duty_cycle;
}

/** Set the PWM period specified in seconds, keeping the duty cycle the same
 *
 * Periods smaller than microseconds (the lowest resolution) are set to zero.
 * @param obj     The pwmout object
 * @param seconds The floating-point seconds period
 */
void pwmout_period(pwmout_t *obj, float seconds) {

    pwmout_period_us(obj, seconds * 1000000.0f);
}

/** Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param ms  The milisecond period
 */
void pwmout_period_ms(pwmout_t *obj, int ms) {

    pwmout_period_us(obj, ms * 1000);
}

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param us  The microsecond period
 */
void pwmout_period_us(pwmout_t *obj, int us) {

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Find and set the best period value */
    pwmout_set_period(obj, us);

    /* Update changes */
    switch(obj->ccu_module) {
        case CCU_MODULE_4:
            /* Stop Timer */
            XMC_CCU4_SLICE_StopTimer((XMC_CCU4_SLICE_t *)obj->slice_ptr);
            /* Set prescaler */
            XMC_CCU4_SLICE_SetPrescaler((XMC_CCU4_SLICE_t *)obj->slice_ptr, obj->prescaler);
            /* Set period & duty cycle */
            XMC_CCU4_SLICE_SetTimerPeriodMatch((XMC_CCU4_SLICE_t *)obj->slice_ptr, obj->period);
            XMC_CCU4_SLICE_SetTimerCompareMatch((XMC_CCU4_SLICE_t *)obj->slice_ptr, obj->period / 2);
            XMC_CCU4_EnableShadowTransfer((XMC_CCU4_MODULE_t *)obj->module_ptr, (uint32_t)(0x1UL << (obj->slice_nbr)*4));
            /* Start Timer */
            XMC_CCU4_SLICE_StartTimer((XMC_CCU4_SLICE_t *)obj->slice_ptr);
            break;
        case CCU_MODULE_8:
            /* Stop Timer */
            XMC_CCU8_SLICE_StopTimer((XMC_CCU8_SLICE_t *)obj->slice_ptr);
            /* Set prescaler */
            XMC_CCU8_SLICE_SetPrescaler((XMC_CCU8_SLICE_t *)obj->slice_ptr, obj->prescaler);
            /* Set period & duty cycle */
            XMC_CCU8_SLICE_SetTimerPeriodMatch((XMC_CCU8_SLICE_t *)obj->slice_ptr,  obj->period);
            XMC_CCU8_SLICE_SetTimerCompareMatch((XMC_CCU8_SLICE_t *)obj->slice_ptr, (uint32_t)obj->channel_nbr, obj->period / 2);
            XMC_CCU8_EnableShadowTransfer((XMC_CCU8_MODULE_t *)obj->module_ptr, (uint32_t)(0x1UL << (obj->slice_nbr)*4));
            /* Start Timer */
            XMC_CCU8_SLICE_StartTimer((XMC_CCU8_SLICE_t *)obj->slice_ptr);
            break;
    }
}

/** Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * @param obj     The pwmout object
 * @param seconds The floating-point pulsewidth in seconds
 */
void pwmout_pulsewidth(pwmout_t *obj, float seconds) {

    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

/** Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param ms  The floating-point pulsewidth in miliseconds
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int ms) {

    pwmout_pulsewidth_us(obj, ms * 1000);
}

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param us  The floating-point pulsewidth in microseconds
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int us) {

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Find the new duty cycle */
    uint32_t us_pulse = us;

    /* Find the new duty cycle */
    double duty_cycle = us_pulse / ((double)obj->period * obj->us_per_cycle);

    /* This call updates pulse width as well as period */
    pwmout_write(obj, duty_cycle);
}

/*EOF*/
