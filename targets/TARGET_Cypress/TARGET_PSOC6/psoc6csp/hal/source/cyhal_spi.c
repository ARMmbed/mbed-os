/***************************************************************************//**
* \file cyhal_spi.c
*
* \brief
* Provides a high level interface for interacting with the Cypress SPI. This is
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
#include "cyhal_spi.h"
#include "cyhal_scb_common.h"
#include "cyhal_gpio.h"
#include "cyhal_system_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_system.h"
#include "cyhal_syspm.h"
#include "cyhal_clock.h"

#if defined(CY_IP_MXSCB) || defined(CY_IP_M0S8SCB)

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_SPI_DEFAULT_SPEED            100000

#define _CYHAL_SPI_OVERSAMPLE_MIN           4
#define _CYHAL_SPI_OVERSAMPLE_MAX           16
#define _CYHAL_SPI_SSEL_NUM                 4

#define _CYHAL_SPI_PENDING_NONE             0
#define _CYHAL_SPI_PENDING_RX               1
#define _CYHAL_SPI_PENDING_TX               2
#define _CYHAL_SPI_PENDING_TX_RX            3

#define _CYHAL_SPI_SSEL_ACTIVATE            true
#define _CYHAL_SPI_SSEL_DEACTIVATE          false

/* Default SPI configuration */
static const cy_stc_scb_spi_config_t _cyhal_spi_default_config =
{
    .spiMode                  = CY_SCB_SPI_MASTER,
    .subMode                  = CY_SCB_SPI_MOTOROLA,
    .sclkMode                 = CY_SCB_SPI_CPHA0_CPOL0,
    .oversample               = _CYHAL_SPI_OVERSAMPLE_MIN,
    .rxDataWidth              = 8,
    .txDataWidth              = 8,
    .enableMsbFirst           = true,
    .enableFreeRunSclk        = false,
    .enableInputFilter        = false,
    .enableMisoLateSample     = true,
    .enableTransferSeperation = false,
    .enableWakeFromSleep      = false,
    .ssPolarity               = CY_SCB_SPI_ACTIVE_LOW,
    .rxFifoTriggerLevel       = 0,
    .rxFifoIntEnableMask      = 0,
    .txFifoTriggerLevel       = 0,
    .txFifoIntEnableMask      = 0,
    .masterSlaveIntEnableMask = 0
};

static void _cyhal_ssel_switch_state(cyhal_spi_t *obj, uint8_t ssel_idx, bool ssel_activate);

static cy_rslt_t _cyhal_spi_int_frequency(cyhal_spi_t *obj, uint32_t hz, uint8_t *over_sample_val)
{
    CY_ASSERT(NULL != obj);
    cy_rslt_t result = CY_RSLT_SUCCESS;
    uint8_t oversample_value;
    uint32_t divider_value;
    uint32_t last_diff = 0xFFFFFFFFU;
    uint8_t last_ovrsmpl_val = 0;
    uint32_t last_dvdr_val = 0;
    uint32_t oversampled_freq = 0;
    uint32_t divided_freq = 0;
    uint32_t diff = 0;

    Cy_SysClk_PeriphDisableDivider((cy_en_divider_types_t)obj->clock.block, obj->clock.channel);

#if defined(COMPONENT_PSOC6HAL)
    uint32_t peri_freq = Cy_SysClk_ClkPeriGetFrequency();
#else /* COMPONENT_PSOC4HAL */
    uint32_t peri_freq = Cy_SysClk_ClkSysGetFrequency();
#endif
    if (!obj->is_slave)
    {
        for (oversample_value = _CYHAL_SPI_OVERSAMPLE_MIN; oversample_value <= _CYHAL_SPI_OVERSAMPLE_MAX; oversample_value++)
        {
            oversampled_freq = hz * oversample_value;
            if ((hz * oversample_value > peri_freq) && (_CYHAL_SPI_OVERSAMPLE_MIN == oversample_value))
            {
                return CYHAL_SPI_RSLT_CLOCK_ERROR;
            }
            else if (hz * oversample_value > peri_freq)
            {
                continue;
            }

            divider_value = _cyhal_utils_divider_value(hz * oversample_value, 0);
            divided_freq = peri_freq /divider_value;
            diff = (oversampled_freq > divided_freq)
                ? oversampled_freq - divided_freq
                : divided_freq - oversampled_freq;

            if (diff < last_diff)
            {
                last_diff = diff;
                last_ovrsmpl_val = oversample_value;
                last_dvdr_val = divider_value;
                if (0 == diff)
                {
                    break;
                }
            }
        }
        *over_sample_val = last_ovrsmpl_val;
    }
    else
    {
        /* Slave requires such frequency: required_frequency = N / ((0.5 * desired_period) – 20 nsec - tDSI,
        *   N is 3 when "Enable Input Glitch Filter" is false and 4 when true.
        *   tDSI Is external master delay which is assumed to be 16.66 nsec */

        /* Divided by 2 desired period to avoid dividing in required_frequency formula */
        float desired_period_us_divided = 5e5f * (1 / (float)hz);
        uint32_t required_frequency = (uint32_t)(3e6f / (desired_period_us_divided - 36.66f / 1e3f));

        if (required_frequency > peri_freq)
        {
            return CYHAL_SPI_RSLT_CLOCK_ERROR;
        }

        /* Use maximum available clock for slave to make it able to work with any master environment */
        last_dvdr_val = 2;
    }

    result = Cy_SysClk_PeriphSetDivider((cy_en_divider_types_t)obj->clock.block, obj->clock.channel, last_dvdr_val - 1);

    if (CY_RSLT_SUCCESS == result)
    {
        Cy_SysClk_PeriphEnableDivider((cy_en_divider_types_t)obj->clock.block, obj->clock.channel);
    }

    return result;
}

static inline cyhal_spi_event_t _cyhal_spi_convert_interrupt_cause(uint32_t pdl_cause)
{
    static const uint32_t status_map[] =
    {
        (uint32_t)CYHAL_SPI_IRQ_ERROR,         // Default error if unknown value is set
        (uint32_t)CYHAL_SPI_IRQ_DATA_IN_FIFO,  // CY_SCB_SPI_TRANSFER_IN_FIFO_EVENT
        (uint32_t)CYHAL_SPI_IRQ_DONE,          // CY_SCB_SPI_TRANSFER_CMPLT_EVENT
        (uint32_t)CYHAL_SPI_IRQ_ERROR,         // CY_SCB_SPI_TRANSFER_ERR_EVENT
    };
    return (cyhal_spi_event_t)_cyhal_utils_convert_flags(status_map, sizeof(status_map) / sizeof(uint32_t), pdl_cause);
}

static void _cyhal_spi_irq_handler(void)
{
    cyhal_spi_t *obj = (cyhal_spi_t*) _cyhal_scb_get_irq_obj();
    if (NULL == obj)
    {
        return;
    }

    Cy_SCB_SPI_Interrupt(obj->base, &(obj->context));

    if (!obj->is_async)
    {
        return;
    }

    if (0 == (Cy_SCB_SPI_GetTransferStatus(obj->base,  &obj->context) & CY_SCB_SPI_TRANSFER_ACTIVE))
    {
        if (obj->tx_buffer)
        {
            /* Start TX Transfer */
            obj->pending = _CYHAL_SPI_PENDING_TX;
            const uint8_t *buf = obj->tx_buffer;
            obj->tx_buffer = NULL;

            Cy_SCB_SPI_Transfer(obj->base, (uint8_t *)buf, NULL, obj->tx_buffer_size, &obj->context);
        }
        else if (obj->rx_buffer)
        {
            /* Start RX Transfer */
            obj->pending = _CYHAL_SPI_PENDING_RX;
            uint8_t *rx_buf = obj->rx_buffer;
            uint8_t *tx_buf;
            size_t trx_size = obj->rx_buffer_size;

            if (obj->rx_buffer_size > 1)
            {
                 /* In this case we don't have a transmit buffer; we only have a receive buffer. While the PDL
                 * is fine with passing NULL for transmit, we don't get to control what data it is sending in
                 * that case, which we allowed the user to set. To honor the user's request, we reuse the rx
                 * buffer as the tx buffer too. We set all bytes beyond the one we will start filling in with
                 * the user provided 'write_fill'. This means the tx buffer is 1 element smaller than the rx
                 * buffer. As a result, we must therefore transfer 1 less element then we really want to in
                 * this transfer. When this transfer is complete, it will call into this again to receive the
                 * final element.
                 */
                trx_size -= 1; // Transfer everything left except for the last byte

                uint8_t **rx_buffer_p = (uint8_t **) &obj->rx_buffer;

                tx_buf = *rx_buffer_p + 1; // Start at second byte to avoid trying to transmit and receive the same byte
                memset(tx_buf, obj->write_fill, trx_size);

                *rx_buffer_p += trx_size; // Move to 1 byte before end
                obj->rx_buffer_size = 1; // Transfer the last byte on the next interrupt
            }
            else
            {
                tx_buf = &obj->write_fill;

                obj->rx_buffer = NULL;
            }

            Cy_SCB_SPI_Transfer(obj->base, tx_buf, rx_buf, trx_size, &obj->context);
        }
        else
        {
            /* Finish Async Transfer */
            obj->pending = _CYHAL_SPI_PENDING_NONE;
            obj->is_async = false;
            _cyhal_ssel_switch_state(obj, obj->active_ssel, _CYHAL_SPI_SSEL_DEACTIVATE);
        }
    }
}

static void _cyhal_spi_cb_wrapper(uint32_t event)
{
    cyhal_spi_t *obj = (cyhal_spi_t*) _cyhal_scb_get_irq_obj();
    cyhal_spi_irq_event_t anded_events = (cyhal_spi_irq_event_t) (obj->irq_cause & (uint32_t) _cyhal_spi_convert_interrupt_cause(event));

    // Don't call the callback until the final transfer has put everything in the FIFO/completed
    if ((anded_events & (CYHAL_SPI_IRQ_DATA_IN_FIFO | CYHAL_SPI_IRQ_DONE)) && !(obj->rx_buffer == NULL && obj->tx_buffer == NULL))
    {
        return;
    }

    if (anded_events)
    {
        cyhal_spi_event_callback_t callback = (cyhal_spi_event_callback_t) obj->callback_data.callback;
        callback(obj->callback_data.callback_arg, anded_events);
    }
}

static cy_en_scb_spi_sclk_mode_t _cyhal_convert_mode_sclk(cyhal_spi_mode_t mode)
{
    uint8_t sclk_mode = (mode & (CYHAL_SPI_MODE_FLAG_CPOL | CYHAL_SPI_MODE_FLAG_CPHA));

    switch (sclk_mode)
    {
        case CYHAL_SPI_MODE_FLAG_CPOL | CYHAL_SPI_MODE_FLAG_CPHA:
            return (CY_SCB_SPI_CPHA1_CPOL1);
        case CYHAL_SPI_MODE_FLAG_CPOL:
            return (CY_SCB_SPI_CPHA0_CPOL1);
        case CYHAL_SPI_MODE_FLAG_CPHA:
            return (CY_SCB_SPI_CPHA1_CPOL0);
        default:
            return (CY_SCB_SPI_CPHA0_CPOL0);
    }
}

static inline bool _is_cyhal_mode_msb(cyhal_spi_mode_t mode)
{
    return ((mode & CYHAL_SPI_MODE_FLAG_LSB) != CYHAL_SPI_MODE_FLAG_LSB);
}

static bool _cyhal_spi_pm_callback_instance(void *obj_ptr, cyhal_syspm_callback_state_t state, cy_en_syspm_callback_mode_t pdl_mode)
{
    cyhal_spi_t *obj = (cyhal_spi_t *)obj_ptr;
    bool allow = true;
    cy_stc_syspm_callback_params_t spi_callback_params = {
        .base = (void *) (obj->base),
        .context = (void *) &(obj->context)
    };

    if (CYHAL_SYSPM_CB_CPU_DEEPSLEEP == state)
        allow = (CY_SYSPM_SUCCESS == Cy_SCB_SPI_DeepSleepCallback(&spi_callback_params, pdl_mode));
    else if (CYHAL_SYSPM_CB_SYSTEM_HIBERNATE == state)
        allow = (CY_SYSPM_SUCCESS == Cy_SCB_SPI_HibernateCallback(&spi_callback_params, pdl_mode));

    return allow;
}

static cy_rslt_t _cyhal_spi_get_ssel_map_idx(cyhal_spi_t *obj, cyhal_gpio_t ssel,
                        const cyhal_resource_pin_mapping_t **ssel_map, uint8_t *idx)
{
    static const cyhal_resource_pin_mapping_t *ssel_s_pin_maps[] = {
        cyhal_pin_map_scb_spi_s_select0, cyhal_pin_map_scb_spi_s_select1,
        cyhal_pin_map_scb_spi_s_select2, cyhal_pin_map_scb_spi_s_select3 };
    static const size_t ssel_s_pin_maps_sizes_bytes[] = {
        sizeof(cyhal_pin_map_scb_spi_s_select0), sizeof(cyhal_pin_map_scb_spi_s_select1),
        sizeof(cyhal_pin_map_scb_spi_s_select2), sizeof(cyhal_pin_map_scb_spi_s_select3) };
    for (uint8_t i = 0; i < sizeof(ssel_s_pin_maps) / sizeof(ssel_s_pin_maps[0]); i++)
    {
        *ssel_map = _cyhal_scb_find_map(ssel, ssel_s_pin_maps[i],
                        ssel_s_pin_maps_sizes_bytes[i] / sizeof(cyhal_resource_pin_mapping_t), &(obj->resource));
        if (NULL != *ssel_map)
        {
            *idx = i;
            return CY_RSLT_SUCCESS;
        }
    }
    return CYHAL_SPI_RSLT_ERR_CANNOT_CONFIG_SSEL;
}

cy_rslt_t cyhal_spi_init(cyhal_spi_t *obj, cyhal_gpio_t mosi, cyhal_gpio_t miso, cyhal_gpio_t sclk, cyhal_gpio_t ssel,
                        const cyhal_clock_t *clk, uint8_t bits, cyhal_spi_mode_t mode, bool is_slave)
{
    CY_ASSERT(NULL != obj);
    memset(obj, 0, sizeof(cyhal_spi_t));

    cy_rslt_t result = CY_RSLT_SUCCESS;
    uint8_t ovr_sample_val = _CYHAL_SPI_OVERSAMPLE_MIN;

    // Explicitly marked not allocated resources as invalid to prevent freeing them.
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->pin_miso = NC;
    obj->pin_mosi = NC;
    obj->pin_sclk = NC;

    for (uint8_t i = 0; i < _CYHAL_SPI_SSEL_NUM; i++)
    {
        obj->pin_ssel[i] = NC;
        obj->ssel_pol[i] = CY_SCB_SPI_ACTIVE_LOW;
    }

    obj->write_fill = (uint8_t) CY_SCB_SPI_DEFAULT_TX;
    obj->active_ssel = 0;

    /* Validate pins configuration. Mandatory pins:*/
    /* Master mode: MOSI pin used, MISO unused:     SCLK, SSEL are optional */
    /* Master mode: MISO pin used, MOSI unused:     SCLK is mandatory, MOSI, SSEL are optional */
    /* Slave  mode: MOSI or MISO are used:          SCLK and SSEL are mandatory */

    /* Slave */
    if (is_slave)
    {
        if ((NC == sclk) || (NC == ssel) || ((NC == mosi) && (NC == miso)))
        {
            return CYHAL_SPI_RSLT_PIN_CONFIG_NOT_SUPPORTED;
        }
    }
    /* Master */
    else
    {
        if ((NC != miso && NC == sclk) || (NC == mosi && NC == miso))
        {
            return CYHAL_SPI_RSLT_PIN_CONFIG_NOT_SUPPORTED;
        }
    }

    obj->is_slave = is_slave;

    /* Get pin configurations */
    const cyhal_resource_pin_mapping_t *mosi_map = NULL;
    const cyhal_resource_pin_mapping_t *miso_map = NULL;
    const cyhal_resource_pin_mapping_t *sclk_map = NULL;
    const cyhal_resource_pin_mapping_t *ssel_map = NULL;
    uint8_t active_ssel = 0;
    if (is_slave)
    {
        mosi_map = _CYHAL_SCB_FIND_MAP(mosi, cyhal_pin_map_scb_spi_s_mosi);
        miso_map = _CYHAL_SCB_FIND_MAP(miso, cyhal_pin_map_scb_spi_s_miso);
        sclk_map = _CYHAL_SCB_FIND_MAP(sclk, cyhal_pin_map_scb_spi_s_clk);
        result = _cyhal_spi_get_ssel_map_idx(obj, ssel, &ssel_map, &active_ssel);
    }
    else
    {
        mosi_map = _CYHAL_SCB_FIND_MAP(mosi, cyhal_pin_map_scb_spi_m_mosi);
        miso_map = _CYHAL_SCB_FIND_MAP(miso, cyhal_pin_map_scb_spi_m_miso);
        sclk_map = _CYHAL_SCB_FIND_MAP(sclk, cyhal_pin_map_scb_spi_m_clk);
        /* No need to find maps for ssel pins, as GPIO used */
    }

    const cyhal_resource_inst_t *spi_inst = (NC != mosi)
        ? (mosi_map != NULL ? mosi_map->inst : NULL)
        : (miso_map != NULL ? miso_map->inst : NULL);

    /* Validate pins mapping */
    if (NULL == spi_inst ||
        ((NC != mosi) && ((NULL == mosi_map) || !_cyhal_utils_resources_equal(spi_inst, mosi_map->inst))) ||
        ((NC != miso) && ((NULL == miso_map) || !_cyhal_utils_resources_equal(spi_inst, miso_map->inst))) ||
        ((NC != sclk) && ((NULL == sclk_map) || !_cyhal_utils_resources_equal(spi_inst, sclk_map->inst))) ||
        ((is_slave) && ((NC != ssel) && ((NULL == ssel_map) || !_cyhal_utils_resources_equal(spi_inst, ssel_map->inst)))))
    {
        return CYHAL_SPI_RSLT_ERR_INVALID_PIN;
    }

    if (CY_RSLT_SUCCESS != (result = cyhal_hwmgr_reserve(spi_inst)))
    {
        return result;
    }

    obj->resource = *spi_inst;
    obj->base = _CYHAL_SCB_BASE_ADDRESSES[obj->resource.block_num];

    // reserve the MOSI pin
    if ((result == CY_RSLT_SUCCESS) && (NC != mosi))
    {
        result = _cyhal_utils_reserve_and_connect(mosi, mosi_map);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_mosi = mosi;
        }
    }

    // reserve the MISO pin
    if ((result == CY_RSLT_SUCCESS) && (NC != miso))
    {
        result = _cyhal_utils_reserve_and_connect(miso, miso_map);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_miso = miso;
        }
    }

    // reserve the SCLK pin
    if (result == CY_RSLT_SUCCESS && (NC != sclk))
    {
        result = _cyhal_utils_reserve_and_connect(sclk, sclk_map);
        if (result == CY_RSLT_SUCCESS)
        {
            obj->pin_sclk = sclk;
        }
    }

    // reserve and configure the SSEL pin
    if ((result == CY_RSLT_SUCCESS) && (NC != ssel))
    {
        result = cyhal_spi_slave_select_config(obj, ssel, CYHAL_SPI_SSEL_ACTIVE_LOW);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        if (clk == NULL)
        {
            result = cyhal_clock_allocate(&(obj->clock), CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT);
            obj->alloc_clock = true;
        }
        else
        {

            obj->clock = *clk;
            obj->alloc_clock = false;
            _cyhal_utils_update_clock_format(&(obj->clock));

            /* Per CDT 315848 and 002-20730 Rev. *E:
             * For SPI, an integer clock divider must be used for both master and slave. */
            if ((obj->clock.block == CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT) || (obj->clock.block == CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT))
            {
                result = CYHAL_SPI_RSLT_CLOCK_NOT_SUPPORTED;
            }
        }
    }
    if (result == CY_RSLT_SUCCESS)
    {
        result = (cy_rslt_t)Cy_SysClk_PeriphAssignDivider(
                _cyhal_scb_get_clock_index(obj->resource.block_num),
                (cy_en_divider_types_t)obj->clock.block, obj->clock.channel);

        if (result == CY_RSLT_SUCCESS)
        {
            result = _cyhal_spi_int_frequency(obj, _CYHAL_SPI_DEFAULT_SPEED, &ovr_sample_val);
        }
    }

    if (result == CY_RSLT_SUCCESS)
    {
        _cyhal_scb_update_instance_data(obj->resource.block_num, (void*)obj, &_cyhal_spi_pm_callback_instance);
        cy_stc_scb_spi_config_t config_structure = _cyhal_spi_default_config;
        config_structure.spiMode = is_slave == 0
            ? CY_SCB_SPI_MASTER
            : CY_SCB_SPI_SLAVE;
        obj->msb_first = _is_cyhal_mode_msb(mode);
        config_structure.enableMsbFirst = obj->msb_first;
        obj->clk_mode = _cyhal_convert_mode_sclk(mode);
        config_structure.sclkMode = obj->clk_mode;
        config_structure.rxDataWidth = bits;
        config_structure.txDataWidth = bits;
        config_structure.oversample = ovr_sample_val;
        obj->data_bits = bits;
        obj->mode = (uint8_t) mode;
        obj->oversample_value = ovr_sample_val;
        Cy_SCB_SPI_Init(obj->base, &config_structure, &(obj->context));

        /* Activating specified by user ssel after init */
        if (NC != ssel)
        {
            result = cyhal_spi_select_active_ssel(obj, ssel);
        }

        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        obj->irq_cause = 0;

        cy_stc_sysint_t irqCfg = { _CYHAL_SCB_IRQ_N[obj->resource.block_num], CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_spi_irq_handler);
        NVIC_EnableIRQ(_CYHAL_SCB_IRQ_N[obj->resource.block_num]);

        Cy_SCB_SPI_Enable(obj->base);
    }
    else
    {
        cyhal_spi_free(obj);
    }
    return result;
}

void cyhal_spi_free(cyhal_spi_t *obj)
{
    if (NULL != obj->base)
    {
        _cyhal_scb_update_instance_data(obj->resource.block_num, NULL, NULL);
        Cy_SCB_SPI_Disable(obj->base, NULL);
        Cy_SCB_SPI_DeInit(obj->base);
        obj->base = NULL;
    }

    if (obj->resource.type != CYHAL_RSC_INVALID)
    {
        IRQn_Type irqn = _CYHAL_SCB_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        cyhal_hwmgr_free(&(obj->resource));
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    _cyhal_utils_release_if_used(&(obj->pin_miso));
    _cyhal_utils_release_if_used(&(obj->pin_mosi));
    _cyhal_utils_release_if_used(&(obj->pin_sclk));
    for (uint8_t i = 0; i < _CYHAL_SPI_SSEL_NUM; i++)
    {
        _cyhal_utils_release_if_used(&(obj->pin_ssel[i]));
    }

    if (obj->alloc_clock)
    {
       cyhal_clock_free(&(obj->clock));
       obj->alloc_clock = false;
    }
}

static void _cyhal_ssel_switch_state(cyhal_spi_t *obj, uint8_t ssel_idx, bool ssel_activate)
{
    if ((!obj->is_slave) && (CYHAL_NC_PIN_VALUE != obj->pin_ssel[ssel_idx]))
    {
        /* Situations described:
        *   ssel_activate = true (need to set SSEL into active state)
        *       CY_SCB_SPI_ACTIVE_LOW - writing 0 to ssel pin
        *       CY_SCB_SPI_ACTIVE_HIGH - writing 1 to ssel pin
        *   ssel_activate = false (need to set SSEL into inactive state)
        *       CY_SCB_SPI_ACTIVE_LOW - writing 1 to ssel pin
        *       CY_SCB_SPI_ACTIVE_HIGH - writing 0 to ssel pin */
        bool ssel_state = (CY_SCB_SPI_ACTIVE_LOW == obj->ssel_pol[ssel_idx]) ? !ssel_activate : ssel_activate;
        cyhal_gpio_write(obj->pin_ssel[ssel_idx], ssel_state);
    }
}

cy_rslt_t cyhal_spi_set_frequency(cyhal_spi_t *obj, uint32_t hz)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    uint8_t   ovr_sample_val;

    if (NULL == obj)
    {
        return CYHAL_SPI_RSLT_BAD_ARGUMENT;
    }

    Cy_SCB_SPI_Disable(obj->base, &obj->context);
    result = _cyhal_spi_int_frequency(obj, hz, &ovr_sample_val);

    /* No need to reconfigure slave since oversample value, that was changed in _cyhal_spi_int_frequency, in slave is ignored */
    if ((CY_RSLT_SUCCESS == result) && !obj->is_slave && (obj->oversample_value != ovr_sample_val))
    {
       cy_stc_scb_spi_config_t config_structure = _cyhal_spi_default_config;
       Cy_SCB_SPI_DeInit(obj->base);
       config_structure.spiMode = obj->is_slave == false
           ? CY_SCB_SPI_MASTER
           : CY_SCB_SPI_SLAVE;
       config_structure.enableMsbFirst = obj->msb_first;
       config_structure.sclkMode = obj->clk_mode;
       config_structure.rxDataWidth = obj->data_bits;
       config_structure.txDataWidth = obj->data_bits;
       config_structure.oversample = ovr_sample_val;
       obj->oversample_value = ovr_sample_val;
       Cy_SCB_SPI_Init(obj->base, &config_structure, &(obj->context));
    }

    Cy_SCB_SPI_Enable(obj->base);

    return result;
}

cy_rslt_t cyhal_spi_select_active_ssel(cyhal_spi_t *obj, cyhal_gpio_t ssel)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    if ((NC != ssel) && (_CYHAL_SPI_PENDING_NONE == obj->pending))
    {
        for (uint8_t i = 0; i < _CYHAL_SPI_SSEL_NUM; i++)
        {
            if(obj->pin_ssel[i] == ssel)
            {
                Cy_SCB_SPI_SetActiveSlaveSelect(obj->base, (cy_en_scb_spi_slave_select_t)i);
                obj->active_ssel = i;
                return CY_RSLT_SUCCESS;
            }
        }
    }
    return CYHAL_SPI_RSLT_ERR_CANNOT_SWITCH_SSEL;
}

static inline cy_en_scb_spi_polarity_t _cyhal_spi_pol_from_hal_to_pdl(cyhal_spi_ssel_polarity_t hal_polarity)
{
    return (hal_polarity == CYHAL_SPI_SSEL_ACTIVE_HIGH) ? CY_SCB_SPI_ACTIVE_HIGH : CY_SCB_SPI_ACTIVE_LOW;
}

cy_rslt_t cyhal_spi_slave_select_config(cyhal_spi_t *obj, cyhal_gpio_t ssel, cyhal_spi_ssel_polarity_t polarity)
{
    CY_ASSERT(NULL != obj);
    cy_rslt_t result = CYHAL_SPI_RSLT_ERR_CANNOT_CONFIG_SSEL;
    uint8_t found_idx = 0;
    bool configuring_existing = false;
    if ((NC != ssel) && (_CYHAL_SPI_PENDING_NONE == obj->pending))
    {
        for (uint8_t i = 0; i < _CYHAL_SPI_SSEL_NUM; i++)
        {
            if ((configuring_existing = (ssel == obj->pin_ssel[i])))
            {
                result = CY_RSLT_SUCCESS;
                found_idx = i;
                break;
            }
            if (!obj->is_slave)
            {
                /* Looking for first available ssel slot */
                if ((NC == obj->pin_ssel[i]))
                {
                    result = cyhal_gpio_init(ssel, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG,
                                (polarity == CYHAL_SPI_SSEL_ACTIVE_LOW) ? true : false);
                    found_idx = i;
                    break;
                }
            }
        }
        if (!configuring_existing && (obj->is_slave))
        {
            const cyhal_resource_pin_mapping_t *ssel_map = NULL;
            if (CY_RSLT_SUCCESS == _cyhal_spi_get_ssel_map_idx(obj, ssel, &ssel_map, &found_idx))
            {
                /* Either mosi or miso should present. Will take one of them as instance SCB instance index source */
                const cyhal_resource_pin_mapping_t *data_pin_map = (NC != obj->pin_mosi) ?
                    _CYHAL_SCB_FIND_MAP_BLOCK(obj->pin_mosi, cyhal_pin_map_scb_spi_s_mosi, &(obj->resource)) :
                    _CYHAL_SCB_FIND_MAP_BLOCK(obj->pin_miso, cyhal_pin_map_scb_spi_s_miso, &(obj->resource));

                if ((NULL != ssel_map) && (NC == obj->pin_ssel[found_idx]) &&
                    _cyhal_utils_resources_equal(data_pin_map->inst, ssel_map->inst))
                {
                    result = _cyhal_utils_reserve_and_connect(ssel, ssel_map);
                }
            }
        }
        if (CY_RSLT_SUCCESS == result)
        {
            if (!configuring_existing)
                obj->pin_ssel[found_idx] = ssel;
            obj->ssel_pol[found_idx] = _cyhal_spi_pol_from_hal_to_pdl(polarity);

            /* Immediatelly apply updated slave select polarity */
            Cy_SCB_SPI_SetActiveSlaveSelectPolarity(obj->base, (cy_en_scb_spi_slave_select_t)found_idx, obj->ssel_pol[found_idx]);
            if (!obj->is_slave)
                _cyhal_ssel_switch_state(obj, found_idx, _CYHAL_SPI_SSEL_DEACTIVATE);
        }
    }
    return result;
}

cy_rslt_t cyhal_spi_recv(cyhal_spi_t *obj, uint32_t *value)
{
    if (NULL == obj)
        return CYHAL_SPI_RSLT_BAD_ARGUMENT;

    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    uint32_t read_value = CY_SCB_SPI_RX_NO_DATA;
    const uint32_t fill_in = 0x0000ffffUL; /* PDL Fill in value */
    uint32_t count = 0;

    if ((obj->is_slave) && (CYHAL_NC_PIN_VALUE == obj->pin_mosi))
    {
        return CYHAL_SPI_RSLT_INVALID_PIN_API_NOT_SUPPORTED;
    }

    if ((!obj->is_slave) && (CYHAL_NC_PIN_VALUE == obj->pin_miso))
    {
        return CYHAL_SPI_RSLT_INVALID_PIN_API_NOT_SUPPORTED;
    }

    if (!obj->is_slave)
    {
        _cyhal_ssel_switch_state(obj, obj->active_ssel, _CYHAL_SPI_SSEL_ACTIVATE);

        /* Clear FIFOs */
        Cy_SCB_SPI_ClearTxFifo(obj->base);
        Cy_SCB_SPI_ClearRxFifo(obj->base);

        while (count == 0)
        {
            count = Cy_SCB_SPI_Write(obj->base, fill_in);
        }

        while (Cy_SCB_SPI_IsTxComplete(obj->base) == false) { }
        while (Cy_SCB_SPI_GetNumInRxFifo(obj->base) == 0) { } /* Wait for RX FIFO not empty */
        _cyhal_ssel_switch_state(obj, obj->active_ssel, _CYHAL_SPI_SSEL_DEACTIVATE);
    }

    while (read_value == CY_SCB_SPI_RX_NO_DATA)
    {
        read_value = Cy_SCB_SPI_Read(obj->base);
    }
    *value = read_value;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_spi_send(cyhal_spi_t *obj, uint32_t value)
{
    if (NULL == obj)
        return CYHAL_SPI_RSLT_BAD_ARGUMENT;

    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    uint32_t count = 0;
    cy_rslt_t result = CY_RSLT_SUCCESS;

    if ((obj->is_slave) && (CYHAL_NC_PIN_VALUE == obj->pin_miso))
    {
        return CYHAL_SPI_RSLT_INVALID_PIN_API_NOT_SUPPORTED;
    }

    if ((!obj->is_slave) && (CYHAL_NC_PIN_VALUE == obj->pin_mosi))
    {
        return CYHAL_SPI_RSLT_INVALID_PIN_API_NOT_SUPPORTED;
    }

    if (!obj->is_slave)
    {
        _cyhal_ssel_switch_state(obj, obj->active_ssel, _CYHAL_SPI_SSEL_ACTIVATE);

        /* Clear FIFOs */
        Cy_SCB_SPI_ClearTxFifo(obj->base);
        Cy_SCB_SPI_ClearRxFifo(obj->base);
    }

    while (count == 0)
    {
        count = Cy_SCB_SPI_Write(obj->base, value);
    }

    if (!obj->is_slave)
    {
        while (Cy_SCB_SPI_IsTxComplete(obj->base) == false) { }
        while (Cy_SCB_SPI_GetNumInRxFifo(obj->base) == 0) { } /* Wait for RX FIFO not empty */
        _cyhal_ssel_switch_state(obj, obj->active_ssel, _CYHAL_SPI_SSEL_DEACTIVATE);
        (void)Cy_SCB_SPI_Read(obj->base);
    }

    return result;
}

cy_rslt_t cyhal_spi_transfer(cyhal_spi_t *obj, const uint8_t *tx, size_t tx_length, uint8_t *rx, size_t rx_length, uint8_t write_fill)
{
    if (NULL == obj)
        return CYHAL_SPI_RSLT_BAD_ARGUMENT;

    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    obj->write_fill = write_fill;
    cy_rslt_t rslt = cyhal_spi_transfer_async(obj, tx, tx_length, rx, rx_length);
    if (rslt == CY_RSLT_SUCCESS)
    {
        while (obj->pending != _CYHAL_SPI_PENDING_NONE) { } /* Wait for async transfer to complete */
    }
    obj->write_fill = (uint8_t) CY_SCB_SPI_DEFAULT_TX;
    return rslt;
}

cy_rslt_t cyhal_spi_transfer_async(cyhal_spi_t *obj, const uint8_t *tx, size_t tx_length, uint8_t *rx, size_t rx_length)
{
    if (NULL == obj)
        return CYHAL_SPI_RSLT_BAD_ARGUMENT;

    if (_cyhal_scb_pm_transition_pending())
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    cy_en_scb_spi_status_t spi_status;

    if ((CYHAL_NC_PIN_VALUE == obj->pin_mosi) || (CYHAL_NC_PIN_VALUE == obj->pin_miso))
    {
        return CYHAL_SPI_RSLT_INVALID_PIN_API_NOT_SUPPORTED;
    }

    _cyhal_ssel_switch_state(obj, obj->active_ssel, _CYHAL_SPI_SSEL_ACTIVATE);
    obj->is_async = true;

    /* Setup transfer */
    if (tx_length > rx_length)
    {
        if (rx_length > 0)
        {
            /* I) write + read, II) write only */
            obj->pending = _CYHAL_SPI_PENDING_TX_RX;
            obj->rx_buffer = NULL;

            obj->tx_buffer = tx + (obj->data_bits <= 8 ? rx_length : (rx_length * 2));
            obj->tx_buffer_size = tx_length - rx_length;

            tx_length = rx_length; // Use tx_length to store entire transfer length
        }
        else
        {
            /*  I) write only */
            obj->pending = _CYHAL_SPI_PENDING_TX;
            obj->rx_buffer = NULL;
            obj->tx_buffer = NULL;

            rx = NULL;
        }
    }
    else if (rx_length > tx_length)
    {
        if (tx_length > 0)
        {
            /*  I) write + read, II) read only */
            obj->pending = _CYHAL_SPI_PENDING_TX_RX;
            obj->tx_buffer = NULL;

            obj->rx_buffer = rx + (obj->data_bits <= 8 ? tx_length : (tx_length * 2));
            obj->rx_buffer_size = rx_length - tx_length;
        }
        else
        {
            /*  I) read only. */
            obj->pending = _CYHAL_SPI_PENDING_RX;
            obj->tx_buffer = NULL;

            obj->rx_buffer = rx_length > 1 ? rx + 1 : NULL;
            obj->rx_buffer_size = rx_length - 1;
            tx = &obj->write_fill;
            tx_length = 1;
        }
    }
    else
    {
        /* RX and TX of the same size: I) write + read. */
        obj->pending = _CYHAL_SPI_PENDING_TX_RX;
        obj->rx_buffer = NULL;
        obj->tx_buffer = NULL;
    }
    spi_status = Cy_SCB_SPI_Transfer(obj->base, (void *)tx, rx, tx_length, &obj->context);
    return spi_status == CY_SCB_SPI_SUCCESS
        ? CY_RSLT_SUCCESS
        : CYHAL_SPI_RSLT_TRANSFER_ERROR;
}

bool cyhal_spi_is_busy(cyhal_spi_t *obj)
{
    return Cy_SCB_SPI_IsBusBusy(obj->base);
}

cy_rslt_t cyhal_spi_abort_async(cyhal_spi_t *obj)
{
    if (NULL == obj)
    {
        return CYHAL_SPI_RSLT_BAD_ARGUMENT;
    }

    Cy_SCB_SPI_AbortTransfer(obj->base, &(obj->context));
    obj->pending = _CYHAL_SPI_PENDING_NONE;
    return CY_RSLT_SUCCESS;
}

void cyhal_spi_register_callback(cyhal_spi_t *obj, cyhal_spi_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
    Cy_SCB_SPI_RegisterCallback(obj->base, _cyhal_spi_cb_wrapper, &(obj->context));

    obj->irq_cause = 0;
}

void cyhal_spi_enable_event(cyhal_spi_t *obj, cyhal_spi_event_t event, uint8_t intr_priority, bool enable)
{
    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }

    IRQn_Type irqn = _CYHAL_SCB_IRQ_N[obj->resource.block_num];
    NVIC_SetPriority(irqn, intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSCB */
