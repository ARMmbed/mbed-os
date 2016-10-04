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
#include "RZ_A1_Init.h"
#include "cpg_iodefine.h"
#include "pwm_iodefine.h"
#include "gpio_addrdefine.h"

#ifdef MAX_PERI
#define MTU2_PWM_NUM            27
#define MTU2_PWM_SIGNAL         2
#define MTU2_PWM_OFFSET         0x20

//  PORT ID, PWM ID, Pin function
static const PinMap PinMap_PWM[] = {
    // TIOC0 A,C
    {P4_0     , MTU2_PWM0_PIN  , 2},	//TIOC0A
    {P5_0     , MTU2_PWM1_PIN  , 6},	//TIOC0A
    {P7_0     , MTU2_PWM2_PIN  , 7},	//TIOC0A
    {P4_2     , MTU2_PWM3_PIN  , 2},	//TIOC0C
    {P5_5     , MTU2_PWM4_PIN  , 6},	//TIOC0C
    {P7_2     , MTU2_PWM5_PIN  , 7},	//TIOC0C
    //TIOC1 A
    {P2_11    , MTU2_PWM6_PIN  , 5},	//TIOC1A
    {P6_0     , MTU2_PWM7_PIN  , 5},	//TIOC1A
    {P7_4     , MTU2_PWM8_PIN  , 7},	//TIOC1A
    {P8_8     , MTU2_PWM9_PIN  , 5},	//TIOC1A
    {P9_7     , MTU2_PWM10_PIN , 4},	//TIOC1A
    //TIOC2 A
    {P2_1     , MTU2_PWM11_PIN , 6},	//TIOC2A
    {P6_2     , MTU2_PWM12_PIN , 6},	//TIOC2A
    {P7_6     , MTU2_PWM13_PIN , 7},	//TIOC2A
    {P8_14    , MTU2_PWM14_PIN , 4},	//TIOC2A
    //TIOC3 A,C
    {P3_4     , MTU2_PWM15_PIN , 6},	//TIOC3A
    {P7_8     , MTU2_PWM16_PIN , 7},	//TIOC3A
    {P8_10    , MTU2_PWM17_PIN , 4},	//TIOC3A
    {P3_6     , MTU2_PWM18_PIN , 6},	//TIOC3C
    {P7_10    , MTU2_PWM19_PIN , 7},	//TIOC3C
    {P8_12    , MTU2_PWM20_PIN , 4},	//TIOC3C
    //TIOC4 A,C
    {P3_8     , MTU2_PWM21_PIN , 6},	//TIOC4A
    {P4_4     , MTU2_PWM22_PIN , 3},	//TIOC4A
    {P7_12    , MTU2_PWM23_PIN , 7},	//TIOC4A
    {P3_10    , MTU2_PWM24_PIN , 6},	//TIOC4C
    {P4_6     , MTU2_PWM25_PIN , 3},	//TIOC4C
    {P7_14    , MTU2_PWM26_PIN , 7},	//TIOC4C
    //PWM1
    {P8_8     , PWM0_PIN       , 6},	//PWM1A
    {P8_9     , PWM1_PIN       , 6},	//PWM1B
    {P8_10    , PWM2_PIN       , 6},	//PWM1C
    {P8_11    , PWM3_PIN       , 6},	//PWM1D
    {P8_12    , PWM4_PIN       , 6},	//PWM1E
    {P8_13    , PWM5_PIN       , 6},	//PWM1F
    {P8_14    , PWM6_PIN       , 6},	//PWM1G
    {P8_15    , PWM7_PIN       , 6},	//PWM1H
    //PWM2
    {P3_0     , PWM8_PIN       , 7},	//PWM2A
    {P3_1     , PWM9_PIN       , 7},	//PWM2B
    {P3_2     , PWM10_PIN      , 7},	//PWM2C
    {P3_3     , PWM11_PIN      , 7},	//PWM2D
    {P4_4     , PWM12_PIN      , 4},	//PWM2E
    {P4_5     , PWM13_PIN      , 4},	//PWM2F
    {P4_6     , PWM14_PIN      , 4},	//PWM2G
    {P4_7     , PWM15_PIN      , 4},	//PWM2H
    {NC       , NC             , 0}
};

static const PWMType PORT[] = {
    PWM1A,          // PWM0_PIN
    PWM1B,          // PWM1_PIN
    PWM1C,          // PWM2_PIN
    PWM1D,          // PWM3_PIN
    PWM1E,          // PWM4_PIN
    PWM1F,          // PWM5_PIN
    PWM1G,          // PWM6_PIN
    PWM1H,          // PWM7_PIN
    PWM2A,          // PWM8_PIN
    PWM2B,          // PWM9_PIN
    PWM2C,          // PWM10_PIN
    PWM2D,          // PWM11_PIN
    PWM2E,          // PWM12_PIN
    PWM2F,          // PWM13_PIN
    PWM2G,          // PWM14_PIN
    PWM2H,          // PWM15_PIN
};

static const MTU2_PWMType MTU2_PORT[] = {
    TIOC0A,         // MTU2_PWM0_PIN
    TIOC0A,         // MTU2_PWM1_PIN
    TIOC0A,         // MTU2_PWM2_PIN
    TIOC0C,         // MTU2_PWM3_PIN
    TIOC0C,         // MTU2_PWM4_PIN
    TIOC0C,         // MTU2_PWM5_PIN
    TIOC1A,         // MTU2_PWM6_PIN
    TIOC1A,         // MTU2_PWM7_PIN
    TIOC1A,         // MTU2_PWM8_PIN
    TIOC1A,         // MTU2_PWM9_PIN
    TIOC1A,         // MTU2_PWM10_PIN
    TIOC2A,         // MTU2_PWM11_PIN
    TIOC2A,         // MTU2_PWM12_PIN
    TIOC2A,         // MTU2_PWM13_PIN
    TIOC2A,         // MTU2_PWM14_PIN
    TIOC3A,         // MTU2_PWM15_PIN
    TIOC3A,         // MTU2_PWM16_PIN
    TIOC3A,         // MTU2_PWM17_PIN
    TIOC3C,         // MTU2_PWM18_PIN
    TIOC3C,         // MTU2_PWM19_PIN
    TIOC3C,         // MTU2_PWM20_PIN
    TIOC4A,         // MTU2_PWM21_PIN
    TIOC4A,         // MTU2_PWM22_PIN
    TIOC4A,         // MTU2_PWM23_PIN
    TIOC4C,         // MTU2_PWM24_PIN
    TIOC4C,         // MTU2_PWM25_PIN
    TIOC4C,         // MTU2_PWM26_PIN
};

static __IO uint16_t *PWM_MATCH[] = {
    &PWMPWBFR_1A,    // PWM0_PIN
    &PWMPWBFR_1A,    // PWM1_PIN
    &PWMPWBFR_1C,    // PWM2_PIN
    &PWMPWBFR_1C,    // PWM3_PIN
    &PWMPWBFR_1E,    // PWM4_PIN
    &PWMPWBFR_1E,    // PWM5_PIN
    &PWMPWBFR_1G,    // PWM6_PIN
    &PWMPWBFR_1G,    // PWM7_PIN
    &PWMPWBFR_2A,    // PWM8_PIN
    &PWMPWBFR_2A,    // PWM9_PIN
    &PWMPWBFR_2C,    // PWM10_PIN
    &PWMPWBFR_2C,    // PWM11_PIN
    &PWMPWBFR_2E,    // PWM12_PIN
    &PWMPWBFR_2E,    // PWM13_PIN
    &PWMPWBFR_2G,    // PWM14_PIN
    &PWMPWBFR_2G,    // PWM15_PIN
};

static __IO uint16_t *MTU2_PWM_MATCH[MTU2_PWM_NUM][MTU2_PWM_SIGNAL] = {
    { &MTU2TGRA_0, &MTU2TGRB_0 }		// MTU2_PWM0_PIN
    { &MTU2TGRA_0, &MTU2TGRB_0 }		// MTU2_PWM1_PIN
    { &MTU2TGRA_0, &MTU2TGRB_0 }		// MTU2_PWM2_PIN
    { &MTU2TGRC_0, &MTU2TGRD_0 }		// MTU2_PWM3_PIN
    { &MTU2TGRC_0, &MTU2TGRD_0 }		// MTU2_PWM4_PIN
    { &MTU2TGRC_0, &MTU2TGRD_0 }		// MTU2_PWM5_PIN
    { &MTU2TGRA_1, &MTU2TGRB_1 }		// MTU2_PWM6_PIN
    { &MTU2TGRA_1, &MTU2TGRB_1 }		// MTU2_PWM7_PIN
    { &MTU2TGRA_1, &MTU2TGRB_1 }		// MTU2_PWM8_PIN
    { &MTU2TGRA_1, &MTU2TGRB_1 }		// MTU2_PWM9_PIN
    { &MTU2TGRA_1, &MTU2TGRB_1 }		// MTU2_PWM10_PIN
    { &MTU2TGRA_2, &MTU2TGRB_2 }		// MTU2_PWM11_PIN
    { &MTU2TGRA_2, &MTU2TGRB_2 }		// MTU2_PWM12_PIN
    { &MTU2TGRA_2, &MTU2TGRB_2 }		// MTU2_PWM13_PIN
    { &MTU2TGRA_2, &MTU2TGRB_2 }		// MTU2_PWM14_PIN
    { &MTU2TGRA_3, &MTU2TGRB_3 }		// MTU2_PWM15_PIN
    { &MTU2TGRA_3, &MTU2TGRB_3 }		// MTU2_PWM16_PIN
    { &MTU2TGRA_3, &MTU2TGRB_3 }		// MTU2_PWM17_PIN
    { &MTU2TGRC_3, &MTU2TGRD_3 }		// MTU2_PWM18_PIN
    { &MTU2TGRC_3, &MTU2TGRD_3 }		// MTU2_PWM19_PIN
    { &MTU2TGRC_3, &MTU2TGRD_3 }		// MTU2_PWM20_PIN
    { &MTU2TGRA_4, &MTU2TGRB_2 }		// MTU2_PWM21_PIN
    { &MTU2TGRA_4, &MTU2TGRB_2 }		// MTU2_PWM22_PIN
    { &MTU2TGRA_4, &MTU2TGRB_2 }		// MTU2_PWM23_PIN
    { &MTU2TGRC_4, &MTU2TGRD_4 } 		// MTU2_PWM24_PIN
    { &MTU2TGRC_4, &MTU2TGRD_4 }		// MTU2_PWM25_PIN
    { &MTU2TGRC_4, &MTU2TGRD_4 }		// MTU2_PWM26_PIN
};
#else
#define MTU2_PWM_NUM            12
#define MTU2_PWM_SIGNAL         2
#define MTU2_PWM_OFFSET         0x20

//  PORT ID, PWM ID, Pin function
static const PinMap PinMap_PWM[] = {
    //TIOC0 A,C
    {P4_0     , MTU2_PWM0_PIN  , 2},	//TIOC0A
    {P5_0     , MTU2_PWM1_PIN  , 6},	//TIOC0A
    {P4_2     , MTU2_PWM2_PIN  , 2},	//TIOC0C
    {P5_5     , MTU2_PWM3_PIN  , 6},	//TIOC0C
    //TIOC2 A
    {P8_14    , MTU2_PWM4_PIN  , 4},	//TIOC2A
    //TIOC3 A,C
    {P8_10    , MTU2_PWM5_PIN  , 4},	//TIOC3A
    {P5_3     , MTU2_PWM6_PIN  , 6}, 	//TIOC3C
    {P8_12    , MTU2_PWM7_PIN  , 4},	//TIOC3C
    //TIOC4 A,C
    {P3_8     , MTU2_PWM8_PIN  , 6},	//TIOC4A
    {P4_4     , MTU2_PWM9_PIN  , 3},	//TIOC4A
    {P3_10    , MTU2_PWM10_PIN , 6},	//TIOC4C
    {P4_6     , MTU2_PWM11_PIN , 3},	//TIOC4C
    //PWM1
    {P8_10    , PWM0_PIN       , 6},	//PWM1C
    {P8_11    , PWM1_PIN       , 6},	//PWM1D
    {P8_12    , PWM2_PIN       , 6},	//PWM1E
    {P8_13    , PWM3_PIN       , 6},	//PWM1F
    {P8_14    , PWM4_PIN       , 6},	//PWM1G
    {P8_15    , PWM5_PIN       , 6},	//PWM1H
    //PWM2
    {P3_0     , PWM6_PIN       , 7},	//PWM2A
    {P3_1     , PWM7_PIN       , 7},	//PWM2B
    {P3_2     , PWM8_PIN       , 7},	//PWM2C
    {P4_4     , PWM9_PIN       , 4},	//PWM2E
    {P4_5     , PWM10_PIN      , 4},	//PWM2F
    {P4_6     , PWM11_PIN      , 4},	//PWM2G
    {P4_7     , PWM12_PIN      , 4},	//PWM2H
    {NC       , NC             , 0}
};

static const PWMType PORT[] = {
    PWM1C,          // PWM0_PIN
    PWM1D,          // PWM1_PIN
    PWM1E,          // PWM2_PIN
    PWM1F,          // PWM3_PIN
    PWM1G,          // PWM4_PIN
    PWM1H,          // PWM5_PIN
    PWM2A,          // PWM6_PIN
    PWM2B,          // PWM7_PIN
    PWM2C,          // PWM8_PIN
    PWM2E,          // PWM9_PIN
    PWM2F,          // PWM10_PIN
    PWM2G,          // PWM11_PIN
    PWM2H,          // PWM12_PIN
};

static const MTU2_PWMType MTU2_PORT[] = {
    TIOC0A,         // MTU2_PWM0_PIN
    TIOC0A,         // MTU2_PWM1_PIN
    TIOC0C,         // MTU2_PWM2_PIN
    TIOC0C,         // MTU2_PWM3_PIN
    TIOC2A,         // MTU2_PWM4_PIN
    TIOC3A,         // MTU2_PWM5_PIN
    TIOC3C,         // MTU2_PWM6_PIN
    TIOC3C,         // MTU2_PWM7_PIN
    TIOC4A,         // MTU2_PWM8_PIN
    TIOC4A,         // MTU2_PWM9_PIN
    TIOC4C,         // MTU2_PWM10_PIN
    TIOC4C,         // MTU2_PWM11_PIN
};

static __IO uint16_t *PWM_MATCH[] = {
    &PWMPWBFR_1C,    // PWM0_PIN
    &PWMPWBFR_1C,    // PWM1_PIN
    &PWMPWBFR_1E,    // PWM2_PIN
    &PWMPWBFR_1E,    // PWM3_PIN
    &PWMPWBFR_1G,    // PWM4_PIN
    &PWMPWBFR_1G,    // PWM5_PIN
    &PWMPWBFR_2A,    // PWM6_PIN
    &PWMPWBFR_2A,    // PWM7_PIN
    &PWMPWBFR_2C,    // PWM8_PIN
    &PWMPWBFR_2E,    // PWM9_PIN
    &PWMPWBFR_2E,    // PWM10_PIN
    &PWMPWBFR_2G,    // PWM11_PIN
    &PWMPWBFR_2G,    // PWM12_PIN
};

static __IO uint16_t *MTU2_PWM_MATCH[MTU2_PWM_NUM][MTU2_PWM_SIGNAL] = {
    { &MTU2TGRA_0, &MTU2TGRB_0 },		// MTU2_PWM0_PIN
    { &MTU2TGRA_0, &MTU2TGRB_0 },		// MTU2_PWM1_PIN
    { &MTU2TGRC_0, &MTU2TGRD_0 },		// MTU2_PWM2_PIN
    { &MTU2TGRC_0, &MTU2TGRD_0 },		// MTU2_PWM3_PIN
    { &MTU2TGRA_2, &MTU2TGRB_2 },		// MTU2_PWM4_PIN
    { &MTU2TGRA_3, &MTU2TGRB_3 },		// MTU2_PWM5_PIN
    { &MTU2TGRC_3, &MTU2TGRD_3 },		// MTU2_PWM6_PIN
    { &MTU2TGRC_3, &MTU2TGRD_3 },		// MTU2_PWM7_PIN
    { &MTU2TGRA_4, &MTU2TGRB_2 },		// MTU2_PWM8_PIN
    { &MTU2TGRA_4, &MTU2TGRB_2 },		// MTU2_PWM9_PIN
    { &MTU2TGRC_4, &MTU2TGRD_4 },		// MTU2_PWM10_PIN
    { &MTU2TGRC_4, &MTU2TGRD_4 },		// MTU2_PWM11_PIN
};
#endif


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
    MODE_PWM = 0,
    MODE_MTU2
} PWMmode;

typedef enum {
    MTU2_PULSE = 0,
    MTU2_PERIOD
} MTU2Signal;

static int pwm_mode = MODE_PWM;
static uint16_t init_period_ch1 = 0;
static uint16_t init_period_ch2 = 0;
static uint16_t init_mtu2_period_ch[5] = {0};
static int32_t  period_ch1 = 1;
static int32_t  period_ch2 = 1;
static int32_t  mtu2_period_ch[5] = {1, 1, 1, 1, 1};

void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    if (pwm >= MTU2_PWM_OFFSET) {
        /* PWM by MTU2 */
        int tmp_pwm;
        
        pwm_mode = MODE_MTU2;
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
    } else {
        /* PWM */
        pwm_mode = MODE_PWM;
        // power on
        CPGSTBCR3 &= ~(CPG_STBCR3_BIT_MSTP30);

        obj->pwm = pwm;
        if (((uint32_t)PORT[obj->pwm] & 0x00000010) == 0x00000010) {
            obj->ch  = 2;
            PWMPWPR_2_BYTE_L = 0x00;
        } else {
            obj->ch  = 1;
            PWMPWPR_1_BYTE_L = 0x00;
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
    }
}

void pwmout_free(pwmout_t* obj) {
    pwmout_write(obj, 0);
}

void pwmout_write(pwmout_t* obj, float value) {
    uint32_t wk_cycle;
    uint16_t v;

    if (pwm_mode == MODE_MTU2) {
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
        *MTU2_PWM_MATCH[tmp_pwm][MTU2_PULSE] = (uint16_t)((float)wk_cycle * value);
    } else {
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
    }
}

float pwmout_read(pwmout_t* obj) {
    uint32_t wk_cycle;
    float value;

    if (pwm_mode == MODE_MTU2) {
        /* PWM by MTU2 */
        uint32_t wk_pulse;
        int tmp_pwm;
        
        tmp_pwm = (int)(obj->pwm - MTU2_PWM_OFFSET);
        wk_cycle = *MTU2_PWM_MATCH[tmp_pwm][MTU2_PERIOD] & 0xffff;
        wk_pulse = *MTU2_PWM_MATCH[tmp_pwm][MTU2_PULSE] & 0xffff;
        value = ((float)wk_pulse / (float)wk_cycle);
    } else {
        /* PWM */
        if (obj->ch == 2) {
            wk_cycle = PWMPWCYR_2 & 0x03ff;
        } else {
            wk_cycle = PWMPWCYR_1 & 0x03ff;
        }
        value = ((float)(*PWM_MATCH[obj->pwm] & 0x03ff) / (float)wk_cycle);
    }

    return (value > 1.0f) ? (1.0f) : (value);
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

static void set_duty_again(__IO uint16_t *p_pwmpbfr, uint16_t last_cycle, uint16_t new_cycle){
    uint16_t wk_pwmpbfr;
    float    value;
    uint16_t v;

    wk_pwmpbfr = *p_pwmpbfr;
    value      = ((float)(wk_pwmpbfr & 0x03ff) / (float)last_cycle);
    v          = (uint16_t)((float)new_cycle * value);
    *p_pwmpbfr = (v | (wk_pwmpbfr & 0x1000));
}

static void set_mtu2_duty_again(__IO uint16_t *p_pwmpbfr, uint16_t last_cycle, uint16_t new_cycle){
    uint16_t wk_pwmpbfr;
    float    value;

    wk_pwmpbfr = *p_pwmpbfr;
    value      = ((float)(wk_pwmpbfr & 0xffff) / (float)last_cycle);
    *p_pwmpbfr = (uint16_t)((float)new_cycle * value);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
    uint64_t wk_cycle_mtu2;
    uint32_t pclk_base;
    uint32_t wk_cycle;
    uint32_t wk_cks = 0;
    uint16_t wk_last_cycle;
    int      max_us = 0;

    if (pwm_mode == MODE_MTU2) {
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
    } else {
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
            PWMPWCR_2_BYTE_L = 0xc0 | wk_cks;
            PWMPWCYR_2       = (uint16_t)wk_cycle;

            // Set duty again
            set_duty_again(&PWMPWBFR_2A, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_2C, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_2E, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_2G, wk_last_cycle, wk_cycle);

            // Counter Start
            PWMPWCR_2_BYTE_L |= 0x08;

            // Save for future use
            period_ch2 = us;
        } else {
            wk_last_cycle    = PWMPWCYR_1 & 0x03ff;
            PWMPWCR_1_BYTE_L = 0xc0 | wk_cks;
            PWMPWCYR_1       = (uint16_t)wk_cycle;

            // Set duty again
            set_duty_again(&PWMPWBFR_1A, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_1C, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_1E, wk_last_cycle, wk_cycle);
            set_duty_again(&PWMPWBFR_1G, wk_last_cycle, wk_cycle);

            // Counter Start
            PWMPWCR_1_BYTE_L |= 0x08;

            // Save for future use
            period_ch1 = us;
        }
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

    if (pwm_mode == MODE_MTU2) {
        /* PWM by MTU2 */
        if (mtu2_period_ch[obj->ch] != 0) {
            value = (float)us / (float)mtu2_period_ch[obj->ch];
        }
    } else {
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
    }
    pwmout_write(obj, value);
}
