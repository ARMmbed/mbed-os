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

#include "qspi_api.h"
#include "mbed_error.h"
#include "cyhal_qspi.h"

#if DEVICE_QSPI

#ifdef __cplusplus
extern "C" {
#endif

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    return CY_RSLT_SUCCESS == cyhal_qspi_init(&(obj->hal_qspi), io0, io1, io2, io3, NC, NC, NC, NC, sclk, ssel, hz, mode) ? QSPI_STATUS_OK : QSPI_STATUS_ERROR;
}

qspi_status_t qspi_free(qspi_t *obj)
{
    cyhal_qspi_free(&(obj->hal_qspi));
    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    /* Return OK since this API is not implemented in cy_hal */
    return QSPI_STATUS_OK;
}

static inline cyhal_qspi_bus_width_t cyhal_qspi_convert_width(qspi_bus_width_t width)
{
    switch (width) {
        case QSPI_CFG_BUS_SINGLE:
            return CYHAL_QSPI_CFG_BUS_SINGLE;
        case QSPI_CFG_BUS_DUAL:
            return CYHAL_QSPI_CFG_BUS_DUAL;
        default: // fallthrough
        case QSPI_CFG_BUS_QUAD:
            return CYHAL_QSPI_CFG_BUS_QUAD;
    }
}

static inline cyhal_qspi_size_t cyhal_qspi_convert_addr_size(qspi_address_size_t size)
{
    switch (size) {
        case QSPI_CFG_ADDR_SIZE_8:
            return CYHAL_QSPI_CFG_SIZE_8;
        case QSPI_CFG_ADDR_SIZE_16:
            return CYHAL_QSPI_CFG_SIZE_16;
        case QSPI_CFG_ADDR_SIZE_24:
            return CYHAL_QSPI_CFG_SIZE_24;
        default: // fallthrough
        case QSPI_CFG_ADDR_SIZE_32:
            return CYHAL_QSPI_CFG_SIZE_32;
    }
}

static void cyhal_qspi_convert_command(const qspi_command_t *from, cyhal_qspi_command_t *to)
{
    to->instruction.bus_width = cyhal_qspi_convert_width(from->instruction.bus_width);
    to->instruction.value = from->instruction.value;
    to->instruction.disabled = from->instruction.disabled;
    to->address.bus_width = cyhal_qspi_convert_width(from->address.bus_width);
    to->address.size = cyhal_qspi_convert_addr_size(from->address.size);
    to->address.value = from->address.value;
    to->address.disabled = from->address.disabled;
    to->mode_bits.bus_width = cyhal_qspi_convert_width(from->alt.bus_width);
    to->mode_bits.size = from->alt.size;
    to->mode_bits.value = from->alt.value;
    to->mode_bits.disabled = from->alt.disabled;
    to->dummy_count = from->dummy_count;
    to->data.bus_width = cyhal_qspi_convert_width(from->data.bus_width);
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    cyhal_qspi_command_t cmd;
    cyhal_qspi_convert_command(command, &cmd);
    return CY_RSLT_SUCCESS == cyhal_qspi_write(&(obj->hal_qspi), &cmd, data, length) ? QSPI_STATUS_OK : QSPI_STATUS_ERROR;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    cyhal_qspi_command_t cmd;
    cyhal_qspi_convert_command(command, &cmd);
    return CY_RSLT_SUCCESS == cyhal_qspi_transfer(&(obj->hal_qspi), &cmd, tx_data, tx_size, rx_data, rx_size) ? QSPI_STATUS_OK : QSPI_STATUS_ERROR;
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    cyhal_qspi_command_t cmd;
    cyhal_qspi_convert_command(command, &cmd);
    return CY_RSLT_SUCCESS == cyhal_qspi_read(&(obj->hal_qspi), &cmd, data, length) ? QSPI_STATUS_OK : QSPI_STATUS_ERROR;
}

const PinMap *qspi_master_sclk_pinmap(void)
{
    return PinMap_QSPI_SCLK;
}

const PinMap *qspi_master_ssel_pinmap(void)
{
    return PinMap_QSPI_SSEL;
}

const PinMap *qspi_master_data0_pinmap(void)
{
    return PinMap_QSPI_DATA0;
}

const PinMap *qspi_master_data1_pinmap(void)
{
    return PinMap_QSPI_DATA1;
}

const PinMap *qspi_master_data2_pinmap(void)
{
    return PinMap_QSPI_DATA2;
}

const PinMap *qspi_master_data3_pinmap(void)
{
    return PinMap_QSPI_DATA3;
}

#ifdef __cplusplus
}
#endif

#endif
