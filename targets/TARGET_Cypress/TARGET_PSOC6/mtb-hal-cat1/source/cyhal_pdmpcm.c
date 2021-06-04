/*******************************************************************************
* File Name: cyhal_pdmpcm.c
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
#include "cyhal_dma.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_pdmpcm.h"
#include "cyhal_syspm_impl.h"
#include "cyhal_system.h"
#include "cyhal_utils.h"
#include "cy_device.h"
#include "cy_device_headers.h"
#include "cy_pdm_pcm.h"
#include "cy_utils.h"

/**
* \addtogroup group_hal_impl_pdmpcm PDM/PCM (Pulse Density Modulation to Pulse Code Modulation Converter)
* \ingroup group_hal_impl
* \{
* The CAT1 (PSoC 6) PDM/PCM Supports the following conversion parameters:<ul>
* <li>Mode: Mono Left, Mono Right, Stereo
* <li>Word Length: 16/18/20/24 bits</li>
* <li>Sampling Rate: up to 48kHz</li>
* <li>Left/Right Gain Amplifier: -12dB to +10.5dB in 1.5dB steps.</li>
* </ul>
* \} group_hal_impl_pdmpcm
*/

#ifdef CY_IP_MXAUDIOSS_INSTANCES

#if defined(__cplusplus)
extern "C"
{
#endif

 // 35-45 PCM samples it takes for PCM to stabilize. Round to even so that same number of left and right samples are removed
#define _CYHAL_PDM_PCM_STABILIZATION_FS 46
#define _CYHAL_PDM_PCM_EVENT_NONE ((cyhal_pdm_pcm_event_t) 0x0)
#define _CYHAL_PDM_PCM_HFCLK (1)
#define _CYHAL_PDM_PCM_HALF_FIFO (0x80U)


static PDM_Type *const _cyhal_pdm_pcm_base[] =
{
#if (CY_IP_MXAUDIOSS_INSTANCES == 1 && defined(AUDIOSS_PDM) && AUDIOSS_PDM)
    PDM,
#elif (CY_IP_MXAUDIOSS_INSTANCES >= 1 && defined(AUDIOSS0_PDM) && AUDIOSS0_PDM)
    PDM0,
#endif
#if (CY_IP_MXAUDIOSS_INSTANCES >= 2 && defined(AUDIOSS1_PDM) && AUDIOSS1_PDM)
    PDM1,
#endif

#if (CY_IP_MXS40AUDIOSS_INSTANCES > 2)
    #warning Unhandled audioss instance count
#endif
};

static cyhal_pdm_pcm_t* _cyhal_pdm_pcm_config_structs[CY_IP_MXAUDIOSS_INSTANCES];

static const IRQn_Type _cyhal_pdm_pcm_irq_n[] =
{
#if (CY_IP_MXAUDIOSS_INSTANCES == 1 && defined(AUDIOSS_PDM) && AUDIOSS_PDM) // Without index suffix
    audioss_interrupt_pdm_IRQn,
#elif (CY_IP_MXAUDIOSS_INSTANCES >= 1 && defined(AUDIOSS0_PDM) && AUDIOSS0_PDM)
    audioss_0_interrupt_pdm_IRQn,
#endif
#if (CY_IP_MXAUDIOSS_INSTANCES >= 2 && defined(AUDIOSS1_PDM) && AUDIOSS1_PDM)
    audioss_1_interrupt_pdm_IRQn,
#endif

#if (CY_IP_MXS40AUDIOSS_INSTANCES > 2)
    #warning Unhandled audioss instance count
#endif
};

static uint8_t _cyhal_pdm_pcm_get_block_from_irqn(IRQn_Type irqn) {
    switch (irqn)
    {
#if (CY_CPU_CORTEX_M4)
#if (CY_IP_MXAUDIOSS_INSTANCES == 1 && defined(AUDIOSS_PDM) && AUDIOSS_PDM) // Without index suffix
    case audioss_interrupt_pdm_IRQn:
        return 0;
#elif (CY_IP_MXAUDIOSS_INSTANCES >= 1 && defined(AUDIOSS0_PDM) && AUDIOSS0_PDM)
    case audioss_0_interrupt_pdm_IRQn:
        return 0;
#endif
#if (CY_IP_MXAUDIOSS_INSTANCES >= 2 && defined(AUDIOSS1_PDM) && AUDIOSS1_PDM)
    case audioss_1_interrupt_pdm_IRQn:
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

static const cy_stc_pdm_pcm_config_t _cyhal_pdm_pcm_default_config =
{
    .clkDiv = CY_PDM_PCM_CLK_DIV_BYPASS, // Configured by cyhal_pdm_pcm_init
    .mclkDiv = CY_PDM_PCM_CLK_DIV_BYPASS, // Configured by cyhal_pdm_pcm_init
    .ckoDiv = 3U, // Configured by cyhal_pdm_pcm_init
    .ckoDelay = 0U,
    .sincDecRate = 32U, // Configured by cyhal_pdm_pcm_init
    .chanSelect = CY_PDM_PCM_OUT_STEREO, // Configured by cyhal_pdm_pcm_init
    .chanSwapEnable = false,
    .highPassFilterGain = 8U,
    .highPassDisable = false,
    .softMuteCycles = CY_PDM_PCM_SOFT_MUTE_CYCLES_96,
    .softMuteFineGain = 1UL,
    .softMuteEnable = false,
    .wordLen = CY_PDM_PCM_WLEN_16_BIT, // Configured by cyhal_pdm_pcm_init
    .signExtension = true,
    .gainLeft = CY_PDM_PCM_BYPASS, // Configured by cyhal_pdm_pcm_init and cyhal_pdm_pcm_set_gain
    .gainRight = CY_PDM_PCM_BYPASS, // Configured by cyhal_pdm_pcm_init and cyhal_pdm_pcm_set_gain
    .rxFifoTriggerLevel = _CYHAL_PDM_PCM_HALF_FIFO - 1,
    .dmaTriggerEnable = false,
    .interruptMask = 0UL,
};

static inline void _cyhal_pdm_pcm_set_rx_fifo_level(cyhal_pdm_pcm_t *obj, uint8_t fifo_level)
{
    PDM_PCM_RX_FIFO_CTL(obj->base) = _VAL2FLD(PDM_RX_FIFO_CTL_TRIGGER_LEVEL, fifo_level - 1);
    Cy_PDM_PCM_ClearInterrupt(obj->base, CY_PDM_PCM_INTR_RX_TRIGGER);
}

static bool _cyhal_pdm_pcm_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    CY_UNUSED_PARAMETER(state);
    cyhal_pdm_pcm_t *obj = (cyhal_pdm_pcm_t *)callback_arg;

    switch (mode)
    {
        case CYHAL_SYSPM_CHECK_READY:
            obj->pm_transition_ready = Cy_PDM_PCM_GetCurrentState(obj->base) == 0 && !cyhal_pdm_pcm_is_pending(obj);
            break;
        case CYHAL_SYSPM_CHECK_FAIL:
        case CYHAL_SYSPM_AFTER_TRANSITION:
            obj->pm_transition_ready = false;
            break;
        default:
            break;
    }
    return obj->pm_transition_ready;
}

static inline void _cyhal_pdm_pcm_increment_async_buffer(cyhal_pdm_pcm_t *obj, size_t increment)
{
    CY_ASSERT(obj->async_read_remaining >= increment);
    uint32_t saved_intr = cyhal_system_critical_section_enter();
    obj->async_read_remaining -= increment;
    obj->async_buffer = (obj->async_read_remaining == 0)
        ? NULL
        : (void*)(((uint8_t*) obj->async_buffer) + increment * obj->word_size);
    cyhal_system_critical_section_exit(saved_intr);
}

static inline void _cyhal_pdm_pcm_try_read_async(cyhal_pdm_pcm_t *obj)
{
    size_t read_remaining = obj->async_read_remaining;
    cyhal_pdm_pcm_read(obj, obj->async_buffer, &read_remaining);
    _cyhal_pdm_pcm_increment_async_buffer(obj, read_remaining);
}

static inline cy_rslt_t _cyhal_pdm_pcm_dma_start(cyhal_pdm_pcm_t *obj)
{
    cy_rslt_t rslt;
    size_t transfer_size = _CYHAL_PDM_PCM_HALF_FIFO;
    if (obj->async_read_remaining <= _CYHAL_PDM_PCM_HALF_FIFO)
    {
        transfer_size = obj->async_read_remaining;
        // Only want the user callback to be call on the last dma transfer.
        cyhal_dma_enable_event(&(obj->dma), CYHAL_DMA_TRANSFER_COMPLETE, CYHAL_ISR_PRIORITY_DEFAULT, true);
    }

    cyhal_dma_cfg_t dma_cfg =
    {
        .src_addr = (uint32_t)(&(obj->base->RX_FIFO_RD)),
        .src_increment = 0,
        .dst_addr = (uint32_t)obj->async_buffer,
        .dst_increment = 1,
        .transfer_width = 8 * obj->word_size,
        .length = transfer_size,
        .burst_size = 0,
        .action = CYHAL_DMA_TRANSFER_BURST,
    };
    rslt = cyhal_dma_configure(&(obj->dma), &dma_cfg);
    if (CY_RSLT_SUCCESS == rslt)
    {
        _cyhal_pdm_pcm_increment_async_buffer(obj, transfer_size);
        rslt = cyhal_dma_start_transfer(&(obj->dma));
    }
    return rslt;
}

static void _cyhal_pdm_pcm_hw_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    cyhal_pdm_pcm_t *obj = _cyhal_pdm_pcm_config_structs[_cyhal_pdm_pcm_get_block_from_irqn(irqn)];

    if (obj != NULL)
    {
        uint32_t irq_status = Cy_PDM_PCM_GetInterruptStatus(obj->base);
        Cy_PDM_PCM_ClearInterrupt(obj->base,  irq_status);

        cyhal_pdm_pcm_event_t event = _CYHAL_PDM_PCM_EVENT_NONE;
        if((CY_PDM_PCM_INTR_RX_TRIGGER & irq_status) || (CY_PDM_PCM_INTR_RX_OVERFLOW & irq_status))
        {
            if (obj->stabilized)
            {
                if (NULL != obj->async_buffer)
                {
                    if (obj->dma.resource.type == CYHAL_RSC_INVALID)
                    {
                        if (obj->async_read_remaining > 0)
                        {
                            _cyhal_pdm_pcm_try_read_async(obj);
                        }
                        if (obj->async_read_remaining == 0)
                        {
                            event |= CYHAL_PDM_PCM_ASYNC_COMPLETE;
                        }
                    }
                    else
                    {
                        if (obj->async_read_remaining > 0 && !cyhal_dma_is_busy(&(obj->dma)))
                        {
                            cy_rslt_t rslt = _cyhal_pdm_pcm_dma_start(obj);
                            CY_UNUSED_PARAMETER(rslt);
                            CY_ASSERT(CY_RSLT_SUCCESS == rslt);
                        }
                    }

                    if (obj->async_read_remaining == 0)
                    {
                        obj->async_buffer = NULL;
                        if (!(obj->irq_cause & CYHAL_PDM_PCM_RX_HALF_FULL))
                        {
                            // Only disable the interrupt mask if the user did not explicitly enable the mask
                            Cy_PDM_PCM_SetInterruptMask(obj->base, Cy_PDM_PCM_GetInterruptMask(obj->base) & ~CY_PDM_PCM_INTR_RX_TRIGGER);
                        }
                    }
                }
                if (CY_PDM_PCM_INTR_RX_TRIGGER & irq_status)
                {
                    event |= CYHAL_PDM_PCM_RX_HALF_FULL;
                }
            }
            else
            {
                // The PDM/PCM block alternates between left and right in stereo.
                // To preserve oddness and eveness of left and right, removes an even number of elements.
                for (int i = 0; i < _CYHAL_PDM_PCM_STABILIZATION_FS; i++)
                {
                    PDM_PCM_RX_FIFO_RD(obj->base);
                }
                _cyhal_pdm_pcm_set_rx_fifo_level(obj, _CYHAL_PDM_PCM_HALF_FIFO);
                if (!cyhal_pdm_pcm_is_pending(obj) && !(CYHAL_PDM_PCM_RX_HALF_FULL & obj->irq_cause))
                {
                    Cy_PDM_PCM_SetInterruptMask(obj->base, Cy_PDM_PCM_GetInterruptMask(obj->base) & ~CY_PDM_PCM_INTR_RX_TRIGGER);
                }
                obj->stabilized = true;
            }
        }

        if (CY_PDM_PCM_INTR_RX_NOT_EMPTY & irq_status)
        {
            event |= CYHAL_PDM_PCM_RX_NOT_EMPTY;
        }
        if (CY_PDM_PCM_INTR_RX_OVERFLOW & irq_status)
        {
            event |= CYHAL_PDM_PCM_RX_OVERFLOW;
        }
        if (CY_PDM_PCM_INTR_RX_UNDERFLOW & irq_status)
        {
            event |= CYHAL_PDM_PCM_RX_UNDERFLOW;
        }

        event &= obj->irq_cause;

        if (event != _CYHAL_PDM_PCM_EVENT_NONE)
        {
            cyhal_pdm_pcm_event_callback_t callback = (cyhal_pdm_pcm_event_callback_t) obj->callback_data.callback;
            if (callback != NULL)
            {
                callback(obj->callback_data.callback_arg, event);
            }
        }
    }
}

static inline bool _cyhal_pdm_pcm_invalid_gain_range(int8_t gain_value)
{
    return gain_value < CYHAL_PDM_PCM_MIN_GAIN || gain_value > CYHAL_PDM_PCM_MAX_GAIN;
}

static inline cy_en_pdm_pcm_gain_t _cyhal_pdm_pcm_scale_gain_value(int8_t gain_value)
{
    // The hardware use gain rate of 1.5 dB per register increment,
    // ranging from -12dB (register value 0x0) to 10.5dB (register value 0xF).
    // Need to scale dB range [-24, 21] to register range [0x0, 0xF]
    return (cy_en_pdm_pcm_gain_t) ((gain_value + 25) / 3);
}

static inline cy_rslt_t _cyhal_pdm_pcm_set_pdl_config_struct(const cyhal_pdm_pcm_cfg_t *cfg, cy_stc_pdm_pcm_config_t *pdl_config)
{
    // PDM_CKO = sample_rate * decimation_rate
    if (cfg->sample_rate > CYHAL_PDM_PCM_MAX_SAMPLE_RATE)
    {
        return CYHAL_PDM_PCM_RSLT_ERR_INVALID_CONFIG_PARAM;
    }
    uint32_t pdm_cko = cfg->sample_rate * cfg->decimation_rate;
    uint32_t hf1_freq = Cy_SysClk_ClkHfGetFrequency(_CYHAL_PDM_PCM_HFCLK);
    // need to use 3 clock dividers to divied hf1_freq to pdm_cko
    // divider 0 and 1 have values 1 to 4, divider 2 has values 2 to 16
    uint8_t best_div0 = 1, best_div1 = 1, best_div2 = 2;
    uint32_t min_error = UINT32_MAX;
    for (uint8_t div1 = 1; div1 <= 4; div1++)
    {
        // start divider 0 at divider 1 's current value
        // (div0, div1) = (2,3) is equivalent to (3,2)
        for (uint8_t div0 = div1; div0 <= 4; div0++)
        {
            uint32_t div01_freq = div0 * div1 * pdm_cko;
            for (uint8_t div2 = 2; div2 <= 16; div2++)
            {
                uint32_t computed_hfclk1_freq = div01_freq * div2;
                uint32_t error = computed_hfclk1_freq < hf1_freq ? hf1_freq - computed_hfclk1_freq : computed_hfclk1_freq - hf1_freq;
                if (error < min_error)
                {
                    best_div0 = div0;
                    best_div1 = div1;
                    best_div2 = div2;
                    min_error = error;
                }
            }
        }
    }
    pdl_config->clkDiv = (cy_en_pdm_pcm_clk_div_t)(best_div0 - 1);
    pdl_config->mclkDiv = (cy_en_pdm_pcm_clk_div_t)(best_div1 - 1);
    pdl_config->ckoDiv = best_div2 - 1;

    // sinc_rate = decimation_rate / 2
    // decimation rate is always valid. The max value for sync rate is 0x7F
    pdl_config->sincDecRate = (cfg->decimation_rate) / 2;

    switch(cfg->mode)
    {
        case CYHAL_PDM_PCM_MODE_LEFT:
            pdl_config->chanSelect = CY_PDM_PCM_OUT_CHAN_LEFT;
            break;
        case CYHAL_PDM_PCM_MODE_RIGHT:
            pdl_config->chanSelect = CY_PDM_PCM_OUT_CHAN_RIGHT;
            break;
        case CYHAL_PDM_PCM_MODE_STEREO:
            pdl_config->chanSelect = CY_PDM_PCM_OUT_STEREO;
            break;
        default:
            return CYHAL_PDM_PCM_RSLT_ERR_INVALID_CONFIG_PARAM;
    }

    switch(cfg->word_length)
    {
        case 16:
            pdl_config->wordLen = CY_PDM_PCM_WLEN_16_BIT;
            break;
        case 18:
            pdl_config->wordLen = CY_PDM_PCM_WLEN_18_BIT;
            break;
        case 20:
            pdl_config->wordLen = CY_PDM_PCM_WLEN_20_BIT;
            break;
        case 24:
            pdl_config->wordLen = CY_PDM_PCM_WLEN_24_BIT;
            break;
        default:
            return CYHAL_PDM_PCM_RSLT_ERR_INVALID_CONFIG_PARAM;
    }

    if (_cyhal_pdm_pcm_invalid_gain_range(cfg->left_gain) || _cyhal_pdm_pcm_invalid_gain_range(cfg->right_gain))
    {
        return CYHAL_PDM_PCM_RSLT_ERR_INVALID_CONFIG_PARAM;
    }
    pdl_config->gainLeft = _cyhal_pdm_pcm_scale_gain_value(cfg->left_gain);
    pdl_config->gainRight = _cyhal_pdm_pcm_scale_gain_value(cfg->right_gain);

    return CY_RSLT_SUCCESS;
}

static inline uint32_t _cyhal_pdm_pcm_get_pdl_event_mask(cyhal_pdm_pcm_event_t event)
{
    static const uint32_t status_map[] =
    {
        0u,                                     // Default, no value
        (uint32_t)CY_PDM_PCM_INTR_RX_TRIGGER,   // CYHAL_PDM_PCM_RX_HALF_FULL
        (uint32_t)CY_PDM_PCM_INTR_RX_NOT_EMPTY, // CYHAL_PDM_PCM_RX_NOT_EMPTY
        (uint32_t)CY_PDM_PCM_INTR_RX_OVERFLOW,  // CYHAL_PDM_PCM_RX_OVERFLOW
        (uint32_t)CY_PDM_PCM_INTR_RX_UNDERFLOW, // CYHAL_PDM_PCM_RX_UNDERFLOW
    };
    return _cyhal_utils_convert_flags(status_map, sizeof(status_map) / sizeof(uint32_t), (uint32_t)event);
}

static void _cyhal_pdm_pcm_dma_callback(void *callback_arg, cyhal_dma_event_t event)
{
    CY_UNUSED_PARAMETER(event);
    cyhal_pdm_pcm_t *obj = (cyhal_pdm_pcm_t *)callback_arg;
    if (obj != NULL)
    {
        // DMA finished trigger callback
        cyhal_pdm_pcm_event_callback_t callback = (cyhal_pdm_pcm_event_callback_t) obj->callback_data.callback;
        if (callback != NULL)
        {
            callback(obj->callback_data.callback_arg, CYHAL_PDM_PCM_ASYNC_COMPLETE);
        }
    }
}


cy_rslt_t cyhal_pdm_pcm_init(cyhal_pdm_pcm_t *obj, cyhal_gpio_t pin_data, cyhal_gpio_t pin_clk,
                const cyhal_clock_t *clk_source, const cyhal_pdm_pcm_cfg_t *cfg)
{
    CY_ASSERT(NULL != obj);
    memset(obj, 0, sizeof(cyhal_pdm_pcm_t));
    // On PSoC6, the PDM/PCM block is always driven from HFCLK1.
    CY_UNUSED_PARAMETER(clk_source);

    /* Explicitly marked not allocated resources as invalid to prevent freeing them. */
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->pin_data = CYHAL_NC_PIN_VALUE;
    obj->pin_clk = CYHAL_NC_PIN_VALUE;
    obj->dma.resource.type = CYHAL_RSC_INVALID;

    /* Reserve the PDM-PCM */
    const cyhal_resource_pin_mapping_t *data_map = _CYHAL_UTILS_GET_RESOURCE(pin_data, cyhal_pin_map_audioss_pdm_data);
    const cyhal_resource_pin_mapping_t *clk_map = _CYHAL_UTILS_GET_RESOURCE(pin_clk, cyhal_pin_map_audioss_pdm_clk);

    if ((NULL == data_map) || (NULL == clk_map) || !_cyhal_utils_resources_equal(data_map->inst, clk_map->inst))
    {
        return CYHAL_PDM_PCM_RSLT_ERR_INVALID_PIN;
    }

    obj->resource = *(data_map->inst);
    // There is only one PDM-PCM instance on PSoC6.
    obj->base = _cyhal_pdm_pcm_base[obj->resource.channel_num];

    cy_rslt_t result = cyhal_hwmgr_reserve(&(obj->resource));
    if (result != CY_RSLT_SUCCESS)
    {
        return result;
    }

    /* Reserve the pdm in pin */
    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_utils_reserve_and_connect(pin_data, data_map);
        if (result == CY_RSLT_SUCCESS)
            obj->pin_data = pin_data;
    }

    /* Reserve the clk pin */
    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_utils_reserve_and_connect(pin_clk, clk_map);
        if (result == CY_RSLT_SUCCESS)
            obj->pin_clk = pin_clk;
    }

    if (result == CY_RSLT_SUCCESS)
    {
        cy_stc_pdm_pcm_config_t pdl_struct = _cyhal_pdm_pcm_default_config;
        result = _cyhal_pdm_pcm_set_pdl_config_struct(cfg, &pdl_struct);
        if (result == CY_RSLT_SUCCESS)
        {
            result = (cy_rslt_t)Cy_PDM_PCM_Init(obj->base, &pdl_struct);
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        _cyhal_pdm_pcm_config_structs[obj->resource.channel_num] = obj;
        obj->word_size = cfg->word_length <= 16 ? 2 : 4;
        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = _CYHAL_PDM_PCM_EVENT_NONE;
        obj->stabilized = false;
        obj->pm_transition_ready = false;

        obj->pm_callback.callback = &_cyhal_pdm_pcm_pm_callback,
        obj->pm_callback.states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE);
        obj->pm_callback.next = NULL;
        obj->pm_callback.args = (void*)obj;
        obj->pm_callback.ignore_modes = CYHAL_SYSPM_BEFORE_TRANSITION;

        _cyhal_syspm_register_peripheral_callback(&(obj->pm_callback));

        cy_stc_sysint_t irqCfg = { _cyhal_pdm_pcm_irq_n[obj->resource.channel_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, &_cyhal_pdm_pcm_hw_irq_handler);
        NVIC_EnableIRQ(_cyhal_pdm_pcm_irq_n[obj->resource.channel_num]);
        cyhal_pdm_pcm_clear(obj);
    }

    if (result != CY_RSLT_SUCCESS)
    {
        cyhal_pdm_pcm_free(obj);
    }
    return result;
}

void cyhal_pdm_pcm_free(cyhal_pdm_pcm_t *obj)
{
    CY_ASSERT(NULL != obj);

    if (CYHAL_RSC_INVALID != obj->resource.type)
    {
        _cyhal_syspm_unregister_peripheral_callback(&(obj->pm_callback));
        Cy_PDM_PCM_DeInit(obj->base);
        NVIC_DisableIRQ(_cyhal_pdm_pcm_irq_n[obj->resource.channel_num]);

        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    _cyhal_utils_release_if_used(&(obj->pin_data));
    _cyhal_utils_release_if_used(&(obj->pin_clk));

    if (CYHAL_RSC_INVALID != obj->dma.resource.type)
    {
        cyhal_dma_free(&(obj->dma));
    }
}

cy_rslt_t cyhal_pdm_pcm_start(cyhal_pdm_pcm_t *obj)
{
    CY_ASSERT(NULL != obj);
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    obj->stabilized = false;
    // Remove any element currently in the FIFO. This ensure the correct stabilization time delay.
    Cy_PDM_PCM_ClearFifo(obj->base);
    Cy_PDM_PCM_Enable(obj->base);
    // After Enable is asserted, there is a transition period of about 35-45 sample cycles.
    _cyhal_pdm_pcm_set_rx_fifo_level(obj, _CYHAL_PDM_PCM_STABILIZATION_FS);
    Cy_PDM_PCM_SetInterruptMask(obj->base, Cy_PDM_PCM_GetInterruptMask(obj->base) | CY_PDM_PCM_INTR_RX_TRIGGER);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_pdm_pcm_stop(cyhal_pdm_pcm_t *obj)
{
    CY_ASSERT(NULL != obj);
    Cy_PDM_PCM_Disable(obj->base);
    return CY_RSLT_SUCCESS;
}

bool cyhal_pdm_pcm_is_enabled(cyhal_pdm_pcm_t *obj)
{
    CY_ASSERT(NULL != obj);
    return (0 != Cy_PDM_PCM_GetCurrentState(obj->base));
}

cy_rslt_t cyhal_pdm_pcm_set_gain(cyhal_pdm_pcm_t *obj, int8_t gain_left, int8_t gain_right)
{
    CY_ASSERT(NULL != obj);
    if (_cyhal_pdm_pcm_invalid_gain_range(gain_left) || _cyhal_pdm_pcm_invalid_gain_range(gain_right))
    {
        return CYHAL_PDM_PCM_RSLT_ERR_INVALID_CONFIG_PARAM;
    }
    Cy_PDM_PCM_SetGain(obj->base, CY_PDM_PCM_CHAN_LEFT, _cyhal_pdm_pcm_scale_gain_value(gain_left));
    Cy_PDM_PCM_SetGain(obj->base, CY_PDM_PCM_CHAN_RIGHT, _cyhal_pdm_pcm_scale_gain_value(gain_right));

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_pdm_pcm_clear(cyhal_pdm_pcm_t *obj)
{
    CY_ASSERT(NULL != obj);
    Cy_PDM_PCM_ClearFifo(obj->base);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_pdm_pcm_read(cyhal_pdm_pcm_t *obj, void *data, size_t *length)
{
    CY_ASSERT(NULL != obj);
    if (!(obj->stabilized))
    {
        *length = 0;
    }
    uint8_t fifo_count = Cy_PDM_PCM_GetNumInFifo(obj->base);
    if (*length > fifo_count)
    {
        *length = fifo_count;
    }
    size_t i;

    if (obj->word_size == 2)
    {
        uint16_t *buffer = (uint16_t *)data;
        for (i = 0; i < *length; i++)
        {
            buffer[i] = (Cy_PDM_PCM_ReadFifo(obj->base) & 0xFFFF);
        }
    }
    else
    {
        uint32_t *buffer = (uint32_t *)data;
        for (i = 0; i < *length; i++)
        {
            buffer[i] = (Cy_PDM_PCM_ReadFifo(obj->base));
        }
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_pdm_pcm_read_async(cyhal_pdm_pcm_t *obj, void *data, size_t length)
{
    CY_ASSERT(NULL != obj);
    if (obj->pm_transition_ready)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    if (cyhal_pdm_pcm_is_pending(obj))
    {
        return CYHAL_PDM_PCM_RSLT_ERR_ASYNC_IN_PROGRESS;
    }

    // Disable PDM interrupts temporarily.
    NVIC_DisableIRQ(_cyhal_pdm_pcm_irq_n[obj->resource.channel_num]);

    obj->async_buffer = data;
    obj->async_read_remaining = length;

    if (obj->stabilized)
    {
        if (obj->dma.resource.type == CYHAL_RSC_INVALID)
        {
            // read as much as we can, if there are left overs, then set interrupt flags
            _cyhal_pdm_pcm_try_read_async(obj);

            if (0 == obj->async_read_remaining)
            {
                cyhal_pdm_pcm_event_callback_t callback = (cyhal_pdm_pcm_event_callback_t) obj->callback_data.callback;
                if (callback != NULL)
                {
                    obj->async_buffer = NULL;
                    callback(obj->callback_data.callback_arg, CYHAL_PDM_PCM_ASYNC_COMPLETE);
                }
            }
        }
    }
    // Setup interrupt for FIFO half full.
    if (0 != obj->async_read_remaining)
    {
        Cy_PDM_PCM_SetInterruptMask(obj->base, Cy_PDM_PCM_GetInterruptMask(obj->base) | CY_PDM_PCM_INTR_RX_TRIGGER);
    }
    NVIC_EnableIRQ(_cyhal_pdm_pcm_irq_n[obj->resource.channel_num]);
    return CY_RSLT_SUCCESS;
}

bool cyhal_pdm_pcm_is_pending(cyhal_pdm_pcm_t *obj)
{
    CY_ASSERT(NULL != obj);
    return obj->async_read_remaining != 0 && obj->async_buffer != NULL;
}

cy_rslt_t cyhal_pdm_pcm_abort_async(cyhal_pdm_pcm_t *obj)
{
    CY_ASSERT(NULL != obj);
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->async_read_remaining = 0;
    obj->async_buffer = NULL;
    // Only disable the interrupt mask if the user did not explicitly enable the mask
    if (!(obj->irq_cause & CYHAL_PDM_PCM_RX_HALF_FULL))
    {
        Cy_PDM_PCM_SetInterruptMask(obj->base, Cy_PDM_PCM_GetInterruptMask(obj->base) & ~CY_PDM_PCM_INTR_RX_TRIGGER);
    }
    cyhal_system_critical_section_exit(savedIntrStatus);
    return CY_RSLT_SUCCESS;
}

void cyhal_pdm_pcm_register_callback(cyhal_pdm_pcm_t *obj, cyhal_pdm_pcm_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_pdm_pcm_enable_event(cyhal_pdm_pcm_t *obj, cyhal_pdm_pcm_event_t event, uint8_t intr_priority, bool enable)
{
    uint32_t mask = _cyhal_pdm_pcm_get_pdl_event_mask(event);
    if (enable)
    {
        obj->irq_cause |= event;
        Cy_PDM_PCM_ClearInterrupt(obj->base, mask);
        Cy_PDM_PCM_SetInterruptMask(obj->base, Cy_PDM_PCM_GetInterruptMask(obj->base) | mask);
    }
    else
    {
        obj->irq_cause &= ~event;
        uint32_t intr_status = cyhal_system_critical_section_enter();
        if (!obj->stabilized && cyhal_pdm_pcm_is_pending(obj))
        {
            // The half full event is used internally by the stabilization code.
            // The start() API clear the FIFO, if we have more data than the half FIFO, then PDM/PCM has stabilized.
            // This half interrupt mask will automatically cleared by the stabilization code.

            // Is an async operation is pending the mask will also be cleared automatically when the async operation finishes
            mask &= ~CY_PDM_PCM_INTR_RX_TRIGGER;
        }
        Cy_PDM_PCM_SetInterruptMask(obj->base, Cy_PDM_PCM_GetInterruptMask(obj->base) & ~mask);
        cyhal_system_critical_section_exit(intr_status);
    }

    NVIC_SetPriority(_cyhal_pdm_pcm_irq_n[obj->resource.channel_num], intr_priority);
}

cy_rslt_t cyhal_pdm_pcm_set_async_mode(cyhal_pdm_pcm_t *obj, cyhal_async_mode_t mode, uint8_t dma_priority)
{
    CY_ASSERT(NULL != obj);

    if (CYHAL_ASYNC_SW == mode)
    {
        if (CYHAL_DMA_PRIORITY_DEFAULT != dma_priority)
        {
            return CYHAL_PDM_PCM_RSLT_ERR_INVALID_CONFIG_PARAM;
        }
        if (CYHAL_RSC_INVALID != obj->dma.resource.type)
        {
            cyhal_dma_free(&(obj->dma));
            obj->dma.resource.type = CYHAL_RSC_INVALID;
        }
    }
    else if (CYHAL_ASYNC_DMA == mode && CYHAL_RSC_INVALID == obj->dma.resource.type)
    {
        cy_rslt_t rslt = cyhal_dma_init(&(obj->dma), dma_priority, CYHAL_DMA_DIRECTION_PERIPH2MEM);
        if (CY_RSLT_SUCCESS != rslt)
        {
            return rslt;
        }
        cyhal_dma_register_callback(&(obj->dma), &_cyhal_pdm_pcm_dma_callback, obj);
    }
    return CY_RSLT_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXAUDIOSS_INSTANCES */
