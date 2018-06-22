/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "qspi_api.h"

#if DEVICE_QSPI

#include "nrf_drv_common.h"
#include "nrf_drv_qspi.h"

/* 
TODO
    - config inside obj - nordic headers have some problems with inclusion
    - free - is it really empty, nothing to do there?
    - prepare command - support more protocols that nordic can do (now limited)
    - nordic does not support
        - alt
        - dummy cycles
*/

#define MBED_HAL_QSPI_HZ_TO_CONFIG(hz)  ((32000000/(hz))-1)
#define MBED_HAL_QSPI_MAX_FREQ          32000000UL

static nrf_drv_qspi_config_t config;

// Private helper function to track initialization
static ret_code_t _qspi_drv_init(void);

qspi_status_t qspi_prepare_command(qspi_t *obj, const qspi_command_t *command, bool write) 
{
    //Use custom command if provided by the caller
    if(command->instruction.value != 0) {
        //Use custom command if provided
        if (write) {
            config.prot_if.writeoc = (nrf_qspi_writeoc_t)command->instruction.value;
        } else {
            config.prot_if.readoc = (nrf_qspi_readoc_t)command->instruction.value;
        }
    } else {
        // we need to remap to command-address-data - x_x_x
        // most commmon are 1-1-1, 1-1-4, 1-4-4
        // 1-1-1
        if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->address.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->data.bus_width == QSPI_CFG_BUS_SINGLE) {
            if (write) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP;
            } else {
                config.prot_if.readoc = NRF_QSPI_READOC_FASTREAD;
            }
        // 1-1-4
        } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->address.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->data.bus_width == QSPI_CFG_BUS_QUAD) {
            // 1_1_4
            if (write) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP4O;
            } else {
                config.prot_if.readoc = NRF_QSPI_READOC_READ4O;
            }
        // 1-4-4
        } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->address.bus_width == QSPI_CFG_BUS_QUAD &&
            command->data.bus_width == QSPI_CFG_BUS_QUAD) {
            // 1_4_4
            if (write) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP4IO;
            } else {
                config.prot_if.readoc = NRF_QSPI_READOC_READ4IO;
            }
        // 1-1-2
        } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->address.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->data.bus_width == QSPI_CFG_BUS_DUAL) {
            // 1-1-2
            if (write) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP2O;
            } else {
                config.prot_if.readoc = NRF_QSPI_READOC_READ2O;
            }
        // 1-2-2
        } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
            command->address.bus_width == QSPI_CFG_BUS_DUAL &&
            command->data.bus_width == QSPI_CFG_BUS_DUAL) {
            // 1-2-2
            if (write) {
                //Currently NRF52840 does not define PP2IO, so use PP2O for 1-2-2 mode
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP2O;
            } else {
                config.prot_if.readoc = NRF_QSPI_READOC_READ2IO;
            }
        }
    }    
    
    qspi_status_t ret = QSPI_STATUS_OK;
    // supporting only 24 or 32 bit address
    if (command->address.size == QSPI_CFG_ADDR_SIZE_24) {
        config.prot_if.addrmode = NRF_QSPI_ADDRMODE_24BIT;
    } else if (command->address.size == QSPI_CFG_ADDR_SIZE_32) {
        config.prot_if.addrmode = NRF_QSPI_ADDRMODE_32BIT;
    } else {
        ret = QSPI_STATUS_INVALID_PARAMETER;
    }
    
    //Configure QSPI with new command format
    if(ret == QSPI_STATUS_OK) {
        ret_code_t ret_status = _qspi_drv_init();
        if (ret_status != NRF_SUCCESS ) {
            if (ret_status == NRF_ERROR_INVALID_PARAM) {
                return QSPI_STATUS_INVALID_PARAMETER;
            } else {
                return QSPI_STATUS_ERROR;
            }
        }
    }
    
    return ret;
}

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    (void)(obj);
    if (hz > MBED_HAL_QSPI_MAX_FREQ) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    // memset(config, 0, sizeof(config));

    config.pins.sck_pin = (uint32_t)sclk;
    config.pins.csn_pin = (uint32_t)ssel;
    config.pins.io0_pin = (uint32_t)io0;
    config.pins.io1_pin = (uint32_t)io1;
    config.pins.io2_pin = (uint32_t)io2;
    config.pins.io3_pin = (uint32_t)io3;
    config.irq_priority = SPI_DEFAULT_CONFIG_IRQ_PRIORITY;

    config.phy_if.sck_freq = (nrf_qspi_frequency_t)MBED_HAL_QSPI_HZ_TO_CONFIG(hz),
    config.phy_if.sck_delay = 0x05,
    config.phy_if.dpmen = false;
    config.phy_if.spi_mode = mode == 0 ? NRF_QSPI_MODE_0 : NRF_QSPI_MODE_1;

    //Use _qspi_drv_init private function to initialize
    ret_code_t ret = _qspi_drv_init();
    if (ret == NRF_SUCCESS ) {
        return QSPI_STATUS_OK;
    } else if (ret == NRF_ERROR_INVALID_PARAM) {
        return QSPI_STATUS_INVALID_PARAMETER;
    } else {
        return QSPI_STATUS_ERROR;
    }
}

qspi_status_t qspi_free(qspi_t *obj)
{
    (void)(obj);
    // possibly here uninit from SDK driver
    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    config.phy_if.sck_freq  = (nrf_qspi_frequency_t)MBED_HAL_QSPI_HZ_TO_CONFIG(hz);
    
    // use sync version, no handler
    ret_code_t ret = _qspi_drv_init();
    if (ret == NRF_SUCCESS ) {
        return QSPI_STATUS_OK;
    } else if (ret == NRF_ERROR_INVALID_PARAM) {
        return QSPI_STATUS_INVALID_PARAMETER;
    } else {
        return QSPI_STATUS_ERROR;
    }
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    qspi_status_t status = qspi_prepare_command(obj, command, true);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

    // write here does not return how much it transfered, we return transfered all
    ret_code_t ret = nrf_drv_qspi_write(data, *length, command->address.value);
    if (ret == NRF_SUCCESS ) {
        return QSPI_STATUS_OK;
    } else {
        return QSPI_STATUS_ERROR;
    }
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    qspi_status_t status = qspi_prepare_command(obj, command, false);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

    ret_code_t ret = nrf_drv_qspi_read(data, *length, command->address.value);
    if (ret == NRF_SUCCESS ) {
        return QSPI_STATUS_OK;
    } else {
        return QSPI_STATUS_ERROR;
    }
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    ret_code_t ret_code;
    uint8_t data[8];
    uint32_t data_size = tx_size + rx_size;
 
    nrf_qspi_cinstr_conf_t qspi_cinstr_config;
    qspi_cinstr_config.opcode    = command->instruction.value;
    qspi_cinstr_config.io2_level = false;
    qspi_cinstr_config.io3_level = false;
    qspi_cinstr_config.wipwait   = false;
    qspi_cinstr_config.wren      = false;
 
    if(!command->address.disabled && data_size == 0) {
        // erase command with address
        if (command->address.size == QSPI_CFG_ADDR_SIZE_24) {
            qspi_cinstr_config.length = NRF_QSPI_CINSTR_LEN_4B;
        } else if (command->address.size == QSPI_CFG_ADDR_SIZE_32) {
            qspi_cinstr_config.length = NRF_QSPI_CINSTR_LEN_5B;
        } else {
            return QSPI_STATUS_INVALID_PARAMETER;
        }
        for (uint32_t i = 0; i < (uint32_t)qspi_cinstr_config.length - 1; ++i) {
            data[i] = ((uint8_t *)&command->address.value)[i];
        }
    } else if (data_size < 9) {
        qspi_cinstr_config.length = (nrf_qspi_cinstr_len_t)(NRF_QSPI_CINSTR_LEN_1B + data_size);
        // preparing data to send
        for (uint32_t i = 0; i < tx_size; ++i) {
            data[i] = ((uint8_t *)tx_data)[i];
        }
    } else {
        return QSPI_STATUS_ERROR;
    }
 
    ret_code = nrf_drv_qspi_cinstr_xfer(&qspi_cinstr_config, data, data);
    if (ret_code != NRF_SUCCESS) {
        return QSPI_STATUS_ERROR;
    }
 
    // preparing received data
    for (uint32_t i = 0; i < rx_size; ++i) {
        // Data is sending as a normal SPI transmission so there is one buffer to send and receive data.
        ((uint8_t *)rx_data)[i] = data[i];
    }
 
    return QSPI_STATUS_OK;
}

// Private helper function to track initialization
static ret_code_t _qspi_drv_init(void) 
{
    static bool _initialized = false;
    ret_code_t ret = NRF_ERROR_INVALID_STATE;
    
    if(_initialized) {
        //NRF implementation prevents calling init again. But we need to call init again to program the new command settings in the IFCONFIG registers. 
        //So, we have to uninit qspi first and call init again. 
        nrf_drv_qspi_uninit();
    }
    ret = nrf_drv_qspi_init(&config, NULL , NULL);
    if( ret == NRF_SUCCESS )
        _initialized = true;
    return ret;
}


#endif

/** @}*/
