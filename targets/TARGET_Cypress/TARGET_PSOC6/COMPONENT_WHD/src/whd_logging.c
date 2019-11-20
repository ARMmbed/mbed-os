/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "whd_debug.h"

#ifdef WHD_LOGGING_BUFFER_ENABLE

#ifndef LOGGING_BUFFER_SIZE
#error LOGGING_BUFFER_SIZE is not defined
#endif  /* LOGGING_BUFFER_SIZE */
whd_logging_t logbuf;

int whd_buffer_printf(const char *format, ...)
{
    int potential_num_written = 0;
    va_list args;
    va_start (args, format);

    potential_num_written = vsnprintf (&(logbuf.buffer[logbuf.buffer_write]),
                                       (size_t)(LOGGING_BUFFER_SIZE - (logbuf.buffer_write) ) + 1, format, args);

    if (potential_num_written > (int)(LOGGING_BUFFER_SIZE - (logbuf.buffer_write) ) )
    {
        /* full print did not fit in buffer - wipe what was just written
           and reprint at start of buffer
         */
        memset(&(logbuf.buffer[logbuf.buffer_write]), 0xf, (size_t)(LOGGING_BUFFER_SIZE - (logbuf.buffer_write) ) );

        logbuf.buffer_write = 0;
        potential_num_written = vsnprintf (&(logbuf.buffer[logbuf.buffer_write]),
                                           (size_t)(LOGGING_BUFFER_SIZE - (logbuf.buffer_write) ) + 1, format, args);

        logbuf.buffer_write += (unsigned)potential_num_written;
        logbuf.buffer_write %= LOGGING_BUFFER_SIZE;

        if (logbuf.roll_over)
            logbuf.over_write = WHD_TRUE;

        logbuf.roll_over = WHD_TRUE;

        if ( (logbuf.roll_over) && (logbuf.buffer_read < (logbuf.buffer_write) ) )
        {
            logbuf.buffer_read = logbuf.buffer_write;
        }
        if (logbuf.over_write && (logbuf.buffer_read != (logbuf.buffer_write) ) )
        {
            logbuf.buffer_read = (logbuf.buffer_write);
        }
    }
    else
    {
        logbuf.buffer_write += (unsigned)potential_num_written;

        if ( (logbuf.buffer_write) >= LOGGING_BUFFER_SIZE )
        {
            logbuf.buffer_write %= LOGGING_BUFFER_SIZE;

            if (logbuf.roll_over)
                logbuf.over_write = WHD_TRUE;

            logbuf.roll_over = WHD_TRUE;
        }

        if (logbuf.roll_over && (logbuf.buffer_read < logbuf.buffer_write) )
        {
            logbuf.buffer_read = logbuf.buffer_write;
        }
        if (logbuf.over_write && (logbuf.buffer_read != logbuf.buffer_write) )
        {
            logbuf.buffer_read = logbuf.buffer_write;
        }
    }

    va_end (args);
    return potential_num_written;
}

void whd_print_logbuffer(void)
{
    while (logbuf.roll_over || logbuf.over_write || (logbuf.buffer_read != logbuf.buffer_write) )
    {
        logbuf.roll_over = logbuf.over_write = WHD_FALSE;

        while (logbuf.buffer[logbuf.buffer_read] == 0xf)
        {
            logbuf.buffer_read = (logbuf.buffer_read + 1) % LOGGING_BUFFER_SIZE;
        }

        putchar(logbuf.buffer[logbuf.buffer_read]);
        logbuf.buffer_read = (logbuf.buffer_read + 1) % LOGGING_BUFFER_SIZE;
    }
}

#endif /* ifdef WHD_LOGGING_BUFFER_ENABLE */

