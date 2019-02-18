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

    /** Gives access to network provided GPS time
     *
     * Network provides a time-stamp for the device synchronization on demand
     * using GPS time base. The request may originate from Application layer
     * Clock Synchronization protocol (for v1.0.2) or stack level DevTimeReq MAC
     * command (for v.10.3 and above). If the request originated from the application
     * layer, application is responsible for relaying the information to the stack.
     * In case of DevTimeReq MAC command, the stack will take care of it automatically.
     *
     * The API looks up the stored GPS time and the monotonic tick time-stamp taken
     * at the moment of storing GPS time, and returns GPS time + difference of stored
     * tick time and current tick time.
     * If the GPS time was not set by the network yet, the API returns zero.
     *
     * @return Current GPS time in seconds
     *         Or 0 if the GPS time is not yet set by the network
     */
    lorawan_time_t get_gps_time(void);

    /** Store GPS time received from the network
     *
     * Once the GPS time is received from the network, we store it along-with the
     * current CPU monotonic tick. Having taken snapshot of the current tick would
     * enable us to have a reference to calculate the time difference given the stored
     * GPS time and the forthcoming CPU tick. Please refer to `get_gps_time()` API
     * documentation for more information.
     *
     * It is important that the caller relays the network provided GPS time (in seconds)
     * as it is without adjustment (for leap seconds or conversions to TAI/UTC etc).
     * The rationale here is that we are not setting system time here. This time base
     * is used only for device level synchronization with network.
     *
     * @param gps_time              Current GPS time provided by the network (seconds)
     */
    void set_gps_time(lorawan_time_t gps_time);

private:
    events::EventQueue *_queue;
    lorawan_time_t _gps_time; // seconds
    lorawan_time_t _monotonic_tick_time; //milliseconds
};

#endif // MBED_LORAWAN_SYS_TIMER_H__
