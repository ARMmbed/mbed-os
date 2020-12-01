/***************************************************************************/ /**
* \file cyhal_adc.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Analog/Digital
* convert. This interface abstracts out the chip specific details. If any chip
* specific functionality is necessary, or performance is critical the low level
* functions can be used directly.
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

/**
 * \addtogroup group_hal_impl_adc ADC (Analog Digital Converter)
 * \ingroup group_hal_impl
 * \{
 * \section cyhal_adc_impl_features Features
 * The PSoC 6 ADC supports the following features:
 * * Resolution: 12 bit
 * * Only @ref CYHAL_POWER_LEVEL_DEFAULT and CYHAL_POWER_LEVEL_OFF are defined. The default power
 *   level will automatically adjust based on smple rate.
 * * Average counts: 2, 4, 8, 16, 32, 64, 128, 256
 * * Up to four unique acquisition times.
 * * DMA-based transfer when using @ref cyhal_adc_read_async. When using @ref cyhal_adc_read_async_uv,
 *   only interrupt-driven software copy is supported.
 * \} group_hal_impl_adc
 */

#include <cmsis_compiler.h>

#include "cyhal_adc.h"
#include "cyhal_analog_common.h"
#include "cyhal_clock.h"
#include "cyhal_dma.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_utils.h"
#include "cyhal_syspm.h"
#include "cyhal_system.h"

#if defined(CY_IP_MXS40PASS_SAR_INSTANCES)

#if defined(__cplusplus)
extern "C"
{
#endif

static SAR_Type *const _cyhal_adc_base[] =
{
#if (CY_IP_MXS40PASS_SAR_INSTANCES == 1)
    SAR,
#elif (CY_IP_MXS40PASS_SAR_INSTANCES == 2)
    SAR0,
    SAR1,
#else
    #warning Unhandled SAR instance count
#endif
};

static const en_clk_dst_t _cyhal_adc_clock[] =
{
#if (CY_IP_MXS40PASS_SAR_INSTANCES == 1)
    PCLK_PASS_CLOCK_SAR,
#elif (CY_IP_MXS40PASS_SAR_INSTANCES == 2)
    PCLK_PASS_CLOCK_SAR0,
    PCLK_PASS_CLOCK_SAR1,
#else
    #warning Unhandled SAR instance count
#endif
};

static cyhal_adc_t* _cyhal_adc_config_structs[CY_IP_MXS40PASS_SAR_INSTANCES];

static const IRQn_Type _cyhal_adc_irq_n[] =
{
#if (CY_IP_MXS40PASS_SAR_INSTANCES == 1)
    pass_interrupt_sar_IRQn,
#elif (CY_IP_MXS40PASS_SAR_INSTANCES == 2)
    pass_interrupt_sar_0_IRQn,
    pass_interrupt_sar_1_IRQn,
#else
    #warning Unhandled SAR instance count
#endif
};

static uint8_t _cyhal_adc_get_block_from_irqn(IRQn_Type irqn)
{
    switch (irqn)
    {
#if (CY_CPU_CORTEX_M4)
#if (CY_IP_MXS40PASS_SAR_INSTANCES == 1)
    case pass_interrupt_sar_IRQn:
        return 0;
#elif (CY_IP_MXS40PASS_SAR_INSTANCES == 2)
    case pass_interrupt_sar_0_IRQn:
        return 0;
    case pass_interrupt_sar_1_IRQn:
        return 1;
#else
    #warning Unhandled SAR instance count
#endif
#endif /* (CY_CPU_CORTEX_M4) */
    default:
        CY_ASSERT(false); // Should never be called with a non-SAR IRQn
        return 0;
    }
}

/* OR in the following user-configurable values: vref, bypass, vneg, */
#define _CYHAL_ADC_DEFAULT_CTRL ((uint32_t)CY_SAR_VREF_PWR_100 | (uint32_t)CY_SAR_VREF_SEL_BGR \
                                | (uint32_t)CY_SAR_BYPASS_CAP_DISABLE | (uint32_t)CY_SAR_CTRL_NEGVREF_HW \
                                | (uint32_t)CY_SAR_CTRL_COMP_DLY_12   | (uint32_t)CY_SAR_COMP_PWR_100 \
                                | (uint32_t)CY_SAR_DEEPSLEEP_SARMUX_OFF | (uint32_t)CY_SAR_SARSEQ_SWITCH_ENABLE)

/* Default configuration. OR in the average count, and average mode */
#define _CYHAL_ADC_DEFAULT_SAMPLE ((uint32_t)CY_SAR_RIGHT_ALIGN | (uint32_t)CY_SAR_TRIGGER_MODE_FW_ONLY \
                                 | (uint32_t)CY_SAR_SINGLE_ENDED_SIGNED | (uint32_t)CY_SAR_DIFFERENTIAL_SIGNED \
                                 | (uint32_t)CY_SAR_TRIGGER_MODE_FW_ONLY)
#define _CYHAL_ADC_RESOLUTION 12u
#define _CYHAL_ADC_INTERNAL_VREF_MV 1200UL
#define _CYHAL_ADC_CONVERSION_CYCLES (_CYHAL_ADC_RESOLUTION + 2)

static const cyhal_adc_config_t _CYHAL_ADC_DEFAULT_CONFIG =
{
    .resolution          = _CYHAL_ADC_RESOLUTION,
    .average_count       = 1,
    .average_mode_flags  = CYHAL_ADC_AVG_MODE_AVERAGE,
    .continuous_scanning = true,
    .vneg                = CYHAL_ADC_VNEG_VREF,
    .vref                = CYHAL_ADC_REF_INTERNAL,
    .ext_vref            = NC,
    .ext_vref_mv         = 0u,
    .is_bypassed         = false,
    .bypass_pin          = NC,
};

static const cy_stc_sar_config_t _CYHAL_ADC_DEFAULT_PDL_CONFIG =
{
    /* .ctrl is populated from _CYHAL_ADC_DEFAULT_CTRL plus the user's configuration */
    /* .sampleCtrl is puopulated from _CYHAL_ADC_DEFAULT_SAMPLE plus the user's configuration */
    .sampleTime01 = (10UL << CY_SAR_SAMPLE_TIME0_SHIFT), // Sample times 1, 2, and 3 are not used
    .sampleTime23 = 0UL,
    .rangeThres = (0UL << CY_SAR_RANGE_HIGH_SHIFT) | (0UL << CY_SAR_RANGE_LOW_SHIFT),
    .rangeCond = CY_SAR_RANGE_COND_BELOW,
    .chanEn = 0UL,
    .chanConfig = { 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL},
    .intrMask = (uint32_t) CY_SAR_INTR_EOS_MASK,
    .satIntrMask = 0UL,
    .rangeIntrMask = 0UL,
    .muxSwitch = 0UL,
    .muxSwitchSqCtrl = 0UL,
    .configRouting = true,
    /* .vrefMvValue is populated from the user's configuration */
};

/*******************************************************************************
*       Internal helper functions
*******************************************************************************/
static uint8_t _cyhal_adc_max_configured_channel(const cyhal_adc_t* obj)
{
    uint8_t max = 0;
    for(uint8_t i = 0; i < CY_SAR_MAX_NUM_CHANNELS; ++i)
    {
        if(NULL != obj->channel_config[i])
        {
            max = i;
        }
    }
    return max;
}

static uint32_t _cyhal_adc_get_mux_switch_control(cyhal_gpio_t gpio)
{
    static const cy_en_sar_mux_switch_sq_ctrl_t mux_lookup[] =
    {
        CY_SAR_MUX_SQ_CTRL_P0,
        CY_SAR_MUX_SQ_CTRL_P1,
        CY_SAR_MUX_SQ_CTRL_P2,
        CY_SAR_MUX_SQ_CTRL_P3,
        CY_SAR_MUX_SQ_CTRL_P4,
        CY_SAR_MUX_SQ_CTRL_P5,
        CY_SAR_MUX_SQ_CTRL_P6,
        CY_SAR_MUX_SQ_CTRL_P7
    };
    uint8_t pin = CYHAL_GET_PIN(gpio);
    CY_ASSERT(pin < sizeof(mux_lookup)/sizeof(mux_lookup[0]));
    return (uint32_t)mux_lookup[pin];
}

static uint32_t _cyhal_adc_get_fw_switch_control(cyhal_gpio_t gpio, bool is_vplus)
{
    static const cy_en_sar_mux_switch_fw_ctrl_t vplus_lookup[] =
    {
        CY_SAR_MUX_FW_P0_VPLUS,
        CY_SAR_MUX_FW_P1_VPLUS,
        CY_SAR_MUX_FW_P2_VPLUS,
        CY_SAR_MUX_FW_P3_VPLUS,
        CY_SAR_MUX_FW_P4_VPLUS,
        CY_SAR_MUX_FW_P5_VPLUS,
        CY_SAR_MUX_FW_P6_VPLUS,
        CY_SAR_MUX_FW_P7_VPLUS
    };

    static const cy_en_sar_mux_switch_fw_ctrl_t vminus_lookup[] =
    {
        CY_SAR_MUX_FW_P0_VMINUS,
        CY_SAR_MUX_FW_P1_VMINUS,
        CY_SAR_MUX_FW_P2_VMINUS,
        CY_SAR_MUX_FW_P3_VMINUS,
        CY_SAR_MUX_FW_P4_VMINUS,
        CY_SAR_MUX_FW_P5_VMINUS,
        CY_SAR_MUX_FW_P6_VMINUS,
        CY_SAR_MUX_FW_P7_VMINUS
    };

    uint8_t pin = CYHAL_GET_PIN(gpio);
    CY_ASSERT(pin < sizeof(vplus_lookup)/sizeof(vplus_lookup[0]));
    return (uint32_t)(is_vplus ? vplus_lookup[pin] : vminus_lookup[pin]);
}

static uint32_t _cyhal_adc_get_pin_addr(cyhal_gpio_t gpio, bool is_vplus)
{
    static const cy_en_sar_chan_config_pos_pin_addr_t vplus_lookup[] =
    {
        CY_SAR_CHAN_POS_PIN_ADDR_0,
        CY_SAR_CHAN_POS_PIN_ADDR_1,
        CY_SAR_CHAN_POS_PIN_ADDR_2,
        CY_SAR_CHAN_POS_PIN_ADDR_3,
        CY_SAR_CHAN_POS_PIN_ADDR_4,
        CY_SAR_CHAN_POS_PIN_ADDR_5,
        CY_SAR_CHAN_POS_PIN_ADDR_6,
        CY_SAR_CHAN_POS_PIN_ADDR_7
    };

    static const cy_en_sar_chan_config_neg_pin_addr_t vminus_lookup[] =
    {
        CY_SAR_CHAN_NEG_PIN_ADDR_0,
        CY_SAR_CHAN_NEG_PIN_ADDR_1,
        CY_SAR_CHAN_NEG_PIN_ADDR_2,
        CY_SAR_CHAN_NEG_PIN_ADDR_3,
        CY_SAR_CHAN_NEG_PIN_ADDR_4,
        CY_SAR_CHAN_NEG_PIN_ADDR_5,
        CY_SAR_CHAN_NEG_PIN_ADDR_6,
        CY_SAR_CHAN_NEG_PIN_ADDR_7
    };
    uint8_t pin = CYHAL_GET_PIN(gpio);
    CY_ASSERT(pin < sizeof(vplus_lookup)/sizeof(vplus_lookup[0]));
    return is_vplus ? (uint32_t)vplus_lookup[pin] : (uint32_t)vminus_lookup[pin];
}

static uint32_t _cyhal_adc_convert_vref(cyhal_adc_vref_t vref)
{
    switch(vref)
    {
        case CYHAL_ADC_REF_INTERNAL:
            return CY_SAR_VREF_SEL_BGR;
        case CYHAL_ADC_REF_EXTERNAL:
            return CY_SAR_VREF_SEL_EXT;
        case CYHAL_ADC_REF_VDDA:
            return CY_SAR_VREF_SEL_VDDA;
        case CYHAL_ADC_REF_VDDA_DIV_2:
            return CY_SAR_VREF_SEL_VDDA_DIV_2;
        default:
            CY_ASSERT(false);
            return CY_SAR_VREF_SEL_BGR;
    }
}

static uint32_t _cyhal_adc_convert_average_mode(uint32_t average_mode_flags)
{
    uint32 result = 0;
    if(0u != (average_mode_flags & CYHAL_ADC_AVG_MODE_ACCUMULATE))
    {
        result |= CY_SAR_AVG_MODE_SEQUENTIAL_ACCUM;
    }
    else if(0u != (average_mode_flags & CYHAL_ADC_AVG_MODE_INTERLEAVED))
    {
        /* INTERLEAVED on its own does not divide the result back down */
        result |= (CY_SAR_AVG_MODE_INTERLEAVED | SAR_SAMPLE_CTRL_AVG_SHIFT_Msk);
    }
    else
    {
        result |= CY_SAR_AVG_MODE_SEQUENTIAL_FIXED;
    }
    return result;
}

static cy_rslt_t _cyhal_adc_convert_average_count(uint32_t hal_count, uint32_t* pdl_count)
{
    switch(hal_count)
    {
        case 1: /* Average count of 1 is achieved by disabling averaging for all channels */
        case 2:
            *pdl_count = CY_SAR_AVG_CNT_2;
            return CY_RSLT_SUCCESS;
        case 4:
            *pdl_count = CY_SAR_AVG_CNT_4;
            return CY_RSLT_SUCCESS;
        case 8:
            *pdl_count = CY_SAR_AVG_CNT_8;
            return CY_RSLT_SUCCESS;
        case 16:
            *pdl_count = CY_SAR_AVG_CNT_16;
            return CY_RSLT_SUCCESS;
        case 32:
            *pdl_count = CY_SAR_AVG_CNT_32;
            return CY_RSLT_SUCCESS;
        case 64:
            *pdl_count = CY_SAR_AVG_CNT_64;
            return CY_RSLT_SUCCESS;
        case 128:
            *pdl_count = CY_SAR_AVG_CNT_128;
            return CY_RSLT_SUCCESS;
        case 256:
            *pdl_count = CY_SAR_AVG_CNT_256;
            return CY_RSLT_SUCCESS;
        default:
            return CYHAL_ADC_RSLT_BAD_ARGUMENT;
    }
}

static uint32_t _cyhal_adc_get_vref_mv(const cyhal_adc_config_t* hal_config)
{
    switch(hal_config->vref)
    {
        case CYHAL_ADC_REF_INTERNAL:
            return _CYHAL_ADC_INTERNAL_VREF_MV;
        case CYHAL_ADC_REF_EXTERNAL:
            CY_ASSERT(hal_config->ext_vref_mv > 0); // Should have been error checked already
            return hal_config->ext_vref_mv;
        case CYHAL_ADC_REF_VDDA_DIV_2:
            return cyhal_syspm_get_supply_voltage(CYHAL_VOLTAGE_SUPPLY_VDDA) / 2;
        default:
            CY_ASSERT(CYHAL_ADC_REF_VDDA == hal_config->vref);
            return cyhal_syspm_get_supply_voltage(CYHAL_VOLTAGE_SUPPLY_VDDA);
    }
}

/* Populates the PDL config struct with settings from the ADC config struct */
static cy_rslt_t _cyhal_adc_populate_pdl_config(const cyhal_adc_config_t* hal_config, cy_stc_sar_config_t* pdl_config)
{
    memset(pdl_config, 0, sizeof(cy_stc_sar_config_t));
    if(hal_config->resolution != _CYHAL_ADC_RESOLUTION /* PSoC 6 SAR does not support configurable resolution */
       || hal_config->bypass_pin != NC /* No P6 parts presently with GPIO for external vref */
       || hal_config->ext_vref != NC)
    {
        return CYHAL_ADC_RSLT_BAD_ARGUMENT;
    }

    if (((hal_config->average_mode_flags & CYHAL_ADC_AVG_MODE_ACCUMULATE) > 0) &&
        ((hal_config->average_mode_flags & CYHAL_ADC_AVG_MODE_INTERLEAVED) > 0))
    {
        /* Accumulate mode is not compatible with interleaved averaging */
        return CYHAL_ADC_RSLT_BAD_ARGUMENT;
    }

    if(false == ((CYHAL_ADC_REF_EXTERNAL == hal_config->vref) ^ (0u == hal_config->ext_vref_mv)))
    {
        /* Must have exactly one of: ext vref selected, ext vref voltage unspecified */
        return CYHAL_ADC_RSLT_BAD_ARGUMENT;
    }

    *pdl_config = _CYHAL_ADC_DEFAULT_PDL_CONFIG;
    uint32_t ctrl = _CYHAL_ADC_DEFAULT_CTRL;
    ctrl |= _cyhal_adc_convert_vref(hal_config->vref);
    ctrl |= (hal_config->is_bypassed) ? CY_SAR_BYPASS_CAP_ENABLE : CY_SAR_BYPASS_CAP_DISABLE;
    ctrl |= (hal_config->vneg == CYHAL_ADC_VNEG_VSSA) ? CY_SAR_NEG_SEL_VSSA_KELVIN : CY_SAR_NEG_SEL_VREF;

    uint32_t sample_ctrl = _CYHAL_ADC_DEFAULT_SAMPLE;
    uint32_t pdl_avg;
    cy_rslt_t result = _cyhal_adc_convert_average_count(hal_config->average_count, &pdl_avg);
    if(CY_RSLT_SUCCESS == result)
    {
        sample_ctrl |= pdl_avg;
        sample_ctrl |= _cyhal_adc_convert_average_mode(hal_config->average_mode_flags);

        pdl_config->ctrl = ctrl;
        pdl_config->sampleCtrl = sample_ctrl;
        pdl_config->vrefMvValue = _cyhal_adc_get_vref_mv(hal_config);
    }
    return result;
}

static void _cyhal_adc_irq_handler(void)
{
    /* The only enabled event is scan finished */
    cyhal_adc_event_t hal_event = CYHAL_ADC_EOS;

    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block = _cyhal_adc_get_block_from_irqn(irqn);
    cyhal_adc_t* obj = _cyhal_adc_config_structs[block];
    Cy_SAR_ClearInterrupt(obj->base, CY_SAR_INTR_EOS_MASK);
    obj->conversion_complete = true;

    uint8_t num_channels = _cyhal_adc_max_configured_channel(obj) + 1;
    if(obj->async_scans_remaining > 0)
    {
        /* Can't read millivolts out via DMA */
        if(CYHAL_ASYNC_SW == obj->async_mode || obj->async_transfer_in_uv)
        {
            for(int i = 0; i < num_channels; ++i)
            {
                int32_t counts = Cy_SAR_GetResult32(obj->base, i);
                *obj->async_buff_next = obj->async_transfer_in_uv ? Cy_SAR_CountsTo_uVolts(obj->base, i, counts) : counts;
                ++obj->async_buff_next;
            }
            --(obj->async_scans_remaining);

            if(0 == obj->async_scans_remaining)
            {
                obj->async_buff_next = obj->async_buff_orig = NULL;
                hal_event |= CYHAL_ADC_ASYNC_READ_COMPLETE;
            }
            else if(false == obj->continuous_scanning)
            {
                Cy_SAR_StartConvert(obj->base, CY_SAR_START_CONVERT_SINGLE_SHOT);
            }
            /* If we're continously scanning, another scan will be kicked off automatically
             * so we don't need to do anything */
        }
        else
        {
            CY_ASSERT(CYHAL_ASYNC_DMA == obj->async_mode);
            cyhal_dma_cfg_t dma_config =
            {
                .src_addr = (uint32_t)obj->base->CHAN_RESULT,
                .src_increment = 1u,
                .dst_addr = (uint32_t)obj->async_buff_next,
                .dst_increment = 1u,
                .transfer_width = 16u,
                .length = num_channels,
                .burst_size = 0u,
                .action = CYHAL_DMA_TRANSFER_FULL
            };

            /* The upper half word of the result are mirrored status bits.
             * We don't want to copy these into the result buffer, so we want to do
             * 16-bit transfers and skip every other half-word. But the peripheral registers
             * only support 32-bit reads, so we need to tell the DMA to read 4 bytes but only
             * transfer 2. The HAL API doesn't support this right now, so we have to manipulate the
             * underlying descriptor directly. */
#if defined(CY_IP_M4CPUSS_DMA)
            if(obj->dma.resource.type == CYHAL_RSC_DW)
            {
                obj->dma.descriptor_config.dw.srcTransferSize = CY_DMA_TRANSFER_SIZE_WORD;
                obj->dma.descriptor_config.dw.dstTransferSize = CY_DMA_TRANSFER_SIZE_WORD;
            }
#endif
#if defined(CY_IP_M4CPUSS_DMAC)
            if(obj->dma.resource.type == CYHAL_RSC_DMA)
            {
                obj->dma.descriptor_config.dmac.srcTransferSize = CY_DMAC_TRANSFER_SIZE_WORD;
                obj->dma.descriptor_config.dmac.dstTransferSize = CY_DMAC_TRANSFER_SIZE_WORD;
            }
#endif
            // Configure needs to happen after we've manipulated the descriptor config
            cy_rslt_t result = cyhal_dma_configure(&(obj->dma), &dma_config);
            if(CY_RSLT_SUCCESS == result)
            {
                result = cyhal_dma_start_transfer(&(obj->dma));
            }
            CY_ASSERT(CY_RSLT_SUCCESS == result);

            /* Don't increment the buffer here - do that when the DMA completes */

            if(false == obj->continuous_scanning)
            {
                Cy_SAR_StartConvert(obj->base, CY_SAR_START_CONVERT_SINGLE_SHOT);
            }
        }
    }

    if(0 != (hal_event & ((cyhal_adc_event_t)obj->user_enabled_events)))
    {
        cyhal_adc_event_callback_t callback = (cyhal_adc_event_callback_t)obj->callback_data.callback;
        if(NULL != callback)
        {
            callback(obj->callback_data.callback_arg, (cyhal_adc_event_t)(hal_event & obj->user_enabled_events));
        }
    }

}

static void _cyhal_adc_dma_handler(void* arg, cyhal_dma_event_t event)
{
    CY_ASSERT(CYHAL_DMA_TRANSFER_COMPLETE == event);
    CY_UNUSED_PARAMETER(event);
    cyhal_adc_t* obj = (cyhal_adc_t*)arg;
    CY_ASSERT(CYHAL_ASYNC_DMA == obj->async_mode);

    uint8_t num_channels = _cyhal_adc_max_configured_channel(obj) + 1;
    CY_ASSERT(false == obj->async_transfer_in_uv);
    obj->async_buff_next += num_channels;
    --(obj->async_scans_remaining);

    if(0 == obj->async_scans_remaining)
    {
        // DMA doesn't sign extend when we copy from 16 to 32 bits, so do the sign extension
        // ourselves once all channel scans are complete.
        while(obj->async_buff_orig != obj->async_buff_next)
        {
            int16_t sar_result = (int16_t)*(obj->async_buff_orig);
            *(obj->async_buff_orig) = sar_result;
            ++(obj->async_buff_orig);
        }
        obj->async_buff_next = obj->async_buff_orig = NULL;
        if(0 != (CYHAL_ADC_ASYNC_READ_COMPLETE & ((cyhal_adc_event_t)obj->user_enabled_events)))
        {
            cyhal_adc_event_callback_t callback = (cyhal_adc_event_callback_t)obj->callback_data.callback;
            if(NULL != callback)
            {
                callback(obj->callback_data.callback_arg, CYHAL_ADC_ASYNC_READ_COMPLETE);
            }
        }
    }
}

/*******************************************************************************
*       ADC HAL Functions
*******************************************************************************/

cy_rslt_t cyhal_adc_init(cyhal_adc_t *obj, cyhal_gpio_t pin, const cyhal_clock_t *clk)
{
    const uint32_t DESIRED_DIVIDER = 8000000u; // 8 MHz. Required range is 1.7 - 18

    CY_ASSERT(NULL != obj);

    cy_rslt_t result = CY_RSLT_SUCCESS;

    memset(obj, 0, sizeof(cyhal_adc_t));
    obj->clock.div_num = CYHAL_RSC_INVALID;
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->async_mode = CYHAL_ASYNC_SW;

    const cyhal_resource_pin_mapping_t* map = _cyhal_utils_try_alloc(pin, cyhal_pin_map_pass_sarmux_pads, sizeof(cyhal_pin_map_pass_sarmux_pads) / sizeof(cyhal_pin_map_pass_sarmux_pads[0]));

    if (NULL == map)
        result = CYHAL_ADC_RSLT_BAD_ARGUMENT;

    cyhal_resource_inst_t adc_inst;
    if (CY_RSLT_SUCCESS == result)
    {
        adc_inst = *map->inst;
        /* No need to reserve - try_alloc did so for us already */
    }

    en_clk_dst_t pclk = (en_clk_dst_t)0;
    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource = adc_inst;

        obj->base = _cyhal_adc_base[adc_inst.block_num];
        pclk = (en_clk_dst_t)(_cyhal_adc_clock[adc_inst.block_num]);
        if (NULL != clk)
        {
            obj->clock = *clk;
            obj->dedicated_clock = false;
        }
        else if (CY_RSLT_SUCCESS ==
            (result = _cyhal_utils_allocate_clock(&(obj->clock), &(obj->resource), CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT, true)))
        {
            obj->dedicated_clock = true;
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, obj->clock.div_type, obj->clock.div_num))
            result = CYHAL_ADC_RSLT_FAILED_CLOCK;
    }

    if (CY_RSLT_SUCCESS == result)
    {
        if(obj->dedicated_clock)
        {
            uint32_t div = Cy_SysClk_ClkPeriGetFrequency() / DESIRED_DIVIDER;
            if (0 == div ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, div - 1) ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num))
            {
                result = CYHAL_ADC_RSLT_FAILED_CLOCK;
            }
        }
    }

    cy_stc_sar_config_t pdl_config;
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_adc_populate_pdl_config(&_CYHAL_ADC_DEFAULT_CONFIG, &pdl_config);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        result = (cy_rslt_t)Cy_SAR_Init(obj->base, &pdl_config);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        Cy_SAR_SetVssaSarSeqCtrl(obj->base, CY_SAR_SWITCH_SEQ_CTRL_ENABLE);
        Cy_SAR_SetVssaVminusSwitch(obj->base, CY_SAR_SWITCH_CLOSE);

        _cyhal_analog_init();

        _cyhal_adc_config_structs[obj->resource.block_num] = obj;
        cy_stc_sysint_t irqCfg = { _cyhal_adc_irq_n[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_adc_irq_handler);
        NVIC_EnableIRQ(_cyhal_adc_irq_n[obj->resource.block_num]);

        /* We always need to listen to the EOS interrupt for internal bookkeeping */
        Cy_SAR_SetInterruptMask(obj->base, CY_SAR_INTR_EOS_MASK);
        Cy_SAR_Enable(obj->base);
    }
    else
    {
        cyhal_adc_free(obj);
    }
    return result;
}

void cyhal_adc_free(cyhal_adc_t *obj)
{
    if (NULL != obj && NULL != obj->base)
    {
        IRQn_Type irqn = _cyhal_adc_irq_n[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);
        _cyhal_adc_config_structs[obj->resource.block_num] = NULL;

        Cy_SAR_SetVssaSarSeqCtrl(obj->base, CY_SAR_SWITCH_SEQ_CTRL_DISABLE);
        Cy_SAR_SetVssaVminusSwitch(obj->base, CY_SAR_SWITCH_OPEN);
        Cy_SAR_Disable(obj->base);

        if(obj->dedicated_clock)
        {
            Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);
            cyhal_hwmgr_free_clock(&obj->clock);
        }

        _cyhal_analog_free();
        cyhal_hwmgr_free(&obj->resource);
        obj->base = NULL;
    }
}

cy_rslt_t _cyhal_adc_populate_acquisition_timers(cyhal_adc_t* obj)
{
    const uint32_t ACQUISITION_CLOCKS_MIN = 2;
    const uint32_t ACQUISITION_CLOCKS_MAX = 1023;

    cy_rslt_t result = CY_RSLT_SUCCESS;

    uint32_t clock_frequency_hz = cyhal_clock_get_frequency(&(obj->clock));
    uint32_t clock_period_ns = (clock_frequency_hz > 0)
        ? _CYHAL_UTILS_NS_PER_SECOND / clock_frequency_hz
        : 0;
    uint16_t sample_timer_ns[] = { 0u, 0u, 0u, 0u };
    uint8_t assigned_timer[CY_SAR_MAX_NUM_CHANNELS];
    for(uint8_t channel = 0; channel < CY_SAR_MAX_NUM_CHANNELS; ++channel)
    {
        cyhal_adc_channel_t* chan_config = obj->channel_config[channel];
        assigned_timer[channel] = 0u;
        /* If the channel isn't in use, what we select doesn't matter */
        if(NULL != chan_config)
        {
            bool found = false;
            for(uint8_t timer = 0; timer < sizeof(sample_timer_ns) / sizeof(sample_timer_ns[0]); ++timer)
            {
                if(chan_config->minimum_acquisition_ns == sample_timer_ns[timer])
                {
                    /* Matched a pre-existing timer; use that */
                    assigned_timer[channel] = timer;
                    found = true;
                    break;
                }
                else if(0 == sample_timer_ns[timer])
                {
                    /* Found a free timer - allocate and use that */
                    sample_timer_ns[timer] = chan_config->minimum_acquisition_ns;
                    assigned_timer[channel] = timer;
                    found = true;
                    break;
                }
            }

            if(false == found)
            {
                /* Ran out of acquisition timers */
                result = CYHAL_ADC_RSLT_BAD_ARGUMENT;
            }
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        uint16_t sample_timer_clocks[sizeof(sample_timer_ns) / sizeof(sample_timer_ns[0])];
        for(uint8_t i = 0; i < sizeof(sample_timer_clocks) / sizeof(sample_timer_clocks[0]); ++i)
        {
            /* Convert from nanoseconds to clock cycles, rounding up */
            uint32_t clock_cycles = (sample_timer_ns[i] + (clock_period_ns / 2)) / clock_period_ns;
            if(clock_cycles < ACQUISITION_CLOCKS_MIN)
            {
                clock_cycles = ACQUISITION_CLOCKS_MIN;
            }
            else if(clock_cycles > ACQUISITION_CLOCKS_MAX)
            {
                clock_cycles = ACQUISITION_CLOCKS_MAX;
            }
            /* Per the register map, this should be one greater than the actual desired sampling cycle count */
            sample_timer_clocks[i] = clock_cycles + 1;
        }

        obj->base->SAMPLE_TIME01 =
            (sample_timer_clocks[0] << CY_SAR_SAMPLE_TIME0_SHIFT) | (sample_timer_clocks[1] << CY_SAR_SAMPLE_TIME1_SHIFT);
        obj->base->SAMPLE_TIME23 =
            (sample_timer_clocks[2] << CY_SAR_SAMPLE_TIME2_SHIFT) | (sample_timer_clocks[3] << CY_SAR_SAMPLE_TIME3_SHIFT);

        for(uint8_t i = 0; i < CY_SAR_MAX_NUM_CHANNELS; ++i)
        {
            obj->base->CHAN_CONFIG[i] &= ~SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Msk;
            obj->base->CHAN_CONFIG[i] |= assigned_timer[i] << SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Pos;
        }
    }

    return result;
}


cy_rslt_t cyhal_adc_configure(cyhal_adc_t *obj, const cyhal_adc_config_t *config)
{
    cy_stc_sar_config_t pdl_config;
    cy_rslt_t result = _cyhal_adc_populate_pdl_config(config, &pdl_config);
    if(CY_RSLT_SUCCESS == result)
    {
        /* Save and restore channel configs */
        pdl_config.chanEn = obj->base->CHAN_EN;
        for(uint8_t i = 0; i < CY_SAR_MAX_NUM_CHANNELS; ++i)
        {
            pdl_config.chanConfig[i] = obj->base->CHAN_CONFIG[i];
        }
        /* Don't deinit routing - we're going to turn the SAR back on in a minute */
        pdl_config.configRouting = false;
        result = (cy_rslt_t)Cy_SAR_Init(obj->base, &pdl_config);
        _cyhal_adc_populate_acquisition_timers(obj);
        Cy_SAR_SetInterruptMask(obj->base, CY_SAR_INTR_EOS_MASK);
        Cy_SAR_Enable(obj->base);
    }

    if(obj->continuous_scanning)
    {
        obj->conversion_complete = false;
        Cy_SAR_StartConvert(obj->base, CY_SAR_START_CONVERT_CONTINUOUS);
    }
    else
    {
        Cy_SAR_StopConvert(obj->base);
    }

    return result;
}

cy_rslt_t cyhal_adc_set_power(cyhal_adc_t *obj, cyhal_power_level_t power)
{
    // The SAR doesn't have selectable power levels in the same way that the opamps do.
    if(CYHAL_POWER_LEVEL_OFF == power)
    {
        Cy_SAR_Disable(obj->base);
    }
    else
    {
        Cy_SAR_Enable(obj->base);
    }
    return CY_RSLT_SUCCESS;
}

static uint16_t _cyhal_adc_get_average_count(cyhal_adc_t* obj, int channel_idx)
{
    uint32_t average_count = 1;
    /* If averaging is in interleaved mode, it does not impact the sample time */
    if(CY_SAR_AVG_MODE_INTERLEAVED != (SAR_SAMPLE_CTRL(obj->base) & SAR_SAMPLE_CTRL_AVG_MODE_Msk))
    {
        average_count = (SAR_SAMPLE_CTRL(obj->base) & SAR_SAMPLE_CTRL_AVG_CNT_Msk) >> SAR_SAMPLE_CTRL_AVG_CNT_Pos;
        average_count = (1uL << (average_count + 1uL));
    }

    return (obj->base->CHAN_CONFIG[channel_idx] & SAR_CHAN_CONFIG_AVG_EN_Msk) ? average_count : 1;
}

/* Gets acquisition times and conversion clocks for all enabled channels, factoring in averaging */
static void _cyhal_adc_get_sample_times(cyhal_adc_t* obj, uint32_t* min_acquisition_ns, uint32_t* conversion_clock_cycles)
{
    *min_acquisition_ns = *conversion_clock_cycles = 0;
    for(uint8_t i = 0; i < CY_SAR_MAX_NUM_CHANNELS; ++i)
    {
        cyhal_adc_channel_t* chan_config = obj->channel_config[i];
        if(NULL != chan_config && (0u != (obj->base->CHAN_EN & 1 << i)))
        {
            uint32_t min_time = chan_config->minimum_acquisition_ns;
            uint8_t clock_cycles = _CYHAL_ADC_CONVERSION_CYCLES;
            uint16_t average_count = _cyhal_adc_get_average_count(obj, i);
            min_time *= average_count;
            clock_cycles *= average_count;

            *min_acquisition_ns += min_time;
            *conversion_clock_cycles += clock_cycles;
        }
    }
}

uint32_t _cyhal_adc_calc_optimal_clock_rate(cyhal_adc_t* obj, uint32_t target_sample_hz)
{
    /* From the architecture TRM */
    const uint32_t ADC_CLOCK_MAX_HZ = 60000000;
    const uint32_t ADC_CLOCK_MIN_HZ = 1000000;

    uint32_t sample_period_ns = _CYHAL_UTILS_NS_PER_SECOND / target_sample_hz;
    uint32_t total_acquisition_ns, conversion_clock_cycles;
    _cyhal_adc_get_sample_times(obj, &total_acquisition_ns, &conversion_clock_cycles);

    uint32_t conversion_budget_ns;
    if(sample_period_ns < total_acquisition_ns)
    {
        // Requested sampling rate is impossible - go as fast as we can.
        conversion_budget_ns = 1;
    }
    else
    {
        conversion_budget_ns = sample_period_ns - total_acquisition_ns;
    }

    uint32_t target_period_ns = conversion_budget_ns / conversion_clock_cycles;
    uint32_t target_clock_hz = _CYHAL_UTILS_NS_PER_SECOND / target_period_ns;
    if(target_clock_hz > ADC_CLOCK_MAX_HZ)
    {
        target_clock_hz = ADC_CLOCK_MAX_HZ;
    }
    else if(target_clock_hz < ADC_CLOCK_MIN_HZ)
    {
        target_clock_hz = ADC_CLOCK_MIN_HZ;
    }

    return target_clock_hz;
}

uint32_t _cyhal_adc_compute_actual_sample_rate(cyhal_adc_t* obj)
{
    /* Assumes that the acquisition timers and clock frequency are already set */
    uint32_t clock_frequency_hz = cyhal_clock_get_frequency(&obj->clock);
    uint32_t clock_period_ns = (clock_frequency_hz > 0)
        ? _CYHAL_UTILS_NS_PER_SECOND / clock_frequency_hz
        : 0;
    uint16_t sample_timer[] =
    {
        (obj->base->SAMPLE_TIME01 & SAR_SAMPLE_TIME01_SAMPLE_TIME0_Msk) >> CY_SAR_SAMPLE_TIME0_SHIFT,
        (obj->base->SAMPLE_TIME01 & SAR_SAMPLE_TIME01_SAMPLE_TIME1_Msk) >> CY_SAR_SAMPLE_TIME1_SHIFT,
        (obj->base->SAMPLE_TIME23 & SAR_SAMPLE_TIME23_SAMPLE_TIME2_Msk) >> CY_SAR_SAMPLE_TIME2_SHIFT,
        (obj->base->SAMPLE_TIME23 & SAR_SAMPLE_TIME23_SAMPLE_TIME3_Msk) >> CY_SAR_SAMPLE_TIME3_SHIFT,
    };

    uint32_t total_sample_time_ns = 0;

    for(uint8_t i = 0; i < CY_SAR_MAX_NUM_CHANNELS; ++i)
    {
        if(0u == (obj->base->CHAN_EN & 1u << i))
        {
            continue;
        }
        uint8_t sample_time_idx =
            (obj->base->CHAN_CONFIG[i] & SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Msk) >> SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Pos;
        /* Per the register map, the register value is one more than the actual cycle number. */
        uint32_t sample_cycles = sample_timer[sample_time_idx] - 1;
        uint32_t total_cycles = sample_cycles + _CYHAL_ADC_CONVERSION_CYCLES;
        uint32_t sample_time_ns = total_cycles * clock_period_ns;
        sample_time_ns *= _cyhal_adc_get_average_count(obj, i);
        total_sample_time_ns += sample_time_ns;
    }

    uint32_t sample_frequency_hz = (total_sample_time_ns > 0)
        ? _CYHAL_UTILS_NS_PER_SECOND / total_sample_time_ns
        : 0;
    return sample_frequency_hz;
}

cy_rslt_t cyhal_adc_set_sample_rate(cyhal_adc_t* obj, uint32_t desired_sample_rate_hz, uint32_t* achieved_sample_rate_hz)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    /* If we don't own the clock, the caller needs to adjust it and/or the acquisition times to achive the desired rate */
    if(obj->dedicated_clock)
    {
        uint32_t desired_hz = _cyhal_adc_calc_optimal_clock_rate(obj, desired_sample_rate_hz);
        result = cyhal_clock_set_frequency(&(obj->clock), desired_hz, NULL);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_adc_populate_acquisition_timers(obj);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        *achieved_sample_rate_hz = _cyhal_adc_compute_actual_sample_rate(obj);
    }
    else
    {
        *achieved_sample_rate_hz = 0u;
    }
    return result;
}

/*******************************************************************************
*       ADC Channel HAL Functions
*******************************************************************************/

uint32_t _cyhal_adc_channel_convert_config(const cyhal_adc_channel_config_t* config, cyhal_gpio_t vplus, cyhal_gpio_t vminus)
{
    uint32_t result = (uint32_t)CY_SAR_CHAN_SAMPLE_TIME_0  /* Placeholder, will be updated by populate_acquisition_timers */
                        | (config->enable_averaging ? (uint32_t)CY_SAR_CHAN_AVG_ENABLE : (uint32_t)CY_SAR_CHAN_AVG_DISABLE)
                        | (uint32_t)CY_SAR_POS_PORT_ADDR_SARMUX
                        | _cyhal_adc_get_pin_addr(vplus, true);

    if(vminus == NC)
    {
        /* Single-ended channel */
        result |= (uint32_t)CY_SAR_CHAN_SINGLE_ENDED;
    }
    else
    {
        /* Differential */
        result |= (uint32_t)CY_SAR_NEG_PORT_ADDR_SARMUX;
        result |= (uint32_t)CY_SAR_CHAN_DIFFERENTIAL_UNPAIRED;
        result |= _cyhal_adc_get_pin_addr(vminus, false);
    }

    return result;
}

static void _cyhal_adc_update_chan_offset(cyhal_adc_channel_t* obj)
{
    /* Normally, the PDL sets the offset in the ADC init. But we change the channel config after we initialize
     * the ADC itself, so we need to set the offset appropriately here. Otherwise the _uv functions will not
     * work correctly.
     * The conditions in the PDL are: singleEnded && vrefNegSelect && singleEndedSigned. We always operate
     * in signed mode so we only need to check the first two.
    */
    bool single_ended = (CYHAL_ADC_VNEG == obj->vminus);
    bool neg_sel_vref = ((uint32_t)CY_SAR_NEG_SEL_VREF == (obj->adc->base->CTRL & SAR_CTRL_NEG_SEL_Msk));
    int16_t offset = (single_ended && neg_sel_vref) ? (-1 * ((int16_t) (CY_SAR_WRK_MAX_12BIT / 2))) : 0;
#if CY_IP_MXS40PASS_SAR_INSTANCES < 2
    Cy_SAR_SetOffset(obj->channel_idx, offset);
#else
    Cy_SAR_SetChannelOffset(obj->adc->base, obj->channel_idx, offset);
#endif
}

static const cyhal_resource_pin_mapping_t* _cyhal_adc_find_matching_resource(cyhal_resource_inst_t* adc, cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *pin_map, size_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        if (pin == pin_map[i].pin && _cyhal_utils_resources_equal(adc, pin_map[i].inst))
        {
            return &pin_map[i];
        }
    }
    return NULL;
}

cy_rslt_t cyhal_adc_channel_init_diff(cyhal_adc_channel_t *obj, cyhal_adc_t* adc, cyhal_gpio_t vplus, cyhal_gpio_t vminus, const cyhal_adc_channel_config_t* cfg)
{
    CY_ASSERT(obj != NULL);
    CY_ASSERT(adc != NULL);

    const uint32_t CYHAL_ADC_MIN_ACQUISITION_TIME_NS = 167;

    cy_rslt_t result = CY_RSLT_SUCCESS;

    memset(obj, 0, sizeof(cyhal_adc_channel_t));
    obj->vplus = NC;
    obj->vminus = NC;

    // Check for invalid pin or pin belonging to a different SAR
    const cyhal_resource_pin_mapping_t *vplus_map = _cyhal_adc_find_matching_resource(&(adc->resource), vplus,
        cyhal_pin_map_pass_sarmux_pads, sizeof(cyhal_pin_map_pass_sarmux_pads)/sizeof(cyhal_pin_map_pass_sarmux_pads[0]));
    const cyhal_resource_pin_mapping_t *vminus_map = NULL;

    if(NULL == vplus_map)
    {
        result = CYHAL_ADC_RSLT_BAD_ARGUMENT;
    }

    if(CY_RSLT_SUCCESS == result && CYHAL_ADC_VNEG != vminus)
    {
        vminus_map = _cyhal_adc_find_matching_resource(&(adc->resource), vminus, cyhal_pin_map_pass_sarmux_pads,
            sizeof(cyhal_pin_map_pass_sarmux_pads)/sizeof(cyhal_pin_map_pass_sarmux_pads[0]));
        if (NULL == vminus_map)
        {
            result = CYHAL_ADC_RSLT_BAD_ARGUMENT;
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_utils_reserve_and_connect(vplus, vplus_map);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->vplus = vplus;
        if(CYHAL_ADC_VNEG != vminus)
        {
            result = _cyhal_utils_reserve_and_connect(vminus, vminus_map);
        }
    }

    uint8_t chosen_channel = 0;

    if (CY_RSLT_SUCCESS == result)
    {
        obj->vminus = vminus;

        // Find the first available channel
        for(chosen_channel = 0; chosen_channel < CY_SAR_MAX_NUM_CHANNELS; ++chosen_channel)
        {
            if(NULL == adc->channel_config[chosen_channel])
            {
                break;
            }
        }
        if (chosen_channel >= CY_SAR_MAX_NUM_CHANNELS) // No channels available
            result = CYHAL_ADC_RSLT_NO_CHANNELS;
    }

    if(CY_RSLT_SUCCESS == result)
    {
        // Don't set the ADC until here so that free knows whether we have allocated
        // the channel on the parent ADC instance (and therefore doesn't try to free it if
        // something fails further up)
        obj->adc = adc;
        obj->channel_idx = chosen_channel;
        obj->adc->channel_config[chosen_channel] = obj;
        obj->minimum_acquisition_ns = (cfg->min_acquisition_ns > CYHAL_ADC_MIN_ACQUISITION_TIME_NS)
                                       ? cfg->min_acquisition_ns : CYHAL_ADC_MIN_ACQUISITION_TIME_NS;
    }

    if(CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_adc_populate_acquisition_timers(obj->adc);
    }

    if(CY_RSLT_SUCCESS == result)
    {
        uint32_t fw_ctrl_plus = _cyhal_adc_get_fw_switch_control(vplus, true);
        uint32_t mux_ctrl_plus = _cyhal_adc_get_mux_switch_control(vplus);
        Cy_SAR_SetAnalogSwitch(obj->adc->base, CY_SAR_MUX_SWITCH0, fw_ctrl_plus, CY_SAR_SWITCH_CLOSE);
        Cy_SAR_SetSwitchSarSeqCtrl(obj->adc->base, mux_ctrl_plus, CY_SAR_SWITCH_SEQ_CTRL_ENABLE);

        if(CYHAL_ADC_VNEG != vminus)
        {
            uint32_t fw_ctrl_minus = _cyhal_adc_get_fw_switch_control(vminus, false);
            uint32_t mux_ctrl_minus = _cyhal_adc_get_mux_switch_control(vminus);

            Cy_SAR_SetAnalogSwitch(obj->adc->base, CY_SAR_MUX_SWITCH0, fw_ctrl_minus, CY_SAR_SWITCH_CLOSE);
            Cy_SAR_SetSwitchSarSeqCtrl(obj->adc->base, mux_ctrl_minus, CY_SAR_SWITCH_SEQ_CTRL_ENABLE);
        }

        result = cyhal_adc_channel_configure(obj, cfg);
    }

    if(CY_RSLT_SUCCESS != result)
    {
        cyhal_adc_channel_free(obj);
    }

    return result;
}

cy_rslt_t cyhal_adc_channel_configure(cyhal_adc_channel_t *obj, const cyhal_adc_channel_config_t *config)
{
    CY_ASSERT(NULL != obj);

    obj->adc->base->CHAN_CONFIG[obj->channel_idx] = _cyhal_adc_channel_convert_config(config, obj->vplus, obj->vminus);
    if(config->enabled)
    {
        obj->adc->base->CHAN_EN |= 1u << obj->channel_idx;
    }
    else
    {
        obj->adc->base->CHAN_EN &= ~(1u << obj->channel_idx);
    }
    _cyhal_adc_update_chan_offset(obj);
    return _cyhal_adc_populate_acquisition_timers(obj->adc);
}

void cyhal_adc_channel_free(cyhal_adc_channel_t *obj)
{
    if(obj->adc != NULL)
    {
        // Disable the channel, the unconfigure it
        obj->adc->channel_config[obj->channel_idx] = NULL;

        if(NC != obj->vplus)
        {
            uint32_t fw_ctrl_plus = _cyhal_adc_get_fw_switch_control(obj->vplus, true);
            uint32_t mux_ctrl_plus = _cyhal_adc_get_mux_switch_control(obj->vplus);

            Cy_SAR_SetAnalogSwitch(obj->adc->base, CY_SAR_MUX_SWITCH0, fw_ctrl_plus, CY_SAR_SWITCH_OPEN);
            Cy_SAR_SetSwitchSarSeqCtrl(obj->adc->base, mux_ctrl_plus, CY_SAR_SWITCH_SEQ_CTRL_DISABLE);
        }

        if(NC != obj->vminus)
        {
            uint32_t mux_ctrl_minus = _cyhal_adc_get_mux_switch_control(obj->vminus);
            uint32_t fw_ctrl_minus = _cyhal_adc_get_fw_switch_control(obj->vminus, false);

            Cy_SAR_SetAnalogSwitch(obj->adc->base, CY_SAR_MUX_SWITCH0, fw_ctrl_minus, CY_SAR_SWITCH_OPEN);
            Cy_SAR_SetSwitchSarSeqCtrl(obj->adc->base, mux_ctrl_minus, CY_SAR_SWITCH_SEQ_CTRL_DISABLE);
        }
        obj->adc->base->CHAN_CONFIG[obj->channel_idx] = 0;
        obj->adc = NULL;
    }

    _cyhal_utils_release_if_used(&(obj->vplus));
    _cyhal_utils_release_if_used(&(obj->vminus));
}

uint16_t cyhal_adc_read_u16(const cyhal_adc_channel_t *obj)
{
    const uint8_t RESULT_SCALING_FACTOR = UINT16_MAX / 0xFFF; // 12-bit SAR resolution
    int32_t signed_result = cyhal_adc_read(obj);
    /* Legacy API for BWC. Convert from signed to unsigned by adding 0x800 to
     * convert the lowest signed 12-bit number to 0x0.
     */
    uint16_t unsigned_result = (uint16_t)(signed_result + 0x800);
    /* The SAR provides a 12-bit result, but this API is defined to fill a full 16-bit range */
    uint16_t scaled_result = unsigned_result * RESULT_SCALING_FACTOR;
    return scaled_result;
}

int32_t cyhal_adc_read(const cyhal_adc_channel_t *obj)
{
    uint32_t old_en_mask = 0u;

    bool isInterleaved = (CY_SAR_AVG_MODE_INTERLEAVED == (SAR_SAMPLE_CTRL(obj->adc->base) & SAR_SAMPLE_CTRL_AVG_MODE_Msk));
    bool isChannelAveraging = (obj->adc->base->CHAN_CONFIG[obj->channel_idx] & SAR_CHAN_CONFIG_AVG_EN_Msk);
    if(!obj->adc->continuous_scanning)
    {
        /* Enable the selected channel only, then perform an on-demand conversion.
         * Save the old enabled channel set to restore after we're done */
        old_en_mask = SAR_CHAN_EN(obj->adc->base);
        Cy_SAR_SetChanMask(obj->adc->base, 1U << obj->channel_idx);
        obj->adc->conversion_complete = false;

        // If interleaved averaging and average is enabled for this channel, set for
        // continuous scanning and then stop the scan once we get a result. This is 
        // because the ADC hardware has a special case where it will not raise
        // the EOC interrupt until AVG_COUNT scans have occurred when all enabled
        // channels are using interleaved channels. This means that for the first AVG_COUNT - 1
        // scans there will be no interrupt, therefore conversion_complete will never 
        // be set true, and therefore the loop below would be stuck waiting forever, 
        // never able to trigger a subsequent scan.
        Cy_SAR_StartConvert(obj->adc->base, (isInterleaved && isChannelAveraging) ? CY_SAR_START_CONVERT_CONTINUOUS : CY_SAR_START_CONVERT_SINGLE_SHOT);
    }

    /* Cy_SAR_IsEndConversion relies on and clears the EOS interrupt status bit.
     * We don't know how this read will be used in combination with interrupts,
     * so implement our own interrupt-driven EOS flag
     */
    while(!obj->adc->conversion_complete) { }

    int32_t result = Cy_SAR_GetResult32(obj->adc->base, obj->channel_idx);

    if(!obj->adc->continuous_scanning)
    {
        if(isInterleaved && isChannelAveraging)
        {
            Cy_SAR_StopConvert(obj->adc->base);
        }
        Cy_SAR_SetChanMask(obj->adc->base, old_en_mask);
    }

    return result;
}

int32_t cyhal_adc_read_uv(const cyhal_adc_channel_t *obj)
{
    CY_ASSERT(NULL != obj);

    int32_t counts = cyhal_adc_read(obj);
    return Cy_SAR_CountsTo_uVolts(obj->adc->base, obj->channel_idx, counts);
}

void _cyhal_adc_start_async_read(cyhal_adc_t* obj, size_t num_scan, int32_t* result_list)
{
    CY_ASSERT(NULL == obj->async_buff_next); /* Transfer already in progress */
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->async_scans_remaining = num_scan;
    obj->async_buff_next = obj->async_buff_orig = result_list;

    if(false == obj->continuous_scanning)
    {
        Cy_SAR_StartConvert(obj->base, CY_SAR_START_CONVERT_SINGLE_SHOT);
    }
    cyhal_system_critical_section_exit(savedIntrStatus);
}

cy_rslt_t cyhal_adc_read_async(cyhal_adc_t* obj, size_t num_scan, int32_t* result_list)
{
    CY_ASSERT(NULL != obj);
    obj->async_transfer_in_uv = false;
    _cyhal_adc_start_async_read(obj, num_scan, result_list);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_adc_read_async_uv(cyhal_adc_t* obj, size_t num_scan, int32_t* result_list)
{
    CY_ASSERT(NULL != obj);
    obj->async_transfer_in_uv = true;
    _cyhal_adc_start_async_read(obj, num_scan, result_list);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_adc_set_async_mode(cyhal_adc_t *obj, cyhal_async_mode_t mode, uint8_t dma_priority)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL == obj->async_buff_next); /* Can't swap mode while a transfer is running */

    cy_rslt_t result = CY_RSLT_SUCCESS;

    if(mode == CYHAL_ASYNC_DMA)
    {
        result = cyhal_dma_init(&(obj->dma), CYHAL_DMA_PRIORITY_DEFAULT, CYHAL_DMA_DIRECTION_PERIPH2MEM);
        if(CY_RSLT_SUCCESS == result)
        {
            cyhal_dma_register_callback(&(obj->dma), &_cyhal_adc_dma_handler, obj);
            cyhal_dma_enable_event(&(obj->dma), CYHAL_DMA_TRANSFER_COMPLETE, dma_priority, true);
        }
    }
    else
    {
        /* Free the DMA instances if we reserved them but don't need them anymore */
        if(CYHAL_RSC_INVALID != obj->dma.resource.type)
        {
            cyhal_dma_free(&obj->dma);
            obj->dma.resource.type = CYHAL_RSC_INVALID;
        }
    }

    if(CY_RSLT_SUCCESS == result)
    {
        obj->async_mode = mode;
    }
    return CY_RSLT_SUCCESS;
}

void cyhal_adc_register_callback(cyhal_adc_t *obj, cyhal_adc_event_callback_t callback, void *callback_arg)
{
    CY_ASSERT(NULL != obj);

    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_adc_enable_event(cyhal_adc_t *obj, cyhal_adc_event_t event, uint8_t intr_priority, bool enable)
{
    /* We always listen to EOS internally, so no need to update a hardware interrupt mask */
    if(enable)
    {
        obj->user_enabled_events |= event;
    }
    else
    {
        obj->user_enabled_events &= ~event;
    }

    IRQn_Type irqn = _cyhal_adc_irq_n[obj->resource.block_num];
    NVIC_SetPriority(irqn, intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXS40PASS_SAR_INSTANCES) */
