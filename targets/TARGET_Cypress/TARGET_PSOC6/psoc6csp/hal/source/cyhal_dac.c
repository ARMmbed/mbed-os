/*******************************************************************************
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
#include <math.h>

#include "cyhal_analog_common.h"
#include "cyhal_dac.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_utils.h"
#include "cyhal_syspm.h"
#include "cy_pdl.h"

/**
* \addtogroup group_hal_impl_dac DAC
* \ingroup group_hal_impl
*
* \section group_hal_impl_dac_power Power Level Mapping
* The following table shows how the HAL-defined power levels map to the hardware-specific power levels
* when cyhal_dac uses output pin buffered mode (with opamp). Unbuffered mode only supports ON and OFF.
* | HAL Power Level                | Opamp Power Level   |
* | ------------------------------ | ------------------- |
* | @ref CYHAL_POWER_LEVEL_HIGH    | CY_CTB_POWER_HIGH   |
* | @ref CYHAL_POWER_LEVEL_MEDIUM  | CY_CTB_POWER_MEDIUM |
* | @ref CYHAL_POWER_LEVEL_LOW     | CY_CTB_POWER_LOW    |
* | @ref CYHAL_POWER_LEVEL_DEFAULT | CY_CTB_POWER_MEDIUM |
*
* cyhal_dac automatically choose between buffered and unbuffered mode by selecting pin.
* Unbuffered mode - dac pin, buffered - opamp pin.
* Buffered mode take care of reserving and configuring the opamp (OA0).
* If AREF voltage reference source is selected cyhal_dac takes care of reserving and configuring the opamp (OA1).
* By default cyhal_dac use VDDA voltage reference source. Use @ref cyhal_dac_set_reference() to change
* between @ref CYHAL_DAC_REF_VDDA and @ref CYHAL_DAC_REF_VREF voltage reference sources.
*/


#if defined(CY_IP_MXS40PASS_CTDAC_INSTANCES)

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_DAC_VALUE_SCALING_FACTOR (UINT16_MAX / CY_CTDAC_UNSIGNED_MAX_CODE_VALUE)

static CTDAC_Type *const _cyhal_dac_base[] = {
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

static const cy_stc_ctdac_config_t _CYHAL_DAC_DEFAULT_CONFIG =
{
    .refSource = CY_CTDAC_REFSOURCE_VDDA,
    .formatMode = CY_CTDAC_FORMAT_UNSIGNED,
    .updateMode = CY_CTDAC_UPDATE_DIRECT_WRITE,
    .deglitchMode = CY_CTDAC_DEGLITCHMODE_UNBUFFERED,
    .outputMode = CY_CTDAC_OUTPUT_VALUE,
    //.outputBuffer is configured automatically depending on pin choice
    .deepSleep = CY_CTDAC_DEEPSLEEP_ENABLE,
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

static const cy_stc_ctb_opamp_config_t cyhal_opamp_default_config =
{
    .deepSleep    = CY_CTB_DEEPSLEEP_ENABLE,
    .oaPower      = CY_CTB_POWER_MEDIUM,
    .oaMode       = CY_CTB_MODE_OPAMP1X,
    .oaPump       = CY_CTB_PUMP_ENABLE,
    .oaCompEdge   = CY_CTB_COMP_EDGE_DISABLE,
    .oaCompLevel  = CY_CTB_COMP_DSI_TRIGGER_OUT_LEVEL,
    .oaCompBypass = CY_CTB_COMP_BYPASS_SYNC,
    .oaCompHyst   = CY_CTB_COMP_HYST_DISABLE,
    .oaCompIntrEn = true,
};

static void _cyhal_dac_unbuffered_set_power(cyhal_dac_t *obj, cyhal_power_level_t hal_power)
{
    switch(hal_power)
        {
            case CYHAL_POWER_LEVEL_OFF:
                Cy_CTDAC_Disable(obj->base_dac);
                break;
            case CYHAL_POWER_LEVEL_LOW:
            case CYHAL_POWER_LEVEL_MEDIUM:
            case CYHAL_POWER_LEVEL_HIGH:
            case CYHAL_POWER_LEVEL_DEFAULT:
                Cy_CTDAC_Enable(obj->base_dac);
                break;
            default:
                CY_ASSERT(false);
                Cy_CTDAC_Disable(obj->base_dac);
                break;
        }
}

static uint32_t _cyhal_dac_convert_reference(cyhal_dac_ref_t ref)
{
    switch(ref)
        {
            case CYHAL_DAC_REF_VDDA:
                return CY_CTDAC_REFSOURCE_VDDA;
            case CYHAL_DAC_REF_VREF:
                return CY_CTDAC_REFSOURCE_EXTERNAL;
            default:
                CY_ASSERT(false);
                return CY_CTDAC_REFSOURCE_VDDA;
        }
}

static uint32_t _cyhal_dac_configure_oa0(cyhal_dac_t *obj, bool init)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    if (init && (CYHAL_RSC_INVALID != obj->resource_opamp.type))
    {
        /* Configure OA0 for buffered output */
        /* OA0 require non defaut CY_CTB_MODE_OPAMP10X */
        cy_stc_ctb_opamp_config_t config = cyhal_opamp_default_config;
        config.oaMode = CY_CTB_MODE_OPAMP10X;
        result = Cy_CTB_OpampInit(obj->base_opamp, CY_CTB_OPAMP_0, &config);
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_OA0_SW, CY_CTB_SW_OA0_NEG_OUT_MASK | CY_CTB_SW_OA0_OUT_SHORT_1X_10X_MASK, CY_CTB_SWITCH_CLOSE);
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_CTD_SW, CY_CTB_SW_CTD_OUT_CHOLD_MASK | CY_CTB_SW_CTD_CHOLD_OA0_POS_MASK, CY_CTB_SWITCH_CLOSE);
    }
    else
    {
        /* Open switches OA0 if used */
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_OA0_SW, CY_CTB_SW_OA0_NEG_OUT_MASK | CY_CTB_SW_OA0_OUT_SHORT_1X_10X_MASK, CY_CTB_SWITCH_OPEN);
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_CTD_SW, CY_CTB_SW_CTD_OUT_CHOLD_MASK | CY_CTB_SW_CTD_CHOLD_OA0_POS_MASK, CY_CTB_SWITCH_OPEN);
    }
    return result;
}

static uint32_t _cyhal_dac_configure_oa1(cyhal_dac_t *obj, bool init)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    if (init && (CYHAL_RSC_INVALID != obj->resource_aref_opamp.type))
    {
        /* Configure OA1 for buffered (AREF) voltage reference source */
        result = Cy_CTB_OpampInit(obj->base_opamp, CY_CTB_OPAMP_1, &cyhal_opamp_default_config);
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_OA1_SW, CY_CTB_SW_OA1_NEG_OUT_MASK | CY_CTB_SW_OA1_POS_AREF_MASK, CY_CTB_SWITCH_CLOSE);
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_CTD_SW, CY_CTB_SW_CTD_REF_OA1_OUT_MASK, CY_CTB_SWITCH_CLOSE);
    }
    else
    {
        /* Open switches OA1 if used */
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_OA1_SW, CY_CTB_SW_OA1_NEG_OUT_MASK | CY_CTB_SW_OA1_POS_AREF_MASK, CY_CTB_SWITCH_OPEN);
        Cy_CTB_SetAnalogSwitch(obj->base_opamp, CY_CTB_SWITCH_CTD_SW, CY_CTB_SW_CTD_REF_OA1_OUT_MASK, CY_CTB_SWITCH_OPEN);
    }
    return result;
}

/*******************************************************************************
*       DAC HAL Functions
*******************************************************************************/
cy_rslt_t cyhal_dac_init(cyhal_dac_t *obj, cyhal_gpio_t pin)
{
    CY_ASSERT(NULL != obj);

    /* Initial values */
    cy_rslt_t result = CY_RSLT_SUCCESS;
    obj->resource_dac.type = CYHAL_RSC_INVALID;
    obj->resource_opamp.type = CYHAL_RSC_INVALID;
    obj->pin = CYHAL_NC_PIN_VALUE;
    obj->resource_aref_opamp.type = CYHAL_RSC_INVALID;

    const cyhal_resource_pin_mapping_t *opamp_map=NULL;

    #ifdef CYHAL_PIN_MAP_DAC_CTDAC_VOUTSW
    const cyhal_resource_pin_mapping_t *dac_map = _CYHAL_UTILS_GET_RESOURCE(pin, cyhal_pin_map_dac_ctdac_voutsw);
    #else
    const cyhal_resource_pin_mapping_t *dac_map = NULL;
    #endif
    if (NULL == dac_map)
    {
        /* Try to get buffered output pin if unbuffered is not specified.  */
        #ifdef CYHAL_PIN_MAP_OPAMP_OUT_10X
        opamp_map = _CYHAL_UTILS_GET_RESOURCE(pin, cyhal_pin_map_opamp_out_10x);
        #endif
    }

    /* Check if mapping is successful */
    if ((NULL == dac_map) && (NULL == opamp_map))
    {
        result = CYHAL_DAC_RSLT_BAD_ARGUMENT;
    }

    /* Verify if opamp instance 0 is selected, buffered output can be connected to OA0 */
    if ((NULL != opamp_map) && (0 != (opamp_map->inst->block_num % _CYHAL_OPAMP_PER_CTB)))
    {
        result = CYHAL_DAC_RSLT_BAD_ARGUMENT;
    }

    cyhal_resource_inst_t opamp_instance;
    cyhal_resource_inst_t dac_instance;

    if (NULL != opamp_map)
    {
        dac_instance.type = CYHAL_RSC_DAC;
        dac_instance.block_num = opamp_map->inst->block_num % _CYHAL_OPAMP_PER_CTB;
    }
    else if (CY_RSLT_SUCCESS == result)
    {
        dac_instance = *dac_map->inst;
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_reserve(&dac_instance);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource_dac = dac_instance;
    }

    if ((NULL != opamp_map) && (CY_RSLT_SUCCESS == result))
    {
        opamp_instance = *opamp_map->inst;
        result = cyhal_hwmgr_reserve(&opamp_instance);
        if (CY_RSLT_SUCCESS == result)
        {
            obj->resource_opamp = opamp_instance;
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        obj->base_dac = _cyhal_dac_base[dac_instance.block_num];
        obj->base_opamp = _cyhal_ctb_base[dac_instance.block_num];

        if (NULL != dac_map)
        {
            result = _cyhal_utils_reserve_and_connect(pin, dac_map);
        }
        else if (NULL != opamp_map)
        {
            result = _cyhal_utils_reserve_and_connect(pin, opamp_map);
        }
        else
        {
            result =  CYHAL_DAC_RSLT_BAD_ARGUMENT;
        }

        if (CY_RSLT_SUCCESS == result)
        {
            obj->pin = pin;
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        /* Verify is output buffered or not */
        cy_stc_ctdac_config_t config = _CYHAL_DAC_DEFAULT_CONFIG;
        config.outputBuffer = (obj->resource_opamp.type != CYHAL_RSC_INVALID) ? CY_CTDAC_OUTPUT_BUFFERED : CY_CTDAC_OUTPUT_UNBUFFERED;
        result = (cy_rslt_t)Cy_CTDAC_Init(obj->base_dac, &config);
    }

    if ((CY_RSLT_SUCCESS == result) && (obj->resource_opamp.type != CYHAL_RSC_INVALID))
    {
        /* Init OA0 is for buffered output, don't touch OA1 im may be used by opamp or comp */
        result = _cyhal_dac_configure_oa0(obj, true);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        (obj->resource_opamp.type != CYHAL_RSC_INVALID) ? cyhal_analog_ctb_init(obj->base_opamp)
                                                        : _cyhal_analog_init();
    }

    if (result == CY_RSLT_SUCCESS)
    {
        Cy_CTDAC_Enable(obj->base_dac);
    }
    else
    {
        /* Freeup resources in case of failure */
        cyhal_dac_free(obj);
    }
    return result;
}

void cyhal_dac_free(cyhal_dac_t *obj)
{
    if (NULL != obj->base_dac)
    {
        /* Power off OA1 if used */
        if (obj->resource_aref_opamp.type != CYHAL_RSC_INVALID)
        {
            Cy_CTB_SetPower(obj->base_opamp, CY_CTB_OPAMP_1, (cy_en_ctb_power_t)_cyhal_opamp_convert_power(CYHAL_POWER_LEVEL_OFF), CY_CTB_PUMP_ENABLE);
            (void)_cyhal_dac_configure_oa1(obj, false);
        }

        /* Power off OA0 if used */
        if (obj->resource_opamp.type != CYHAL_RSC_INVALID)
        {
            Cy_CTB_SetPower(obj->base_opamp, CY_CTB_OPAMP_0, (cy_en_ctb_power_t)_cyhal_opamp_convert_power(CYHAL_POWER_LEVEL_OFF), CY_CTB_PUMP_ENABLE);
            (void)_cyhal_dac_configure_oa0(obj, false);
        }

        /* Disable CTB block if used */
        if ((obj->resource_aref_opamp.type != CYHAL_RSC_INVALID) || (obj->resource_opamp.type != CYHAL_RSC_INVALID))
        {
            cyhal_analog_ctb_free(obj->base_opamp);
        }
        else
        {
            _cyhal_analog_free();
        }

        Cy_CTDAC_Disable(obj->base_dac);

        cyhal_hwmgr_free(&obj->resource_dac);
        cyhal_hwmgr_free(&obj->resource_opamp);
        cyhal_hwmgr_free(&obj->resource_aref_opamp);

        _cyhal_utils_release_if_used(&(obj->pin));

        obj->base_dac = NULL;
        obj->base_opamp = NULL;
    }
}

void cyhal_dac_write(const cyhal_dac_t *obj, uint16_t value)
{
    uint16_t scaled_value = value / _CYHAL_DAC_VALUE_SCALING_FACTOR;
    Cy_CTDAC_SetValue(obj->base_dac, scaled_value);
}

cy_rslt_t cyhal_dac_write_mv(const cyhal_dac_t *obj, uint16_t value)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    uint32_t reference_voltage_mv = 0;

    if (obj->resource_aref_opamp.type == CYHAL_RSC_INVALID)
    {
        reference_voltage_mv = cyhal_syspm_get_supply_voltage(CYHAL_VOLTAGE_SUPPLY_VDDA);

        if (0 == reference_voltage_mv)
        {
            result = CYHAL_DAC_RSLT_BAD_REF_VOLTAGE;
        }
    }
    else
    {
        /* AREF voltage in millivolts */
        reference_voltage_mv = 1200;
    }

    if (result == CY_RSLT_SUCCESS)
    {
        uint32_t count =  (value << 12) / reference_voltage_mv;
        Cy_CTDAC_SetValue(obj->base_dac, count);
    }

    return result;
}

uint16_t cyhal_dac_read(cyhal_dac_t *obj)
{
    uint16_t value = (uint16_t)obj->base_dac->CTDAC_VAL;
    uint16_t scaled_value = value * _CYHAL_DAC_VALUE_SCALING_FACTOR;
    return scaled_value;
}

cy_rslt_t cyhal_dac_set_reference(cyhal_dac_t *obj, cyhal_dac_ref_t ref)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    if (CYHAL_DAC_REF_VDDA == ref)
    {
        /* Unreserve OA1, not needed for VDDA */
        if (obj->resource_aref_opamp.type != CYHAL_RSC_INVALID)
        {
            cyhal_hwmgr_free(&obj->resource_aref_opamp);
            obj->resource_aref_opamp.type = CYHAL_RSC_INVALID;

            /* Freeup OA1. Not needed when VDDA reference is set  */
            result = _cyhal_dac_configure_oa1(obj, false);
        }
    }
    else if (CYHAL_DAC_REF_VREF == ref)
    {
        if (obj->resource_aref_opamp.type == CYHAL_RSC_INVALID)
        {
            /* Reserve OA1 to be able connect to AREF voltage source */
            obj->resource_aref_opamp.type = CYHAL_RSC_OPAMP;
            obj->resource_aref_opamp.block_num = (obj->resource_dac.block_num * _CYHAL_OPAMP_PER_CTB) + 1;
            obj->resource_aref_opamp.channel_num = 0;

            result = cyhal_hwmgr_reserve(&obj->resource_aref_opamp);
            if (CY_RSLT_SUCCESS != result)
            {
                obj->resource_aref_opamp.type = CYHAL_RSC_INVALID;
            }
            else
            {
                /* Init OA1 to be able connect to AREF voltage source. OA0 is untouched */
                result = _cyhal_dac_configure_oa1(obj, true);
            }
        }
    }
    else
    {
        result = CYHAL_DAC_RSLT_BAD_REF_VOLTAGE;
    }

    if (result == CY_RSLT_SUCCESS)
    {
        Cy_CTDAC_SetRef(obj->base_dac, (cy_en_ctdac_ref_source_t)_cyhal_dac_convert_reference(ref));
    }

    return result;
}

cy_rslt_t cyhal_dac_set_power(cyhal_dac_t *obj, cyhal_power_level_t power)
{
    if ((obj->resource_opamp.type != CYHAL_RSC_INVALID) || (obj->resource_aref_opamp.type != CYHAL_RSC_INVALID))
    {
        /* Safe convert power level from HAL (cyhal_power_level_t) to PDL (cy_en_ctb_power_t) */
        cy_en_ctb_power_t power_level = (cy_en_ctb_power_t)_cyhal_opamp_convert_power(power);
        Cy_CTB_SetPower(obj->base_opamp, _cyhal_opamp_convert_sel(obj->resource_opamp.block_num), power_level, CY_CTB_PUMP_ENABLE);

        if (CYHAL_POWER_LEVEL_OFF == power)
        {
            Cy_CTB_Disable(obj->base_opamp);
            Cy_CTDAC_Disable(obj->base_dac);
        }
        else
        {
            Cy_CTB_Enable(obj->base_opamp);
            Cy_CTDAC_Enable(obj->base_dac);
        }
    }
    else
    {
        _cyhal_dac_unbuffered_set_power(obj, power);
    }
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXS40PASS_CTDAC_INSTANCES) */
