/*******************************************************************************
* \file cyhal_comp_lp.c
*
* \brief
* Provides an implementation of the comp HAL on top of the LP (low power) comp.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
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
#include "cyhal_comp_lp.h"
#include "cyhal_analog_common.h"
#include "cyhal_hwmgr.h"
#include "cyhal_system.h"

#define _CYHAL_COMP_PER_LP (2u)

#if defined(CY_IP_MXLPCOMP_INSTANCES) && (CY_IP_MXLPCOMP_INSTANCES > 0)

static cyhal_comp_t* _cyhal_comp_lp_config_structs[CY_IP_MXLPCOMP_INSTANCES * _CYHAL_COMP_PER_LP];

LPCOMP_Type *const _cyhal_lpcomp_base[] =
{
#if (CY_IP_MXLPCOMP_INSTANCES == 1)
    LPCOMP,
#endif
#if (CY_IP_MXLPCOMP_INSTANCES > 1)
    #error Unhandled LP_COMP instance count
#endif
};

static const IRQn_Type _cyhal_lp_comp_irq_n[] =
{
#if (CY_IP_MXLPCOMP_INSTANCES == 1)
    lpcomp_interrupt_IRQn,
#endif
#if (CY_IP_MXLPCOMP_INSTANCES > 1)
    #error Unhandled LP_COMP instance count
#endif
};

static uint8_t _cyhal_lpcomp_get_block_from_irqn(IRQn_Type irqn) {
    switch (irqn)
    {
#if (CY_IP_MXLPCOMP_INSTANCES == 1)
    case lpcomp_interrupt_IRQn:
        return 0;
#endif
#if (CY_IP_MXLPCOMP_INSTANCES > 1)
    #error Unhandled LP_COMP instance count
#endif
    default:
        CY_ASSERT(false); // Should never be called
        return 0;
    }
}

static uint32_t _cyhal_comp_lp_convert_power(cyhal_power_level_t hal_power)
{
    switch(hal_power)
    {
        case CYHAL_POWER_LEVEL_OFF:
            return (uint32_t)CY_LPCOMP_MODE_OFF;
        case CYHAL_POWER_LEVEL_LOW:
            return (uint32_t)CY_LPCOMP_MODE_ULP;
        case CYHAL_POWER_LEVEL_MEDIUM:
            return (uint32_t)CY_LPCOMP_MODE_LP;
        case CYHAL_POWER_LEVEL_HIGH:
            return (uint32_t)CY_LPCOMP_MODE_NORMAL;
        case CYHAL_POWER_LEVEL_DEFAULT:
            return (uint32_t)CY_LPCOMP_MODE_LP;
        default:
            CY_ASSERT(false);
            return CY_LPCOMP_MODE_OFF;
    }
}

static cyhal_comp_event_t _cyhal_comp_lp_get_enabled_events(cyhal_comp_t * obj)
{
    switch((cy_en_lpcomp_int_t)obj->irq_cause)
    {
        case CY_LPCOMP_INTR_DISABLE:
            return (cyhal_comp_event_t)0u;
        case CY_LPCOMP_INTR_RISING:
            return CYHAL_COMP_RISING_EDGE;
        case CY_LPCOMP_INTR_FALLING:
            return CYHAL_COMP_FALLING_EDGE;
        case CY_LPCOMP_INTR_BOTH:
            return (cyhal_comp_event_t)(CYHAL_COMP_RISING_EDGE | CYHAL_COMP_FALLING_EDGE);
        default:
            CY_ASSERT(false);
            return (cyhal_comp_event_t)0u;
    }
}

static cy_en_lpcomp_int_t _cyhal_comp_lp_convert_hal_event(cyhal_comp_event_t event)
{
    switch((uint8_t)event)
    {
        case 0u:
            return CY_LPCOMP_INTR_DISABLE;
        case (uint8_t)CYHAL_COMP_RISING_EDGE:
            return CY_LPCOMP_INTR_RISING;
        case (uint8_t)CYHAL_COMP_FALLING_EDGE:
            return CY_LPCOMP_INTR_FALLING;
        case (uint8_t)(CYHAL_COMP_RISING_EDGE | CYHAL_COMP_FALLING_EDGE):
            return CY_LPCOMP_INTR_BOTH;
        default:
            CY_ASSERT(false);
            return CY_LPCOMP_INTR_DISABLE;
    }
}

static void _cyhal_comp_lp_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block = _cyhal_lpcomp_get_block_from_irqn(irqn);
    uint32_t instance_num = (Cy_LPComp_GetInterruptStatusMasked(_cyhal_comp_lp_config_structs[block * _CYHAL_COMP_PER_LP]->base_lpcomp) >> 1) ? 1 : 0;

    cyhal_comp_t* obj = _cyhal_comp_lp_config_structs[block * _CYHAL_COMP_PER_LP + instance_num];

    Cy_LPComp_ClearInterrupt(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? 2 : 1);

    cyhal_comp_event_callback_t callback = (cyhal_comp_event_callback_t)obj->callback_data.callback;
    if(NULL != callback)
    {
        cyhal_comp_event_t event = _cyhal_comp_lp_get_enabled_events(obj);
        callback(obj->callback_data.callback_arg, event);
    }
}

cy_rslt_t _cyhal_comp_lp_init(cyhal_comp_t *obj, cyhal_gpio_t vin_p, cyhal_gpio_t vin_m, cyhal_gpio_t output, cyhal_comp_config_t *cfg)
{
    CY_ASSERT(NULL != obj);

    /* Initial values */
    cy_rslt_t result = CY_RSLT_SUCCESS;
    memset(obj, 0, sizeof(cyhal_comp_t));
    obj->irq_cause = 0;

    /* Mark pins in obj NC until they are successfully reserved */
    obj->pin_vin_p = NC;
    obj->pin_vin_m = NC;
    obj->pin_out = NC;

    obj->base_lpcomp = NULL;
    obj->resource.type = CYHAL_RSC_INVALID;

    /* Validate pins. vin_p and vin_m are mandatory pins, output is optional. */
    if ((NC == vin_p) || (NC == vin_m))
    {
        result = CYHAL_COMP_RSLT_ERR_INVALID_PIN;
    }

    /* Get mapping for pins */
    const cyhal_resource_pin_mapping_t *vin_p_map  = (NC != vin_p)     ? CY_UTILS_GET_RESOURCE(vin_p, cyhal_pin_map_lpcomp_inp_comp)     : NULL;
    const cyhal_resource_pin_mapping_t *vin_m_map  = (NC != vin_m)     ? CY_UTILS_GET_RESOURCE(vin_m, cyhal_pin_map_lpcomp_inn_comp)     : NULL;
    const cyhal_resource_pin_mapping_t *output_map = (NC != output)    ? CY_UTILS_GET_RESOURCE(output, cyhal_pin_map_lpcomp_dsi_comp)    : NULL;

    /* Verify if mapping successful */
    if((NULL == vin_p_map) || (NULL == vin_m_map) || ((NC != output) && (NULL == output_map)))
    {
        result = CYHAL_COMP_RSLT_ERR_INVALID_PIN;
    }

    if(CY_RSLT_SUCCESS == result &&
        ((NULL != vin_m_map  && false == _cyhal_utils_resources_equal(vin_p_map->inst, vin_m_map->inst))
      || (NULL != output_map && false == _cyhal_utils_resources_equal(vin_p_map->inst, output_map->inst))))
    {
        result = CYHAL_COMP_RSLT_ERR_INVALID_PIN;
    }

    /* Reserve resources */
    if(CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_reserve(vin_p_map->inst);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->resource = *(vin_p_map->inst);
        result = _cyhal_utils_reserve_and_connect(vin_p, vin_p_map);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->pin_vin_p = vin_p;
        result = _cyhal_utils_reserve_and_connect(vin_m, vin_m_map);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->pin_vin_m = vin_m;
        if(NC != output)
        {
            result = _cyhal_utils_reserve_and_connect(output, output_map);
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->pin_out = output;
    }

    if (result == CY_RSLT_SUCCESS)
    {
        obj->base_lpcomp = _cyhal_lpcomp_base[obj->resource.block_num / _CYHAL_COMP_PER_LP];

        /* Configure LPComp output mode and hysteresis for channel 0 or 1 */
        cy_stc_lpcomp_config_t comp_lp_config;

        comp_lp_config.outputMode = CY_LPCOMP_OUT_DIRECT;
        comp_lp_config.intType = CY_LPCOMP_INTR_DISABLE;
        comp_lp_config.hysteresis = cfg->hysteresis ? CY_LPCOMP_HYST_ENABLE : CY_LPCOMP_HYST_DISABLE;
        comp_lp_config.power = (cy_en_lpcomp_pwr_t)_cyhal_comp_lp_convert_power(cfg->power);

        result = Cy_LPComp_Init(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0, &comp_lp_config);

        /* Set both terminals to GPIO inputs */
        Cy_LPComp_SetInputs(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0, CY_LPCOMP_SW_GPIO, CY_LPCOMP_SW_GPIO);

        /* Set channel 0 or 1 power mode - Ultra Low Power mode */
        Cy_LPComp_SetPower(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0, comp_lp_config.power);

        Cy_LPComp_SetInterruptMask(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? 2 : 1);

        /* Initialize the programmable analog */
        Cy_LPComp_Enable(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0);

        _cyhal_comp_lp_config_structs[obj->resource.block_num] = obj;
        cy_stc_sysint_t irqCfg = { _cyhal_lp_comp_irq_n[obj->resource.block_num / _CYHAL_COMP_PER_LP], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_comp_lp_irq_handler);
        NVIC_EnableIRQ(_cyhal_lp_comp_irq_n[obj->resource.block_num / _CYHAL_COMP_PER_LP]);
    }

    /* Free LP_COMP in case of failure */
    if (result != CY_RSLT_SUCCESS)
    {
        _cyhal_comp_lp_free(obj);
    }
    return result;
}

void _cyhal_comp_lp_free(cyhal_comp_t *obj)
{
    CY_ASSERT(NULL != obj);

    if(CYHAL_RSC_INVALID != obj->resource.type)
    {
        _cyhal_comp_lp_config_structs[obj->resource.block_num] = NULL;

        /* Free the programmable analog */
        uint8_t lpcomp_num = obj->resource.block_num / _CYHAL_COMP_PER_LP;
        if((NULL == _cyhal_comp_lp_config_structs[(lpcomp_num * _CYHAL_COMP_PER_LP)])
                && (NULL == _cyhal_comp_lp_config_structs[(lpcomp_num * _CYHAL_COMP_PER_LP) + 1]))
        {
            NVIC_DisableIRQ(_cyhal_lp_comp_irq_n[obj->resource.block_num / _CYHAL_COMP_PER_LP]);
        }

        if(NULL != obj->base_lpcomp)
        {
            Cy_LPComp_Disable(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0);
        }
        cyhal_hwmgr_free(&(obj->resource));
        obj->base_lpcomp = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    _cyhal_utils_release_if_used(&(obj->pin_vin_p));
    _cyhal_utils_release_if_used(&(obj->pin_vin_m));
    _cyhal_utils_release_if_used(&(obj->pin_out));
}

cy_rslt_t _cyhal_comp_lp_configure(cyhal_comp_t *obj, cyhal_comp_config_t *cfg)
{
    CY_ASSERT(NULL != obj);
    cy_rslt_t result = CY_RSLT_SUCCESS;

    result = _cyhal_comp_lp_set_power(obj, cfg->power);
    if (result == CY_RSLT_SUCCESS)
    {
        Cy_LPComp_SetHysteresis(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0, cfg->hysteresis ? CY_LPCOMP_HYST_ENABLE : CY_LPCOMP_HYST_DISABLE);
    }
    return result;
}

cy_rslt_t _cyhal_comp_lp_set_power(cyhal_comp_t *obj, cyhal_power_level_t power)
{
    CY_ASSERT(NULL != obj);
    cy_en_lpcomp_pwr_t power_level = (cy_en_lpcomp_pwr_t)_cyhal_comp_lp_convert_power(power);

    Cy_LPComp_SetPower(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0, power_level);
    return CY_RSLT_SUCCESS;
}

bool _cyhal_comp_lp_read(cyhal_comp_t *obj)
{
    CY_ASSERT(NULL != obj);
    return (1UL == Cy_LPComp_GetCompare(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0));
}

void _cyhal_comp_lp_enable_event(cyhal_comp_t *obj, cyhal_comp_event_t event, uint8_t intr_priority, bool enable)
{
    if(enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= (~event);
    }

    IRQn_Type irqn = _cyhal_lp_comp_irq_n[obj->resource.block_num / _CYHAL_COMP_PER_LP];
    NVIC_SetPriority(irqn, intr_priority);

    cy_en_lpcomp_int_t pdl_event = _cyhal_comp_lp_convert_hal_event((cyhal_comp_event_t)obj->irq_cause);
    Cy_LPComp_SetInterruptTriggerMode(obj->base_lpcomp, (obj->resource.block_num % _CYHAL_COMP_PER_LP) ? CY_LPCOMP_CHANNEL_1 : CY_LPCOMP_CHANNEL_0, pdl_event);
}

#endif /* defined(CY_IP_MXLPCOMP_INSTANCES) && (CY_IP_MXLPCOMP_INSTANCES > 0) */
