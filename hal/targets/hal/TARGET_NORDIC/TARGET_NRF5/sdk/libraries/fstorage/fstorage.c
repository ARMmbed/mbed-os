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

#include "fstorage.h"
#include "fstorage_config.h"
#include "fstorage_internal_defs.h"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nrf_error.h"
#include "nrf_soc.h"


static uint8_t       m_flags;       // fstorage status flags.
static fs_op_queue_t m_queue;       // Queue of requested operations.
static uint8_t       m_retry_count; // Number of times the last flash operation was retried.


// Sends events to the application.
static void send_event(fs_op_t const * const p_op, fs_ret_t result)
{
    fs_evt_t evt;
    memset(&evt, 0x00, sizeof(fs_evt_t));

    switch (p_op->op_code)
    {
        case FS_OP_STORE:
            evt.id                 = FS_EVT_STORE;
            evt.store.p_data       = p_op->store.p_dest;
            evt.store.length_words = p_op->store.length_words;
            break;

        case FS_OP_ERASE:
            evt.id               = FS_EVT_ERASE;
            evt.erase.first_page = p_op->erase.page - p_op->erase.pages_erased;
            evt.erase.last_page  = p_op->erase.page;
            break;

        default:
            // Should not happen.
            break;
    }

    p_op->p_config->callback(&evt, result);
}


// Checks that a configuration is non-NULL and within section variable bounds.
static bool check_config(fs_config_t const * const config)
{
    if ((config != NULL) &&
        (FS_SECTION_VARS_START_ADDR <= (uint32_t)config) &&
        (FS_SECTION_VARS_END_ADDR   >  (uint32_t)config))
    {
        return true;
    }
   
    return false;
}


// Executes a store operation.
static uint32_t store_execute(fs_op_t const * const p_op)
{
    uint16_t chunk_len;

    if ((p_op->store.length_words - p_op->store.offset) < FS_MAX_WRITE_SIZE_WORDS)
    {
        chunk_len = p_op->store.length_words - p_op->store.offset;
    }
    else
    {
        chunk_len = FS_MAX_WRITE_SIZE_WORDS;
    }

    return sd_flash_write((uint32_t*)p_op->store.p_dest + p_op->store.offset,
                          (uint32_t*)p_op->store.p_src  + p_op->store.offset,
                          chunk_len);
}


// Executes an erase operation.
static uint32_t erase_execute(fs_op_t const * const p_op)
{
    return sd_flash_page_erase(p_op->erase.page);
}


// Advances the queue, wrapping around if necessary.
// If no elements are left in the queue, clears the FS_FLAG_PROCESSING flag.
static void queue_advance(void)
{
    if (--m_queue.count == 0)
    {
        m_flags &= ~FS_FLAG_PROCESSING;
    }

    if (++m_queue.rp == FS_QUEUE_SIZE)
    {
        m_queue.rp = 0;
    }
}


// Processes the current element in the queue. If the queue is empty, does nothing.
static void queue_process(void)
{
    uint32_t         ret;
    fs_op_t  * const p_op = &m_queue.op[m_queue.rp];

    if (m_queue.count > 0)
    {
        switch (p_op->op_code)
        {
            case FS_OP_STORE:
                ret = store_execute(p_op);
                break;

            case FS_OP_ERASE:
                ret = erase_execute(p_op);
                break;

             default:
                ret = FS_ERR_INTERNAL;
                break;
        }

        // There is a pending flash operation which was not initiated by this module.
        // Stop processing the queue and wait for a system event.
        if (ret == NRF_ERROR_BUSY)
        {
            m_flags &= ~FS_FLAG_PROCESSING;
            m_flags |= FS_FLAG_FLASH_REQ_PENDING;
        }
        else if (ret != NRF_SUCCESS)
        {
            // An error has occurred.
            send_event(p_op, FS_ERR_INTERNAL);
        }
        else
        {
            // Operation is executing.
        }
    }
}


// Starts processing the queue if there are no pending flash operations, both inside and
// outside this module. Returns immediately otherwise.
static void queue_start(void)
{
    if (!(m_flags & FS_FLAG_PROCESSING) &&
        !(m_flags & FS_FLAG_FLASH_REQ_PENDING))
    {
        m_flags |= FS_FLAG_PROCESSING;
        queue_process();
    }
}


// Flash operation success callback handler. Keeps track of the progress of an operation.
// If it has finished, advances the queue and notifies the application.
static void on_operation_success(fs_op_t * const p_op)
{
    m_retry_count = 0;

    switch (p_op->op_code)
    {
        case FS_OP_STORE:
        {
            uint16_t chunk_len;

            if ((p_op->store.length_words - p_op->store.offset) < FS_MAX_WRITE_SIZE_WORDS)
            {
                chunk_len = p_op->store.length_words - p_op->store.offset;
            }
            else
            {
                chunk_len = FS_MAX_WRITE_SIZE_WORDS;
            }

            p_op->store.offset += chunk_len;

            if (p_op->store.offset == p_op->store.length_words)
            {
                // The operation has finished.
                send_event(p_op, FS_SUCCESS);
                queue_advance();
            }
        }
        break;

        case FS_OP_ERASE:
        {
            p_op->erase.page++;
            p_op->erase.pages_erased++;

            if (p_op->erase.pages_erased == p_op->erase.pages_to_erase)
            {
                send_event(p_op, FS_SUCCESS);
                queue_advance();
            }
        }
        break;

        default:
            // Should not happen.
            break;
    }
}


// Flash operation failure callback handler. If the maximum number of retries has
// been reached, notifies the application and advances the queue.
static void on_operation_failure(fs_op_t const * const p_op)
{
    if (++m_retry_count > FS_OP_MAX_RETRIES)
    {
        m_retry_count = 0;

        send_event(p_op, FS_ERR_OPERATION_TIMEOUT);
        queue_advance();
    }
}


// Retrieves a pointer to the next free element in the queue.
// Additionally, increases the number of elements stored in the queue.
static bool queue_get_next_free(fs_op_t ** p_op)
{
    uint32_t idx;

    if (m_queue.count == FS_QUEUE_SIZE)
    {
        return false;
    }

    idx = ((m_queue.rp + m_queue.count) < FS_QUEUE_SIZE) ?
           (m_queue.rp + m_queue.count) : 0;

    m_queue.count++;

    // Zero the element so that unassigned fields will be zero.
    memset(&m_queue.op[idx], 0x00, sizeof(fs_op_t));

    *p_op = &m_queue.op[idx];

    return true;
}


fs_ret_t fs_init(void)
{
    uint32_t const   users         = FS_SECTION_VARS_COUNT;
    uint32_t const * p_current_end = FS_PAGE_END_ADDR;
    uint32_t         index_max     = 0x00;
    uint32_t         index_last    = 0xFFFFFFFF;

    if (m_flags & FS_FLAG_INITIALIZED)
    {
        return FS_SUCCESS;
    }

    #if 0
    // Check for configurations with duplicate priority.
    for (uint32_t i = 0; i < users; i++)
    {
        for (uint32_t j = i + 1; j < users; j++)
        {
            fs_config_t const * const p_config_i = FS_SECTION_VARS_GET(i);
            fs_config_t const * const p_config_j = FS_SECTION_VARS_GET(j);

            if (p_config_i->page_order == p_config_j->page_order)
            {
                // Error.
                return FS_ERR_INVALID_CFG;
            }
        }
    }
    #endif

    // Assign pages to registered users, beginning with the ones with the highest
    // priority, which will be assigned pages with the highest memory address.

    for (uint32_t i = 0; i < users; i++)
    {
        uint8_t max_priority  = 0x00;

        for (uint32_t j = 0; j < users; j++)
        {
            fs_config_t * const p_config = FS_SECTION_VARS_GET(j);

            // Skip the one assigned during last iteration.
            if (j == index_last)
            {
                continue;
            }

            if (p_config->priority >= max_priority)
            {
                max_priority = p_config->priority;
                index_max    = j;
            }
        }

        fs_config_t * const p_config = FS_SECTION_VARS_GET(index_max);

        p_config->p_end_addr   = p_current_end;
        p_config->p_start_addr = p_current_end - (p_config->num_pages * FS_PAGE_SIZE_WORDS);

        p_current_end = p_config->p_start_addr;

        index_last = index_max;
    }

    m_flags |= FS_FLAG_INITIALIZED;

    return FS_SUCCESS;
}


fs_ret_t fs_store(fs_config_t const * const p_config,
                  uint32_t    const * const p_dest,
                  uint32_t    const * const p_src,
                  uint16_t    const         length_words)
{
    fs_op_t * p_op;

    if (!(m_flags & FS_FLAG_INITIALIZED))
    {
        return FS_ERR_NOT_INITIALIZED;
    }

    if (!check_config(p_config))
    {
        return FS_ERR_INVALID_CFG;
    }

    if ((p_src == NULL) || (p_dest == NULL))
    {
        return FS_ERR_NULL_ARG;
    }

    // Check that both pointers are word aligned.
    if (((uint32_t)p_src  & 0x03) ||
        ((uint32_t)p_dest & 0x03))
    {
        return FS_ERR_UNALIGNED_ADDR;
    }

    // Check that the operation doesn't go outside the client's memory boundaries.
    if ((p_config->p_start_addr > p_dest) ||
        (p_config->p_end_addr   < (p_dest + length_words)))
    {
        return FS_ERR_INVALID_ADDR;
    }

    if (length_words == 0)
    {
        return FS_ERR_INVALID_ARG;
    }

    if (!queue_get_next_free(&p_op))
    {
        return FS_ERR_QUEUE_FULL;
    }

    // Initialize the operation.
    p_op->p_config           = p_config;
    p_op->op_code            = FS_OP_STORE;
    p_op->store.p_src        = p_src;
    p_op->store.p_dest       = p_dest;
    p_op->store.length_words = length_words;

    queue_start();

    return FS_SUCCESS;
}


fs_ret_t fs_erase(fs_config_t const * const p_config,
                  uint32_t    const * const p_page_addr,
                  uint16_t    const         num_pages)
{
    fs_op_t * p_op;

    if (!(m_flags & FS_FLAG_INITIALIZED))
    {
        return FS_ERR_NOT_INITIALIZED;
    }

    if (!check_config(p_config))
    {
        return FS_ERR_INVALID_CFG;
    }

    if (p_page_addr == NULL)
    {
        return FS_ERR_NULL_ARG;
    }

    // Check that the page is aligned to a page boundary.
    if (((uint32_t)p_page_addr % FS_PAGE_SIZE) != 0)
    {
        return FS_ERR_UNALIGNED_ADDR;
    }

    // Check that the operation doesn't go outside the client's memory boundaries.
    if ((p_page_addr < p_config->p_start_addr) ||
        (p_page_addr + (FS_PAGE_SIZE_WORDS * num_pages) > p_config->p_end_addr))
    {
        return FS_ERR_INVALID_ADDR;
    }

    if (num_pages == 0)
    {
        return FS_ERR_INVALID_ARG;
    }

    if (!queue_get_next_free(&p_op))
    {
        return FS_ERR_QUEUE_FULL;
    }

    // Initialize the operation.
    p_op->p_config             = p_config;
    p_op->op_code              = FS_OP_ERASE;
    p_op->erase.page           = ((uint32_t)p_page_addr / FS_PAGE_SIZE);
    p_op->erase.pages_to_erase = num_pages;

    queue_start();

    return FS_SUCCESS;
}


fs_ret_t fs_queued_op_count_get(uint32_t * const p_op_count)
{
    if (p_op_count == NULL)
    {
        return FS_ERR_NULL_ARG;
    }

    *p_op_count = m_queue.count;

    return FS_SUCCESS;
}


void fs_sys_event_handler(uint32_t sys_evt)
{
    fs_op_t * const p_op = &m_queue.op[m_queue.rp];
    
    if (m_flags & FS_FLAG_PROCESSING)
    {
        // A flash operation was initiated by this module. Handle the result.
        switch (sys_evt)
        {
            case NRF_EVT_FLASH_OPERATION_SUCCESS:
                on_operation_success(p_op);
                break;

            case NRF_EVT_FLASH_OPERATION_ERROR:
                on_operation_failure(p_op);
                break;
        }
    }
    else if ((m_flags & FS_FLAG_FLASH_REQ_PENDING))
    {
        // A flash operation was initiated outside this module.
        // A callback which indicates that it has finished was received.
        m_flags &= ~FS_FLAG_FLASH_REQ_PENDING;

        // If there are any elements left in the queue, set FS_FLAG_PROCESSING.
        if (m_queue.count > 0)
        {
           m_flags |= FS_FLAG_PROCESSING;
        }
    }

    // Resume processing the queue, if necessary.
    queue_process();
}

