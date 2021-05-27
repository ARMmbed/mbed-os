/***************************************************************************/ /**
* \file cyhal_comp_ctb.c
*
* \brief
* Provides an implementation of the comp HAL on top of the CTB opamps.
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
#include "cyhal_comp_ctb.h"
#include "cyhal_gpio.h"
#include "cyhal_analog_common.h"
#include "cyhal_hwmgr.h"
#include "cyhal_system.h"

#if defined(CY_IP_MXS40PASS_CTB_INSTANCES) && (CY_IP_MXS40PASS_CTB_INSTANCES > 0)

#define _CYHAL_COMP_CTB_DEFAULT_LEVEL CY_CTB_COMP_DSI_TRIGGER_OUT_LEVEL
#define _CYHAL_COMP_CTB_DEFAULT_BYPASS CY_CTB_COMP_BYPASS_NO_SYNC

static const cy_stc_ctb_opamp_config_t _cyhal_comp_ctb_default_config =
{
    /* oaPower is specified in init */
    .deepSleep    = CY_CTB_DEEPSLEEP_ENABLE,
    .oaMode       = CY_CTB_MODE_COMP,
    .oaPump       = CY_CTB_PUMP_ENABLE,
    .oaCompEdge   = CY_CTB_COMP_EDGE_DISABLE,
    .oaCompLevel  = _CYHAL_COMP_CTB_DEFAULT_LEVEL,
    .oaCompBypass = _CYHAL_COMP_CTB_DEFAULT_BYPASS,
    /* oaCompHyst is specified in init */
    .oaCompIntrEn = true,
};

static cyhal_comp_t* _cyhal_comp_ctb_config_structs[CY_IP_MXS40PASS_CTB_INSTANCES * _CYHAL_OPAMP_PER_CTB];

static const IRQn_Type _cyhal_ctb_irq_n[] =
{
#if (CY_IP_MXS40PASS_CTB_INSTANCES == 1)
    pass_interrupt_ctbs_IRQn,
#endif
#if (CY_IP_MXS40PASS_CTB_INSTANCES > 1)
    #error Unhandled CTB instance count
#endif
};

/** Get the comp config struct for the opamp that caused the current interrupt */
static cyhal_comp_t* _cyhal_ctb_get_interrupt_source()
{
    uint32_t cause_register_value = PASS->INTR_CAUSE;
    for(uint8_t ctb_num = 0; ctb_num < CY_IP_MXS40PASS_CTB_INSTANCES; ++ctb_num)
    {
        if(cause_register_value & (1u << ctb_num))
        {
            for(uint8_t oa_num = 0; oa_num < _CYHAL_OPAMP_PER_CTB; ++oa_num)
            {
                cyhal_comp_t* inst = _cyhal_comp_ctb_config_structs[(ctb_num * _CYHAL_OPAMP_PER_CTB) + oa_num];
                if(inst != NULL && Cy_CTB_GetInterruptStatusMasked(inst->base_ctb, _cyhal_opamp_convert_sel(oa_num)))
                {
                    return inst;
                }
            }
        }
    }

    return NULL;
}

static cyhal_comp_event_t _cyhal_comp_ctb_get_enabled_events(cyhal_comp_t * obj)
{
    uint32_t edge_config_val = (0u == obj->resource.block_num)
        ? (CTBM_OA_RES0_CTRL(obj->base_ctb) & CTBM_OA_RES0_CTRL_OA0_COMPINT_Msk)
        : (CTBM_OA_RES1_CTRL(obj->base_ctb) & CTBM_OA_RES1_CTRL_OA1_COMPINT_Msk);

    switch(edge_config_val)
    {
        case CY_CTB_COMP_EDGE_DISABLE:
            return (cyhal_comp_event_t)0u;
        case CY_CTB_COMP_EDGE_RISING:
            return CYHAL_COMP_RISING_EDGE;
        case CY_CTB_COMP_EDGE_FALLING:
            return CYHAL_COMP_FALLING_EDGE;
        case CY_CTB_COMP_EDGE_BOTH:
            return (cyhal_comp_event_t)(CYHAL_COMP_RISING_EDGE | CYHAL_COMP_FALLING_EDGE);
        default:
            CY_ASSERT(false);
            return (cyhal_comp_event_t)0u;
    }
}

static cy_en_ctb_comp_edge_t _cyhal_comp_ctb_convert_hal_event(cyhal_comp_event_t event)
{
    switch((uint8_t)event)
    {
        case 0u:
            return CY_CTB_COMP_EDGE_DISABLE;
        case (uint8_t)CYHAL_COMP_RISING_EDGE:
            return CY_CTB_COMP_EDGE_RISING;
        case (uint8_t)CYHAL_COMP_FALLING_EDGE:
            return CY_CTB_COMP_EDGE_FALLING;
        case (uint8_t)(CYHAL_COMP_RISING_EDGE | CYHAL_COMP_FALLING_EDGE):
            return CY_CTB_COMP_EDGE_BOTH;
        default:
            CY_ASSERT(false);
            return CY_CTB_COMP_EDGE_DISABLE;
    }
}

static void _cyhal_comp_ctb_irq_handler(void)
{
    cyhal_comp_t* obj = _cyhal_ctb_get_interrupt_source();
    Cy_CTB_ClearInterrupt(obj->base_ctb, _cyhal_opamp_convert_sel(obj->resource.block_num));
    cyhal_comp_event_callback_t callback = (cyhal_comp_event_callback_t)obj->callback_data.callback;
    if(NULL != callback)
    {
        // The CTB hardware doesn't directly capture the event, so just pass on the converted mask
        cyhal_comp_event_t event = _cyhal_comp_ctb_get_enabled_events(obj);
        callback(obj->callback_data.callback_arg, event);
    }
}

cy_rslt_t _cyhal_comp_ctb_init(cyhal_comp_t *obj, cyhal_gpio_t vin_p, cyhal_gpio_t vin_m, cyhal_gpio_t output, cyhal_comp_config_t *cfg)
{
    CY_ASSERT(NULL != obj);

    /* Initial values */
    cy_rslt_t result = CY_RSLT_SUCCESS;
    memset(obj, 0, sizeof(cyhal_comp_t));
    obj->resource.type = CYHAL_RSC_INVALID;

    /* Validate pins. vin_p and vin_m are mandatory pins, vout is optional. */
    if ((NC == vin_p) || (NC == vin_m))
    {
        result = CYHAL_COMP_RSLT_ERR_INVALID_PIN;
    }

    /* Allocate resources */
    if(CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_opamp_init_common(&(obj->resource), CYHAL_COMP_RSLT_ERR_INVALID_PIN, vin_p, vin_m, NC /* vout unused by comparator */, output);
    }

    /* Configure the opamp */
    if (result == CY_RSLT_SUCCESS)
    {
        obj->pin_vin_p = vin_p;
        obj->pin_vin_m = vin_m;
        obj->pin_out = output;
        obj->base_ctb = _cyhal_ctb_base[obj->resource.block_num / _CYHAL_OPAMP_PER_CTB];
        cy_stc_ctb_opamp_config_t config = _cyhal_comp_ctb_default_config;
        config.oaPower = (cy_en_ctb_power_t)_cyhal_opamp_convert_power(cfg->power);
        config.oaCompHyst = cfg->hysteresis ? CY_CTB_COMP_HYST_10MV : CY_CTB_COMP_HYST_DISABLE;
        result = Cy_CTB_OpampInit(obj->base_ctb, _cyhal_opamp_convert_sel(obj->resource.block_num), &config);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        /* Initialize the programmable analog */
        cyhal_analog_ctb_init(obj->base_ctb);

        /* OPAMP Routing. Close input switches for OA0 or OA1. */
        Cy_CTB_SetAnalogSwitch(obj->base_ctb, _cyhal_opamp_convert_switch(obj->resource.block_num), _cyhal_opamp_pin_to_mask(obj->resource.block_num % _CYHAL_OPAMP_PER_CTB, vin_p, vin_m, NC), CY_CTB_SWITCH_CLOSE);
        _cyhal_opamp_set_isolation_switch(obj->resource.block_num % _CYHAL_OPAMP_PER_CTB, obj->base_ctb, CY_CTB_SWITCH_CLOSE);
        _cyhal_comp_ctb_config_structs[obj->resource.block_num] = obj;

        cy_stc_sysint_t irqCfg = { _cyhal_ctb_irq_n[obj->resource.block_num / _CYHAL_OPAMP_PER_CTB], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_comp_ctb_irq_handler);
        NVIC_EnableIRQ(_cyhal_ctb_irq_n[obj->resource.block_num / _CYHAL_OPAMP_PER_CTB]);
    }
    else
    {
        obj->base_ctb = NULL;
    }

    /* Free OPAMP in case of failure */
    if (result != CY_RSLT_SUCCESS)
    {
        _cyhal_comp_ctb_free(obj);
    }
    return result;
}

void _cyhal_comp_ctb_free(cyhal_comp_t *obj)
{
    CY_ASSERT(NULL != obj);

    if(CYHAL_RSC_INVALID != obj->resource.type)
    {
        Cy_CTB_SetAnalogSwitch(obj->base_ctb, _cyhal_opamp_convert_switch(obj->resource.block_num), _cyhal_opamp_pin_to_mask(obj->resource.block_num % _CYHAL_OPAMP_PER_CTB, obj->pin_vin_p, obj->pin_vin_m, NC), CY_CTB_SWITCH_OPEN);
        _cyhal_opamp_set_isolation_switch(obj->resource.block_num % _CYHAL_OPAMP_PER_CTB, obj->base_ctb, CY_CTB_SWITCH_OPEN);
        cyhal_analog_ctb_free(obj->base_ctb);

        _cyhal_comp_ctb_config_structs[obj->resource.block_num] = NULL;

        uint8_t ctb_num = obj->resource.block_num / _CYHAL_OPAMP_PER_CTB;
        /* If neither opamp in this ctb is in use, disable the ISR */
        if(NULL == _cyhal_comp_ctb_config_structs[ctb_num * _CYHAL_OPAMP_PER_CTB]
            && NULL == _cyhal_comp_ctb_config_structs[(ctb_num * _CYHAL_OPAMP_PER_CTB) + 1])
        {
            NVIC_DisableIRQ(_cyhal_ctb_irq_n[obj->resource.block_num / _CYHAL_OPAMP_PER_CTB]);
        }

        cyhal_hwmgr_free(&(obj->resource));
        obj->base_ctb = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    _cyhal_utils_release_if_used(&(obj->pin_vin_p));
    _cyhal_utils_release_if_used(&(obj->pin_vin_m));
    _cyhal_utils_release_if_used(&(obj->pin_out));
}

cy_rslt_t _cyhal_comp_ctb_set_power(cyhal_comp_t *obj, cyhal_power_level_t power)
{
    CY_ASSERT(NULL != obj);

    cy_en_ctb_power_t power_level = (cy_en_ctb_power_t)_cyhal_opamp_convert_power(power);
    Cy_CTB_SetPower(obj->base_ctb, _cyhal_opamp_convert_sel(obj->resource.block_num), power_level, CY_CTB_PUMP_ENABLE);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t _cyhal_comp_ctb_configure(cyhal_comp_t *obj, cyhal_comp_config_t *cfg)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t result = _cyhal_comp_ctb_set_power(obj, cfg->power);
    if(CY_RSLT_SUCCESS == result)
    {
        Cy_CTB_CompSetConfig(obj->base_ctb, _cyhal_opamp_convert_sel(obj->resource.block_num),
            _CYHAL_COMP_CTB_DEFAULT_LEVEL, _CYHAL_COMP_CTB_DEFAULT_BYPASS,
            cfg->hysteresis ? CY_CTB_COMP_HYST_10MV : CY_CTB_COMP_HYST_DISABLE);
    }
    return result;
}

bool _cyhal_comp_ctb_read(cyhal_comp_t *obj)
{
    CY_ASSERT(NULL != obj);

    return (1UL == Cy_CTB_CompGetStatus(obj->base_ctb, _cyhal_opamp_convert_sel(obj->resource.block_num)));
}

void _cyhal_comp_ctb_enable_event(cyhal_comp_t *obj, cyhal_comp_event_t event, uint8_t intr_priority, bool enable)
{
    CY_UNUSED_PARAMETER(intr_priority);
    cyhal_comp_event_t enabled_events = _cyhal_comp_ctb_get_enabled_events(obj);
    if(enable)
    {
        enabled_events |= event;
    }
    else
    {
        enabled_events &= (~event);
    }

    IRQn_Type irqn = _cyhal_ctb_irq_n[obj->resource.block_num / _CYHAL_OPAMP_PER_CTB];
    NVIC_SetPriority(irqn, intr_priority);

    cy_en_ctb_comp_edge_t pdl_event = _cyhal_comp_ctb_convert_hal_event(enabled_events);
    Cy_CTB_CompSetInterruptEdgeType(obj->base_ctb, _cyhal_opamp_convert_sel(obj->resource.block_num), pdl_event);
}

#endif /* defined(CY_IP_MXS40PASS_CTB_INSTANCES) && (CY_IP_MXS40PASS_CTB_INSTANCES > 0) */
