/*
 * Copyright (c) 2016-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "greentea-client/test_env.h"
#include "cmsis.h"
#include "mbed.h"
#include "rtos.h"
#include "mbed_assert.h"

// Amount to malloc for each iteration
#define MALLOC_TEST_SIZE            256
// Malloc fill pattern
#define MALLOC_FILL                 0x55

extern uint32_t mbed_heap_start;
extern uint32_t mbed_heap_size;
extern uint32_t mbed_stack_isr_start;
extern uint32_t mbed_stack_isr_size;

static uint32_t max_allocation_size = 0;

static bool inrange(uint32_t addr, uint32_t start, uint32_t size);
static bool rangeinrange(uint32_t addr, uint32_t size, uint32_t start, uint32_t len);
static bool valid_fill(uint8_t * data, uint32_t size, uint8_t fill);
static bool allocate_and_fill_heap(void);
static bool check_and_free_heap(void);

int main (void) {
    GREENTEA_SETUP(30, "default_auto");

    char c;
    char * initial_stack = &c;
    char *initial_heap;

    // Sanity check malloc
    initial_heap = (char*)malloc(1);
    if (initial_heap == NULL) {
        printf("Unable to malloc a single byte\n");
        GREENTEA_TESTSUITE_RESULT(false);
    }

    if (!inrange((uint32_t)initial_heap, mbed_heap_start, mbed_heap_size)) {
        printf("Heap in wrong location\n");
        GREENTEA_TESTSUITE_RESULT(false);
    }
    // MSP stack should be very near end (test using within 128 bytes)
    uint32_t msp = __get_MSP();
    if (!inrange(msp, mbed_stack_isr_start + mbed_stack_isr_size - 128, 128)) {
        printf("Interrupt stack in wrong location\n");
        GREENTEA_TESTSUITE_RESULT(false);
    }

    // Fully allocate the heap and stack
    bool ret = true;
    ret = ret && allocate_and_fill_heap();
    ret = ret && check_and_free_heap();

    // Force a task switch so a stack check is performed
    Thread::wait(10);

    printf("Total size dynamically allocated: %lu\n", max_allocation_size);

    GREENTEA_TESTSUITE_RESULT(ret);
}

/*
 * Return true if addr is in range [start:start+size)
 */
static bool inrange(uint32_t addr, uint32_t start, uint32_t size)
{
    return (addr >= start) && (addr < start + size) ? true : false;
}

/*
 * Return true if [addr:addr+size] is inside [start:start+len]
 */
static bool rangeinrange(uint32_t addr, uint32_t size, uint32_t start, uint32_t len)
{
    if (addr + size > start + len) {
        return false;
    }
    if (addr < start) {
        return false;
    }
    return true;
}

/*
 * Return true of the region is filled only the the specified fill value
 */
static bool valid_fill(uint8_t * data, uint32_t size, uint8_t fill)
{
    for (uint32_t i = 0; i < size; i++) {
        if (data[i] != fill) {
            return false;
        }
    }
    return true;
}

struct linked_list {
    linked_list * next;
    uint8_t data[MALLOC_TEST_SIZE];
};

static linked_list *head = NULL;
static bool allocate_and_fill_heap()
{

    linked_list *current;

    current = (linked_list*)malloc(sizeof(linked_list));
    if (0 == current) {
        return false;
    }
    current->next = NULL;
    memset((void*)current->data, MALLOC_FILL, sizeof(current->data));

    // Allocate until malloc returns NULL
    bool pass = true;
    head = current;
    while (true) {

        // Allocate
        linked_list *temp = (linked_list*)malloc(sizeof(linked_list));
        if (NULL == temp) {
            break;
        }
        if (!rangeinrange((uint32_t)temp, sizeof(linked_list), mbed_heap_start, mbed_heap_size)) {
            printf("Memory allocation out of range\n");
            pass = false;
            break;
        }

        // Init
        temp->next = NULL;
        memset((void*)temp->data, MALLOC_FILL, sizeof(current->data));

        // Add to list
        current->next = temp;
        current = temp;
    }
    return pass;
}

static bool check_and_free_heap()
{
    uint32_t total_size = 0;
    linked_list * current = head;
    bool pass = true;
    while (current != NULL) {
        total_size += sizeof(linked_list);
        if (!valid_fill(current->data, sizeof(current->data), MALLOC_FILL)) {
            pass = false;
        }
        linked_list * next = current->next;
        free(current);
        current = next;
    }

    max_allocation_size = total_size;
    return pass;
}
