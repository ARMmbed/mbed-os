/* mbed Microcontroller Library
 * Copyright (c) 2018, ARM Limited
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
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "qspi_device.h"
#include "platform/mbed_critical.h"

/* Look-up table entry indices */
#define LUT1_SEQ_INDEX   0   // Pre-defined read sequence
#define LUT2_SEQ_INDEX   4   // Pre-defined write sequence
#define LUT3_SEQ_INDEX   8   // User-define sequence
/* Minimum write size */
#define MIN_SIZE         16  // At least four words of data must be written into the TX Buffer

/* Array of QSPI peripheral base address. */
static QuadSPI_Type *const qspi_addrs[] = QuadSPI_BASE_PTRS;

extern uint32_t qspi_get_freq(void);

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    uint32_t clockSourceFreq = 0;
    qspi_config_t config = {0};

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

    clockSourceFreq = qspi_get_freq();

    /*Get QSPI default settings and configure the qspi */
    QSPI_GetDefaultQspiConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus */
    config.AHBbufferSize[3] = FLASH_PAGE_SIZE;

    // tested all combinations, take first
    obj->instance = qspi_data_first;

    QSPI_Init(qspi_addrs[obj->instance], &config, clockSourceFreq);

    /* Copy the pre-defined LUT table */
    memcpy(single_config.lookuptable, lut, sizeof(uint32_t) * FSL_FEATURE_QSPI_LUT_DEPTH);

    /*According to serial flash feature to configure flash settings */
    QSPI_SetFlashConfig(qspi_addrs[obj->instance], &single_config);

    qspi_frequency(obj, hz);

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
    QSPI_Deinit(qspi_addrs[obj->instance]);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    qspi_status_t status = QSPI_STATUS_OK;
    QuadSPI_Type *base = qspi_addrs[obj->instance];
    int div = qspi_get_freq() / hz;

    if ((qspi_get_freq() % hz) == 0) {
        /* Incase the exact requested baud rate can be derived then set right div,
         * else set baudrate to the closest lower value
         */
        div--;
    }

    if (div > 16 || div < 1) {
        status = QSPI_STATUS_INVALID_PARAMETER;
        return status;
    }


    /* Configure QSPI */
    QSPI_Enable(base, false);

    /* Set the divider of QSPI clock */
    base->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;
    base->MCR |= QuadSPI_MCR_SCLKCFG(div);

    QSPI_Enable(base, true);

    return status;
}

static void qspi_prepare_command(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    uint32_t lut_seq[4] = {0, 0, 0, 0};
    uint8_t instr1 = QSPI_CMD, instr2 = 0;
    uint8_t pad1 = command->instruction.bus_width, pad2 = 0;
    uint8_t op1 = command->instruction.value, op2 = 0;
    bool set_jmp_instr = false;
    QuadSPI_Type *base = qspi_addrs[obj->instance];
    uint32_t addr = FSL_FEATURE_QSPI_AMBA_BASE;

    /* Check if the flash address is provided */
    if (command->address.disabled) {
        /* Check if a second instruction is needed, this is for register accesses */
        if (tx_data != NULL && tx_size) {
            instr2 = QSPI_WRITE;
            pad2 = command->data.bus_width;
            op2 = tx_size;
            /* Read and write pointers of the TX buffer are reset to 0 */
            QSPI_ClearFifo(base, kQSPI_TxFifo);
            /* Clear underrun error flag */
            QSPI_ClearErrorFlag(base, kQSPI_TxBufferUnderrun);
        }
        if (rx_data != NULL && rx_size) {
            instr2 = QSPI_READ;
            pad2 = command->data.bus_width;
            op2 = rx_size;
            /* Read and write pointers of the RX buffer are reset to 0 */
            QSPI_ClearFifo(base, kQSPI_RxFifo);
        }

        /* Setup the LUT entry */
        lut_seq[0] = QSPI_LUT_SEQ(instr1, pad1, op1, instr2, pad2, op2);
    } else {
        instr2 = QSPI_ADDR;
        pad2 = command->address.bus_width;
        /* Number of address bits */
        op2 = (8 * (command->address.size + 1));
        addr += command->address.value;

        /* Setup the first LUT entry */
        lut_seq[0] = QSPI_LUT_SEQ(instr1, pad1, op1, instr2, pad2, op2);

        /* Clear the variables */
        instr1 = instr2 = 0;
        pad1 = pad2 = 0;
        op1 = op2 = 0;

        /* Check if a second LUT entry is needed */
        if (tx_data != NULL && tx_size) {
            instr1 = QSPI_WRITE;
            pad1 = command->data.bus_width;
            op1 = 0x80;
            /* Read and write pointers of the TX buffer are reset to 0 */
            QSPI_ClearFifo(base, kQSPI_TxFifo);
            /* Clear underrun error flag */
            QSPI_ClearErrorFlag(base, kQSPI_TxBufferUnderrun);
        }
        if (rx_data != NULL && rx_size) {
            instr1 = QSPI_READ;
            pad1 = command->data.bus_width;
            op1 = 0x80;
            /* Read operations need a jump command at the end of the sequence */
            set_jmp_instr = true;
            /* Read and write pointers of the TX buffer are reset to 0 */
            QSPI_ClearFifo(base, kQSPI_RxFifo);
        }

        /* Setup more LUT entries if needed */
        if (instr1 != 0) {
            /* Check if we need to add dummy entries */
            if (command->dummy_count > 0) {
                instr2 = instr1;
                pad2 = pad1;
                op2 = op1;

                instr1 = QSPI_DUMMY;
                pad1 = command->address.bus_width;
                op1 = command->dummy_count;
            }
            /* Check if need to add jump command entry */
            if (set_jmp_instr) {
                /* Need to add a jump command in the sequence */
                if (command->dummy_count > 0) {
                    /* Third LUT entry for jump command */
                    lut_seq[2] = QSPI_LUT_SEQ(QSPI_JMP_ON_CS, QSPI_PAD_1, LUT3_SEQ_INDEX, 0, 0, 0);
                } else {
                    /* As there is no dummy we have space in the second LUT entry to add jump command */
                    instr2 = QSPI_JMP_ON_CS;
                    pad2 = QSPI_PAD_1;
                    op2 = 8;
                }
            }
            /* Second LUT entry for read & write operations */
            lut_seq[1] = QSPI_LUT_SEQ(instr1, pad1, op1, instr2, pad2, op2);
        }
    }

    /* Update the LUT registers from index 8, prior entries have pre-defined LUT sequences
     * which is used when the instruction is disabled
     */
    if (!(command->instruction.disabled)) {
        QSPI_UpdateLUT(base, LUT3_SEQ_INDEX, lut_seq);
    }

    /* Setup the address */
    QSPI_SetIPCommandAddress(base, addr);

    /* Reset AHB domain and serial flash domain flops */
    QSPI_SoftwareReset(base);
    while (QSPI_GetStatusFlags(base) & kQSPI_Busy) {
    }
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    uint32_t to_write = *length;
    uint8_t *data_send = (uint8_t *)data;
    QuadSPI_Type *base = qspi_addrs[obj->instance];

    /* At least four words of data must be written to the TX buffer */
    if (to_write < MIN_SIZE) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    /* Enforce word-sized access */
    if ((to_write & 0x3) != 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    core_util_critical_section_enter();
    /* Prepare the write command */
    qspi_prepare_command(obj, command, data, to_write, NULL, 0);

    QSPI_SetIPCommandSize(base, to_write);

    if (to_write > (FSL_FEATURE_QSPI_TXFIFO_DEPTH * 4)) {
        /* First write some data into TXFIFO to prevent underrun */
        QSPI_WriteBlocking(base, (uint32_t *)data_send, FSL_FEATURE_QSPI_TXFIFO_DEPTH * 4);
        data_send += (FSL_FEATURE_QSPI_TXFIFO_DEPTH * 4);
        to_write -= (FSL_FEATURE_QSPI_TXFIFO_DEPTH * 4);
    } else {
        QSPI_WriteBlocking(base, (uint32_t *)data_send, to_write);
        to_write = 0;
    }

    /* Start the program */
    if (command->instruction.disabled) {
        /* If no instruction provided then use the pre-defined write sequence */
        QSPI_ExecuteIPCommand(base, LUT2_SEQ_INDEX);
    } else {
        /* Use the user-defined write sequence */
        QSPI_ExecuteIPCommand(base, LUT3_SEQ_INDEX);
    }

    /* Write the remaining data to TXFIFO */
    if (to_write) {
        QSPI_WriteBlocking(base, (uint32_t *)data_send, to_write);
    }
    core_util_critical_section_exit();

    while (QSPI_GetStatusFlags(base) & (kQSPI_Busy | kQSPI_IPAccess)) {
    }

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    uint32_t dest_addr = FSL_FEATURE_QSPI_AMBA_BASE + command->address.value;
    uint32_t to_read = *length;
    QuadSPI_Type *base = qspi_addrs[obj->instance];

    /* Enforce word-sized access */
    if ((to_read & 0x3) != 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    /* Prepare for read command */
    qspi_prepare_command(obj, command, NULL, 0, data, to_read);

    /* Point to the read sequence in the LUT */
    if (command->instruction.disabled) {
        /* If no instruction provided then use the pre-defined read sequence */
        QSPI_ExecuteAHBCommand(base, LUT1_SEQ_INDEX);
    } else {
        /* Use the user-defined write sequence */
        QSPI_ExecuteAHBCommand(base, LUT3_SEQ_INDEX);
    }

    for (uint32_t i = 0; i < to_read / 4; i++) {
        ((uint32_t*)data)[i] = *((uint32_t *)(dest_addr) + i);
    }

    while (QSPI_GetStatusFlags(base) & (kQSPI_Busy | kQSPI_AHBAccess)) {
    }

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    QuadSPI_Type *base = qspi_addrs[obj->instance];

    if (tx_size > MIN_SIZE || rx_size > MIN_SIZE) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        /* Setup the sequence in the Look-up Table (LUT) */
        qspi_prepare_command(obj, command, tx_data, tx_size, rx_data, rx_size);

        /* Execute the sequence */
        QSPI_ExecuteIPCommand(base, LUT3_SEQ_INDEX);
        while (QSPI_GetStatusFlags(base) & (kQSPI_Busy | kQSPI_IPAccess)) {
        }
    } else {
        if (tx_data != NULL && tx_size) {
            /* Transmit data to QSPI */
            /* Need to write at least 16 bytes into TX buffer */
            uint8_t val[MIN_SIZE];
            memset(val, 0, sizeof(val));
            memcpy(val, tx_data, tx_size);

            /* Setup the sequence in the Look-up Table (LUT) */
            qspi_prepare_command(obj, command, tx_data, tx_size, rx_data, rx_size);

            /* First write some data into TXFIFO to prevent from underrun */
            QSPI_WriteBlocking(base, (uint32_t *)val, MIN_SIZE);

            /* Use the user-defined write sequence */
            QSPI_ExecuteIPCommand(base, LUT3_SEQ_INDEX);
            while (QSPI_GetStatusFlags(base) & (kQSPI_Busy | kQSPI_IPAccess)) {
            }
        }

        if (rx_data != NULL && rx_size) {
            /* Receive data from QSPI */
            uint32_t val[MIN_SIZE / 4];
            memset(val, 0, sizeof(val));

            /* Read data from the IP read buffers */
            QSPI_SetReadDataArea(base, kQSPI_ReadIP);

            /* Setup the sequence in the Look-up Table (LUT) */
            qspi_prepare_command(obj, command, tx_data, tx_size, rx_data, rx_size);

            /* Execute the sequence */
            QSPI_SetIPCommandSize(base, rx_size);
            QSPI_ExecuteIPCommand(base, LUT3_SEQ_INDEX);
            while (QSPI_GetStatusFlags(base) & (kQSPI_Busy | kQSPI_IPAccess)) {
            }

            for (uint32_t i = 0, j =0; i < rx_size; i += 4, j++) {
                val[j] = QSPI_ReadData(base);
            }

            memcpy(rx_data, val, rx_size);
        }
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

