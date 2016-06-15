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

#include <stddef.h>
#include "nrf_drv_common.h"
#include "nrf_assert.h"
#include "app_util_platform.h"

#ifdef SOFTDEVICE_PRESENT
#include "nrf_soc.h"
#endif


#if PERIPHERAL_RESOURCE_SHARING_ENABLED

typedef struct {
    nrf_drv_irq_handler_t handler;
    bool                  acquired;
} shared_resource_t;

// SPIM0, SPIS0, SPI0, TWIM0, TWIS0, TWI0
#if (SPI0_ENABLED || SPIS0_ENABLED || TWI0_ENABLED || TWIS0_ENABLED)
    #define SERIAL_BOX_0_IN_USE
    // [this checking may need a different form in unit tests, hence macro]
    #ifndef IS_SERIAL_BOX_0
        #define IS_SERIAL_BOX_0(p_per_base)  (p_per_base == NRF_SPI0)
    #endif

    static shared_resource_t m_serial_box_0 = { .acquired = false };
    void SPI0_TWI0_IRQHandler(void)
    {
        ASSERT(m_serial_box_0.handler);
        m_serial_box_0.handler();
    }
#endif // (SPI0_ENABLED || SPIS0_ENABLED || TWI0_ENABLED || TWIS0_ENABLED)

// SPIM1, SPIS1, SPI1, TWIM1, TWIS1, TWI1
#if (SPI1_ENABLED || SPIS1_ENABLED || TWI1_ENABLED || TWIS1_ENABLED)
    #define SERIAL_BOX_1_IN_USE
    // [this checking may need a different form in unit tests, hence macro]
    #ifndef IS_SERIAL_BOX_1
        #define IS_SERIAL_BOX_1(p_per_base)  (p_per_base == NRF_SPI1)
    #endif

    static shared_resource_t m_serial_box_1 = { .acquired = false };
    void SPI1_TWI1_IRQHandler(void)
    {
        ASSERT(m_serial_box_1.handler);
        m_serial_box_1.handler();
    }
#endif // (SPI1_ENABLED || SPIS1_ENABLED || TWI1_ENABLED || TWIS1_ENABLED)

// SPIM2, SPIS2, SPI2
#if (SPI2_ENABLED || SPIS2_ENABLED)
    #define SERIAL_BOX_2_IN_USE
    // [this checking may need a different form in unit tests, hence macro]
    #ifndef IS_SERIAL_BOX_2
        #define IS_SERIAL_BOX_2(p_per_base)  (p_per_base == NRF_SPI2)
    #endif

    static shared_resource_t m_serial_box_2 = { .acquired = false };
    void SPIM2_SPIS2_SPI2_IRQHandler(void)
    {
        ASSERT(m_serial_box_2.handler);
        m_serial_box_2.handler();
    }
#endif // (SPI2_ENABLED || SPIS2_ENABLED)

// COMP, LPCOMP
#if (COMP_ENABLED || LPCOMP_ENABLED)
	#define COMP_LPCOMP_IN_USE

	#ifndef IS_COMP_LPCOMP
		#define IS_COMP_LPCOMP(p_per_base)  ((p_per_base) == NRF_LPCOMP)
	#endif

    static shared_resource_t m_comp_lpcomp = { .acquired = false };
    void LPCOMP_IRQHandler(void)
    {
    	ASSERT(m_comp_lpcomp.handler);
    	m_comp_lpcomp.handler();
    }
#endif	// (COMP_ENABLED || LPCOMP_ENABLED)

#if defined(SERIAL_BOX_0_IN_USE) || \
    defined(SERIAL_BOX_1_IN_USE) || \
    defined(SERIAL_BOX_2_IN_USE) || \
	defined(COMP_LPCOMP_IN_USE)
static ret_code_t acquire_shared_resource(shared_resource_t * p_resource,
                                          nrf_drv_irq_handler_t handler)
{
    bool busy = false;

    CRITICAL_REGION_ENTER();
    if (p_resource->acquired)
    {
        busy = true;
    }
    else
    {
        p_resource->acquired = true;
    }
    CRITICAL_REGION_EXIT();

    if (busy)
    {
        return NRF_ERROR_BUSY;
    }

    p_resource->handler = handler;
    return NRF_SUCCESS;
}
#endif

ret_code_t nrf_drv_common_per_res_acquire(void const * p_per_base,
                                          nrf_drv_irq_handler_t handler)
{
#ifdef SERIAL_BOX_0_IN_USE
    if (IS_SERIAL_BOX_0(p_per_base))
    {
        return acquire_shared_resource(&m_serial_box_0, handler);
    }
#endif

#ifdef SERIAL_BOX_1_IN_USE
    if (IS_SERIAL_BOX_1(p_per_base))
    {
        return acquire_shared_resource(&m_serial_box_1, handler);
    }
#endif

#ifdef SERIAL_BOX_2_IN_USE
    if (IS_SERIAL_BOX_2(p_per_base))
    {
        return acquire_shared_resource(&m_serial_box_2, handler);
    }
#endif

#ifdef COMP_LPCOMP_IN_USE
    if (IS_COMP_LPCOMP(p_per_base))
    {
    	return acquire_shared_resource(&m_comp_lpcomp, handler);
    }
#endif

    return NRF_ERROR_INVALID_PARAM;
}

void nrf_drv_common_per_res_release(void const * p_per_base)
{
#ifdef SERIAL_BOX_0_IN_USE
    if (IS_SERIAL_BOX_0(p_per_base))
    {
        m_serial_box_0.acquired = false;
    }
    else
#endif

#ifdef SERIAL_BOX_1_IN_USE
    if (IS_SERIAL_BOX_1(p_per_base))
    {
        m_serial_box_1.acquired = false;
    }
    else
#endif

#ifdef SERIAL_BOX_2_IN_USE
    if (IS_SERIAL_BOX_2(p_per_base))
    {
        m_serial_box_2.acquired = false;
    }
    else
#endif

#ifdef COMP_LPCOMP_IN_USE
	if (IS_COMP_LPCOMP(p_per_base))
    {
        m_comp_lpcomp.acquired = false;
    }
	else
#endif

    {}
}

#endif // PERIPHERAL_RESOURCE_SHARING_ENABLED


void nrf_drv_common_irq_enable(IRQn_Type IRQn, uint8_t priority)
{

#ifdef SOFTDEVICE_PRESENT
    ASSERT((priority == APP_IRQ_PRIORITY_LOW) || (priority == APP_IRQ_PRIORITY_HIGH));
#endif

    NVIC_SetPriority(IRQn, priority);
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_EnableIRQ(IRQn);
}
