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
 
#include "hci_mem_pool.h"
#include "hci_mem_pool_internal.h"
#include <stdbool.h>
#include <stdio.h>

/**@brief RX buffer element instance structure. 
 */
typedef struct 
{
    uint8_t  rx_buffer[RX_BUF_SIZE];                                /**< RX buffer memory array. */  
    uint32_t length;                                                /**< Length of the RX buffer memory array. */
} rx_buffer_elem_t;

/**@brief RX buffer queue element instance structure. 
 */
typedef struct 
{
    rx_buffer_elem_t * p_buffer;                                    /**< Pointer to RX buffer element. */
    uint32_t           free_window_count;                           /**< Free space element count. */
    uint32_t           free_available_count;                        /**< Free area element count. */
    uint32_t           read_available_count;                        /**< Read area element count. */
    uint32_t           write_index;                                 /**< Write position index. */                                      
    uint32_t           read_index;                                  /**< Read position index. */                                                                            
    uint32_t           free_index;                                  /**< Free position index. */                                                                                                                  
} rx_buffer_queue_t;

static bool              m_is_tx_allocated;                         /**< Boolean value to determine if the TX buffer is allocated. */
static rx_buffer_elem_t  m_rx_buffer_elem_queue[RX_BUF_QUEUE_SIZE]; /**< RX buffer element instances. */
static rx_buffer_queue_t m_rx_buffer_queue;                         /**< RX buffer queue element instance. */


uint32_t hci_mem_pool_open(void)
{
    m_is_tx_allocated                      = false;    
    m_rx_buffer_queue.p_buffer             = m_rx_buffer_elem_queue;
    m_rx_buffer_queue.free_window_count    = RX_BUF_QUEUE_SIZE;
    m_rx_buffer_queue.free_available_count = 0;
    m_rx_buffer_queue.read_available_count = 0;
    m_rx_buffer_queue.write_index          = 0;    
    m_rx_buffer_queue.read_index           = 0;        
    m_rx_buffer_queue.free_index           = 0;            
    
    return NRF_SUCCESS;
}


uint32_t hci_mem_pool_close(void)
{    
    return NRF_SUCCESS;
}


uint32_t hci_mem_pool_tx_alloc(void ** pp_buffer)
{
    static uint8_t tx_buffer[TX_BUF_SIZE];  

    uint32_t err_code;
    
    if (pp_buffer == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
    if (!m_is_tx_allocated)
    {        
            m_is_tx_allocated = true;
            *pp_buffer        = tx_buffer;
            err_code          = NRF_SUCCESS;
    }
    else
    {
        err_code              = NRF_ERROR_NO_MEM;
    }
    
    return err_code;
}


uint32_t hci_mem_pool_tx_free(void)
{
    m_is_tx_allocated = false;
    
    return NRF_SUCCESS;
}


uint32_t hci_mem_pool_rx_produce(uint32_t length, void ** pp_buffer)
{
    uint32_t err_code; 

    if (pp_buffer == NULL)
    {
        return NRF_ERROR_NULL;
    }    
    *pp_buffer = NULL;
    
    if (m_rx_buffer_queue.free_window_count != 0)
    {    
        if (length <= RX_BUF_SIZE)
        {    
            --(m_rx_buffer_queue.free_window_count);            
            ++(m_rx_buffer_queue.read_available_count);            

            *pp_buffer                    = 
                    m_rx_buffer_queue.p_buffer[m_rx_buffer_queue.write_index].rx_buffer;

            m_rx_buffer_queue.free_index |= (1u << m_rx_buffer_queue.write_index);

            // @note: Adjust the write_index making use of the fact that the buffer size is of 
            // power of two and two's complement arithmetic. For details refer example to book 
            // "Making embedded systems: Elicia White".
            m_rx_buffer_queue.write_index = 
                    (m_rx_buffer_queue.write_index + 1u) & (RX_BUF_QUEUE_SIZE - 1u);
            
            err_code                      = NRF_SUCCESS;
        }
        else
        {
            err_code = NRF_ERROR_DATA_SIZE;    
        }        
    }
    else
    {
        err_code = NRF_ERROR_NO_MEM;    
    }
    
    return err_code;
}


uint32_t hci_mem_pool_rx_consume(uint8_t * p_buffer)
{
    uint32_t err_code;
    uint32_t consume_index;
    uint32_t start_index;
    
    if (m_rx_buffer_queue.free_available_count != 0)
    {
        // Find the buffer that has been freed -
        // Start at read_index minus free_available_count and then increment until read index.
        err_code      = NRF_ERROR_INVALID_ADDR;
        consume_index = (m_rx_buffer_queue.read_index - m_rx_buffer_queue.free_available_count) & 
                        (RX_BUF_QUEUE_SIZE - 1u);
        start_index   = consume_index;
        
        do
        {
            if (m_rx_buffer_queue.p_buffer[consume_index].rx_buffer == p_buffer)
            {
                m_rx_buffer_queue.free_index ^= (1u << consume_index);
                err_code = NRF_SUCCESS;
                break;
            }
            else
            {
                consume_index = (consume_index + 1u) & (RX_BUF_QUEUE_SIZE - 1u);
            }
        }
        while (consume_index != m_rx_buffer_queue.read_index);

        while (!(m_rx_buffer_queue.free_index & (1 << start_index)) && 
                (m_rx_buffer_queue.free_available_count != 0))
        {
            --(m_rx_buffer_queue.free_available_count);
            ++(m_rx_buffer_queue.free_window_count);            
            start_index = (consume_index + 1u) & (RX_BUF_QUEUE_SIZE - 1u);
        }
    }
    else
    {
        err_code = NRF_ERROR_NO_MEM;
    }
        
    return err_code;    
}


uint32_t hci_mem_pool_rx_data_size_set(uint32_t length)
{
    // @note: Adjust the write_index making use of the fact that the buffer size is of power
    // of two and two's complement arithmetic. For details refer example to book 
    // "Making embedded systems: Elicia White".
    const uint32_t index = (m_rx_buffer_queue.write_index - 1u) & (RX_BUF_QUEUE_SIZE - 1u);
    m_rx_buffer_queue.p_buffer[index].length = length;    
    
    return NRF_SUCCESS;
}


uint32_t hci_mem_pool_rx_extract(uint8_t ** pp_buffer, uint32_t * p_length)
{
    uint32_t err_code;
    
    if ((pp_buffer == NULL) || (p_length == NULL))
    {
        return NRF_ERROR_NULL;
    }
    
    if (m_rx_buffer_queue.read_available_count != 0)
    {
        --(m_rx_buffer_queue.read_available_count);
        ++(m_rx_buffer_queue.free_available_count);        
        
        *pp_buffer                   = 
            m_rx_buffer_queue.p_buffer[m_rx_buffer_queue.read_index].rx_buffer;
        *p_length                    = 
            m_rx_buffer_queue.p_buffer[m_rx_buffer_queue.read_index].length;
        
        // @note: Adjust the write_index making use of the fact that the buffer size is of power
        // of two and two's complement arithmetic. For details refer example to book 
        // "Making embedded systems: Elicia White".            
        m_rx_buffer_queue.read_index = 
            (m_rx_buffer_queue.read_index + 1u) & (RX_BUF_QUEUE_SIZE - 1u); 
        
        err_code                     = NRF_SUCCESS;
    }
    else
    {
        err_code                     = NRF_ERROR_NO_MEM;        
    }
    
    return err_code;
}
