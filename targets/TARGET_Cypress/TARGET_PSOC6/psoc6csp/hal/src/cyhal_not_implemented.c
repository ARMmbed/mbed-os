/***************************************************************************//**
* \file cyhal_not_implemented.c
*
* \brief
* This file contains place holder functions for drivers that are not yet
* implemented but are referenced by mbed and cause build failures on some
* toolchains. As the drivers are implemented, these items need to be deleted.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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

#include "cyhal_adc.h"
#include "cyhal_dac.h"
#include "cyhal_dma.h"

uint8_t* cyhal_dest_to_mux;
uint8_t* cyhal_mux_dest_index;
uint8_t* cyhal_source_count_per_mux;
cyhal_source_t** cyhal_mux_to_sources;
cyhal_dest_t* cyhal_intra_trigger_source;

cy_rslt_t cyhal_dma_init(cyhal_dma_t *obj, uint8_t priority, cyhal_dma_direction_t direction)
{
    return CY_RSLT_SUCCESS;
}

void cyhal_dma_free(cyhal_dma_t *obj)
{
}

cy_rslt_t cyhal_dma_setup_transfer(cyhal_dma_t *obj, const cyhal_dma_cfg_t *cfg)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_dma_start_transfer(cyhal_dma_t *obj)
{
    return CY_RSLT_SUCCESS;
}

bool cyhal_dma_busy(cyhal_dma_t *obj)
{
    return false;
}

void cyhal_dma_register_irq(cyhal_dma_t *obj, cyhal_dma_irq_handler_t handler, void *handler_arg)
{
}

void cyhal_dma_irq_enable(cyhal_dma_t *obj, cyhal_dma_irq_event_t event, bool enable)
{
}
