/***************************************************************************//**
* \file cyhal_timer.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Timer/Counter.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
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
#include "cy_device_headers.h"
#include "cyhal_timer_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_gpio.h"
#include "cyhal_interconnect.h"
#include "cyhal_syspm.h"
#include "cyhal_clock.h"

#if defined(CY_IP_MXTCPWM_INSTANCES) || defined(CY_IP_M0S8TCPWM_INSTANCES)

#if defined(__cplusplus)
extern "C" {
#endif

static const cy_stc_tcpwm_counter_config_t _cyhal_timer_default_config =
{
    .period = 32768,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_NONE,
    .captureInputMode = 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
};

/** Convert timer direction from the HAL enum to the corresponding PDL constant
  *
  * @param[in] direction The direction, as a HAL enum value
  * @return The direction, as a PDL constant
  */
static inline uint32_t _cyhal_timer_convert_direction(cyhal_timer_direction_t direction)
{
    switch (direction)
    {
        case CYHAL_TIMER_DIR_UP:
            return CY_TCPWM_COUNTER_COUNT_UP;
        case CYHAL_TIMER_DIR_DOWN:
            return CY_TCPWM_COUNTER_COUNT_DOWN;
        case CYHAL_TIMER_DIR_UP_DOWN:
            return CY_TCPWM_COUNTER_COUNT_UP_DOWN_2;
    }
    return CY_TCPWM_COUNTER_COUNT_UP;
}

/*******************************************************************************
*       Timer HAL Functions
*******************************************************************************/

cy_rslt_t cyhal_timer_init(cyhal_timer_t *obj, cyhal_gpio_t pin, const cyhal_clock_t *clk)
{
    CY_ASSERT(NULL != obj);

    // No support currently for pin connections on this device
    if (CYHAL_NC_PIN_VALUE != pin)
        return CYHAL_TIMER_RSLT_ERR_BAD_ARGUMENT;

    memset(obj, 0, sizeof(cyhal_timer_t));
    cy_rslt_t result = cyhal_hwmgr_allocate(CYHAL_RSC_TCPWM, &obj->tcpwm.resource);
    if (CY_RSLT_SUCCESS == result)
    {
        cyhal_resource_inst_t *timer = &obj->tcpwm.resource;
        obj->tcpwm.base = _CYHAL_TCPWM_DATA[timer->block_num].base;
        en_clk_dst_t pclk = (en_clk_dst_t)(_CYHAL_TCPWM_DATA[timer->block_num].clock_dst + timer->channel_num);

        if (NULL != clk)
        {
            obj->tcpwm.clock = *clk;
            _cyhal_utils_update_clock_format(&obj->tcpwm.clock);
            obj->tcpwm.clock_hz = cyhal_clock_get_frequency(&obj->tcpwm.clock);
            if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, (cy_en_divider_types_t)obj->tcpwm.clock.block, obj->tcpwm.clock.channel))
            {
                result = CYHAL_TIMER_RSLT_ERR_CLOCK_INIT;
            }
        }
        else if (CY_RSLT_SUCCESS == (result = cyhal_clock_allocate(&(obj->tcpwm.clock), CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT)))
        {
            obj->tcpwm.dedicated_clock = true;
            result = cyhal_timer_set_frequency(obj, CYHAL_TIMER_DEFAULT_FREQ);
            if (CY_RSLT_SUCCESS == result)
            {
                if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, (cy_en_divider_types_t)obj->tcpwm.clock.block, obj->tcpwm.clock.channel))
                {
                    result = CYHAL_TIMER_RSLT_ERR_CLOCK_INIT;
                }
            }
        }

        if (CY_RSLT_SUCCESS == result)
        {
            result = Cy_TCPWM_Counter_Init(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource), &_cyhal_timer_default_config);
        }

        if (result == CY_RSLT_SUCCESS)
        {
            _cyhal_tcpwm_init_data(&obj->tcpwm);
            Cy_TCPWM_SetInterruptMask(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource), CY_TCPWM_INT_NONE);
            Cy_TCPWM_Counter_Enable(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
        }
        else
        {
            cyhal_timer_free(obj);
        }
    }

    return result;
}

cy_rslt_t cyhal_timer_configure(cyhal_timer_t *obj, const cyhal_timer_cfg_t *cfg)
{
    cy_rslt_t rslt;
    obj->default_value = cfg->value;
    cy_stc_tcpwm_counter_config_t config = _cyhal_timer_default_config;
    config.period = cfg->period;
    config.compare0 = cfg->compare_value;
    config.runMode = cfg->is_continuous ? CY_TCPWM_COUNTER_CONTINUOUS : CY_TCPWM_COUNTER_ONESHOT;
    config.compareOrCapture = cfg->is_compare ? CY_TCPWM_COUNTER_MODE_COMPARE : CY_TCPWM_COUNTER_MODE_CAPTURE;
    config.countDirection = _cyhal_timer_convert_direction(cfg->direction);
    // DeInit will clear the interrupt mask; save it now and restore after we re-nit
    uint32_t old_mask = Cy_TCPWM_GetInterruptMask(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    Cy_TCPWM_Counter_DeInit(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource), &config);
    rslt = (cy_rslt_t)Cy_TCPWM_Counter_Init(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource), &config);
    Cy_TCPWM_Counter_Enable(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    Cy_TCPWM_SetInterruptMask(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource), old_mask);

    // This must be called after Cy_TCPWM_Counter_Init
    cyhal_timer_reset(obj);

    return rslt;
}

cy_rslt_t cyhal_timer_set_frequency(cyhal_timer_t *obj, uint32_t hz)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    if(!obj->tcpwm.dedicated_clock)
    {
        result = CYHAL_TIMER_RSLT_ERR_SHARED_CLOCK;
    }

    const cyhal_clock_tolerance_t tolerance = {
        .type = CYHAL_TOLERANCE_PERCENT,
        .value = 2,
    };
    if(CY_RSLT_SUCCESS == result)
    {
        if((CY_RSLT_SUCCESS == cyhal_clock_set_enabled(&obj->tcpwm.clock, false, false)) &&
           (CY_RSLT_SUCCESS == cyhal_clock_set_frequency(&obj->tcpwm.clock, hz, &tolerance)) &&
           (CY_RSLT_SUCCESS == cyhal_clock_set_enabled(&obj->tcpwm.clock, true, false)))
        {
            obj->tcpwm.clock_hz = cyhal_clock_get_frequency(&obj->tcpwm.clock);
        }
        else
        {
            result = CYHAL_TIMER_RSLT_ERR_CLOCK_INIT;
        }
    }

    return result;
}

cy_rslt_t cyhal_timer_start(cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);
    if (_cyhal_tcpwm_pm_transition_pending())
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    Cy_TCPWM_Counter_Enable(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    #if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
    Cy_TCPWM_TriggerStart_Single(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    #else
    Cy_TCPWM_TriggerStart(obj->tcpwm.base, (1 << _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource)));
    #endif
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_timer_stop(cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);
    Cy_TCPWM_Counter_Disable(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_timer_reset(cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);
    Cy_TCPWM_Counter_SetCounter(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource), obj->default_value);

    return CY_RSLT_SUCCESS;
}

uint32_t cyhal_timer_read(const cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);
    return Cy_TCPWM_Counter_GetCounter(obj->tcpwm.base, _CYHAL_TCPWM_CNT_NUMBER(obj->tcpwm.resource));
}

static cyhal_tcpwm_input_t _cyhal_timer_translate_input_signal(cyhal_timer_input_t event)
{
    switch(event)
    {
        case CYHAL_TIMER_INPUT_START:
            return CYHAL_TCPWM_INPUT_START;
        case CYHAL_TIMER_INPUT_STOP:
            return CYHAL_TCPWM_INPUT_STOP;
        case CYHAL_TIMER_INPUT_RELOAD:
            return CYHAL_TCPWM_INPUT_RELOAD;
        case CYHAL_TIMER_INPUT_COUNT:
            return CYHAL_TCPWM_INPUT_COUNT;
        case CYHAL_TIMER_INPUT_CAPTURE:
            return CYHAL_TCPWM_INPUT_CAPTURE;
    }
    CY_ASSERT(false);
    return (cyhal_tcpwm_input_t)0;
}

static cyhal_tcpwm_output_t _cyhal_timer_translate_output_signal(cyhal_timer_output_t signal)
{
    switch(signal)
    {
        case CYHAL_TIMER_OUTPUT_OVERFLOW:
            return CYHAL_TCPWM_OUTPUT_OVERFLOW;
        case CYHAL_TIMER_OUTPUT_UNDERFLOW:
            return CYHAL_TCPWM_OUTPUT_UNDERFLOW;
        case CYHAL_TIMER_OUTPUT_COMPARE_MATCH:
            return CYHAL_TCPWM_OUTPUT_COMPARE_MATCH;
        case CYHAL_TIMER_OUTPUT_TERMINAL_COUNT:
            return CYHAL_TCPWM_OUTPUT_TERMINAL_COUNT;
    }
    CY_ASSERT(false);
    return (cyhal_tcpwm_output_t)0;
}

cy_rslt_t cyhal_timer_connect_digital(cyhal_timer_t *obj, cyhal_source_t source, cyhal_timer_input_t signal, cyhal_edge_type_t type)
{
    cyhal_tcpwm_input_t tcpwm_signal = _cyhal_timer_translate_input_signal(signal);
    return _cyhal_tcpwm_connect_digital(&(obj->tcpwm), source, tcpwm_signal, type);
}

cy_rslt_t cyhal_timer_enable_output(cyhal_timer_t *obj, cyhal_timer_output_t signal, cyhal_source_t *source)
{
    cyhal_tcpwm_output_t tcpwm_signal = _cyhal_timer_translate_output_signal(signal);
    return _cyhal_tcpwm_enable_output(&(obj->tcpwm), tcpwm_signal, source);
}

cy_rslt_t cyhal_timer_disconnect_digital(cyhal_timer_t *obj, cyhal_source_t source, cyhal_timer_input_t signal)
{
    return _cyhal_tcpwm_disconnect_digital(&(obj->tcpwm), source, _cyhal_timer_translate_input_signal(signal));
}

cy_rslt_t cyhal_timer_disable_output(cyhal_timer_t *obj, cyhal_timer_output_t signal)
{
    return _cyhal_tcpwm_disable_output(&(obj->tcpwm), _cyhal_timer_translate_output_signal(signal));
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
