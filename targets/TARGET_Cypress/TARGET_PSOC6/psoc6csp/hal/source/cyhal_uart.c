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
#include <string.h>
#include "cyhal_uart.h"
#include "cyhal_scb_common.h"
#include "cyhal_gpio.h"
#include "cyhal_system_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_syspm.h"
#include "cyhal_clock.h"

#if defined (CY_IP_MXSCB) || defined(CY_IP_M0S8SCB)

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_UART_OVERSAMPLE                 12UL
#define _CYHAL_UART_OVERSAMPLE_MIN             8UL
#define _CYHAL_UART_OVERSAMPLE_MAX             16UL

/* Default UART configuration */
static const cy_stc_scb_uart_config_t _cyhal_uart_default_config = {
    .uartMode                   = CY_SCB_UART_STANDARD,
    .enableMutliProcessorMode   = false,
    .smartCardRetryOnNack       = false,
    .irdaInvertRx               = false,
    .irdaEnableLowPowerReceiver = false,

    .oversample                 = _CYHAL_UART_OVERSAMPLE,

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
#if defined(COMPONENT_PSOC6HAL)
    .rtsRxFifoLevel             = 20UL,
#else
    .rtsRxFifoLevel             = 3UL,
#endif
    .rtsPolarity                = CY_SCB_UART_ACTIVE_LOW,

    .rxFifoTriggerLevel         = 0UL,  /* Level triggers when at least one element is in FIFO */
    .rxFifoIntEnableMask        = 0x0UL,

    .txFifoTriggerLevel         = (CY_SCB_FIFO_SIZE/2 - 1), /* Level triggers when half-fifo is half empty */
    .txFifoIntEnableMask        = 0x0UL
};

static void _cyhal_uart_irq_handler(void)
{
    cyhal_uart_t *obj = (cyhal_uart_t*) _cyhal_scb_get_irq_obj();
    Cy_SCB_UART_Interrupt(obj->base, &(obj->context));
}

static void _cyhal_uart_cb_wrapper(uint32_t event)
{
    static const uint32_t status_map[] =            //Note: HAL defines in PDL order for mapping
    {
        (uint32_t)CYHAL_UART_IRQ_NONE,                 // Default no IRQ
        (uint32_t)CYHAL_UART_IRQ_TX_TRANSMIT_IN_FIFO,  // CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT
        (uint32_t)CYHAL_UART_IRQ_TX_DONE,              // CY_SCB_UART_TRANSMIT_DONE_EVENT
        (uint32_t)CYHAL_UART_IRQ_RX_DONE,              // CY_SCB_UART_RECEIVE_DONE_EVENT
        (uint32_t)CYHAL_UART_IRQ_RX_FULL,              // CY_SCB_UART_RB_FULL_EVENT
        (uint32_t)CYHAL_UART_IRQ_RX_ERROR,             // CY_SCB_UART_RECEIVE_ERR_EVENT
        (uint32_t)CYHAL_UART_IRQ_TX_ERROR,             // CY_SCB_UART_TRANSMIT_ERR_EVENT
        (uint32_t)CYHAL_UART_IRQ_RX_NOT_EMPTY,         // CY_SCB_UART_RECEIVE_NOT_EMTPY
        (uint32_t)CYHAL_UART_IRQ_TX_EMPTY,             // CY_SCB_UART_TRANSMIT_EMTPY
    };
    uint32_t hal_event = _cyhal_utils_convert_flags(status_map, sizeof(status_map) / sizeof(uint32_t), event);

    cyhal_uart_t *obj = (cyhal_uart_t*) _cyhal_scb_get_irq_obj();
    cyhal_uart_event_t anded_events = (cyhal_uart_event_t)(obj->irq_cause & hal_event);
    if (anded_events)
    {
        cyhal_uart_event_callback_t callback = (cyhal_uart_event_callback_t) obj->callback_data.callback;
        callback(obj->callback_data.callback_arg, anded_events);
    }
}
static bool _cyhal_uart_pm_callback_instance(void *obj_ptr, cyhal_syspm_callback_state_t state, cy_en_syspm_callback_mode_t pdl_mode)
{
    CY_UNUSED_PARAMETER(state);
    cyhal_uart_t *obj = (cyhal_uart_t*)obj_ptr;
    bool allow = false;

    // The output pins need to be set to high before going to deepsleep.
    // Otherwise the UART on the other side would see incoming data as '0'.
    GPIO_PRT_Type *txport = obj->pin_tx != NC ? CYHAL_GET_PORTADDR(obj->pin_tx) : NULL;
    GPIO_PRT_Type *rtsport = obj->pin_rts != NC ? CYHAL_GET_PORTADDR(obj->pin_rts) : NULL;
    uint8_t txpin = (uint8_t)CYHAL_GET_PIN(obj->pin_tx);
    uint8_t rtspin = (uint8_t)CYHAL_GET_PIN(obj->pin_rts);

    switch (pdl_mode)
    {
        case CY_SYSPM_CHECK_READY:
            /* Check whether the High-level API is not busy executing the transmit
            * or receive operation.
            */
            if ((0UL == (CY_SCB_UART_TRANSMIT_ACTIVE & Cy_SCB_UART_GetTransmitStatus(obj->base, &(obj->context)))) &&
                (0UL == (CY_SCB_UART_RECEIVE_ACTIVE  & Cy_SCB_UART_GetReceiveStatus (obj->base, &(obj->context)))))
            {
                /* If all data elements are transmitted from the TX FIFO and
                * shifter and the RX FIFO is empty: the UART is ready to enter
                * Deep Sleep mode.
                */
                if (Cy_SCB_UART_IsTxComplete(obj->base))
                {
                    if (0UL == Cy_SCB_UART_GetNumInRxFifo(obj->base))
                    {
                        /* Disable the UART. The transmitter stops driving the
                        * lines and the receiver stops receiving data until
                        * the UART is enabled.
                        * This happens when the device failed to enter Deep
                        * Sleep or it is awaken from Deep Sleep mode.
                        */

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

                        Cy_SCB_UART_Disable(obj->base, &(obj->context));
                        allow = true;

                    }
                }
            }
            break;

        case CY_SYSPM_CHECK_FAIL:
        case CY_SYSPM_AFTER_TRANSITION:
            allow = true;
            Cy_SCB_UART_Enable(obj->base);
            if (NULL != txport)
            {
                Cy_GPIO_SetHSIOM(txport, txpin, obj->saved_tx_hsiom);
            }
            if (NULL != rtsport)
            {
                Cy_GPIO_SetHSIOM(rtsport, rtspin, obj->saved_rts_hsiom);
            }
            break;

        case CY_SYSPM_BEFORE_TRANSITION:
            allow = true;
            break;

        default:
            CY_ASSERT(false);
            break;
    }
    return allow;
}
static cy_en_scb_uart_parity_t _cyhal_uart_convert_parity(cyhal_uart_parity_t parity)
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

static cy_en_scb_uart_stop_bits_t _cyhal_uart_convert_stopbits(uint8_t stopbits)
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

static uint32_t _cyhal_uart_actual_baud(uint32_t divider, uint32_t oversample)
{
    #if defined(COMPONENT_PSOC6HAL)
    return Cy_SysClk_ClkPeriGetFrequency() / (divider * oversample);
    #else /* COMPONENT_PSOC4HAL */
    return Cy_SysClk_ClkSysGetFrequency() / (divider * oversample);
    #endif
}

static uint32_t _cyhal_uart_baud_perdif(uint32_t desired_baud, uint32_t actual_baud)
{
    return (actual_baud > desired_baud)
        ? ((actual_baud * 100) - (desired_baud * 100)) / desired_baud
        : ((desired_baud * 100) - (actual_baud * 100)) / desired_baud;
}

static uint8_t _cyhal_uart_best_oversample(uint32_t baudrate)
{
    uint8_t best_oversample = _CYHAL_UART_OVERSAMPLE_MIN;
    uint8_t best_difference = 0xFF;

    for (uint8_t i = _CYHAL_UART_OVERSAMPLE_MIN; i < _CYHAL_UART_OVERSAMPLE_MAX + 1; i++)
    {
        uint32_t divider = _cyhal_utils_divider_value(baudrate * i, 0);
        uint8_t difference = (uint8_t)_cyhal_uart_baud_perdif(baudrate, _cyhal_uart_actual_baud(divider, i));

        if (difference < best_difference)
        {
            best_difference = difference;
            best_oversample = i;
        }
    }

    return best_oversample;
}



cy_rslt_t cyhal_uart_init(cyhal_uart_t *obj, cyhal_gpio_t tx, cyhal_gpio_t rx, const cyhal_clock_t *clk, const cyhal_uart_cfg_t *cfg)
{
    CY_ASSERT(NULL != obj);
    memset(obj, 0, sizeof(cyhal_uart_t));

    // Explicitly marked not allocated resources as invalid to prevent freeing them.
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->is_user_clock = true;
    obj->pin_rx = CYHAL_NC_PIN_VALUE;
    obj->pin_tx = CYHAL_NC_PIN_VALUE;
    obj->pin_cts = CYHAL_NC_PIN_VALUE;
    obj->pin_rts = CYHAL_NC_PIN_VALUE;

    cy_rslt_t result = CY_RSLT_SUCCESS;

    // Reserve the UART
    const cyhal_resource_pin_mapping_t *tx_map = _CYHAL_SCB_FIND_MAP(tx, cyhal_pin_map_scb_uart_tx);
    const cyhal_resource_pin_mapping_t *rx_map = _CYHAL_SCB_FIND_MAP(rx, cyhal_pin_map_scb_uart_rx);
    if (NULL == tx_map || NULL == rx_map || !_cyhal_utils_resources_equal(tx_map->inst, rx_map->inst))
    {
        return CYHAL_UART_RSLT_ERR_INVALID_PIN;
    }

    cyhal_resource_inst_t rsc = *rx_map->inst;
    if (CY_RSLT_SUCCESS != (result = cyhal_hwmgr_reserve(&rsc)))
        return result;

    obj->resource = rsc;
    obj->base = _CYHAL_SCB_BASE_ADDRESSES[obj->resource.block_num];

    // reserve the TX pin
    result = _cyhal_utils_reserve_and_connect(tx, tx_map);
    if (result == CY_RSLT_SUCCESS)
    {
        obj->pin_tx = tx;

        //reseve the RX pin
        result = _cyhal_utils_reserve_and_connect(rx, rx_map);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_rx = rx;
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        if (clk == NULL)
        {
            obj->is_user_clock = false;
            result = cyhal_clock_allocate(&(obj->clock), CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT);
        }
        else
        {
            obj->is_user_clock = true;
            obj->clock = *clk;
            _cyhal_utils_update_clock_format(&(obj->clock));
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        result = (cy_rslt_t)Cy_SysClk_PeriphAssignDivider(
            _cyhal_scb_get_clock_index(obj->resource.block_num), (cy_en_divider_types_t)obj->clock.block, obj->clock.channel);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        obj->config = _cyhal_uart_default_config;

        if (cfg == NULL)
        {
            Cy_SCB_UART_Init(obj->base, &(obj->config), &(obj->context));
        }
        else
        {
            obj->config.dataWidth = cfg->data_bits;
            obj->config.stopBits = _cyhal_uart_convert_stopbits((uint8_t)cfg->stop_bits);
            obj->config.parity = _cyhal_uart_convert_parity(cfg->parity);
            Cy_SCB_UART_Init(obj->base, &(obj->config), &(obj->context));
            if (cfg->rx_buffer != NULL)
            {
                Cy_SCB_UART_StartRingBuffer(obj->base, cfg->rx_buffer, cfg->rx_buffer_size, &(obj->context));
            }
        }

        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = CYHAL_UART_IRQ_NONE;

        cy_stc_sysint_t irqCfg = { _CYHAL_SCB_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_uart_irq_handler);
        NVIC_EnableIRQ(_CYHAL_SCB_IRQ_N[obj->resource.block_num]);

        _cyhal_scb_update_instance_data(obj->resource.block_num, (void*)obj, &_cyhal_uart_pm_callback_instance);

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
        IRQn_Type irqn = _CYHAL_SCB_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        cyhal_hwmgr_free(&(obj->resource));

        _cyhal_scb_update_instance_data(obj->resource.block_num, NULL, NULL);
    }

    _cyhal_utils_release_if_used(&(obj->pin_rx));
    _cyhal_utils_release_if_used(&(obj->pin_tx));
    _cyhal_utils_release_if_used(&(obj->pin_rts));
    _cyhal_utils_release_if_used(&(obj->pin_cts));

    if (!(obj->is_user_clock))
    {
        cyhal_clock_free(&(obj->clock));
    }
}

cy_rslt_t cyhal_uart_set_baud(cyhal_uart_t *obj, uint32_t baudrate, uint32_t *actualbaud)
{
    cy_rslt_t status;
    uint8_t oversample_value;
    uint32_t calculated_baud;
    uint32_t divider;

    Cy_SCB_UART_Disable(obj->base, NULL);
    status = cyhal_clock_set_enabled(&(obj->clock), false, false);
    if(status != CY_RSLT_SUCCESS)
    {
        Cy_SCB_UART_Enable(obj->base);
        return status;
    }

    oversample_value = _cyhal_uart_best_oversample(baudrate);
    obj->config.oversample = oversample_value;

    divider = _cyhal_utils_divider_value(baudrate * oversample_value, 0);

    /* Set baud rate */
    status = cyhal_clock_set_divider(&(obj->clock), divider);
    if(status != CY_RSLT_SUCCESS)
    {
        cyhal_clock_set_enabled(&(obj->clock), true, false);
        Cy_SCB_UART_Enable(obj->base);
        return status;
    }

    calculated_baud = _cyhal_uart_actual_baud(divider, oversample_value);

    if (actualbaud != NULL)
        *actualbaud = calculated_baud;
    uint32_t baud_difference = _cyhal_uart_baud_perdif(baudrate, calculated_baud);
    if (baud_difference > CYHAL_UART_MAX_BAUD_PERCENT_DIFFERENCE)
        status = CY_RSLT_WRN_CSP_UART_BAUD_TOLERANCE;

    status = cyhal_clock_set_enabled(&(obj->clock), true, false);

    /* Configure the UART interface */
    #if (CY_IP_MXSCB_VERSION >= 2) /* Versions 2 and later */
    SCB_CTRL(obj->base) = _BOOL2FLD(SCB_CTRL_ADDR_ACCEPT, obj->config.acceptAddrInFifo)     |
                _BOOL2FLD(SCB_CTRL_MEM_WIDTH, (obj->config.dataWidth <= CY_SCB_BYTE_WIDTH)
                    ? CY_SCB_CTRL_MEM_WIDTH_BYTE : CY_SCB_CTRL_MEM_WIDTH_HALFWORD)          |
                _VAL2FLD(SCB_CTRL_OVS, oversample_value - 1)                                |
                _VAL2FLD(SCB_CTRL_MODE, CY_SCB_CTRL_MODE_UART);
    #else /* Older versions of the block */
    SCB_CTRL(obj->base) = _BOOL2FLD(SCB_CTRL_ADDR_ACCEPT, obj->config.acceptAddrInFifo)     |
                _BOOL2FLD(SCB_CTRL_BYTE_MODE, (obj->config.dataWidth <= CY_SCB_BYTE_WIDTH)) |
                _VAL2FLD(SCB_CTRL_OVS, oversample_value - 1)                                |
                _VAL2FLD(SCB_CTRL_MODE, CY_SCB_CTRL_MODE_UART);
    #endif

    Cy_SCB_UART_Enable(obj->base);
    return status;
}

cy_rslt_t cyhal_uart_configure(cyhal_uart_t *obj, const cyhal_uart_cfg_t *cfg)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != cfg);
    Cy_SCB_UART_Disable(obj->base, NULL);
    obj->config.dataWidth = cfg->data_bits;
    obj->config.stopBits = _cyhal_uart_convert_stopbits((uint8_t)cfg->stop_bits);
    obj->config.parity = _cyhal_uart_convert_parity(cfg->parity);
    // Do not pass obj->context here because Cy_SCB_UART_Init will destroy it
    Cy_SCB_UART_Init(obj->base, &(obj->config), NULL);
    Cy_SCB_UART_Enable(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_getc(cyhal_uart_t *obj, uint8_t *value, uint32_t timeout)
{
    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

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
    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

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

    if (cts != obj->pin_cts)
    {
        if (NC == cts)
        {
            if (obj->pin_cts != NC)
            {
                _cyhal_utils_disconnect_and_free(obj->pin_cts);
                Cy_SCB_UART_DisableCts(obj->base);
            }
        }
        else
        {
            const cyhal_resource_pin_mapping_t *cts_map = _CYHAL_UTILS_GET_RESOURCE(cts, cyhal_pin_map_scb_uart_cts);
            if (!_cyhal_utils_resources_equal(&(obj->resource), cts_map->inst))
            {
                return CYHAL_UART_RSLT_ERR_INVALID_PIN;
            }

            result = _cyhal_utils_reserve_and_connect(cts, cts_map);
            if (CY_RSLT_SUCCESS == result)
            {
                Cy_SCB_UART_EnableCts(obj->base);
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
            if (obj->pin_rts != NC)
            {
                _cyhal_utils_disconnect_and_free(obj->pin_rts);
            }
        }
        else
        {
            const cyhal_resource_pin_mapping_t *rts_map = _CYHAL_UTILS_GET_RESOURCE(rts, cyhal_pin_map_scb_uart_rts);
            if (!_cyhal_utils_resources_equal(&(obj->resource), rts_map->inst))
            {
                return CYHAL_UART_RSLT_ERR_INVALID_PIN;
            }

            result = _cyhal_utils_reserve_and_connect(rts, rts_map);
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
    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    *tx_length = Cy_SCB_UART_PutArray(obj->base, tx, *tx_length);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_read(cyhal_uart_t *obj, void *rx, size_t *rx_length)
{
    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    *rx_length = Cy_SCB_UART_GetArray(obj->base, rx, *rx_length);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_uart_write_async(cyhal_uart_t *obj, void *tx, size_t length)
{
    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    return Cy_SCB_UART_Transmit(obj->base, tx, length, &(obj->context));
}

cy_rslt_t cyhal_uart_read_async(cyhal_uart_t *obj, void *rx, size_t length)
{
    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    return Cy_SCB_UART_Receive(obj->base, rx, length, &(obj->context));
}

bool cyhal_uart_is_tx_active(cyhal_uart_t *obj)
{
    return (0UL != (obj->context.txStatus & CY_SCB_UART_TRANSMIT_ACTIVE)) || !Cy_SCB_IsTxComplete(obj->base);
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

void cyhal_uart_register_callback(cyhal_uart_t *obj, cyhal_uart_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
    Cy_SCB_UART_RegisterCallback(obj->base, _cyhal_uart_cb_wrapper, &(obj->context));

    obj->irq_cause = CYHAL_UART_IRQ_NONE;
}

void cyhal_uart_enable_event(cyhal_uart_t *obj, cyhal_uart_event_t event, uint8_t intr_priority, bool enable)
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

    NVIC_SetPriority(_CYHAL_SCB_IRQ_N[obj->resource.block_num], intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
