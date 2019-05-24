/*
 * mbed Microcontroller Library
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
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

#if DEVICE_QSPI

#include "qspi_api.h"
#include "mbed_error.h"
#include "mbed_assert.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "cy_smif_memslot.h"
#include "cy_smif.h"

#define TIMEOUT_10_MS                  (10000UL)  /* in microseconds, timeout for all blocking functions */
#define SMIF_MAX_RX_COUNT              (65536UL)
#define QSPI_DESELECT_DELAY            (7UL)

#define QSPI_PIN_MAP_IDX0              (0UL)
#define QSPI_PIN_MAP_IDX1              (1UL)
#define QSPI_PIN_MAP_IDX2              (2UL)
#define QSPI_PIN_MAP_IDX3              (3UL)
#define QSPI_PIN_MAP_IDX4              (4UL)
#define QSPI_PIN_MAP_IDX5              (5UL)
#define QSPI_PIN_MAP_IDX6              (6UL)
#define QSPI_PIN_MAP_IDX7              (7UL)

/* Default QSPI configuration */
static const cy_stc_smif_config_t default_qspi_config =
{
	.mode = (uint32_t)CY_SMIF_NORMAL,
	.deselectDelay = QSPI_DESELECT_DELAY,
	.rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
	.blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};


static uint32_t get_addr_size(qspi_address_size_t size)
{
    uint32_t addr_size = 0; 

    switch(size) {
        case QSPI_CFG_ADDR_SIZE_8:
        addr_size = 1; 
        break; 

        case QSPI_CFG_ADDR_SIZE_16:
        addr_size = 2; 
        break; 

        case QSPI_CFG_ADDR_SIZE_24:
        addr_size = 3; 
        break; 

        case QSPI_CFG_ADDR_SIZE_32:
        addr_size = 4; 
        break; 

        default:
        addr_size = 0; 
    }

    return addr_size; 
}

static uint32_t get_alt_size(qspi_alt_size_t size)
{
    uint32_t alt_size = 0; 

    switch(size) {
        case QSPI_CFG_ALT_SIZE_8:
        alt_size = 1; 
        break; 

        case QSPI_CFG_ALT_SIZE_16:
        alt_size = 2; 
        break; 

        case QSPI_CFG_ALT_SIZE_24:
        alt_size = 3; 
        break; 

        case QSPI_CFG_ALT_SIZE_32:
        alt_size = 4; 
        break; 

        default:
        alt_size = 0; 
    }

    return alt_size; 
}

static cy_en_smif_txfr_width_t get_cy_bus_width(qspi_bus_width_t bus_width)
{
    cy_en_smif_txfr_width_t cy_bus_width = CY_SMIF_WIDTH_SINGLE; 

    switch(bus_width) {
        case QSPI_CFG_BUS_SINGLE:
        cy_bus_width = CY_SMIF_WIDTH_SINGLE;
        break;

        case QSPI_CFG_BUS_DUAL:
        cy_bus_width = CY_SMIF_WIDTH_DUAL;
        break;

        case QSPI_CFG_BUS_QUAD:
        cy_bus_width = CY_SMIF_WIDTH_QUAD;
        break;

        default:    
        cy_bus_width = CY_SMIF_WIDTH_SINGLE;    
    }

    return cy_bus_width;
}

static void convert_to_cy_cmd_config(const qspi_command_t *qspi_command, cy_stc_smif_mem_cmd_t *const cy_cmd_config)
{
    // This function does not check 'disabled' of each sub-structure in qspi_command_t
    // It is the responsibility of the caller to check it. 
    cy_cmd_config->command = qspi_command->instruction.value; 
    cy_cmd_config->cmdWidth = get_cy_bus_width(qspi_command->instruction.bus_width); 
    cy_cmd_config->addrWidth = get_cy_bus_width(qspi_command->address.bus_width); 
    cy_cmd_config->mode = qspi_command->alt.value; 
    cy_cmd_config->modeWidth = get_cy_bus_width(qspi_command->alt.bus_width); 
    cy_cmd_config->dummyCycles = qspi_command->dummy_count; 
    cy_cmd_config->dataWidth = get_cy_bus_width(qspi_command->data.bus_width); 

}

static void uint32_to_byte_array(uint32_t value, uint8_t *byteArray, uint32_t startPos, uint32_t size)
{
	do {
		size--;
		byteArray[size + startPos] = (uint8_t)value;
		value >>= 0x08;
	} while(size > 0);
}

static qspi_status_t validate_and_find_slaveselect(cy_en_smif_slave_select_t *slaveSelect, PinName ssel){
    if (ssel == PinMap_QSPI_SSEL[QSPI_PIN_MAP_IDX0].pin) {
        *slaveSelect = CY_SMIF_SLAVE_SELECT_0; 
    } else if(ssel == PinMap_QSPI_SSEL[QSPI_PIN_MAP_IDX1].pin) {
        *slaveSelect = CY_SMIF_SLAVE_SELECT_1; 
    } else if(ssel == PinMap_QSPI_SSEL[QSPI_PIN_MAP_IDX2].pin) {
        *slaveSelect = CY_SMIF_SLAVE_SELECT_2; 
    } else if(ssel == PinMap_QSPI_SSEL[QSPI_PIN_MAP_IDX3].pin) {
        *slaveSelect = CY_SMIF_SLAVE_SELECT_3; 
    } else {
        MBED_ERROR( MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_ARGUMENT), "Invalid slave select (ssel) pin." );
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    return QSPI_STATUS_OK;
}

static qspi_status_t validate_pins_and_find_dataselect(cy_en_smif_data_select_t *dataSelect, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk){
    bool isQuadSpi = false; 

    // ----------------------
    //    io0    | DATA_SELx
    // ----------------------
    // spi_data0 | DATA_SEL0
    // spi_data2 | DATA_SEL1
    // spi_data4 | DATA_SEL2
    // spi_data6 | DATA_SEL3
    // ----------------------
    //
    // valid combinations (io0, io1, io2, io3) 
    // ----------------------------
    // spi_data0, spi_data1, NC, NC                 - single SPI or Dual SPI 
    // spi_data2, spi_data3, NC, NC                 - single SPI or Dual SPI
    // spi_data4, spi_data5, NC, NC                 - single SPI or Dual SPI
    // spi_data6, spi_data7, NC, NC                 - single SPI or Dual SPI
    // spi_data0, spi_data1, spi_data2, spi_data3, ssel0   - Quad SPI
    // spi_data4, spi_data5, spi_data6, spi_data7, ssel2   - Quad SPI

    if (sclk != PinMap_QSPI_SCLK[QSPI_PIN_MAP_IDX0].pin) {
        goto qspi_pin_error;    
    }

    if ( (io2 != NC) && (io3 != NC) ){
        isQuadSpi = true; 
    }

    if (io0 == PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX0].pin) {
        if (io1 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX1].pin) {
            goto qspi_pin_error;
        }
        
        if (isQuadSpi) {
            if ((io2 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX2].pin) || (io3 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX3].pin)) {
                goto qspi_pin_error; 
            }
        }

        *dataSelect = CY_SMIF_DATA_SEL0; 
    } else if(io0 == PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX2].pin) {
        // quad spi is not valid in this case
        if (io1 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX3].pin) {
            goto qspi_pin_error; 
        }

        *dataSelect = CY_SMIF_DATA_SEL1;
    } else if(io0 == PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX4].pin) {
        if (io1 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX5].pin) {
            goto qspi_pin_error; 
        }
        
        if (isQuadSpi){
            if ((io2 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX6].pin) || (io3 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX7].pin)) {
                goto qspi_pin_error; 
            }
        }

        *dataSelect = CY_SMIF_DATA_SEL2;        
    } else if(io0 == PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX6].pin) {
        // quad spi is not valid in this case
        if (io1 != PinMap_QSPI_DATA[QSPI_PIN_MAP_IDX7].pin) {
           goto qspi_pin_error; 
        }

        *dataSelect = CY_SMIF_DATA_SEL3;
    } else {
        goto qspi_pin_error;     // invalid io0
    }

    return QSPI_STATUS_OK;

    qspi_pin_error:
        MBED_ERROR( MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_ARGUMENT), 
        "Invalid QSPI pin list. For single or dual SPI, io0 & io1 must be valid and io2 & io3 must be NC. For quad SPI, all pins must be valid." );
        return QSPI_STATUS_INVALID_PARAMETER;
}

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    cy_en_smif_data_select_t dataSelect = CY_SMIF_DATA_SEL0;

    MBED_ASSERT(obj);
    MBED_ASSERT(io0 != (PinName)NC);
    MBED_ASSERT(io1 != (PinName)NC);
    
    // single SPI or Dual SPI - io0 & io1 must be valid and io2 & io3 must be NC.
    // Quad SPI - all IOs must be valid. 
    if (io2 != (PinName)NC) {
        MBED_ASSERT(io3 != (PinName)NC);
    } else {
        MBED_ASSERT(io3 == (PinName)NC);
    }

    MBED_ASSERT(sclk != (PinName)NC);
    MBED_ASSERT(ssel != (PinName)NC);

    // mode (CPOL and CPHA) is not supported in PSoC 6
    MBED_ASSERT(mode == 0u);

    // No pin-wise instance check is done since PSoC 6 has only one SMIF instance.
    // But all the pins are checked against the pin map
    if ( QSPI_STATUS_OK != validate_and_find_slaveselect(&obj->slaveSelect, ssel) ) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    if (QSPI_STATUS_OK != validate_pins_and_find_dataselect(&dataSelect, io0, io1, io2, io3, sclk)) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    // configure drive mode for the pins
    pin_function(io0, pinmap_function(io0, PinMap_QSPI_DATA));
    pin_function(io1, pinmap_function(io1, PinMap_QSPI_DATA));
    pin_function(io2, pinmap_function(io2, PinMap_QSPI_DATA));
    pin_function(io3, pinmap_function(io3, PinMap_QSPI_DATA));
    pin_function(sclk, pinmap_function(sclk, PinMap_QSPI_SCLK));
    pin_function(ssel, pinmap_function(ssel, PinMap_QSPI_SSEL));

    obj->base = SMIF0;
    cy_en_smif_status_t smif_status = Cy_SMIF_Init(obj->base, &default_qspi_config, TIMEOUT_10_MS, &obj->context);
    if (CY_SMIF_SUCCESS != smif_status) {
        return QSPI_STATUS_ERROR;
    }

    Cy_SMIF_SetDataSelect(obj->base, obj->slaveSelect, dataSelect); // Configures data select
    Cy_SMIF_Enable(obj->base, &obj->context);

    return qspi_frequency(obj, hz);
}

qspi_status_t qspi_free(qspi_t *obj)
{
    Cy_SMIF_DeInit(obj->base);
    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    // Not implemented yet
    // PSoC supports only divide by 1, 2, 4, and 8. 

    return QSPI_STATUS_OK;
}

// Address passed through 'command' is not used, instead the value in 'addr' is used.  
static qspi_status_t smif_command_transfer(qspi_t *obj, const qspi_command_t *command, uint32_t addr, bool endOfTransfer)
{
    // Does not support command->instruction.diabled 
    if (command->instruction.disabled) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }
    
    uint8_t cmd_param[8] = {0};  // max address size is 4 bytes and max alt size is 4 bytes
    uint32_t startPos = 0; 
    uint32_t addr_size = 0;
    uint32_t alt_size = 0; 
    cy_en_smif_txfr_width_t bus_width = CY_SMIF_WIDTH_SINGLE;
    cy_stc_smif_mem_cmd_t cy_cmd_config; 

    convert_to_cy_cmd_config(command, &cy_cmd_config);
    
    // Does not support different bus_width for address and alt. 
    // bus_width is selected based on what (address or alt) is enabled. 
    // If both are enabled, bus_width of alt is selected. 

    // It is either possible to support 1 byte alt with different bus_width
    // by sending the alt byte as command as done in Cy_SMIF_Memslot_CmdRead()
    // in cy_smif_memslot.c or support multiple bytes of alt with same bus_width
    // as address by passing the alt bytes as cmd_param to Cy_SMIF_TransmitCommand(). 
    // Second approach is implemented here. This restriction is because of the way
    // PDL API is implemented. 
    if (!command->address.disabled && !command->alt.disabled) {
        if (cy_cmd_config.addrWidth != cy_cmd_config.modeWidth) {
            return QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    if (!command->address.disabled) {
        addr_size = get_addr_size(command->address.size);
        uint32_to_byte_array(addr, cmd_param, startPos, addr_size);
        startPos += addr_size; 
        bus_width = cy_cmd_config.addrWidth;
    }

    if (!command->alt.disabled) {
        alt_size = get_alt_size(command->alt.size);
        uint32_to_byte_array(cy_cmd_config.mode, cmd_param, startPos, alt_size);
        bus_width = cy_cmd_config.modeWidth;
    }

    uint32_t cmpltTxfr = ((endOfTransfer) ? 1UL : 0UL);
    cy_en_smif_status_t smifStatus = Cy_SMIF_TransmitCommand( obj->base, cy_cmd_config.command, 
                cy_cmd_config.cmdWidth, cmd_param, (addr_size + alt_size),
                bus_width, obj->slaveSelect, cmpltTxfr, &obj->context);

    if (CY_SMIF_SUCCESS != smifStatus) {
        return QSPI_STATUS_ERROR;
    }

    return QSPI_STATUS_OK; 
}

// length can be up to 65536.
qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    cy_en_smif_status_t smifStatus = CY_SMIF_SUCCESS;
    qspi_status_t status = smif_command_transfer(obj, command, command->address.value, false);

    if (QSPI_STATUS_OK == status) {
        if (command->dummy_count > 0u) {
            smifStatus = Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
        }

        if (CY_SMIF_SUCCESS == smifStatus) {
            smifStatus = Cy_SMIF_TransmitDataBlocking(obj->base, (uint8_t *)data, *length,
                        get_cy_bus_width(command->data.bus_width), &obj->context);

            if (CY_SMIF_SUCCESS != smifStatus) {
                status = QSPI_STATUS_ERROR; 
            }
        }
    }

    return status;
}

// no restriction on the value of length. This function splits the read into multiple
// chunked transfers.
qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    cy_en_smif_status_t smifStatus = CY_SMIF_SUCCESS;
    qspi_status_t status = QSPI_STATUS_OK; 
    uint32_t chunk = 0; 
    size_t read_bytes = *length;
    uint32_t addr = command->address.value; 

    // SMIF can read only up to 65536 bytes in one go. Split the larger read 
    // into multiple chunks
    while (read_bytes > 0) {
        chunk = (read_bytes > SMIF_MAX_RX_COUNT) ? (SMIF_MAX_RX_COUNT) : read_bytes; 

        // Address is passed outside command during a read of more than 65536 bytes. Since that
        // read has to be split into multiple chunks, the address value needs to be incremented
        // after every chunk has been read. This requires either modifying the address stored in
        // 'command' passed to read() which is not possible since command is a const pointer or 
        // to create a copy of the command object. Instead of copying the object, the address is
        // passed separately. 
        status = smif_command_transfer(obj, command, addr, false);

        if (QSPI_STATUS_OK == status) {
            if (command->dummy_count > 0u) {
                smifStatus = Cy_SMIF_SendDummyCycles(obj->base, command->dummy_count);
            }

            if (CY_SMIF_SUCCESS == smifStatus) {
                smifStatus = Cy_SMIF_ReceiveDataBlocking(obj->base, (uint8_t *)data, chunk,
                            get_cy_bus_width(command->data.bus_width), &obj->context);
                if (CY_SMIF_SUCCESS != smifStatus) {
                    status = QSPI_STATUS_ERROR;                     
                    break;
                }
            }
        }
        read_bytes -= chunk;
        addr += chunk; 
        data = (uint8_t *)data + chunk; 
    }

    return status;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    qspi_status_t status = QSPI_STATUS_OK;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        // only command, no rx or tx
        status = smif_command_transfer(obj, command, command->address.value, true);        
    }
    else {
        if (tx_data != NULL && tx_size) {
            status = qspi_write(obj, command, tx_data, &tx_size);
            if (status != QSPI_STATUS_OK) {
                return status;
            }
        }

        if (rx_data != NULL && rx_size) {
            status = qspi_read(obj, command, rx_data, &rx_size);
        }
    }
    return status;
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
    return PinMap_QSPI_DATA;
}

const PinMap *qspi_master_data1_pinmap(void)
{
    return PinMap_QSPI_DATA;
}

const PinMap *qspi_master_data2_pinmap(void)
{
    return PinMap_QSPI_DATA;
}

const PinMap *qspi_master_data3_pinmap(void)
{
    return PinMap_QSPI_DATA;
}


#endif

/** @}*/
