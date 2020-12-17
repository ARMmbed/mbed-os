/***************************************************************************//**
* \file cyhal_dma.c
*
* \brief
* Implements a high level interface for interacting with the Cypress DMA.
* This implementation abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cyhal_dma_dmac.h"
#include "cyhal_dma_dw.h"
#include "cyhal_system.h"
#include "cyhal_hwmgr.h"

/**
* \addtogroup group_hal_dma DMA (Direct Memory Access)
* \ingroup group_hal
* \{
*/

#if defined(CY_IP_M4CPUSS_DMAC) || defined(CY_IP_M4CPUSS_DMA)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

cy_rslt_t cyhal_dma_init(cyhal_dma_t *obj, uint8_t priority, cyhal_dma_direction_t direction)
{
    CY_ASSERT(NULL != obj);

#if !defined(CY_IP_M4CPUSS_DMAC) && defined(CY_IP_M4CPUSS_DMA)
    /* Only DW available. Ignore direction for purpose of choosing DMA type. */
    CY_UNUSED_PARAMETER(direction);
    return _cyhal_dma_dw_init(obj, priority);
#elif defined(CY_IP_M4CPUSS_DMAC) && !defined(CY_IP_M4CPUSS_DMA)
    /* Only DMAC available. Ignore direction for purpose of choosing DMA type. */
    CY_UNUSED_PARAMETER(direction);
    return _cyhal_dma_dmac_init(obj, priority);
#else
    /* DMAC is designed with high memory bandwidth for memory to memory
     * transfers so prefer it when direction is MEM2MEM. Otherwise prefer
     * Datawire as it is designed for low latency memory to peripheral or
     * peripheral to memory transfers. Note: Both DMA types can handle any
     * direction value so using a non-ideal DMA type is ok.*/
    cy_rslt_t rslt;
    if(direction == CYHAL_DMA_DIRECTION_MEM2MEM)
    {
        rslt = _cyhal_dma_dmac_init(obj, priority);
        /* If no DMAC channels are available fall back on DW. */
        if(CYHAL_HWMGR_RSLT_ERR_NONE_FREE == rslt)
            rslt = _cyhal_dma_dw_init(obj, priority);
    }
    else
    {
        rslt = _cyhal_dma_dw_init(obj, priority);
        /* If no DW channels are available fall back on DMAC. */
        if(CYHAL_HWMGR_RSLT_ERR_NONE_FREE == rslt)
            rslt = _cyhal_dma_dmac_init(obj, priority);
    }
    return rslt;
#endif
}

void cyhal_dma_free(cyhal_dma_t *obj)
{
    CY_ASSERT(NULL != obj);

    CY_ASSERT(!cyhal_dma_is_busy(obj));

#ifdef CY_IP_M4CPUSS_DMAC
    if(obj->resource.type == CYHAL_RSC_DMA)
    {
        _cyhal_dma_dmac_free(obj);
    }
#endif
#ifdef CY_IP_M4CPUSS_DMA
    if(obj->resource.type == CYHAL_RSC_DW)
    {
        _cyhal_dma_dw_free(obj);
    }
#endif
}

cy_rslt_t cyhal_dma_configure(cyhal_dma_t *obj, const cyhal_dma_cfg_t *cfg)
{
    CY_ASSERT(NULL != obj);

#ifdef CY_IP_M4CPUSS_DMAC
    if(obj->resource.type == CYHAL_RSC_DMA)
    {
        return _cyhal_dma_dmac_configure(obj, cfg);
    }
#endif
#ifdef CY_IP_M4CPUSS_DMA
    if(obj->resource.type == CYHAL_RSC_DW)
    {
        return _cyhal_dma_dw_configure(obj, cfg);
    }
#endif

    /* Control should never reach here but return value anyway to appease
     * compilers */
    CY_ASSERT(false);
    return CYHAL_DMA_RSLT_FATAL_UNSUPPORTED_HARDWARE;
}

cy_rslt_t cyhal_dma_start_transfer(cyhal_dma_t *obj)
{
    CY_ASSERT(NULL != obj);

#ifdef CY_IP_M4CPUSS_DMAC
    if(obj->resource.type == CYHAL_RSC_DMA)
    {
        return _cyhal_dma_dmac_start_transfer(obj);
    }
#endif
#ifdef CY_IP_M4CPUSS_DMA
    if(obj->resource.type == CYHAL_RSC_DW)
    {
        return _cyhal_dma_dw_start_transfer(obj);
    }
#endif

    /* Control should never reach here but return value anyway to appease
     * compilers */
    CY_ASSERT(false);
    return CYHAL_DMA_RSLT_FATAL_UNSUPPORTED_HARDWARE;
}

bool cyhal_dma_is_busy(cyhal_dma_t *obj)
{
    CY_ASSERT(NULL != obj);

#ifdef CY_IP_M4CPUSS_DMAC
    if(obj->resource.type == CYHAL_RSC_DMA)
    {
        return _cyhal_dma_dmac_is_busy(obj);
    }
#endif
#ifdef CY_IP_M4CPUSS_DMA
    if(obj->resource.type == CYHAL_RSC_DW)
    {
        return _cyhal_dma_dw_is_busy(obj);
    }
#endif

    /* Control should never reach here but return value anyway to appease
     * compilers */
    CY_ASSERT(false);
    return CYHAL_DMA_RSLT_FATAL_UNSUPPORTED_HARDWARE;
}

void cyhal_dma_register_callback(cyhal_dma_t *obj, cyhal_dma_event_callback_t callback, void *callback_arg)
{
    CY_ASSERT(NULL != obj);

    uint32_t saved_intr_status = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress)callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(saved_intr_status);
}

void cyhal_dma_enable_event(cyhal_dma_t *obj, cyhal_dma_event_t event, uint8_t intr_priority, bool enable)
{
    CY_ASSERT(NULL != obj);

#ifdef CY_IP_M4CPUSS_DMAC
    if(obj->resource.type == CYHAL_RSC_DMA)
    {
        _cyhal_dma_dmac_enable_event(obj, event, intr_priority, enable);
    }
#endif
#ifdef CY_IP_M4CPUSS_DMA
    if(obj->resource.type == CYHAL_RSC_DW)
    {
        _cyhal_dma_dw_enable_event(obj, event, intr_priority, enable);
    }
#endif
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* defined(CY_IP_M4CPUSS_DMAC) || defined(CY_IP_M4CPUSS_DMA) */

/** \} group_hal_dma */
