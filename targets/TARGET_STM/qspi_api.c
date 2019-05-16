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
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

/* Max amount of flash size is 4Gbytes */
/* hence 2^(31+1), then FLASH_SIZE_DEFAULT = 1<<31 */
#define QSPI_FLASH_SIZE_DEFAULT 0x80000000

void qspi_prepare_command(const qspi_command_t *command, QSPI_CommandTypeDef *st_command)
{
    // TODO: shift these around to get more dynamic mapping
    switch (command->instruction.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->InstructionMode = QSPI_INSTRUCTION_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->InstructionMode = QSPI_INSTRUCTION_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->InstructionMode = QSPI_INSTRUCTION_4_LINES;
            break;
        default:
            st_command->InstructionMode = QSPI_INSTRUCTION_NONE;
            break;
    }

    st_command->Instruction = command->instruction.value;
    st_command->DummyCycles = command->dummy_count;
    // these are target specific settings, use default values
    st_command->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    st_command->DdrMode = QSPI_DDR_MODE_DISABLE;
    st_command->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;

    switch (command->address.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->AddressMode = QSPI_ADDRESS_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->AddressMode = QSPI_ADDRESS_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->AddressMode = QSPI_ADDRESS_4_LINES;
            break;
        default:
            st_command->AddressMode = QSPI_ADDRESS_NONE;
            break;
    }

    if (command->address.disabled == true) {
        st_command->AddressMode = QSPI_ADDRESS_NONE;
        st_command->AddressSize = 0;
    } else {
        st_command->Address = command->address.value;
        /* command->address.size needs to be shifted by QUADSPI_CCR_ADSIZE_Pos */
        st_command->AddressSize = (command->address.size << QUADSPI_CCR_ADSIZE_Pos) & QUADSPI_CCR_ADSIZE_Msk;
    }

    switch (command->alt.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
            break;
        default:
            st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
            break;
    }

    if (command->alt.disabled == true) {
        st_command->AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
        st_command->AlternateBytesSize = 0;
    } else {
        st_command->AlternateBytes = command->alt.value;
        /* command->AlternateBytesSize needs to be shifted by QUADSPI_CCR_ABSIZE_Pos */
        st_command->AlternateBytesSize = (command->alt.size << QUADSPI_CCR_ABSIZE_Pos) & QUADSPI_CCR_ABSIZE_Msk;
        st_command->AlternateBytesSize = command->alt.size;
    }

    switch (command->data.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->DataMode = QSPI_DATA_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->DataMode = QSPI_DATA_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->DataMode = QSPI_DATA_4_LINES;
            break;
        default:
            st_command->DataMode = QSPI_DATA_NONE;
            break;
    }

    st_command->NbData = 0;
}


qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    // Enable interface clock for QSPI
    __HAL_RCC_QSPI_CLK_ENABLE();

    // Reset QSPI
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();

    // Reset handle internal state
    obj->handle.State = HAL_QSPI_STATE_RESET;
    obj->handle.Lock = HAL_UNLOCKED;

    // Set default QSPI handle values
    obj->handle.Init.ClockPrescaler = 1;
    obj->handle.Init.FifoThreshold = 1;
    obj->handle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
    obj->handle.Init.FlashSize = POSITION_VAL(QSPI_FLASH_SIZE_DEFAULT) - 1;
    obj->handle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
    obj->handle.Init.ClockMode = QSPI_CLOCK_MODE_0;
#ifdef QSPI_DUALFLASH_ENABLE
    obj->handle.Init.FlashID = QSPI_FLASH_ID_1;
    obj->handle.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
#endif

    obj->handle.Init.ClockMode = mode == 0 ? QSPI_CLOCK_MODE_0 : QSPI_CLOCK_MODE_3;

    QSPIName qspiio0name = (QSPIName)pinmap_peripheral(io0, PinMap_QSPI_DATA0);
    QSPIName qspiio1name = (QSPIName)pinmap_peripheral(io1, PinMap_QSPI_DATA1);
    QSPIName qspiio2name = (QSPIName)pinmap_peripheral(io2, PinMap_QSPI_DATA2);
    QSPIName qspiio3name = (QSPIName)pinmap_peripheral(io3, PinMap_QSPI_DATA3);
    QSPIName qspiclkname = (QSPIName)pinmap_peripheral(sclk, PinMap_QSPI_SCLK);
    QSPIName qspisselname = (QSPIName)pinmap_peripheral(ssel, PinMap_QSPI_SSEL);

    QSPIName qspi_data_first = (QSPIName)pinmap_merge(qspiio0name, qspiio1name);
    QSPIName qspi_data_second = (QSPIName)pinmap_merge(qspiio2name, qspiio3name);
    QSPIName qspi_data_third = (QSPIName)pinmap_merge(qspiclkname, qspisselname);

    if (qspi_data_first != qspi_data_second || qspi_data_second != qspi_data_third ||
            qspi_data_first != qspi_data_third) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    // tested all combinations, take first
    obj->handle.Instance = (QUADSPI_TypeDef *)qspi_data_first;

    // pinmap for pins (enable clock)
    obj->io0 = io0;
    pinmap_pinout(io0, PinMap_QSPI_DATA0);
    obj->io1 = io1;
    pinmap_pinout(io1, PinMap_QSPI_DATA1);
    obj->io2 = io2;
    pinmap_pinout(io2, PinMap_QSPI_DATA2);
    obj->io3 = io3;
    pinmap_pinout(io3, PinMap_QSPI_DATA3);

    obj->sclk = sclk;
    pinmap_pinout(sclk, PinMap_QSPI_SCLK);
    obj->ssel = ssel;
    pinmap_pinout(ssel, PinMap_QSPI_SSEL);

    return qspi_frequency(obj, hz);
}

qspi_status_t qspi_free(qspi_t *obj)
{
    if (HAL_QSPI_DeInit(&obj->handle) != HAL_OK) {
        return QSPI_STATUS_ERROR;
    }

    // Reset QSPI
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();

    // Disable interface clock for QSPI
    __HAL_RCC_QSPI_CLK_DISABLE();

    // Configure GPIOs
    pin_function(obj->io0, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(obj->io1, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(obj->io2, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(obj->io3, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(obj->sclk, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(obj->ssel, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));

    (void)(obj);
    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    qspi_status_t status = QSPI_STATUS_OK;

    /* HCLK drives QSPI. QSPI clock depends on prescaler value:
    *  0: Freq = HCLK
    *  1: Freq = HCLK/2
    *  ...
    *  255: Freq = HCLK/256 (minimum value)
    */

    int div = HAL_RCC_GetHCLKFreq() / hz;
    if (div > 255) {
        div = 255;
    } else {
        if ((HAL_RCC_GetHCLKFreq() % hz) == 0) {
            div = div - 1;
        }
    }

    obj->handle.Init.ClockPrescaler = div;

    if (HAL_QSPI_Init(&obj->handle) != HAL_OK) {
        status = QSPI_STATUS_ERROR;
    }

    return status;
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    QSPI_CommandTypeDef st_command;
    qspi_prepare_command(command, &st_command);

    st_command.NbData = *length;
    qspi_status_t status = QSPI_STATUS_OK;

    if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        status = QSPI_STATUS_ERROR;
    } else {
        if (HAL_QSPI_Transmit(&obj->handle, (uint8_t *)data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = QSPI_STATUS_ERROR;
        }
    }

    return status;
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    QSPI_CommandTypeDef st_command;
    qspi_prepare_command(command, &st_command);

    st_command.NbData = *length;
    qspi_status_t status = QSPI_STATUS_OK;

    if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        status = QSPI_STATUS_ERROR;
    } else {
        if (HAL_QSPI_Receive(&obj->handle, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = QSPI_STATUS_ERROR;
        }
    }

    return status;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    qspi_status_t status = QSPI_STATUS_OK;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        // only command, no rx or tx
        QSPI_CommandTypeDef st_command;
        qspi_prepare_command(command, &st_command);

        st_command.NbData = 1;
        st_command.DataMode = QSPI_DATA_NONE; /* Instruction only */
        if (HAL_QSPI_Command(&obj->handle, &st_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = QSPI_STATUS_ERROR;
            return status;
        }
    } else {
        // often just read a register, check if we need to transmit anything prior reading
        if (tx_data != NULL && tx_size) {
            size_t tx_length = tx_size;
            status = qspi_write(obj, command, tx_data, &tx_length);
            if (status != QSPI_STATUS_OK) {
                return status;
            }
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

/** @}*/
