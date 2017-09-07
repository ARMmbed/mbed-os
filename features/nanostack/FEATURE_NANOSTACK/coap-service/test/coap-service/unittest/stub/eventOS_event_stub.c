/*
 * Copyright (c) 2015-2017 ARM Limited. All Rights Reserved.
 */

#include <string.h>
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_event_stub.h"

eventOs_event_stub_def eventOs_event_stub;

int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_s *), uint8_t init_event_type)
{
    eventOs_event_stub.event_ptr = handler_func_ptr;
    return eventOs_event_stub.int8_value;
}
