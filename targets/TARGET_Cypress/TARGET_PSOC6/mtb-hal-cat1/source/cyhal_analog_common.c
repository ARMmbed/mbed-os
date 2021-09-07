/***************************************************************************//**
* \file cyhal_analog_common.c
*
* \brief
* Provides common functionality that needs to be shared among all drivers that
* interact with the Programmable Analog Subsystem.
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

#include "cy_pdl.h"
#include "cyhal_hw_types.h"
#include "cyhal_hwmgr.h"
#include "cyhal_pin_package.h"
#include "cyhal_gpio.h"
#include "cyhal_system_impl.h"

#if defined(CY_IP_MXS40PASS_INSTANCES)
#include "cy_ctb.h"
#endif

#if defined(CY_IP_MXS40PASS_INSTANCES) || defined(CY_IP_M0S8PASS4A_INSTANCES)

#if defined(__cplusplus)
extern "C"
{
#endif

#if defined(CY_IP_MXS40PASS_INSTANCES)
static uint16_t cyhal_analog_ref_count = 0;
#endif

#ifdef CY_IP_MXS40PASS_CTB_INSTANCES
static uint16_t cyhal_analog_ctb_ref_count = 0;

CTBM_Type *const _cyhal_ctb_base[] =
{
#if (CY_IP_MXS40PASS_CTB_INSTANCES == 1)
    CTBM0,
#endif

/* All current CAT1/CAT2 devices have only one CTB block */
#if (CY_IP_MXS40PASS_CTB_INSTANCES > 1)
    #error "Unhandled CTB instance count"
#endif
};


#endif

void _cyhal_analog_init(void)
{
#if defined(CY_IP_MXS40PASS_INSTANCES)
    uint32_t saved_intr = cyhal_system_critical_section_enter();
    if(cyhal_analog_ref_count == 0)
    {
        Cy_SysAnalog_Init(&Cy_SysAnalog_Fast_Local);
        Cy_SysAnalog_Enable();
    }

    ++cyhal_analog_ref_count;
    cyhal_system_critical_section_exit(saved_intr);
#endif
}

void _cyhal_analog_free(void)
{
#if defined(CY_IP_MXS40PASS_INSTANCES)
    uint32_t saved_intr = cyhal_system_critical_section_enter();
    CY_ASSERT(cyhal_analog_ref_count > 0);
    --cyhal_analog_ref_count;
    if(cyhal_analog_ref_count == 0)
    {
        Cy_SysAnalog_Disable();
        Cy_SysAnalog_DeInit();
    }
    cyhal_system_critical_section_exit(saved_intr);
#endif
}

#ifdef CY_IP_MXS40PASS_CTB_INSTANCES
void cyhal_analog_ctb_init(CTBM_Type *base)
{
    uint32_t saved_intr = cyhal_system_critical_section_enter();
    _cyhal_analog_init();
    if(cyhal_analog_ctb_ref_count == 0)
    {
        Cy_CTB_Enable(base);
    }
    ++cyhal_analog_ctb_ref_count;
    cyhal_system_critical_section_exit(saved_intr);
}

void cyhal_analog_ctb_free(CTBM_Type *base)
{
    uint32_t saved_intr = cyhal_system_critical_section_enter();
    CY_ASSERT(cyhal_analog_ctb_ref_count > 0);
    --cyhal_analog_ctb_ref_count;
    if(cyhal_analog_ctb_ref_count == 0)
    {
        Cy_CTB_Disable(base);
        Cy_CTB_DeInit(base, true);
    }
    _cyhal_analog_free();
    cyhal_system_critical_section_exit(saved_intr);
}

uint32_t _cyhal_opamp_pin_to_mask(uint8_t opamp_num, cyhal_gpio_t vin_p, cyhal_gpio_t vin_m, cyhal_gpio_t vout)
{
    /* Follower mode require close switch from vin_p to OAP+, from OAP- to OPAMP_OUT, from OPAMP_OUT to vout */
    /* Non follower mode require close switch from vin_p to OAP+, from vin_m to OAP- */

    const cyhal_resource_pin_mapping_t *vin_p0_map = CY_UTILS_GET_RESOURCE(vin_p, cyhal_pin_map_opamp_vin_p0);
    // If we have a valid vplus pin, it must be in either vin_p0 or vin_p1
#ifdef CYHAL_PIN_MAP_OPAMP_VIN_P1
    CY_ASSERT(NULL != vin_p0_map || NULL != CY_UTILS_GET_RESOURCE(vin_p, cyhal_pin_map_opamp_vin_p1));
#else
    CY_ASSERT(NULL != vin_p0_map);
#endif
    uint32_t switch_mask = (0u == opamp_num)
                            ? ((NULL != vin_p0_map) ? CY_CTB_SW_OA0_POS_PIN0_MASK : CY_CTB_SW_OA0_POS_PIN6_MASK)
                            : ((NULL != vin_p0_map) ? CY_CTB_SW_OA1_POS_PIN5_MASK : CY_CTB_SW_OA1_POS_PIN7_MASK);

    if (NC == vin_m)
    {
        if(NC != vout)
        {
            // Set switches for internal feedback
            switch_mask |= (0u == opamp_num) ? CY_CTB_SW_OA0_OUT_SHORT_1X_10X_MASK : CY_CTB_SW_OA1_OUT_SHORT_1X_10X_MASK ;
            switch_mask |= (0u == opamp_num) ? CY_CTB_SW_OA0_NEG_OUT_MASK          : CY_CTB_SW_OA1_NEG_OUT_MASK;
        }
    }
    else
    {
        switch_mask |= (0u == opamp_num) ? CY_CTB_SW_OA0_NEG_PIN1_MASK :  CY_CTB_SW_OA1_NEG_PIN4_MASK;
    }

    return switch_mask;
}

cy_rslt_t _cyhal_opamp_init_common(cyhal_resource_inst_t* rsc, cy_rslt_t bad_arg_error, cyhal_gpio_t vin_p, cyhal_gpio_t vin_m, cyhal_gpio_t vout, cyhal_gpio_t comp_out)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    rsc->type = CYHAL_RSC_INVALID;
    cyhal_gpio_t reserved_vin_p = NC;
    cyhal_gpio_t reserved_vin_m = NC;
    cyhal_gpio_t reserved_vout = NC;
    cyhal_gpio_t reserved_comp_out = NC;

    const cyhal_resource_pin_mapping_t *vin_p_map = CY_UTILS_GET_RESOURCE(vin_p, cyhal_pin_map_opamp_vin_p0);
#ifdef CYHAL_PIN_MAP_OPAMP_VIN_P1
    if(NULL == vin_p_map)
    {
        vin_p_map = CY_UTILS_GET_RESOURCE(vin_p, cyhal_pin_map_opamp_vin_p1);
    }
#endif

    // In at least one use case, everything except for vin_p is optional. Leave it to the calling driver to
    // validate which combinations are allowed in its particular use case.
    const cyhal_resource_pin_mapping_t *vin_m_map = (NC != vin_m)    ? CY_UTILS_GET_RESOURCE(vin_m, cyhal_pin_map_opamp_vin_m)      : NULL;
    const cyhal_resource_pin_mapping_t *vout_map  = (NC != vout)     ? CY_UTILS_GET_RESOURCE(vout, cyhal_pin_map_opamp_out_10x)     : NULL;
    const cyhal_resource_pin_mapping_t *comp_map  = (NC != comp_out) ? CY_UTILS_GET_RESOURCE(comp_out, cyhal_pin_map_opamp_dsi_ctb_cmp) : NULL;


    if(NULL == vin_p_map || (NC != vin_m && NULL == vin_m_map) || (NC != vout && NULL == vout_map) || (NC != comp_out && NULL == comp_map))
    {
        result = bad_arg_error;
    }

    if(CY_RSLT_SUCCESS == result &&
        ((NULL != vin_m_map && false == _cyhal_utils_resources_equal(vin_p_map->inst, vin_m_map->inst))
        || (NULL != vout_map && false == _cyhal_utils_resources_equal(vin_p_map->inst, vout_map->inst))
        || (NULL != comp_map && false == _cyhal_utils_resources_equal(vin_p_map->inst, comp_map->inst))))
    {
        result = bad_arg_error;
    }

    if(CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_reserve(vin_p_map->inst);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        *rsc = *(vin_p_map->inst);

        /* If we fail to reserve any of the pins, clear it out so that free doesn't try to release it */
        result = _cyhal_utils_reserve_and_connect(vin_p, vin_p_map);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        reserved_vin_p = vin_p;
        if(NC != vin_m)
        {
            result = _cyhal_utils_reserve_and_connect(vin_m, vin_m_map);
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        reserved_vin_m = vin_m;
        if(NC != vout)
        {
            result = _cyhal_utils_reserve_and_connect(vout, vout_map);
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        reserved_vout = vout;
        if(NC != comp_out)
        {
            result = _cyhal_utils_reserve_and_connect(comp_out, comp_map);
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        reserved_comp_out = comp_out;
    }

    if(CY_RSLT_SUCCESS != result)
    {
        _cyhal_utils_release_if_used(&reserved_vin_p);
        _cyhal_utils_release_if_used(&reserved_vin_m);
        _cyhal_utils_release_if_used(&reserved_vout);
        _cyhal_utils_release_if_used(&reserved_comp_out);

        if(CYHAL_RSC_INVALID != rsc->type)
        {
            cyhal_hwmgr_free(rsc);
        }
    }

    return result;
}

uint32 _cyhal_opamp_convert_power(cyhal_power_level_t hal_power)
{
    switch(hal_power)
    {
        case CYHAL_POWER_LEVEL_OFF:
            return (uint32_t)CY_CTB_POWER_OFF;
        case CYHAL_POWER_LEVEL_LOW:
            return (uint32_t)CY_CTB_POWER_LOW;
        case CYHAL_POWER_LEVEL_MEDIUM:
            return (uint32_t)CY_CTB_POWER_MEDIUM;
        case CYHAL_POWER_LEVEL_HIGH:
            return (uint32_t)CY_CTB_POWER_HIGH;
        case CYHAL_POWER_LEVEL_DEFAULT:
            return (uint32_t)CY_CTB_POWER_MEDIUM;
        default:
            CY_ASSERT(false);
            return CYHAL_POWER_LEVEL_OFF;
    }
}
#endif

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXS40PASS_INSTANCES || defined(CY_IP_M0S8PASS4A_INSTANCES) */
