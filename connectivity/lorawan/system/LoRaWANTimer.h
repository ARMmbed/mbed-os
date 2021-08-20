/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Timer objects and scheduling management

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef MBED_LORAWAN_SYS_TIMER_H__
#define MBED_LORAWAN_SYS_TIMER_H__

#include <stdint.h>
#include "events/EventQueue.h"

#include "lorawan_data_structures.h"

class LoRaWANTimeHandler {
public:
    LoRaWANTimeHandler();
    ~LoRaWANTimeHandler();

    /** Activates the timer subsystem.
     *
     * Embeds EventQueue object to timer subsystem which is subsequently
     * used to extract timer information.
     *
     * @param [in] queue  Handle to EventQueue object
     */
    void activate_timer_subsystem(events::EventQueue *queue);

    /** Read the current time.
     *
     * @return time The current time.
     */
    lorawan_time_t get_current_time(void);

    /** Return the time elapsed since a fixed moment in time.
     *
     * @param [in] saved_time    The fixed moment in time.
     * @return     time          The elapsed time.
     */
    lorawan_time_t get_elapsed_time(lorawan_time_t saved_time);

    /** Initializes the timer object.
     *
     * @remark The TimerSetValue function must be called before starting the timer.
     *         This function initializes the time-stamp and reloads the value at 0.
     *
     * @param [in] obj          The structure containing the timer object parameters.
     * @param [in] callback     The function callback called at the end of the timeout.
     */
    void init(timer_event_t &obj, mbed::Callback<void()> callback);

    /** Starts and adds the timer object to the list of timer events.
     *
     * @param [in] obj     The structure containing the timer object parameters.
     * @param [in] timeout The new timeout value.
     */
    void start(timer_event_t &obj, const uint32_t timeout);

    /** Stops and removes the timer object from the list of timer events.
     *
     * @param [in] obj The structure containing the timer object parameters.
     */
    void stop(timer_event_t &obj);

    /** Clear timer state so it is not inadvertently canceled. This function
     * must be called by the callback registered in init.
     *
     * @param [in] obj The structure containing the timer object parameters.
     */
    void clear(timer_event_t &obj);

private:
    events::EventQueue *_queue;
};

#endif // MBED_LORAWAN_SYS_TIMER_H__
