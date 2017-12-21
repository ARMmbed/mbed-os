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

static events::EventQueue *_queue = NULL;

void TimerTimeCounterInit(events::EventQueue *queue)
{
    _queue = queue;
}

TimerTime_t TimerGetCurrentTime( void )
{
    const uint32_t current_time = _queue->tick();
    return (TimerTime_t)current_time;
}

TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime )
{
    return TimerGetCurrentTime() - savedTime;
}

void TimerInit( TimerEvent_t *obj, void ( *callback )( void ) )
{
    obj->value = 0;
    obj->Callback = callback;
}

void TimerStart( TimerEvent_t *obj )
{
    obj->Timer.attach_us( mbed::callback( obj->Callback ), obj->value * 1000 );
}

void TimerStop( TimerEvent_t *obj )
{
    obj->Timer.detach( );
}

void TimerSetValue( TimerEvent_t *obj, uint32_t value )
{
    obj->value = value;
}
