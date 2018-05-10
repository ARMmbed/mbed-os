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
#include "pinmap.h"
#include "objects.h"
#include "emscripten.h"

void pwmout_init(pwmout_t* obj, PinName pin) {
    obj->pin = pin;

    EM_ASM_({
        MbedJSHal.gpio.init_pwmout($0, $1, $2, $3);
    }, obj, obj->pin, 20 /* period_ms */, 0);
}

void pwmout_free(pwmout_t* obj) {
    // [TODO]
}

void pwmout_write(pwmout_t* obj, float value) {
    EM_ASM_({
        MbedJSHal.gpio.write($0, $1);
    }, obj->pin, (int)(value * 1024.0f));
}

float pwmout_read(pwmout_t* obj) {
    int v = EM_ASM_({
        return MbedJSHal.gpio.read($0);
    }, obj->pin);

    return ((float)v) / 1024.0f;
}

void pwmout_period(pwmout_t* obj, float seconds) {
    EM_ASM_({
        MbedJSHal.gpio.period_ms($0, $1);
    }, obj->pin, (int)(seconds * 1000.0f));
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    EM_ASM_({
        MbedJSHal.gpio.period_ms($0, $1);
    }, obj->pin, ms);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
    EM_ASM_({
        MbedJSHal.gpio.period_ms($0, $1);
    }, obj->pin, us / 1000);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    EM_ASM_({
        MbedJSHal.gpio.pulsewidth_ms($0, $1);
    }, obj->pin, (int)(seconds * 1000.0f));
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    EM_ASM_({
        MbedJSHal.gpio.pulsewidth_ms($0, $1);
    }, obj->pin, ms);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    EM_ASM_({
        MbedJSHal.gpio.pulsewidth_ms($0, $1);
    }, obj->pin, us / 1000);
}
