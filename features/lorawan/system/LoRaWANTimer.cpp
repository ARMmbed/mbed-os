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

#include <math.h>
#include "LoRaWANTimer.h"

LoRaWANTimeHandler::LoRaWANTimeHandler()
    : _queue(NULL),
      _gps_time(0),
      _monotonic_tick_time(0)
{
}

LoRaWANTimeHandler::~LoRaWANTimeHandler()
{
}

void LoRaWANTimeHandler::activate_timer_subsystem(events::EventQueue *queue)
{
    _queue = queue;
}

lorawan_time_t LoRaWANTimeHandler::get_current_time(void)
{
    const uint32_t current_time = _queue->tick();
    return (lorawan_time_t)current_time;
}

lorawan_time_t LoRaWANTimeHandler::get_elapsed_time(lorawan_time_t saved_time)
{
    return get_current_time() - saved_time;
}

void LoRaWANTimeHandler::init(timer_event_t &obj, mbed::Callback<void()> callback)
{
    obj.callback = callback;
    obj.timer_id = 0;
}

void LoRaWANTimeHandler::start(timer_event_t &obj, const uint32_t timeout)
{
    obj.timer_id = _queue->call_in(timeout, obj.callback);
    MBED_ASSERT(obj.timer_id != 0);
}

void LoRaWANTimeHandler::stop(timer_event_t &obj)
{
    _queue->cancel(obj.timer_id);
    obj.timer_id = 0;
}

lorawan_time_t LoRaWANTimeHandler::get_gps_time(void)
{
    if (_gps_time == 0) {
        return _gps_time;
    }

    return _gps_time + floor((get_current_time() - _monotonic_tick_time) / 1000);
}

void LoRaWANTimeHandler::set_gps_time(lorawan_time_t gps_time)
{
    // store in seconds
    _gps_time = gps_time;
    // store snapshot of current tick time in (ms)
    _monotonic_tick_time = get_current_time();

}
