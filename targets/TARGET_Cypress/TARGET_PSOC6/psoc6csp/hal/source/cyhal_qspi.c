/***************************************************************************//**
* \file cyhal_qspi.c
*
* Description:
* Provides a high level interface for interacting with the Cypress QSPI (SMIF).
* This is a wrapper around the lower level PDL API.
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

#include <string.h>
#include "cy_smif.h"
#include "cyhal_utils.h"
#include "cyhal_qspi.h"
#include "cyhal_hwmgr.h"
#include "cyhal_gpio.h"
#include "cyhal_interconnect.h"
#include "cyhal_system_impl.h"

#ifdef CY_IP_MXSMIF

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
*       Internal
*******************************************************************************/
/* in microseconds, timeout for all blocking functions */
#define TIMEOUT_10_MS (10000UL)

#define MAX_DATA_PINS 8

#define SMIF_MAX_RX_COUNT (65536UL)
#define QSPI_DESELECT_DELAY (7UL)

#if (defined(SMIF_CHIP_TOP_DATA8_PRESENT) && (SMIF_CHIP_TOP_DATA8_PRESENT))   || \
    (defined(SMIF0_CHIP_TOP_DATA8_PRESENT) && (SMIF0_CHIP_TOP_DATA8_PRESENT))
    #define DATA8_PRESENT   1
#else
    #define DATA8_PRESENT   0
#endif
#if (SMIF_CHIP_TOP_SPI_SEL_NR > 1)
    #define SPI_SEL1   1
#else
    #define SPI_SEL1   0
#endif
#if (SMIF_CHIP_TOP_SPI_SEL_NR > 2)
    #define SPI_SEL2   1
#else
    #define SPI_SEL2   0
#endif
#if (SMIF_CHIP_TOP_SPI_SEL_NR > 3)
    #define SPI_SEL3   1
#else
    #define SPI_SEL3   0
#endif

static cyhal_qspi_t *cyhal_qspi_config_structs[CY_IP_MXSMIF_INSTANCES];

/* Default QSPI configuration */
static const cy_stc_smif_config_t default_qspi_config =
{
    .mode = (uint32_t)CY_SMIF_NORMAL,
    .deselectDelay = QSPI_DESELECT_DELAY,
    .rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
    .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};

/* List of available QSPI instances */
static SMIF_Type *const smif_base_addresses[CY_IP_MXSMIF_INSTANCES] =
{
#ifdef SMIF0
    SMIF0,
#endif /* ifdef SMIF0 */
};

/* List of available QSPI interrupt sources */
static const IRQn_Type CYHAL_QSPI_IRQ_N[CY_IP_MXSMIF_INSTANCES] =
{
#ifdef SMIF0
    smif_interrupt_IRQn,
#endif /* ifdef SMIF0 */
};

static uint8_t cyhal_qspi_get_block_from_irqn(IRQn_Type irqn)
{
    switch (irqn)
    {
#if (CY_CPU_CORTEX_M4)
#ifdef SMIF0
        case smif_interrupt_IRQn: return 0;
#endif /* ifdef SMIF0 */
#endif /* (CY_CPU_CORTEX_M4) */
        default:
            CY_ASSERT(false); // Should never be called with a non-SMIF IRQn
            return 0;
    }
}

/* Translate PDL QSPI interrupt cause into QSPI HAL interrupt cause */
static cyhal_qspi_event_t cyhal_convert_event_from_pdl(uint32_t pdl_intr_cause)
{
    cyhal_qspi_event_t intr_cause;

    switch (pdl_intr_cause)
    {
        case CY_SMIF_SEND_CMPLT:
            intr_cause = CYHAL_QSPI_IRQ_TRANSMIT_DONE;
            break;
        case CY_SMIF_REC_CMPLT:
            intr_cause = CYHAL_QSPI_IRQ_RECEIVE_DONE;
            break;
        default:
            intr_cause = CYHAL_QSPI_EVENT_NONE;
    }

    return intr_cause;
}

static cyhal_qspi_t *cyhal_qspi_get_irq_obj(void)
{
    IRQn_Type irqn = CYHAL_GET_CURRENT_IRQN();
    uint8_t block = cyhal_qspi_get_block_from_irqn(irqn);
    return cyhal_qspi_config_structs[block];
}

/*******************************************************************************
*       Dispatcher Interrrupt Service Routine
*******************************************************************************/

static void cyhal_qspi_cb_wrapper(uint32_t event)
{
    cyhal_qspi_t *obj = cyhal_qspi_get_irq_obj();
    cyhal_qspi_event_t anded_events = (cyhal_qspi_event_t)(obj->irq_cause & (uint32_t)cyhal_convert_event_from_pdl(event));
    if (anded_events)
    {
        cyhal_qspi_event_callback_t callback = (cyhal_qspi_event_callback_t) obj->callback_data.callback;
        callback(obj->callback_data.callback_arg, cyhal_convert_event_from_pdl(event));
    }
}

/*******************************************************************************
*       (Internal) Interrrupt Service Routines
*******************************************************************************/

/* Interrupt call, needed for SMIF Async operations */
static void cyhal_qspi_irq_handler()
{
    cyhal_qspi_t *obj = cyhal_qspi_get_irq_obj();
    Cy_SMIF_Interrupt(obj->base, &(obj->context));
}

/*******************************************************************************
*       (Internal) QSPI Pin Related Functions
*******************************************************************************/

#ifndef __MBED__

/* Check if pin valid as resource and reserve it */
static cy_rslt_t check_pin_and_reserve(cyhal_gpio_t pin)
{
    cyhal_resource_inst_t pin_rsc = cyhal_utils_get_gpio_resource(pin);
    return cyhal_hwmgr_reserve(&pin_rsc);
}

/* Checks what pins are provided by user and calls check_pin_and_reserve for each */
static cy_rslt_t make_pin_reservations(cyhal_qspi_t *obj)
{
    cy_rslt_t result;

    /* reserve the SCLK pin */
    result = check_pin_and_reserve(obj->pin_sclk);

    /* reserve the ssel pin */
    if (result == CY_RSLT_SUCCESS)
    {
        result = check_pin_and_reserve(obj->pin_ssel);
    }

    /* reserve the io pins */
    for (uint8_t i = 0; (i < MAX_DATA_PINS) && (result == CY_RSLT_SUCCESS); i++)
    {
        if (NC != obj->pin_ios[i])
        {
            result = check_pin_and_reserve(obj->pin_ios[i]);
        }
    }

    return result;
}

#endif

/*******************************************************************************
*       (Internal) QSPI Config Related Functions
*******************************************************************************/

/* Translates HAL bus width to PDL bus width */
static cy_en_smif_txfr_width_t get_cyhal_bus_width(cyhal_qspi_bus_width_t bus_width)
{
    cy_en_smif_txfr_width_t cyhal_bus_width;

    switch (bus_width)
    {
        case CYHAL_QSPI_CFG_BUS_SINGLE:
            cyhal_bus_width = CY_SMIF_WIDTH_SINGLE;
            break;
        case CYHAL_QSPI_CFG_BUS_DUAL:
            cyhal_bus_width = CY_SMIF_WIDTH_DUAL;
            break;
        case CYHAL_QSPI_CFG_BUS_QUAD:
            cyhal_bus_width = CY_SMIF_WIDTH_QUAD;
            break;
        case CYHAL_QSPI_CFG_BUS_OCTAL:
            cyhal_bus_width = CY_SMIF_WIDTH_OCTAL;
            break;
        default:
            cyhal_bus_width = CY_SMIF_WIDTH_SINGLE;
    }

    return cyhal_bus_width;
}

/* Translates cyhal_qspi_command_t to cy_stc_smif_mem_cmd_t */
static void convert_to_cyhal_cmd_config(const cyhal_qspi_command_t *qspi_command,
    cy_stc_smif_mem_cmd_t *const cyhal_cmd_config)
{
    /* This function does not check 'disabled' of each sub-structure in qspi_command_t
    *  It is the responsibility of the caller to check it. */
    cyhal_cmd_config->command = qspi_command->instruction.value;
    cyhal_cmd_config->cmdWidth = get_cyhal_bus_width(qspi_command->instruction.bus_width);
    cyhal_cmd_config->addrWidth = get_cyhal_bus_width(qspi_command->address.bus_width);
    cyhal_cmd_config->mode = qspi_command->mode_bits.value;
    cyhal_cmd_config->modeWidth = get_cyhal_bus_width(qspi_command->mode_bits.bus_width);
    cyhal_cmd_config->dummyCycles = qspi_command->dummy_count;
    cyhal_cmd_config->dataWidth = get_cyhal_bus_width(qspi_command->data.bus_width);
}

static void uint32_to_byte_array(uint32_t value, uint8_t *byteArray, uint32_t startPos, uint32_t size)
{
    do
    {
        size--;
        byteArray[size + startPos] = (uint8_t)(value & 0xFF);
        value >>= 0x08;
    } while (size > 0);
}

/* cyhal_qspi_size_t to number bytes */
static inline uint32_t get_size(cyhal_qspi_size_t hal_size)
{
    return ((uint32_t)hal_size >> 3); /* convert bits to bytes */
}

/* Sends QSPI command with certain set of data */
/* Address passed through 'command' is not used, instead the value in 'addr' is used. */
static cy_rslt_t qspi_command_transfer(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command,
    uint32_t addr, bool endOfTransfer)
{
    /* max address size is 4 bytes and max mode bits size is 4 bytes */
    uint8_t cmd_param[8] = {0};
    uint32_t start_pos = 0;
    uint32_t addr_size = 0;
    uint32_t mode_bits_size = 0;
    cy_en_smif_txfr_width_t bus_width = CY_SMIF_WIDTH_SINGLE;
    cy_stc_smif_mem_cmd_t cyhal_cmd_config;
    cy_rslt_t result = CY_RSLT_SUCCESS;

    convert_to_cyhal_cmd_config(command, &cyhal_cmd_config);

    /* Does not support different bus_width for address and mode bits.
     * bus_width is selected based on what (address or mode bits) is enabled.
     * If both are enabled, bus_width of mode bits is selected
     * It is either possible to support 1 byte mode bits with different bus_width
     * by sending the mode byte as command as done in Cy_SMIF_Memslot_CmdRead()
     * in cyhal_smif_memslot.c or support multiple bytes of mode bits with same bus_width
     * as address by passing the mode bytes as cmd_param to Cy_SMIF_TransmitCommand().
     * Second approach is implemented here. This restriction is because of the way
     * PDL API is implemented.
     */

    if (!command->address.disabled && !command->mode_bits.disabled)
    {
        if (cyhal_cmd_config.addrWidth != cyhal_cmd_config.modeWidth)
        {
            result = CYHAL_QSPI_RSLT_ERR_BUS_WIDTH;
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        if (!command->address.disabled)
        {
            addr_size = get_size(command->address.size);
            uint32_to_byte_array(addr, cmd_param, start_pos, addr_size);
            start_pos += addr_size;
            bus_width = cyhal_cmd_config.addrWidth;
        }

        if (!command->mode_bits.disabled)
        {
            mode_bits_size = get_size(command->mode_bits.size);
            uint32_to_byte_array(cyhal_cmd_config.mode, cmd_param, start_pos, mode_bits_size);
            bus_width = cyhal_cmd_config.modeWidth;
        }

        uint32_t cmpltTxfr = ((endOfTransfer) ? 1UL : 0UL);
        result = (cy_rslt_t)Cy_SMIF_TransmitCommand(obj->base, cyhal_cmd_config.command,
                                                         cyhal_cmd_config.cmdWidth, cmd_param, (addr_size + mode_bits_size),
                                                         bus_width, obj->slave_select, cmpltTxfr, &obj->context);
    }
    return result;
}

/* Checks, that user provided all needed pins and returns max bus width */
static cy_rslt_t check_user_pins(cyhal_qspi_t *obj, cyhal_qspi_bus_width_t *max_width)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    if (NC == obj->pin_sclk || NC == obj->pin_ssel)
    {
        *max_width = CYHAL_QSPI_CFG_BUS_SINGLE;
        result = CYHAL_QSPI_RSLT_ERR_PIN;
    }
    else
    {
#if DATA8_PRESENT
        if (NC != obj->pin_ios[4])
        {
            *max_width = CYHAL_QSPI_CFG_BUS_OCTAL;
        }
        else
#endif
        if (NC != obj->pin_ios[2])
        {
            *max_width = CYHAL_QSPI_CFG_BUS_QUAD;
        }
        else if (NC != obj->pin_ios[1])
        {
            *max_width = CYHAL_QSPI_CFG_BUS_DUAL;
        }
        else
        {
            *max_width = CYHAL_QSPI_CFG_BUS_SINGLE;
        }

        for (uint8_t i = 1; i <= MAX_DATA_PINS; i++)
        {
            /* Pins with index lower than width must be provided, pins above should be NC */
            if ((NC == obj->pin_ios[i-1]) != (i > *max_width))
            {
                result = CYHAL_QSPI_RSLT_ERR_PIN;
            }
        }
    }

    return result;
}

/* Based on ssel pin chosen, determines SMIF slave select parameter and pin mapping */
static const cyhal_resource_pin_mapping_t *get_slaveselect(cyhal_gpio_t ssel, cy_en_smif_slave_select_t *slave_select)
{
#if SPI_SEL1 || SPI_SEL2 || SPI_SEL3
    bool pin_found = false;
#endif
    const cyhal_resource_pin_mapping_t *pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select0);
    if (NULL != pin_mapping)
    {
#if SPI_SEL1 || SPI_SEL2 || SPI_SEL3
        pin_found = true;
#endif
        *slave_select = CY_SMIF_SLAVE_SELECT_0;
    }
#if SPI_SEL1
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select1);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *slave_select = CY_SMIF_SLAVE_SELECT_1;
        }
    }
#endif
#if SPI_SEL2
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select2);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *slave_select = CY_SMIF_SLAVE_SELECT_2;
        }
    }
#endif
#if SPI_SEL3
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select3);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *slave_select = CY_SMIF_SLAVE_SELECT_3;
        }
    }
#endif

    return pin_mapping;
}

/* Based on data pins chosen, determines SMIF data select parameter */
static const cyhal_resource_pin_mapping_t *get_dataselect(cyhal_gpio_t io0, cy_en_smif_data_select_t *data_select)
{
    bool pin_found = false;
    const cyhal_resource_pin_mapping_t *pin_mapping = CY_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data0);
    if (NULL != pin_mapping)
    {
        pin_found = true;
        *data_select = CY_SMIF_DATA_SEL0;
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data2);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL1;
        }
    }
#if DATA8_PRESENT
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data4);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL2;
        }
    }
    if (!pin_found)
    {
        pin_mapping = CY_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data6);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL3;
        }
    }
#endif
    return pin_mapping;
}

/*******************************************************************************
*       Functions
*******************************************************************************/

cy_rslt_t cyhal_qspi_init(
    cyhal_qspi_t *obj, cyhal_gpio_t io0, cyhal_gpio_t io1, cyhal_gpio_t io2, cyhal_gpio_t io3,
    cyhal_gpio_t io4, cyhal_gpio_t io5, cyhal_gpio_t io6, cyhal_gpio_t io7, cyhal_gpio_t sclk,
    cyhal_gpio_t ssel, uint32_t hz, uint8_t mode)
{
    /* mode (CPOL and CPHA) is not supported in PSoC 6 */
    (void)mode;

    CY_ASSERT(NULL != obj);

    /* Explicitly marked not allocated resources as invalid to prevent freeing them. */
    memset(obj, 0, sizeof(cyhal_qspi_t));
    obj->resource.type = CYHAL_RSC_INVALID;

    cy_en_smif_slave_select_t slave_select = CY_SMIF_SLAVE_SELECT_0;
    cy_en_smif_data_select_t data_select = CY_SMIF_DATA_SEL0;
    cyhal_qspi_bus_width_t max_width;

    obj->pin_sclk = sclk;
    obj->pin_ssel = ssel;
    obj->pin_ios[0] = io0;
    obj->pin_ios[1] = io1;
    obj->pin_ios[2] = io2;
    obj->pin_ios[3] = io3;
    obj->pin_ios[4] = io4;
    obj->pin_ios[5] = io5;
    obj->pin_ios[6] = io6;
    obj->pin_ios[7] = io7;

    cy_rslt_t result = check_user_pins(obj, &max_width);

    const cyhal_resource_pin_mapping_t *ssel_map = NULL;
    const cyhal_resource_pin_mapping_t *io0_map = NULL;
    const cyhal_resource_pin_mapping_t *io1_map = NULL;
    const cyhal_resource_pin_mapping_t *io2_map = NULL;
    const cyhal_resource_pin_mapping_t *io3_map = NULL;
    const cyhal_resource_pin_mapping_t *io4_map = NULL;
    const cyhal_resource_pin_mapping_t *io5_map = NULL;
    const cyhal_resource_pin_mapping_t *io6_map = NULL;
    const cyhal_resource_pin_mapping_t *io7_map = NULL;
    const cyhal_resource_pin_mapping_t *sclk_map = CY_UTILS_GET_RESOURCE(sclk, cyhal_pin_map_smif_spi_clk);

    if (NULL == sclk_map)
    {
        result = CYHAL_QSPI_RSLT_ERR_PIN;
    }
    if (CY_RSLT_SUCCESS == result)
    {
        ssel_map = get_slaveselect(ssel, &slave_select);
        if (ssel_map == NULL)
        {
            result = CYHAL_QSPI_RSLT_ERR_PIN;
        }
        else
        {
            obj->slave_select = slave_select;
        }
    }
    if (CY_RSLT_SUCCESS == result)
    {
        io0_map = get_dataselect(obj->pin_ios[0], &data_select);
        if (io0_map == NULL)
        {
            result = CYHAL_QSPI_RSLT_ERR_PIN;
        }
        else
        {
            obj->data_select = data_select;
        }
    }
    if (CY_RSLT_SUCCESS == result)
    {
        switch (data_select)
        {
            case CY_SMIF_DATA_SEL0:
                if (max_width >= CYHAL_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[1], cyhal_pin_map_smif_spi_data1);
                    if (NULL == io1_map)
                    {
                        result = CYHAL_QSPI_RSLT_ERR_PIN;
                    }
                }
                if (max_width >= CYHAL_QSPI_CFG_BUS_QUAD)
                {
                    io2_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[2], cyhal_pin_map_smif_spi_data2);
                    io3_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[3], cyhal_pin_map_smif_spi_data3);
                    if ((NULL == io2_map) || (NULL == io3_map))
                    {
                        result = CYHAL_QSPI_RSLT_ERR_PIN;
                    }
                }
#if DATA8_PRESENT
                if (max_width >= CYHAL_QSPI_CFG_BUS_OCTAL)
                {
                    io4_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[4], cyhal_pin_map_smif_spi_data4);
                    io5_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[5], cyhal_pin_map_smif_spi_data5);
                    io6_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[6], cyhal_pin_map_smif_spi_data6);
                    io7_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[7], cyhal_pin_map_smif_spi_data7);
                    if ((NULL == io4_map) || (NULL == io5_map) || (NULL == io6_map) || (NULL == io7_map))
                    {
                        result = CYHAL_QSPI_RSLT_ERR_PIN;
                    }
                }
#endif
                break;
            case CY_SMIF_DATA_SEL1:
                if (max_width >= CYHAL_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[1], cyhal_pin_map_smif_spi_data3);
                    if (NULL == io1_map)
                    {
                        result = CYHAL_QSPI_RSLT_ERR_PIN;
                    }
                }
                break;
#if DATA8_PRESENT
            case CY_SMIF_DATA_SEL2:
                if (max_width >= CYHAL_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[1], cyhal_pin_map_smif_spi_data5);
                    if (NULL == io1_map)
                    {
                        result = CYHAL_QSPI_RSLT_ERR_PIN;
                    }
                }
                if (max_width >= CYHAL_QSPI_CFG_BUS_QUAD)
                {
                    io2_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[2], cyhal_pin_map_smif_spi_data6);
                    io3_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[3], cyhal_pin_map_smif_spi_data7);
                    if ((NULL == io2_map) || (NULL == io3_map))
                    {
                        result = CYHAL_QSPI_RSLT_ERR_PIN;
                    }
                }
                break;

            case CY_SMIF_DATA_SEL3:
                if (max_width >= CYHAL_QSPI_CFG_BUS_DUAL)
                {
                    io1_map = CY_UTILS_GET_RESOURCE(obj->pin_ios[1], cyhal_pin_map_smif_spi_data7);
                    if (NULL == io1_map)
                    {
                        result = CYHAL_QSPI_RSLT_ERR_PIN;
                    }
                }
                break;
#endif
            default:
                result = CYHAL_QSPI_RSLT_ERR_DATA_SEL;
        }
    }
    /* Check that all pins are belongs to same instance */
    if (CY_RSLT_SUCCESS == result)
    {
        if (sclk_map->inst->block_num != ssel_map->inst->block_num ||
            sclk_map->inst->block_num != io0_map->inst->block_num)
        {
            result = CYHAL_QSPI_RSLT_ERR_PIN;
        }
    }
    if (CY_RSLT_SUCCESS == result && (max_width >= CYHAL_QSPI_CFG_BUS_DUAL))
    {
        if (sclk_map->inst->block_num != io1_map->inst->block_num)
        {
            result = CYHAL_QSPI_RSLT_ERR_PIN;
        }
    }
    /* Pins io2 and io3 are only available in CY_SMIF_DATA_SEL0 and CY_SMIF_DATA_SEL2 modes */
    if ((CY_RSLT_SUCCESS == result) && ((data_select == CY_SMIF_DATA_SEL0) || (data_select == CY_SMIF_DATA_SEL2))
        && (max_width >= CYHAL_QSPI_CFG_BUS_QUAD))
    {
        if ((sclk_map->inst->block_num != io2_map->inst->block_num) ||
            (sclk_map->inst->block_num != io3_map->inst->block_num))
        {
            result = CYHAL_QSPI_RSLT_ERR_PIN;
        }
    }
    /* Pins io4, io5, io6 and io7 are only available in CY_SMIF_DATA_SEL0 mode */
    if ((CY_RSLT_SUCCESS == result) && (data_select == CY_SMIF_DATA_SEL0) && (max_width >= CYHAL_QSPI_CFG_BUS_OCTAL))
    {
        if ((sclk_map->inst->block_num != io4_map->inst->block_num) ||
            (sclk_map->inst->block_num != io5_map->inst->block_num) ||
            (sclk_map->inst->block_num != io6_map->inst->block_num) ||
            (sclk_map->inst->block_num != io7_map->inst->block_num))
        {
            result = CYHAL_QSPI_RSLT_ERR_PIN;
        }
    }

#ifndef __MBED__
    // Mbed calls qspi_init multiple times without calling qspi_free to update the QSPI frequency/mode.
    // As a result, we won't worry about resource reservation if running through mbed.
    if (CY_RSLT_SUCCESS == result)
    {
        result = make_pin_reservations(obj);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_reserve(sclk_map->inst);
    }
#endif

    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource = *sclk_map->inst;
        obj->base = smif_base_addresses[obj->resource.block_num];

        result = cyhal_connect_pin(sclk_map);
        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(ssel_map);
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(io0_map);
        }
        if ((CY_RSLT_SUCCESS == result) && (max_width >= CYHAL_QSPI_CFG_BUS_DUAL))
        {
            result = cyhal_connect_pin(io1_map);
        }
    }
    if ((CY_RSLT_SUCCESS == result) && ((data_select == CY_SMIF_DATA_SEL0) || (data_select == CY_SMIF_DATA_SEL2)) &&
        (max_width >= CYHAL_QSPI_CFG_BUS_QUAD))
    {
        result = cyhal_connect_pin(io2_map);
        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(io3_map);
        }
    }
    if ((CY_RSLT_SUCCESS == result) && (data_select == CY_SMIF_DATA_SEL0) && (max_width >= CYHAL_QSPI_CFG_BUS_OCTAL))
    {
        result = cyhal_connect_pin(io4_map);
        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(io5_map);
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(io6_map);
        }
        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(io7_map);
        }
    }

    /* cyhal_qspi_set_frequency should be called here.
       Changing clock frequency is not supported on this device.
    */
   (void)hz;

    if (CY_RSLT_SUCCESS == result)
    {
        result = (cy_rslt_t) Cy_SMIF_Init(obj->base, &default_qspi_config, TIMEOUT_10_MS, &obj->context);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        Cy_SMIF_SetDataSelect(obj->base, slave_select, data_select);
        Cy_SMIF_Enable(obj->base, &obj->context);

        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = CYHAL_QSPI_EVENT_NONE;
        cyhal_qspi_config_structs[obj->resource.block_num] = obj;

        cy_stc_sysint_t irqCfg = { CYHAL_QSPI_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, cyhal_qspi_irq_handler);
        NVIC_EnableIRQ(CYHAL_QSPI_IRQ_N[obj->resource.block_num]);
    }

    if (CY_RSLT_SUCCESS != result)
    {
        cyhal_qspi_free(obj);
    }

    return result;
}

void cyhal_qspi_free(cyhal_qspi_t *obj)
{
    if (CYHAL_RSC_INVALID != obj->resource.type)
    {
        IRQn_Type irqn = CYHAL_QSPI_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        if (obj->base != NULL)
        {
            Cy_SMIF_DeInit(obj->base);
            obj->base = NULL;
        }
        cyhal_hwmgr_free(&(obj->resource));
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    cyhal_utils_release_if_used(&(obj->pin_sclk));
    cyhal_utils_release_if_used(&(obj->pin_ssel));
    for (uint8_t i = 0; (i < MAX_DATA_PINS); i++)
    {
        cyhal_utils_release_if_used(&(obj->pin_ios[i]));
    }
}

cy_rslt_t cyhal_qspi_set_frequency(cyhal_qspi_t *obj, uint32_t hz)
{
    /* Changing clock frequency is not supported on this device. */
    (void) obj;
    (void) hz;
    return CYHAL_QSPI_RSLT_ERR_FREQUENCY;
}

/* no restriction on the value of length. This function splits the read into multiple chunked transfers. */
cy_rslt_t cyhal_qspi_read(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, void *data, size_t *length)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint32_t chunk = 0;
    size_t read_bytes = *length;
    uint32_t addr = command->address.value;

    /* SMIF can read only up to 65536 bytes in one go. Split the larger read into multiple chunks */
    while (read_bytes > 0)
    {
        chunk = (read_bytes > SMIF_MAX_RX_COUNT) ? (SMIF_MAX_RX_COUNT) : read_bytes;

        /* Address is passed outside command during a read of more than 65536 bytes. Since that
         * read has to be split into multiple chunks, the address value needs to be incremented
         * after every chunk has been read. This requires either modifying the address stored in
         * 'command' passed to read() which is not possible since command is a const pointer or
         * to create a copy of the command object. Instead of copying the object, the address is
         * passed separately.
         */
        status = qspi_command_transfer(obj, command, addr, false);

        if (CY_RSLT_SUCCESS == status)
        {
            if (command->dummy_count > 0u)
            {
                status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
            }

            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_ReceiveDataBlocking(obj->base, (uint8_t *)data, chunk,
                    get_cyhal_bus_width(command->data.bus_width), &obj->context);
                if (CY_RSLT_SUCCESS != status)
                {
                    break;
                }
            }
        }
        read_bytes -= chunk;
        addr += chunk;
        data = (uint8_t *)data + chunk;
    }

    return status;
}

cy_rslt_t cyhal_qspi_read_async(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, void *data, size_t *length)
{
    cy_rslt_t status = qspi_command_transfer(obj, command, command->address.value, false);

    if (CY_RSLT_SUCCESS == status)
    {
        if (command->dummy_count > 0u)
        {
            status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
        }

        if (CY_RSLT_SUCCESS == status)
        {
            status = (cy_rslt_t)Cy_SMIF_ReceiveData(obj->base, (uint8_t *)data, (uint32_t)*length,
                get_cyhal_bus_width(command->data.bus_width), cyhal_qspi_cb_wrapper, &obj->context);
        }
    }
    return status;
}

/* length can be up to 65536. */
cy_rslt_t cyhal_qspi_write(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length)
{
    cy_rslt_t status = qspi_command_transfer(obj, command, command->address.value, false);

    if (CY_RSLT_SUCCESS == status)
    {
        if (command->dummy_count > 0u)
        {
            status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
        }

        if ((CY_SMIF_SUCCESS == status) && (*length > 0))
        {
            status = (cy_rslt_t)Cy_SMIF_TransmitDataBlocking(obj->base, (uint8_t *)data, *length,
                get_cyhal_bus_width(command->data.bus_width), &obj->context);
        }
    }

    return status;
}

/* length can be up to 65536. */
cy_rslt_t cyhal_qspi_write_async(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length)
{
    cy_rslt_t status = qspi_command_transfer(obj, command, command->address.value, false);

    if (CY_RSLT_SUCCESS == status)
    {
        if (command->dummy_count > 0u)
        {
            status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
        }

        if ((CY_SMIF_SUCCESS == status) && (*length > 0))
        {
            status = (cy_rslt_t)Cy_SMIF_TransmitData(obj->base, (uint8_t *)data, *length,
                get_cyhal_bus_width(command->data.bus_width), cyhal_qspi_cb_wrapper, &obj->context);
        }
    }
    return status;
}

cy_rslt_t cyhal_qspi_transfer(
    cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data,
    size_t rx_size)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0))
    {
        /* only command, no rx or tx */
        status = qspi_command_transfer(obj, command, command->address.value, true);
    }
    else
    {
        if (tx_data != NULL && tx_size)
        {
            status = cyhal_qspi_write(obj, command, tx_data, &tx_size);
        }

        if (status == CY_RSLT_SUCCESS)
        {
            if (rx_data != NULL && rx_size)
            {
                status = cyhal_qspi_read(obj, command, rx_data, &rx_size);
            }
        }
    }
    return status;
}

void cyhal_qspi_register_callback(cyhal_qspi_t *obj, cyhal_qspi_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);

    obj->irq_cause = CYHAL_QSPI_EVENT_NONE;
}

void cyhal_qspi_enable_event(cyhal_qspi_t *obj, cyhal_qspi_event_t event, uint8_t intrPriority, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }

    IRQn_Type irqn = CYHAL_QSPI_IRQ_N[obj->resource.block_num];
    NVIC_SetPriority(irqn, intrPriority);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_IP_MXSMIF */
