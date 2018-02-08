/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "wwd_buffer.h"
#include "wwd_constants.h"
#include "wwd_assert.h"
#include "network/wwd_buffer_interface.h"
#include "mbed_critical.h"

wwd_result_t host_buffer_check_leaked(void)
{
    /* Nothing to do */
    return WWD_SUCCESS;
}

wwd_result_t internal_host_buffer_get(wiced_buffer_t * buffer, wwd_buffer_dir_t direction, unsigned short size, unsigned long timeout_ms)
{
    assert(core_util_are_interrupts_enabled());
    void *p = malloc(sizeof(wiced_buffer_impl_t));
    if (!p) {
        return WWD_BUFFER_UNAVAILABLE_PERMANENT;
    }
    *buffer = p;

    p = malloc(size);
    if (!p) {
        free(p);
        return WWD_BUFFER_UNAVAILABLE_PERMANENT;
    }
    (*buffer)->ptr = p;

    (*buffer)->size = size;
    (*buffer)->offset = 0;
    return WWD_SUCCESS;
}

wwd_result_t host_buffer_get(/*@out@*/ wiced_buffer_t * buffer, wwd_buffer_dir_t direction, unsigned short size, wiced_bool_t wait)
{
    return internal_host_buffer_get(buffer, direction, size, wait);
}

void host_buffer_release(wiced_buffer_t buffer, wwd_buffer_dir_t direction )
{
    assert(buffer != NULL);
    assert(core_util_are_interrupts_enabled());
    free(buffer->ptr); // bug here
    free(buffer);
}

uint8_t* host_buffer_get_current_piece_data_pointer(wiced_buffer_t buffer )
{
    return &buffer->ptr[buffer->offset];
}

uint16_t host_buffer_get_current_piece_size(wiced_buffer_t buffer)
{
    UNUSED_PARAMETER(buffer);
    return buffer->size;
}

wiced_buffer_t host_buffer_get_next_piece(wiced_buffer_t buffer)
{
    UNUSED_PARAMETER(buffer);
    return NULL;
}

wwd_result_t host_buffer_add_remove_at_front(wiced_buffer_t * buffer, int32_t add_remove_amount)
{
    assert((*buffer)->offset >= -add_remove_amount);
    (*buffer)->offset += add_remove_amount;
    (*buffer)->size -= add_remove_amount;
    return WWD_SUCCESS;
}

wwd_result_t host_buffer_set_size(wiced_buffer_t buffer, unsigned short size)
{
    buffer->size = size;
    return WWD_SUCCESS;
}
