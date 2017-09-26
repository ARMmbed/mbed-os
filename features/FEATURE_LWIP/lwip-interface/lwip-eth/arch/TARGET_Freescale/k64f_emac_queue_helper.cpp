/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
 */


#include "mbed.h"

#include "k64f_emac_queue_helper.h"

extern "C" int k64f_emac_call_every(int ms, void (*fn)(void *), void *handle)
{
    EventQueue *queue = mbed_event_queue();
    return queue->call_every(ms, fn, handle);
}
