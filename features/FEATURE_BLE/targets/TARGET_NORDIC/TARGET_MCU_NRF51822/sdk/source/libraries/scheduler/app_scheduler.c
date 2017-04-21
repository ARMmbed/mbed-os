/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "app_scheduler.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "nrf_soc.h"
#include "nrf_assert.h"
#include "app_util.h"
#include "app_util_platform.h"

/**@brief Structure for holding a scheduled event header. */
typedef struct
{
    app_sched_event_handler_t handler;          /**< Pointer to event handler to receive the event. */
    uint16_t                  event_data_size;  /**< Size of event data. */
} event_header_t;

STATIC_ASSERT(sizeof(event_header_t) <= APP_SCHED_EVENT_HEADER_SIZE);

static event_header_t * m_queue_event_headers;  /**< Array for holding the queue event headers. */
static uint8_t        * m_queue_event_data;     /**< Array for holding the queue event data. */
static volatile uint8_t m_queue_start_index;    /**< Index of queue entry at the start of the queue. */
static volatile uint8_t m_queue_end_index;      /**< Index of queue entry at the end of the queue. */
static uint16_t         m_queue_event_size;     /**< Maximum event size in queue. */
static uint16_t         m_queue_size;           /**< Number of queue entries. */

/**@brief Function for incrementing a queue index, and handle wrap-around.
 *
 * @param[in]   index   Old index.
 *
 * @return      New (incremented) index.
 */
static __INLINE uint8_t next_index(uint8_t index)
{
    return (index < m_queue_size) ? (index + 1) : 0;
}


static __INLINE uint8_t app_sched_queue_full()
{
  uint8_t tmp = m_queue_start_index;
  return next_index(m_queue_end_index) == tmp;
}

/**@brief Macro for checking if a queue is full. */
#define APP_SCHED_QUEUE_FULL() app_sched_queue_full()


static __INLINE uint8_t app_sched_queue_empty()
{
  uint8_t tmp = m_queue_start_index;
  return m_queue_end_index == tmp;
}

/**@brief Macro for checking if a queue is empty. */
#define APP_SCHED_QUEUE_EMPTY() app_sched_queue_empty()


uint32_t app_sched_init(uint16_t event_size, uint16_t queue_size, void * p_event_buffer)
{
    uint16_t data_start_index = (queue_size + 1) * sizeof(event_header_t);

    // Check that buffer is correctly aligned
    if (!is_word_aligned(p_event_buffer))
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Initialize event scheduler
    m_queue_event_headers = p_event_buffer;
    m_queue_event_data    = &((uint8_t *)p_event_buffer)[data_start_index];
    m_queue_end_index     = 0;
    m_queue_start_index   = 0;
    m_queue_event_size    = event_size;
    m_queue_size          = queue_size;

    return NRF_SUCCESS;
}


uint32_t app_sched_event_put(void                    * p_event_data,
                             uint16_t                  event_data_size,
                             app_sched_event_handler_t handler)
{
    uint32_t err_code;

    if (event_data_size <= m_queue_event_size)
    {
        uint16_t event_index = 0xFFFF;

        CRITICAL_REGION_ENTER();

        if (!APP_SCHED_QUEUE_FULL())
        {
            event_index       = m_queue_end_index;
            m_queue_end_index = next_index(m_queue_end_index);
        }

        CRITICAL_REGION_EXIT();

        if (event_index != 0xFFFF)
        {
            // NOTE: This can be done outside the critical region since the event consumer will
            //       always be called from the main loop, and will thus never interrupt this code.
            m_queue_event_headers[event_index].handler = handler;
            if ((p_event_data != NULL) && (event_data_size > 0))
            {
                memcpy(&m_queue_event_data[event_index * m_queue_event_size],
                       p_event_data,
                       event_data_size);
                m_queue_event_headers[event_index].event_data_size = event_data_size;
            }
            else
            {
                m_queue_event_headers[event_index].event_data_size = 0;
            }

            err_code = NRF_SUCCESS;
        }
        else
        {
            err_code = NRF_ERROR_NO_MEM;
        }
    }
    else
    {
        err_code = NRF_ERROR_INVALID_LENGTH;
    }

    return err_code;
}


/**@brief Function for reading the next event from specified event queue.
 *
 * @param[out]  pp_event_data       Pointer to pointer to event data.
 * @param[out]  p_event_data_size   Pointer to size of event data.
 * @param[out]  p_event_handler     Pointer to event handler function pointer.
 *
 * @return      NRF_SUCCESS if new event, NRF_ERROR_NOT_FOUND if event queue is empty.
 */
static uint32_t app_sched_event_get(void                     ** pp_event_data,
                                    uint16_t *                  p_event_data_size,
                                    app_sched_event_handler_t * p_event_handler)
{
    uint32_t err_code = NRF_ERROR_NOT_FOUND;

    if (!APP_SCHED_QUEUE_EMPTY())
    {
        uint16_t event_index;

        // NOTE: There is no need for a critical region here, as this function will only be called
        //       from app_sched_execute() from inside the main loop, so it will never interrupt
        //       app_sched_event_put(). Also, updating of (i.e. writing to) the start index will be
        //       an atomic operation.
        event_index         = m_queue_start_index;
        m_queue_start_index = next_index(m_queue_start_index);

        *pp_event_data     = &m_queue_event_data[event_index * m_queue_event_size];
        *p_event_data_size = m_queue_event_headers[event_index].event_data_size;
        *p_event_handler   = m_queue_event_headers[event_index].handler;

        err_code = NRF_SUCCESS;
    }

    return err_code;
}


void app_sched_execute(void)
{
    void                    * p_event_data;
    uint16_t                  event_data_size;
    app_sched_event_handler_t event_handler;

    // Get next event (if any), and execute handler
    while ((app_sched_event_get(&p_event_data, &event_data_size, &event_handler) == NRF_SUCCESS))
    {
        event_handler(p_event_data, event_data_size);
    }
}
