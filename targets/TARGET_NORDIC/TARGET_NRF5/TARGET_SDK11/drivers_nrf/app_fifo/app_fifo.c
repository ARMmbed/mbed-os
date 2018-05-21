/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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

#include "app_fifo.h"
#include "nrf_error.h"
#include "app_util.h"

/**
 * @brief Verify NULL parameters are not passed to an API by application.
 */
#define NULL_PARAM_CHECK(PARAM)                                                                    \
        if ((PARAM) == NULL)                                                                       \
        {                                                                                          \
            return (NRF_ERROR_NULL);                                                               \
        }


static __INLINE uint32_t fifo_length(app_fifo_t * p_fifo)
{
  uint32_t tmp = p_fifo->read_pos;
  return p_fifo->write_pos - tmp;
}


#define FIFO_LENGTH fifo_length(p_fifo)  /**< Macro for calculating the FIFO length. */


/**@brief Put one byte to the FIFO. */
static __INLINE void fifo_put(app_fifo_t * p_fifo, uint8_t byte)
{
    p_fifo->p_buf[p_fifo->write_pos & p_fifo->buf_size_mask] = byte;
    p_fifo->write_pos++;
}


/**@brief Get one byte to the FIFO. */
static __INLINE void fifo_get(app_fifo_t * p_fifo, uint8_t * p_byte)
{
    *p_byte = p_fifo->p_buf[p_fifo->read_pos & p_fifo->buf_size_mask];
     p_fifo->read_pos++;
}


uint32_t app_fifo_init(app_fifo_t * p_fifo, uint8_t * p_buf, uint16_t buf_size)
{
    // Check buffer for null pointer.
    if (p_buf == NULL)
    {
        return NRF_ERROR_NULL;
    }

    // Check that the buffer size is a power of two.
    if (!IS_POWER_OF_TWO(buf_size))
    {
        return NRF_ERROR_INVALID_LENGTH;
    }

    p_fifo->p_buf         = p_buf;
    p_fifo->buf_size_mask = buf_size - 1;
    p_fifo->read_pos      = 0;
    p_fifo->write_pos     = 0;

    return NRF_SUCCESS;
}


uint32_t app_fifo_put(app_fifo_t * p_fifo, uint8_t byte)
{
    if (FIFO_LENGTH <= p_fifo->buf_size_mask)
    {
        fifo_put(p_fifo, byte);
        return NRF_SUCCESS;
    }

    return NRF_ERROR_NO_MEM;
}


uint32_t app_fifo_get(app_fifo_t * p_fifo, uint8_t * p_byte)
{
    if (FIFO_LENGTH != 0)
    {
        fifo_get(p_fifo, p_byte);
        return NRF_SUCCESS;
    }

    return NRF_ERROR_NOT_FOUND;

}


uint32_t app_fifo_flush(app_fifo_t * p_fifo)
{
    p_fifo->read_pos = p_fifo->write_pos;
    return NRF_SUCCESS;
}


uint32_t app_fifo_read(app_fifo_t * p_fifo, uint8_t * p_byte_array, uint32_t * p_size)
{
    NULL_PARAM_CHECK(p_fifo);
    NULL_PARAM_CHECK(p_size);
    
    const uint32_t byte_count    = fifo_length(p_fifo);
    const uint32_t requested_len = (*p_size);
    uint32_t       index         = 0;
    uint32_t       read_size     = 0;
    
    (*p_size) = byte_count;
    
    // Check if the FIFO is empty.
    if (byte_count == 0)
    {
        return NRF_ERROR_NOT_FOUND; 
    }
    
    // Check if application has requested only the size.
    if (p_byte_array == NULL)
    {
        (*p_size) = byte_count;
        return NRF_SUCCESS;
    }    
    
    // Check is available bytes in FIFO less than requested.
    if (requested_len < byte_count)
    {
        read_size = requested_len;
    }
    else
    {
        read_size = byte_count;
    }
    
    // Fetch bytes from the FIFO.
    do
    {
        fifo_get(p_fifo, &p_byte_array[index++]);
    } while (index < read_size);
    
    (*p_size) = read_size;

    return NRF_SUCCESS;
}


uint32_t app_fifo_write(app_fifo_t * p_fifo, uint8_t const * p_byte_array, uint32_t * p_size)
{
    NULL_PARAM_CHECK(p_fifo);
    NULL_PARAM_CHECK(p_size);
    
    const uint32_t available_count = p_fifo->buf_size_mask - fifo_length(p_fifo) + 1;
    const uint32_t requested_len   = (*p_size);
    uint32_t       index           = 0;
    uint32_t       write_size      = 0;
    
    (*p_size) = available_count;
    
    // Check if the FIFO is FULL.
    if (available_count == 0)
    {
        return NRF_ERROR_NO_MEM; 
    }
    
    // Check if application has requested only the size.
    if (p_byte_array == NULL)
    {
        (*p_size) = available_count;
        return NRF_SUCCESS;
    }    
    
    // Check is available bytes in FIFO less than requested.
    if (requested_len < available_count)
    {
        write_size = requested_len;
    }
    else
    {
        write_size = available_count;
    }
    
    //Fetch bytes from the FIFO.
    do
    {
        fifo_put(p_fifo, p_byte_array[index++]);
    } while (index < write_size);
    
    (*p_size) = write_size;

    return NRF_SUCCESS;
}
