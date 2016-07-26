/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#include "eventOS_event_timer.h"

static void (*test_callback)(void *);
static void *test_args;

timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg)
{
    test_callback = callback;
    test_args = arg;
    return NULL;
}

void eventOS_timeout_cancel(timeout_t *t)
{

}

void test_eventOS_timeout_trigger() {
    test_callback(test_args);
}

