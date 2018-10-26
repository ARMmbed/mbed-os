
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_LOW_POWER_TICKER_WRAPPER_H
#define MBED_LOW_POWER_TICKER_WRAPPER_H

#include "device.h"

#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "drivers/Timeout.h"


class LowPowerTickerWrapper {
public:


    /**
     * Create a new wrapped low power ticker object
     *
     * @param data Low power ticker data to wrap
     * @param interface new ticker interface functions
     * @param min_cycles_between_writes The number of whole low power clock periods
     * which must complete before subsequent calls to set_interrupt
     * @param min_cycles_until_match The minimum number of whole low power clock periods
     * from the current time for which the match timestamp passed to set_interrupt is
     * guaranteed to fire.
     *
     *  N = min_cycles_between_writes
     *
     *       0       1             N - 1     N     N + 1   N + 2   N + 3
     *       |-------|------...------|-------|-------|-------|-------|
     *           ^                                    ^
     *           |                                    |
     *       set_interrupt                   Next set_interrupt allowed
     *
     * N = min_cycles_until_match
     *
     *      0       1             N - 1     N     N + 1   N + 2   N + 3
     *      |-------|------...------|-------|-------|-------|-------|
     *          ^                                   ^
     *          |                                   |
     *      set_interrupt              Earliest match timestamp allowed
     *
     *
     */

    LowPowerTickerWrapper(const ticker_data_t *data, const ticker_interface_t *interface, uint32_t min_cycles_between_writes, uint32_t min_cycles_until_match);

    /**
     * Interrupt handler called by the underlying driver/hardware
     *
     * @param handler The callback which would normally be called by the underlying driver/hardware
     */
    void irq_handler(ticker_irq_handler_type handler);

    /**
     * Suspend wrapper operation and pass through interrupts.
     *
     * This stops to wrapper layer from using the microsecond ticker.
     * This should be called before using the low power ticker APIs directly.
     *
     * @warning: Make sure to suspend the LP ticker first (call ticker_suspend()),
     * otherwise the behavior is undefined.
     */
    void suspend();

    /**
     * Resume wrapper operation and filter interrupts normally
     */
    void resume();

    /**
     * Check if a Timeout object is being used
     *
     * @return true if Timeout is used for scheduling false otherwise
     */
    bool timeout_pending();

    /*
     * Implementation of ticker_init
     */
    void init();

    /*
     * Implementation of free
     */
    void free();

    /*
     * Implementation of read
     */
    uint32_t read();

    /*
     * Implementation of set_interrupt
     */
    void set_interrupt(timestamp_t timestamp);

    /*
     * Implementation of disable_interrupt
     */
    void disable_interrupt();

    /*
     * Implementation of clear_interrupt
     */
    void clear_interrupt();

    /*
     * Implementation of fire_interrupt
     */
    void fire_interrupt();

    /*
     * Implementation of get_info
     */
    const ticker_info_t *get_info();

    ticker_data_t data;

private:
    mbed::Timeout _timeout;
    const ticker_interface_t *const _intf;

    /*
     * The number of low power clock cycles which must pass between subsequent
     * calls to intf->set_interrupt
     */
    const uint32_t _min_count_between_writes;

    /*
     * The minimum number of low power clock cycles in the future that
     * a match value can be set to and still fire
     */
    const uint32_t _min_count_until_match;

    /*
     * Flag to indicate if the timer is suspended
     */
    bool _suspended;

    /*
     * _cur_match_time is valid and Timeout is scheduled to fire
     */
    bool _pending_timeout;

    /*
     * set_interrupt has been called and _cur_match_time is valid
     */
    bool _pending_match;

    /*
     * The function LowPowerTickerWrapper::fire_interrupt has been called
     * and an interrupt is expected.
     */
    bool _pending_fire_now;

    /*
     * It is safe to call intf->set_interrupt
     */
    bool _set_interrupt_allowed;

    /*
     * Last value written by LowPowerTickerWrapper::set_interrupt
     */
    timestamp_t _cur_match_time;

    /*
     * Time of last call to LowPowerTickerWrapper::set_interrupt
     */
    uint32_t _last_set_interrupt;

    /*
     * Time of last call to intf->set_interrupt
     */
    uint32_t _last_actual_set_interrupt;

    /*
     * Mask of valid bits from intf->read()
     */
    uint32_t _mask;

    /*
     * Microsecond per low power tick (rounded up)
     */
    uint32_t _us_per_tick;


    void _reset();

    /**
     * Set the low power ticker match time when hardware is ready
     *
     * This event is scheduled to set the lp timer after the previous write
     * has taken effect and it is safe to write a new value without blocking.
     * If the time has already passed then this function fires and interrupt
     * immediately.
     */
    void _timeout_handler();

    /*
     * Check match time has passed
     */
    bool _match_check(timestamp_t current);

    /*
     * Convert low power ticks to approximate microseconds
     *
     * This value is always larger or equal to exact value.
     */
    uint32_t _lp_ticks_to_us(uint32_t);

    /*
     * Schedule a match interrupt to fire at the correct time
     *
     * @param current The current low power ticker time
     */
    void _schedule_match(timestamp_t current);

};

#endif

/** @}*/


