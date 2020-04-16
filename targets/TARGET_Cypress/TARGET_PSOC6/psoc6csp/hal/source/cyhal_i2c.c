/*******************************************************************************
* File Name: cyhal_i2c.c
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
#include "cyhal_i2c.h"
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

#define PENDING_NONE                    0
#define PENDING_RX                      1
#define PENDING_TX                      2
#define PENDING_TX_RX                   3

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

#define CYHAL_I2C_MASTER_DEFAULT_FREQ 100000

static const cy_stc_scb_i2c_config_t default_i2c_config = {
        .i2cMode   = CY_SCB_I2C_MASTER,
        .useRxFifo = false,
        .useTxFifo = true,
        .slaveAddress     = 0U,
        .slaveAddressMask = 0U,
        .acceptAddrInFifo = false,
        .ackGeneralAddr   = false,
        .enableWakeFromSleep = false,
        .enableDigitalFilter = false,
        .lowPhaseDutyCycle = 8U,
        .highPhaseDutyCycle = 8U,
};

static cyhal_i2c_event_t cyhal_convert_interrupt_cause(uint32_t pdl_cause);

static void cyhal_i2c_irq_handler(void)
{
    cyhal_i2c_t *obj = (cyhal_i2c_t*) cyhal_scb_get_irq_obj();

    Cy_SCB_I2C_Interrupt(obj->base, &(obj->context));

    if (obj->async)
    {
        /* This code is part of cyhal_i2c_master_transfer_async() API functionality */
        /* cyhal_i2c_master_transfer_async() API uses this interrupt handler for RX Transfer */
        if (0 == (Cy_SCB_I2C_MasterGetStatus(obj->base,  &obj->context) & CY_SCB_I2C_MASTER_BUSY))
        {
            if (obj->tx_config.bufferSize)
            {
                /* Start RX Transfer */
                obj->pending = PENDING_RX;
                Cy_SCB_I2C_MasterRead(obj->base, &obj->rx_config, &obj->context);
                /* Finish Async Transfer */
                obj->pending = PENDING_NONE;
                obj->async = false;
            }
        }
    }
}

static void cyhal_i2c_cb_wrapper(uint32_t event)
{
    cyhal_i2c_t *obj = (cyhal_i2c_t*) cyhal_scb_get_irq_obj();
    cyhal_i2c_irq_event_t anded_events = (cyhal_i2c_irq_event_t)(obj->irq_cause & (uint32_t)cyhal_convert_interrupt_cause(event));
    if (anded_events)
    {
        cyhal_i2c_event_callback_t callback = (cyhal_i2c_event_callback_t) obj->callback_data.callback;
        callback(obj->callback_data.callback_arg, anded_events);
    }
}

static uint32_t cyhal_set_peri_divider(cyhal_i2c_t *obj, uint32_t freq, bool is_slave)
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
    cy_en_sysclk_status_t status = Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)((uint8_t)PCLK_SCB0_CLOCK + obj->resource.block_num), obj->clock.div_type, obj->clock.div_num);
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, cyhal_divider_value(peri_freq, 0u));
    if (status == CY_SYSCLK_SUCCESS)
        status = Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num);
    CY_ASSERT(CY_SYSCLK_SUCCESS == status);

    /* According to PDL API Reference Guide - Cy_SysClk_PeriphGetFrequency() use only for i2c master role */
    if(!is_slave)
    {
    	return Cy_SCB_I2C_SetDataRate(obj->base, freq, Cy_SysClk_PeriphGetFrequency(obj->clock.div_type, obj->clock.div_num));
    }
    else
    {
    	return Cy_SCB_I2C_GetDataRate(obj->base, Cy_SysClk_PeriphGetFrequency(obj->clock.div_type, obj->clock.div_num));
    }
}

/* Start API implementing */

cy_rslt_t cyhal_i2c_init(cyhal_i2c_t *obj, cyhal_gpio_t sda, cyhal_gpio_t scl, const cyhal_clock_divider_t *clk)
{
    CY_ASSERT(NULL != obj);

    /* Explicitly marked not allocated resources as invalid to prevent freeing them. */
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->pin_scl = CYHAL_NC_PIN_VALUE;
    obj->pin_sda = CYHAL_NC_PIN_VALUE;
    obj->is_shared_clock = true;

    /* Reserve the I2C */
    const cyhal_resource_pin_mapping_t *sda_map = CY_UTILS_GET_RESOURCE(sda, cyhal_pin_map_scb_i2c_sda);
    const cyhal_resource_pin_mapping_t *scl_map = CY_UTILS_GET_RESOURCE(scl, cyhal_pin_map_scb_i2c_scl);
    if ((NULL == sda_map) || (NULL == scl_map) || (sda_map->inst->block_num != scl_map->inst->block_num))
    {
        return CYHAL_I2C_RSLT_ERR_INVALID_PIN;
    }
    obj->resource = *(scl_map->inst);
    cy_rslt_t result = cyhal_hwmgr_reserve(&(obj->resource));
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    /* Reserve the SDA pin */
    cyhal_resource_inst_t pin_rsc = cyhal_utils_get_gpio_resource(sda);
    result = cyhal_hwmgr_reserve(&pin_rsc);
    if (result == CY_RSLT_SUCCESS)
    {
        obj->pin_sda = sda;
    }
    Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(sda), CYHAL_GET_PIN(sda), CY_GPIO_CFG_GET_HSIOM(scl_map->cfg));
    Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(sda), CYHAL_GET_PIN(sda), CY_GPIO_DM_OD_DRIVESLOW);

    /* Reserve the SCL pin */
    if (result == CY_RSLT_SUCCESS)
    {
        pin_rsc = cyhal_utils_get_gpio_resource(scl);
        /* Connect SCB I2C function to pins */
        cy_rslt_t result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_scl = scl;
        }
        Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(scl), CYHAL_GET_PIN(scl), CY_GPIO_CFG_GET_HSIOM(scl_map->cfg));
        Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(scl), CYHAL_GET_PIN(scl), CY_GPIO_DM_OD_DRIVESLOW);

    }

    obj->base = CYHAL_SCB_BASE_ADDRESSES[obj->resource.block_num];

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

    if (result == CY_RSLT_SUCCESS)
    {
        uint32_t dataRate = cyhal_set_peri_divider(obj, CYHAL_I2C_MASTER_DEFAULT_FREQ, false);
        if (dataRate == 0)
        {
            /* Can not reach desired data rate */
            result = CYHAL_I2C_RSLT_ERR_CAN_NOT_REACH_DR;
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        /* Configure I2C to operate */
        result = (cy_rslt_t)Cy_SCB_I2C_Init(obj->base, &default_i2c_config, &(obj->context));
    }

    if (result == CY_RSLT_SUCCESS)
    {
        /* Enable I2C to operate */
        Cy_SCB_I2C_Enable(obj->base);

        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = CYHAL_I2C_EVENT_NONE;
        cyhal_scb_config_structs[obj->resource.block_num] = obj;

        cy_stc_sysint_t irqCfg = { CYHAL_SCB_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, cyhal_i2c_irq_handler);
        NVIC_EnableIRQ(CYHAL_SCB_IRQ_N[obj->resource.block_num]);
    }

    if (result != CY_RSLT_SUCCESS)
    {
        cyhal_i2c_free(obj);
    }
    return result;
}

void cyhal_i2c_free(cyhal_i2c_t *obj)
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

cy_rslt_t cyhal_i2c_configure(cyhal_i2c_t *obj, const cyhal_i2c_cfg_t *cfg)
{
    (void) Cy_SCB_I2C_Disable(obj->base, &obj->context);

    cy_stc_scb_i2c_config_t config_structure = default_i2c_config;
    config_structure.i2cMode = (cfg->is_slave)
        ? CY_SCB_I2C_SLAVE
        : CY_SCB_I2C_MASTER;

    config_structure.slaveAddress  = cfg->address;

    /* Set slave address mask if I2C is operate in slave mode */
    if (cfg->is_slave)
    {
        config_structure.slaveAddressMask = 0xFEU;
    }

    /* Set data rate */
    uint32_t dataRate = cyhal_set_peri_divider(obj, cfg->frequencyhal_hz, cfg->is_slave);
    if (dataRate == 0)
    {
        /* Can not reach desired data rate */
        return CYHAL_I2C_RSLT_ERR_CAN_NOT_REACH_DR;
    }

    cy_rslt_t result = (cy_rslt_t)Cy_SCB_I2C_Init(obj->base, &config_structure, &(obj->context));
    (void) Cy_SCB_I2C_Enable(obj->base);

    return result;
}

cy_rslt_t cyhal_i2c_master_write(cyhal_i2c_t *obj, uint16_t dev_addr, const uint8_t *data, uint16_t size, uint32_t timeout, bool send_stop)
{
    cy_en_scb_i2c_status_t status = obj->context.state == CY_SCB_I2C_IDLE
        ? Cy_SCB_I2C_MasterSendStart(obj->base, dev_addr, CY_SCB_I2C_WRITE_XFER, timeout, &obj->context)
        : Cy_SCB_I2C_MasterSendReStart(obj->base, dev_addr, CY_SCB_I2C_WRITE_XFER, timeout, &obj->context);

    if (status == CY_SCB_I2C_SUCCESS)
    {
        while (size > 0)
        {
            status = Cy_SCB_I2C_MasterWriteByte(obj->base, *data, timeout, &obj->context);
            if (status != CY_SCB_I2C_SUCCESS)
            {
                break;
            }
            --size;
            ++data;
        }
    }

    if (send_stop)
    {
        /* SCB in I2C mode is very time sensitive. In practice we have to request STOP after */
        /* each block, otherwise it may break the transmission */
        Cy_SCB_I2C_MasterSendStop(obj->base, timeout, &obj->context);
    }

    return status;
}

cy_rslt_t cyhal_i2c_master_read(cyhal_i2c_t *obj, uint16_t dev_addr, uint8_t *data, uint16_t size, uint32_t timeout, bool send_stop)
{
    cy_en_scb_i2c_command_t ack = CY_SCB_I2C_ACK;

    /* Start transaction, send dev_addr */
    cy_en_scb_i2c_status_t status = obj->context.state == CY_SCB_I2C_IDLE
        ? Cy_SCB_I2C_MasterSendStart(obj->base, dev_addr, CY_SCB_I2C_READ_XFER, timeout, &obj->context)
        : Cy_SCB_I2C_MasterSendReStart(obj->base, dev_addr, CY_SCB_I2C_READ_XFER, timeout, &obj->context);

    if (status == CY_SCB_I2C_SUCCESS)
    {
        while (size > 0) {
            if (size == 1)
            {
                ack = CY_SCB_I2C_NAK;
            }
            status = Cy_SCB_I2C_MasterReadByte(obj->base, ack, (uint8_t *)data, timeout, &obj->context);
            if (status != CY_SCB_I2C_SUCCESS)
            {
                break;
            }
            --size;
            ++data;
        }
    }

    if (send_stop)
    {
        /* SCB in I2C mode is very time sensitive. In practice we have to request STOP after */
        /* each block, otherwise it may break the transmission */
        Cy_SCB_I2C_MasterSendStop(obj->base, timeout, &obj->context);
    }
    return status;
}

cy_rslt_t cyhal_i2c_slave_config_write_buff(cyhal_i2c_t *obj, const uint8_t *data, uint16_t size)
{
    if (obj->context.state == CY_SCB_I2C_IDLE)
    {
        Cy_SCB_I2C_SlaveConfigReadBuf(obj->base, (uint8_t *)data, size, &obj->context);
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2c_slave_config_read_buff(cyhal_i2c_t *obj, uint8_t *data, uint16_t size)
{
    if (obj->context.state == CY_SCB_I2C_IDLE)
    {
        Cy_SCB_I2C_SlaveConfigWriteBuf(obj->base, (uint8_t *)data, size, &obj->context);
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2c_master_mem_write(cyhal_i2c_t *obj, uint16_t address, uint16_t mem_addr, uint16_t mem_addr_size, const uint8_t *data, uint16_t size, uint32_t timeout)
{
    uint8_t mem_addr_buf[2];
    if (mem_addr_size == 1)
    {
        mem_addr_buf[0] = (uint8_t)mem_addr;
    }
    else if (mem_addr_size == 2)
    {
        mem_addr_buf[0] = (uint8_t)(mem_addr >> 8);
        mem_addr_buf[1] = (uint8_t)mem_addr;
    }
    else
    {
        return CYHAL_I2C_RSLT_ERR_INVALID_ADDRESS_SIZE;
    }

    cy_rslt_t status = cyhal_i2c_master_write(obj, address, mem_addr_buf, mem_addr_size, timeout, false);

    if (status == CY_RSLT_SUCCESS)
    {
        while (size > 0)
        {
            status = Cy_SCB_I2C_MasterWriteByte(obj->base, *data, timeout, &obj->context);
            if (status != CY_SCB_I2C_SUCCESS)
            {
                break;
            }
            --size;
            ++data;
        }
        /* SCB in I2C mode is very time sensitive. In practice we have to request STOP after */
        /* each block, otherwise it may break the transmission */
        Cy_SCB_I2C_MasterSendStop(obj->base, timeout, &obj->context);
    }
    return status;
}

cy_rslt_t cyhal_i2c_master_mem_read(cyhal_i2c_t *obj, uint16_t address, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *data, uint16_t size, uint32_t timeout)
{
    uint8_t mem_addr_buf[2];
    if (mem_addr_size == 1)
    {
        mem_addr_buf[0] = (uint8_t)mem_addr;
    }
    else if (mem_addr_size == 2)
    {
        mem_addr_buf[0] = (uint8_t)(mem_addr >> 8);
        mem_addr_buf[1] = (uint8_t)mem_addr;
    }
    else
    {
        return CYHAL_I2C_RSLT_ERR_INVALID_ADDRESS_SIZE;
    }

    cy_rslt_t status = cyhal_i2c_master_write(obj, address, mem_addr_buf, mem_addr_size, timeout, false);
    if (status == CY_RSLT_SUCCESS)
    {
        status = cyhal_i2c_master_read(obj, address, data, size, timeout, true);
    }
    return status;
}

cy_rslt_t cyhal_i2c_master_transfer_async(cyhal_i2c_t *obj, uint16_t address, const void *tx, size_t tx_size, void *rx, size_t rx_size)
{
    obj->rx_config.slaveAddress = address;
    obj->tx_config.slaveAddress = address;

    obj->rx_config.buffer = rx;
    obj->rx_config.bufferSize = rx_size;

    obj->tx_config.buffer = (void *)tx;
    obj->tx_config.bufferSize = tx_size;

    obj->async = true;

    if (tx_size)
    {
        /* Write first, then read, or write only. */
        if (rx_size > 0)
        {
            obj->pending = PENDING_TX_RX;
        }
        else
        {
            obj->pending = PENDING_TX;
        }
        /* Transmit */
        Cy_SCB_I2C_MasterWrite(obj->base, &obj->tx_config, &obj->context);

        /* Receive covered by interrupt handler */
    }
    else if (rx_size)
    {
        /* Read transaction */
        obj->pending = PENDING_RX;
        Cy_SCB_I2C_MasterRead(obj->base, &obj->rx_config, &obj->context);
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2c_abort_async(cyhal_i2c_t *obj)
{
    if (obj->pending != PENDING_NONE)
    {
        if (obj->pending == PENDING_RX)
        {
            Cy_SCB_I2C_MasterAbortRead(obj->base, &obj->context);
        }
        else
        {
            Cy_SCB_I2C_MasterAbortWrite(obj->base, &obj->context);
        }
    }
    return CY_RSLT_SUCCESS;
}

static cyhal_i2c_event_t cyhal_convert_interrupt_cause(uint32_t pdl_cause)
{
    cyhal_i2c_event_t cause;
    switch(pdl_cause)
    {
        case CY_SCB_I2C_SLAVE_READ_EVENT:
            cause = CYHAL_I2C_SLAVE_READ_EVENT;
            break;
        case CY_SCB_I2C_SLAVE_WRITE_EVENT:
            cause = CYHAL_I2C_SLAVE_WRITE_EVENT;
            break;
        case CY_SCB_I2C_SLAVE_RD_IN_FIFO_EVENT:
            cause = CYHAL_I2C_SLAVE_RD_IN_FIFO_EVENT;
            break;
        case CY_SCB_I2C_SLAVE_RD_BUF_EMPTY_EVENT:
            cause = CYHAL_I2C_SLAVE_RD_BUF_EMPTY_EVENT;
            break;
        case CY_SCB_I2C_SLAVE_RD_CMPLT_EVENT:
            cause = CYHAL_I2C_SLAVE_RD_CMPLT_EVENT;
            break;
        case CY_SCB_I2C_SLAVE_WR_CMPLT_EVENT:
            cause = CYHAL_I2C_SLAVE_WR_CMPLT_EVENT;
            break;
        case CY_SCB_I2C_SLAVE_ERR_EVENT:
            cause = CYHAL_I2C_SLAVE_ERR_EVENT;
            break;
        case CY_SCB_I2C_MASTER_WR_IN_FIFO_EVENT:
            cause = CYHAL_I2C_MASTER_WR_IN_FIFO_EVENT;
            break;
        case CY_SCB_I2C_MASTER_WR_CMPLT_EVENT:
            cause = CYHAL_I2C_MASTER_WR_CMPLT_EVENT;
            break;
        case CY_SCB_I2C_MASTER_RD_CMPLT_EVENT:
            cause = CYHAL_I2C_MASTER_RD_CMPLT_EVENT;
            break;
        case CY_SCB_I2C_MASTER_ERR_EVENT:
            cause = CYHAL_I2C_MASTER_ERR_EVENT;
            break;
        default:
            cause = CYHAL_I2C_EVENT_NONE;
            break;
    }
    return cause;
}

void cyhal_i2c_register_callback(cyhal_i2c_t *obj, cyhal_i2c_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
    Cy_SCB_I2C_RegisterEventCallback(obj->base, cyhal_i2c_cb_wrapper, &(obj->context));

    obj->irq_cause = CYHAL_I2C_EVENT_NONE;
}

void cyhal_i2c_enable_event(cyhal_i2c_t *obj, cyhal_i2c_event_t event, uint8_t intrPriority, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }

    IRQn_Type irqn = CYHAL_SCB_IRQ_N[obj->resource.block_num];
    NVIC_SetPriority(irqn, intrPriority);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
