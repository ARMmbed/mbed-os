/* mbed Microcontroller Library
 * Copyright (c) 2017, ARM Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if DEVICE_QSPI

#include "qspi_api.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include "cmsis.h"
#include "pinmap.h"

#define QSPI_FLASH_SIZE_DEFAULT 0x80000000
#define HAL_QSPI_TIMEOUT_DEFAULT_VALUE       ((uint32_t)5000U)                                             

static uint32_t get_alt_bytes_size(const uint32_t num_bytes)
{
    return 0xFFFFFFFF;
}

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    return QSPI_STATUS_OK; 
}

qspi_status_t qspi_free(qspi_t *obj)
{
    return QSPI_STATUS_OK; 
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    qspi_status_t status = QSPI_STATUS_OK;
    return status;
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    qspi_status_t status = QSPI_STATUS_OK;
    //printf("%s command 0x%x, data 0x%x, len %d\n", __func__, command->instruction.value, *(uint32_t*)data, *length);
    if (HAL_QSPI_CommandReceive(command, data, length, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        status = QSPI_STATUS_ERROR;
    } 

    return status;
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    qspi_status_t status = QSPI_STATUS_OK;
    //printf("%s command 0x%x, data 0x%x, len %d\n", __func__, command->instruction.value, *(uint32_t*)data, *length);
	
    if (HAL_QSPI_CommandReceive(command, data, length, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        status = QSPI_STATUS_ERROR;
    } 
    return status;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    //printf("\r\nqspi_command_transfer tx_size %u rx_size %u command %x", tx_size, rx_size, command->instruction.value);
    //printf("\r\ntx_data %s \r\n", tx_data);
    qspi_status_t status = QSPI_STATUS_OK;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
    	if (HAL_QSPI_CommandReceive(command, 0, 0, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = QSPI_STATUS_ERROR;
            return status;
        }
    } else {
        // often just read a register, check if we need to transmit anything prior reading
        if (tx_data != NULL && tx_size) {
            size_t tx_length = tx_size;
            status = qspi_write(obj, command, tx_data, &tx_length);
        }

        if (rx_data != NULL && rx_size) {
            size_t rx_length = rx_size;
            status = qspi_read(obj, command, rx_data, &rx_length);
        }
	}
    return status;
}

const PinMap *qspi_master_sclk_pinmap()
{
    return 0;//PinMap_QSPI_SCLK;
}

const PinMap *qspi_master_ssel_pinmap()
{
    return 0;//PinMap_QSPI_SSEL;
}

const PinMap *qspi_master_data0_pinmap()
{
    return 0;//PinMap_QSPI_DATA0;
}

const PinMap *qspi_master_data1_pinmap()
{
    return 0;//PinMap_QSPI_DATA1;
}

const PinMap *qspi_master_data2_pinmap()
{
    return 0;//PinMap_QSPI_DATA2;
}

const PinMap *qspi_master_data3_pinmap()
{
    return 0;//PinMap_QSPI_DATA3;
}

#endif

/** @}*/
