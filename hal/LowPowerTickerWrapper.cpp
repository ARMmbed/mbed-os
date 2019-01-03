/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "hal/LowPowerTickerWrapper.h"
#include "platform/Callback.h"

LowPowerTickerWrapper::LowPowerTickerWrapper(const ticker_data_t *data, const ticker_interface_t *interface, uint32_t min_cycles_between_writes, uint32_t min_cycles_until_match)
    : _intf(data->interface), _min_count_between_writes(min_cycles_between_writes + 1), _min_count_until_match(min_cycles_until_match + 1), _suspended(false)
{
    core_util_critical_section_enter();

    this->data.interface = interface;
    this->data.queue = data->queue;
    _reset();

    core_util_critical_section_exit();
}

void LowPowerTickerWrapper::irq_handler(ticker_irq_handler_type handler)
{
    core_util_critical_section_enter();

    // This code no longer filters out early interrupts. Instead it
    // passes them through to the next layer and ignores further interrupts
    // until the next call to set_interrrupt or fire_interrupt (when not suspended).
    // This is to ensure that the device doesn't get stuck in sleep due to an
    // early low power ticker interrupt that was ignored.
    if (_pending_fire_now || _pending_match || _suspended) {
        _timeout.detach();
        _pending_timeout = false;
        _pending_match = false;
        _pending_fire_now = false;
        if (handler) {
            handler(&data);
        }
    } else {
        // Spurious interrupt
        _intf->clear_interrupt();
    }

    core_util_critical_section_exit();
}

void LowPowerTickerWrapper::suspend()
{
    core_util_critical_section_enter();

    // Wait until rescheduling is allowed
    while (!_set_interrupt_allowed) {
        timestamp_t current = _intf->read();
        if (((current - _last_actual_set_interrupt) & _mask) >= _min_count_between_writes) {
            _set_interrupt_allowed  = true;
        }
    }

    _reset();
    _suspended = true;

    core_util_critical_section_exit();
}

void LowPowerTickerWrapper::resume()
{
    core_util_critical_section_enter();

    // Wait until rescheduling is allowed
    while (!_set_interrupt_allowed) {
        timestamp_t current = _intf->read();
        if (((current - _last_actual_set_interrupt) & _mask) >= _min_count_between_writes) {
            _set_interrupt_allowed  = true;
        }
    }

    _suspended = false;

    core_util_critical_section_exit();
}

bool LowPowerTickerWrapper::timeout_pending()
{
    core_util_critical_section_enter();

    bool pending = _pending_timeout;

    core_util_critical_section_exit();
    return pending;
}

void LowPowerTickerWrapper::init()
{
    core_util_critical_section_enter();

    _reset();
    _intf->init();

    core_util_critical_section_exit();
}

void LowPowerTickerWrapper::free()
{
    core_util_critical_section_enter();

    _reset();
    _intf->free();

    core_util_critical_section_exit();
}

uint32_t LowPowerTickerWrapper::read()
{
    core_util_critical_section_enter();

    timestamp_t current = _intf->read();
    if (!_suspended && _match_check(current)) {
        _intf->fire_interrupt();
    }

    core_util_critical_section_exit();
    return current;
}

void LowPowerTickerWrapper::set_interrupt(timestamp_t timestamp)
{
    core_util_critical_section_enter();

    _last_set_interrupt = _intf->read();
    _cur_match_time = timestamp;
    _pending_match = true;
    if (!_suspended) {
        _schedule_match(_last_set_interrupt);
    } else {
        _intf->set_interrupt(timestamp);
        _last_actual_set_interrupt = _last_set_interrupt;
        _set_interrupt_allowed = false;
    }

    core_util_critical_section_exit();
}

void LowPowerTickerWrapper::disable_interrupt()
{
    core_util_critical_section_enter();

    _intf->disable_interrupt();

    core_util_critical_section_exit();
}

void LowPowerTickerWrapper::clear_interrupt()
{
    core_util_critical_section_enter();

    _intf->clear_interrupt();

    core_util_critical_section_exit();
}

void LowPowerTickerWrapper::fire_interrupt()
{
    core_util_critical_section_enter();

    _pending_fire_now = 1;
    _intf->fire_interrupt();

    core_util_critical_section_exit();
}

const ticker_info_t *LowPowerTickerWrapper::get_info()
{

    core_util_critical_section_enter();

    const ticker_info_t *info = _intf->get_info();

    core_util_critical_section_exit();
    return info;
}

void LowPowerTickerWrapper::_reset()
{
    MBED_ASSERT(core_util_in_critical_section());

    _timeout.detach();
    _pending_timeout = false;
    _pending_match = false;
    _pending_fire_now = false;
    _set_interrupt_allowed = true;
    _cur_match_time = 0;
    _last_set_interrupt = 0;
    _last_actual_set_interrupt = 0;

    const ticker_info_t *info = _intf->get_info();
    if (info->bits >= 32) {
        _mask = 0xffffffff;
    } else {
        _mask = ((uint64_t)1 << info->bits) - 1;
    }

    // Round us_per_tick up
    _us_per_tick = (1000000 + info->frequency - 1) / info->frequency;
}

void LowPowerTickerWrapper::_timeout_handler()
{
    core_util_critical_section_enter();
    _pending_timeout = false;

    timestamp_t current = _intf->read();
    /* Add extra check for '_last_set_interrupt == _cur_match_time'
     *
     * When '_last_set_interrupt == _cur_match_time', _ticker_match_interval_passed sees it as
     * one-round interval rather than just-pass, so add extra check for it. In rare cases, we
     * may trap in _timeout_handler/_schedule_match loop. This check can break it.
     */
    if ((_last_set_interrupt == _cur_match_time) ||
            _ticker_match_interval_passed(_last_set_interrupt, current, _cur_match_time)) {
        _intf->fire_interrupt();
    } else {
        _schedule_match(current);
    }

    core_util_critical_section_exit();
}

bool LowPowerTickerWrapper::_match_check(timestamp_t current)
{
    MBED_ASSERT(core_util_in_critical_section());

    if (!_pending_match) {
        return false;
    }
    /* Add extra check for '_last_set_interrupt == _cur_match_time' as above */
    return (_last_set_interrupt == _cur_match_time) ||
           _ticker_match_interval_passed(_last_set_interrupt, current, _cur_match_time);
}

uint32_t LowPowerTickerWrapper::_lp_ticks_to_us(uint32_t ticks)
{
    MBED_ASSERT(core_util_in_critical_section());

    // Add 4 microseconds to round up the micro second ticker time (which has a frequency of at least 250KHz - 4us period)
    return _us_per_tick * ticks + 4;
}

void LowPowerTickerWrapper::_schedule_match(timestamp_t current)
{
    MBED_ASSERT(core_util_in_critical_section());

    // Check if _intf->set_interrupt is allowed
    if (!_set_interrupt_allowed) {
        if (((current - _last_actual_set_interrupt) & _mask) >= _min_count_between_writes) {
            _set_interrupt_allowed  = true;
        }
    }

    uint32_t cycles_until_match = (_cur_match_time - current) & _mask;
    bool too_close = cycles_until_match < _min_count_until_match;

    if (!_set_interrupt_allowed) {

        // Can't use _intf->set_interrupt so use microsecond Timeout instead

        // Speed optimization - if a timer has already been scheduled
        // then don't schedule it again.
        if (!_pending_timeout) {
            uint32_t ticks = cycles_until_match < _min_count_until_match ? cycles_until_match : _min_count_until_match;
            _timeout.attach_us(mbed::callback(this, &LowPowerTickerWrapper::_timeout_handler), _lp_ticks_to_us(ticks));
            _pending_timeout = true;
        }
        return;
    }

    if (!too_close) {

        // Schedule LP ticker
        _intf->set_interrupt(_cur_match_time);
        current = _intf->read();
        _last_actual_set_interrupt = current;
        _set_interrupt_allowed = false;

        // Check for overflow
        uint32_t new_cycles_until_match = (_cur_match_time - current) & _mask;
        if (new_cycles_until_match > cycles_until_match) {
            // Overflow so fire now
            _intf->fire_interrupt();
            return;
        }

        // Update variables with new time
        cycles_until_match = new_cycles_until_match;
        too_close = cycles_until_match < _min_count_until_match;
    }

    if (too_close) {

        // Low power ticker incremented to less than _min_count_until_match
        // so low power ticker may not fire. Use Timeout to ensure it does fire.
        uint32_t ticks = cycles_until_match < _min_count_until_match ? cycles_until_match : _min_count_until_match;
        _timeout.attach_us(mbed::callback(this, &LowPowerTickerWrapper::_timeout_handler), _lp_ticks_to_us(ticks));
        _pending_timeout = true;
        return;
    }
}
