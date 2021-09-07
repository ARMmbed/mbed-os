/*******************************************************************************
* File Name: cyhal_quaddec.c
*
* Description:
* Provides a high level interface for interacting with the Quadrature Decoder.
* This is a wrapper around the lower level PDL API.
*
********************************************************************************
* \copyright
* Copyright 2020-2021 Cypress Semiconductor Corporation
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
 * \addtogroup group_hal_impl_quaddec QuadDec (Quadrature Decoder)
 * \ingroup group_hal_impl
 * \{
 * \section section_hal_impl_quaddec_pins Selecting Pins for Quadrature Decoding
 * To identify the pins that are available to use with the Quadrature Decoder, open the PSoC
 * device datasheet and navigate to the 'Multiple Alternate Functions' table.  Any pin that has
 * the peri.tr_io_input function can be used.
 *
 * \} group_hal_impl_quaddec
 */

#if defined(COMPONENT_CAT2)
/**
 * \addtogroup group_hal_impl_quaddec QuadDec (Quadrature Decoder)
 * \ingroup group_hal_impl
 * \{
 * \note The cyhal_quaddec_connect_digital() and cyhal_quaddec_disconnect_digital() functions are
 * not supported with this device and will return a CYHAL_QUADDEC_RSLT_ERR_NOT_SUPPORTED error if
 * it is called.  This device does not have the internal circuitry routing that this function is
 * used to configure.
 *
 * \} group_hal_impl_quaddec
 */
#endif

#include "cy_tcpwm.h"
#include "cy_tcpwm_quaddec.h"
#include "cyhal_clock.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_interconnect.h"
#include "cyhal_quaddec.h"
#include "cyhal_syspm.h"
#include "cyhal_tcpwm_common.h"
#include "cyhal_utils.h"

#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define _CYHAL_CNT_NUM _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource)

#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
static cyhal_tcpwm_input_t _cyhal_quaddec_translate_input_signal(cyhal_quaddec_input_t signal);
#endif

static inline cy_rslt_t _cyhal_quaddec_configure_clock(cyhal_tcpwm_t *tcpwm, en_clk_dst_t pclk, uint32_t frequency)
{
    cy_rslt_t rslt;

    const cyhal_clock_tolerance_t tolerance = {
        .type = CYHAL_TOLERANCE_PERCENT,
        .value = 2,
    };

    #if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
    rslt = _cyhal_utils_allocate_clock(&tcpwm->clock, &tcpwm->resource, CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT, true);
    #elif defined(COMPONENT_CAT2)
    rslt = cyhal_clock_allocate(&tcpwm->clock, CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT);
    #endif

    if (rslt == CY_RSLT_SUCCESS)
    {
        tcpwm->dedicated_clock = true;

        if (cyhal_clock_set_frequency(&tcpwm->clock, frequency, &tolerance) == CY_RSLT_SUCCESS)
        {
            if (Cy_SysClk_PeriphAssignDivider(pclk, (cy_en_divider_types_t)tcpwm->clock.block, tcpwm->clock.channel) ==
                CY_SYSCLK_SUCCESS)
            {
                cyhal_clock_set_enabled(&tcpwm->clock, true, false);
                return CY_RSLT_SUCCESS;
            }
        }
    }

    return CYHAL_QUADDEC_RSLT_ERR_CLOCK_INIT;
}

#if defined(COMPONENT_CAT2)
//--------------------------------------------------------------------------------------------------
// _cyhal_quaddec_connect_pin
//
// NOTE: This function should be called after the pin has been initialized with cyhal_gpio_init().
//--------------------------------------------------------------------------------------------------
cy_rslt_t _cyhal_quaddec_connect_pin(cyhal_quaddec_input_t signal, cyhal_gpio_t pin,
                                     TCPWM_Type* base, uint8_t channel_num)
{
#if defined(CYHAL_PIN_MAP_TCPWM_TR_IN)
    bool found = false;
    uint8_t index;
    uint8_t array_size = sizeof(cyhal_pin_map_tcpwm_tr_in) / sizeof(cyhal_resource_pin_mapping_t);
    cyhal_resource_pin_mapping_t mapping;

    // Search through cyhal_pin_map_tcpwm_tr_in to determine if pin can be
    // used to drive a trigger line.
    for (index = 0; index < array_size; index++)
    {
        mapping = cyhal_pin_map_tcpwm_tr_in[index];

        if (mapping.pin == pin)
        {
            found = true;
            Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), mapping.hsiom);
            break;
        }
    }

    if (!found)
    {
        return CYHAL_GPIO_RSLT_ERR_NO_OUTPUT_SIGNAL;
    }

    switch (signal)
    {
        case CYHAL_QUADDEC_INPUT_PHI_A:
            TCPWM_CNT_TR_CTRL0(base, channel_num) &= ~TCPWM_CNT_TR_CTRL0_COUNT_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(base, channel_num) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_COUNT_SEL,
                                                              (uint32_t)(mapping.inst) + 2);
            TCPWM_CNT_TR_CTRL1(base, channel_num) &= ~TCPWM_CNT_TR_CTRL1_COUNT_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(base, channel_num) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_COUNT_EDGE,
                                                              CYHAL_EDGE_TYPE_RISING_EDGE);
            break;

        case CYHAL_QUADDEC_INPUT_PHI_B:
            TCPWM_CNT_TR_CTRL0(base, channel_num) &= ~TCPWM_CNT_TR_CTRL0_START_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(base, channel_num) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_START_SEL,
                                                              (uint32_t)(mapping.inst) + 2);
            TCPWM_CNT_TR_CTRL1(base, channel_num) &= ~TCPWM_CNT_TR_CTRL1_START_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(base, channel_num) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_START_EDGE,
                                                              CYHAL_EDGE_TYPE_RISING_EDGE);
            break;

        case CYHAL_QUADDEC_INPUT_INDEX:
            TCPWM_CNT_TR_CTRL0(base, channel_num) &= ~TCPWM_CNT_TR_CTRL0_RELOAD_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(base, channel_num) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_RELOAD_SEL,
                                                              (uint32_t)(mapping.inst) + 2);
            TCPWM_CNT_TR_CTRL1(base, channel_num) &= ~TCPWM_CNT_TR_CTRL1_RELOAD_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(base, channel_num) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_RELOAD_EDGE,
                                                              CYHAL_EDGE_TYPE_RISING_EDGE);
            break;

        default:
            return CYHAL_GPIO_RSLT_ERR_NO_OUTPUT_SIGNAL;
    }

    return CY_RSLT_SUCCESS;
#else
    CY_UNUSED_PARAMETER(signal);
    CY_UNUSED_PARAMETER(pin);
    CY_UNUSED_PARAMETER(base);
    CY_UNUSED_PARAMETER(channel_num);

    return CYHAL_GPIO_RSLT_ERR_NO_OUTPUT_SIGNAL;
#endif /* defined(CYHAL_PIN_MAP_TCPWM_TR_IN) */
}
#endif

cy_rslt_t cyhal_quaddec_init(cyhal_quaddec_t *obj, cyhal_gpio_t phi_a, cyhal_gpio_t phi_b,
                             cyhal_gpio_t index, cyhal_quaddec_resolution_t resolution,
                             const cyhal_clock_t *clk, uint32_t frequency)
{
    CY_ASSERT(obj != NULL);

    cy_rslt_t rslt = CY_RSLT_SUCCESS;

    // Initialize the quadrature object
    // Explicitly marked not allocated resources as invalid to prevent freeing them.
    memset(obj, 0, sizeof(cyhal_quaddec_t));
    obj->phi_a = NC;
    obj->phi_b = NC;
    obj->index = NC;
    obj->tcpwm.resource.type = CYHAL_RSC_INVALID;
    obj->last_counter_value = 1 << (_CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].max_count - 1);

    // Allocate TCPWM resource
    rslt = cyhal_hwmgr_allocate(CYHAL_RSC_TCPWM, &obj->tcpwm.resource);

    // Basic configuration of TCPWM for quadrature functionality
    if (rslt == CY_RSLT_SUCCESS)
    {
        cy_stc_tcpwm_quaddec_config_t config;

        obj->tcpwm.base = _CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].base;
        _cyhal_tcpwm_init_data(&obj->tcpwm);

        memset(&config, 0, sizeof(cy_stc_tcpwm_quaddec_config_t));

        config.resolution = resolution;

        #if (CY_IP_MXTCPWM_VERSION >= 2U)
        config.phiAInputMode  = CY_TCPWM_INPUT_LEVEL;                // Pass thorugh (no edge detection)
        config.phiBInputMode  = CY_TCPWM_INPUT_LEVEL;                // Pass thorugh (no edge detection)
        config.trigger0Event  = CY_TCPWM_CNT_TRIGGER_ON_DISABLED;    // Disable output trigger0 event generation
        config.trigger1Event  = CY_TCPWM_CNT_TRIGGER_ON_DISABLED;    // Disable output trigger1 event generation
        #endif

        // This function is only used to set the resolution and configure the TCPWM for quadrature mode.
        // It also configures the inputs, but that will be overwritten by cyhal_quaddec_connect_digital().
        // So, this must be called before that function is called.
        rslt = Cy_TCPWM_QuadDec_Init(obj->tcpwm.base, _CYHAL_CNT_NUM, &config);
    }

    // Pin configuration
    if (rslt == CY_RSLT_SUCCESS)
    {
        rslt = cyhal_gpio_init(phi_a, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);

        if (rslt == CY_RSLT_SUCCESS)
        {
            obj->phi_a = phi_a;
            #if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
            uint8_t idx = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_PHI_A);
            rslt = cyhal_gpio_enable_output(phi_a, &(obj->tcpwm.inputs[idx]));
            #endif
        }

        if (rslt == CY_RSLT_SUCCESS)
        {
            rslt = cyhal_gpio_init(phi_b, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
        }

        if (rslt == CY_RSLT_SUCCESS)
        {
            obj->phi_b = phi_b;
            #if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
            uint8_t idx = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_PHI_B);
            rslt = cyhal_gpio_enable_output(phi_b, &(obj->tcpwm.inputs[idx]));
            #endif
        }

        if (index != NC)
        {
            if (rslt == CY_RSLT_SUCCESS)
            {
                rslt = cyhal_gpio_init(index, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
            }

            if (rslt == CY_RSLT_SUCCESS)
            {
                obj->index = index;
                #if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
                uint8_t idx = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_INDEX);
                rslt = cyhal_gpio_enable_output(index, &(obj->tcpwm.inputs[idx]));
                #endif
            }
        }
        #if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
        else
        {
            uint8_t idx = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_INDEX);
            obj->tcpwm.inputs[idx] = CYHAL_TRIGGER_CPUSS_ZERO;
        }
        #endif

        if (rslt != CY_RSLT_SUCCESS)
        {
            rslt = CYHAL_QUADDEC_RSLT_ERR_BAD_ARGUMENT;
        }
    }

    #if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)

    // Internal signal configuration
    if (rslt == CY_RSLT_SUCCESS)
    {
        uint8_t idx = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_PHI_A);
        rslt = cyhal_quaddec_connect_digital(obj, obj->tcpwm.inputs[idx], CYHAL_QUADDEC_INPUT_PHI_A);
        if (rslt != CY_RSLT_SUCCESS)
        {
            obj->tcpwm.inputs[idx] = CYHAL_TRIGGER_CPUSS_ZERO;
        }
    }

    if (rslt == CY_RSLT_SUCCESS)
    {
        uint8_t idx = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_PHI_B);
        rslt = cyhal_quaddec_connect_digital(obj, obj->tcpwm.inputs[idx], CYHAL_QUADDEC_INPUT_PHI_B);
        if (rslt != CY_RSLT_SUCCESS)
        {
            obj->tcpwm.inputs[idx] = CYHAL_TRIGGER_CPUSS_ZERO;
        }
    }

    if ((rslt == CY_RSLT_SUCCESS) && (index != NC))
    {
        uint8_t idx = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_INDEX);
        rslt = cyhal_quaddec_connect_digital(obj, obj->tcpwm.inputs[idx], CYHAL_QUADDEC_INPUT_INDEX);
        if (rslt != CY_RSLT_SUCCESS)
        {
            obj->tcpwm.inputs[idx] = CYHAL_TRIGGER_CPUSS_ZERO;
        }
    }

    #elif defined(COMPONENT_CAT2)

    if (rslt == CY_RSLT_SUCCESS)
    {
        rslt = _cyhal_quaddec_connect_pin(CYHAL_QUADDEC_INPUT_PHI_A, obj->phi_a, obj->tcpwm.base,
                                          obj->tcpwm.resource.channel_num);
    }

    if (rslt == CY_RSLT_SUCCESS)
    {
        rslt = _cyhal_quaddec_connect_pin(CYHAL_QUADDEC_INPUT_PHI_B, obj->phi_b, obj->tcpwm.base,
                                          obj->tcpwm.resource.channel_num);
    }

    if ((rslt == CY_RSLT_SUCCESS) && (index != NC))
    {
        rslt = _cyhal_quaddec_connect_pin(CYHAL_QUADDEC_INPUT_INDEX, obj->index, obj->tcpwm.base,
                                          obj->tcpwm.resource.channel_num);
    }

    #endif

    // Clock configuration
    if (rslt == CY_RSLT_SUCCESS)
    {
        en_clk_dst_t pclk =
            (en_clk_dst_t)(_CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].clock_dst +
                           obj->tcpwm.resource.channel_num);

        if (clk != NULL)
        {
            if (frequency != 0)
            {
                rslt = CYHAL_QUADDEC_RSLT_ERR_BAD_ARGUMENT;
            }
            else
            {
                obj->tcpwm.clock = *clk;
                _cyhal_utils_update_clock_format(&obj->tcpwm.clock);

                if (Cy_SysClk_PeriphAssignDivider(pclk, (cy_en_divider_types_t)obj->tcpwm.clock.block,
                                                  obj->tcpwm.clock.channel) != CY_SYSCLK_SUCCESS)
                {
                    rslt = CYHAL_QUADDEC_RSLT_ERR_CLOCK_INIT;
                }
            }
        }
        else
        {
            rslt = _cyhal_quaddec_configure_clock(&obj->tcpwm, pclk, frequency);
        }

        if (rslt == CY_RSLT_SUCCESS)
        {
            obj->tcpwm.clock_hz = cyhal_clock_get_frequency(&obj->tcpwm.clock);
        }
    }

    // Enable the quadrature function
    if (rslt == CY_RSLT_SUCCESS)
    {
        Cy_TCPWM_QuadDec_Enable(obj->tcpwm.base, _CYHAL_CNT_NUM);
    }
    else
    {
        cyhal_quaddec_free(obj);
    }

    return rslt;
}

void cyhal_quaddec_free(cyhal_quaddec_t *obj)
{
    CY_ASSERT(obj != NULL);
    #if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
    uint8_t idx_phi_a = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_PHI_A);
    if ((obj->phi_a != NC) && (obj->tcpwm.inputs[idx_phi_a] != CYHAL_TRIGGER_CPUSS_ZERO))
    {
        cyhal_quaddec_disconnect_digital(obj, obj->tcpwm.inputs[idx_phi_a], CYHAL_QUADDEC_INPUT_PHI_A);
    }
    uint8_t idx_phi_b = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_PHI_B);
    if ((obj->phi_b != NC) && (obj->tcpwm.inputs[idx_phi_b] != CYHAL_TRIGGER_CPUSS_ZERO))
    {
        cyhal_quaddec_disconnect_digital(obj, obj->tcpwm.inputs[idx_phi_b], CYHAL_QUADDEC_INPUT_PHI_B);
    }
    uint8_t idx_index = (uint8_t)_cyhal_quaddec_translate_input_signal(CYHAL_QUADDEC_INPUT_INDEX);
    if ((obj->index != NC) && (obj->tcpwm.inputs[idx_index] != CYHAL_TRIGGER_CPUSS_ZERO))
    {
        cyhal_quaddec_disconnect_digital(obj, obj->tcpwm.inputs[idx_index], CYHAL_QUADDEC_INPUT_INDEX);
    }
    #endif
    _cyhal_utils_release_if_used(&obj->phi_a);
    _cyhal_utils_release_if_used(&obj->phi_b);
    _cyhal_utils_release_if_used(&obj->index);
    _cyhal_tcpwm_free(&obj->tcpwm);
}

cy_rslt_t cyhal_quaddec_start(cyhal_quaddec_t *obj)
{
    CY_ASSERT(obj != NULL);
    if (_cyhal_tcpwm_pm_transition_pending())
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    Cy_TCPWM_QuadDec_Enable(obj->tcpwm.base, _CYHAL_CNT_NUM);

    // Trigger reload/index the QuadDec.
    // From the TRM, "A reload trigger must be provided through firmware to start the counter if
    // the hardware reload signal is not enabled."
    #if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
    Cy_TCPWM_TriggerReloadOrIndex_Single(obj->tcpwm.base, _CYHAL_CNT_NUM);
    #else
    Cy_TCPWM_TriggerReloadOrIndex(obj->tcpwm.base, 1 << _CYHAL_CNT_NUM);
    #endif

    obj->last_counter_value = 1 << (_CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].max_count - 1);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_quaddec_stop(cyhal_quaddec_t *obj)
{
    CY_ASSERT(obj != NULL);
    Cy_TCPWM_QuadDec_Disable(obj->tcpwm.base, _CYHAL_CNT_NUM);
    return CY_RSLT_SUCCESS;
}

int32_t cyhal_quaddec_get_delta(cyhal_quaddec_t *obj)
{
    CY_ASSERT(obj != NULL);

    uint32_t midpoint = 1 << (_CYHAL_TCPWM_DATA[obj->tcpwm.resource.block_num].max_count - 1);
    uint32_t max_delta = midpoint >> 1;
    uint32_t max_counter_value = (midpoint == 0x8000) ? 0xFFFF : 0xFFFFFFFF;
    uint32_t current_counter_value = Cy_TCPWM_QuadDec_GetCounter(obj->tcpwm.base, _CYHAL_CNT_NUM);
    int32_t  delta = current_counter_value - obj->last_counter_value;

    // Overflow has occurred
    if (delta < (int32_t)(-max_delta))
    {
        delta = max_counter_value - obj->last_counter_value;
        delta += current_counter_value - midpoint;
    }
    // Underflow has occurred
    else if (delta > (int32_t)max_delta)
    {
        delta = (int32_t)(-obj->last_counter_value);
        delta += current_counter_value - midpoint;
    }

    obj->last_counter_value = current_counter_value;

    return delta;
}

uint32_t cyhal_quaddec_read_counter(const cyhal_quaddec_t *obj)
{
    CY_ASSERT(obj != NULL);
    return Cy_TCPWM_QuadDec_GetCounter(obj->tcpwm.base, _CYHAL_CNT_NUM);
}

uint32_t cyhal_quaddec_read_capture(const cyhal_quaddec_t *obj)
{
    CY_ASSERT(obj != NULL);
    #if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
    return Cy_TCPWM_QuadDec_GetCapture0Val(obj->tcpwm.base, _CYHAL_CNT_NUM);
    #else
    return Cy_TCPWM_QuadDec_GetCapture(obj->tcpwm.base, _CYHAL_CNT_NUM);
    #endif
}

#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)

static cyhal_tcpwm_input_t _cyhal_quaddec_translate_input_signal(cyhal_quaddec_input_t signal)
{
    switch (signal)
    {
        case CYHAL_QUADDEC_INPUT_PHI_A:
            return CYHAL_TCPWM_INPUT_COUNT;
        case CYHAL_QUADDEC_INPUT_PHI_B:
            return CYHAL_TCPWM_INPUT_START;
        case CYHAL_QUADDEC_INPUT_STOP:
            return CYHAL_TCPWM_INPUT_STOP;
        case CYHAL_QUADDEC_INPUT_INDEX:
            return CYHAL_TCPWM_INPUT_RELOAD;
    }
    CY_ASSERT(false);
    return (cyhal_tcpwm_input_t)0;
}

cy_rslt_t cyhal_quaddec_connect_digital(cyhal_quaddec_t *obj, cyhal_source_t source, cyhal_quaddec_input_t signal)
{
    cyhal_tcpwm_input_t tcpwm_signal;
    cyhal_edge_type_t input_type;

    switch (signal)
    {
        case CYHAL_QUADDEC_INPUT_PHI_A:
        case CYHAL_QUADDEC_INPUT_PHI_B:
            input_type = CYHAL_EDGE_TYPE_LEVEL;
            break;
        case CYHAL_QUADDEC_INPUT_STOP:
        case CYHAL_QUADDEC_INPUT_INDEX:
            input_type = CYHAL_EDGE_TYPE_RISING_EDGE;
            break;
        default:
            return CYHAL_QUADDEC_RSLT_ERR_BAD_ARGUMENT;
    }

    tcpwm_signal = _cyhal_quaddec_translate_input_signal(signal);

    return _cyhal_tcpwm_connect_digital(&(obj->tcpwm), source, tcpwm_signal, input_type);
}

cy_rslt_t cyhal_quaddec_disconnect_digital(cyhal_quaddec_t *obj, cyhal_source_t source, cyhal_quaddec_input_t signal)
{
    return _cyhal_tcpwm_disconnect_digital(&(obj->tcpwm), source,
                                           _cyhal_quaddec_translate_input_signal(signal));
}

#elif defined(COMPONENT_CAT2)

cy_rslt_t cyhal_quaddec_connect_digital(cyhal_quaddec_t *obj, cyhal_source_t source, cyhal_quaddec_input_t signal)
{
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(source);
    CY_UNUSED_PARAMETER(signal);
    return CYHAL_QUADDEC_RSLT_ERR_NOT_SUPPORTED;
}

cy_rslt_t cyhal_quaddec_disconnect_digital(cyhal_quaddec_t *obj, cyhal_source_t source, cyhal_quaddec_input_t signal)
{
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(source);
    CY_UNUSED_PARAMETER(signal);
    return CYHAL_QUADDEC_RSLT_ERR_NOT_SUPPORTED;
}

#endif

static cyhal_tcpwm_output_t _cyhal_quaddec_translate_output_signal(cyhal_quaddec_output_t signal)
{
    switch (signal)
    {
        case CYHAL_QUADDEC_OUTPUT_COMPARE_MATCH:
            return CYHAL_TCPWM_OUTPUT_COMPARE_MATCH;
    }
    CY_ASSERT(false);
    return (cyhal_tcpwm_output_t)0;
}

cy_rslt_t cyhal_quaddec_enable_output(cyhal_quaddec_t *obj, cyhal_quaddec_output_t signal, cyhal_source_t *source)
{
    if (signal != CYHAL_QUADDEC_OUTPUT_COMPARE_MATCH)
    {
        return CYHAL_QUADDEC_RSLT_ERR_BAD_ARGUMENT;
    }

    return _cyhal_tcpwm_enable_output(&(obj->tcpwm), _cyhal_quaddec_translate_output_signal(signal), source);
}

cy_rslt_t cyhal_quaddec_disable_output(cyhal_quaddec_t *obj, cyhal_quaddec_output_t signal)
{
    if (signal != CYHAL_QUADDEC_OUTPUT_COMPARE_MATCH)
        return CYHAL_QUADDEC_RSLT_ERR_BAD_ARGUMENT;

    return _cyhal_tcpwm_disable_output(&(obj->tcpwm), _cyhal_quaddec_translate_output_signal(signal));
}
