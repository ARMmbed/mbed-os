/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#include "pm_buffer.h"

#include <stdbool.h>
#include <string.h>
#include "nrf_error.h"
#include "pm_mutex.h"


#define BUFFER_IS_VALID(p_buffer) ((p_buffer != NULL)             \
                                && (p_buffer->p_memory != NULL)   \
                                && (p_buffer->p_mutex  != NULL))



ret_code_t pm_buffer_init(pm_buffer_t * p_buffer,
                          uint8_t     * p_buffer_memory,
                          uint32_t      buffer_memory_size,
                          uint8_t     * p_mutex_memory,
                          uint32_t      mutex_memory_size,
                          uint32_t      n_blocks,
                          uint32_t      block_size)
{
    if (   (p_buffer           != NULL)
        && (p_buffer_memory    != NULL)
        && (p_mutex_memory     != NULL)
        && (buffer_memory_size >= (n_blocks*block_size))
        && (mutex_memory_size  >= MUTEX_STORAGE_SIZE(n_blocks))
        && (n_blocks           != 0)
        && (block_size         != 0))
    {
        p_buffer->p_memory   = p_buffer_memory;
        p_buffer->p_mutex    = p_mutex_memory;
        p_buffer->n_blocks   = n_blocks;
        p_buffer->block_size = block_size;
        pm_mutex_init(p_buffer->p_mutex, n_blocks);

        return NRF_SUCCESS;
    }
    else
    {
        return NRF_ERROR_INVALID_PARAM;
    }
}


uint8_t pm_buffer_block_acquire(pm_buffer_t * p_buffer, uint32_t n_blocks)
{
    if (!BUFFER_IS_VALID(p_buffer))
    {
        return ( BUFFER_INVALID_ID );
    }

    uint8_t first_locked_mutex = BUFFER_INVALID_ID;

    for (uint8_t i = 0; i < p_buffer->n_blocks; i++)
    {
        if (pm_mutex_lock(p_buffer->p_mutex, i))
        {
            if (first_locked_mutex == BUFFER_INVALID_ID)
            {
                first_locked_mutex = i;
            }
            if ((i - first_locked_mutex + 1) == n_blocks)
            {
                return first_locked_mutex;
            }
        }
        else if (first_locked_mutex != BUFFER_INVALID_ID)
        {
            for (uint8_t j = first_locked_mutex; j < i; j++)
            {
                pm_buffer_release(p_buffer, j);
            }
            first_locked_mutex = BUFFER_INVALID_ID;
        }
    }

    return ( BUFFER_INVALID_ID );
}


uint8_t * pm_buffer_ptr_get(pm_buffer_t * p_buffer, uint8_t id)
{
    if (!BUFFER_IS_VALID(p_buffer))
    {
        return ( NULL );
    }

    if ( (id != BUFFER_INVALID_ID)
    &&   pm_mutex_lock_status_get(p_buffer->p_mutex, id) )
    {
        return ( &p_buffer->p_memory[id*p_buffer->block_size] );
    }
    else
    {
        return ( NULL );
    }
}


void pm_buffer_release(pm_buffer_t * p_buffer, uint8_t id)
{
    if (    BUFFER_IS_VALID(p_buffer)
       &&  (id != BUFFER_INVALID_ID)
       &&   pm_mutex_lock_status_get(p_buffer->p_mutex, id))
    {
        pm_mutex_unlock(p_buffer->p_mutex, id);
    }
}
