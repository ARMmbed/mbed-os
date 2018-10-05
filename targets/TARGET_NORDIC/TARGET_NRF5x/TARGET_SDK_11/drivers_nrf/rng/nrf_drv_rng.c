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

#include <stdint.h>
#include <stddef.h>

#include "nrf_drv_rng.h"
#include "nrf_assert.h"
#include "nrf_drv_common.h"
#include "nordic_common.h"
#include "nrf_error.h"
#include "nrf_assert.h"
#ifdef SOFTDEVICE_PRESENT
#include "nrf_sdm.h"
#include "nrf_soc.h"
#else
#include "app_fifo.h"
#include "app_util_platform.h"

static __INLINE uint32_t fifo_length(app_fifo_t * p_fifo)
{
    uint32_t tmp = p_fifo->read_pos;
    return p_fifo->write_pos - tmp;
}

#define FIFO_LENGTH(fifo) fifo_length(&(fifo))  /**< Macro for calculating the FIFO length. */

#endif // SOFTDEVICE_PRESENT
typedef struct
{
    nrf_drv_state_t state;
#ifndef SOFTDEVICE_PRESENT
    app_fifo_t rand_pool;
    uint8_t    buffer[RNG_CONFIG_POOL_SIZE];
#endif // SOFTDEVICE_PRESENT
} nrf_drv_rng_cb_t;

static nrf_drv_rng_cb_t m_rng_cb;
#ifndef SOFTDEVICE_PRESENT
static const nrf_drv_rng_config_t m_default_config = NRF_DRV_RNG_DEFAULT_CONFIG;
static void rng_start(void)
{
    if (FIFO_LENGTH(m_rng_cb.rand_pool) <= m_rng_cb.rand_pool.buf_size_mask)
    {
        nrf_rng_event_clear(NRF_RNG_EVENT_VALRDY);
        nrf_rng_int_enable(NRF_RNG_INT_VALRDY_MASK);
        nrf_rng_task_trigger(NRF_RNG_TASK_START);
    }
}


static void rng_stop(void)
{
    nrf_rng_int_disable(NRF_RNG_INT_VALRDY_MASK);
    nrf_rng_task_trigger(NRF_RNG_TASK_STOP);
}


#endif // SOFTDEVICE_PRESENT


ret_code_t nrf_drv_rng_init(nrf_drv_rng_config_t const * p_config)
{
    uint32_t result;

    if (m_rng_cb.state == NRF_DRV_STATE_UNINITIALIZED)
    {
#ifndef SOFTDEVICE_PRESENT

        result = app_fifo_init(&m_rng_cb.rand_pool, m_rng_cb.buffer, RNG_CONFIG_POOL_SIZE);

        if (p_config == NULL)
        {
            p_config = &m_default_config;
        }

        if (result == NRF_SUCCESS)
        {
            if (p_config->error_correction)
            {
                nrf_rng_error_correction_enable();
            }

            nrf_drv_common_irq_enable(RNG_IRQn, p_config->interrupt_priority);

            nrf_rng_shorts_disable(NRF_RNG_SHORT_VALRDY_STOP_MASK);

            rng_start();
            m_rng_cb.state = NRF_DRV_STATE_INITIALIZED;
        }
#else
        UNUSED_VARIABLE(p_config);
        uint8_t softdevice_is_enabled;
        result = sd_softdevice_is_enabled(&softdevice_is_enabled);

        if (softdevice_is_enabled)
        {
            m_rng_cb.state = NRF_DRV_STATE_INITIALIZED;
        }
        else
        {
            result = NRF_ERROR_SOFTDEVICE_NOT_ENABLED;
        }
#endif // SOFTDEVICE_PRESENT
    }
    else
    {
        result = NRF_ERROR_INVALID_STATE;
    }
    return result;
}


void nrf_drv_rng_uninit(void)
{
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

    m_rng_cb.state = NRF_DRV_STATE_UNINITIALIZED;
#ifndef SOFTDEVICE_PRESENT
    rng_stop();
    nrf_drv_common_irq_disable(RNG_IRQn);
#endif // SOFTDEVICE_PRESENT
}

ret_code_t nrf_drv_rng_bytes_available(uint8_t * p_bytes_available)
{
    ret_code_t result;
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

#ifndef SOFTDEVICE_PRESENT

    result             = NRF_SUCCESS;
    *p_bytes_available = FIFO_LENGTH(m_rng_cb.rand_pool);

#else

    result = sd_rand_application_bytes_available_get(p_bytes_available);

#endif // SOFTDEVICE_PRESENT

    return result;
}

ret_code_t nrf_drv_rng_pool_capacity(uint8_t * p_pool_capacity)
{
    ret_code_t result;
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

#ifndef SOFTDEVICE_PRESENT

    result           = NRF_SUCCESS;
    *p_pool_capacity = RNG_CONFIG_POOL_SIZE;

#else

    result = sd_rand_application_pool_capacity_get(p_pool_capacity);

#endif // SOFTDEVICE_PRESENT
    return result;
}

ret_code_t nrf_drv_rng_rand(uint8_t * p_buff, uint8_t length)
{
    ret_code_t result;

    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

#ifndef SOFTDEVICE_PRESENT
    if (FIFO_LENGTH(m_rng_cb.rand_pool) >= length)
    {
        result = NRF_SUCCESS;

        for (uint32_t i = 0; (i < length) && (result == NRF_SUCCESS); i++)
        {
            result = app_fifo_get(&(m_rng_cb.rand_pool), &p_buff[i]);
        }
        rng_start();
    }
    else
    {
        result = NRF_ERROR_NO_MEM;
    }
#else

    result = sd_rand_application_vector_get(p_buff, length);

#endif // SOFTDEVICE_PRESENT


    return result;
}

ret_code_t nrf_drv_rng_block_rand(uint8_t * p_buff, uint32_t length)
{
    uint32_t count = 0, poolsz = 0;
    ret_code_t result;
    ASSERT(m_rng_cb.state == NRF_DRV_STATE_INITIALIZED);

    result = nrf_drv_rng_pool_capacity((uint8_t *) &poolsz);
    if(result != NRF_SUCCESS)
    {
        return result;
    }

    while(length)
    {
        uint32_t len = length >= poolsz ? poolsz : length;
        while((result = nrf_drv_rng_rand(&p_buff[count], len)) != NRF_SUCCESS)
        {
#ifndef SOFTDEVICE_PRESENT
            ASSERT(result == NRF_ERROR_NO_MEM);
#else
            ASSERT(result == NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES);
#endif
        }

        length -= len;
        count += len;
    }

    return result;
}


#ifndef SOFTDEVICE_PRESENT
void RNG_IRQHandler(void)
{
    if (nrf_rng_event_get(NRF_RNG_EVENT_VALRDY) &&
        nrf_rng_int_get(NRF_RNG_INT_VALRDY_MASK))
    {
        nrf_rng_event_clear(NRF_RNG_EVENT_VALRDY);
        uint32_t nrf_error = app_fifo_put(&m_rng_cb.rand_pool, nrf_rng_random_value_get());

        if ((FIFO_LENGTH(m_rng_cb.rand_pool) > m_rng_cb.rand_pool.buf_size_mask) || (nrf_error == NRF_ERROR_NO_MEM))
        {
            rng_stop();
        }
    }
}

#endif // SOFTDEVICE_PRESENT
