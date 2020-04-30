/***************************************************************************//**
* \file cyhal_scb_common.c
*
* \brief
* Provides a struct definitions for configuration resources in the SCB (UART, I2C, SPI).
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

#include "cyhal_scb_common.h"
#include "cyhal_hwmgr.h"
#include "cyhal_syspm.h"

#ifdef CY_IP_MXSCB

#if defined(__cplusplus)
extern "C"
{
#endif

CySCB_Type* const CYHAL_SCB_BASE_ADDRESSES[CY_IP_MXSCB_INSTANCES] =
{
#ifdef SCB0
    SCB0,
#endif
#ifdef SCB1
    SCB1,
#endif
#ifdef SCB2
    SCB2,
#endif
#ifdef SCB3
    SCB3,
#endif
#ifdef SCB4
    SCB4,
#endif
#ifdef SCB5
    SCB5,
#endif
#ifdef SCB6
    SCB6,
#endif
#ifdef SCB7
    SCB7,
#endif
#ifdef SCB8
    SCB8,
#endif
#ifdef SCB9
    SCB9,
#endif
#ifdef SCB10
    SCB10,
#endif
#ifdef SCB11
    SCB11,
#endif
#ifdef SCB12
    SCB12,
#endif
#ifdef SCB13
    SCB13,
#endif
#ifdef SCB14
    SCB14,
#endif
#ifdef SCB15
    SCB15,
#endif
};

const IRQn_Type CYHAL_SCB_IRQ_N[CY_IP_MXSCB_INSTANCES] =
{
#ifdef SCB0
    scb_0_interrupt_IRQn,
#endif
#ifdef SCB1
    scb_1_interrupt_IRQn,
#endif
#ifdef SCB2
    scb_2_interrupt_IRQn,
#endif
#ifdef SCB3
    scb_3_interrupt_IRQn,
#endif
#ifdef SCB4
    scb_4_interrupt_IRQn,
#endif
#ifdef SCB5
    scb_5_interrupt_IRQn,
#endif
#ifdef SCB6
    scb_6_interrupt_IRQn,
#endif
#ifdef SCB7
    scb_7_interrupt_IRQn,
#endif
#ifdef SCB8
    scb_8_interrupt_IRQn,
#endif
#ifdef SCB9
    scb_9_interrupt_IRQn,
#endif
#ifdef SCB10
    scb_10_interrupt_IRQn,
#endif
#ifdef SCB11
    scb_11_interrupt_IRQn,
#endif
#ifdef SCB12
    scb_12_interrupt_IRQn,
#endif
#ifdef SCB13
    scb_13_interrupt_IRQn,
#endif
#ifdef SCB14
    scb_14_interrupt_IRQn,
#endif
#ifdef SCB15
    scb_15_interrupt_IRQn,
#endif
};

/** The configuration structs for the resource in use on each SCB block */
static void *cyhal_scb_config_structs[CY_IP_MXSCB_INSTANCES];
/** The callback to use for each scb instance */
static bool (*cyhal_scb_config_pm_callback[CY_IP_MXSCB_INSTANCES]) (void *obj_ptr, cyhal_syspm_callback_state_t state, cy_en_syspm_callback_mode_t pdl_mode);

uint8_t cyhal_scb_get_block_from_irqn(IRQn_Type irqn) {
    switch (irqn)
    {
#if (CY_CPU_CORTEX_M4)
#if (CY_IP_MXSCB_INSTANCES > 0)
    case scb_0_interrupt_IRQn: return 0;
#endif
#if (CY_IP_MXSCB_INSTANCES > 1)
    case scb_1_interrupt_IRQn: return 1;
#endif
#if (CY_IP_MXSCB_INSTANCES > 2)
    case scb_2_interrupt_IRQn: return 2;
#endif
#if (CY_IP_MXSCB_INSTANCES > 3)
    case scb_3_interrupt_IRQn: return 3;
#endif
#if (CY_IP_MXSCB_INSTANCES > 4)
    case scb_4_interrupt_IRQn: return 4;
#endif
#if (CY_IP_MXSCB_INSTANCES > 5)
    case scb_5_interrupt_IRQn: return 5;
#endif
#if (CY_IP_MXSCB_INSTANCES > 6)
    case scb_6_interrupt_IRQn: return 6;
#endif
#if (CY_IP_MXSCB_INSTANCES > 7)
    case scb_7_interrupt_IRQn: return 7;
#endif
#if (CY_IP_MXSCB_INSTANCES > 8)
    case scb_8_interrupt_IRQn: return 8;
#endif
#if (CY_IP_MXSCB_INSTANCES > 9)
    case scb_9_interrupt_IRQn: return 9;
#endif
#if (CY_IP_MXSCB_INSTANCES > 10)
    case scb_10_interrupt_IRQn: return 10;
#endif
#if (CY_IP_MXSCB_INSTANCES > 11)
    case scb_11_interrupt_IRQn: return 11;
#endif
#if (CY_IP_MXSCB_INSTANCES > 12)
    case scb_12_interrupt_IRQn: return 12;
#endif
#if (CY_IP_MXSCB_INSTANCES > 13)
    case scb_13_interrupt_IRQn: return 13;
#endif
#if (CY_IP_MXSCB_INSTANCES > 14)
    case scb_14_interrupt_IRQn: return 14;
#endif
#if (CY_IP_MXSCB_INSTANCES > 15)
    case scb_15_interrupt_IRQn: return 15;
#endif
#if (CY_IP_MXSCB_INSTANCES > 16)
    case scb_16_interrupt_IRQn: return 16;
#endif
#if (CY_IP_MXSCB_INSTANCES > 17)
    case scb_17_interrupt_IRQn: return 17;
#endif
#if (CY_IP_MXSCB_INSTANCES > 18)
    case scb_18_interrupt_IRQn: return 18;
#endif
#if (CY_IP_MXSCB_INSTANCES > 19)
    case scb_19_interrupt_IRQn: return 19;
#endif
#if (CY_IP_MXSCB_INSTANCES > 20)
#error "Unhandled scb count"
#endif
#endif /* (CY_CPU_CORTEX_M4) */
    default:
        CY_ASSERT(false); // Should never be called with a non-SCB IRQn
        return 0;
    }
}

void *cyhal_scb_get_irq_obj(void)
{
    IRQn_Type irqn = CYHAL_GET_CURRENT_IRQN();
    uint8_t block = cyhal_scb_get_block_from_irqn(irqn);
    return cyhal_scb_config_structs[block];
}

/* Peripheral clock values for different I2C speeds according PDL API Reference Guide */
/* Must be between 1.55 MHz and 12.8 MHz for running i2c master at 100KHz   */
#define SCB_PERI_CLOCK_SLAVE_STD      8000000
/* Must be between 7.82 MHz and 15.38 MHz for running i2c master at 400KHz  */
#define SCB_PERI_CLOCK_SLAVE_FST      12500000
/* Must be between 15.84 MHz and 89.0 MHz for running i2c master at 1MHz */
#define SCB_PERI_CLOCK_SLAVE_FSTP     50000000
/* Must be between 1.55 MHz and 3.2 MHz for running i2c slave at 100KHz     */
#define SCB_PERI_CLOCK_MASTER_STD     2000000
/* Must be between 7.82 MHz and 10 MHz for running i2c slave at 400KHz      */
#define SCB_PERI_CLOCK_MASTER_FST     8500000
/* Must be between 14.32 MHz and 25.8 MHz for running i2c slave at 1MHz  */
#define SCB_PERI_CLOCK_MASTER_FSTP    20000000

uint32_t cyhal_i2c_set_peri_divider(CySCB_Type *base, uint32_t block_num, cyhal_clock_t *clock, uint32_t freq, bool is_slave)
{
    /* Return the actual data rate on success, 0 otherwise */
    uint32_t peri_freq = 0;
    if (freq == 0)
    {
        return 0;
    }
    if (freq <= CY_SCB_I2C_STD_DATA_RATE)
    {
        peri_freq = is_slave ? SCB_PERI_CLOCK_SLAVE_STD : SCB_PERI_CLOCK_MASTER_STD;
    }
    else if (freq <= CY_SCB_I2C_FST_DATA_RATE)
    {
        peri_freq = is_slave ? SCB_PERI_CLOCK_SLAVE_FST : SCB_PERI_CLOCK_MASTER_FST;
    }
    else if (freq <= CY_SCB_I2C_FSTP_DATA_RATE)
    {
        peri_freq = is_slave ? SCB_PERI_CLOCK_SLAVE_FSTP : SCB_PERI_CLOCK_MASTER_FSTP;
    }
    else
    {
        return 0;
    }

    cy_en_sysclk_status_t status = Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)((uint8_t)PCLK_SCB0_CLOCK + block_num), clock->div_type, clock->div_num);
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphDisableDivider(clock->div_type, clock->div_num);
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphSetDivider(clock->div_type, clock->div_num, cyhal_divider_value(peri_freq, 0u));
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphEnableDivider(clock->div_type, clock->div_num);
    CY_ASSERT(CY_SYSCLK_SUCCESS == status);

    return (is_slave)
        ? Cy_SCB_I2C_GetDataRate(base, Cy_SysClk_PeriphGetFrequency(clock->div_type, clock->div_num))
        : Cy_SCB_I2C_SetDataRate(base, freq, Cy_SysClk_PeriphGetFrequency(clock->div_type, clock->div_num));
}

const cyhal_resource_pin_mapping_t* cyhal_find_scb_map(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *pin_map, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (pin == pin_map[i].pin && CY_RSLT_SUCCESS == cyhal_hwmgr_reserve(pin_map[i].inst))
        {
            cyhal_hwmgr_free(pin_map[i].inst);
            return &pin_map[i];
        }
    }
    return NULL;
}

static bool cyhal_scb_pm_transition_pending_value = false;

bool cyhal_scb_pm_transition_pending(void)
{
    return cyhal_scb_pm_transition_pending_value;
}

static bool cyhal_scb_pm_callback_index(uint8_t index, cyhal_syspm_callback_state_t state, cy_en_syspm_callback_mode_t pdl_mode)
{
    void *obj = cyhal_scb_config_structs[index];
    cyhal_scb_instance_pm_callback callback = cyhal_scb_config_pm_callback[index];
    return ((NULL != obj) && (callback != NULL)) ? callback(obj, state, pdl_mode) : true;
}

static bool cyhal_scb_pm_callback_common(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    CY_UNUSED_PARAMETER(callback_arg);
    bool allow = true;
    cy_en_syspm_callback_mode_t pdl_mode = cyhal_utils_convert_haltopdl_pm_mode(mode);
    for (uint8_t instance_idx = 0; instance_idx < CY_IP_MXSCB_INSTANCES; instance_idx++)
    {
        allow = cyhal_scb_pm_callback_index(instance_idx, state, pdl_mode);

        if (!allow && mode == CYHAL_SYSPM_CHECK_READY)
        {
            for (uint8_t revert_idx = 0; revert_idx < instance_idx; revert_idx++)
            {
                cyhal_scb_pm_callback_index(revert_idx, state, CY_SYSPM_CHECK_FAIL);
            }
            break;
        }
    }

    if (mode == CYHAL_SYSPM_CHECK_FAIL || mode == CYHAL_SYSPM_AFTER_TRANSITION)
    {
        cyhal_scb_pm_transition_pending_value = false;
    }
    else if (mode == CYHAL_SYSPM_CHECK_READY && allow)
    {
        cyhal_scb_pm_transition_pending_value = true;
    }
    return allow;
}

cyhal_syspm_callback_data_t cyhal_scb_pm_callback_data =
{
    .callback = &cyhal_scb_pm_callback_common,
    .states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE),
    .args = NULL,
    .next = NULL,
    .ignore_modes = (cyhal_syspm_callback_mode_t)0,
};

void cyhal_scb_update_instance_data(uint8_t block_num, void *obj, cyhal_scb_instance_pm_callback pm_callback)
{
    cyhal_scb_config_structs[block_num] = obj;
    cyhal_scb_config_pm_callback[block_num] = pm_callback;

    int count = 0;
    for (uint8_t i = 0; i < CY_IP_MXSCB_INSTANCES; i++)
    {
        if (cyhal_scb_config_structs[i])
        {
            if (count == 1)
            {
                return;
            }
            count++;
        }
    }
    
    if (count == 0)
    {
        CY_ASSERT(obj == NULL);
        cyhal_syspm_unregister_peripheral_callback(&cyhal_scb_pm_callback_data);
    }
    else if (count == 1 && obj != NULL)
    {
        cyhal_syspm_register_peripheral_callback(&cyhal_scb_pm_callback_data);
    }
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
