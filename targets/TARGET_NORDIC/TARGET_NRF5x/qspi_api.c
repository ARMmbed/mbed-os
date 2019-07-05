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

#include <string.h>
#include "PeripheralPins.h"
#include "nrfx_qspi.h"

/* 
TODO
    - config inside obj - nordic headers have some problems with inclusion
    - free - is it really empty, nothing to do there?
    - prepare command - support more protocols that nordic can do (now limited)
    - nordic does not support
        - alt
        - dummy cycles
*/

#define MBED_HAL_QSPI_MAX_FREQ          32000000UL

// NRF supported R/W opcodes
#define FASTREAD_opcode     0x0B
#define READ2O_opcode       0x3B
#define READ2IO_opcode      0xBB
#define READ4O_opcode       0x6B
#define READ4IO_opcode      0xEB
#define READSFDP_opcode     0x5A

#define PP_opcode           0x02
#define PP2O_opcode         0xA2
#define PP4O_opcode         0x32
#define PP4IO_opcode        0x38

#define SCK_DELAY           0x05
#define WORD_MASK           0x03
#define WORD_COUNT          16

// NRF SFDP defines
#define DWORD_LEN           4
#define SFDP_CMD_LEN        DWORD_LEN
#define SFDP_DATA_LEN       128  // SFPD data buffer length in bytes, may need to be increased for other flash parts
#define SFDP_READ_LEN       8    // 8 SFDP bytes can be read at a time
#define SFDP_READ_MAX       (SFDP_DATA_LEN / SFDP_READ_LEN)

static nrfx_qspi_config_t config;

// Private helper function to track initialization
static ret_code_t _qspi_drv_init(void);
// Private helper function to set NRF frequency divider
nrf_qspi_frequency_t nrf_frequency(int hz);
// Private helper function to read SFDP data
qspi_status_t sfdp_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length);

qspi_status_t qspi_prepare_command(qspi_t *obj, const qspi_command_t *command, bool write) 
{
    // we need to remap opcodes to NRF ID's
    // most commmon are 1-1-1, 1-1-4, 1-4-4

    // 1-1-1
    if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->address.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->data.bus_width == QSPI_CFG_BUS_SINGLE) {
        if (write) {
            if (command->instruction.value == PP_opcode) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        } else {
            if (command->instruction.value == FASTREAD_opcode) {
                config.prot_if.readoc = NRF_QSPI_READOC_FASTREAD;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        }
    // 1-1-4
    } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->address.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->data.bus_width == QSPI_CFG_BUS_QUAD) {
        // 1_1_4
        if (write) {
            if (command->instruction.value == PP4O_opcode) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP4O;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        } else {
            if (command->instruction.value == READ4O_opcode) {
                config.prot_if.readoc = NRF_QSPI_READOC_READ4O;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        }
    // 1-4-4
    } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->address.bus_width == QSPI_CFG_BUS_QUAD &&
        command->data.bus_width == QSPI_CFG_BUS_QUAD) {
        // 1_4_4
        if (write) {
            if (command->instruction.value == PP4IO_opcode) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP4IO;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        } else {
            if (command->instruction.value == READ4IO_opcode) {
                config.prot_if.readoc = NRF_QSPI_READOC_READ4IO;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        }
    // 1-1-2
    } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->address.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->data.bus_width == QSPI_CFG_BUS_DUAL) {
        // 1-1-2
        if (write) {
            if (command->instruction.value == PP2O_opcode) {
                config.prot_if.writeoc = NRF_QSPI_WRITEOC_PP2O;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        } else {
            if (command->instruction.value == READ2O_opcode) {
                config.prot_if.readoc = NRF_QSPI_READOC_READ2O;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        }
    // 1-2-2
    } else if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE &&
        command->address.bus_width == QSPI_CFG_BUS_DUAL &&
        command->data.bus_width == QSPI_CFG_BUS_DUAL) {
        // 1-2-2
        if (write) {
            // 1-2-2 write is not supported
            return QSPI_STATUS_INVALID_PARAMETER;
        } else {
            if (command->instruction.value == READ2IO_opcode) {
                config.prot_if.readoc = NRF_QSPI_READOC_READ2IO;
            } else {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
        }
    } else {
        return QSPI_STATUS_INVALID_PARAMETER;
    }
    
    // supporting only 24 or 32 bit address
    if (command->address.size == QSPI_CFG_ADDR_SIZE_24) {
        config.prot_if.addrmode = NRF_QSPI_ADDRMODE_24BIT;
    } else if (command->address.size == QSPI_CFG_ADDR_SIZE_32) {
        config.prot_if.addrmode = NRF_QSPI_ADDRMODE_32BIT;
    } else {
        return QSPI_STATUS_INVALID_PARAMETER;
    }
    
    //Configure QSPI with new command format
    ret_code_t ret_status = _qspi_drv_init();
    if (ret_status != NRFX_SUCCESS ) {
        if (ret_status == NRFX_ERROR_INVALID_PARAM) {
            return QSPI_STATUS_INVALID_PARAMETER;
        } else {
            return QSPI_STATUS_ERROR;
        }
    }
    
    return QSPI_STATUS_OK;
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

    config.phy_if.sck_freq  = nrf_frequency(hz);
    config.phy_if.sck_delay = SCK_DELAY;
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
    config.phy_if.sck_freq  = nrf_frequency(hz);

    // use sync version, no handler
    ret_code_t ret = _qspi_drv_init();
    if (ret == NRFX_SUCCESS ) {
        return QSPI_STATUS_OK;
    } else if (ret == NRF_ERROR_INVALID_PARAM) {
        return QSPI_STATUS_INVALID_PARAMETER;
    } else {
        return QSPI_STATUS_ERROR;
    }
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    // flash address and buffer length must be divisible by 4
    if ((*length & WORD_MASK) > 0 ||
        (command->address.value & WORD_MASK) > 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }
		
    qspi_status_t status = qspi_prepare_command(obj, command, true);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

    if (is_word_aligned(data) &&
        nrfx_is_in_ram(data)) {
        // write here does not return how much it transfered, we return transfered all
        ret_code_t ret = nrfx_qspi_write(data, *length, command->address.value);
        if (ret == NRF_SUCCESS ) {
            return QSPI_STATUS_OK;
        } else {
            return QSPI_STATUS_ERROR;
        }
    }
    else {
        // if the data buffer is not WORD/4-byte aligned, use an aligned buffer on the stack
        uint32_t aligned_buffer[WORD_COUNT];
        uint32_t pos = 0;
        size_t bytes_to_write = *length;

        while(pos < *length) {
            // copy into the aligned buffer
            size_t diff = bytes_to_write <= sizeof(aligned_buffer) ? bytes_to_write : sizeof(aligned_buffer);
            memcpy(aligned_buffer, &((const uint8_t *)data)[pos], diff);

            // write one buffer over QSPI
            ret_code_t ret = nrfx_qspi_write(aligned_buffer, diff, command->address.value+pos);
            if (ret != NRF_SUCCESS ) {
                return QSPI_STATUS_ERROR;
            }
            pos += diff;
            bytes_to_write -= diff;
        }
    }
    return QSPI_STATUS_OK;
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    // flash address and buffer length must be divisible by 4
    if ((*length & WORD_MASK) > 0 ||
        (command->address.value & WORD_MASK) > 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    // check to see if this is an SFDP read
    if (command->instruction.value == READSFDP_opcode) {
        qspi_status_t status = sfdp_read(obj, command, data, length );
        return status;
    }

    qspi_status_t status = qspi_prepare_command(obj, command, false);
    if (status != QSPI_STATUS_OK) {
        return status;
    }

    if (is_word_aligned(data) &&
        nrfx_is_in_ram(data)) {
        ret_code_t ret = nrfx_qspi_read(data, *length, command->address.value);
        if (ret == NRF_SUCCESS ) {
            return QSPI_STATUS_OK;
        } else {
            return QSPI_STATUS_ERROR;
        }
    }
    else {
       // if the data buffer is not WORD/4-byte aligned or in RAM, use an aligned buffer on the stack
        uint32_t aligned_buffer[WORD_COUNT];
        uint32_t pos = 0;
        size_t bytes_to_read = *length;

        while(pos < *length) {

            size_t diff = bytes_to_read <= sizeof(aligned_buffer) ? bytes_to_read : sizeof(aligned_buffer);

            // read one buffer over QSPI
            ret_code_t ret = nrfx_qspi_read(aligned_buffer, diff, command->address.value+pos);
            if (ret != NRF_SUCCESS ) {
                return QSPI_STATUS_ERROR;
            }

            // copy into original read buffer
            memcpy(&((uint8_t *)data)[pos], aligned_buffer, diff);

            pos += diff;
            bytes_to_read -= diff;
        }
    }
    return QSPI_STATUS_OK;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    ret_code_t ret_code;
    uint8_t data[8] = { 0 };
    uint32_t data_size = tx_size + rx_size;

    nrf_qspi_cinstr_conf_t qspi_cinstr_config = { 0 };
    qspi_cinstr_config.opcode    = command->instruction.value;
    qspi_cinstr_config.io2_level = true;
    qspi_cinstr_config.io3_level = true;
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
        uint32_t address_size = (uint32_t)qspi_cinstr_config.length - 1;
        uint8_t *address_bytes = (uint8_t *)&command->address.value;
        for (uint32_t i = 0; i < address_size; ++i) {
            data[i] = address_bytes[address_size - 1 - i];
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
 
    ret_code = nrfx_qspi_cinstr_xfer(&qspi_cinstr_config, data, data);
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
        nrfx_qspi_uninit();
    }
    ret = nrfx_qspi_init(&config, NULL , NULL);
    if( ret == NRF_SUCCESS )
        _initialized = true;
    return ret;
}

// Private helper to set NRF frequency divider
nrf_qspi_frequency_t nrf_frequency(int hz)
{
    nrf_qspi_frequency_t freq = NRF_QSPI_FREQ_32MDIV16;

    // Convert hz to closest NRF frequency divider
    if (hz < 2130000)
        freq = NRF_QSPI_FREQ_32MDIV16; // 2.0 MHz, minimum supported frequency
    else if (hz < 2290000)
        freq = NRF_QSPI_FREQ_32MDIV15; // 2.13 MHz
    else if (hz < 2460000)
        freq = NRF_QSPI_FREQ_32MDIV14; // 2.29 MHz
    else if (hz < 2660000)
        freq = NRF_QSPI_FREQ_32MDIV13; // 2.46 Mhz
    else if (hz < 2900000)
        freq = NRF_QSPI_FREQ_32MDIV12; // 2.66 MHz
    else if (hz < 3200000)
        freq = NRF_QSPI_FREQ_32MDIV11; // 2.9 MHz
    else if (hz < 3550000)
        freq = NRF_QSPI_FREQ_32MDIV10; // 3.2 MHz
    else if (hz < 4000000)
        freq = NRF_QSPI_FREQ_32MDIV9; // 3.55 MHz
    else if (hz < 4570000)
        freq = NRF_QSPI_FREQ_32MDIV8; // 4.0 MHz
    else if (hz < 5330000)
        freq = NRF_QSPI_FREQ_32MDIV7; // 4.57 MHz
    else if (hz < 6400000)
        freq = NRF_QSPI_FREQ_32MDIV6; // 5.33 MHz
    else if (hz < 8000000)
        freq = NRF_QSPI_FREQ_32MDIV5; // 6.4 MHz
    else if (hz < 10600000)
        freq = NRF_QSPI_FREQ_32MDIV4; // 8.0 MHz
    else if (hz < 16000000)
        freq = NRF_QSPI_FREQ_32MDIV3; // 10.6 MHz
    else if (hz < 32000000)
        freq = NRF_QSPI_FREQ_32MDIV2; // 16 MHz
    else
        freq = NRF_QSPI_FREQ_32MDIV1; // 32 MHz

    return freq;
}

// Private helper to read nRF5x SFDP data using QSPI
qspi_status_t sfdp_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    ret_code_t ret_code;
    static bool b_init = false;
    static uint8_t sfdp_rx[SFDP_DATA_LEN] = { 0 };

    if (b_init == false) {
        // get the SFDP data usig nRF5x QSPI custom instuctions and long frame mode (lfen)
        nrf_qspi_cinstr_conf_t qspi_cinstr_config = { 0 };
        qspi_cinstr_config.opcode = command->instruction.value;
        qspi_cinstr_config.io2_level = true;
        qspi_cinstr_config.io3_level = true;
        qspi_cinstr_config.wipwait   = false;
        qspi_cinstr_config.wren      = false;
        qspi_cinstr_config.lfen      = true;
        qspi_cinstr_config.lfstop    = false;

        // read the SFDP data, cmd + 8 bytes at a time
        uint8_t sfdp_data[SFDP_READ_LEN];
        qspi_cinstr_config.length = NRF_QSPI_CINSTR_LEN_9B;

        for (uint32_t i = 0; i < SFDP_READ_MAX; ++i) {

            static uint32_t rx_i = 0;
            memset(sfdp_data, 0, SFDP_READ_LEN);

            if (i == (SFDP_READ_MAX - 1) ){
                qspi_cinstr_config.lfstop = true;
            }

            ret_code = nrfx_qspi_cinstr_xfer(&qspi_cinstr_config, sfdp_data, sfdp_data);
            if (ret_code != NRF_SUCCESS) {
                return QSPI_STATUS_ERROR;
            }

            // copy the second DWORD from the command data, the first DWORD is 0's
            for (uint32_t c = DWORD_LEN; c < SFDP_READ_LEN; ++c) {
                ((uint8_t *)sfdp_rx)[rx_i] = sfdp_data[c];
                ++rx_i;
            }
            rx_i += DWORD_LEN;
        }

        // re-send just the SFDP CMD to offset the next read by DWORD
        uint8_t sfdp_cmd[SFDP_CMD_LEN] = { 0 };
        qspi_cinstr_config.lfstop = false;
        qspi_cinstr_config.length = NRF_QSPI_CINSTR_LEN_5B;

        ret_code = nrfx_qspi_cinstr_xfer(&qspi_cinstr_config, sfdp_cmd, sfdp_cmd);
        if (ret_code != NRF_SUCCESS) {
            return QSPI_STATUS_ERROR;
        }

        // read the offset SFDP data, cmd + 8 bytes at a time
        qspi_cinstr_config.length = NRF_QSPI_CINSTR_LEN_9B;
        for (uint32_t i = 0; i < SFDP_READ_MAX; ++i) {

            static uint32_t rx_i = DWORD_LEN;  // offset sfdp_rx data start
            memset(sfdp_data, 0, SFDP_READ_LEN);

            if (i == (SFDP_READ_MAX - 1) ){
                qspi_cinstr_config.lfstop = true;
            }

            ret_code = nrfx_qspi_cinstr_xfer(&qspi_cinstr_config, sfdp_data, sfdp_data);
            if (ret_code != NRF_SUCCESS) {
                return QSPI_STATUS_ERROR;
            }

            // copy the second DWORD from the command data, the first DWORD is 0's
            for (uint32_t c = DWORD_LEN; c < SFDP_READ_LEN; ++c) {
                ((uint8_t *)sfdp_rx)[rx_i] = sfdp_data[c];
                ++rx_i;
            }
            rx_i += DWORD_LEN;
        }

        b_init = true;
    }

    if ( b_init == true) {
        // check for valid SFDP data, last basic header byte is always 0xFF
        // NOTE: "nRF52840-Preview-DK" boards do not support SFDP read
        if (sfdp_rx[7] != 0xFF) {
            return QSPI_STATUS_ERROR;
        }

        // calculate the SFDP data length based on the parameter table offset
        // provided at index 12, plus the table length in DWORDS at index 11
        uint32_t sfdp_length = sfdp_rx[12] + (sfdp_rx[11] * DWORD_LEN);

        // check if the data request is within the SFDP data array
        // increase SFDP_DATA_LEN to match sfdp_length, if necessary
        if ( sfdp_length <= SFDP_DATA_LEN &&
             sfdp_length >= (command->address.value + *length) ) {
            memcpy(data, (sfdp_rx + command->address.value), *length);
            return QSPI_STATUS_OK;
        } else {
            return QSPI_STATUS_INVALID_PARAMETER;
        }
    } else {
        return QSPI_STATUS_ERROR;
    }
}

const PinMap *qspi_master_sclk_pinmap()
{
    return PinMap_QSPI_testing;
}

const PinMap *qspi_master_ssel_pinmap()
{
    return PinMap_QSPI_testing;
}

const PinMap *qspi_master_data0_pinmap()
{
    return PinMap_QSPI_testing;
}

const PinMap *qspi_master_data1_pinmap()
{
    return PinMap_QSPI_testing;
}

const PinMap *qspi_master_data2_pinmap()
{
    return PinMap_QSPI_testing;
}

const PinMap *qspi_master_data3_pinmap()
{
    return PinMap_QSPI_testing;
}


#endif

/** @}*/
