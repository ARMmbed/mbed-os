/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#include "drivers/PwmOut.h"


#if DEVICE_PWMOUT

#include "platform/mbed_critical.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_assert.h"

namespace mbed {

PwmOut::PwmOut(PinName pin) :
    _pin(pin),
    _deep_sleep_locked(false),
    _initialized(false),
    _duty_cycle(0)
{
    PwmOut::init();
}

PwmOut::PwmOut(const PinMap &pinmap) : _deep_sleep_locked(false)
{
    core_util_critical_section_enter();
    pwmout_init_direct(&_pwm, &pinmap);
    core_util_critical_section_exit();
}

PwmOut::~PwmOut()
{
    PwmOut::deinit();
}

void PwmOut::write(float value)
{
    core_util_critical_section_enter();
    pwmout_write(&_pwm, value);
    core_util_critical_section_exit();
}

float PwmOut::read()
{
    core_util_critical_section_enter();
    float val = pwmout_read(&_pwm);
    core_util_critical_section_exit();
    return val;
}

void PwmOut::period(float seconds)
{
    core_util_critical_section_enter();
    pwmout_period(&_pwm, seconds);
    core_util_critical_section_exit();
}

void PwmOut::period_ms(int ms)
{
    core_util_critical_section_enter();
    pwmout_period_ms(&_pwm, ms);
    core_util_critical_section_exit();
}

void PwmOut::period_us(int us)
{
    core_util_critical_section_enter();
    pwmout_period_us(&_pwm, us);
    core_util_critical_section_exit();
}

void PwmOut::pulsewidth(float seconds)
{
    core_util_critical_section_enter();
    pwmout_pulsewidth(&_pwm, seconds);
    core_util_critical_section_exit();
}

void PwmOut::pulsewidth_ms(int ms)
{
    core_util_critical_section_enter();
    pwmout_pulsewidth_ms(&_pwm, ms);
    core_util_critical_section_exit();
}

void PwmOut::pulsewidth_us(int us)
{
    core_util_critical_section_enter();
    pwmout_pulsewidth_us(&_pwm, us);
    core_util_critical_section_exit();
}

void PwmOut::suspend()
{
    core_util_critical_section_enter();
    if (_initialized) {
        _duty_cycle = PwmOut::read();
        PwmOut::deinit();
    }
    core_util_critical_section_exit();
}

void PwmOut::resume()
{
    core_util_critical_section_enter();
    if (!_initialized) {
        PwmOut::init();
        PwmOut::write(_duty_cycle);
    }
    core_util_critical_section_exit();
}

void PwmOut::lock_deep_sleep()
{
    if (_deep_sleep_locked == false) {
        sleep_manager_lock_deep_sleep();
        _deep_sleep_locked = true;
    }
}

void PwmOut::unlock_deep_sleep()
{
    if (_deep_sleep_locked == true) {
        sleep_manager_unlock_deep_sleep();
        _deep_sleep_locked = false;
    }
}

void PwmOut::init()
{
    core_util_critical_section_enter();

    if (!_initialized) {
        pwmout_init(&_pwm, _pin);
        lock_deep_sleep();
        _initialized = true;
    }

    core_util_critical_section_exit();
}

void PwmOut::deinit()
{
    core_util_critical_section_enter();

    if (_initialized) {
        pwmout_free(&_pwm);
        unlock_deep_sleep();
        _initialized = false;
    }

    core_util_critical_section_exit();
}

} // namespace mbed

#endif // #if DEVICE_PWMOUT
