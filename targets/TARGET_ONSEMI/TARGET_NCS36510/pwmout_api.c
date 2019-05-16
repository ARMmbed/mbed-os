/**
 ******************************************************************************
 * @file pwmout_api.c
 * @brief Implementation of a PWM driver
 * @internal
 * @author ON Semiconductor
 * $Rev:
 * $Date:
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
*/
#include "pwmout_api.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"
#include "clock.h"

#if DEVICE_PWMOUT

/**
 * \defgroup hal_pwmout Pwmout hal functions
 * @{
 */

/** Initialize the pwm out peripheral and configure the pin
 *
 * @param obj The pwmout object to initialize
 * @param pin The pwmout pin to initialize
 */
void pwmout_init(pwmout_t *obj, PinName pin)
{
    /* Get the base address of the PWM register using the pinmap functions ; pwmout_s struct contains base address only */
    PWMName pwm;

    pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    pinmap_pinout(pin, PinMap_PWM);

    obj->pwmReg = (PwmReg_pt)pwm;
    MBED_ASSERT(obj->pwmReg != 0x00000000);

    CLOCK_ENABLE(CLOCK_PWM);

    /* Configuration parameters of duty cycle 0x4000B000, and prescaler 0x4000B00C, shall be set to default values */
    /* Duty cycle shall be 50% and prescaler shall be disabled by default */
    obj->pwmReg->DUTYCYCLE = 0x80;

    /* Write the PWM output enable register 0x4000B004, to 1 */
    obj->pwmReg->PWM_ENABLE = 0x1;

    obj->pwmReg->PRESCALE_DISABLE = 0x1;

}

/** Deinitialize the pwmout object
 *
 * @param obj The pwmout object
 */
void pwmout_free(pwmout_t *obj)
{
    /* Write the PWM output disable register 0x4000B008, to 1 */
    obj->pwmReg->PWM_DISABLE = 0x1;
}

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * Value 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * @param obj     The pwmout object
 * @param percent The floating-point percentage number
 */
void pwmout_write(pwmout_t *obj, float percent)
{
    if (percent == 0.0) {
        obj->pwmReg->DUTYCYCLE = 0x00;
    } else if (percent == 1.0) {
        obj->pwmReg->DUTYCYCLE = 0xFF;
    } else {
        /* Write the duty cycle config register 0x4000B000, with the value passed on */
        /* ((percent * 255) + 1) is the duty cycle. Plus 1 is for accounting for round off errors; like a ceil function */
        obj->pwmReg->DUTYCYCLE = (uint8_t)((percent * 255) + 1);
    }
}

/** Read the current float-point output duty-cycle
 *
 * @param obj The pwmout object
 * @return A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj)
{
    float retVal = 0.0;
    float dc = 0.0;

    /* Read out the value of duty cycle register 0x4000B000 and return as a percent */
    /* Read value / 255 is the percent returned */
    dc = obj->pwmReg->DUTYCYCLE;
    retVal = dc/ (float)255;

    return(retVal);
}

/** Set the PWM period specified in seconds, keeping the duty cycle the same
 *
 * Periods smaller than microseconds (the lowest resolution) are set to zero.
 * @param obj     The pwmout object
 * @param seconds The floating-point seconds period
 */
void pwmout_period(pwmout_t *obj, float seconds)
{
    /* Cannot be configured, prescaler is either 256 or 4096 */
    return;
}

/** Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param ms  The milisecond period
 */
void pwmout_period_ms(pwmout_t *obj, int ms)
{
    /* Cannot be configured, prescaler is either 256 or 4096 */
    return;
}

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * @param obj The pwmout object
 * @param us  The microsecond period
 */
void pwmout_period_us(pwmout_t *obj, int us)
{
    /* Cannot be configured, prescaler is either 256 or 4096 */
    return;
}

/** Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * @param obj     The pwmout object
 * @param seconds The floating-point pulsewidth in seconds
 */
void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    /* Pulse width can never be in seconds since the period
    * itself is limited to either 8uSec or 128uSec
    */
    return;
}

/** Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param ms  The floating-point pulsewidth in miliseconds
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{

    /* Pulse width can never be in seconds since the period
    * itself is limited to either 8uSec or 128uSec
    */
    return;
}

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * @param obj The pwmout object
 * @param us  The floating-point pulsewidth in microseconds
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    int pulseWidth = 0;

    /* Check if the uSec value is greater than 128uSec, if so reject */
    if (us > 128) {
        return;
    }
    /* If pulsewidth is less than 128uSec, set the prescaler to 4096
     * by enabling prescale register 0x4000B00C to 1 */
    obj->pwmReg->PRESCALE_ENABLE = 0x1;

    /* Calculate the duty cycle based on the width of the pulse */
    /* ((255 * us) / 128) + 1 = duty cycle */
    pulseWidth = (int)((float)(255 * us)/(float)128) + 1;
    if (us == 0) {
        obj->pwmReg->DUTYCYCLE = 0x0;
    } else if (us == 128) {
        obj->pwmReg->DUTYCYCLE = 0xFF;
    } else {
        obj->pwmReg->DUTYCYCLE = (uint8_t)pulseWidth;
    }
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}

/**@}*/

#endif // DEVICE_PWMOUT
