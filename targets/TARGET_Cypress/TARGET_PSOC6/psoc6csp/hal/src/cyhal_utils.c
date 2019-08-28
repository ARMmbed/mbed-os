/***************************************************************************//**
* \file cyhal_utils.c
*
* \brief
* Provides utility functions for working with the PSoC 6 HAL implementation.
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

#include "cy_result.h"
#include "cyhal_utils.h"
#include "cyhal_hwmgr.h"
#include "cyhal_interconnect.h"

#if defined(__cplusplus)
extern "C"
{
#endif

const cyhal_resource_pin_mapping_t *cyhal_utils_get_resource(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t* mappings, size_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        if (pin == mappings[i].pin)
        {
            return &mappings[i];
        }
    }
    return NULL;
}

void cyhal_utils_disconnect_and_free(cyhal_gpio_t pin)
{
    cy_rslt_t rslt = cyhal_disconnect_pin(pin);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    cyhal_resource_inst_t rsc = cyhal_utils_get_gpio_resource(pin);
    cyhal_hwmgr_free(&rsc);
}

#if defined(__cplusplus)
}
#endif
