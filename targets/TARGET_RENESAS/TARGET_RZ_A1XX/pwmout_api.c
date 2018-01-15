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
#include "PeripheralPins.h"
#include "RZ_A1_Init.h"
#include "iodefine.h"
#include "gpio_addrdefine.h"
#include "mbed_drv_cfg.h"

#define MTU2_PWM_OFFSET         0x20

#ifdef FUNC_MOTOR_CTL_PWM
typedef enum {
    PWM1A  = 0,
    PWM1B,
    PWM1C,
    PWM1D,
    PWM1E,
    PWM1F,
    PWM1G,
    PWM1H,
    PWM2A  = 0x10,
    PWM2B,
    PWM2C,
    PWM2D,
    PWM2E,
    PWM2F,
    PWM2G,
    PWM2H,
} PWMType;

static const PWMType PORT[] = {
     PWM1A,          // PWM_PWM1A
     PWM1B,          // PWM_PWM1B
     PWM1C,          // PWM_PWM1C
     PWM1D,          // PWM_PWM1D
     PWM1E,          // PWM_PWM1E
     PWM1F,          // PWM_PWM1F
     PWM1G,          // PWM_PWM1G
     PWM1H,          // PWM_PWM1H
     PWM2A,          // PWM_PWM2A
     PWM2B,          // PWM_PWM2B
     PWM2C,          // PWM_PWM2C
     PWM2D,          // PWM_PWM2D
     PWM2E,          // PWM_PWM2E
     PWM2F,          // PWM_PWM2F
     PWM2G,          // PWM_PWM2G
     PWM2H,          // PWM_PWM2H
};

static __IO uint16_t *PWM_MATCH[] = {
    &PWMPWBFR_1A,    // PWM_PWM1A
    &PWMPWBFR_1A,    // PWM_PWM1B
    &PWMPWBFR_1C,    // PWM_PWM1C
    &PWMPWBFR_1C,    // PWM_PWM1D
    &PWMPWBFR_1E,    // PWM_PWM1E
    &PWMPWBFR_1E,    // PWM_PWM1F
    &PWMPWBFR_1G,    // PWM_PWM1G
    &PWMPWBFR_1G,    // PWM_PWM1H
    &PWMPWBFR_2A,    // PWM_PWM2A
    &PWMPWBFR_2A,    // PWM_PWM2B
    &PWMPWBFR_2C,    // PWM_PWM2C
    &PWMPWBFR_2C,    // PWM_PWM2D
    &PWMPWBFR_2E,    // PWM_PWM2E
    &PWMPWBFR_2E,    // PWM_PWM2F
    &PWMPWBFR_2G,    // PWM_PWM2G
    &PWMPWBFR_2G,    // PWM_PWM2H
};

static uint16_t init_period_ch1 = 0;
static uint16_t init_period_ch2 = 0;
static int32_t  period_ch1 = 1;
static int32_t  period_ch2 = 1;
#endif

#ifdef FUMC_MTU2_PWM
#define MTU2_PWM_SIGNAL         2

typedef enum {
    TIOC0A  = 0,
    TIOC0B,
    TIOC0C,
    TIOC0D,
    TIOC1A = 0x10,
    TIOC1B,
    TIOC2A = 0x20,
    TIOC2B,
    TIOC3A = 0x30,
    TIOC3B,
    TIOC3C,
    TIOC3D,
    TIOC4A = 0x40,
    TIOC4B,
    TIOC4C,
    TIOC4D,
} MTU2_PWMType;

static const MTU2_PWMType MTU2_PORT[] = {
     TIOC0A,         // PWM_TIOC0A
     TIOC0C,         // PWM_TIOC0C
     TIOC1A,         // PWM_TIOC1A
     TIOC2A,         // PWM_TIOC2A
     TIOC3A,         // PWM_TIOC3A
     TIOC3C,         // PWM_TIOC3C
     TIOC4A,         // PWM_TIOC4A
     TIOC4C,         // PWM_TIOC4C
};

static __IO uint16_t *MTU2_PWM_MATCH[][MTU2_PWM_SIGNAL] = {
    { &MTU2TGRA_0, &MTU2TGRB_0 },       // PWM_TIOC0A
    { &MTU2TGRC_0, &MTU2TGRD_0 },       // PWM_TIOC0C
    { &MTU2TGRA_1, &MTU2TGRB_1 },       // PWM_TIOC1A
    { &MTU2TGRA_2, &MTU2TGRB_2 },       // PWM_TIOC2A
    { &MTU2TGRA_3, &MTU2TGRB_3 },       // PWM_TIOC3A
    { &MTU2TGRC_3, &MTU2TGRD_3 },       // PWM_TIOC3C
    { &MTU2TGRA_4, &MTU2TGRB_4 },       // PWM_TIOC4A
    { &MTU2TGRC_4, &MTU2TGRD_4 },       // PWM_TIOC4C
};

static __IO uint8_t *TCR_MATCH[] = {
    &MTU2TCR_0,
    &MTU2TCR_1,
    &MTU2TCR_2,
    &MTU2TCR_3,
    &MTU2TCR_4,
};

static __IO uint8_t *TIORH_MATCH[] = {
    &MTU2TIORH_0,
    &MTU2TIOR_1,
    &MTU2TIOR_2,
    &MTU2TIORH_3,
    &MTU2TIORH_4,
};

static __IO uint8_t *TIORL_MATCH[] = {
    &MTU2TIORL_0,
    NULL,
    NULL,
    &MTU2TIORL_3,
    &MTU2TIORL_4,
};

static __IO uint16_t *TGRA_MATCH[] = {
    &MTU2TGRA_0,
    &MTU2TGRA_1,
    &MTU2TGRA_2,
    &MTU2TGRA_3,
    &MTU2TGRA_4,
};

static __IO uint16_t *TGRC_MATCH[] = {
    &MTU2TGRC_0,
    NULL,
    NULL,
    &MTU2TGRC_3,
    &MTU2TGRC_4,
};

static __IO uint8_t *TMDR_MATCH[] = {
    &MTU2TMDR_0,
    &MTU2TMDR_1,
    &MTU2TMDR_2,
    &MTU2TMDR_3,
    &MTU2TMDR_4,
};

static int MAX_PERIOD[] = {
    125000,
    503000,
    2000000,
    2000000,
    2000000,
};

typedef enum {
    MTU2_PULSE = 0,
    MTU2_PERIOD
} MTU2Signal;

static uint16_t init_mtu2_period_ch[5] = {0};
static int32_t  mtu2_period_ch[5] = {1, 1, 1, 1, 1};
#endif

void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    if (pwm >= MTU2_PWM_OFFSET) {
#ifdef FUMC_MTU2_PWM
        /* PWM by MTU2 */
        int tmp_pwm;
        
        // power on
        CPGSTBCR3 &= ~(CPG_STBCR3_BIT_MSTP33);
        
        obj->pwm = pwm;
        tmp_pwm = (int)(obj->pwm - MTU2_PWM_OFFSET);
        if (((uint32_t)MTU2_PORT[tmp_pwm] & 0x00000040) == 0x00000040) {
            obj->ch  = 4;
            MTU2TOER |= 0x36;
        } else if (((uint32_t)MTU2_PORT[tmp_pwm] & 0x00000030) == 0x00000030) {
            obj->ch  = 3;
            MTU2TOER |= 0x09;
        } else if (((uint32_t)MTU2_PORT[tmp_pwm] & 0x00000020) == 0x00000020) {
            obj->ch  = 2;
        } else if (((uint32_t)MTU2_PORT[tmp_pwm] & 0x00000010) == 0x00000010) {
            obj->ch  = 1;
        } else {
            obj->ch  = 0;
        }
        // Wire pinout
        pinmap_pinout(pin, PinMap_PWM);

        int bitmask = 1 << (pin  & 0xf);

        *PMSR(PINGROUP(pin)) = (bitmask << 16) | 0;

        // default duty 0.0f
        pwmout_write(obj, 0);
        if (init_mtu2_period_ch[obj->ch] == 0) {
            // default period 1ms
            pwmout_period_us(obj, 1000);
            init_mtu2_period_ch[obj->ch] = 1;
        }
#endif
    } else {
#ifdef FUNC_MOTOR_CTL_PWM
        /* PWM */
        // power on
        CPGSTBCR3 &= ~(CPG_STBCR3_BIT_MSTP30);

        obj->pwm = pwm;
        if (((uint32_t)PORT[obj->pwm] & 0x00000010) == 0x00000010) {
            obj->ch  = 2;
            PWMPWPR_2 = 0x00;
        } else {
            obj->ch  = 1;
            PWMPWPR_1 = 0x00;
        }

        // Wire pinout
        pinmap_pinout(pin, PinMap_PWM);

        // default to 491us: standard for servos, and fine for e.g. brightness control
        pwmout_write(obj, 0);
        if ((obj->ch == 2) && (init_period_ch2 == 0)) {
            pwmout_period_us(obj, 491);
            init_period_ch2 = 1;
        }
        if ((obj->ch == 1) && (init_period_ch1 == 0)) {
            pwmout_period_us(obj, 491);
            init_period_ch1 = 1;
        }
#endif
    }
}

void pwmout_free(pwmout_t* obj) {
    pwmout_write(obj, 0);
}

void pwmout_write(pwmout_t* obj, float value) {
    uint32_t wk_cycle;

    if (obj->pwm >= MTU2_PWM_OFFSET) {
#ifdef FUMC_MTU2_PWM
        /* PWM by MTU2 */
        int tmp_pwm;

        if (value < 0.0f) {
            value = 0.0f;
        } else if (value > 1.0f) {
            value = 1.0f;
        } else {
            // Do Nothing
        }
        tmp_pwm = (int)(obj->pwm - MTU2_PWM_OFFSET);
        wk_cycle = *MTU2_PWM_MATCH[tmp_pwm][MTU2_PERIOD] & 0xffff;
        // set channel match to percentage
        if (value == 1.0f) {
            *MTU2_PWM_MATCH[tmp_pwm][MTU2_PULSE] = (uint16_t)(wk_cycle - 1);
        } else {
            *MTU2_PWM_MATCH[tmp_pwm][MTU2_PULSE] = (uint16_t)((float)wk_cycle * value);
        }
#endif
    } else {
#ifdef FUNC_MOTOR_CTL_PWM
        uint16_t v;

        /* PWM */
        if (value < 0.0f) {
            value = 0.0f;
        } else if (value > 1.0f) {
            value = 1.0f;
        } else {
            // Do Nothing
        }

        if (obj->ch == 2) {
            wk_cycle = PWMPWCYR_2 & 0x03ff;
        } else {
            wk_cycle = PWMPWCYR_1 & 0x03ff;
        }

        // set channel match to percentage
        v = (uint16_t)((float)wk_cycle * value);
        *PWM_MATCH[obj->pwm] = (v | ((PORT[obj->pwm] & 1) << 12));
#endif
    }
}

float pwmout_read(pwmout_t* obj) {
    uint32_t wk_cycle;
    float value;

    if (obj->pwm >= MTU2_PWM_OFFSET) {
#ifdef FUMC_MTU2_PWM
        /* PWM by MTU2 */
        uint32_t wk_pulse;
        int tmp_pwm;
        
        tmp_pwm = (int)(obj->pwm - MTU2_PWM_OFFSET);
        wk_cycle = *MTU2_PWM_MATCH[tmp_pwm][MTU2_PERIOD] & 0xffff;
        wk_pulse = *MTU2_PWM_MATCH[tmp_pwm][MTU2_PULSE] & 0xffff;
        value = ((float)wk_pulse / (float)wk_cycle);
#endif
    } else {
#ifdef FUNC_MOTOR_CTL_PWM
        /* PWM */
        if (obj->ch == 2) {
            wk_cycle = PWMPWCYR_2 & 0x03ff;
        } else {
            wk_cycle = PWMPWCYR_1 & 0x03ff;
        }
        value = ((float)(*PWM_MATCH[obj->pwm] & 0x03ff) / (float)wk_cycle);
#endif
    }

    return (value > 1.0f) ? (1.0f) : (value);
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

#ifdef FUNC_MOTOR_CTL_PWM
static void set_duty_again(__IO uint16_t *p_pwmpbfr, uint16_t last_cycle, uint16_t new_cycle){
    uint16_t wk_pwmpbfr;
    float    value;
    uint16_t v;

    wk_pwmpbfr = *p_pwmpbfr;
    value      = ((float)(wk_pwmpbfr & 0x03ff) / (float)last_cycle);
    v          = (uint16_t)((float)new_cycle * value);
    *p_pwmpbfr = (v | (wk_pwmpbfr & 0x1000));
}
#endif

#ifdef FUMC_MTU2_PWM
static void set_mtu2_duty_again(__IO uint16_t *p_pwmpbfr, uint16_t last_cycle, uint16_t new_cycle){
    uint16_t wk_pwmpbfr;
    float    value;

    wk_pwmpbfr = *p_pwmpbfr;
    value      = ((float)(wk_pwmpbfr & 0xffff) / (float)last_cycle);
    *p_pwmpbfr = (uint16_t)((float)new_cycle * value);
}
#endif

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
    uint32_t pclk_base;
    uint32_t wk_cycle;
    uint32_t wk_cks = 0;
    uint16_t wk_last_cycle;

    if (obj->pwm >= MTU2_PWM_OFFSET) {
#ifdef FUMC_MTU2_PWM
        uint64_t wk_cycle_mtu2;
        int      max_us = 0;

        /* PWM by MTU2 */
        int tmp_pwm;
        uint8_t tmp_tcr_up;
        uint8_t tmp_tstr_sp;
        uint8_t tmp_tstr_st;
        
        max_us = MAX_PERIOD[obj->ch];
        if (us > max_us) {
            us = max_us;
        } else if (us < 1) {
            us = 1;
        } else {
            // Do Nothing
        }

        if (RZ_A1_IsClockMode0() == false) {
            pclk_base = (uint32_t)CM1_RENESAS_RZ_A1_P0_CLK;
        } else {
            pclk_base = (uint32_t)CM0_RENESAS_RZ_A1_P0_CLK;
        }

        wk_cycle_mtu2 = (uint64_t)pclk_base * us;
        while (wk_cycle_mtu2 >= 65535000000) {
            if ((obj->ch == 1) && (wk_cks == 3)) {
                wk_cks+=2;
            } else if ((obj->ch == 2) && (wk_cks == 3)) {
                wk_cycle_mtu2 >>= 2;
                wk_cks+=3;
            }
            wk_cycle_mtu2 >>= 2;
            wk_cks++;
        }
        wk_cycle = (uint32_t)(wk_cycle_mtu2 / 1000000);

        tmp_pwm = (int)(obj->pwm - MTU2_PWM_OFFSET);
        if (((uint8_t)MTU2_PORT[tmp_pwm] & 0x02) == 0x02) {
            tmp_tcr_up = 0xC0;
        } else {
            tmp_tcr_up = 0x40;
        }
        if ((obj->ch == 4) || (obj->ch == 3)) {
            tmp_tstr_sp = ~(0x38 | (1 << (obj->ch + 3)));
            tmp_tstr_st = (1 << (obj->ch + 3));
        } else {
            tmp_tstr_sp = ~(0x38 | (1 << obj->ch));
            tmp_tstr_st = (1 << obj->ch);
        }
        // Counter Stop
        MTU2TSTR &= tmp_tstr_sp;
        wk_last_cycle = *MTU2_PWM_MATCH[tmp_pwm][MTU2_PERIOD] & 0xffff;
        *TCR_MATCH[obj->ch] = tmp_tcr_up | wk_cks;
        *TIORH_MATCH[obj->ch] = 0x21;
        if ((obj->ch == 0) || (obj->ch == 3) || (obj->ch == 4)) {
            *TIORL_MATCH[obj->ch] = 0x21;
        }
        *MTU2_PWM_MATCH[tmp_pwm][MTU2_PERIOD] = (uint16_t)wk_cycle;     // Set period

        // Set duty again(TGRA)
        set_mtu2_duty_again(TGRA_MATCH[obj->ch], wk_last_cycle, wk_cycle);
        if ((obj->ch == 0) || (obj->ch == 3) || (obj->ch == 4)) {
            // Set duty again(TGRC)
            set_mtu2_duty_again(TGRC_MATCH[obj->ch], wk_last_cycle, wk_cycle);
        }
        *TMDR_MATCH[obj->ch] = 0x02;                                    // PWM mode 1

        // Counter Start
        MTU2TSTR |= tmp_tstr_st;
        // Save for future use
        mtu2_period_ch[obj->ch] = us;
#endif
    } else {
#ifdef FUNC_MOTOR_CTL_PWM
        /* PWM */
        if (us > 491) {
            us = 491;
        } else if (us < 1) {
            us = 1;
        } else {
            // Do Nothing
        }

        if (RZ_A1_IsClockMode0() == false) {
            pclk_base = (uint32_t)CM1_RENESAS_RZ_A1_P0_CLK / 10000;
        } else {
            pclk_base = (uint32_t)CM0_RENESAS_RZ_A1_P0_CLK / 10000;
        }

        wk_cycle = pclk_base * us;
        while (wk_cycle >= 102350) {
            wk_cycle >>= 1;
            wk_cks++;
        }
        wk_cycle = (wk_cycle + 50) / 100;

        if (obj->ch == 2) {
            wk_last_cycle    = PWMPWCYR_2 & 0x03ff;
            PWMPWCR_2        = 0xc0 | wk_cks;
            PWMPWCYR_2       = (uint16_t)wk_cycle;

            // Set duty again
            set_duty_again(&PWMPWBFR_2A, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_2C, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_2E, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_2G, wk_last_cycle, wk_cycle);

            // Counter Start
            PWMPWCR_2 |= 0x08;

            // Save for future use
            period_ch2 = us;
        } else {
            wk_last_cycle    = PWMPWCYR_1 & 0x03ff;
            PWMPWCR_1        = 0xc0 | wk_cks;
            PWMPWCYR_1       = (uint16_t)wk_cycle;

            // Set duty again
            set_duty_again(&PWMPWBFR_1A, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_1C, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_1E, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_1G, wk_last_cycle, wk_cycle);

            // Counter Start
            PWMPWCR_1 |= 0x08;

            // Save for future use
            period_ch1 = us;
        }
#endif
    }
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    float value = 0;

    if (obj->pwm >= MTU2_PWM_OFFSET) {
#ifdef FUMC_MTU2_PWM
        /* PWM by MTU2 */
        if (mtu2_period_ch[obj->ch] != 0) {
            value = (float)us / (float)mtu2_period_ch[obj->ch];
        }
#endif
    } else {
#ifdef FUNC_MOTOR_CTL_PWM
        /* PWM */
        if (obj->ch == 2) {
            if (period_ch2 != 0) {
                value = (float)us / (float)period_ch2;
            }
        } else {
            if (period_ch1 != 0) {
                value = (float)us / (float)period_ch1;
            }
        }
#endif
    }
    pwmout_write(obj, value);
}
