/*
 * Copyright (c) 2014-2016 ARM Limited. All rights reserved.
 */

#include "nsdynmemLIB_stub.h"
#include <stdint.h>
#include <string.h>
#include "nsdynmemLIB.h"
#include "platform/arm_hal_interrupt.h"
#include <stdlib.h>

nsdynmemlib_stub_data_t nsdynmemlib_stub;

void ns_dyn_mem_init(uint8_t *heap, uint16_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr)
{
}

void *ns_dyn_mem_alloc(int16_t alloc_size)
{
    if (nsdynmemlib_stub.returnCounter > 0)
    {
        nsdynmemlib_stub.returnCounter--;
        return malloc(alloc_size);
    }
    else
    {
        return(nsdynmemlib_stub.expectedPointer);
    }
}

void *ns_dyn_mem_temporary_alloc(int16_t alloc_size)
{
    if (nsdynmemlib_stub.returnCounter > 0)
    {
        nsdynmemlib_stub.returnCounter--;
        return malloc(alloc_size);
    }
    else
    {
        return(nsdynmemlib_stub.expectedPointer);
    }
}

void ns_dyn_mem_free(void *block)
{
        free(block);
}

