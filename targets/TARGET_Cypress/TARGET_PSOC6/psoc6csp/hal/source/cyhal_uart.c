/*******************************************************************************
* File Name: cyhal_uart.c
*
* Description:
* Provides a high level interface for interacting with the Cypress UART. This is
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
#include "cyhal_uart.h"
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

#define UART_OVERSAMPLE                 12UL
#define UART_OVERSAMPLE_MIN             8UL
#define UART_OVERSAMPLE_MAX             16UL

/* Default UART configuration */
static const cy_stc_scb_uart_config_t default_uart_config = {
    .uartMode                   = CY_SCB_UART_STANDARD,
    .enableMutliProcessorMode   = false,
    .smartCardRetryOnNack       = false,
    .irdaInvertRx               = false,
    .irdaEnableLowPowerReceiver = false,

    .oversample                 = UART_OVERSAMPLE,

    .enableMsbFirst             = false,
    .dataWidth                  = 8UL,
    .parity                     = CY_SCB_UART_PARITY_NONE,
    .stopBits                   = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter          = false,
    .breakWidth                 = 11UL,
    .dropOnFrameError           = false,
    .dropOnParityError          = false,

    .receiverAddress            = 0x0UL,
    .receiverAddressMask        = 0x0UL,
    .acceptAddrInFifo           = false,

    .enableCts                  = false,
    .ctsPolarity                = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel             = 20UL,
    .rtsPolarity                = CY_SCB_UART_ACTIVE_LOW,

    .rxFifoTriggerLevel         = 0UL,  /* Level triggers when at least one element is in FIFO */
    .rxFifoIntEnableMask        = 0x0UL,

    .txFifoTriggerLevel         = 63UL, /* Level triggers when half-fifo is half empty */
    .txFifoIntEnableMask        = 0x0UL
};

static cyhal_uart_event_t cyhal_convert_interrupt_cause(uint32_t pdl_cause);

static void cyhal_uart_irq_handler(void)
{
    cyhal_uart_t *obj = (cyhal_uart_t*) cyhal_scb_get_irq_obj();
    Cy_SCB_UART_Interrupt(obj->base, &(obj->context));
}

static void cyhal_uart_cb_wrapper(uint32_t event)
{
    cyhal_uart_t *obj = (cyhal_uart_t*) cyhal_scb_get_irq_obj();
    cyhal_uart_event_t anded_events = (cyhal_uart_event_t)(obj->irq_cause & (uint32_t)cyhal_convert_interrupt_cause(event));
    if (anded_events)
    {
        cyhal_uart_event_callback_t callback = (cyhal_uart_event_callback_t) obj->callback_data.callback;
        callback(obj->callback_data.callback_arg, anded_events);
    }
}

static cy_en_syspm_status_t cyhal_uart_pm_callback(cy_stc_syspm_callback_params_t *params, cy_en_syspm_callback_mode_t mode)
{
    cyhal_uart_t *obj = params->context;
    cy_stc_syspm_callback_params_t pdl_params = { .base = obj->base, .context = &(obj->context) };
    cy_en_syspm_status_t rslt = Cy_SCB_UART_DeepSleepCallback(&pdl_params, mode);
    GPIO_PRT_Type *txport = obj->pin_tx != NC ? CYHAL_GET_PORTADDR(obj->pin_tx) : NULL,
                  *rtsport = obj->pin_rts != NC ? CYHAL_GET_PORTADDR(obj->pin_rts) : NULL;
    uint8_t txpin = (uint8_t)CYHAL_GET_PIN(obj->pin_tx), rtspin = (uint8_t)CYHAL_GET_PIN(obj->pin_rts);
    switch (mode)
    {
        case CY_SYSPM_CHECK_READY:
            if (rslt == CY_SYSPM_SUCCESS)
            {
                if (NULL != txport)
                {
                    obj->saved_tx_hsiom = Cy_GPIO_GetHSIOM(txport, txpin);
                    Cy_GPIO_Set(txport, txpin);
                    Cy_GPIO_SetHSIOM(txport, txpin, HSIOM_SEL_GPIO);
                }
                if (NULL != rtsport)
                {
                    obj->saved_rts_hsiom = Cy_GPIO_GetHSIOM(rtsport, rtspin);
                    Cy_GPIO_Set(rtsport, rtspin);
                    Cy_GPIO_SetHSIOM(rtsport, rtspin, HSIOM_SEL_GPIO);
                }
            }
            break;

        case CY_SYSPM_CHECK_FAIL: // fallthrough
        case CY_SYSPM_AFTER_TRANSITION:
            if (NULL != txport)
            {
                Cy_GPIO_SetHSIOM(txport, txpin, obj->saved_tx_hsiom);
            }
            if (NULL != rtsport)
            {
                Cy_GPIO_SetHSIOM(rtsport, rtspin, obj->saved_rts_hsiom);
            }
            break;

        default:
            break;
    }
    return rslt;
}

static cy_en_scb_uart_parity_t convert_parity(cyhal_uart_parity_t parity)
{
    switch (parity)
    {
        case CYHAL_UART_PARITY_NONE:
            return CY_SCB_UART_PARITY_NONE;
        case CYHAL_UART_PARITY_EVEN:
            return CY_SCB_UART_PARITY_EVEN;
        case CYHAL_UART_PARITY_ODD:
            return CY_SCB_UART_PARITY_ODD;
        default:
            return CY_SCB_UART_PARITY_NONE;
    }
}

static cy_en_scb_uart_stop_bits_t convert_stopbits(uint8_t stopbits)
{
    switch (stopbits)
    {
        case 1:
            return CY_SCB_UART_STOP_BITS_1;
        case 2:
            return CY_SCB_UART_STOP_BITS_2;
        case 3:
            return CY_SCB_UART_STOP_BITS_3;
        case 4:
            return CY_SCB_UART_STOP_BITS_4;
        default:
            CY_ASSERT(false);
            return CY_SCB_UART_STOP_BITS_1;
    }
}

cy_rslt_t cyhal_uart_init(cyhal_uart_t *obj, cyhal_gpio_t tx, cyhal_gpio_t rx, const cyhal_clock_divider_t *clk, const cyhal_uart_cfg_t *cfg)
{
    CY_ASSERT(NULL != obj);

    // Explicitly marked not allocated resources as invalid to prevent freeing them.
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->is_user_clock = true;
    obj->pin_rx = CYHAL_NC_PIN_VALUE;
    obj->pin_tx = CYHAL_NC_PIN_VALUE;
    obj->pin_cts = CYHAL_NC_PIN_VALUE;
    obj->pin_rts = CYHAL_NC_PIN_VALUE;

    cy_rslt_t result = CY_RSLT_SUCCESS;
    cyhal_resource_inst_t pin_rsc;

    // Reserve the UART
    const cyhal_resource_pin_mapping_t *tx_map = CY_UTILS_GET_RESOURCE(tx, cyhal_pin_map_scb_uart_tx);
    const cyhal_resource_pin_mapping_t *rx_map = CY_UTILS_GET_RESOURCE(rx, cyhal_pin_map_scb_uart_rx);
    if (NULL == tx_map || NULL == rx_map || tx_map->inst->block_num != rx_map->inst->block_num)
    {
        return CYHAL_UART_RSLT_ERR_INVALID_PIN;
    }

    cyhal_resource_inst_t rsc = *rx_map->inst;
    if (CY_RSLT_SUCCESS != (result = cyhal_hwmgr_reserve(&rsc)))
        return result;

    obj->resource = rsc;

    // reserve the TX pin
    pin_rsc = cyhal_utils_get_gpio_resource(tx);
    result = cyhal_hwmgr_reserve(&pin_rsc);
    if (result == CY_RSLT_SUCCESS)
    {
        obj->pin_tx = tx;
    }

    //reseve the RX pin
    if (result == CY_RSLT_SUCCESS)
    {
        pin_rsc = cyhal_utils_get_gpio_resource(rx);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_rx = rx;
        }
    }

    obj->base = CYHAL_SCB_BASE_ADDRESSES[obj->resource.block_num];

    if (result == CY_RSLT_SUCCESS)
    {
        if (clk == NULL)
        {
            obj->is_user_clock = false;
            result = cyhal_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_16_BIT, true);
        }
        else
        {
            obj->is_user_clock = true;
            obj->clock = *clk;
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        result = (cy_rslt_t)Cy_SysClk_PeriphAssignDivider(
            (en_clk_dst_t)((uint8_t)PCLK_SCB0_CLOCK + obj->resource.block_num), obj->clock.div_type, obj->clock.div_num);
    }
    if (result == CY_RSLT_SUCCESS)
    {
        result = cyhal_connect_pin(rx_map);
    }
    if (result == CY_RSLT_SUCCESS)
    {
        result = cyhal_connect_pin(tx_map);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        obj->config = default_uart_config;

        if (cfg == NULL)
        {
            Cy_SCB_UART_Init(obj->base, &(obj->config), &(obj->context));
        }
        else
        {
            obj->config.dataWidth = cfg->data_bits;
            obj->config.stopBits = convert_stopbits(cfg->stop_bits);
            obj->config.parity = convert_parity(cfg->parity);
            Cy_SCB_UART_Init(obj->base, &(obj->config), &(obj->context));
            if (cfg->rx_buffer != NULL)
            {
                Cy_SCB_UART_StartRingBuffer(obj->base, cfg->rx_buffer, cfg->rx_buffer_size, &(obj->context));
            }
        }

        obj->pm_params.base = obj->base;
        obj->pm_params.context = obj;
        obj->pm_callback.callback = &cyhal_uart_pm_callback;
        obj->pm_callback.type = CY_SYSPM_DEEPSLEEP;
        obj->pm_callback.skipMode = 0;
        obj->pm_callback.callbackParams = &(obj->pm_params);
        obj->pm_callback.prevItm = NULL;
        obj->pm_callback.nextItm = NULL;
        if (!Cy_SysPm_RegisterCallback(&(obj->pm_callback)))
            result = CYHAL_UART_RSLT_ERR_PM_CALLBACK;

        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = CYHAL_UART_IRQ_NONE;
        cyhal_scb_config_structs[obj->resource.block_num] = obj;

        cy_stc_sysint_t irqCfg = { CYHAL_SCB_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, cyhal_uart_irq_handler);
        NVIC_EnableIRQ(CYHAL_SCB_IRQ_N[obj->resource.block_num]);

        if (obj->is_user_clock)
        {
            Cy_SCB_UART_Enable(obj->base);
        }
        else
        {
            result = cyhal_uart_set_baud(obj, CYHAL_UART_DEFAULT_BAUD, NULL);
        }
    }

    if (result != CY_RSLT_SUCCESS)
    {
        cyhal_uart_free(obj);
    }
    return result;
}

void cyhal_uart_free(cyhal_uart_t *obj)
{
    CY_ASSERT(NULL != obj);

    if (obj->resource.type != CYHAL_RSC_INVALID)
    {
        IRQn_Type irqn = CYHAL_SCB_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        cyhal_hwmgr_free(&(obj->resource));
        Cy_SysPm_UnregisterCallback(&(obj->pm_callback));
    }

    cyhal_utils_release_if_used(&(obj->pin_rx));
    cyhal_utils_release_if_used(&(obj->pin_tx));
    cyhal_utils_release_if_used(&(obj->pin_rts));
    cyhal_utils_release_if_used(&(obj->pin_cts));

    if (!(obj->is_user_clock))
    {
        cyhal_hwmgr_free_clock(&(obj->clock));
    }
}

static uint32_t cyhal_uart_actual_baud(uint32_t divider, uint32_t oversample)
{
    return Cy_SysClk_ClkPeriGetFrequency() / ((divider +  1) * oversample);
}

static uint32_t cyhal_uart_baud_perdif(uint32_t desired_baud, uint32_t actual_baud)
{
    uint32_t perdif;
    if(actual_baud > desired_baud)
    {
        perdif = ((actual_baud * 100) - (desired_baud * 100)) / desired_baud;
    }
    else
    {
        perdif = ((desired_baud * 100) - (actual_baud * 100)) / desired_baud;
    }

    return perdif;
}

static uint8_t cyhal_uart_best_oversample(uint32_t baudrate)
{
    uint8_t differences[UART_OVERSAMPLE_MAX + 1];
    uint8_t index;
    uint32_t divider;

    for(index = UART_OVERSAMPLE_MIN; index < UART_OVERSAMPLE_MAX + 1; index++)
    {
        divider = cyhal_divider_value(baudrate * index, 0);
        differences[index] = cyhal_uart_baud_perdif(baudrate, cyhal_uart_actual_baud(divider, index));
    }

    uint8_t best_oversample = UART_OVERSAMPLE_MIN;
    uint8_t best_difference = differences[UART_OVERSAMPLE_MIN];

    for(index = UART_OVERSAMPLE_MIN; index < UART_OVERSAMPLE_MAX + 1; index++)
    {
        if(differences[index] < best_difference)
        {
            best_difference = differences[index];
            best_oversample = index;
        }
    }

    return best_oversample;
}

cy_rslt_t cyhal_uart_set_baud(cyhal_uart_t *obj, uint32_t baudrate, uint32_t *actualbaud)
{
    cy_rslt_t status;
    uint8_t oversample_value;
    uint32_t calculated_baud;
    uint32_t divider;

    Cy_SCB_UART_Disable(obj->base, NULL);
    Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);

    oversample_value = cyhal_uart_best_oversample(baudrate);
    obj->config.oversample = oversample_value;

    divider = cyhal_divider_value(baudrate * oversample_value, 0);

    /* Set baud rate */
    if ((obj->clock.div_type == CY_SYSCLK_DIV_16_5_BIT) || (obj->clock.div_type == CY_SYSCLK_DIV_24_5_BIT))
    {
        status = (cy_rslt_t) Cy_SysClk_PeriphSetFracDivider(obj->clock.div_type,
                                                obj->clock.div_num,
                                                divider, 0U);

    }
    else
    {
        status = (cy_rslt_t) Cy_SysClk_PeriphSetDivider(obj->clock.div_type,
                                            obj->clock.div_num,
                                            divider);
    }

    calculated_baud = cyhal_uart_actual_baud(divider, oversample_value);

    if(actualbaud != NULL) *actualbaud = calculated_baud;
    uint32_t baud_difference = cyhal_uart_baud_perdif(baudrate, calculated_baud);
    if(baud_difference > CYHAL_UART_MAX_BAUD_PERCENT_DIFFERENCE) status = CY_RSLT_WRN_CSP_UART_BAUD_TOLERANCE;

    Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num);

    /* Configure the UART interface */
    SCB_CTRL(obj->base) = _BOOL2FLD(SCB_CTRL_ADDR_ACCEPT, obj->config.acceptAddrInFifo)       |
                 _BOOL2FLD(SCB_CTRL_BYTE_MODE, (obj->config.dataWidth <= CY_SCB_BYTE_WIDTH))  |
                 _VAL2FLD(SCB_CTRL_OVS, oversample_value - 1)                                 |
                 _VAL2FLD(SCB_CTRL_MODE, CY_SCB_CTRL_MODE_UART);

    Cy_SCB_UART_Enable(obj->base);
    return status;
}

cy_rslt_t cyhal_uart_configure(cyhal_uart_t *obj, const cyhal_uart_cfg_t *cfg)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != cfg);
    Cy_SCB_UART_Disable(obj->base, NULL);
    obj->config.dataWidth = cfg->data_bits;
    obj->config.stopBits = convert_stopbits((uint8_t)cfg->stop_bits);
    obj->config.parity = convert_parity(cfg->parity);
    // Do not pass obj->context here because Cy_SCB_UART_Init will destroy it
    Cy_SCB_UART_Init(obj->base, &(obj->config), NULL);
    Cy_SCB_UART_Enable(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_getc(cyhal_uart_t *obj, uint8_t *value, uint32_t timeout)
{
    uint32_t read_value = Cy_SCB_UART_Get(obj->base);
    uint32_t timeoutTicks = timeout;
    while (read_value == CY_SCB_UART_RX_NO_DATA)
    {
        if(timeout != 0UL)
        {
            if(timeoutTicks > 0UL)
            {
                Cy_SysLib_Delay(1);
                timeoutTicks--;
            }
            else
            {
                return CY_RSLT_ERR_CSP_UART_GETC_TIMEOUT;
            }
        }
        read_value = Cy_SCB_UART_Get(obj->base);
    }
    *value = (uint8_t)read_value;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_putc(cyhal_uart_t *obj, uint32_t value)
{
    uint32_t count = 0;
    while (count == 0)
    {
        count = Cy_SCB_UART_Put(obj->base, value);
    }
    return CY_RSLT_SUCCESS;
}

uint32_t cyhal_uart_readable(cyhal_uart_t *obj)
{
    uint32_t number_available = Cy_SCB_UART_GetNumInRxFifo(obj->base);
    if(obj->context.rxRingBuf != NULL)
    {
        number_available += Cy_SCB_UART_GetNumInRingBuffer(obj->base, &(obj->context));
    }
    return number_available;
}

uint32_t cyhal_uart_writable(cyhal_uart_t *obj)
{
    return Cy_SCB_GetFifoSize(obj->base) - Cy_SCB_GetNumInTxFifo(obj->base);
}

cy_rslt_t cyhal_uart_clear(cyhal_uart_t *obj)
{
    Cy_SCB_UART_ClearRxFifo(obj->base);
    Cy_SCB_UART_ClearTxFifo(obj->base);

    if(obj->context.rxRingBuf != NULL)
    {
        Cy_SCB_UART_ClearRingBuffer(obj->base, &(obj->context));
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_set_flow_control(cyhal_uart_t *obj, cyhal_gpio_t cts, cyhal_gpio_t rts)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    cyhal_resource_inst_t pin_rsc;

    if (cts != obj->pin_cts)
    {
        if (NC == cts)
        {
            if(obj->pin_cts != NC)
            {
                result = cyhal_disconnect_pin(obj->pin_cts);

                if (CY_RSLT_SUCCESS == result)
                {
                    Cy_SCB_UART_DisableCts(obj->base);

                    pin_rsc = cyhal_utils_get_gpio_resource(obj->pin_cts);
                    cyhal_hwmgr_free(&pin_rsc);
                }
            }
        }
        else
        {
            const cyhal_resource_pin_mapping_t *cts_map = CY_UTILS_GET_RESOURCE(cts, cyhal_pin_map_scb_uart_cts);
            if (obj->resource.block_num != cts_map->inst->block_num ||
                obj->resource.channel_num != cts_map->inst->channel_num)
            {
                return CYHAL_UART_RSLT_ERR_INVALID_PIN;
            }

            pin_rsc = cyhal_utils_get_gpio_resource(cts);
            result = cyhal_hwmgr_reserve(&pin_rsc);

            if (CY_RSLT_SUCCESS == result)
            {
                Cy_SCB_UART_EnableCts(obj->base);
                result = cyhal_connect_pin(cts_map);
            }
        }

        if (result != CY_RSLT_SUCCESS)
        {
            return result;
        }
        obj->pin_cts = cts;
    }
    if (rts != obj->pin_rts)
    {
        if (NC == rts)
        {
            if(obj->pin_rts != NC)
            {
                result = cyhal_disconnect_pin(obj->pin_rts);

                if (CY_RSLT_SUCCESS == result)
                {
                    pin_rsc = cyhal_utils_get_gpio_resource(obj->pin_rts);
                    cyhal_hwmgr_free(&pin_rsc);
                }
            }
        }
        else
        {
            const cyhal_resource_pin_mapping_t *rts_map = CY_UTILS_GET_RESOURCE(rts, cyhal_pin_map_scb_uart_rts);
            if (obj->resource.block_num != rts_map->inst->block_num ||
                obj->resource.channel_num != rts_map->inst->channel_num)
            {
                return CYHAL_UART_RSLT_ERR_INVALID_PIN;
            }

            pin_rsc = cyhal_utils_get_gpio_resource(rts);
            result = cyhal_hwmgr_reserve(&pin_rsc);

            if (CY_RSLT_SUCCESS == result)
            {
                result = cyhal_connect_pin(rts_map);
            }
        }

        if (result != CY_RSLT_SUCCESS)
        {
            return result;
        }
        obj->pin_rts = rts;
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_write(cyhal_uart_t *obj, void *tx, size_t *tx_length)
{
    *tx_length = Cy_SCB_UART_PutArray(obj->base, tx, *tx_length);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_read(cyhal_uart_t *obj, void *rx, size_t *rx_length)
{
    *rx_length = Cy_SCB_UART_GetArray(obj->base, rx, *rx_length);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_write_async(cyhal_uart_t *obj, void *tx, size_t length)
{
    cy_en_scb_uart_status_t uart_status = Cy_SCB_UART_Transmit(obj->base, tx, length, &(obj->context));
    return uart_status == CY_SCB_UART_SUCCESS
        ? CY_RSLT_SUCCESS
        : CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_UART, 0);
}

cy_rslt_t cyhal_uart_read_async(cyhal_uart_t *obj, void *rx, size_t length)
{
    cy_en_scb_uart_status_t uart_status = Cy_SCB_UART_Receive(obj->base, rx, length, &(obj->context));
    return uart_status == CY_SCB_UART_SUCCESS
        ? CY_RSLT_SUCCESS
        : CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_UART, 0);
}

bool cyhal_uart_is_tx_active(cyhal_uart_t *obj)
{
    return (0UL != (obj->context.txStatus & CY_SCB_UART_TRANSMIT_ACTIVE));
}

bool cyhal_uart_is_rx_active(cyhal_uart_t *obj)
{
    return (0UL != (obj->context.rxStatus & CY_SCB_UART_RECEIVE_ACTIVE));
}

cy_rslt_t cyhal_uart_write_abort(cyhal_uart_t *obj)
{
    Cy_SCB_UART_AbortTransmit(obj->base, &(obj->context));
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_read_abort(cyhal_uart_t *obj)
{
    Cy_SCB_UART_AbortReceive(obj->base, &(obj->context));
    return CY_RSLT_SUCCESS;
}

static cyhal_uart_event_t cyhal_convert_interrupt_cause(uint32_t pdl_cause)
{
    cyhal_uart_event_t cause;
    switch (pdl_cause)
    {
        case CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT:
            cause = CYHAL_UART_IRQ_TX_TRANSMIT_IN_FIFO;
            break;
        case CY_SCB_UART_TRANSMIT_DONE_EVENT:
            cause = CYHAL_UART_IRQ_TX_DONE;
            break;
        case CY_SCB_UART_RECEIVE_DONE_EVENT:
            cause = CYHAL_UART_IRQ_RX_DONE;
            break;
        case CY_SCB_UART_RB_FULL_EVENT:
            cause = CYHAL_UART_IRQ_RX_FULL;
            break;
        case CY_SCB_UART_RECEIVE_ERR_EVENT:
            cause = CYHAL_UART_IRQ_RX_ERROR;
            break;
        case CY_SCB_UART_TRANSMIT_ERR_EVENT:
            cause = CYHAL_UART_IRQ_TX_ERROR;
            break;
        case CY_SCB_UART_RECEIVE_NOT_EMTPY:
            cause = CYHAL_UART_IRQ_RX_NOT_EMPTY;
            break;
        case CY_SCB_UART_TRANSMIT_EMTPY:
            cause = CYHAL_UART_IRQ_TX_EMPTY;
            break;
        default:
            cause = CYHAL_UART_IRQ_NONE;
            break;
    }
    return cause;
}

void cyhal_uart_register_callback(cyhal_uart_t *obj, cyhal_uart_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
    Cy_SCB_UART_RegisterCallback(obj->base, cyhal_uart_cb_wrapper, &(obj->context));

    obj->irq_cause = CYHAL_UART_IRQ_NONE;
}

void cyhal_uart_enable_event(cyhal_uart_t *obj, cyhal_uart_event_t event, uint8_t intrPriority, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
        if (event & CYHAL_UART_IRQ_RX_NOT_EMPTY)
        {
            Cy_SCB_ClearRxInterrupt(obj->base, CY_SCB_RX_INTR_NOT_EMPTY);
            Cy_SCB_SetRxInterruptMask(obj->base, Cy_SCB_GetRxInterruptMask(obj->base) | CY_SCB_RX_INTR_NOT_EMPTY);
        }
        if (event & CYHAL_UART_IRQ_TX_EMPTY)
        {
            Cy_SCB_ClearTxInterrupt(obj->base, CY_SCB_UART_TX_EMPTY);
            Cy_SCB_SetTxInterruptMask(obj->base, Cy_SCB_GetTxInterruptMask(obj->base) | CY_SCB_UART_TX_EMPTY);
        }
    }
    else
    {
        obj->irq_cause &= ~event;
        if (event & CYHAL_UART_IRQ_RX_NOT_EMPTY)
        {
            Cy_SCB_SetRxInterruptMask(obj->base, Cy_SCB_GetRxInterruptMask(obj->base) & ~CY_SCB_RX_INTR_NOT_EMPTY);
        }
        if (event & CYHAL_UART_IRQ_TX_EMPTY)
        {
            Cy_SCB_SetTxInterruptMask(obj->base, Cy_SCB_GetTxInterruptMask(obj->base) & ~CY_SCB_UART_TX_EMPTY);
        }
    }

    NVIC_SetPriority(CYHAL_SCB_IRQ_N[obj->resource.block_num], intrPriority);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
