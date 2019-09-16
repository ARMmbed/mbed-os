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
#include "PeripheralPins.h"

// activate / de-activate debug
#define qspi_api_c_debug 0

/* Max amount of flash size is 4Gbytes */
/* hence 2^(31+1), then FLASH_SIZE_DEFAULT = 1<<31 */
#define QSPI_FLASH_SIZE_DEFAULT 0x80000000

#if defined(OCTOSPI1)
void qspi_prepare_command(const qspi_command_t *command, OSPI_RegularCmdTypeDef *st_command)
{
    debug_if(qspi_api_c_debug, "qspi_prepare_command In: instruction.value %x dummy_count %x address.bus_width %x address.disabled %x address.value %x address.size %x\n",
        command->instruction.value, command->dummy_count, command->address.bus_width, command->address.disabled, command->address.value, command->address.size);

    st_command->FlashId = HAL_OSPI_FLASH_ID_1;

    if (command->instruction.disabled == true) {
        st_command->InstructionMode = HAL_OSPI_INSTRUCTION_NONE;
        st_command->Instruction = 0;
    } else {
        st_command->Instruction = command->instruction.value;
        switch (command->instruction.bus_width) {
            case QSPI_CFG_BUS_SINGLE:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
                break;
            case QSPI_CFG_BUS_DUAL:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_2_LINES;
                break;
            case QSPI_CFG_BUS_QUAD:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_4_LINES;
                break;
            default:
                error("Command param error: wrong istruction format\n");
                break;
        }
    }

    st_command->InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    st_command->InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    st_command->DummyCycles = command->dummy_count;
    // these are target specific settings, use default values
    st_command->SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
    st_command->DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;
    st_command->AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE;
    st_command->AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
    st_command->DQSMode = HAL_OSPI_DQS_DISABLE;

    st_command->OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
    if (command->address.disabled == true) {
        st_command->AddressMode = HAL_OSPI_ADDRESS_NONE;
        st_command->AddressSize = 0;
    } else {
        st_command->Address = command->address.value;
        switch (command->address.bus_width) {
            case QSPI_CFG_BUS_SINGLE:
                st_command->AddressMode = HAL_OSPI_ADDRESS_1_LINE;
                break;
            case QSPI_CFG_BUS_DUAL:
                st_command->AddressMode = HAL_OSPI_ADDRESS_2_LINES;
                break;
            case QSPI_CFG_BUS_QUAD:
                st_command->AddressMode = HAL_OSPI_ADDRESS_4_LINES;
                break;
            default:
                error("Command param error: wrong address size\n");
                break;
        }
        switch(command->address.size) {
            case QSPI_CFG_ADDR_SIZE_8:
                st_command->AddressSize = HAL_OSPI_ADDRESS_8_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_16:
                st_command->AddressSize = HAL_OSPI_ADDRESS_16_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_24:
                st_command->AddressSize = HAL_OSPI_ADDRESS_24_BITS;
                break;
            case QSPI_CFG_ADDR_SIZE_32:
                st_command->AddressSize = HAL_OSPI_ADDRESS_32_BITS;
                break;
            default:
                error("Command param error: wrong address size\n");
                break;
        }
    }

    if (command->alt.disabled == true) {
        st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
        st_command->AlternateBytesSize = 0;
    } else {
        st_command->AlternateBytes = command->alt.value;
        switch (command->alt.bus_width) {
            case QSPI_CFG_BUS_SINGLE:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_1_LINE;
                break;
            case QSPI_CFG_BUS_DUAL:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_2_LINES;
                break;
            case QSPI_CFG_BUS_QUAD:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_4_LINES;
                break;
            default:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
                break;
        }
        switch(command->alt.size) {
            case QSPI_CFG_ALT_SIZE_8:
                st_command->AlternateBytesSize = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
                break;
            case QSPI_CFG_ALT_SIZE_16:
                st_command->AlternateBytesSize = HAL_OSPI_ALTERNATE_BYTES_16_BITS;
                break;
            case QSPI_CFG_ALT_SIZE_24:
                st_command->AlternateBytesSize = HAL_OSPI_ALTERNATE_BYTES_24_BITS;
                break;
            case QSPI_CFG_ALT_SIZE_32:
                st_command->AlternateBytesSize = HAL_OSPI_ALTERNATE_BYTES_32_BITS;
                break;
            default:
                st_command->AlternateBytesSize = 0;
                printf("Command param error: wrong address size\n");
                break;
        }
    }

    switch (command->data.bus_width) {
        case QSPI_CFG_BUS_SINGLE:
            st_command->DataMode = HAL_OSPI_DATA_1_LINE;
            break;
        case QSPI_CFG_BUS_DUAL:
            st_command->DataMode = HAL_OSPI_DATA_2_LINES;
            break;
        case QSPI_CFG_BUS_QUAD:
            st_command->DataMode = HAL_OSPI_DATA_4_LINES;
            break;
        default:
            st_command->DataMode = HAL_OSPI_DATA_NONE;
            break;
    }

    debug_if(qspi_api_c_debug, "qspi_prepare_command Out: InstructionMode %x Instruction %x AddressMode %x AddressSize %x Address %x DataMode %x\n",
        st_command->InstructionMode, st_command->Instruction, st_command->AddressMode, st_command->AddressSize, st_command->Address, st_command->DataMode);
}
#else /* OCTOSPI */
void qspi_prepare_command(const qspi_command_t *command, QSPI_CommandTypeDef *st_command)
{
    debug_if(qspi_api_c_debug, "qspi_prepare_command In: instruction.value %x dummy_count %x address.bus_width %x address.disabled %x address.value %x address.size %x\n",
        command->instruction.value, command->dummy_count, command->address.bus_width, command->address.disabled, command->address.value, command->address.size);

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
    debug_if(qspi_api_c_debug, "qspi_prepare_command Out: InstructionMode %x Instruction %x AddressMode %x AddressSize %x Address %x DataMode %x\n",
        st_command->InstructionMode, st_command->Instruction, st_command->AddressMode, st_command->AddressSize, st_command->Address, st_command->DataMode);
}
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    OSPIM_CfgTypeDef OSPIM_Cfg_Struct = {0};
    debug_if(qspi_api_c_debug, "qspi_init mode %u\n", mode);

    // Reset handle internal state
    obj->handle.State = HAL_OSPI_STATE_RESET;

    // Set default OCTOSPI handle values
    obj->handle.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
    obj->handle.Init.MemoryType = HAL_OSPI_MEMTYPE_MICRON;
    obj->handle.Init.ClockPrescaler = 4; // default value, will be overwritten in qspi_frequency
    obj->handle.Init.FifoThreshold = 4;
    obj->handle.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
    obj->handle.Init.DeviceSize = POSITION_VAL(QSPI_FLASH_SIZE_DEFAULT) - 1;
    obj->handle.Init.ChipSelectHighTime = 3;
    obj->handle.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
    obj->handle.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
    obj->handle.Init.ClockMode = mode == 0 ? HAL_OSPI_CLOCK_MODE_0 : HAL_OSPI_CLOCK_MODE_3;
    obj->handle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
    obj->handle.Init.ChipSelectBoundary = 0;

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
        debug_if(qspi_api_c_debug, "QSPI_STATUS_INVALID_PARAMETER error\n");
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    // tested all combinations, take first
    obj->qspi = qspi_data_third;

#if defined(OCTOSPI1)
    if(obj->qspi == QSPI_1) {
        obj->handle.Instance = OCTOSPI1;
    }
#endif
#if defined(OCTOSPI2)
    if(obj->qspi == QSPI_2) {
        obj->handle.Instance = OCTOSPI2;
    }
#endif

#if defined(OCTOSPI1)
    if(obj->qspi == QSPI_1) {
        __HAL_RCC_OSPI1_CLK_ENABLE();
        __HAL_RCC_OSPIM_CLK_ENABLE();
        __HAL_RCC_OSPI1_FORCE_RESET();
        __HAL_RCC_OSPI1_RELEASE_RESET();
    }
#endif
#if defined(OCTOSPI2)
    if(obj->qspi == QSPI_2) {
        __HAL_RCC_OSPI2_CLK_ENABLE();
        __HAL_RCC_OSPIM_CLK_ENABLE();
        __HAL_RCC_OSPI2_FORCE_RESET();
        __HAL_RCC_OSPI2_RELEASE_RESET();
    }
#endif

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

    /* The OctoSPI IO Manager OCTOSPIM configuration is supported in a simplified mode in mbed-os
     * QSPI1 signals are mapped to port 1 and QSPI2 signals are mapped to port 2.
     * This  is coded in this way in PeripheralPins.c */
    if(obj->qspi == QSPI_1) {
        OSPIM_Cfg_Struct.ClkPort = 1;
        OSPIM_Cfg_Struct.DQSPort    = 1;
        OSPIM_Cfg_Struct.NCSPort = 1;
        OSPIM_Cfg_Struct.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;
        OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
    } else {
        OSPIM_Cfg_Struct.ClkPort = 2;
        OSPIM_Cfg_Struct.DQSPort    = 2;
        OSPIM_Cfg_Struct.NCSPort = 2;
        OSPIM_Cfg_Struct.IOLowPort = HAL_OSPIM_IOPORT_2_LOW;
        OSPIM_Cfg_Struct.IOHighPort = HAL_OSPIM_IOPORT_2_HIGH;
    }

    if (HAL_OSPIM_Config(&obj->handle, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        debug_if(qspi_api_c_debug, "HAL_OSPIM_Config error\n");
        return QSPI_STATUS_ERROR;
    }

    return qspi_frequency(obj, hz);
}
#else /* OCTOSPI */
qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    debug_if(qspi_api_c_debug, "qspi_init mode %u\n", mode);
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
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_free(qspi_t *obj)
{
    debug_if(qspi_api_c_debug, "qspi_free\n");
    if (HAL_OSPI_DeInit(&obj->handle) != HAL_OK) {
        return QSPI_STATUS_ERROR;
    }

#if defined(OCTOSPI1)
    if(obj->qspi == QSPI_1) {
        __HAL_RCC_OSPI1_FORCE_RESET();
        __HAL_RCC_OSPI1_CLK_DISABLE();
    }
#endif
#if defined(OCTOSPI2)
    if(obj->qspi == QSPI_2) {
        __HAL_RCC_OSPI2_FORCE_RESET();
        __HAL_RCC_OSPI2_CLK_DISABLE();
    }
#endif

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
#else /* OCTOSPI */
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
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    debug_if(qspi_api_c_debug, "qspi_frequency hz %d\n", hz);
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

    if (HAL_OSPI_Init(&obj->handle) != HAL_OK) {
        debug_if(qspi_api_c_debug, "HAL_OSPI_Init error\n");
        status = QSPI_STATUS_ERROR;
    }

    return status;
}
#else /* OCTOSPI */
qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    debug_if(qspi_api_c_debug, "qspi_frequency hz %d\n", hz);
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
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    debug_if(qspi_api_c_debug, "qspi_write size %u\n", *length);

    OSPI_RegularCmdTypeDef st_command;
    qspi_prepare_command(command, &st_command);

    st_command.NbData = *length;
    qspi_status_t status = QSPI_STATUS_OK;

    if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        debug_if(qspi_api_c_debug, "HAL_OSPI_Command error\n");
        status = QSPI_STATUS_ERROR;
    } else {
        if (HAL_OSPI_Transmit(&obj->handle, (uint8_t *)data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            debug_if(qspi_api_c_debug, "HAL_OSPI_Transmit error\n");
            status = QSPI_STATUS_ERROR;
        }
    }

    return status;
}
#else /* OCTOSPI */
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

    debug_if(qspi_api_c_debug, "qspi_write size %u\n", *length);

    return status;
}
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    OSPI_RegularCmdTypeDef st_command;
    qspi_prepare_command(command, &st_command);

    st_command.NbData = *length;
    qspi_status_t status = QSPI_STATUS_OK;

    if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        debug_if(qspi_api_c_debug, "HAL_OSPI_Command error\n");
        status = QSPI_STATUS_ERROR;
    } else {
        if (HAL_OSPI_Receive(&obj->handle, data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            debug_if(qspi_api_c_debug, "HAL_OSPI_Receive error\n");
            status = QSPI_STATUS_ERROR;
        }
    }

    debug_if(qspi_api_c_debug, "qspi_read size %u\n", *length);

    return status;
}
#else /* OCTOSPI */
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

    debug_if(qspi_api_c_debug, "qspi_read size %u\n", *length);

    return status;
}
#endif /* OCTOSPI */


#if defined(OCTOSPI1)
qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    debug_if(qspi_api_c_debug, "qspi_command_transfer tx %u rx %u command %x\n", tx_size, rx_size, command->instruction.value);
    qspi_status_t status = QSPI_STATUS_OK;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        // only command, no rx or tx
        OSPI_RegularCmdTypeDef st_command;
        qspi_prepare_command(command, &st_command);

        st_command.NbData = 1;
        st_command.DataMode = HAL_OSPI_DATA_NONE; /* Instruction only */
        if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = QSPI_STATUS_ERROR;
            debug_if(qspi_api_c_debug, "HAL_OSPI_Command error\n");
            return status;
        }
    } else {
        // often just read a register, check if we need to transmit anything prior reading
        if (tx_data != NULL && tx_size) {
            size_t tx_length = tx_size;
            status = qspi_write(obj, command, tx_data, &tx_length);
            if (status != QSPI_STATUS_OK) {
                debug_if(qspi_api_c_debug, "qspi_write error\n");
                return status;
            }
        }

        if (rx_data != NULL && rx_size) {
            size_t rx_length = rx_size;
            status = qspi_read(obj, command, rx_data, &rx_length);
            // debug_if(qspi_api_c_debug, "qspi_read %d\n", status);
        }
    }
    return status;
}
#else /* OCTOSPI */
qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    debug_if(qspi_api_c_debug, "qspi_command_transfer tx %u rx %u command %x\n", tx_size, rx_size, command->instruction.value);
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
#endif /* OCTOSPI */


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
