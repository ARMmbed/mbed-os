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

#include <cmsis_compiler.h>

#include "cyhal_adc.h"
#include "cyhal_analog_common.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_interconnect.h"
#include "cyhal_utils.h"

#if defined(CY_IP_MXS40PASS_SAR_INSTANCES)

#if defined(__cplusplus)
extern "C"
{
#endif

static SAR_Type *const cyhal_sar_base[] =
{
#if (CY_IP_MXS40PASS_SAR_INSTANCES == 1)
    SAR,
#endif
#if (CY_IP_MXS40PASS_SAR_INSTANCES > 1)
    #warning Unhandled SAR instance count
#endif
};

static const en_clk_dst_t cyhal_sar_clock[] =
{
#if (CY_IP_MXS40PASS_SAR_INSTANCES == 1)
    PCLK_PASS_CLOCK_SAR,
#endif
#if (CY_IP_MXS40PASS_SAR_INSTANCES > 1)
    #warning Unhandled SAR instance count
#endif
};

#define CYHAL_SAR_DEFAULT_CTRL ((uint32_t)CY_SAR_VREF_PWR_100 | (uint32_t)CY_SAR_VREF_SEL_BGR \
                                | (uint32_t)CY_SAR_BYPASS_CAP_DISABLE | (uint32_t)CY_SAR_NEG_SEL_VREF \
                                | (uint32_t)CY_SAR_CTRL_NEGVREF_HW | (uint32_t)CY_SAR_CTRL_COMP_DLY_12 \
                                | (uint32_t)CY_SAR_COMP_PWR_100 | (uint32_t)CY_SAR_DEEPSLEEP_SARMUX_OFF \
                                | (uint32_t)CY_SAR_SARSEQ_SWITCH_ENABLE)
#define CYHAL_SAR_DEFAULT_SAMPLE ((uint32_t)CY_SAR_RIGHT_ALIGN | (uint32_t)CY_SAR_TRIGGER_MODE_FW_ONLY | (uint32_t)CY_SAR_SINGLE_ENDED_UNSIGNED)
#define CYHAL_SAR_DEFAULT_VREF_MV 1200UL

/** Default configuration for a generic channel; OR in a POS addr for the appropriate pin before configuring the channel */
#define CYHAL_SAR_DEFAULT_CH_CONFIG ((uint32_t)CY_SAR_POS_PORT_ADDR_SARMUX | (uint32_t)CY_SAR_CHAN_SINGLE_ENDED \
                                     | (uint32_t)CY_SAR_CHAN_AVG_DISABLE | (uint32_t)CY_SAR_CHAN_SAMPLE_TIME_0)

const cy_stc_sar_config_t CYHAL_SAR_DEFAULT_CONFIG =
{
    .ctrl = CYHAL_SAR_DEFAULT_CTRL,
    .sampleCtrl = CYHAL_SAR_DEFAULT_SAMPLE,
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
    .vrefMvValue = CYHAL_SAR_DEFAULT_VREF_MV,
};

/*******************************************************************************
*       Internal helper functions
*******************************************************************************/
static uint32_t cyhal_adc_get_mux_switch_control(cyhal_gpio_t gpio)
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

static uint32_t cyhal_adc_get_fw_switch_control(cyhal_gpio_t gpio, bool is_vplus)
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

static uint8_t cyhal_adc_get_pin_addr(cyhal_gpio_t gpio, bool is_vplus)
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
    return is_vplus ? (uint8_t)vplus_lookup[pin] : (uint8_t)vminus_lookup[pin];
}

/*******************************************************************************
*       ADC HAL Functions
*******************************************************************************/
cy_rslt_t cyhal_adc_init(cyhal_adc_t *obj, cyhal_gpio_t pin, const cyhal_clock_divider_t *clk)
{
    const uint32_t DESIRED_DIVIDER = 8000000u; // 8 MHz. Required range is 1.7 - 18

    CY_ASSERT(NULL != obj);

    cy_rslt_t result = CY_RSLT_SUCCESS;

    if (CYHAL_NC_PIN_VALUE == pin)
        result = CYHAL_ADC_RSLT_BAD_ARGUMENT;

    if (CY_RSLT_SUCCESS == result)
    {
        memset(obj, 0, sizeof(cyhal_adc_t));
        obj->base = NULL;
        obj->channel_used = 0;
        obj->clock.div_num = CYHAL_RSC_INVALID;
        obj->resource.type = CYHAL_RSC_INVALID;
        obj->dedicated_clock = false;
    }

    const cyhal_resource_pin_mapping_t *map = CY_UTILS_GET_RESOURCE(pin, cyhal_pin_map_pass_sarmux_pads);
    if (NULL == map)
        result = CYHAL_ADC_RSLT_BAD_ARGUMENT;

    cyhal_resource_inst_t adc_inst;
    if (CY_RSLT_SUCCESS == result)
    {
        adc_inst = *map->inst;
        result = cyhal_hwmgr_reserve(&adc_inst);
    }

    en_clk_dst_t pclk;
    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource = adc_inst;

        obj->base = cyhal_sar_base[adc_inst.block_num];
        pclk = (en_clk_dst_t)(cyhal_sar_clock[adc_inst.block_num]);
        if (NULL != clk)
        {
            obj->clock = *clk;
            obj->dedicated_clock = false;
        }
        else if (CY_RSLT_SUCCESS == (result = cyhal_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_16_BIT, false)))
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

    if (CY_RSLT_SUCCESS == result)
    {
        result = (cy_rslt_t)Cy_SAR_Init(obj->base, &CYHAL_SAR_DEFAULT_CONFIG);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        Cy_SAR_SetVssaSarSeqCtrl(obj->base, CY_SAR_SWITCH_SEQ_CTRL_ENABLE);
        Cy_SAR_SetVssaVminusSwitch(obj->base, CY_SAR_SWITCH_CLOSE);

        cyhal_analog_init();
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
        Cy_SAR_SetVssaSarSeqCtrl(obj->base, CY_SAR_SWITCH_SEQ_CTRL_DISABLE);
        Cy_SAR_SetVssaVminusSwitch(obj->base, CY_SAR_SWITCH_OPEN);
        Cy_SAR_Disable(obj->base);

        if(obj->dedicated_clock)
        {
            Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);
            cyhal_hwmgr_free_clock(&obj->clock);
        }

        cyhal_analog_free();

        cyhal_hwmgr_free(&obj->resource);

        obj->base = NULL;
    }
}
/*******************************************************************************
*       ADC Channel HAL Functions
*******************************************************************************/
cy_rslt_t cyhal_adc_channel_init(cyhal_adc_channel_t *obj, cyhal_adc_t* adc, cyhal_gpio_t pin)
{
    CY_ASSERT(obj != NULL);
    CY_ASSERT(adc != NULL);
    if (CYHAL_NC_PIN_VALUE == pin)
        return CYHAL_ADC_RSLT_BAD_ARGUMENT;

    // Check for invalid pin or pin belonging to a different SAR
    const cyhal_resource_pin_mapping_t *map = CY_UTILS_GET_RESOURCE(pin, cyhal_pin_map_pass_sarmux_pads);
    if (NULL == map || map->inst->block_num != adc->resource.block_num)
        return CYHAL_ADC_RSLT_BAD_ARGUMENT;

    memset(obj, 0, sizeof(cyhal_adc_channel_t));
    cy_rslt_t result;

    cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(pin);
    if (CY_RSLT_SUCCESS != (result = cyhal_hwmgr_reserve(&pinRsc)))
        return result;
    obj->pin = pin;

    if (CY_RSLT_SUCCESS == result)
        result = cyhal_connect_pin(map);

    // Find the first available channel
    uint8_t chosen_channel = __CLZ(__RBIT(~adc->channel_used));
    if(chosen_channel >= CY_SAR_MAX_NUM_CHANNELS) // No channels available
        result = CYHAL_ADC_RSLT_NO_CHANNELS;
    else
    {
        // Don't set the ADC until here so that free knows whether we have allocated
        // the channel on the parent ADC instance (and therefore doesn't try to free it if
        // something fails further up)
        obj->adc = adc;
        obj->channel_idx = chosen_channel;
        obj->adc->channel_used |= 1U << chosen_channel;
    }

    // The current version only supports single-ended channels, so always set the vplus switch
    uint32_t fw_ctrl = cyhal_adc_get_fw_switch_control(pin, true);
    uint32_t mux_ctrl = cyhal_adc_get_mux_switch_control(pin);

    Cy_SAR_SetAnalogSwitch(obj->adc->base, CY_SAR_MUX_SWITCH0, fw_ctrl, CY_SAR_SWITCH_CLOSE);
    Cy_SAR_SetSwitchSarSeqCtrl(obj->adc->base, mux_ctrl, CY_SAR_SWITCH_SEQ_CTRL_ENABLE);

    uint8_t pin_select = cyhal_adc_get_pin_addr(pin, true);
    uint32_t channel_config = CYHAL_SAR_DEFAULT_CH_CONFIG | pin_select;
    obj->adc->base->CHAN_CONFIG[chosen_channel] = channel_config;

    return result;
}

void cyhal_adc_channel_free(cyhal_adc_channel_t *obj)
{
    if(obj->adc != NULL)
    {
        // Disable the channel, the unconfigure it
        obj->adc->channel_used &= ~(1U << obj->channel_idx);

        uint32_t fw_ctrl = cyhal_adc_get_fw_switch_control(obj->pin, true);
        uint32_t mux_ctrl = cyhal_adc_get_mux_switch_control(obj->pin);
        Cy_SAR_SetAnalogSwitch(obj->adc->base, CY_SAR_MUX_SWITCH0, fw_ctrl, CY_SAR_SWITCH_OPEN);
        Cy_SAR_SetSwitchSarSeqCtrl(obj->adc->base, mux_ctrl, CY_SAR_SWITCH_SEQ_CTRL_DISABLE);

        obj->adc->base->CHAN_CONFIG[obj->channel_idx] = 0;
        cyhal_utils_release_if_used(&(obj->pin));
        obj->adc = NULL;
    }
}

uint16_t cyhal_adc_read_u16(const cyhal_adc_channel_t *obj)
{
    const uint8_t RESULT_SCALING_FACTOR = UINT16_MAX / 0xFFF; // 12-bit SAR resolution
    // Enable the selected channel only, then perform an on-demand conversion
    Cy_SAR_SetChanMask(obj->adc->base, 1U << obj->channel_idx);
    Cy_SAR_StartConvert(obj->adc->base, CY_SAR_START_CONVERT_SINGLE_SHOT);
    Cy_SAR_IsEndConversion(obj->adc->base, CY_SAR_WAIT_FOR_RESULT);

    // GetResult16 returns a signed value; it is safe to cast to unsigned because
    // we always configure the channel in an unsigned mode. We also need to scale
    // the value up because the SAR returns a 12-bit value (we don't configure averaging
    // or summing) but our result is defined to fill the full 16-bit range.
    uint16_t result = (uint16_t)Cy_SAR_GetResult16(obj->adc->base, obj->channel_idx);
    uint16_t scaled_result = result * RESULT_SCALING_FACTOR;
    return scaled_result;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXS40PASS_SAR_INSTANCES) */
