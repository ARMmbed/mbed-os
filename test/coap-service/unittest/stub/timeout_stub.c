/*
 * Copyright (c) 2016 ARM Limited. All Rights Reserved.
 */

 #include "eventOS_event_timer.h"

// Timeout structure, already typedefed to timeout_t
struct timeout_entry_t {
    uint8_t id;
};

static timeout_t timeout_stub;

timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg)
{
    return &timeout_stub;
}

void eventOS_timeout_cancel(timeout_t *t)
{

}
