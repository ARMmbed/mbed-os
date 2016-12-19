/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
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

#include "sdk_common.h"
#if NRF_MODULE_ENABLED(NRF_QUEUE)
#include "nrf_queue.h"
#include "app_util_platform.h"

/**@brief Get next element index.
 *
 * @param[in]   p_queue     Pointer to the queue instance.
 * @param[in]   idx         Current index.
 *
 * @return      Next element index.
 */
__STATIC_INLINE size_t nrf_queue_next_idx(nrf_queue_t const * p_queue, size_t idx)
{
    ASSERT(p_queue != NULL);
    return (idx < p_queue->size) ? (idx + 1) : 0;
}

/**@brief Get current queue utilization. This function assumes that this process will not be interrupted.
 *
 * @param[in]   p_queue     Pointer to the queue instance.
 *
 * @return      Current queue utilization.
 */
__STATIC_INLINE size_t queue_utilization_get(nrf_queue_t const * p_queue)
{
    return (p_queue->p_cb->back >= p_queue->p_cb->front) ?
           (p_queue->p_cb->back - p_queue->p_cb->front)  :
           (p_queue->size + 1 - p_queue->p_cb->front + p_queue->p_cb->back);
}

bool nrf_queue_is_full(nrf_queue_t const * p_queue)
{
    ASSERT(p_queue != NULL);
    return (nrf_queue_next_idx(p_queue, p_queue->p_cb->back) == p_queue->p_cb->front);
}

ret_code_t nrf_queue_push(nrf_queue_t const * p_queue, void const * p_element)
{
    ret_code_t status = NRF_SUCCESS;

    ASSERT(p_queue != NULL);
    ASSERT(p_element != NULL);

    CRITICAL_REGION_ENTER();
    bool is_full = nrf_queue_is_full(p_queue);

    if (!is_full || (p_queue->mode == NRF_QUEUE_MODE_OVERFLOW))
    {
        // Get write position.
        size_t write_pos = p_queue->p_cb->back;
        p_queue->p_cb->back = nrf_queue_next_idx(p_queue, p_queue->p_cb->back);
        if (is_full)
        {
            // Overwrite the oldest element.
            p_queue->p_cb->front = nrf_queue_next_idx(p_queue, p_queue->p_cb->front);
        }

        // Write a new element.
        switch (p_queue->element_size)
        {
            case sizeof(uint8_t):
                ((uint8_t *)p_queue->p_buffer)[write_pos] = *((uint8_t *)p_element);
                break;

            case sizeof(uint16_t):
                ((uint16_t *)p_queue->p_buffer)[write_pos] = *((uint16_t *)p_element);
                break;

            case sizeof(uint32_t):
                ((uint32_t *)p_queue->p_buffer)[write_pos] = *((uint32_t *)p_element);
                break;

            case sizeof(uint64_t):
                ((uint64_t *)p_queue->p_buffer)[write_pos] = *((uint64_t *)p_element);
                break;

            default:
                memcpy((void *)((size_t)p_queue->p_buffer + write_pos * p_queue->element_size),
                       p_element,
                       p_queue->element_size);
                break;
        }

        // Update utilization.
        size_t utilization = queue_utilization_get(p_queue);
        if (p_queue->p_cb->max_utilization < utilization)
        {
            p_queue->p_cb->max_utilization = utilization;
        }
    }
    else
    {
        status = NRF_ERROR_NO_MEM;
    }

    CRITICAL_REGION_EXIT();

    return status;
}

ret_code_t nrf_queue_generic_pop(nrf_queue_t const * p_queue,
                                 void              * p_element,
                                 bool                just_peek)
{
    ret_code_t status = NRF_SUCCESS;

    ASSERT(p_queue != NULL);
    ASSERT(p_element != NULL);

    CRITICAL_REGION_ENTER();

    if (!nrf_queue_is_empty(p_queue))
    {
        // Get read position.
        size_t read_pos = p_queue->p_cb->front;

        // Update next read position.
        if (!just_peek)
        {
            p_queue->p_cb->front = nrf_queue_next_idx(p_queue, p_queue->p_cb->front);
        }

        // Read element.
        switch (p_queue->element_size)
        {
            case sizeof(uint8_t):
                *((uint8_t *)p_element) = ((uint8_t *)p_queue->p_buffer)[read_pos];
                break;

            case sizeof(uint16_t):
                *((uint16_t *)p_element) = ((uint16_t *)p_queue->p_buffer)[read_pos];
                break;

            case sizeof(uint32_t):
                *((uint32_t *)p_element) = ((uint32_t *)p_queue->p_buffer)[read_pos];
                break;

            case sizeof(uint64_t):
                *((uint64_t *)p_element) = ((uint64_t *)p_queue->p_buffer)[read_pos];
                break;

            default:
                memcpy(p_element,
                       (void const *)((size_t)p_queue->p_buffer + read_pos * p_queue->element_size),
                       p_queue->element_size);
                break;
        }
    }
    else
    {
        status = NRF_ERROR_NOT_FOUND;
    }

    CRITICAL_REGION_EXIT();

    return status;
}

/**@brief Write elements to the queue. This function assumes that there is enough room in the queue
 *        to write the requested number of elements and that this process will not be interrupted.
 *
 * @param[in]   p_queue             Pointer to the nrf_queue_t instance.
 * @param[in]   p_data              Pointer to the buffer with elements to write.
 * @param[in]   element_count       Number of elements to write.
 */
static void queue_write(nrf_queue_t const * p_queue, void const * p_data, uint32_t element_count)
{
    size_t prev_available = nrf_queue_available_get(p_queue);
    size_t continuous     = p_queue->size + 1 - p_queue->p_cb->back;
    void * p_write_ptr    = (void *)((size_t)p_queue->p_buffer
                          + p_queue->p_cb->back * p_queue->element_size);
    if (element_count <= continuous)
    {
        memcpy(p_write_ptr,
               p_data,
               element_count * p_queue->element_size);

        p_queue->p_cb->back = ((p_queue->p_cb->back + element_count) <= p_queue->size)
                            ? (p_queue->p_cb->back + element_count)
                            : 0;
    }
    else
    {
        size_t first_write_length = continuous * p_queue->element_size;
        memcpy(p_write_ptr,
               p_data,
               first_write_length);

        size_t elements_left = element_count - continuous;
        memcpy(p_queue->p_buffer,
               (void const *)((size_t)p_data + first_write_length),
               elements_left * p_queue->element_size);

        p_queue->p_cb->back = elements_left;
        if (prev_available < element_count)
        {
            // Overwrite the oldest elements.
            p_queue->p_cb->front = nrf_queue_next_idx(p_queue, p_queue->p_cb->back);
        }
    }

    // Update utilization.
    size_t utilization = queue_utilization_get(p_queue);
    if (p_queue->p_cb->max_utilization < utilization)
    {
        p_queue->p_cb->max_utilization = utilization;
    }
}

ret_code_t nrf_queue_write(nrf_queue_t const * p_queue,
                           void const        * p_data,
                           size_t              element_count)
{
    ret_code_t status = NRF_SUCCESS;

    ASSERT(p_queue != NULL);
    ASSERT(p_data != NULL);
    ASSERT(element_count <= p_queue->size);

    if (element_count == 0)
    {
        return NRF_SUCCESS;
    }

    CRITICAL_REGION_ENTER();

    if ((nrf_queue_available_get(p_queue) >= element_count)
     || (p_queue->mode == NRF_QUEUE_MODE_OVERFLOW))
    {
        queue_write(p_queue, p_data, element_count);
    }
    else
    {
        status = NRF_ERROR_NO_MEM;
    }

    CRITICAL_REGION_EXIT();

    return status;
}


size_t nrf_queue_in(nrf_queue_t const * p_queue,
                    void const        * p_data,
                    size_t              element_count)
{
    ASSERT(p_queue != NULL);
    ASSERT(p_data != NULL);

    if (element_count == 0)
    {
        return 0;
    }

    CRITICAL_REGION_ENTER();

    if (p_queue->mode == NRF_QUEUE_MODE_OVERFLOW)
    {
        element_count = MIN(element_count, p_queue->size);
    }
    else
    {
        size_t available = nrf_queue_available_get(p_queue);
        element_count    = MIN(element_count, available);
    }

    queue_write(p_queue, p_data, element_count);

    CRITICAL_REGION_EXIT();

    return element_count;
}

/**@brief Read elements from the queue. This function assumes that there are enough elements
 *        in the queue to read and that this process will not be interrupted.
 *
 * @param[in]   p_queue             Pointer to the nrf_queue_t instance.
 * @param[out]  p_data              Pointer to the buffer where elements will be copied.
 * @param[in]   element_count       Number of elements to read.
 */
static void queue_read(nrf_queue_t const * p_queue, void * p_data, uint32_t element_count)
{
    size_t     continuous = (p_queue->p_cb->front <= p_queue->p_cb->back)
                          ? p_queue->p_cb->back - p_queue->p_cb->front
                          : p_queue->size + 1 - p_queue->p_cb->front;
    void const * p_read_ptr = (void const *)((size_t)p_queue->p_buffer
                            + p_queue->p_cb->front * p_queue->element_size);

    if (element_count <= continuous)
    {
        memcpy(p_data,
               p_read_ptr,
               element_count * p_queue->element_size);

        p_queue->p_cb->front = ((p_queue->p_cb->front + element_count) <= p_queue->size)
                             ? (p_queue->p_cb->front + element_count)
                             : 0;
    }
    else
    {
        size_t first_read_length = continuous * p_queue->element_size;
        memcpy(p_data,
               p_read_ptr,
               first_read_length);

        size_t elements_left = element_count - continuous;
        memcpy((void *)((size_t)p_data + first_read_length),
               p_queue->p_buffer,
               elements_left * p_queue->element_size);

        p_queue->p_cb->front = elements_left;
    }
}

ret_code_t nrf_queue_read(nrf_queue_t const * p_queue,
                          void              * p_data,
                          size_t              element_count)
{
    ret_code_t status = NRF_SUCCESS;

    ASSERT(p_queue != NULL);
    ASSERT(p_data != NULL);

    if (element_count == 0)
    {
        return NRF_SUCCESS;
    }

    CRITICAL_REGION_ENTER();

    if (element_count <= queue_utilization_get(p_queue))
    {
        queue_read(p_queue, p_data, element_count);
    }
    else
    {
        status = NRF_ERROR_NOT_FOUND;
    }

    CRITICAL_REGION_EXIT();

    return status;
}

size_t nrf_queue_out(nrf_queue_t const * p_queue,
                     void              * p_data,
                     size_t              element_count)
{
    ASSERT(p_queue != NULL);
    ASSERT(p_data != NULL);

    if (element_count == 0)
    {
        return 0;
    }

    CRITICAL_REGION_ENTER();

    size_t utilization = queue_utilization_get(p_queue);
    element_count      = MIN(element_count, utilization);

    queue_read(p_queue, p_data, element_count);

    CRITICAL_REGION_EXIT();

    return element_count;
}

void nrf_queue_reset(nrf_queue_t const * p_queue)
{
    ASSERT(p_queue != NULL);

    CRITICAL_REGION_ENTER();

    memset(p_queue->p_cb, 0, sizeof(nrf_queue_cb_t));

    CRITICAL_REGION_EXIT();
}

size_t nrf_queue_utilization_get(nrf_queue_t const * p_queue)
{
    size_t utilization;
    ASSERT(p_queue != NULL);

    CRITICAL_REGION_ENTER();

    utilization = queue_utilization_get(p_queue);

    CRITICAL_REGION_EXIT();

    return utilization;
}

#endif // NRF_MODULE_ENABLED(NRF_QUEUE)
