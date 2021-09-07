/*******************************************************************************
* File Name: cyhal_ezi2c.c
*
* Description:
* Provides a high level interface for interacting with the Cypress I2C. This is
* a wrapper around the lower level PDL API.
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

#include <stdlib.h>
#include <string.h>
#include "cyhal_ezi2c.h"
#include "cyhal_scb_common.h"
#include "cyhal_gpio.h"
#include "cyhal_system_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_utils.h"
#include "cyhal_clock.h"

#if defined(CY_IP_MXSCB) || defined(CY_IP_M0S8SCB)

#if defined(__cplusplus)
extern "C"
{
#endif

static inline cyhal_ezi2c_status_t _cyhal_ezi2c_convert_activity_status(uint32_t pdl_status)
{
    /* Structure to map EZI2C (PDL) status on HAL EZI2C status */
    static const uint32_t status_map[] =
    {
        0U,                                  // Default value
        (uint32_t)CYHAL_EZI2C_STATUS_READ1,  // CY_SCB_EZI2C_STATUS_READ1
        (uint32_t)CYHAL_EZI2C_STATUS_WRITE1, // CY_SCB_EZI2C_STATUS_WRITE1
        (uint32_t)CYHAL_EZI2C_STATUS_READ2,  // CY_SCB_EZI2C_STATUS_READ2
        (uint32_t)CYHAL_EZI2C_STATUS_WRITE2, // CY_SCB_EZI2C_STATUS_WRITE2
        (uint32_t)CYHAL_EZI2C_STATUS_BUSY,   // CY_SCB_EZI2C_STATUS_BUSY
        (uint32_t)CYHAL_EZI2C_STATUS_ERR,    // CY_SCB_EZI2C_STATUS_ERR
    };

    cyhal_ezi2c_status_t hal_status = (cyhal_ezi2c_status_t)_cyhal_utils_convert_flags(
        status_map, sizeof(status_map) / sizeof(uint32_t), pdl_status);
    if ((hal_status & (CYHAL_EZI2C_STATUS_BUSY | CYHAL_EZI2C_STATUS_ERR)) == 0)
    {
        hal_status |= CYHAL_EZI2C_STATUS_OK;
    }
    return hal_status;
}

/* Implement ISR for EZI2C */
static void _cyhal_ezi2c_irq_handler(void)
{
    cyhal_ezi2c_t *obj = (cyhal_ezi2c_t*) _cyhal_scb_get_irq_obj();
    Cy_SCB_EZI2C_Interrupt(obj->base, &(obj->context));

    /* Check if callback is registered */
    cyhal_ezi2c_event_callback_t callback = (cyhal_ezi2c_event_callback_t) obj->callback_data.callback;
    if (callback != NULL)
    {
        /* Check status of EZI2C and verify which events are enabled */
        cyhal_ezi2c_status_t status = cyhal_ezi2c_get_activity_status(obj);
        if(status & obj->irq_cause)
        {
            (void) (callback) (obj->callback_data.callback_arg, (cyhal_ezi2c_status_t)(status & obj->irq_cause));
        }
    }
}

static bool _cyhal_ezi2c_pm_callback_instance(void *obj_ptr, cyhal_syspm_callback_state_t state, cy_en_syspm_callback_mode_t pdl_mode)
{
    cyhal_ezi2c_t *obj = (cyhal_ezi2c_t*)(obj_ptr);

    cy_stc_syspm_callback_params_t ezi2c_callback_params = {
        .base = (void *) (obj->base),
        .context = (void *) &(obj->context)
    };

    bool allow = true;
    switch(state)
    {
        case CYHAL_SYSPM_CB_CPU_DEEPSLEEP:
            allow = (CY_SYSPM_SUCCESS == Cy_SCB_EZI2C_DeepSleepCallback(&ezi2c_callback_params, pdl_mode));
            break;
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
        case CYHAL_SYSPM_CB_SYSTEM_HIBERNATE:
            allow = (CY_SYSPM_SUCCESS == Cy_SCB_EZI2C_HibernateCallback(&ezi2c_callback_params, pdl_mode));
            break;
#endif
        default:
            break;
    }
    return allow;
}

cy_rslt_t cyhal_ezi2c_init(cyhal_ezi2c_t *obj, cyhal_gpio_t sda, cyhal_gpio_t scl, const cyhal_clock_t *clk, const cyhal_ezi2c_cfg_t *cfg)
{
    CY_ASSERT(NULL != obj);
    memset(obj, 0, sizeof(cyhal_ezi2c_t));

    /* Validate input configuration structure */
    if ((0 == cfg->slave1_cfg.slave_address) || ((cfg->two_addresses) && (0 == cfg->slave2_cfg.slave_address)))
    {
        return CYHAL_EZI2C_RSLT_ERR_CHECK_USER_CONFIG;
    }

    /* Populate configuration structure */
    const cy_stc_scb_ezi2c_config_t ezI2cConfig =
    {
        .numberOfAddresses   = cfg->two_addresses ? CY_SCB_EZI2C_TWO_ADDRESSES : CY_SCB_EZI2C_ONE_ADDRESS,
        .slaveAddress1       = cfg->slave1_cfg.slave_address,
        .slaveAddress2       = cfg->two_addresses ? cfg->slave2_cfg.slave_address : 0,
        .subAddressSize      = (cy_en_scb_ezi2c_sub_addr_size_t)cfg->sub_address_size,
        .enableWakeFromSleep = cfg->enable_wake_from_sleep,
    };

    /* Explicitly marked not allocated resources as invalid to prevent freeing them. */
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->pin_scl = CYHAL_NC_PIN_VALUE;
    obj->pin_sda = CYHAL_NC_PIN_VALUE;
    obj->is_shared_clock = true;

    cy_rslt_t result;

    /* Reserve the I2C */
    const cyhal_resource_pin_mapping_t *sda_map = _CYHAL_SCB_FIND_MAP(sda, cyhal_pin_map_scb_i2c_sda);
    const cyhal_resource_pin_mapping_t *scl_map = _CYHAL_SCB_FIND_MAP(scl, cyhal_pin_map_scb_i2c_scl);
    if ((NULL == sda_map) || (NULL == scl_map) || !_cyhal_utils_resources_equal(sda_map->inst, scl_map->inst))
    {
        return CYHAL_EZI2C_RSLT_ERR_INVALID_PIN;
    }

    result = cyhal_hwmgr_reserve(scl_map->inst);

    /* Reserve the SDA pin */
    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_utils_reserve_and_connect(sda, sda_map);
        if (result == CY_RSLT_SUCCESS)
            obj->pin_sda = sda;
    }

    /* Reserve the SCL pin */
    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_utils_reserve_and_connect(scl, scl_map);
        if (result == CY_RSLT_SUCCESS)
            obj->pin_scl = scl;
    }

    if (result == CY_RSLT_SUCCESS)
    {
        obj->is_shared_clock = (clk != NULL);
        if (clk == NULL)
        {
            result = cyhal_clock_allocate(&(obj->clock), CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT);
        }
        else
        {
            obj->clock = *clk;
            _cyhal_utils_update_clock_format(&(obj->clock));
        }
    }

    obj->resource = *(scl_map->inst);
    obj->base = _CYHAL_SCB_BASE_ADDRESSES[obj->resource.block_num];

    if (result == CY_RSLT_SUCCESS)
    {
        /* Configure I2C to operate */
        result = Cy_SCB_EZI2C_Init(obj->base, &ezI2cConfig, &(obj->context));
    }

    /* Set data rate */
    uint32_t dataRate = _cyhal_i2c_set_peri_divider(obj->base, obj->resource.block_num, &(obj->clock), (uint32_t)cfg->data_rate, true);
    if (dataRate == 0)
    {
        /* Can not reach desired data rate */
        return CYHAL_EZI2C_RSLT_ERR_CAN_NOT_REACH_DR;
    }

    if (result == CY_RSLT_SUCCESS)
    {
        /* Configure buffer for communication with master */
        Cy_SCB_EZI2C_SetBuffer1(obj->base, cfg->slave1_cfg.buf, cfg->slave1_cfg.buf_size, cfg->slave1_cfg.buf_rw_boundary, &(obj->context));
        /* Check if user set one or two addresses */
        if(cfg->two_addresses)
        {
            Cy_SCB_EZI2C_SetBuffer2(obj->base, cfg->slave2_cfg.buf, cfg->slave2_cfg.buf_size, cfg->slave2_cfg.buf_rw_boundary, &(obj->context));
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        _cyhal_scb_update_instance_data(obj->resource.block_num, (void*)obj, &_cyhal_ezi2c_pm_callback_instance);
        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = 0;

        cy_stc_sysint_t irqCfg = { _CYHAL_SCB_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_ezi2c_irq_handler);
        NVIC_EnableIRQ(_CYHAL_SCB_IRQ_N[obj->resource.block_num]);

        /* Enable EZI2C to operate */
        (void)Cy_SCB_EZI2C_Enable(obj->base);
    }

    if (result != CY_RSLT_SUCCESS)
    {
        cyhal_ezi2c_free(obj);
    }
    return result;
}

void cyhal_ezi2c_free(cyhal_ezi2c_t *obj)
{
    CY_ASSERT(NULL != obj);

    if (CYHAL_RSC_INVALID != obj->resource.type)
    {
        IRQn_Type irqn = _CYHAL_SCB_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
        _cyhal_scb_update_instance_data(obj->resource.block_num, NULL, NULL);
    }

    _cyhal_utils_release_if_used(&(obj->pin_sda));
    _cyhal_utils_release_if_used(&(obj->pin_scl));

    if (!obj->is_shared_clock)
    {
        cyhal_clock_free(&(obj->clock));
    }
}

cyhal_ezi2c_status_t cyhal_ezi2c_get_activity_status(cyhal_ezi2c_t *obj)
{
    return _cyhal_ezi2c_convert_activity_status(Cy_SCB_EZI2C_GetActivity(obj->base, &(obj->context)));
}

void cyhal_ezi2c_register_callback(cyhal_ezi2c_t *obj, cyhal_ezi2c_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_ezi2c_enable_event(cyhal_ezi2c_t *obj, cyhal_ezi2c_status_t event, uint8_t intr_priority, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }

    IRQn_Type irqn = _CYHAL_SCB_IRQ_N[obj->resource.block_num];
    NVIC_SetPriority(irqn, intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
