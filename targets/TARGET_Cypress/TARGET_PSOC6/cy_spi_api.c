/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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
 */

#include "spi_api.h"
#include "cyhal_hwmgr.h"
#include "cyhal_spi.h"
#include "cyhal_scb_common.h"
#include "cyhal_utils.h"
#include "cy_scb_spi.h"
#include "mbed_error.h"

#if DEVICE_SPI

#ifdef __cplusplus
extern "C" {
#endif

static struct spi_s *cy_get_spi(spi_t *obj)
{
#if DEVICE_SPI_ASYNCH
    return &(obj->spi);
#else
    return obj;
#endif
}

SPIName spi_get_peripheral_name(PinName mosi, PinName miso, PinName mclk)
{
    const cyhal_resource_pin_mapping_t *map = cyhal_utils_get_resource(mclk, cyhal_pin_map_scb_spi_s_clk, sizeof(cyhal_pin_map_scb_spi_s_clk) / sizeof(*cyhal_pin_map_scb_spi_s_clk));
    if (NULL != map) {
        return (SPIName)CYHAL_SCB_BASE_ADDRESSES[map->inst->block_num];
    }
    MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "SPI not found");
}

static void cy_spi_irq_handler_internal(void *handler_arg, cyhal_spi_event_t event)
{
    struct spi_s *spi = cy_get_spi((spi_t *)handler_arg);
    spi->async_events = 0;
    if (0 != (event & CYHAL_SPI_IRQ_DONE)) {
        spi->async_events |= SPI_EVENT_COMPLETE;
        if (spi->async_in_progress == true) {
            /* Disable event CYHAL_SPI_IRQ_DONE */
            cyhal_spi_enable_event(&(spi->hal_spi), CYHAL_SPI_IRQ_DONE, CYHAL_ISR_PRIORITY_DEFAULT, false);
            /* Clear status */
            spi->async_in_progress = false;
        }
    }
    if (0 != (event & CYHAL_SPI_IRQ_ERROR)) {
        spi->async_events |= SPI_EVENT_ERROR;
    }
    if (0 != (spi->async_events & spi->async_event_mask)) {
        void (*async_handler)(void) = spi->async_handler;
        if (async_handler != NULL) {
            (*async_handler)();
        }
    }
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *spi = cy_get_spi(obj);
    spi->cfg.mode = CYHAL_SPI_MODE_00_MSB;
    spi->cfg.data_bits = 8;
    spi->cfg.is_slave = false;
    spi->mosi = mosi;
    spi->miso = miso;
    spi->sclk = sclk;
    spi->ssel = ssel;
    if (CY_RSLT_SUCCESS != cyhal_spi_init(&(spi->hal_spi), mosi, miso, sclk, ssel, NULL, spi->cfg.data_bits, spi->cfg.mode, spi->cfg.is_slave)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_init");
    }

    /* Register callback mechanism */
    cyhal_spi_register_callback(&(spi->hal_spi), &cy_spi_irq_handler_internal, obj);
}

void spi_free(spi_t *obj)
{
    struct spi_s *spi = cy_get_spi(obj);
    cyhal_spi_free(&(spi->hal_spi));
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *spi = cy_get_spi(obj);
    cyhal_gpio_t mosi = spi->mosi, miso = spi->miso, sclk = spi->sclk, ssel = spi->ssel;
    int hz = spi->hz;
    cyhal_spi_free(&(spi->hal_spi));
    cyhal_spi_mode_t hal_mode;
    switch (mode) {
        default: // fallthrough
        case 0:
            hal_mode = CYHAL_SPI_MODE_00_MSB;
            break;
        case 1:
            hal_mode = CYHAL_SPI_MODE_01_MSB;
            break;
        case 2:
            hal_mode = CYHAL_SPI_MODE_10_MSB;
            break;
        case 3:
            hal_mode = CYHAL_SPI_MODE_11_MSB;
            break;
    }
    if (CY_RSLT_SUCCESS != cyhal_spi_init(&(spi->hal_spi), mosi, miso, sclk, ssel, NULL, (uint8_t)bits, hal_mode, slave != 0)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_init");
    }
    if (spi->hz != 0) {
        spi_frequency(obj, hz);
    }

    /* Register callback mechanism after second init */
    cyhal_spi_register_callback(&(spi->hal_spi), &cy_spi_irq_handler_internal, obj);
}

void spi_frequency(spi_t *obj, int hz)
{
    struct spi_s *spi = cy_get_spi(obj);
    if (CY_RSLT_SUCCESS != cyhal_spi_set_frequency(&(spi->hal_spi), hz)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_set_frequency");
    }
    spi->hz = hz;
}

int spi_master_write(spi_t *obj, int value)
{
    struct spi_s *spi = cy_get_spi(obj);
    int received;

    if (CY_RSLT_SUCCESS != cyhal_spi_transfer(&(spi->hal_spi), (const uint8_t *)&value, 1, (uint8_t *)&received, 1, 0xff)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_transfer");
    }
    return received;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    struct spi_s *spi = cy_get_spi(obj);
    if (CY_RSLT_SUCCESS != cyhal_spi_transfer(&(spi->hal_spi), (const uint8_t *)tx_buffer, (size_t)tx_length, (uint8_t *)rx_buffer, (size_t)rx_length, (uint8_t)write_fill)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_transfer");
    }
    return tx_length > rx_length ? tx_length : rx_length;
}

int spi_slave_receive(spi_t *obj)
{
    struct spi_s *spi = cy_get_spi(obj);
    return 0 != Cy_SCB_SPI_GetNumInRxFifo(spi->hal_spi.base);
}

int spi_slave_read(spi_t *obj)
{
    uint32_t value;
    struct spi_s *spi = cy_get_spi(obj);
    if (CY_RSLT_SUCCESS != cyhal_spi_recv(&(spi->hal_spi), &value)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_recv");
    }
    return value;
}

void spi_slave_write(spi_t *obj, int value)
{
    struct spi_s *spi = cy_get_spi(obj);
    if (CY_RSLT_SUCCESS != cyhal_spi_send(&(spi->hal_spi), value)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_send");
    }
}

int spi_busy(spi_t *obj)
{
    struct spi_s *spi = cy_get_spi(obj);
    return cyhal_spi_is_busy(&(spi->hal_spi));
}

uint8_t spi_get_module(spi_t *obj)
{
    return (uint8_t)cy_get_spi(obj)->hal_spi.resource.block_num;
}

const PinMap *spi_master_mosi_pinmap(void)
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_master_miso_pinmap(void)
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_master_clk_pinmap(void)
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_master_cs_pinmap(void)
{
    return PinMap_SPI_SSEL;
}

const PinMap *spi_slave_mosi_pinmap(void)
{
    return PinMap_SPI_MOSI;
}

const PinMap *spi_slave_miso_pinmap(void)
{
    return PinMap_SPI_MISO;
}

const PinMap *spi_slave_clk_pinmap(void)
{
    return PinMap_SPI_SCLK;
}

const PinMap *spi_slave_cs_pinmap(void)
{
    return PinMap_SPI_SSEL;
}

#if DEVICE_SPI_ASYNCH

void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, MBED_UNUSED uint8_t bit_width, uint32_t handler, uint32_t event, MBED_UNUSED DMAUsage hint)
{
    struct spi_s *spi = cy_get_spi(obj);
    spi->async_handler = (void (*)(void))handler;
    spi->async_event_mask = event;
    spi->async_in_progress = false;

    /* Enable event CYHAL_SPI_IRQ_DONE to know when asynchronous transfer is done */
    cyhal_spi_enable_event(&(spi->hal_spi), CYHAL_SPI_IRQ_DONE, CYHAL_ISR_PRIORITY_DEFAULT, true);

    if (CY_RSLT_SUCCESS != cyhal_spi_transfer_async(&(spi->hal_spi), (const uint8_t *)tx, (size_t)tx_length, (uint8_t *)rx, (size_t)rx_length)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_transfer_async");
    }
    spi->async_in_progress = true;
}

uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    struct spi_s *spi = cy_get_spi(obj);
    return spi->async_events;
}

uint8_t spi_active(spi_t *obj)
{
    return (uint8_t)spi_busy(obj);
}

void spi_abort_asynch(spi_t *obj)
{
    struct spi_s *spi = cy_get_spi(obj);
    if (CY_RSLT_SUCCESS != cyhal_spi_abort_async(&(spi->hal_spi))) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_spi_abort_async");
    }
}

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DEVICE_SPI
