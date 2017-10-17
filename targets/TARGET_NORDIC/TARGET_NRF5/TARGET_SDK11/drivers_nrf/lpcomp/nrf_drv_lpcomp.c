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

#include "nrf_drv_lpcomp.h"

#include "nrf_assert.h"
#include "nrf_error.h"
#include "nrf_soc.h"
#include "nrf_drv_common.h"
#include "app_util_platform.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LPCOMP_IRQ			LPCOMP_IRQn
#define LPCOMP_IRQ_HANDLER 	LPCOMP_IRQHandler

static lpcomp_events_handler_t m_lpcomp_events_handler = NULL;
static nrf_drv_state_t         m_state = NRF_DRV_STATE_UNINITIALIZED;

static const nrf_drv_lpcomp_config_t m_default_config = NRF_DRV_LPCONF_DEFAULT_CONFIG;

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    #define IRQ_HANDLER_NAME    irq_handler_for_lpcomp
    #define IRQ_HANDLER      	static void IRQ_HANDLER_NAME(void)

	IRQ_HANDLER;
#else
    #define IRQ_HANDLER void LPCOMP_IRQ_HANDLER(void)
#endif // PERIPHERAL_RESOURCE_SHARING_ENABLED

static void lpcomp_execute_handler(nrf_lpcomp_event_t event, uint32_t event_mask)
{
    if ( nrf_lpcomp_event_check(event) && nrf_lpcomp_int_enable_check(event_mask) )
    {
        nrf_lpcomp_event_clear(event);

        m_lpcomp_events_handler(event);
    }
}


IRQ_HANDLER
{
    lpcomp_execute_handler(NRF_LPCOMP_EVENT_READY, LPCOMP_INTENSET_READY_Msk);
    lpcomp_execute_handler(NRF_LPCOMP_EVENT_DOWN, LPCOMP_INTENSET_DOWN_Msk);
    lpcomp_execute_handler(NRF_LPCOMP_EVENT_UP, LPCOMP_INTENSET_UP_Msk);
    lpcomp_execute_handler(NRF_LPCOMP_EVENT_CROSS, LPCOMP_INTENSET_CROSS_Msk);
}


ret_code_t nrf_drv_lpcomp_init(const nrf_drv_lpcomp_config_t * p_config,
                               lpcomp_events_handler_t   events_handler)
{
    if (m_state != NRF_DRV_STATE_UNINITIALIZED)
    { // LPCOMP driver is already initialized
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config;
    }

#if PERIPHERAL_RESOURCE_SHARING_ENABLED
    if (nrf_drv_common_per_res_acquire(NRF_LPCOMP, IRQ_HANDLER_NAME) != NRF_SUCCESS)
    {
        return NRF_ERROR_BUSY;
    }
#endif

    nrf_lpcomp_configure(&(p_config->hal) );

    if (events_handler)
    {
        m_lpcomp_events_handler = events_handler;
    }
    else
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    nrf_lpcomp_input_select(p_config->input);

    switch (p_config->hal.detection)
    {
        case NRF_LPCOMP_DETECT_UP:
            nrf_lpcomp_int_enable(LPCOMP_INTENSET_UP_Msk);
            break;

        case NRF_LPCOMP_DETECT_DOWN:
            nrf_lpcomp_int_enable(LPCOMP_INTENSET_DOWN_Msk);
            break;

        case NRF_LPCOMP_DETECT_CROSS:
            nrf_lpcomp_int_enable(LPCOMP_INTENSET_CROSS_Msk);
            break;

        default:
            break;
    }
    nrf_lpcomp_shorts_enable(NRF_LPCOMP_SHORT_READY_SAMPLE_MASK);

    nrf_drv_common_irq_enable(LPCOMP_IRQn, p_config->interrupt_priority);

    m_state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}


void nrf_drv_lpcomp_uninit(void)
{
    ASSERT(m_state != NRF_DRV_STATE_UNINITIALIZED);
    nrf_drv_common_irq_disable(LPCOMP_IRQn);
    nrf_drv_lpcomp_disable();
    m_state = NRF_DRV_STATE_UNINITIALIZED;
    m_lpcomp_events_handler = NULL;
}

void nrf_drv_lpcomp_enable(void)
{
    ASSERT(m_state == NRF_DRV_STATE_INITIALIZED);
    nrf_lpcomp_enable();
    nrf_lpcomp_task_trigger(NRF_LPCOMP_TASK_START);
    m_state = NRF_DRV_STATE_POWERED_ON;
}

void nrf_drv_lpcomp_disable(void)
{
    ASSERT(m_state == NRF_DRV_STATE_POWERED_ON);
    nrf_lpcomp_disable();
    nrf_lpcomp_task_trigger(NRF_LPCOMP_TASK_STOP);
    m_state = NRF_DRV_STATE_POWERED_ON;
}

void nrf_drv_lpcomp_event_handler_register(lpcomp_events_handler_t lpcomp_events_handler)
{
    m_lpcomp_events_handler = lpcomp_events_handler;
}


