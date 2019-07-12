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
#include "mbed_assert.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include <ti/devices/cc32xx/inc/hw_apps_config.h>
#include <ti/devices/cc32xx/inc/hw_ocp_shared.h>
#include <ti/devices/cc32xx/inc/hw_types.h>
#include <ti/devices/cc32xx/inc/hw_timer.h>
#include <ti/devices/cc32xx/inc/hw_memmap.h>
#include <ti/devices/cc32xx/driverlib/rom.h>
#include <ti/devices/cc32xx/driverlib/rom_map.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>
#include <ti/devices/cc32xx/driverlib/pin.h>
#include <ti/devices/cc32xx/driverlib/ti_timer.h>

static const uint32_t timerBaseAddresses[4] = {
    CC3220SF_TIMERA0_BASE,
    CC3220SF_TIMERA1_BASE,
    CC3220SF_TIMERA2_BASE,
    CC3220SF_TIMERA3_BASE,
};

static const uint32_t timerHalves[2] = {
    TIMER_A,
    TIMER_B,
};

/*static const uint32_t gpioBaseAddresses[4] = {
    CC3220SF_GPIOA0_BASE,
    CC3220SF_GPIOA1_BASE,
    CC3220SF_GPIOA2_BASE,
    CC3220SF_GPIOA3_BASE,
};*/

/*static const uint32_t gpioPinIndexes[8] = {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
};*/

#define PinConfigTimerPort(config)     (((config) >> 28) & 0xF)
#define PinConfigTimerHalf(config)     (((config) >> 24) & 0xF)
#define PinConfigGPIOPort(config)      (((config) >> 20) & 0xF)
#define PinConfigGPIOPinIndex(config)  (((config) >> 16) & 0xF)
#define PinConfigPinMode(config)       (((config) >> 8) & 0xF)
#define PinConfigPin(config)           (((config) >> 0) & 0x3F)

#define PWMTimerCC32XX_T0A  (0x00 << 24)
#define PWMTimerCC32XX_T0B  (0x01 << 24)
#define PWMTimerCC32XX_T1A  (0x10 << 24)
#define PWMTimerCC32XX_T1B  (0x11 << 24)
#define PWMTimerCC32XX_T2A  (0x20 << 24)
#define PWMTimerCC32XX_T2B  (0x21 << 24)
#define PWMTimerCC32XX_T3A  (0x30 << 24)
#define PWMTimerCC32XX_T3B  (0x31 << 24)

#define PWMTimerCC32XX_GPIO9   (0x11 << 16)
#define PWMTimerCC32XX_GPIO10  (0x12 << 16)
#define PWMTimerCC32XX_GPIO11  (0x13 << 16)
#define PWMTimerCC32XX_GPIO24  (0x30 << 16)
#define PWMTimerCC32XX_GPIO25  (0x31 << 16)

#define PWMTimerCC32XX_GPIONONE  (0xFF << 16)

#define PWMTimerCC32XX_PIN_01  (PWMTimerCC32XX_T3A | PWMTimerCC32XX_GPIO10 | 0x0300)
#define PWMTimerCC32XX_PIN_02  (PWMTimerCC32XX_T3B | PWMTimerCC32XX_GPIO11 | 0x0301)
#define PWMTimerCC32XX_PIN_17  (PWMTimerCC32XX_T0A | PWMTimerCC32XX_GPIO24 | 0x0510)
#define PWMTimerCC32XX_PIN_19  (PWMTimerCC32XX_T1B | PWMTimerCC32XX_GPIONONE | 0x0812)
#define PWMTimerCC32XX_PIN_21  (PWMTimerCC32XX_T1A | PWMTimerCC32XX_GPIO25 | 0x0914)
#define PWMTimerCC32XX_PIN_64  (PWMTimerCC32XX_T2B | PWMTimerCC32XX_GPIO9 | 0x033F)

//static unsigned int pwm_clock_mhz;

void pwmout_init(pwmout_t* obj, PinName pin) {
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);
    obj->pwm = pwm;

    switch(pin) {
        case PIN_01: obj->pwmPin = PWMTimerCC32XX_PIN_01; break;
        case PIN_02: obj->pwmPin = PWMTimerCC32XX_PIN_02; break;
        case PIN_17: obj->pwmPin = PWMTimerCC32XX_PIN_17; break;
        case PIN_19: obj->pwmPin = PWMTimerCC32XX_PIN_19; break;
        case PIN_21: obj->pwmPin = PWMTimerCC32XX_PIN_21; break;
        case PIN_64: obj->pwmPin = PWMTimerCC32XX_PIN_64; break;
        default: break;
    }

    uint32_t timerBaseAddr = timerBaseAddresses[PinConfigTimerPort(obj->pwmPin)];
    uint16_t halfTimer = timerHalves[PinConfigTimerHalf(obj->pwmPin)];

    MAP_TimerDisable(timerBaseAddr, halfTimer);

    /*
     * The CC32XX SDK TimerConfigure API halts both timers when it is
     * used to configure a single half timer.  The code below performs
     * the register operations necessary to configure each half timer
     * individually.
     */
    /* Enable CCP to IO path */
    HWREG(APPS_CONFIG_BASE + APPS_CONFIG_O_GPT_TRIG_SEL) = 0xFF;

    /* Split the timer and configure it as a PWM */
    uint32_t timerConfigVal = ((halfTimer & (TIMER_CFG_A_PWM | TIMER_CFG_B_PWM)) |
        TIMER_CFG_SPLIT_PAIR);
    HWREG(timerBaseAddr + TIMER_O_CFG) |= (timerConfigVal >> 24);
    if (halfTimer & TIMER_A) {
        HWREG(timerBaseAddr + TIMER_O_TAMR) = timerConfigVal & 255;
    }
    else {
        HWREG(timerBaseAddr + TIMER_O_TBMR) = (timerConfigVal >> 8) & 255;
    }

    /* Set the peripheral output to active-high */
    MAP_TimerControlLevel(timerBaseAddr, halfTimer, true);

    uint16_t mode = PinConfigPinMode(obj->pwmPin);

    /* Start the timer & set pinmux to PWM mode */
    MAP_TimerEnable(timerBaseAddr, halfTimer);
    MAP_PinTypeTimer((unsigned long)pin, (unsigned long)mode);
}

void pwmout_free(pwmout_t* obj) {
    // [TODO]
}

void pwmout_write(pwmout_t* obj, float value) {

}

float pwmout_read(pwmout_t* obj) {
    return 0;
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {

}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {

}
