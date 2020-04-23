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

#include "cyhal_timer_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_gpio.h"

#if defined(CY_IP_MXTCPWM_INSTANCES)

#if defined(__cplusplus)
extern "C" {
#endif

static const cy_stc_tcpwm_counter_config_t default_config =
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
static inline uint32_t convert_direction(cyhal_timer_direction_t direction)
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

cy_rslt_t cyhal_timer_init(cyhal_timer_t *obj, cyhal_gpio_t pin, const cyhal_clock_divider_t *clk)
{
    CY_ASSERT(NULL != obj);

    // No support currently for pin connections on this device
    if (CYHAL_NC_PIN_VALUE != pin)
        return CYHAL_TIMER_RSLT_ERR_BAD_ARGUMENT;

    cy_rslt_t result = cyhal_hwmgr_allocate(CYHAL_RSC_TCPWM, &obj->resource);
    if (CY_RSLT_SUCCESS == result)
    {
        cyhal_resource_inst_t *timer = &obj->resource;

        obj->base = CYHAL_TCPWM_DATA[timer->block_num].base;

        en_clk_dst_t pclk = (en_clk_dst_t)(CYHAL_TCPWM_DATA[timer->block_num].clock_dst + timer->channel_num);

        if (NULL != clk)
        {
            obj->clock = *clk;
            obj->dedicated_clock = false;
            obj->clock_hz = Cy_SysClk_ClkPeriGetFrequency() / (1 + Cy_SysClk_PeriphGetDivider(obj->clock.div_type, obj->clock.div_num));
            if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, clk->div_type, clk->div_num))
            {
                result = CYHAL_TIMER_RSLT_ERR_CLOCK_INIT;
            }
        }
        else if (CY_RSLT_SUCCESS == (result = cyhal_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_16_BIT, false)))
        {
            obj->dedicated_clock = true;
            result = cyhal_timer_set_frequency(obj, CYHAL_TIMER_DEFAULT_FREQ);
            if(CY_RSLT_SUCCESS == result)
            {
                if(CY_RSLT_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, obj->clock.div_type, obj->clock.div_num))
                {
                    result = CYHAL_TIMER_RSLT_ERR_CLOCK_INIT;
                }
            }
        }

        if (CY_RSLT_SUCCESS == result)
        {
            result = Cy_TCPWM_Counter_Init(obj->base, obj->resource.channel_num, &default_config);
        }

        if (result == CY_RSLT_SUCCESS)
        {
            cyhal_tcpwm_init_callback_data(&(obj->resource), &(obj->callback_data));
            Cy_TCPWM_SetInterruptMask(obj->base, obj->resource.channel_num, CY_TCPWM_INT_NONE);
            Cy_TCPWM_Counter_Enable(obj->base, obj->resource.channel_num);
        }
        else
        {
            cyhal_timer_free(obj);
        }
    }

    return result;
}

void cyhal_timer_free(cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);

    IRQn_Type irqn = (IRQn_Type)(CYHAL_TCPWM_DATA[obj->resource.block_num].isr_offset + obj->resource.channel_num);
    NVIC_DisableIRQ(irqn);

    if (NULL != obj->base)
    {
        Cy_TCPWM_Counter_Disable(obj->base, obj->resource.channel_num);

        cyhal_hwmgr_free(&obj->resource);
        obj->base = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;

        if (obj->dedicated_clock)
        {
            cy_en_sysclk_status_t rslt = Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);
            CY_UNUSED_PARAMETER(rslt); /* CY_ASSERT only processes in DEBUG, ignores for others */
            CY_ASSERT(CY_SYSCLK_SUCCESS == rslt);
            cyhal_hwmgr_free_clock(&(obj->clock));
            obj->dedicated_clock = false;
        }
    }
}

cy_rslt_t cyhal_timer_configure(cyhal_timer_t *obj, const cyhal_timer_cfg_t *cfg)
{
    cy_rslt_t rslt;
    if (obj->is_continuous != cfg->is_continuous ||
        obj->direction != cfg->direction ||
        obj->is_compare != cfg->is_compare)
    {
        cy_stc_tcpwm_counter_config_t config = default_config;
        config.period = cfg->period;
        config.compare0 = cfg->compare_value;
        config.runMode = cfg->is_continuous ? CY_TCPWM_COUNTER_CONTINUOUS : CY_TCPWM_COUNTER_ONESHOT;
        config.compareOrCapture = cfg->is_compare ? CY_TCPWM_COUNTER_MODE_COMPARE : CY_TCPWM_COUNTER_MODE_CAPTURE;
        config.countDirection = convert_direction(cfg->direction);
        // DeInit will clear the interrupt mask; save it now and restore after we re-nit
        uint32_t old_mask = Cy_TCPWM_GetInterruptMask(obj->base, obj->resource.channel_num);
        Cy_TCPWM_Counter_DeInit(obj->base, obj->resource.channel_num, &config);
        rslt = (cy_rslt_t)Cy_TCPWM_Counter_Init(obj->base, obj->resource.channel_num, &config);
        Cy_TCPWM_SetInterruptMask(obj->base, obj->resource.channel_num, old_mask);
        if (CY_TCPWM_SUCCESS == rslt)
        {
            obj->is_continuous = cfg->is_continuous;
            obj->direction = cfg->direction;
            obj->is_compare = cfg->is_compare;
        }
    }
    else
    {
        Cy_TCPWM_Counter_SetCounter(obj->base, obj->resource.channel_num, cfg->value);
        Cy_TCPWM_Counter_SetPeriod(obj->base, obj->resource.channel_num, cfg->period);
        if (cfg->is_compare)
        {
            Cy_TCPWM_Counter_SetCompare0(obj->base, obj->resource.channel_num, cfg->compare_value);
        }
        rslt = CY_RSLT_SUCCESS;
    }

    return rslt;
}

cy_rslt_t cyhal_timer_set_frequency(cyhal_timer_t *obj, uint32_t hz)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    if(false == obj->dedicated_clock)
    {
        result = CYHAL_TIMER_RSLT_ERR_SHARED_CLOCK;
    }

    if(CY_RSLT_SUCCESS == result)
    {
        uint32_t div = Cy_SysClk_ClkPeriGetFrequency() / hz;
        if (0 == div ||
            CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num) ||
            CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, div - 1) ||
            CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num))
        {
            result = CYHAL_TIMER_RSLT_ERR_CLOCK_INIT;
        }
        else
        {
            obj->clock_hz = Cy_SysClk_ClkPeriGetFrequency() / div;
        }
    }

    return result;
}

cy_rslt_t cyhal_timer_start(cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);
    Cy_TCPWM_TriggerReloadOrIndex(obj->base, 1u << obj->resource.channel_num);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_timer_stop(cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);
    Cy_TCPWM_TriggerStopOrKill(obj->base, 1u << obj->resource.channel_num);
    return CY_RSLT_SUCCESS;
}

uint32_t cyhal_timer_read(const cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);
    return Cy_TCPWM_Counter_GetCounter(obj->base, obj->resource.channel_num);
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
