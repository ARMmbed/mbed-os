/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "net_interface.h"
#include "socket_api.h"
#include "mbed-trace/mbed_trace.h"
#include "common_functions.h"

#include "ns_cmdline.h"
#include "cmd_commands.h"
#include "cmd_lwm2m.h"

#define TRACE_GROUP "cApp"

#define ESC 0x03
#define EVENT_DATA_COMMAND_EXECUTED 6

// Prototypes
void cmd_ready_cb(int retcode);

static void cmdline_event(arm_event_s *event);

/**
* Callback function for thread handling.
*/
void* __thread_poll_function(void*);

typedef struct {
  uint8_t tasklet_id;
} cmd_commands_t;

cmd_commands_t  cmd_commands;
pthread_t       input_thread; /* Thread for input_terminal-function */

void initialize_app_commands(int8_t /*rf_driver_id*/)
{
  //initialize ready cb
  cmd_set_ready_cb( cmd_ready_cb );
  lwm2m_command_init();

  cmd_commands.tasklet_id = eventOS_event_handler_create(&cmdline_event, EVENT_TYPE_CMDLINE);
  pthread_create(&input_thread, NULL,__thread_poll_function, NULL);
}

void cmd_ready_cb(int retcode)
{
  tr_debug("cmd_ready_cb(%d)", retcode);
  arm_event_s event;
  event.sender = cmd_commands.tasklet_id;
  event.receiver = cmd_commands.tasklet_id;
  event.event_type = APPLICATION_EVENT;
  event.event_id = EVENT_DATA_COMMAND_EXECUTED;
  event.event_data = retcode;
  eventOS_event_send(&event);
}

static void cmdline_event(arm_event_s *event)
{
    switch( event->event_type )
    {
        case ARM_LIB_TASKLET_INIT_EVENT:
            //tasklet up and running
            tr_warning("cmdline_event-ARM_LIB_TASKLET_INIT_EVENT");
            break;

        case APPLICATION_EVENT:
            if( event->event_id == EVENT_DATA_COMMAND_EXECUTED )
            {
                int retcode = event->event_data;
                cmd_next( retcode );
            }
            break;
        default:
            tr_warning("Unknown event type (type: %i, id: %i)", event->event_type, event->event_id);
        break;
    }
}

void* __thread_poll_function(void*)
{
    int16_t c = getchar();
    while( c >= 0 ) {
        cmd_char_input(c);
        c = getchar();
    }
    return NULL;
}
