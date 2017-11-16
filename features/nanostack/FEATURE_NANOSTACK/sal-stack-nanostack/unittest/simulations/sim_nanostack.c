/*
 * Copyright (c) 2014-2015, Arm Limited and affiliates.
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

/**
* \file \test_libTrace\Test.c
*
* \brief Unit tests for libTrace
*/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "unity.h"


#ifdef DEBUG_MALLOC
void *ns_dyn_mem_temporary_alloc(int16_t alloc_size)
{
    mem_t *str_ptr;
    str_ptr = malloc(sizeof(mem_t));
    str_ptr->size = alloc_size;
    str_ptr->ptr = malloc(alloc_size + 8);
    str_ptr->end_ptr = str_ptr->ptr + alloc_size + 4;
    ns_list_add_to_start(&mem_list, str_ptr);
    put_uint32(str_ptr->ptr, 0xdeadbeef);
    put_uint32(str_ptr->end_ptr, 0xdeadbeef);
    printf("Mem alloc ptr = %x\n", str_ptr->ptr + 4);
    return str_ptr->ptr + 4;
}
void ns_dyn_mem_free(void *heap_ptr)
{
    mem_t *str_ptr;
    printf("Mem Free ptr = %p\n", heap_ptr);
    ns_list_foreach(mem_t, cur_ptr, &mem_list) {
        if (cur_ptr->ptr + 4 == heap_ptr) {
            str_ptr = cur_ptr;
        }
    }
    if (str_ptr == NULL) {
        printf("Unknown block deallocated\n");
    }
    if (get_uint32(str_ptr->ptr) != 0xdeadbeef) {
        printf("MEM HEAD override\n");
        return;
    }
    if (get_uint32(str_ptr->end_ptr) != 0xdeadbeef) {
        printf("MEM TAIL override\n");
        return;
    }
    ns_list_remove(&mem_list, str_ptr);
    free(str_ptr->ptr);
    free(str_ptr);
    return;
}
#else
void *ns_dyn_mem_temporary_alloc(int16_t alloc_size)
{
    return malloc(alloc_size);
}
void ns_dyn_mem_free(void *heap_ptr)
{
    free(heap_ptr);
    return;
}
#endif
void *ns_dyn_mem_alloc(int16_t alloc_size)
{
    return ns_dyn_mem_temporary_alloc(alloc_size);
}

#include "eventOS_event.h"

int8_t eventOS_event_handler_create(void (*handler_func_ptr)(arm_event_s *), uint8_t init_event_type)
{
    return 1;
}
#include "eventOS_event_timer.h"
int8_t eventOS_event_timer_request(uint8_t snmessage, uint8_t event_type, int8_t tasklet_id, uint32_t time)
{
    return 1;
}

/* Random library*/
static uint32_t randoms = 1;
uint32_t randLIB_get_32bit(void)
{
    return randoms;
}
uint32_t randLIB_randomise_base(uint32_t base, uint16_t min_factor, uint16_t max_factor)
{
    return base;
}
