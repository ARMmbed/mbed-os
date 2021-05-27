/***************************************************************************//**
* \file cyhal_pwm.c
*
* \brief
* Provides a high level interface for interacting with the Cypress PWM. This is
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

/**
 * \addtogroup group_hal_impl_pwm PWM (Pulse Width Modulator)
 * \ingroup group_hal_impl
 * \{
 * \section section_hal_impl_pwm_compl_pins Complementary PWM output
 * The PWM HAL driver allows generation of a normal and an inverted output. PSoC devices support
 * complementary pin pairs to which the normal and inverted signals can be routed. To identify
 * the complementary pin for a given pin, open the PSoC device datasheet and navigate to the
 * 'Multiple Alternate Functions' table. Each column represents an alternate function of the pin
 * in the corresponding row. Find your pin and make a note of the tcpwm[X].line[Y]:Z. The
 * complementary pin is found by looking up the pin against tcpwm[X].line_<b>compl</b>[Y]:Z from
 * the same column. For example, the image below shows a pair of complementary pins (P0.0 and P0.1)
 * identified by the tcpwm[0].line[0]:0 and tcpwm[0].line_compl[0]:0 mapping. These complementary
 * pins can be supplied to \ref cyhal_pwm_init_adv using <b>pin</b> and <b>compl_pin</b> parameters
 * in any order. \image html pwm_compl_pins.png "Complementary PWM pins"
 *
 * \section section_psoc6_pwm_resolution PWM Resolution
 * On CAT1 (PSoC 6) devices, not all PWMs hardware blocks are of the same resolution. The
 * resolution of the PWM associated with a given pin is specified by the `TCPWM<idx>_CNT_CNT_WIDTH`
 * macro (provided by cy_device_headers.h in mtb-pdl-cat1), where `<idx>` is the index associated
 * with the `tcpwm` portion of the entry in the pin function table. For example, if the pin
 * function is `tcpwm[1].line[3]:Z`, `<idx>` would be 1.
 *
 * By default, the PWM HAL driver will configure the input clock frequency such that all PWM
 * instances are able to provide the same maximum period regardless of the underlying resolution,
 * but period and duty cycle can be specified with reduced granularity on lower-resolution PWM
 * instances. If an application is more sensitive to PWM precision than maximum period, or if a
 * longer maximum period is required (with correspondingly reduced precision), it is possible to
 * override the default clock by passing a \ref cyhal_clock_t instance to the
 * \ref cyhal_pwm_init function with a custom frequency specified. See the \ref group_hal_clock
 * HAL documentation for more details.
 *
 * \} group_hal_impl_pwm
 */

#include <string.h>

#include "cyhal_clock.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_pwm.h"
#include "cyhal_interconnect.h"
#include "cyhal_syspm.h"
#include "cyhal_utils.h"

#if defined(CY_IP_MXTCPWM) || defined(CY_IP_M0S8TCPWM_INSTANCES)

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(CY_IP_MXTCPWM_INSTANCES)
#define _CYHAL_PWM_MAX_WIDTH         32
#else
#define _CYHAL_PWM_MAX_WIDTH         16
#endif
#define _CYHAL_PWM_MAX_DEAD_TIME_CYCLES    255
static const uint32_t _CYHAL_PWM_US_PER_SEC = 1000000u;

#if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
/** The configuration of PWM output signal for Center and Asymmetric alignment with overflow and underflow swapped */
#define _CYHAL_PWM_MODE_CNTR_OR_ASYMM_UO_SWAPPED (_VAL2FLD(TCPWM_GRP_CNT_V2_TR_PWM_CTRL_CC0_MATCH_MODE, CY_TCPWM_PWM_TR_CTRL2_INVERT) | \
                                         _VAL2FLD(TCPWM_GRP_CNT_V2_TR_PWM_CTRL_OVERFLOW_MODE, CY_TCPWM_PWM_TR_CTRL2_CLEAR) | \
                                         _VAL2FLD(TCPWM_GRP_CNT_V2_TR_PWM_CTRL_UNDERFLOW_MODE, CY_TCPWM_PWM_TR_CTRL2_SET))
#else
/** The configuration of PWM output signal for Center and Asymmetric alignment with overflow and underflow swapped */
#define _CYHAL_PWM_MODE_CNTR_OR_ASYMM_UO_SWAPPED (_VAL2FLD(TCPWM_CNT_TR_CTRL2_CC_MATCH_MODE, CY_TCPWM_PWM_TR_CTRL2_INVERT) | \
                                         _VAL2FLD(TCPWM_CNT_TR_CTRL2_OVERFLOW_MODE, CY_TCPWM_PWM_TR_CTRL2_CLEAR) | \
                                         _VAL2FLD(TCPWM_CNT_TR_CTRL2_UNDERFLOW_MODE, CY_TCPWM_PWM_TR_CTRL2_SET))
#endif


static cy_rslt_t _cyhal_pwm_convert_alignment(cyhal_pwm_alignment_t hal_alignment, uint32_t *pdl_alignment, bool swapped)
{
    switch (hal_alignment)
    {
        case CYHAL_PWM_LEFT_ALIGN:
            *pdl_alignment = (swapped) ? CY_TCPWM_PWM_RIGHT_ALIGN : CY_TCPWM_PWM_LEFT_ALIGN;
            return CY_RSLT_SUCCESS;
        case CYHAL_PWM_RIGHT_ALIGN:
            *pdl_alignment = (swapped) ? CY_TCPWM_PWM_LEFT_ALIGN : CY_TCPWM_PWM_RIGHT_ALIGN;
            return CY_RSLT_SUCCESS;
        case CYHAL_PWM_CENTER_ALIGN:
            *pdl_alignment = CY_TCPWM_PWM_CENTER_ALIGN;
            return CY_RSLT_SUCCESS;
        default:
            return CYHAL_PWM_RSLT_BAD_ARGUMENT;
    }
}

static cy_rslt_t cyhal_pwm_set_period_and_compare(cyhal_pwm_t *obj, uint32_t period, uint32_t compare)
{
    if (period < 1 || period > (uint32_t)((1 << _CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].max_count)) - 1)
        return CYHAL_PWM_RSLT_BAD_ARGUMENT;
    if (compare >= period)
        compare = period - 1;

    cyhal_gpio_t pin = obj->pin;
    cyhal_gpio_t pin_compl = obj->pin_compl;

    Cy_TCPWM_PWM_SetCompare0(obj->tcpwm.base, obj->tcpwm.resource.channel_num, 0u);
    Cy_TCPWM_PWM_SetPeriod0(obj->tcpwm.base, obj->tcpwm.resource.channel_num, period - 1u);

    #if defined(CYHAL_PIN_MAP_TCPWM_LINE_COMPL)
    bool swapped_pins = (_CYHAL_UTILS_GET_RESOURCE(pin, cyhal_pin_map_tcpwm_line_compl) != NULL) && (_CYHAL_UTILS_GET_RESOURCE(pin_compl, cyhal_pin_map_tcpwm_line) != NULL);
    #else
    CY_UNUSED_PARAMETER(pin);
    CY_UNUSED_PARAMETER(pin_compl);
    bool swapped_pins = false;
    #endif /* defined(CYHAL_PIN_MAP_TCPWM_LINE_COMPL) */

    #if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
    uint32_t pwm_ctrl_reg = TCPWM_GRP_CNT_TR_PWM_CTRL(obj->tcpwm.base, TCPWM_GRP_CNT_GET_GRP(_CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource)),
                                _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    uint32_t cc1_ignore_mask = (0 == TCPWM_GRP_CNT_GET_GRP(_CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource))) ?
                                0 : CY_TCPWM_PWM_MODE_CC1_IGNORE;
    bool is_center_aligned = (pwm_ctrl_reg == (CY_TCPWM_PWM_MODE_CNTR_OR_ASYMM | cc1_ignore_mask)) ||
                                (pwm_ctrl_reg == (_CYHAL_PWM_MODE_CNTR_OR_ASYMM_UO_SWAPPED | cc1_ignore_mask));
    #else
    bool is_center_aligned = (TCPWM_CNT_TR_CTRL2(obj->tcpwm.base, obj->tcpwm.resource.channel_num) == CY_TCPWM_PWM_MODE_CNTR_OR_ASYMM) ||
                                (TCPWM_CNT_TR_CTRL2(obj->tcpwm.base, obj->tcpwm.resource.channel_num) == _CYHAL_PWM_MODE_CNTR_OR_ASYMM_UO_SWAPPED);
    #endif /* CY_IP_MXTCPWM_VERSION >= 2 or other */

    uint32_t new_compare_value = (swapped_pins && !is_center_aligned)
        ? period - compare
        : compare;
    Cy_TCPWM_PWM_SetCompare0(obj->tcpwm.base, obj->tcpwm.resource.channel_num, new_compare_value);
    if (Cy_TCPWM_PWM_GetCounter(obj->tcpwm.base, obj->tcpwm.resource.channel_num) >= new_compare_value)
    {
        Cy_TCPWM_PWM_SetCounter(obj->tcpwm.base, obj->tcpwm.resource.channel_num, 0);
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_pwm_init_adv(cyhal_pwm_t *obj, cyhal_gpio_t pin, cyhal_gpio_t pin_compl, cyhal_pwm_alignment_t pwm_alignment, bool continuous, uint32_t dead_time_us, bool invert, const cyhal_clock_t *clk)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t result = CY_RSLT_SUCCESS;
    bool swapped = false;

    /* Explicitly marked not allocated resources as invalid to prevent freeing them. */
    memset(obj, 0, sizeof(cyhal_pwm_t));

    const cyhal_resource_pin_mapping_t* map = _cyhal_utils_try_alloc(pin, cyhal_pin_map_tcpwm_line, sizeof(cyhal_pin_map_tcpwm_line) / sizeof(cyhal_resource_pin_mapping_t));
    #if defined(CYHAL_PIN_MAP_TCPWM_LINE_COMPL)
    if (map == NULL)
    {
        swapped = true;
        map = _cyhal_utils_try_alloc(pin, cyhal_pin_map_tcpwm_line_compl, sizeof(cyhal_pin_map_tcpwm_line_compl) / sizeof(cyhal_resource_pin_mapping_t));
    }
    #endif
    if (map == NULL)
    {
        return CYHAL_PWM_RSLT_BAD_ARGUMENT;
    }
    else
    {
        obj->tcpwm.resource = *map->inst;
        obj->tcpwm.base = _CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].base;
        obj->pin = CYHAL_NC_PIN_VALUE;
        obj->pin_compl = CYHAL_NC_PIN_VALUE;
        result = _cyhal_utils_reserve_and_connect(pin, map);
        if (CY_RSLT_SUCCESS == result)
        {
            obj->pin = pin;
        }
    }

    if (CY_RSLT_SUCCESS == result && NC != pin_compl)
    {
    #if defined(CYHAL_PIN_MAP_TCPWM_LINE_COMPL)
        const cyhal_resource_pin_mapping_t *map_compl = swapped
            ? _cyhal_utils_get_resource(pin_compl, cyhal_pin_map_tcpwm_line,
                sizeof(cyhal_pin_map_tcpwm_line) / sizeof(cyhal_resource_pin_mapping_t), &(obj->tcpwm.resource))
            :
    #if defined(CYHAL_PIN_MAP_TCPWM_LINE_COMPL)
              _cyhal_utils_get_resource(pin_compl, cyhal_pin_map_tcpwm_line_compl,
                sizeof(cyhal_pin_map_tcpwm_line_compl) / sizeof(cyhal_resource_pin_mapping_t), &(obj->tcpwm.resource))
    #else
              NULL
    #endif
              ;

        if ((NULL == map_compl) || !_cyhal_utils_resources_equal(map->inst, map_compl->inst))
        {
            result = CYHAL_PWM_RSLT_BAD_ARGUMENT;
        }
        else
        {
            result = _cyhal_utils_reserve_and_connect(pin_compl, map_compl);
            if (CY_RSLT_SUCCESS == result)
            {
                obj->pin_compl = pin_compl;
            }
        }
    #else
        result = CYHAL_PWM_RSLT_BAD_ARGUMENT;
    #endif /* defined(CYHAL_PIN_MAP_TCPWM_LINE_COMPL) */
    }

    if (CY_RSLT_SUCCESS == result)
    {
#if defined(CY_IP_M0S8TCPWM_INSTANCES)
        uint32_t source_hz = Cy_SysClk_ClkSysGetFrequency();
#else
        uint32_t source_hz = Cy_SysClk_ClkPeriGetFrequency();
#endif
        en_clk_dst_t pclk = (en_clk_dst_t)(_CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].clock_dst + obj->tcpwm.resource.channel_num);
        if (NULL != clk)
        {
            obj->tcpwm.clock = *clk;
            _cyhal_utils_update_clock_format(&obj->tcpwm.clock);
            if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, (cy_en_divider_types_t)obj->tcpwm.clock.block, obj->tcpwm.clock.channel))
            {
                result = CYHAL_PWM_RSLT_FAILED_CLOCK_INIT;
            }
        }
        else if (CY_RSLT_SUCCESS == (result = cyhal_clock_allocate(&obj->tcpwm.clock, CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT)))
        {
            obj->tcpwm.dedicated_clock = true;
            uint32_t div = (dead_time_us > 0)
                ? (((uint64_t)source_hz * dead_time_us) / (_CYHAL_PWM_US_PER_SEC * _CYHAL_PWM_MAX_DEAD_TIME_CYCLES)) + 1
                : (uint32_t)(1 << (_CYHAL_PWM_MAX_WIDTH - _CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].max_count));

            if (0 == div ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphSetDivider((cy_en_divider_types_t)obj->tcpwm.clock.block, obj->tcpwm.clock.channel, div - 1) ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphEnableDivider((cy_en_divider_types_t)obj->tcpwm.clock.block, obj->tcpwm.clock.channel) ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, (cy_en_divider_types_t)obj->tcpwm.clock.block, obj->tcpwm.clock.channel))
            {
                result = CYHAL_PWM_RSLT_FAILED_CLOCK_INIT;
            }
        }

        if (CY_RSLT_SUCCESS == result)
        {
            obj->tcpwm.clock_hz = cyhal_clock_get_frequency(&obj->tcpwm.clock);
        }
    }

    uint32_t pdl_alignment = CY_TCPWM_PWM_LEFT_ALIGN;
    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_pwm_convert_alignment(pwm_alignment, &pdl_alignment, swapped);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        uint8_t dead_time = (uint8_t)(dead_time_us * obj->tcpwm.clock_hz / _CYHAL_PWM_US_PER_SEC);

        cy_stc_tcpwm_pwm_config_t config =
        {
            .pwmMode           = (dead_time == 0) ? CY_TCPWM_PWM_MODE_PWM : CY_TCPWM_PWM_MODE_DEADTIME,
            .clockPrescaler    = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
            .pwmAlignment      = pdl_alignment,
            .deadTimeClocks    = dead_time,
            .runMode           = (continuous) ? CY_TCPWM_PWM_CONTINUOUS : CY_TCPWM_PWM_ONESHOT,
            .period0           = 0UL,
            .period1           = 0UL,
            .enablePeriodSwap  = false,
            .compare0          = 0UL,
            .compare1          = 0UL,
            .enableCompareSwap = false,
            .interruptSources  = CY_TCPWM_INT_NONE,
            .invertPWMOut      = (invert) ? CY_TCPWM_PWM_INVERT_ENABLE : CY_TCPWM_PWM_INVERT_DISABLE,
            .invertPWMOutN     = (invert) ? CY_TCPWM_PWM_INVERT_ENABLE : CY_TCPWM_PWM_INVERT_DISABLE,
            .killMode          = CY_TCPWM_PWM_STOP_ON_KILL,
            .swapInputMode     = CY_TCPWM_INPUT_RISINGEDGE,
            .swapInput         = CY_TCPWM_INPUT_0,
            .reloadInputMode   = CY_TCPWM_INPUT_RISINGEDGE,
            .reloadInput       = CY_TCPWM_INPUT_0,
            .startInputMode    = CY_TCPWM_INPUT_RISINGEDGE,
            .startInput        = CY_TCPWM_INPUT_0,
            .killInputMode     = CY_TCPWM_INPUT_RISINGEDGE,
            .killInput         = CY_TCPWM_INPUT_0,
            .countInputMode    = CY_TCPWM_INPUT_LEVEL,
            .countInput        = CY_TCPWM_INPUT_1
        };
        result = Cy_TCPWM_PWM_Init(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource), &config);
        if ((swapped) && (pwm_alignment == CYHAL_PWM_CENTER_ALIGN))
        {
            #if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
            uint32_t cntr_asym_swapped_reg_val = (0 == TCPWM_GRP_CNT_GET_GRP(_CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource))) ?
                /* Group 0 counters does not have CC1 */
                _CYHAL_PWM_MODE_CNTR_OR_ASYMM_UO_SWAPPED :
                _CYHAL_PWM_MODE_CNTR_OR_ASYMM_UO_SWAPPED | CY_TCPWM_PWM_MODE_CC1_IGNORE;
            TCPWM_GRP_CNT_TR_PWM_CTRL(obj->tcpwm.base, TCPWM_GRP_CNT_GET_GRP(_CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource)),
                    _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource)) = cntr_asym_swapped_reg_val;
            #else
            TCPWM_CNT_TR_CTRL2(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource)) = _CYHAL_PWM_MODE_CNTR_OR_ASYMM_UO_SWAPPED;
            #endif /* CY_IP_MXTCPWM_VERSION >= 2 or other */
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        _cyhal_tcpwm_init_data(&obj->tcpwm);
        Cy_TCPWM_PWM_Enable(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    }
    else
    {
        cyhal_pwm_free(obj);
    }

    return result;
}

void cyhal_pwm_free(cyhal_pwm_t *obj)
{
    CY_ASSERT(NULL != obj);
    _cyhal_utils_release_if_used(&obj->pin);
    _cyhal_utils_release_if_used(&obj->pin_compl);
    _cyhal_tcpwm_free(&obj->tcpwm);
}

cy_rslt_t cyhal_pwm_set_period(cyhal_pwm_t *obj, uint32_t period_us, uint32_t pulse_width_us)
{
    CY_ASSERT(NULL != obj);
    uint32_t period = (uint32_t)((uint64_t)period_us * obj->tcpwm.clock_hz / _CYHAL_PWM_US_PER_SEC);
    uint32_t width = (uint32_t)((uint64_t)pulse_width_us * obj->tcpwm.clock_hz / _CYHAL_PWM_US_PER_SEC);
    return cyhal_pwm_set_period_and_compare(obj, period, width);
}

cy_rslt_t cyhal_pwm_set_duty_cycle(cyhal_pwm_t *obj, float duty_cycle, uint32_t frequencyhal_hz)
{
    CY_ASSERT(NULL != obj);
    if (duty_cycle < 0.0f || duty_cycle > 100.0f || frequencyhal_hz < 1)
        return CYHAL_PWM_RSLT_BAD_ARGUMENT;
    uint32_t period = (obj->tcpwm.clock_hz + (frequencyhal_hz >> 1)) / frequencyhal_hz;
    uint32_t width = (uint32_t)(duty_cycle * 0.01f * period);
    return cyhal_pwm_set_period_and_compare(obj, period, width);
}

cy_rslt_t cyhal_pwm_start(cyhal_pwm_t *obj)
{
    CY_ASSERT(NULL != obj);
    if (_cyhal_tcpwm_pm_transition_pending())
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    Cy_TCPWM_PWM_Enable(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    #if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
    Cy_TCPWM_TriggerReloadOrIndex_Single(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    #else
    Cy_TCPWM_TriggerReloadOrIndex(obj->tcpwm.base, 1 << _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    #endif
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_pwm_stop(cyhal_pwm_t *obj)
{
    CY_ASSERT(NULL != obj);
    Cy_TCPWM_PWM_Disable(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    return CY_RSLT_SUCCESS;
}

static cyhal_tcpwm_input_t _cyhal_pwm_translate_input_signal(cyhal_pwm_input_t signal)
{
    switch(signal)
    {
        case CYHAL_PWM_INPUT_START:
            return CYHAL_TCPWM_INPUT_START;
        case CYHAL_PWM_INPUT_STOP:
            return CYHAL_TCPWM_INPUT_STOP;
        case CYHAL_PWM_INPUT_RELOAD:
            return CYHAL_TCPWM_INPUT_RELOAD;
        case CYHAL_PWM_INPUT_COUNT:
            return CYHAL_TCPWM_INPUT_COUNT;
        case CYHAL_PWM_INPUT_CAPTURE:
            return CYHAL_TCPWM_INPUT_CAPTURE;
    }
    CY_ASSERT(false);
    return (cyhal_tcpwm_input_t)0;
}

static cyhal_tcpwm_output_t _cyhal_pwm_translate_output_signal(cyhal_pwm_output_t signal)
{
    switch(signal)
    {
        case CYHAL_PWM_OUTPUT_OVERFLOW:
            return CYHAL_TCPWM_OUTPUT_OVERFLOW;
        case CYHAL_PWM_OUTPUT_UNDERFLOW:
            return CYHAL_TCPWM_OUTPUT_UNDERFLOW;
        case CYHAL_PWM_OUTPUT_COMPARE_MATCH:
            return CYHAL_TCPWM_OUTPUT_COMPARE_MATCH;
        case CYHAL_PWM_OUTPUT_LINE_OUT:
            return CYHAL_TCPWM_OUTPUT_LINE_OUT;
    }
    CY_ASSERT(false);
    return (cyhal_tcpwm_output_t)0;
}

cy_rslt_t cyhal_pwm_connect_digital(cyhal_pwm_t *obj, cyhal_source_t source, cyhal_pwm_input_t signal, cyhal_edge_type_t type)
{
    cyhal_tcpwm_input_t tcpwm_signal = _cyhal_pwm_translate_input_signal(signal);
    return _cyhal_tcpwm_connect_digital(&(obj->tcpwm), source, tcpwm_signal, type);
}

cy_rslt_t cyhal_pwm_enable_output(cyhal_pwm_t *obj, cyhal_pwm_output_t signal, cyhal_source_t *source)
{
    cyhal_tcpwm_output_t tcpwm_signal = _cyhal_pwm_translate_output_signal(signal);
    return _cyhal_tcpwm_enable_output(&(obj->tcpwm), tcpwm_signal, source);
}

cy_rslt_t cyhal_pwm_disconnect_digital(cyhal_pwm_t *obj, cyhal_source_t source, cyhal_pwm_input_t signal)
{
    return _cyhal_tcpwm_disconnect_digital(&(obj->tcpwm), source, _cyhal_pwm_translate_input_signal(signal));
}

cy_rslt_t cyhal_pwm_disable_output(cyhal_pwm_t *obj, cyhal_pwm_output_t signal)
{
    return _cyhal_tcpwm_disable_output(&(obj->tcpwm), _cyhal_pwm_translate_output_signal(signal));
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXTCPWM */
