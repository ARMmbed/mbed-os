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

#include "lorawan/system/LoRaWANTimer.h"

LoRaWANTimeHandler::LoRaWANTimeHandler()
    : _queue(NULL)
{
}

LoRaWANTimeHandler::~LoRaWANTimeHandler()
{
}

void LoRaWANTimeHandler::TimerTimeCounterInit(events::EventQueue *queue)
{
    _queue = queue;
}

lorawan_time_t LoRaWANTimeHandler::TimerGetCurrentTime( void )
{
    const uint32_t current_time = _queue->tick();
    return (lorawan_time_t)current_time;
}

lorawan_time_t LoRaWANTimeHandler::TimerGetElapsedTime( lorawan_time_t savedTime )
{
    return TimerGetCurrentTime() - savedTime;
}

void LoRaWANTimeHandler::TimerInit( timer_event_t *obj, mbed::Callback<void()> callback)
{
    obj->value = 0;
    obj->callback = callback;
}

void LoRaWANTimeHandler::TimerStart( timer_event_t *obj )
{
    obj->timer.get()->attach_us(obj->callback, obj->value * 1000 );
}

void LoRaWANTimeHandler::TimerStop( timer_event_t *obj )
{
    obj->timer.get()->detach( );
}

void LoRaWANTimeHandler::TimerSetValue( timer_event_t *obj, uint32_t value )
{
    obj->value = value;
}
