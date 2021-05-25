/***************************************************************************//**
* \file cyhal_qspi.c
*
* Description:
* Provides a high level interface for interacting with the Cypress QSPI (SMIF).
* This is a wrapper around the lower level PDL API.
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

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "cy_smif.h"
#include "cyhal_utils.h"
#include "cyhal_qspi.h"
#include "cyhal_hwmgr.h"
#include "cyhal_gpio.h"
#include "cyhal_interconnect.h"
#include "cyhal_system_impl.h"
#include "cyhal_syspm.h"
#include "cyhal_clock.h"

#ifdef CY_IP_MXSMIF

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
*       Internal
*******************************************************************************/
/* in microseconds, timeout for all blocking functions */
#define _CYHAL_QSPI_TIMEOUT_10_MS (10000UL)

#define _CYHAL_QSPI_MAX_DATA_PINS 8

#define _CYHAL_QSPI_MAX_RX_COUNT (65536UL)
#define _CYHAL_QSPI_DESELECT_DELAY (7UL)

#if (defined(SMIF_CHIP_TOP_DATA8_PRESENT) && (SMIF_CHIP_TOP_DATA8_PRESENT))   || \
    (defined(SMIF0_CHIP_TOP_DATA8_PRESENT) && (SMIF0_CHIP_TOP_DATA8_PRESENT))
    #define _CYHAL_QSPI_DATA8_PRESENT   1
#else
    #define _CYHAL_QSPI_DATA8_PRESENT   0
#endif
#if (SMIF_CHIP_TOP_SPI_SEL_NR > 1)
    #define _CYHAL_QSPI_SEL1   1
#else
    #define _CYHAL_QSPI_SEL1   0
#endif
#if (SMIF_CHIP_TOP_SPI_SEL_NR > 2)
    #define _CYHAL_QSPI_SEL2   1
#else
    #define _CYHAL_QSPI_SEL2   0
#endif
#if (SMIF_CHIP_TOP_SPI_SEL_NR > 3)
    #define _CYHAL_QSPI_SEL3   1
#else
    #define _CYHAL_QSPI_SEL3   0
#endif

static cyhal_qspi_t *_cyhal_qspi_config_structs[CY_IP_MXSMIF_INSTANCES];

/* Default QSPI configuration */
static const cy_stc_smif_config_t _cyhal_qspi_default_config =
{
    .mode = (uint32_t)CY_SMIF_NORMAL,
    .deselectDelay = _CYHAL_QSPI_DESELECT_DELAY,
    .rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
    .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};

/* List of available QSPI instances */
static SMIF_Type *const _cyhal_qspi_base_addresses[CY_IP_MXSMIF_INSTANCES] =
{
#ifdef SMIF0
    SMIF0,
#endif /* ifdef SMIF0 */
};

/* List of available QSPI interrupt sources */
static const IRQn_Type _CYHAL_QSPI_IRQ_N[CY_IP_MXSMIF_INSTANCES] =
{
#ifdef SMIF0
    smif_interrupt_IRQn,
#endif /* ifdef SMIF0 */
};

static inline uint8_t _cyhal_qspi_get_block_from_irqn(IRQn_Type irqn)
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

static cyhal_qspi_t *_cyhal_qspi_get_irq_obj(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block = _cyhal_qspi_get_block_from_irqn(irqn);
    return _cyhal_qspi_config_structs[block];
}

static void _cyhal_qspi_set_pins_frozen(cyhal_qspi_t* obj, bool freeze)
{
    GPIO_PRT_Type* port;
    uint8_t pin;
    cyhal_gpio_t gpio;
    for(size_t i = 0; i < sizeof(obj->pin_ios)/sizeof(obj->pin_ios[0]); ++i)
    {
        gpio = obj->pin_ios[i];
        if(NC != gpio)
        {
            port = CYHAL_GET_PORTADDR(gpio);
            pin = (uint8_t)CYHAL_GET_PIN(gpio);
            if(freeze)
            {
                obj->saved_io_hsiom[i] = Cy_GPIO_GetHSIOM(port, pin);
                Cy_GPIO_Clr(port, pin);
                Cy_GPIO_SetHSIOM(port, pin, HSIOM_SEL_GPIO);
            }
            else
            {
                Cy_GPIO_SetHSIOM(port, pin, obj->saved_io_hsiom[i]);
            }
        }
    }

    gpio = obj->pin_sclk;
    if(NC != gpio)
    {
        port = CYHAL_GET_PORTADDR(gpio);
        pin = (uint8_t)CYHAL_GET_PIN(gpio);
        if(freeze)
        {
            obj->saved_sclk_hsiom = Cy_GPIO_GetHSIOM(port, pin);
            Cy_GPIO_Clr(port, pin);
            Cy_GPIO_SetHSIOM(port, pin, HSIOM_SEL_GPIO);
        }
        else
        {
            Cy_GPIO_SetHSIOM(port, pin, obj->saved_sclk_hsiom);
        }
    }

    for(size_t i = 0; i < sizeof(obj->pin_ssel)/sizeof(obj->pin_ssel[0]); ++i)
    {
        gpio = obj->pin_ssel[i];
        if(NC != gpio)
        {
            port = CYHAL_GET_PORTADDR(gpio);
            pin = (uint8_t)CYHAL_GET_PIN(gpio);
            if(freeze)
            {
                obj->saved_ssel_hsiom[i] = Cy_GPIO_GetHSIOM(port, pin);
                Cy_GPIO_Set(port, pin); // The SMIF IP requires SSEL to be active low
                Cy_GPIO_SetHSIOM(port, pin, HSIOM_SEL_GPIO);
            }
            else
            {
                Cy_GPIO_SetHSIOM(port, pin, obj->saved_ssel_hsiom[i]);
            }
        }
    }
}

static bool _cyhal_qspi_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    CY_UNUSED_PARAMETER(state);
    cyhal_qspi_t *obj = (cyhal_qspi_t *)callback_arg;
    bool allow = true;
    switch(mode)
    {
        case CYHAL_SYSPM_CHECK_READY:
            allow &= obj->context.txBufferCounter == 0;
            allow &= obj->context.rxBufferCounter == 0;
            allow &= Cy_SMIF_GetRxFifoStatus(obj->base) == 0;
            allow &= Cy_SMIF_GetTxFifoStatus(obj->base) == 0;
            if (allow)
            {
                obj->pm_transition_pending = true;
            }
            break;
        case CYHAL_SYSPM_BEFORE_TRANSITION:
            _cyhal_qspi_set_pins_frozen(obj, true);
            break;
        case CYHAL_SYSPM_AFTER_TRANSITION:
            _cyhal_qspi_set_pins_frozen(obj, false);
            obj->pm_transition_pending = false;
            break;
        case CYHAL_SYSPM_CHECK_FAIL:
            obj->pm_transition_pending = false;
            break;
        default:
            break;
    }
    return allow;
}

/*******************************************************************************
*       Dispatcher Interrupt Service Routine
*******************************************************************************/

static void _cyhal_qspi_cb_wrapper(uint32_t event)
{
    cyhal_qspi_event_t hal_event = CYHAL_QSPI_EVENT_NONE;
    if (event == CY_SMIF_SEND_CMPLT)
        hal_event = CYHAL_QSPI_IRQ_TRANSMIT_DONE;
    else if (event == CY_SMIF_REC_CMPLT)
        hal_event = CYHAL_QSPI_IRQ_RECEIVE_DONE;

    cyhal_qspi_t *obj = _cyhal_qspi_get_irq_obj();
    if ((obj->irq_cause & (uint32_t)hal_event) > 0) // Make sure a user requested event is set before calling
    {
        cyhal_qspi_event_callback_t callback = (cyhal_qspi_event_callback_t) obj->callback_data.callback;
        callback(obj->callback_data.callback_arg, hal_event);
    }
}

/*******************************************************************************
*       (Internal) Interrupt Service Routines
*******************************************************************************/

/* Interrupt call, needed for SMIF Async operations */
static void _cyhal_qspi_irq_handler(void)
{
    cyhal_qspi_t *obj = _cyhal_qspi_get_irq_obj();
    Cy_SMIF_Interrupt(obj->base, &(obj->context));
}

/*******************************************************************************
*       (Internal) QSPI Pin Related Functions
*******************************************************************************/

/* Check if pin valid as resource and reserve it */
static cy_rslt_t _cyhal_qspi_check_pin_and_reserve(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *mapping)
{
    // Mbed calls qspi_init multiple times without calling qspi_free to update the QSPI frequency/mode.
    // As a result, we can't worry about resource reservation if running through mbed.
#ifndef __MBED__
    cy_rslt_t result = _cyhal_utils_reserve_and_connect(pin, mapping);
#else
    cy_rslt_t result = cyhal_connect_pin(mapping);
#endif

    return result;
}

/*******************************************************************************
*       (Internal) QSPI Config Related Functions
*******************************************************************************/

/* Translates HAL bus width to PDL bus width */
static cy_en_smif_txfr_width_t _cyhal_qspi_convert_bus_width(cyhal_qspi_bus_width_t bus_width)
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
static void _cyhal_qspi_convert_cmd_config(const cyhal_qspi_command_t *qspi_command,
    cy_stc_smif_mem_cmd_t *const cyhal_cmd_config)
{
    /* This function does not check 'disabled' of each sub-structure in qspi_command_t
    *  It is the responsibility of the caller to check it. */
    cyhal_cmd_config->command = qspi_command->instruction.value;
    cyhal_cmd_config->cmdWidth = _cyhal_qspi_convert_bus_width(qspi_command->instruction.bus_width);
    cyhal_cmd_config->addrWidth = _cyhal_qspi_convert_bus_width(qspi_command->address.bus_width);
    cyhal_cmd_config->mode = qspi_command->mode_bits.value;
    cyhal_cmd_config->modeWidth = _cyhal_qspi_convert_bus_width(qspi_command->mode_bits.bus_width);
    cyhal_cmd_config->dummyCycles = qspi_command->dummy_count;
    cyhal_cmd_config->dataWidth = _cyhal_qspi_convert_bus_width(qspi_command->data.bus_width);
}

static void _cyhal_qspi_uint32_to_byte_array(uint32_t value, uint8_t *byteArray, uint32_t startPos, uint32_t size)
{
    do
    {
        size--;
        byteArray[size + startPos] = (uint8_t)(value & 0xFF);
        value >>= 0x08;
    } while (size > 0);
}

/* cyhal_qspi_size_t to number bytes */
static inline uint32_t _cyhal_qspi_get_size(cyhal_qspi_size_t hal_size)
{
    return ((uint32_t)hal_size >> 3); /* convert bits to bytes */
}

/* Sends QSPI command with certain set of data */
/* Address passed through 'command' is not used, instead the value in 'addr' is used. */
static cy_rslt_t _cyhal_qspi_command_transfer(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command,
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

    _cyhal_qspi_convert_cmd_config(command, &cyhal_cmd_config);

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
            addr_size = _cyhal_qspi_get_size(command->address.size);
            _cyhal_qspi_uint32_to_byte_array(addr, cmd_param, start_pos, addr_size);
            start_pos += addr_size;
            bus_width = cyhal_cmd_config.addrWidth;
        }

        if (!command->mode_bits.disabled)
        {
            mode_bits_size = _cyhal_qspi_get_size(command->mode_bits.size);
            _cyhal_qspi_uint32_to_byte_array(cyhal_cmd_config.mode, cmd_param, start_pos, mode_bits_size);
            bus_width = cyhal_cmd_config.modeWidth;
        }

        uint32_t cmpltTxfr = ((endOfTransfer) ? 1UL : 0UL);
        result = (cy_rslt_t)Cy_SMIF_TransmitCommand(obj->base, (uint8_t)cyhal_cmd_config.command,
                                                         cyhal_cmd_config.cmdWidth, cmd_param, (addr_size + mode_bits_size),
                                                         bus_width, obj->slave_select, cmpltTxfr, &obj->context);
    }
    return result;
}

static inline cy_en_smif_slave_select_t _cyhal_qspi_slave_idx_to_smif_ss(uint8_t ssel_idx)
{
    return (cy_en_smif_slave_select_t)(1 << ssel_idx);
}

/* Checks, that user provided all needed pins and returns max bus width */
static cy_rslt_t _cyhal_qspi_check_user_pins(cyhal_qspi_t *obj, cyhal_qspi_bus_width_t *max_width)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    if (NC == obj->pin_sclk)
    {
        *max_width = CYHAL_QSPI_CFG_BUS_SINGLE;
        result = CYHAL_QSPI_RSLT_ERR_PIN;
    }
    else
    {
#if _CYHAL_QSPI_DATA8_PRESENT
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

        for (uint8_t i = 1; i <= _CYHAL_QSPI_MAX_DATA_PINS; i++)
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
static const cyhal_resource_pin_mapping_t *_cyhal_qspi_get_slaveselect(cyhal_gpio_t ssel, uint8_t *ssel_idx)
{
#if _CYHAL_QSPI_SEL1 || _CYHAL_QSPI_SEL2 || _CYHAL_QSPI_SEL3
    bool pin_found = false;
#endif
    const cyhal_resource_pin_mapping_t *pin_mapping = _CYHAL_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select0);
    if (NULL != pin_mapping)
    {
#if _CYHAL_QSPI_SEL1 || _CYHAL_QSPI_SEL2 || _CYHAL_QSPI_SEL3
        pin_found = true;
#endif
        /* Provided by user ssel is related to Slave # 0 */
        *ssel_idx = 0;
    }
#if _CYHAL_QSPI_SEL1
    if (!pin_found)
    {
        pin_mapping = _CYHAL_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select1);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            /* Provided by user ssel is related to Slave # 1 */
            *ssel_idx = 1;
        }
    }
#endif
#if _CYHAL_QSPI_SEL2
    if (!pin_found)
    {
        pin_mapping = _CYHAL_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select2);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            /* Provided by user ssel is related to Slave # 2 */
            *ssel_idx = 2;
        }
    }
#endif
#if _CYHAL_QSPI_SEL3
    if (!pin_found)
    {
        pin_mapping = _CYHAL_UTILS_GET_RESOURCE(ssel, cyhal_pin_map_smif_spi_select3);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            /* Provided by user ssel is related to Slave # 3 */
            *ssel_idx = 3;
        }
    }
#endif

    return pin_mapping;
}

/* Based on data pins chosen, determines SMIF data select parameter */
static const cyhal_resource_pin_mapping_t *_cyhal_qspi_get_dataselect(cyhal_gpio_t io0, cy_en_smif_data_select_t *data_select, uint8_t *offset)
{
    bool pin_found = false;
    const cyhal_resource_pin_mapping_t *pin_mapping = _CYHAL_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data0);
    if (NULL != pin_mapping)
    {
        pin_found = true;
        *data_select = CY_SMIF_DATA_SEL0;
        *offset = 0;
    }
    if (!pin_found)
    {
        pin_mapping = _CYHAL_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data2);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL1;
            *offset = 2;
        }
    }
#if _CYHAL_QSPI_DATA8_PRESENT
    if (!pin_found)
    {
        pin_mapping = _CYHAL_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data4);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL2;
            *offset = 4;
        }
    }
    if (!pin_found)
    {
        pin_mapping = _CYHAL_UTILS_GET_RESOURCE(io0, cyhal_pin_map_smif_spi_data6);
        if (NULL != pin_mapping)
        {
            pin_found = true;
            *data_select = CY_SMIF_DATA_SEL3;
            *offset = 6;
        }
    }
#endif
    return pin_mapping;
}

/*******************************************************************************
*       Functions
*******************************************************************************/

static cy_rslt_t _cyhal_qspi_slave_select_check_reserve(cyhal_qspi_t *obj, cyhal_gpio_t ssel, uint8_t *found_idx,
                    bool *config_needed)
{
    CY_ASSERT(NULL != obj);
    *config_needed = true;
    const cyhal_resource_pin_mapping_t *ssel_map = _cyhal_qspi_get_slaveselect(ssel, found_idx);
    const cyhal_resource_pin_mapping_t *sclk_map = _CYHAL_UTILS_GET_RESOURCE(obj->pin_sclk, cyhal_pin_map_smif_spi_clk);
    /* Found ssel map is not null and belong to same block as sclk */
    if ((NULL == ssel_map) || (NULL == sclk_map) || !_cyhal_utils_resources_equal(sclk_map->inst, ssel_map->inst))
    {
        return CYHAL_QSPI_RSLT_ERR_CANNOT_CONFIG_SSEL;
    }
    /* Specified SSEL is already configured */
    if (NC != obj->pin_ssel[*found_idx])
    {
        *config_needed = false;
        return CY_RSLT_SUCCESS;
    }
    /* No problems with pins, proceeding to reservation */
    cy_rslt_t result = _cyhal_qspi_check_pin_and_reserve(ssel, ssel_map);
    if (CY_RSLT_SUCCESS == result)
    {
        obj->pin_ssel[*found_idx] = ssel;
    }
    return result;
}

cy_rslt_t cyhal_qspi_init(
    cyhal_qspi_t *obj, cyhal_gpio_t io0, cyhal_gpio_t io1, cyhal_gpio_t io2, cyhal_gpio_t io3,
    cyhal_gpio_t io4, cyhal_gpio_t io5, cyhal_gpio_t io6, cyhal_gpio_t io7, cyhal_gpio_t sclk,
    cyhal_gpio_t ssel, uint32_t hz, uint8_t mode)
{
    /* mode (CPOL and CPHA) are not supported in CAT1 (PSoC 6) */
    (void)mode;

    CY_ASSERT(NULL != obj);

    /* Explicitly marked not allocated resources as invalid to prevent freeing them. */
    memset(obj, 0, sizeof(cyhal_qspi_t));
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->is_clock_owned = false;

    cyhal_qspi_bus_width_t max_width;

    obj->pin_sclk = sclk;
    obj->pin_ios[0] = io0;
    obj->pin_ios[1] = io1;
    obj->pin_ios[2] = io2;
    obj->pin_ios[3] = io3;
    obj->pin_ios[4] = io4;
    obj->pin_ios[5] = io5;
    obj->pin_ios[6] = io6;
    obj->pin_ios[7] = io7;

    cy_rslt_t result = _cyhal_qspi_check_user_pins(obj, &max_width);

    for (uint8_t i = 0; i < SMIF_CHIP_TOP_SPI_SEL_NR; ++i)
    {
        obj->pin_ssel[i] = NC;
    }

    uint8_t pin_offset = 0;
    const cyhal_resource_pin_mapping_t *sclk_map = _CYHAL_UTILS_GET_RESOURCE(sclk, cyhal_pin_map_smif_spi_clk);
    const cyhal_resource_pin_mapping_t *io_maps[8] = { NULL };
    const size_t data_pin_map_sizes[_CYHAL_QSPI_MAX_DATA_PINS - 1] = // Must compute sizes here since we can't get them from the map pointers
    {
        sizeof(cyhal_pin_map_smif_spi_data1) / sizeof(cyhal_resource_pin_mapping_t),
        sizeof(cyhal_pin_map_smif_spi_data2) / sizeof(cyhal_resource_pin_mapping_t),
        sizeof(cyhal_pin_map_smif_spi_data3) / sizeof(cyhal_resource_pin_mapping_t),
#if _CYHAL_QSPI_DATA8_PRESENT
        sizeof(cyhal_pin_map_smif_spi_data4) / sizeof(cyhal_resource_pin_mapping_t),
        sizeof(cyhal_pin_map_smif_spi_data5) / sizeof(cyhal_resource_pin_mapping_t),
        sizeof(cyhal_pin_map_smif_spi_data6) / sizeof(cyhal_resource_pin_mapping_t),
        sizeof(cyhal_pin_map_smif_spi_data7) / sizeof(cyhal_resource_pin_mapping_t),
#endif
    };
    const cyhal_resource_pin_mapping_t *data_pin_maps[_CYHAL_QSPI_MAX_DATA_PINS - 1] = // Not used to get the map for data pin 0
    {
        cyhal_pin_map_smif_spi_data1,
        cyhal_pin_map_smif_spi_data2,
        cyhal_pin_map_smif_spi_data3,
#if _CYHAL_QSPI_DATA8_PRESENT
        cyhal_pin_map_smif_spi_data4,
        cyhal_pin_map_smif_spi_data5,
        cyhal_pin_map_smif_spi_data6,
        cyhal_pin_map_smif_spi_data7,
#endif
    };

    if (CY_RSLT_SUCCESS == result)
    {
        io_maps[0] = _cyhal_qspi_get_dataselect(obj->pin_ios[0], &(obj->data_select), &pin_offset);
        if (NULL == sclk_map || NULL == io_maps[0] ||
            !_cyhal_utils_resources_equal(sclk_map->inst, io_maps[0]->inst))
        {
            result = CYHAL_QSPI_RSLT_ERR_PIN;
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        /* Check that all data pins are valid and belong to same instance */
        for (uint8_t i = 1; i < max_width; i++)
        {
            io_maps[i] = _cyhal_utils_get_resource(obj->pin_ios[i], data_pin_maps[i - 1 + pin_offset],
                data_pin_map_sizes[i - 1 + pin_offset], NULL);
            if (NULL == io_maps[i] || !_cyhal_utils_resources_equal(sclk_map->inst, io_maps[i]->inst))
            {
                result = CYHAL_QSPI_RSLT_ERR_PIN;
                break;
            }
        }
    }

    uint8_t found_ssel_idx = 0;
    bool datasel_cfg_needed = true;
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_qspi_slave_select_check_reserve(obj, ssel, &found_ssel_idx, &datasel_cfg_needed);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        /* reserve the SCLK pin */
        result = _cyhal_qspi_check_pin_and_reserve(obj->pin_sclk, sclk_map);

        /* reserve the io pins */
        for (uint8_t i = 0; (i < _CYHAL_QSPI_MAX_DATA_PINS) && (result == CY_RSLT_SUCCESS); i++)
        {
            if (NC != obj->pin_ios[i])
            {
                result = _cyhal_qspi_check_pin_and_reserve(obj->pin_ios[i], io_maps[i]);
            }
        }
    }

#ifndef __MBED__
    // Mbed calls qspi_init multiple times without calling qspi_free to update the QSPI frequency/mode.
    // As a result, we won't worry about resource reservation if running through mbed.
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_reserve(sclk_map->inst);
    }
#endif

    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource = *sclk_map->inst;
        obj->base = _cyhal_qspi_base_addresses[obj->resource.block_num];
    }

    if (CY_RSLT_SUCCESS == result)
    {
        // The hardware is generally going to be hardwired to an hfclk, which has very limited divider options. In the event
        // that we're hooked up a PERI divider, we don't have any particular expectations about its width - so just ask for 8-bit
        result = _cyhal_utils_allocate_clock(&(obj->clock), &(obj->resource), CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT, true);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->is_clock_owned = true;
        result = cyhal_qspi_set_frequency(obj, hz);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        result = cyhal_clock_set_enabled(&(obj->clock), true, true);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = (cy_rslt_t) Cy_SMIF_Init(obj->base, &_cyhal_qspi_default_config, _CYHAL_QSPI_TIMEOUT_10_MS, &obj->context);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        obj->slave_select = _cyhal_qspi_slave_idx_to_smif_ss(found_ssel_idx);
        if (datasel_cfg_needed)
        {
            Cy_SMIF_SetDataSelect(obj->base, obj->slave_select, obj->data_select);
        }

        Cy_SMIF_Enable(obj->base, &obj->context);

        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = CYHAL_QSPI_EVENT_NONE;
        _cyhal_qspi_config_structs[obj->resource.block_num] = obj;
        obj->pm_transition_pending = false;
        obj->pm_callback.callback = &_cyhal_qspi_pm_callback;
        obj->pm_callback.states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE);
        obj->pm_callback.args = obj;
        obj->pm_callback.next = NULL;
        obj->pm_callback.ignore_modes = (cyhal_syspm_callback_mode_t)0;
        _cyhal_syspm_register_peripheral_callback(&(obj->pm_callback));

        cy_stc_sysint_t irqCfg = { _CYHAL_QSPI_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_qspi_irq_handler);
        NVIC_EnableIRQ(_CYHAL_QSPI_IRQ_N[obj->resource.block_num]);
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
        IRQn_Type irqn = _CYHAL_QSPI_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);
        _cyhal_syspm_unregister_peripheral_callback(&(obj->pm_callback));
        if (obj->base != NULL)
        {
            Cy_SMIF_DeInit(obj->base);
            obj->base = NULL;
        }
        cyhal_hwmgr_free(&(obj->resource));
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    _cyhal_utils_release_if_used(&(obj->pin_sclk));
    for (uint8_t i = 0; i < SMIF_CHIP_TOP_SPI_SEL_NR; i++)
    {
        _cyhal_utils_release_if_used(&(obj->pin_ssel[i]));
    }
    for (uint8_t i = 0; (i < _CYHAL_QSPI_MAX_DATA_PINS); i++)
    {
        _cyhal_utils_release_if_used(&(obj->pin_ios[i]));
    }
    if (obj->is_clock_owned)
    {
        cyhal_clock_free(&(obj->clock));
        obj->is_clock_owned = false;
    }
}

cy_rslt_t cyhal_qspi_set_frequency(cyhal_qspi_t *obj, uint32_t hz)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(hz != 0);
    CY_ASSERT(obj->is_clock_owned);

    const cyhal_clock_tolerance_t tolerance = { CYHAL_TOLERANCE_PERCENT, 10 };
    return _cyhal_utils_set_clock_frequency2(&(obj->clock), hz, &tolerance);
}

uint32_t cyhal_qspi_get_frequency(cyhal_qspi_t *obj)
{
    CY_ASSERT(NULL != obj);
    return cyhal_clock_get_frequency(&(obj->clock));
}

cy_rslt_t cyhal_qspi_slave_select_config(cyhal_qspi_t *obj, cyhal_gpio_t ssel)
{
    CY_ASSERT(NULL != obj);
    bool datasel_cfg_needed = false;
    uint8_t ssel_idx = 0;
    cy_rslt_t result = _cyhal_qspi_slave_select_check_reserve(obj, ssel, &ssel_idx, &datasel_cfg_needed);
    if ((CY_RSLT_SUCCESS == result) && datasel_cfg_needed)
    {
        Cy_SMIF_SetDataSelect(obj->base, _cyhal_qspi_slave_idx_to_smif_ss(ssel_idx), obj->data_select);
    }
    return result;
}

cy_rslt_t cyhal_qspi_select_active_ssel(cyhal_qspi_t *obj, cyhal_gpio_t ssel)
{
    CY_ASSERT(NULL != obj);
    for (uint8_t ssel_idx = 0; ssel_idx < SMIF_CHIP_TOP_SPI_SEL_NR; ssel_idx++)
    {
        if (ssel == obj->pin_ssel[ssel_idx])
        {
            obj->slave_select = _cyhal_qspi_slave_idx_to_smif_ss(ssel_idx);
            return CY_RSLT_SUCCESS;
        }
    }
    return CYHAL_QSPI_RSLT_ERR_CANNOT_SWITCH_SSEL;
}

static cy_rslt_t _cyhal_qspi_wait_for_cmd_fifo(cyhal_qspi_t *obj)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint32_t timeout = _CYHAL_QSPI_TIMEOUT_10_MS;
    while ((Cy_SMIF_GetCmdFifoStatus(obj->base) == CY_SMIF_TX_CMD_FIFO_STATUS_RANGE) &&
                    (CY_RSLT_SUCCESS == status))
    {
        /* Waiting for 1 us per iteration */
        Cy_SysLib_DelayUs(1);
        --timeout;
        status = (0u == timeout)? CYHAL_QSPI_RSLT_ERR_TIMEOUT: CY_RSLT_SUCCESS;
    }
    return status;
}

/* no restriction on the value of length. This function splits the read into multiple chunked transfers. */
cy_rslt_t cyhal_qspi_read(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, void *data, size_t *length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint32_t chunk = 0;
    size_t read_bytes = *length;
    uint32_t addr = command->address.value;

    /* SMIF can read only up to 65536 bytes in one go. Split the larger read into multiple chunks */
    while (read_bytes > 0)
    {
        chunk = (read_bytes > _CYHAL_QSPI_MAX_RX_COUNT) ? (_CYHAL_QSPI_MAX_RX_COUNT) : read_bytes;

        /* Address is passed outside command during a read of more than 65536 bytes. Since that
         * read has to be split into multiple chunks, the address value needs to be incremented
         * after every chunk has been read. This requires either modifying the address stored in
         * 'command' passed to read() which is not possible since command is a const pointer or
         * to create a copy of the command object. Instead of copying the object, the address is
         * passed separately.
         */
        status = _cyhal_qspi_command_transfer(obj, command, addr, false);

        if (CY_RSLT_SUCCESS == status)
        {
            if (command->dummy_count > 0u)
            {
                status = _cyhal_qspi_wait_for_cmd_fifo(obj);
                if (CY_RSLT_SUCCESS == status)
                {
                    status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
                }
            }

            if (CY_RSLT_SUCCESS == status)
            {
                status = _cyhal_qspi_wait_for_cmd_fifo(obj);
                if (CY_RSLT_SUCCESS == status)
                {
                    status = (cy_rslt_t)Cy_SMIF_ReceiveDataBlocking(obj->base, (uint8_t *)data, chunk,
                        _cyhal_qspi_convert_bus_width(command->data.bus_width), &obj->context);
                    if (CY_RSLT_SUCCESS != status)
                    {
                        break;
                    }
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
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    cy_rslt_t status = _cyhal_qspi_command_transfer(obj, command, command->address.value, false);

    if (CY_RSLT_SUCCESS == status)
    {
        if (command->dummy_count > 0u)
        {
            status = _cyhal_qspi_wait_for_cmd_fifo(obj);
            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
            }
        }

        if (CY_RSLT_SUCCESS == status)
        {
            status = _cyhal_qspi_wait_for_cmd_fifo(obj);
            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_ReceiveData(obj->base, (uint8_t *)data, (uint32_t)*length,
                    _cyhal_qspi_convert_bus_width(command->data.bus_width), _cyhal_qspi_cb_wrapper, &obj->context);
            }
        }
    }
    return status;
}

/* length can be up to 65536. */
cy_rslt_t cyhal_qspi_write(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    cy_rslt_t status = _cyhal_qspi_command_transfer(obj, command, command->address.value, false);

    if (CY_RSLT_SUCCESS == status)
    {
        if (command->dummy_count > 0u)
        {
            status = _cyhal_qspi_wait_for_cmd_fifo(obj);
            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
            }
        }

        if ((CY_SMIF_SUCCESS == status) && (*length > 0))
        {
            status = _cyhal_qspi_wait_for_cmd_fifo(obj);
            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_TransmitDataBlocking(obj->base, (uint8_t *)data, *length,
                    _cyhal_qspi_convert_bus_width(command->data.bus_width), &obj->context);
            }
        }
    }

    return status;
}

/* length can be up to 65536. */
cy_rslt_t cyhal_qspi_write_async(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    cy_rslt_t status = _cyhal_qspi_command_transfer(obj, command, command->address.value, false);

    if (CY_RSLT_SUCCESS == status)
    {
        if (command->dummy_count > 0u)
        {
            status = _cyhal_qspi_wait_for_cmd_fifo(obj);
            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
            }
        }

        if ((CY_SMIF_SUCCESS == status) && (*length > 0))
        {
            status = _cyhal_qspi_wait_for_cmd_fifo(obj);
            if (CY_RSLT_SUCCESS == status)
            {
                status = (cy_rslt_t)Cy_SMIF_TransmitData(obj->base, (uint8_t *)data, *length,
                    _cyhal_qspi_convert_bus_width(command->data.bus_width), _cyhal_qspi_cb_wrapper, &obj->context);
            }
        }
    }
    return status;
}

cy_rslt_t cyhal_qspi_transfer(
    cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data,
    size_t rx_size)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0))
    {
        /* only command, no rx or tx */
        status = _cyhal_qspi_command_transfer(obj, command, command->address.value, true);
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

void cyhal_qspi_enable_event(cyhal_qspi_t *obj, cyhal_qspi_event_t event, uint8_t intr_priority, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }

    IRQn_Type irqn = _CYHAL_QSPI_IRQ_N[obj->resource.block_num];
    NVIC_SetPriority(irqn, intr_priority);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_IP_MXSMIF */
