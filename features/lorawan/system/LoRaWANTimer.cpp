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

TimerTime_t LoRaWANTimeHandler::TimerGetCurrentTime( void )
{
    const uint32_t current_time = _queue->tick();
    return (TimerTime_t)current_time;
}

TimerTime_t LoRaWANTimeHandler::TimerGetElapsedTime( TimerTime_t savedTime )
{
    return TimerGetCurrentTime() - savedTime;
}

void LoRaWANTimeHandler::TimerInit( TimerEvent_t *obj, mbed::Callback<void()> callback)
{
    obj->value = 0;
    obj->Callback = callback;
}

void LoRaWANTimeHandler::TimerStart( TimerEvent_t *obj )
{
    obj->Timer.get()->attach_us( mbed::callback( obj->Callback ), obj->value * 1000 );
}

void LoRaWANTimeHandler::TimerStop( TimerEvent_t *obj )
{
    obj->Timer.get()->detach( );
}

void LoRaWANTimeHandler::TimerSetValue( TimerEvent_t *obj, uint32_t value )
{
    obj->value = value;
}
