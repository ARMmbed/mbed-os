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

#include "LoRaWANTimer.h"

LoRaWANTimeHandler::LoRaWANTimeHandler()
    : _queue(NULL)
{
}

LoRaWANTimeHandler::~LoRaWANTimeHandler()
{
}

void LoRaWANTimeHandler::activate_timer_subsystem(events::EventQueue *queue)
{
}

lorawan_time_t LoRaWANTimeHandler::get_current_time( void )
{
    return (lorawan_time_t)0;
}

lorawan_time_t LoRaWANTimeHandler::get_elapsed_time(lorawan_time_t saved_time)
{
    return (lorawan_time_t)0;
}

void LoRaWANTimeHandler::init(timer_event_t &obj, mbed::Callback<void()> callback)
{
}

void LoRaWANTimeHandler::start(timer_event_t &obj, const uint32_t timeout)
{
}

void LoRaWANTimeHandler::stop(timer_event_t &obj)
{
}
