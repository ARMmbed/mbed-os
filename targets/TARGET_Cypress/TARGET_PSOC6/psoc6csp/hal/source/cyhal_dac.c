/***************************************************************************/ /**
* \file cyhal_dac.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Digital/Analog converter.
* This interface abstracts out the chip specific details. If any chip specific
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

#include <limits.h>

#include "cyhal_analog_common.h"
#include "cyhal_dac.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_interconnect.h"
#include "cyhal_utils.h"
#include "cy_pdl.h"

#if defined(CY_IP_MXS40PASS_CTDAC_INSTANCES)

#if defined(__cplusplus)
extern "C"
{
#endif

#define CYHAL_DAC_VALUE_SCALING_FACTOR (UINT16_MAX / CY_CTDAC_UNSIGNED_MAX_CODE_VALUE)

static CTDAC_Type *const cyhal_ctdac_base[] = {
#if (CY_IP_MXS40PASS_CTDAC_INSTANCES > 0)
    CTDAC0,
#endif
#if (CY_IP_MXS40PASS_CTDAC_INSTANCES > 1)
    CTDAC1,
#endif
#if (CY_IP_MXS40PASS_CTDAC_INSTANCES > 2)
#warning Unhandled CTDAC instance count
#endif
};

static const cy_stc_ctdac_config_t CYHAL_CTDAC_DEFAULT_CONFIG =
{
    .refSource = CY_CTDAC_REFSOURCE_VDDA,
    .formatMode = CY_CTDAC_FORMAT_UNSIGNED,
    .updateMode = CY_CTDAC_UPDATE_DIRECT_WRITE,
    .deglitchMode = CY_CTDAC_DEGLITCHMODE_UNBUFFERED,
    .outputMode = CY_CTDAC_OUTPUT_VALUE,
    .outputBuffer = CY_CTDAC_OUTPUT_UNBUFFERED,
    .deepSleep = CY_CTDAC_DEEPSLEEP_DISABLE,
    .deglitchCycles = 0,
    .value = 0,
    .nextValue = 0,
    .enableInterrupt = true,
    .configClock = false,
    // The following values are simply placeholders because configClock is false
    .dividerType = CY_SYSCLK_DIV_8_BIT,
    .dividerNum = 0,
    .dividerIntValue = 0,
    .dividerFracValue = 0,
};

/*******************************************************************************
*       DAC HAL Functions
*******************************************************************************/
cy_rslt_t cyhal_dac_init(cyhal_dac_t *obj, cyhal_gpio_t pin)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t result = CY_RSLT_SUCCESS;

    if (CYHAL_NC_PIN_VALUE == pin)
        result = CYHAL_DAC_RSLT_BAD_ARGUMENT;

    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource.type = CYHAL_RSC_INVALID;
        obj->base = NULL;
        obj->pin = CYHAL_NC_PIN_VALUE;
    }

    const cyhal_resource_pin_mapping_t *map = CY_UTILS_GET_RESOURCE(pin, cyhal_pin_map_pass_ctdac_voutsw);
    if (NULL == map)
        result =  CYHAL_DAC_RSLT_BAD_ARGUMENT;

    cyhal_resource_inst_t dac_inst;
    if (CY_RSLT_SUCCESS == result)
    {
        dac_inst = *map->inst;
        result = cyhal_hwmgr_reserve(&dac_inst);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource = dac_inst;

        obj->base = cyhal_ctdac_base[dac_inst.block_num];

        // We don't need any special configuration of the pin, so just reserve it
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(pin);
        if (CY_RSLT_SUCCESS == (result = cyhal_hwmgr_reserve(&pinRsc)))
            obj->pin = pin;
    }

    if (CY_RSLT_SUCCESS == result)
        result = cyhal_connect_pin(map);

    if (CY_RSLT_SUCCESS == result)
    {
        result = (cy_rslt_t)Cy_CTDAC_Init(obj->base, &CYHAL_CTDAC_DEFAULT_CONFIG);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        cyhal_analog_init();
        Cy_CTDAC_Enable(obj->base);
    }
    else
    {
        cyhal_dac_free(obj);
    }
    return result;
}

void cyhal_dac_free(cyhal_dac_t *obj)
{
    if (NULL != obj && NULL != obj->base)
    {
        Cy_CTDAC_Disable(obj->base);
        cyhal_analog_free();

        cyhal_hwmgr_free(&obj->resource);

        cyhal_utils_release_if_used(&(obj->pin));

        obj->base = NULL;
    }
}

void cyhal_dac_write(const cyhal_dac_t *obj, uint16_t value)
{
    uint16_t scaled_value = value / CYHAL_DAC_VALUE_SCALING_FACTOR;
    Cy_CTDAC_SetValue(obj->base, scaled_value);
}

uint16_t cyhal_dac_read(cyhal_dac_t *obj)
{
    uint16_t value = obj->base->CTDAC_VAL;
    uint16_t scaled_value = value * CYHAL_DAC_VALUE_SCALING_FACTOR;
    return scaled_value;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXS40PASS_CTDAC_INSTANCES) */
