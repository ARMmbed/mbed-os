/* mbed Microcontroller Library
 * Copyright (c) 2018, ARM Limited
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
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "qspi_device.h"

/* Command table indices */
#define COMMAND_READ_INDEX  (0)
#define COMMAND_WRITE_INDEX (1)

#define LUT1_SEQ_INDEX   0   // Pre-defined read sequence
#define LUT2_SEQ_INDEX   4   // Pre-defined write sequence
#define LUT3_SEQ_INDEX   8   // User-define sequence
/* Minimum write size */
#define MIN_SIZE         16  // At least four words of data must be written into the TX Buffer

/* Array of QSPI peripheral base address. */
static SPIFI_Type *const qspi_addrs[] = SPIFI_BASE_PTRS;

extern uint32_t qspi_get_freq(void);

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    spifi_config_t config = {0};

    uint32_t qspiio0name = pinmap_peripheral(io0, PinMap_QSPI_DATA0);
    uint32_t qspiio1name = pinmap_peripheral(io1, PinMap_QSPI_DATA1);
    uint32_t qspiio2name = pinmap_peripheral(io2, PinMap_QSPI_DATA2);
    uint32_t qspiio3name = pinmap_peripheral(io3, PinMap_QSPI_DATA3);
    uint32_t qspiclkname = pinmap_peripheral(sclk, PinMap_QSPI_SCLK);
    uint32_t qspisselname = pinmap_peripheral(ssel, PinMap_QSPI_SSEL);

    uint32_t qspi_data_first = pinmap_merge(qspiio0name, qspiio1name);
    uint32_t qspi_data_second = pinmap_merge(qspiio2name, qspiio3name);
    uint32_t qspi_data_third = pinmap_merge(qspiclkname, qspisselname);

    if (qspi_data_first != qspi_data_second || qspi_data_second != qspi_data_third ||
        qspi_data_first != qspi_data_third) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    if (qspi_frequency(obj, hz) != QSPI_STATUS_OK) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    /* Initialize SPIFI */
    SPIFI_GetDefaultConfig(&config);
    SPIFI_Init(SPIFI0, &config);

    // tested all combinations, take first
    obj->instance = qspi_data_first;

    pinmap_pinout(io0, PinMap_QSPI_DATA0);
    pinmap_pinout(io1, PinMap_QSPI_DATA1);
    pinmap_pinout(io2, PinMap_QSPI_DATA2);
    pinmap_pinout(io3, PinMap_QSPI_DATA3);

    pinmap_pinout(sclk, PinMap_QSPI_SCLK);
    pinmap_pinout(ssel, PinMap_QSPI_SSEL);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_free(qspi_t *obj)
{
    SPIFI_Deinit(qspi_addrs[obj->instance]);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    qspi_status_t status = QSPI_STATUS_OK;
    int div = qspi_get_freq() / hz;

    if ((qspi_get_freq() % hz) != 0) {
        /* Incase the exact requested baud rate can be derived then set right div,
         * else set baudrate to the closest lower value
         */
        div++;
    }

    if (div > 256 || div < 1) {
        status = QSPI_STATUS_INVALID_PARAMETER;
        return status;
    }

    /* Set the clock divider */
    CLOCK_SetClkDiv(kCLOCK_DivSpifiClk, div, false);

    return status;
}

static void qspi_prepare_command(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    spifi_command_t spifi_command = {0};
    SPIFI_Type *base = qspi_addrs[obj->instance];
    bool use_memory_command = false;

    /* Set the opcode & poll mode */
    spifi_command.opcode = command->instruction.value;
    spifi_command.isPollMode = false;

    /* Check if this is a data transmit */
    if (tx_data != NULL && tx_size) {
        spifi_command.dataLen = tx_size;
        spifi_command.direction = kSPIFI_DataOutput;
    }

    /* Check if this is a data receive */
    if (rx_data != NULL && rx_size) {
        spifi_command.dataLen = rx_size;
        spifi_command.direction = kSPIFI_DataInput;
    }

    /* Check if we need to set dual bit */
    if ((command->instruction.bus_width == QSPI_CFG_BUS_DUAL) ||
        (command->address.bus_width == QSPI_CFG_BUS_DUAL) ||
        (command->data.bus_width == QSPI_CFG_BUS_DUAL)) {
        base->CTRL |= SPIFI_CTRL_DUAL_MASK;
    } else {
        base->CTRL &= ~SPIFI_CTRL_DUAL_MASK;
    }

    if (command->address.disabled == true) {
        /* No flash address specified */
        spifi_command.type = kSPIFI_CommandOpcodeOnly;
        spifi_command.format = kSPIFI_CommandAllSerial;
        if ((command->instruction.bus_width == QSPI_CFG_BUS_DUAL) ||
            (command->instruction.bus_width == QSPI_CFG_BUS_QUAD)) {
            spifi_command.format = kSPIFI_CommandAllQuad;
        }
    } else {
        /* Set the address size */
        spifi_command.type = (spifi_command_type_t)(command->address.size + 2);

        /* Default to 1-1-1 mode */
        spifi_command.format = kSPIFI_CommandAllSerial;

        /* Check if it is 2-2-2 or 4-4-4 mode */
        if (((command->instruction.bus_width == QSPI_CFG_BUS_DUAL) &&
            (command->address.bus_width == QSPI_CFG_BUS_DUAL) &&
            (command->data.bus_width == QSPI_CFG_BUS_DUAL)) ||
            ((command->instruction.bus_width == QSPI_CFG_BUS_QUAD) &&
            (command->address.bus_width == QSPI_CFG_BUS_QUAD) &&
            (command->data.bus_width == QSPI_CFG_BUS_QUAD))) {
            /* All quad/dual. All fields of the command are in quad/dual format. */
            spifi_command.format = kSPIFI_CommandAllQuad;
        }

        /* Check if it is 1-2-2 or 1-4-4 mode */
        if (((command->instruction.bus_width == QSPI_CFG_BUS_SINGLE) &&
            (command->address.bus_width == QSPI_CFG_BUS_DUAL) &&
            (command->data.bus_width == QSPI_CFG_BUS_DUAL)) ||
            ((command->instruction.bus_width == QSPI_CFG_BUS_SINGLE) &&
            (command->address.bus_width == QSPI_CFG_BUS_QUAD) &&
            (command->data.bus_width == QSPI_CFG_BUS_QUAD))) {
            /* Serial opcode. Opcode field is serial. Other fields are quad/dual. */
            spifi_command.format = kSPIFI_CommandOpcodeSerial;
        }

        /* Check if it is 1-1-2 or 1-1-4 mode */
        if (((command->instruction.bus_width == QSPI_CFG_BUS_SINGLE) &&
            (command->address.bus_width == QSPI_CFG_BUS_SINGLE) &&
            (command->data.bus_width == QSPI_CFG_BUS_DUAL)) ||
            ((command->instruction.bus_width == QSPI_CFG_BUS_SINGLE) &&
            (command->address.bus_width == QSPI_CFG_BUS_SINGLE) &&
            (command->data.bus_width == QSPI_CFG_BUS_QUAD))) {
            /* Quad/dual data. Data field is quad/dual, other fields are serial. */
            spifi_command.format = kSPIFI_CommandDataQuad;
        }

        /* Check if this is a data receive */
        if (rx_data != NULL && rx_size) {
            uint32_t cycles_per_byte = 8;

            if ((command->address.bus_width == QSPI_CFG_BUS_DUAL) &&
                (command->data.bus_width == QSPI_CFG_BUS_DUAL)) {
                cycles_per_byte = 4;
            }
            if ((command->address.bus_width == QSPI_CFG_BUS_QUAD) &&
                (command->data.bus_width == QSPI_CFG_BUS_QUAD)) {
                cycles_per_byte = 2;
            }

            /* Set dummy bytes */
            spifi_command.intermediateBytes = command->dummy_count / cycles_per_byte;

            use_memory_command = true;
        }

        /* Set the flash address */
        SPIFI_SetCommandAddress(base, command->address.value);
    }

    if (use_memory_command) {
        /* Setup command */
        SPIFI_SetMemoryCommand(base, &spifi_command);
    } else {
        /* Setup memory command */
        SPIFI_SetCommand(base, &spifi_command);
    }
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    uint32_t to_write = *length;
    uint32_t *data_send = (uint32_t *)data;
    SPIFI_Type *base = qspi_addrs[obj->instance];
    uint32_t i = 0;

    /* Enforce word-sized access */
    if ((to_write & 0x3) != 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    /* Use a pre-defined command is no write instruction is provided */
    if (command->instruction.disabled) {
        /* Set the flash address */
        SPIFI_SetCommandAddress(base, command->address.value);

        /* If no instruction provided then use the pre-defined read sequence */
        preset_spifi_command[COMMAND_WRITE_INDEX].dataLen = to_write;
        SPIFI_SetCommand(base, &preset_spifi_command[COMMAND_WRITE_INDEX]);
    } else {
        /* Prepare the write command */
        qspi_prepare_command(obj, command, data, to_write, NULL, 0);
    }

    /* Write the data */
    for (i = 0; i < to_write; i += 4, data_send++) {
        SPIFI_WriteData(base, *data_send);
    }

    /* Wait for deasseration of CS */
    while (SPIFI_GetStatusFlag(base) & kSPIFI_CommandWriteFinished) {
    }

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    uint32_t dest_addr = FSL_FEATURE_SPIFI_START_ADDR + command->address.value;
    uint32_t to_read = *length;
    SPIFI_Type *base = qspi_addrs[obj->instance];

    /* Enforce word-sized access */
    if ((to_read & 0x3) != 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    /* Use a pre-defined command is no read instruction is provided */
    if (command->instruction.disabled) {
        /* If no instruction provided then use the pre-defined read sequence */
        preset_spifi_command[COMMAND_READ_INDEX].dataLen = to_read;
        SPIFI_SetMemoryCommand(base, &preset_spifi_command[COMMAND_READ_INDEX]);
    } else {
        /* Prepare for read command */
        qspi_prepare_command(obj, command, NULL, 0, data, to_read);
    }

    for (uint32_t i = 0; i < to_read / 4; i++) {
        ((uint32_t*)data)[i] = *((uint32_t *)(dest_addr) + i);
    }

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    SPIFI_Type *base = qspi_addrs[obj->instance];

    if (tx_size > MIN_SIZE || rx_size > MIN_SIZE) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        /* Setup the command */
        qspi_prepare_command(obj, command, tx_data, tx_size, rx_data, rx_size);
    } else {
        if (tx_data != NULL && tx_size) {
            /* Transmit data to QSPI */
            uint8_t val[MIN_SIZE];

            memset(val, 0, sizeof(val));
            memcpy(val, tx_data, tx_size);

            /* Setup the command */
            qspi_prepare_command(obj, command, tx_data, tx_size, rx_data, rx_size);

            for (uint32_t i = 0; i < tx_size; i++) {
                SPIFI_WriteDataByte(base, val[i]);
            }
        }

        if (rx_data != NULL && rx_size) {
            /* Receive data from QSPI */
            uint8_t val[MIN_SIZE];

            memset(val, 0, sizeof(val));

            /* Setup the command */
            qspi_prepare_command(obj, command, tx_data, tx_size, rx_data, rx_size);

            for (uint32_t i = 0; i < rx_size; i++) {
                val[i] = SPIFI_ReadDataByte(base);
            }

            memcpy(rx_data, val, rx_size);
        }
    }

    /* Wait for deasseration of CS */
    while (SPIFI_GetStatusFlag(base) & kSPIFI_CommandWriteFinished) {
    }

    return QSPI_STATUS_OK;
}

const PinMap *qspi_master_sclk_pinmap()
{
    return PinMap_QSPI_SCLK;
}

const PinMap *qspi_master_ssel_pinmap()
{
    return PinMap_QSPI_SSEL;
}

const PinMap *qspi_master_data0_pinmap()
{
    return PinMap_QSPI_DATA0;
}

const PinMap *qspi_master_data1_pinmap()
{
    return PinMap_QSPI_DATA1;
}

const PinMap *qspi_master_data2_pinmap()
{
    return PinMap_QSPI_DATA2;
}

const PinMap *qspi_master_data3_pinmap()
{
    return PinMap_QSPI_DATA3;
}

#endif

