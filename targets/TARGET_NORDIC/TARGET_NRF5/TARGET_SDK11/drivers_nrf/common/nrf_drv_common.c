/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
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
    #ifdef NRF51
            ASSERT((priority == APP_IRQ_PRIORITY_LOW) || (priority == APP_IRQ_PRIORITY_HIGH));
    #elif defined(NRF52)
            ASSERT((priority == APP_IRQ_PRIORITY_LOWEST) || (priority == APP_IRQ_PRIORITY_HIGH));
    #endif    
#endif

    NVIC_SetPriority(IRQn, priority);
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_EnableIRQ(IRQn);
}
