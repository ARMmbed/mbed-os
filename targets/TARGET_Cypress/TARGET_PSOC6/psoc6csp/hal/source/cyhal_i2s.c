/*******************************************************************************
* File Name: cyhal_i2s.c
*
* Description:
* Provides a high level interface for interacting with the Cypress I2S. This is
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

#include <math.h>
#include <stdlib.h>
#include "cyhal_i2s.h"
#include "cy_i2s.h"
#include "cyhal_clock.h"
#include "cyhal_gpio.h"
#include "cyhal_hw_resources.h"
#include "cyhal_system_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_utils.h"
#include "cyhal_dma.h"
#include "cyhal_syspm.h"
#include "cy_device.h"

/**
* \addtogroup group_hal_impl_i2s I2S (Inter-IC Sound)
* \ingroup group_hal_impl
* \{
* The CAT1 (PSoC 6) I2S Supports the following values for word and channel lengths (with the
* constraint that word length must be less than or equal to channel length):
* - 8 bits
* - 16 bits
* - 18 bits
* - 20 bits
* - 24 bits
* - 32 bits
*
* The sclk signal is formed by integer division of the input clock source (either internally
* provided or from the mclk pin). The CAT1 I2S supports sclk divider values from 1 to 64.
* \} group_hal_impl_i2s
*/


#ifdef CY_IP_MXAUDIOSS

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_I2S_FIFO_DEPTH (256)
#define _CYHAL_I2S_DMA_BURST_SIZE (_CYHAL_I2S_FIFO_DEPTH / 2)

static I2S_Type *const _cyhal_i2s_base[] =
{
#if (CY_IP_MXAUDIOSS_INSTANCES == 1 && defined(AUDIOSS_I2S) && AUDIOSS_I2S)
    I2S,
#elif (CY_IP_MXAUDIOSS_INSTANCES >= 1 && defined(AUDIOSS0_I2S) && AUDIOSS0_I2S)
    I2S0,
#endif
#if (CY_IP_MXAUDIOSS_INSTANCES >= 2 && defined(AUDIOSS1_I2S) && AUDIOSS1_I2S)
    I2S1,
#endif

#if (CY_IP_MXS40AUDIOSS_INSTANCES > 2)
    #warning Unhandled audioss instance count
#endif
};

static cyhal_i2s_t* _cyhal_i2s_config_structs[CY_IP_MXAUDIOSS_INSTANCES];

static const IRQn_Type _cyhal_i2s_irq_n[] =
{
#if (CY_IP_MXAUDIOSS_INSTANCES == 1 && defined(AUDIOSS_I2S) && AUDIOSS_I2S) // Without index suffix
    audioss_interrupt_i2s_IRQn,
#elif (CY_IP_MXAUDIOSS_INSTANCES >= 1 && defined(AUDIOSS0_I2S) && AUDIOSS0_I2S)
    audioss_0_interrupt_i2s_IRQn,
#endif
#if (CY_IP_MXAUDIOSS_INSTANCES >= 2 && defined(AUDIOSS1_I2S) && AUDIOSS1_I2S)
    audioss_1_interrupt_i2s_IRQn,
#endif

#if (CY_IP_MXS40AUDIOSS_INSTANCES > 2)
    #warning Unhandled audioss instance count
#endif
};

static uint8_t _cyhal_i2s_get_block_from_irqn(IRQn_Type irqn) {
    switch (irqn)
    {
#if (CY_CPU_CORTEX_M4)
#if (CY_IP_MXAUDIOSS_INSTANCES == 1 && defined(AUDIOSS_I2S) && AUDIOSS_I2S) // Without index suffix
    case audioss_interrupt_i2s_IRQn:
        return 0;
#elif (CY_IP_MXAUDIOSS_INSTANCES >= 1 && defined(AUDIOSS0_I2S) && AUDIOSS0_I2S)
    case audioss_0_interrupt_i2s_IRQn:
        return 0;
#endif
#if (CY_IP_MXAUDIOSS_INSTANCES >= 2 && defined(AUDIOSS1_I2S) && AUDIOSS1_I2S)
    case audioss_1_interrupt_i2s_IRQn:
        return 1;
#endif
#if (CY_IP_MXS40AUDIOSS_INSTANCES > 2)
    #warning Unhandled audioss instance count
#endif
#endif /* (CY_CPU_CORTEX_M4) */
    default:
        CY_ASSERT(false); // Should never be called with a non-I2S IRQn
        return 0;
    }
}

static cyhal_i2s_event_t _cyhal_i2s_convert_interrupt_cause(uint32_t pdl_cause);
static uint32_t _cyhal_i2s_convert_event(cyhal_i2s_event_t event);
static cy_rslt_t _cyhal_i2s_convert_length(uint8_t user_length, cy_en_i2s_len_t *pdl_length);
static void _cyhal_i2s_irq_handler(void);
static void _cyhal_i2s_process_event(cyhal_i2s_t *obj, cyhal_i2s_event_t event);
static void _cyhal_i2s_update_enabled_events(cyhal_i2s_t* obj);
static void _cyhal_i2s_update_rx_trigger_level(cyhal_i2s_t* obj);
static cy_rslt_t _cyhal_i2s_dma_perform_rx(cyhal_i2s_t *obj);
static cy_rslt_t _cyhal_i2s_dma_perform_tx(cyhal_i2s_t *obj);
static void _cyhal_i2s_dma_handler_rx(void *callback_arg, cyhal_dma_event_t event);
static void _cyhal_i2s_dma_handler_tx(void *callback_arg, cyhal_dma_event_t event);
static uint8_t _cyhal_i2s_rounded_word_length(cyhal_i2s_t *obj);
static bool _cyhal_i2s_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg);
static cy_rslt_t _cyhal_i2s_populate_pdl_config(cyhal_i2s_t *obj, cy_stc_i2s_config_t* pdl_config, uint8_t sclk_div);
static cy_rslt_t _cyhal_i2s_compute_sclk_div(cyhal_i2s_t *obj, uint32_t sample_rate_hz, uint8_t *sclk_div);

static const cy_stc_i2s_config_t _cyhal_i2s_default_config = {
    /* tx_enabled and rx_enabled set per-instance */
    .txDmaTrigger = false,
    .rxDmaTrigger = false,
    /* clkDiv set per-instance */
    /* extclk set per-instance */
    /* txMasterMode set per-instance */
    .txAlignment = CY_I2S_I2S_MODE,
    .txWsPulseWidth = CY_I2S_WS_ONE_CHANNEL_LENGTH, /* only supported value for I2S mode */
    .txWatchdogEnable = false,
    .txWatchdogValue = 0u,
    .txSdoLatchingTime = false, /* to match the I2S standard */
    .txSckoInversion = false,
    .txSckiInversion = false,
    .txChannels = 2, /* Only supported value for I2S mode */
    /* txChannelLength set per-instance */
    /* txWordLength set per-instance */
    .txOverheadValue = CY_I2S_OVHDATA_ZERO,
    .txFifoTriggerLevel = _CYHAL_I2S_FIFO_DEPTH / 2 + 1, // Trigger at half empty
    /* rxMasterMode set per-instance */
    .rxAlignment = CY_I2S_I2S_MODE,        /**< RX data alignment, see: #cy_en_i2s_alignment_t. */
    .rxWsPulseWidth = CY_I2S_WS_ONE_CHANNEL_LENGTH, /* only supported value for I2S mode */
    .rxWatchdogEnable = false,
    .rxWatchdogValue = 0u,
    .rxSdiLatchingTime = false, /* to match the I2S standard */
    .rxSckoInversion = false,
    .rxSckiInversion = false,
    .rxChannels = 2, /* Only supported value for I2s mode */
    /* rxChannelLength set per-instance */
    /* rxWordLength set per-instance */
    .rxSignExtension = false, /* All MSB are filled by zeros */
    .rxFifoTriggerLevel = _CYHAL_I2S_FIFO_DEPTH / 2 - 1, // Trigger at half full
};

cy_rslt_t cyhal_i2s_init(cyhal_i2s_t *obj, const cyhal_i2s_pins_t* tx_pins, const cyhal_i2s_pins_t* rx_pins, cyhal_gpio_t mclk,
                         const cyhal_i2s_config_t* config, cyhal_clock_t* clk)
{
    CY_ASSERT(NULL != obj);
    memset(obj, 0, sizeof(cyhal_i2s_t));
    /* Explicitly marked not allocated resources as invalid to prevent freeing them. */
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->pin_tx_sck    = CYHAL_NC_PIN_VALUE;
    obj->pin_tx_ws     = CYHAL_NC_PIN_VALUE;
    obj->pin_tx_sdo    = CYHAL_NC_PIN_VALUE;
    obj->pin_rx_sck    = CYHAL_NC_PIN_VALUE;
    obj->pin_rx_ws     = CYHAL_NC_PIN_VALUE;
    obj->pin_rx_sdi    = CYHAL_NC_PIN_VALUE;
    obj->pin_mclk      = CYHAL_NC_PIN_VALUE;

    obj->is_tx_slave    = config->is_tx_slave;
    obj->is_rx_slave    = config->is_rx_slave;
    obj->mclk_hz        = config->mclk_hz;
    obj->channel_length = config->channel_length;
    obj->word_length    = config->word_length;
    obj->sample_rate_hz = config->sample_rate_hz;

    /*
     * We will update this to owned later if appropriate - for now set to false
     * so we don't try to free if we fail before allocating a clock
     */
    obj->is_clock_owned = false;

    obj->user_enabled_events = 0u;

    obj->callback_data.callback = NULL;
    obj->callback_data.callback_arg = NULL;
    obj->async_mode = CYHAL_ASYNC_SW;
    obj->async_tx_buff = NULL;
    obj->async_rx_buff = NULL;
    obj->tx_dma.resource.type = CYHAL_RSC_INVALID;
    obj->rx_dma.resource.type = CYHAL_RSC_INVALID;

    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* Determine which I2S instance to use */
    const cyhal_resource_pin_mapping_t *tx_sck_map = (NULL != tx_pins) ? _CYHAL_UTILS_GET_RESOURCE(tx_pins->sck, cyhal_pin_map_audioss_tx_sck) : NULL;
    const cyhal_resource_pin_mapping_t *tx_ws_map  = (NULL != tx_pins) ? _CYHAL_UTILS_GET_RESOURCE(tx_pins->ws, cyhal_pin_map_audioss_tx_ws) : NULL;
    const cyhal_resource_pin_mapping_t *tx_sdo_map = (NULL != tx_pins) ? _CYHAL_UTILS_GET_RESOURCE(tx_pins->data, cyhal_pin_map_audioss_tx_sdo) : NULL;

    const cyhal_resource_pin_mapping_t *rx_sck_map = (NULL != rx_pins) ? _CYHAL_UTILS_GET_RESOURCE(rx_pins->sck, cyhal_pin_map_audioss_rx_sck) : NULL;
    const cyhal_resource_pin_mapping_t *rx_ws_map  = (NULL != rx_pins) ? _CYHAL_UTILS_GET_RESOURCE(rx_pins->ws, cyhal_pin_map_audioss_rx_ws) : NULL;
    const cyhal_resource_pin_mapping_t *rx_sdi_map = (NULL != rx_pins) ? _CYHAL_UTILS_GET_RESOURCE(rx_pins->data, cyhal_pin_map_audioss_rx_sdi) : NULL;

    const cyhal_resource_pin_mapping_t *mclk_map = _CYHAL_UTILS_GET_RESOURCE(mclk, cyhal_pin_map_audioss_clk_i2s_if);

    if(NULL != tx_pins) /* It is valid to leave either tx or rx empty */
    {
        if(NULL != tx_sck_map && NULL != tx_ws_map && NULL != tx_sdo_map
            && _cyhal_utils_resources_equal_all(3, tx_sck_map->inst, tx_ws_map->inst, tx_sdo_map->inst))
        {
            obj->resource = *(tx_sck_map->inst);
        }
        else
        {
            result = CYHAL_I2S_RSLT_ERR_INVALID_PIN;
        }
    }

    if(CY_RSLT_SUCCESS == result && NULL != rx_pins)
    {
        if(NULL == rx_sck_map || NULL == rx_ws_map || NULL == rx_sdi_map ||
            (false == _cyhal_utils_resources_equal_all(3, rx_sck_map->inst, rx_ws_map->inst, rx_sdi_map->inst)))
        {
            result = CYHAL_I2S_RSLT_ERR_INVALID_PIN;
        }
        else
        {
            if((obj->resource.type != CYHAL_RSC_INVALID)
                && (false == _cyhal_utils_resources_equal(&(obj->resource), rx_sck_map->inst)))
            {
                /* TX pins and RX pins don't map to the same instance */
                result = CYHAL_I2S_RSLT_ERR_INVALID_PIN;
            }
            obj->resource = *(rx_sck_map->inst);
        }
    }

    if(CYHAL_RSC_INVALID == obj->resource.type) /* If this happens it means neither rx nor tx was specified */
    {
        result = CYHAL_I2S_RSLT_ERR_INVALID_PIN;
    }

    if(CY_RSLT_SUCCESS == result && CYHAL_NC_PIN_VALUE != mclk )
    {
        if(NULL == mclk_map || (false == _cyhal_utils_resources_equal(&(obj->resource), mclk_map->inst)))
        {
            result = CYHAL_I2S_RSLT_ERR_INVALID_PIN;
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_reserve(&(obj->resource));
        obj->base = _cyhal_i2s_base[obj->resource.block_num];
    }

    /* Reserve the pins */
    if(CY_RSLT_SUCCESS == result && NULL != tx_pins)
    {
        result = _cyhal_utils_reserve_and_connect(tx_pins->sck, tx_sck_map);
        if(CY_RSLT_SUCCESS == result)
        {
            obj->pin_tx_sck = tx_pins->sck;
            result = _cyhal_utils_reserve_and_connect(tx_pins->ws, tx_ws_map);
        }
        if(CY_RSLT_SUCCESS == result)
        {
            obj->pin_tx_ws = tx_pins->ws;
            result = _cyhal_utils_reserve_and_connect(tx_pins->data, tx_sdo_map);
        }
        if(CY_RSLT_SUCCESS == result)
        {
            obj->pin_tx_sdo = tx_pins->data;
        }

        // In slave mode, the clock and word select pins are inputs
        if(CY_RSLT_SUCCESS == result && obj->is_tx_slave)
        {
            result = cyhal_gpio_configure(obj->pin_tx_sck, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE);
            if(CY_RSLT_SUCCESS == result)
            {
                result = cyhal_gpio_configure(obj->pin_tx_ws, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE);
            }
        }
    }

    if(CY_RSLT_SUCCESS == result && NULL != rx_pins)
    {
        result = _cyhal_utils_reserve_and_connect(rx_pins->sck, rx_sck_map);
        if(CY_RSLT_SUCCESS == result)
        {
            obj->pin_rx_sck = rx_pins->sck;
            result = _cyhal_utils_reserve_and_connect(rx_pins->ws, rx_ws_map);
        }
        if(CY_RSLT_SUCCESS == result)
        {
            obj->pin_rx_ws = rx_pins->ws;
            result = _cyhal_utils_reserve_and_connect(rx_pins->data, rx_sdi_map);
        }
        if(CY_RSLT_SUCCESS == result)
        {
            obj->pin_rx_sdi = rx_pins->data;
        }

        // In slave mode, the clock and word select pins are inputs
        if(CY_RSLT_SUCCESS == result && obj->is_rx_slave)
        {
            result = cyhal_gpio_configure(obj->pin_rx_sck, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE);
            if(CY_RSLT_SUCCESS == result)
            {
                result = cyhal_gpio_configure(obj->pin_rx_ws, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE);
            }
        }
    }

    if(CY_RSLT_SUCCESS == result && CYHAL_NC_PIN_VALUE != mclk)
    {
        if(obj->mclk_hz == 0)
        {
            // Must specify mclk frequency when using mclk
            result = CYHAL_I2S_RSLT_ERR_INVALID_ARG;
        }
        else
        {
            result = _cyhal_utils_reserve_and_connect(mclk, mclk_map);
            if(CY_RSLT_SUCCESS == result)
            {
                obj->pin_mclk = mclk;
            }
        }
    }

    if(CY_RSLT_SUCCESS == result && CYHAL_NC_PIN_VALUE == mclk)
    {
        // Must not specify mclk frequency when mclk pin is not in use
        if(obj->mclk_hz != 0)
        {
            result = CYHAL_I2S_RSLT_ERR_INVALID_ARG;
        }
    }

    if(CY_RSLT_SUCCESS == result && obj->word_length > obj->channel_length)
    {
        // Word length must be less than or equal to channel length
        result = CYHAL_I2S_RSLT_ERR_INVALID_ARG;
    }

    if (CY_RSLT_SUCCESS == result)
    {
        if (clk != NULL)
        {
            obj->clock = *clk;
        }
        else if (CYHAL_NC_PIN_VALUE == mclk) // No need to reserve a clock if we're using the mclk pin
        {
            // The hardware is generally going to be hardwired to an hfclk, which has very limited divider options. In the event
            // that we're hooked up a PERI divider, we don't have any particular expectations about its width - so just ask for 8-bit
            result = _cyhal_utils_allocate_clock(&(obj->clock), &(obj->resource), CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT, true);
            if(CY_RSLT_SUCCESS == result)
            {
                obj->is_clock_owned = true;
                result = cyhal_clock_set_enabled(&(obj->clock), true, true);
            }
        }
    }

    uint8_t sclk_div;
    if(CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_i2s_compute_sclk_div(obj, obj->sample_rate_hz, &sclk_div);
    }

    cy_stc_i2s_config_t pdl_config;
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_i2s_populate_pdl_config(obj, &pdl_config, sclk_div);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = (cy_rslt_t)Cy_I2S_Init(obj->base, &pdl_config);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        if(pdl_config.txEnabled)
        {
            Cy_I2S_ClearTxFifo(obj->base);
        }
        if(pdl_config.rxEnabled)
        {
            Cy_I2S_ClearRxFifo(obj->base);
        }

        obj->pm_callback.states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE);
        obj->pm_callback.callback = &_cyhal_i2s_pm_callback;
        obj->pm_callback.next = NULL;
        obj->pm_callback.args = (void*)obj;
        obj->pm_callback.ignore_modes = CYHAL_SYSPM_BEFORE_TRANSITION;
        obj->pm_transition_ready = false;
        _cyhal_syspm_register_peripheral_callback(&(obj->pm_callback));

        _cyhal_i2s_config_structs[obj->resource.block_num] = obj;
        cy_stc_sysint_t irqCfg = { _cyhal_i2s_irq_n[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_i2s_irq_handler);
        NVIC_EnableIRQ(_cyhal_i2s_irq_n[obj->resource.block_num]);
    }

    if (CY_RSLT_SUCCESS != result)
    {
        cyhal_i2s_free(obj);
    }
    return result;
}

void cyhal_i2s_free(cyhal_i2s_t *obj)
{
    CY_ASSERT(NULL != obj);

    if(CYHAL_RSC_INVALID != obj->resource.type)
    {
        IRQn_Type irqn = _cyhal_i2s_irq_n[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        _cyhal_syspm_unregister_peripheral_callback(&(obj->pm_callback));
        cyhal_i2s_stop_rx(obj);
        cyhal_i2s_stop_tx(obj);
        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    _cyhal_utils_release_if_used(&(obj->pin_tx_sck));
    _cyhal_utils_release_if_used(&(obj->pin_tx_ws));
    _cyhal_utils_release_if_used(&(obj->pin_tx_sdo));
    _cyhal_utils_release_if_used(&(obj->pin_rx_sck));
    _cyhal_utils_release_if_used(&(obj->pin_rx_ws));
    _cyhal_utils_release_if_used(&(obj->pin_rx_sdi));
    _cyhal_utils_release_if_used(&(obj->pin_mclk));

    if(obj->is_clock_owned)
    {
        cyhal_hwmgr_free_clock(&(obj->clock));
    }

    if(CYHAL_RSC_INVALID != obj->rx_dma.resource.type)
    {
        cyhal_dma_free(&obj->rx_dma);
    }

    if(CYHAL_RSC_INVALID != obj->tx_dma.resource.type)
    {
        cyhal_dma_free(&obj->tx_dma);
    }
}

static cy_rslt_t _cyhal_i2s_compute_sclk_div(cyhal_i2s_t *obj, uint32_t sample_rate_hz, uint8_t *sclk_div)
{
    const uint8_t MAX_SCLK_DIVIDER = 64; // Divider value internal to the I2S block
    const cyhal_clock_tolerance_t SCLK_TOLERANCE = { .type = CYHAL_TOLERANCE_PERCENT, .value = 1 };
    uint32_t sclk_target = sample_rate_hz * obj->channel_length * 2 /* left + right channel */;
    *sclk_div = 0;

    if(obj->is_clock_owned)
    {
        // Try each of the divider values that we support internally, and see whether any of them gets us
        // within our tolerance of a frequency that our source clock can provide.
        for(uint8_t i = 1; i <= MAX_SCLK_DIVIDER; ++i)
        {
            uint32_t desired_source_freq = sclk_target * i * 8; // I2S hw has a hard-wired 8x divider
            cy_rslt_t freq_result = _cyhal_utils_set_clock_frequency(&(obj->clock), desired_source_freq, &SCLK_TOLERANCE);
            if(CY_RSLT_SUCCESS == freq_result)
            {
                *sclk_div = i;
                break;
            }
        }
    }
    else // Using user-provided clock, or using the mclk pin
    {
        // We can't change the clock, so just check if it's within tolerance
        uint32_t desired_divided_freq = sclk_target * 8; // I2S hw has a hard-wired 8x divider
        uint32_t actual_source_freq = (CYHAL_NC_PIN_VALUE == obj->pin_mclk) ? cyhal_clock_get_frequency(&obj->clock) : obj->mclk_hz;
        uint32_t best_divider = (actual_source_freq + (desired_divided_freq / 2)) / desired_divided_freq; // Round to nearest divider
        uint32_t desired_source_freq = desired_divided_freq * best_divider;
        uint32_t diff = (uint32_t)abs(_cyhal_utils_calculate_tolerance(SCLK_TOLERANCE.type, desired_source_freq, actual_source_freq));
        if(diff <= SCLK_TOLERANCE.value && best_divider <= MAX_SCLK_DIVIDER)
        {
            *sclk_div = (uint8_t)best_divider;
        }
    }

    return (0 == *sclk_div) ? CYHAL_I2S_RSLT_ERR_CLOCK : CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_set_sample_rate(cyhal_i2s_t *obj, uint32_t sample_rate_hz)
{
    uint8_t sclk_div;
    cy_stc_i2s_config_t pdl_config;

    cy_rslt_t result = _cyhal_i2s_compute_sclk_div(obj, sample_rate_hz, &sclk_div);
    if(CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_i2s_populate_pdl_config(obj, &pdl_config, sclk_div);
    }
    if(CY_RSLT_SUCCESS == result)
    {
        Cy_I2S_DeInit(obj->base);
        result = (cy_rslt_t)Cy_I2S_Init(obj->base, &pdl_config);
    }
    if(CY_RSLT_SUCCESS == result)
    {
        obj->sample_rate_hz = sample_rate_hz;
    }

    return result;
}

void cyhal_i2s_register_callback(cyhal_i2s_t *obj, cyhal_i2s_event_callback_t callback, void *callback_arg)
{
    CY_ASSERT(NULL != obj);

    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_i2s_enable_event(cyhal_i2s_t *obj, cyhal_i2s_event_t event, uint8_t intr_priority, bool enable)
{
    CY_ASSERT(NULL != obj);

    if (enable)
    {
        obj->user_enabled_events |= event;
    }
    else
    {
        obj->user_enabled_events &= ~event;
    }

    _cyhal_i2s_update_enabled_events(obj);
    IRQn_Type irqn = _cyhal_i2s_irq_n[obj->resource.block_num];
    NVIC_SetPriority(irqn, intr_priority);
}

cy_rslt_t cyhal_i2s_start_tx(cyhal_i2s_t *obj)
{
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    Cy_I2S_EnableTx(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_stop_tx(cyhal_i2s_t *obj)
{
    Cy_I2S_DisableTx(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_clear_tx(cyhal_i2s_t *obj)
{
    Cy_I2S_ClearTxFifo(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_start_rx(cyhal_i2s_t *obj)
{
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    Cy_I2S_EnableRx(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_stop_rx(cyhal_i2s_t *obj)
{
    Cy_I2S_DisableRx(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_clear_rx(cyhal_i2s_t *obj)
{
    Cy_I2S_ClearRxFifo(obj->base);
    return CY_RSLT_SUCCESS;
}

// Reads until empty, then updates the length and buffer address to their new locations
static void cyhal_i2s_read_until_empty(cyhal_i2s_t *obj, void** buffer, size_t* length)
{
    // The buffer is the smallest type that will hold the word length
    // The structure of this function deliberately accepts duplication of the outer loop
    // structure in order to avoid having to recheck the word length every time around,
    // because this function is in a performance sensitive code path.
    if(obj->word_length <= 8)
    {
        uint8_t *cast_buffer = (uint8_t*)(*buffer);

        while(*length > 0 && Cy_I2S_GetNumInRxFifo(obj->base) > 0)
        {
            *cast_buffer = (uint8_t)Cy_I2S_ReadRxData(obj->base);
            ++cast_buffer;
            --(*length);
        }
        *buffer = (void*)cast_buffer;
    }
    else if(obj->word_length <= 16)
    {
        uint16_t *cast_buffer = (uint16_t*)(*buffer);

        while(*length > 0 && Cy_I2S_GetNumInRxFifo(obj->base) > 0)
        {
            *cast_buffer = (uint16_t)Cy_I2S_ReadRxData(obj->base);
            ++cast_buffer;
            --(*length);
        }
        *buffer = (void*)cast_buffer;
    }
    else
    {
        CY_ASSERT(obj->word_length <= 32);
        uint32_t *cast_buffer = (uint32_t*)(*buffer);

        while(*length > 0 && Cy_I2S_GetNumInRxFifo(obj->base) > 0)
        {
            *cast_buffer = Cy_I2S_ReadRxData(obj->base);
            ++cast_buffer;
            --(*length);
        }
        *buffer = (void*)cast_buffer;
    }
}

cy_rslt_t cyhal_i2s_read(cyhal_i2s_t *obj, void *data, size_t* length)
{
    CY_ASSERT(NULL != obj);
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    size_t remaining = *length;
    cyhal_i2s_read_until_empty(obj, &data, &remaining);
    *length -= remaining;
    return CY_RSLT_SUCCESS;
}

static void cyhal_i2s_write_until_full(cyhal_i2s_t *obj, const void** buffer, size_t *length)
{
    // The buffer is the smallest type that will hold the word length
    // The structure of this function deliberately accepts duplication of the outer loop
    // structure in order to avoid having to recheck the word length every time around,
    // because this function is in a performance sensitive code path.
    if(obj->word_length <= 8)
    {
        const uint8_t *cast_buffer = (const uint8_t*)(*buffer);

        while(*length > 0 && Cy_I2S_GetNumInTxFifo(obj->base) < _CYHAL_I2S_FIFO_DEPTH)
        {
            Cy_I2S_WriteTxData(obj->base, *cast_buffer);
            ++cast_buffer;
            --(*length);
        }
        *buffer = (void*)cast_buffer;
    }
    else if(obj->word_length <= 16)
    {
        const uint16_t *cast_buffer = (const uint16_t*)(*buffer);

        while(*length > 0 && Cy_I2S_GetNumInTxFifo(obj->base) < _CYHAL_I2S_FIFO_DEPTH)
        {
            Cy_I2S_WriteTxData(obj->base, *cast_buffer);
            ++cast_buffer;
            --(*length);
        }
        *buffer = (void*)cast_buffer;
    }
    else
    {
        CY_ASSERT(obj->word_length <= 32);
        const uint32_t *cast_buffer = (const uint32_t*)(*buffer);

        while(*length > 0 && Cy_I2S_GetNumInTxFifo(obj->base) < _CYHAL_I2S_FIFO_DEPTH)
        {
            Cy_I2S_WriteTxData(obj->base, *cast_buffer);
            ++cast_buffer;
            --(*length);
        }
        *buffer = (void*)cast_buffer;
    }
}

cy_rslt_t cyhal_i2s_write(cyhal_i2s_t *obj, const void *data, size_t *length)
{
    CY_ASSERT(NULL != obj);
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    size_t remaining = *length;
    cyhal_i2s_write_until_full(obj, &data, &remaining);
    *length -= remaining;
    return CY_RSLT_SUCCESS;
}

bool cyhal_i2s_is_tx_enabled(cyhal_i2s_t *obj)
{
    CY_ASSERT(NULL != obj);

    return (0 != (CY_I2S_TX_START & Cy_I2S_GetCurrentState(obj->base)));
}

bool cyhal_i2s_is_tx_busy(cyhal_i2s_t *obj)
{
    CY_ASSERT(NULL != obj);

    return (0 != Cy_I2S_GetNumInTxFifo(obj->base)) || cyhal_i2s_is_write_pending(obj);
}

bool cyhal_i2s_is_rx_enabled(cyhal_i2s_t *obj)
{
    CY_ASSERT(NULL != obj);

    return (0 != (CY_I2S_RX_START & Cy_I2S_GetCurrentState(obj->base)));
}

bool cyhal_i2s_is_rx_busy(cyhal_i2s_t *obj)
{
    CY_ASSERT(NULL != obj);

    return (0 != Cy_I2S_GetNumInRxFifo(obj->base)) || cyhal_i2s_is_read_pending(obj);
}

cy_rslt_t cyhal_i2s_read_async(cyhal_i2s_t *obj, void *rx, size_t rx_length)
{
    CY_ASSERT(NULL != obj);
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->async_rx_buff = rx;
    obj->async_rx_length = rx_length;
    cyhal_system_critical_section_exit(savedIntrStatus);
    switch(obj->async_mode)
    {
        case CYHAL_ASYNC_SW:
        {
            /* Read as much as we can now, then set up an interrupt to do the rest
             * This is a potentially long operation but we don't want other I2S operations to
             * interleave with it. So do a "mini critical section" and disable the interrupts for this block only.
             */
            uint32_t old_interrupt_mask = Cy_I2S_GetInterruptMask(obj->base);
            Cy_I2S_SetInterruptMask(obj->base, 0u);
            // Safe to cast away volatile here because we're calling read_until_empty from within
            // a critical section, so it should not change out from under us during this call
            cyhal_i2s_read_until_empty(obj, (void**)(&obj->async_rx_buff), (size_t*)(&obj->async_rx_length));
            _cyhal_i2s_update_rx_trigger_level(obj);
            Cy_I2S_SetInterruptMask(obj->base, old_interrupt_mask);
            if(obj->async_rx_length > 0)
            {
                _cyhal_i2s_update_enabled_events(obj);
            }
            else
            {
                _cyhal_i2s_process_event(obj, CYHAL_I2S_ASYNC_RX_COMPLETE);
            }
           break;
        }
        case CYHAL_ASYNC_DMA:
        {
            // Don't directly kick off the DMA here - it will be triggered
            // from the interrupt handler when the FIFO rised above the threshold
            // (which may have already happened by the time we get here if the
            // application already had the full or half-full event enabled)
            _cyhal_i2s_update_rx_trigger_level(obj);
            _cyhal_i2s_update_enabled_events(obj);
            break;
        }
        default:
           CY_ASSERT(0); /* Unrecognized async mode */
    }

    return CY_RSLT_SUCCESS;
}

static cy_rslt_t _cyhal_i2s_populate_pdl_config(cyhal_i2s_t *obj, cy_stc_i2s_config_t* pdl_config, uint8_t sclk_div)
{
    cy_en_i2s_len_t pdl_word_length, pdl_channel_length;
    cy_rslt_t result = _cyhal_i2s_convert_length(obj->channel_length, &pdl_channel_length);
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_i2s_convert_length(obj->word_length, &pdl_word_length);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        *pdl_config = _cyhal_i2s_default_config;
        pdl_config->txEnabled = (CYHAL_NC_PIN_VALUE != obj->pin_tx_sdo);
        pdl_config->rxEnabled = (CYHAL_NC_PIN_VALUE != obj->pin_rx_sdi);
        pdl_config->extClk = (CYHAL_NC_PIN_VALUE != obj->pin_mclk);
        pdl_config->clkDiv = sclk_div;
        pdl_config->txMasterMode = !obj->is_tx_slave;
        pdl_config->rxMasterMode = !obj->is_rx_slave;
        pdl_config->rxChannelLength = pdl_channel_length;
        pdl_config->rxWordLength = pdl_word_length;
        pdl_config->txChannelLength = pdl_channel_length;
        pdl_config->txWordLength = pdl_word_length;
    }
    return result;
}

// Round up the word length to the next power of 2
static uint8_t _cyhal_i2s_rounded_word_length(cyhal_i2s_t *obj)
{
    CY_ASSERT(obj->word_length <= 32);
    if(obj->word_length <= 8)
    {
        return 8u;
    }
    else if(obj->word_length <= 16)
    {
        return 16u;
    }

    return 32u;
}

cy_rslt_t cyhal_i2s_write_async(cyhal_i2s_t *obj, const void *tx, size_t tx_length)
{
    CY_ASSERT(NULL != obj);
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->async_tx_buff = tx;
    obj->async_tx_length = tx_length;
    cyhal_system_critical_section_exit(savedIntrStatus);
    switch(obj->async_mode)
    {
        case CYHAL_ASYNC_SW:
        {
            /* Write as much as we can now, then set up an interrupt to do the rest
            * This is a potentially long operation but we don't want other I2S operations to
            * interleave with it. So do a "mini critical section" and disable the interrupts for this block only.
            */
            uint32_t old_interrupt_mask = Cy_I2S_GetInterruptMask(obj->base);
            Cy_I2S_SetInterruptMask(obj->base, 0u);
            // Safe to cast away volatile here because we're calling write_until_full from within
            // a critical section, so it should not change out from under us during this call
            cyhal_i2s_write_until_full(obj, (const void**)(&obj->async_tx_buff), (size_t *)(&obj->async_tx_length));
            Cy_I2S_SetInterruptMask(obj->base, old_interrupt_mask);
            if(obj->async_tx_length > 0)
            {
                _cyhal_i2s_update_enabled_events(obj);
            }
            else
            {
                _cyhal_i2s_process_event(obj, CYHAL_I2S_ASYNC_TX_COMPLETE);
            }
            break;
        }
        case CYHAL_ASYNC_DMA:
        {
            // Don't directly kick off the DMA here - it will be triggered
            // from the interrupt handler when the FIFO drops below the threshold
            // (which may have already happened by the time we get here if the
            // application already had the half-empty or empty event enabled)
            _cyhal_i2s_update_enabled_events(obj);
            break;
        }
        default:
            CY_ASSERT(0); /* Unrecognized async mode */
            break;
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_set_async_mode(cyhal_i2s_t *obj, cyhal_async_mode_t mode, uint8_t dma_priority)
{
    CY_ASSERT(NULL != obj);
    // We don't support swapping the async mode out from under a pending transfer.
    CY_ASSERT(false == cyhal_i2s_is_read_pending(obj) && false == cyhal_i2s_is_write_pending(obj));

    cy_rslt_t result = CY_RSLT_SUCCESS;

    if(mode == CYHAL_ASYNC_DMA)
    {
        // Reserve a DMA channel for each direction that is enabled
        if(CYHAL_NC_PIN_VALUE != obj->pin_tx_sck && CYHAL_RSC_INVALID == obj->tx_dma.resource.type)
        {
            /* Reserve a DMA channel for async transmit if tx is enabled */
            result = cyhal_dma_init(&obj->tx_dma, CYHAL_DMA_PRIORITY_DEFAULT, CYHAL_DMA_DIRECTION_MEM2PERIPH);
            cyhal_dma_register_callback(&obj->tx_dma, &_cyhal_i2s_dma_handler_tx, obj);
        }
        if(mode == CYHAL_ASYNC_DMA && CYHAL_NC_PIN_VALUE != obj->pin_rx_sck && CYHAL_RSC_INVALID == obj->rx_dma.resource.type)
        {
            /* Reserve a DMA channel for async receive if rx is enabled */
            result = cyhal_dma_init(&obj->rx_dma, CYHAL_DMA_PRIORITY_DEFAULT, CYHAL_DMA_DIRECTION_PERIPH2MEM);
            cyhal_dma_register_callback(&obj->rx_dma, &_cyhal_i2s_dma_handler_rx, obj);
        }
    }
    else
    {
        /* Free the DMA instances if we reserved them but don't need them anymore */
        if(CYHAL_RSC_INVALID != obj->tx_dma.resource.type)
        {
            cyhal_dma_free(&obj->tx_dma);
            obj->tx_dma.resource.type = CYHAL_RSC_INVALID;
        }
        if(CYHAL_RSC_INVALID != obj->rx_dma.resource.type)
        {
            cyhal_dma_free(&obj->rx_dma);
            obj->rx_dma.resource.type = CYHAL_RSC_INVALID;
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->async_mode = mode;
        obj->async_dma_priority = dma_priority;
    }
    return result;
}

bool cyhal_i2s_is_read_pending(cyhal_i2s_t *obj)
{
    return (NULL != obj->async_rx_buff);
}

bool cyhal_i2s_is_write_pending(cyhal_i2s_t *obj)
{
    return (NULL != obj->async_tx_buff);
}

cy_rslt_t cyhal_i2s_abort_read_async(cyhal_i2s_t *obj) {
    uint32_t saved_intr = cyhal_system_critical_section_enter();
    obj->async_rx_buff = NULL;
    _cyhal_i2s_update_enabled_events(obj);
    cyhal_system_critical_section_exit(saved_intr);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_i2s_abort_write_async(cyhal_i2s_t *obj)
{
    uint32_t saved_intr = cyhal_system_critical_section_enter();
    obj->async_tx_buff = NULL;
    _cyhal_i2s_update_enabled_events(obj);
    cyhal_system_critical_section_exit(saved_intr);
    return CY_RSLT_SUCCESS;
}

static cyhal_i2s_event_t _cyhal_i2s_convert_interrupt_cause(uint32_t pdl_cause)
{
    cyhal_i2s_event_t result = (cyhal_i2s_event_t)0u;
    if(0 != (pdl_cause & CY_I2S_INTR_TX_NOT_FULL))
    {
        result |= CYHAL_I2S_TX_NOT_FULL;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_TX_TRIGGER))
    {
        result |= CYHAL_I2S_TX_HALF_EMPTY;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_TX_EMPTY))
    {
        result |= CYHAL_I2S_TX_EMPTY;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_TX_OVERFLOW))
    {
        result |= CYHAL_I2S_TX_OVERFLOW;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_TX_UNDERFLOW))
    {
        result |= CYHAL_I2S_TX_UNDERFLOW ;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_RX_NOT_EMPTY))
    {
        result |= CYHAL_I2S_RX_NOT_EMPTY;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_RX_TRIGGER))
    {
        result |= CYHAL_I2S_RX_HALF_FULL;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_RX_FULL))
    {
        result |= CYHAL_I2S_RX_FULL;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_RX_OVERFLOW))
    {
        result |= CYHAL_I2S_RX_OVERFLOW;
    }
    if(0 != (pdl_cause & CY_I2S_INTR_RX_UNDERFLOW))
    {
        result |= CYHAL_I2S_RX_UNDERFLOW;
    }

    return result;
}

static uint32_t _cyhal_i2s_convert_event(cyhal_i2s_event_t event)
{
    uint32_t pdl_event = 0u;
    if(0 != (event & CYHAL_I2S_TX_NOT_FULL))
    {
        pdl_event |= CY_I2S_INTR_TX_NOT_FULL;
    }
    if(0 != (event & CYHAL_I2S_TX_HALF_EMPTY))
    {
        pdl_event |= CY_I2S_INTR_TX_TRIGGER;
    }
    if(0 != (event & CYHAL_I2S_TX_EMPTY))
    {
        pdl_event |= CY_I2S_INTR_TX_EMPTY;
    }
    if(0 != (event & CYHAL_I2S_TX_OVERFLOW))
    {
        pdl_event |= CY_I2S_INTR_TX_OVERFLOW;
    }
    if(0 != (event & CYHAL_I2S_TX_UNDERFLOW ))
    {
        pdl_event |= CY_I2S_INTR_TX_UNDERFLOW;
    }
    if(0 != (event & CYHAL_I2S_RX_NOT_EMPTY))
    {
        pdl_event |= CY_I2S_INTR_RX_NOT_EMPTY;
    }
    if(0 != (event & CYHAL_I2S_RX_HALF_FULL))
    {
        pdl_event |= CY_I2S_INTR_RX_TRIGGER;
    }
    if(0 != (event & CYHAL_I2S_RX_FULL))
    {
        pdl_event |= CY_I2S_INTR_RX_FULL;
    }
    if(0 != (event & CYHAL_I2S_RX_OVERFLOW))
    {
        pdl_event |= CY_I2S_INTR_RX_OVERFLOW;
    }
    if(0 != (event & CYHAL_I2S_RX_UNDERFLOW))
    {
        pdl_event |= CY_I2S_INTR_RX_UNDERFLOW;
    }

    return pdl_event;
}

static cy_rslt_t _cyhal_i2s_convert_length(uint8_t user_length, cy_en_i2s_len_t *pdl_length)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    switch(user_length)
    {
        case 8u:
            *pdl_length = CY_I2S_LEN8;
            break;
        case 16u:
            *pdl_length = CY_I2S_LEN16;
            break;
        case 18u:
            *pdl_length = CY_I2S_LEN18;
            break;
        case 20u:
            *pdl_length = CY_I2S_LEN20;
            break;
        case 24u:
            *pdl_length = CY_I2S_LEN24;
            break;
        case 32u:
            *pdl_length = CY_I2S_LEN32;
            break;
        default:
            result = CYHAL_I2S_RSLT_ERR_INVALID_ARG;
    }
    return result;
}

static void _cyhal_i2s_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block = _cyhal_i2s_get_block_from_irqn(irqn);
    cyhal_i2s_t* obj = _cyhal_i2s_config_structs[block];

    uint32_t interrupt_status = Cy_I2S_GetInterruptStatusMasked(obj->base);
    Cy_I2S_ClearInterrupt(obj->base, interrupt_status);
    cyhal_i2s_event_t event = _cyhal_i2s_convert_interrupt_cause(interrupt_status);
    _cyhal_i2s_process_event(obj, event);
}

static void _cyhal_i2s_update_enabled_events(cyhal_i2s_t *obj)
{
    cyhal_i2s_event_t events = (cyhal_i2s_event_t)obj->user_enabled_events;
    if(NULL != obj->async_tx_buff && obj->async_tx_length > 0)
    {
        events |= (CYHAL_I2S_TX_EMPTY | CYHAL_I2S_TX_HALF_EMPTY);
    }
    if(NULL != obj->async_rx_buff && obj->async_rx_length > 0)
    {
        events |= (CYHAL_I2S_RX_FULL | CYHAL_I2S_RX_HALF_FULL);
    }

    uint32_t mask = _cyhal_i2s_convert_event(events);
    // The register is 24 bits wide but the hardware pads the value out with 1's when read.
    // So mask down to just the bits that we actually care about.
    uint32_t old_mask = Cy_I2S_GetInterruptMask(obj->base) & CY_I2S_INTR_MASK;

    // Clear the interrupts that are about to be enabled to avoid spurious firing
    uint32_t new_interrupts = mask & (~old_mask);
    Cy_I2S_ClearInterrupt(obj->base, new_interrupts);

    Cy_I2S_SetInterruptMask(obj->base, mask);
}

static void _cyhal_i2s_update_rx_trigger_level(cyhal_i2s_t *obj)
{
    // If we're doing an async read and the amount remaining is less than
    // the standard trigger level, temporarily reduce it so that we get
    // an interrupt as soon as the amount the user requested is ready
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    uint8_t trigger_level = _CYHAL_I2S_FIFO_DEPTH / 2;
    if(NULL != obj->async_rx_buff
       && obj->async_rx_length < trigger_level
       && obj->async_rx_length > 0)
    {
        trigger_level = obj->async_rx_length;
    }

    // Safe to do a blind write of this register because the only other bits are
    // CLEAR, which is only set temporarily from clear_tx, and FREEZE, which is
    // never used by this driver (it exists for debugging purposes only)
    obj->base->RX_FIFO_CTL = (trigger_level << I2S_RX_FIFO_CTL_TRIGGER_LEVEL_Pos);
    cyhal_system_critical_section_exit(savedIntrStatus);
}

static cy_rslt_t _cyhal_i2s_dma_perform_rx(cyhal_i2s_t *obj)
{
    // We could have received an event after we started the DMA but before it
    // managed to bring the FIFO below the threshold
    if(cyhal_dma_is_busy(&(obj->rx_dma)))
        return CY_RSLT_SUCCESS;
    size_t transfer_size = _CYHAL_I2S_DMA_BURST_SIZE;
    if (transfer_size >= obj->async_rx_length)
    {
        transfer_size = obj->async_rx_length;
        // Only want the user callback to be call on the last dma transfer.
        cyhal_dma_enable_event(&(obj->rx_dma), CYHAL_DMA_TRANSFER_COMPLETE, obj->async_dma_priority, true);
    }

    cyhal_dma_cfg_t dma_cfg =
    {
        .src_addr = (uint32_t)(&(obj->base->RX_FIFO_RD)),
        .src_increment = 0,
        .dst_addr = (uint32_t)obj->async_rx_buff,
        .dst_increment = 1,
        .transfer_width = _cyhal_i2s_rounded_word_length(obj),
        .length = transfer_size,
        .burst_size = 0,
        .action = CYHAL_DMA_TRANSFER_FULL,
    };
    cy_rslt_t result = cyhal_dma_configure(&(obj->rx_dma), &dma_cfg);

    // Update the buffer first so that it's guaranteed to be correct whenever the DMA completes
    if(CY_RSLT_SUCCESS == result)
    {
        size_t increment_bytes = transfer_size * (_cyhal_i2s_rounded_word_length(obj) / 8);
        uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
        obj->async_rx_buff = (void*)(((uint8_t*) obj->async_rx_buff) + increment_bytes);
        obj->async_rx_length -= transfer_size;
        _cyhal_i2s_update_rx_trigger_level(obj);
        _cyhal_i2s_update_enabled_events(obj);
        cyhal_system_critical_section_exit(savedIntrStatus);

        result = cyhal_dma_start_transfer(&(obj->rx_dma));
    }

    return result;
}

static cy_rslt_t _cyhal_i2s_dma_perform_tx(cyhal_i2s_t *obj)
{
    // We could have received an event after the DMA brought the FIFO below the
    // threshold but before the DMA is entirely complete
    if(cyhal_dma_is_busy(&(obj->tx_dma)))
        return CY_RSLT_SUCCESS;

    CY_ASSERT(NULL != obj->async_tx_buff);

    size_t transfer_size = _CYHAL_I2S_DMA_BURST_SIZE;
    if (transfer_size >= obj->async_tx_length)
    {
        transfer_size = obj->async_tx_length;
        // Only want the user callback to be call on the last dma transfer.
        cyhal_dma_enable_event(&(obj->tx_dma), CYHAL_DMA_TRANSFER_COMPLETE, obj->async_dma_priority, true);
    }

    cyhal_dma_cfg_t dma_cfg =
    {
        .src_addr = (uint32_t)obj->async_tx_buff,
        .src_increment = 1,
        .dst_addr = (uint32_t)(&(obj->base->TX_FIFO_WR)),
        .dst_increment = 0,
        .transfer_width = _cyhal_i2s_rounded_word_length(obj),
        .length = transfer_size,
        .burst_size = 0,
        .action = CYHAL_DMA_TRANSFER_FULL,
    };
    cy_rslt_t result = cyhal_dma_configure(&(obj->tx_dma), &dma_cfg);

    // Update the buffer first so that it's guaranteed to be correct whenever the DMA completes
    if(CY_RSLT_SUCCESS == result)
    {
        size_t increment_bytes = transfer_size * (_cyhal_i2s_rounded_word_length(obj) / 8);
        uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
        obj->async_tx_buff = (void*)(((uint8_t*) obj->async_tx_buff) + increment_bytes);
        obj->async_tx_length -= transfer_size;

        // Do this after we've updated async_tx_buff/length because once we have kicked
        // off the final DMA transfer there is no further action we will take on the
        // half-empty/empty events, and we don't want those to wind upstarving the
        // DMA complete event
        _cyhal_i2s_update_enabled_events(obj);
        cyhal_system_critical_section_exit(savedIntrStatus);

        result = cyhal_dma_start_transfer(&(obj->tx_dma));
    }

    return result;
}

/* Callback argument is the I2S instance */
static void _cyhal_i2s_dma_handler_rx(void *callback_arg, cyhal_dma_event_t event)
{
    CY_UNUSED_PARAMETER(event);
    /* We only hook this handler up when we're doing the final transfer, so send the completed event */
    CY_ASSERT(CYHAL_DMA_TRANSFER_COMPLETE == event);

    cyhal_i2s_t *obj = (cyhal_i2s_t*)callback_arg;
    obj->async_rx_buff = NULL;
    cyhal_dma_enable_event(&obj->rx_dma, CYHAL_DMA_TRANSFER_COMPLETE, obj->async_dma_priority, false);
    _cyhal_i2s_process_event(obj, CYHAL_I2S_ASYNC_RX_COMPLETE);
}

/* Callback argument is the I2S instance */
static void _cyhal_i2s_dma_handler_tx(void *callback_arg, cyhal_dma_event_t event)
{
    CY_UNUSED_PARAMETER(event);
    /* We only hook this handler up when we're doing the final transfer, so send the completed event */
    CY_ASSERT(CYHAL_DMA_TRANSFER_COMPLETE == event);

    cyhal_i2s_t *obj = (cyhal_i2s_t*)callback_arg;
    obj->async_tx_buff = NULL;
    cyhal_dma_enable_event(&obj->tx_dma, CYHAL_DMA_TRANSFER_COMPLETE, obj->async_dma_priority, false);
    _cyhal_i2s_process_event(obj, CYHAL_I2S_ASYNC_TX_COMPLETE);
}

static void _cyhal_i2s_process_event(cyhal_i2s_t *obj, cyhal_i2s_event_t event)
{
    if(0 != (event & (CYHAL_I2S_TX_HALF_EMPTY | CYHAL_I2S_TX_EMPTY)))
    {
        /* We should normally not get the "empty" interrupt during an async transfer because we
         * should be topping the FIFO back up after each half-empty interrupt. But in case something
         * delays our response and the FIFO gets all the way to empty, listen for that as well
         */
        uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
        if(NULL != obj->async_tx_buff && obj->async_tx_length > 0)
        {
            switch(obj->async_mode)
            {
                case CYHAL_ASYNC_SW:
                {
                    /* Write as much as we can out until the FIFO is full
                     * This is a potentially long operation but we don't want other I2S operations to
                     * interleave with it. So switch to a "mini critical section" and disable the
                     * interrupts for this block only while we're copying
                     */
                    uint32_t old_interrupt_mask = Cy_I2S_GetInterruptMask(obj->base);
                    Cy_I2S_SetInterruptMask(obj->base, 0u);
                    cyhal_system_critical_section_exit(savedIntrStatus);
                    // Safe to cast away volatile here because we're calling write_until_full from within
                    // a critical section, so it should not change out from under us during this call
                    cyhal_i2s_write_until_full(obj, (const void**)(&obj->async_tx_buff), (size_t *)(&obj->async_tx_length));

                    // Re-enter the global critical section so that the exit below behaves correctly
                    savedIntrStatus = cyhal_system_critical_section_enter();
                    Cy_I2S_SetInterruptMask(obj->base, old_interrupt_mask);
                    if(0 == obj->async_tx_length)
                    {
                        /* We finished the async transfer.  */
                        event |= CYHAL_I2S_ASYNC_TX_COMPLETE;
                    }
                    break;
                }
                case CYHAL_ASYNC_DMA:
                {
                    cy_rslt_t result = _cyhal_i2s_dma_perform_tx(obj);
                    CY_UNUSED_PARAMETER(result);
                    CY_ASSERT(CY_RSLT_SUCCESS == result);
                    break;
                }
                default:
                    CY_ASSERT(0); /* Unrecognized async mode */
                    break;
            }
        }

        cyhal_system_critical_section_exit(savedIntrStatus);
    }
    if(0 != (event & (CYHAL_I2S_RX_HALF_FULL | CYHAL_I2S_RX_FULL)))
    {
        /* Similar to TX, we don't expect to receive the "full" interrupt, but check for it out of caution */
        uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
        if(NULL != obj->async_rx_buff && obj->async_rx_length > 0)
        {
            switch(obj->async_mode)
            {
                case CYHAL_ASYNC_SW:
                {
                   /* Read as much as we can until the FIFO is empty
                     * This is a potentially long operation but we don't want other I2S operations to
                     * interleave with it. So switch to a "mini critical section" and disable the
                     * interrupts for this block only while we're copying
                    */
                   uint32_t old_interrupt_mask = Cy_I2S_GetInterruptMask(obj->base);
                   Cy_I2S_SetInterruptMask(obj->base, 0u);
                   cyhal_system_critical_section_exit(savedIntrStatus);
                   // Safe to cast away volatile here because we're calling read_until_empty from within
                   // a critical section, so it should not change out from under us during this call
                   cyhal_i2s_read_until_empty(obj, (void**)(&obj->async_rx_buff), (size_t*)(&obj->async_rx_length));
                   // Re-enter the global critical section so that the exit below behaves correctly
                   savedIntrStatus = cyhal_system_critical_section_enter();
                   Cy_I2S_SetInterruptMask(obj->base, old_interrupt_mask);
                   _cyhal_i2s_update_enabled_events(obj);
                   if(0 == obj->async_rx_length)
                   {
                       /* We finished the async transfer.  */
                       event |= CYHAL_I2S_ASYNC_RX_COMPLETE;
                   }
                   break;
                }
                case CYHAL_ASYNC_DMA:
                   _cyhal_i2s_dma_perform_rx(obj);
                   break;

                default:
                   CY_ASSERT(0); /* Unrecognized async mode */
            }

            // During async rx transfers, we may temporarily set the trigger level below half-full.
            // So make sure that it's a real "half full" and skip propagating to the user if it isn't
            uint8_t trigger_level = (obj->base->TX_FIFO_CTL & I2S_TX_FIFO_CTL_TRIGGER_LEVEL_Msk) >> I2S_TX_FIFO_CTL_TRIGGER_LEVEL_Pos;
            if(trigger_level != _CYHAL_I2S_FIFO_DEPTH / 2)
            {
                event &= ~CYHAL_I2S_RX_HALF_FULL;
            }
        }

        cyhal_system_critical_section_exit(savedIntrStatus);
    }

    /* Mark async transfer as complete if we just finished one. */
    if(0 != (event & CYHAL_I2S_ASYNC_TX_COMPLETE))
    {
        obj->async_tx_buff = NULL;
        _cyhal_i2s_update_enabled_events(obj);
    }
    if(0 != (event & CYHAL_I2S_ASYNC_RX_COMPLETE))
    {
        obj->async_rx_buff = NULL;
        _cyhal_i2s_update_enabled_events(obj);
    }

    if(0 != (event & ((cyhal_i2s_event_t)obj->user_enabled_events)))
    {
        cyhal_i2s_event_callback_t callback = (cyhal_i2s_event_callback_t)obj->callback_data.callback;
        if(NULL != callback)
        {
            callback(obj->callback_data.callback_arg, (cyhal_i2s_event_t)(event & obj->user_enabled_events));
        }
    }
}

static bool _cyhal_i2s_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    cyhal_i2s_t *obj = (cyhal_i2s_t *)callback_arg;
    CY_UNUSED_PARAMETER(state);

    switch(mode)
    {
        case CYHAL_SYSPM_CHECK_READY:
            obj->pm_transition_ready = Cy_I2S_GetCurrentState(obj->base) == 0 && !(cyhal_i2s_is_read_pending(obj) || cyhal_i2s_is_tx_busy(obj));
            return obj->pm_transition_ready;
        case CYHAL_SYSPM_CHECK_FAIL:
        case CYHAL_SYSPM_AFTER_TRANSITION:
            obj->pm_transition_ready = false;
            return true;
        default:
            return true;
    }
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
