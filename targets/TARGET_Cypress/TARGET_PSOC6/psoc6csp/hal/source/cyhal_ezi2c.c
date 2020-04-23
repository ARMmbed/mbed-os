/*******************************************************************************
* File Name: cyhal_ezi2c.c
*
* Description:
* Provides a high level interface for interacting with the Cypress I2C. This is
* a wrapper around the lower level PDL API.
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

#include <stdlib.h>
#include "cyhal_ezi2c.h"
#include "cyhal_scb_common.h"
#include "cyhal_gpio.h"
#include "cyhal_interconnect.h"
#include "cyhal_system_impl.h"
#include "cyhal_hwmgr.h"

#ifdef CY_IP_MXSCB

#if defined(__cplusplus)
extern "C"
{
#endif

/* Peripheral clock values for different EZI2C speeds according PDL API Reference Guide */
#define SCB_PERI_CLOCK_SLAVE_STD      8000000
#define SCB_PERI_CLOCK_SLAVE_FST      12500000
#define SCB_PERI_CLOCK_SLAVE_FSTP     50000000
#define SCB_PERI_CLOCK_MASTER_STD     2000000
#define SCB_PERI_CLOCK_MASTER_FST     8500000
#define SCB_PERI_CLOCK_MASTER_FSTP    20000000

/* Implement ISR for EZI2C */
static void cyhal_ezi2c_irq_handler(void)
{
    cyhal_ezi2c_t *obj = (cyhal_ezi2c_t*) cyhal_scb_get_irq_obj();
    Cy_SCB_EZI2C_Interrupt(obj->base, &(obj->context));

    /* Call if registered callback here */
    cyhal_ezi2c_event_callback_t callback = (cyhal_ezi2c_event_callback_t) obj->callback_data.callback;
    if (callback != NULL)
    {
        callback(obj->callback_data.callback_arg, CYHAL_EZI2C_EVENT_NONE);
    }
}

static uint32_t cyhal_set_peri_divider(cyhal_ezi2c_t *obj, uint32_t freq)
{
    /* Return the actual data rate on success, 0 otherwise */
    uint32_t peri_freq = 0;
    if (freq == 0)
    {
        return 0;
    }
    if (freq <= CY_SCB_I2C_STD_DATA_RATE)
    {
        peri_freq = SCB_PERI_CLOCK_SLAVE_STD;
    }
    else if (freq <= CY_SCB_I2C_FST_DATA_RATE)
    {
        peri_freq = SCB_PERI_CLOCK_SLAVE_FST;
    }
    else if (freq <= CY_SCB_I2C_FSTP_DATA_RATE)
    {
        peri_freq = SCB_PERI_CLOCK_SLAVE_FSTP;
    }
    else
    {
        return 0;
    }

    /* Connect assigned divider to be a clock source for EZI2C */
    cy_en_sysclk_status_t status = Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)((uint8_t)PCLK_SCB0_CLOCK + obj->resource.block_num), obj->clock.div_type, obj->clock.div_num);
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphSetDivider   (obj->clock.div_type, obj->clock.div_num, cyhal_divider_value(peri_freq, 0u));
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num);
    CY_ASSERT(CY_SYSCLK_SUCCESS == status);

    return Cy_SCB_I2C_SetDataRate(obj->base, freq, Cy_SysClk_PeriphGetFrequency(obj->clock.div_type, obj->clock.div_num));
}

cy_rslt_t cyhal_ezi2c_init(cyhal_ezi2c_t *obj, cyhal_gpio_t sda, cyhal_gpio_t scl, const cyhal_clock_divider_t *clk, const cyhal_ezi2c_cfg_t *cfg)
{
    /* Validate input configuration structure */
    if ((0 == cfg->slave1_cfg.slave_address) || ((cfg->two_addresses) && (0 == cfg->slave2_cfg.slave_address)))
    {
        return CYHAL_EZI2C_RSLT_ERR_CHECK_USER_CONFIG;
    }

    CY_ASSERT(NULL != obj);

    /* Populate configuration structure */
    const cy_stc_scb_ezi2c_config_t ezI2cConfig =
    {
        .numberOfAddresses   = cfg->two_addresses ? CY_SCB_EZI2C_TWO_ADDRESSES : CY_SCB_EZI2C_ONE_ADDRESS,
        .slaveAddress1       = cfg->slave1_cfg.slave_address,
        .slaveAddress2       = cfg->slave2_cfg.slave_address,
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
    const cyhal_resource_pin_mapping_t *sda_map = CY_UTILS_GET_RESOURCE(sda, cyhal_pin_map_scb_i2c_sda);
    const cyhal_resource_pin_mapping_t *scl_map = CY_UTILS_GET_RESOURCE(scl, cyhal_pin_map_scb_i2c_scl);
    if ((NULL == sda_map) || (NULL == scl_map) || (sda_map->inst->block_num != scl_map->inst->block_num))
    {
        return CYHAL_EZI2C_RSLT_ERR_INVALID_PIN;
    }

    result = cyhal_hwmgr_reserve(scl_map->inst);

    /* Reserve the SDA pin */
    if (result == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pin_rsc = cyhal_utils_get_gpio_resource(sda);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_sda = sda;
            /* Configures the HSIOM connection to the pin */
            Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(sda), CYHAL_GET_PIN(sda), CY_GPIO_CFG_GET_HSIOM(scl_map->cfg));
            /* Configures the pin output buffer drive mode and input buffer enable */
            Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(sda), CYHAL_GET_PIN(sda), CY_GPIO_DM_OD_DRIVESLOW);
        }
    }

    /* Reserve the SCL pin */
    if (result == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pin_rsc = cyhal_utils_get_gpio_resource(scl);
        /* Connect SCB I2C function to pins */
        cy_rslt_t result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_scl = scl;
            /* Configures the HSIOM connection to the pin */
            Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(scl), CYHAL_GET_PIN(scl), CY_GPIO_CFG_GET_HSIOM(scl_map->cfg));
            /* Configures the pin output buffer drive mode and input buffer enable */
            Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(scl), CYHAL_GET_PIN(scl), CY_GPIO_DM_OD_DRIVESLOW);
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        obj->is_shared_clock = (clk != NULL);
        if (clk == NULL)
        {
            result = cyhal_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_16_BIT, false);
        }
        else
        {
            obj->clock = *clk;
        }
    }

    obj->resource = *(scl_map->inst);
    obj->base = CYHAL_SCB_BASE_ADDRESSES[obj->resource.block_num];

    if (result == CY_RSLT_SUCCESS)
    {
        /* Configure I2C to operate */
        result = Cy_SCB_EZI2C_Init(obj->base, &ezI2cConfig, &(obj->context));
    }

    int32_t ezi2c_freq;
    switch(cfg->data_rate)
    {
       	case CYHAL_EZI2C_DATA_RATE_100KHZ:
       		ezi2c_freq = 100000;
            break;
      	case CYHAL_EZI2C_DATA_RATE_400KHZ:
      	    ezi2c_freq = 400000;
       	    break;
       	case CYHAL_EZI2C_DATA_RATE_1MHZ:
       		ezi2c_freq = 1000000;
       	    break;
        default:
           	return CYHAL_EZI2C_RSLT_ERR_CHECK_USER_CONFIG;
    }

    /* Set data rate */
    int32_t dataRate = cyhal_set_peri_divider(obj, ezi2c_freq);
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
        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = 0;
        cyhal_scb_config_structs[obj->resource.block_num] = obj;

        cy_stc_sysint_t irqCfg = { CYHAL_SCB_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, cyhal_ezi2c_irq_handler);
        NVIC_EnableIRQ(CYHAL_SCB_IRQ_N[obj->resource.block_num]);

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
        IRQn_Type irqn = CYHAL_SCB_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    cyhal_utils_release_if_used(&(obj->pin_sda));
    cyhal_utils_release_if_used(&(obj->pin_scl));

    if (!obj->is_shared_clock)
    {
        cyhal_hwmgr_free_clock(&(obj->clock));
    }
}

cyhal_ezi2c_status_t cyhal_ezi2c_get_activity_status(cyhal_ezi2c_t *obj)
{
    return (cyhal_ezi2c_status_t)Cy_SCB_EZI2C_GetActivity(obj->base, &(obj->context));
}

void cyhal_ezi2c_register_callback(cyhal_ezi2c_t *obj, cyhal_ezi2c_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
