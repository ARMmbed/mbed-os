/*
 * Copyright (c) 2020, Arm Limited and affiliates.
 * Copyright (c) 2020, STMicroelectronics.
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

#if DEVICE_OSPI

#include "ospi_api.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "STOS"

// activate / de-activate debug
#define ospi_api_c_debug 0

/* Max amount of flash size is 4Gbytes */
/* hence 2^(31+1), then FLASH_SIZE_DEFAULT = 1<<31 */
#define OSPI_FLASH_SIZE_DEFAULT 0x4000000 //512Mbits

static uint32_t get_alt_bytes_size(const uint32_t num_bytes)
{
    switch (num_bytes) {
        case 1:
            return HAL_OSPI_ALTERNATE_BYTES_8_BITS;
        case 2:
            return HAL_OSPI_ALTERNATE_BYTES_16_BITS;
        case 3:
            return HAL_OSPI_ALTERNATE_BYTES_24_BITS;
        case 4:
            return HAL_OSPI_ALTERNATE_BYTES_32_BITS;
    }
    error("Invalid alt bytes size");
    return 0xFFFFFFFF;
}


ospi_status_t ospi_prepare_command(const ospi_command_t *command, OSPI_RegularCmdTypeDef *st_command)
{
    debug_if(ospi_api_c_debug, "ospi_prepare_command In: instruction.value %x dummy_count %x address.bus_width %x address.disabled %x address.value %x address.size %x\n",
             command->instruction.value, command->dummy_count, command->address.bus_width, command->address.disabled, command->address.value, command->address.size);

    st_command->FlashId = HAL_OSPI_FLASH_ID_1;

    if (command->instruction.disabled == true) {
        st_command->InstructionMode = HAL_OSPI_INSTRUCTION_NONE;
        st_command->Instruction = 0;
    } else {
        st_command->Instruction = ((command->instruction.bus_width == OSPI_CFG_BUS_OCTA) || (command->instruction.bus_width == OSPI_CFG_BUS_OCTA_DTR))
                                  ? command->instruction.value << 8 | (0xFF - command->instruction.value) : command->instruction.value;
        switch (command->instruction.bus_width) {
            case OSPI_CFG_BUS_SINGLE:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
                break;
            case OSPI_CFG_BUS_DUAL:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_2_LINES;
                break;
            case OSPI_CFG_BUS_QUAD:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_4_LINES;
                break;
            case OSPI_CFG_BUS_OCTA:
            case OSPI_CFG_BUS_OCTA_DTR:
                st_command->InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
                break;
            default:
                error("Command param error: wrong instruction format\n");
                return OSPI_STATUS_ERROR;
        }
    }

    st_command->InstructionSize    = (st_command->InstructionMode == HAL_OSPI_INSTRUCTION_8_LINES) ? HAL_OSPI_INSTRUCTION_16_BITS : HAL_OSPI_INSTRUCTION_8_BITS;
    st_command->InstructionDtrMode = (command->instruction.bus_width == OSPI_CFG_BUS_OCTA_DTR) ? HAL_OSPI_INSTRUCTION_DTR_ENABLE : HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    st_command->DummyCycles = command->dummy_count;
    // these are target specific settings, use default values
    st_command->SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
    st_command->DataDtrMode = (command->instruction.bus_width == OSPI_CFG_BUS_OCTA_DTR) ? HAL_OSPI_DATA_DTR_ENABLE : HAL_OSPI_DATA_DTR_DISABLE;
    st_command->AddressDtrMode = (command->instruction.bus_width == OSPI_CFG_BUS_OCTA_DTR) ? HAL_OSPI_ADDRESS_DTR_ENABLE : HAL_OSPI_ADDRESS_DTR_DISABLE;
    st_command->AlternateBytesDtrMode = (command->instruction.bus_width == OSPI_CFG_BUS_OCTA_DTR) ? HAL_OSPI_ALTERNATE_BYTES_DTR_ENABLE : HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
    st_command->DQSMode = (command->instruction.bus_width == OSPI_CFG_BUS_OCTA_DTR) ? HAL_OSPI_DQS_ENABLE : HAL_OSPI_DQS_DISABLE;

    st_command->OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
    if (command->address.disabled == true) {
        st_command->AddressMode = HAL_OSPI_ADDRESS_NONE;
        st_command->AddressSize = 0;
    } else {
        st_command->Address = command->address.value;
        switch (command->address.bus_width) {
            case OSPI_CFG_BUS_SINGLE:
                st_command->AddressMode = HAL_OSPI_ADDRESS_1_LINE;
                break;
            case OSPI_CFG_BUS_DUAL:
                st_command->AddressMode = HAL_OSPI_ADDRESS_2_LINES;
                break;
            case OSPI_CFG_BUS_QUAD:
                st_command->AddressMode = HAL_OSPI_ADDRESS_4_LINES;
                break;
            case OSPI_CFG_BUS_OCTA:
            case OSPI_CFG_BUS_OCTA_DTR:
                st_command->AddressMode = HAL_OSPI_ADDRESS_8_LINES;
                break;
            default:
                error("Command param error: wrong address size\n");
                return OSPI_STATUS_ERROR;
        }
        switch (command->address.size) {
            case OSPI_CFG_ADDR_SIZE_8:
                st_command->AddressSize = HAL_OSPI_ADDRESS_8_BITS;
                break;
            case OSPI_CFG_ADDR_SIZE_16:
                st_command->AddressSize = HAL_OSPI_ADDRESS_16_BITS;
                break;
            case OSPI_CFG_ADDR_SIZE_24:
                st_command->AddressSize = HAL_OSPI_ADDRESS_24_BITS;
                break;
            case OSPI_CFG_ADDR_SIZE_32:
                st_command->AddressSize = HAL_OSPI_ADDRESS_32_BITS;
                break;
            default:
                error("Command param error: wrong address size\n");
                return OSPI_STATUS_ERROR;
        }
    }

    if (command->alt.disabled == true) {
        st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
        st_command->AlternateBytesSize = 0;
    } else {
        uint8_t alt_lines = 0;
        switch (command->alt.bus_width) {
            case OSPI_CFG_BUS_SINGLE:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_1_LINE;
                alt_lines = 1;
                break;
            case OSPI_CFG_BUS_DUAL:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_2_LINES;
                alt_lines = 2;
                break;
            case OSPI_CFG_BUS_QUAD:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_4_LINES;
                alt_lines = 4;
                break;
            case OSPI_CFG_BUS_OCTA:
            case OSPI_CFG_BUS_OCTA_DTR:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_8_LINES;
                alt_lines = 8;
                break;
            default:
                st_command->AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
                error("Command param error: invalid alt bytes mode\n");
                return OSPI_STATUS_ERROR;
        }

        // Alt size must be a multiple of the number of bus lines used (i.e. a whole number of cycles)
        if (command->alt.size % alt_lines != 0) {
            error("Command param error: incompatible alt size and alt bus width\n");
            return OSPI_STATUS_ERROR;
        }

        // Round up to nearest byte - unused parts of byte act as dummy cycles
        uint32_t alt_bytes = ((command->alt.size - 1) >> 3) + 1;
        // Maximum of 4 alt bytes
        if (alt_bytes > 4) {
            error("Command param error: alt size exceeds maximum of 32 bits\n");
            return OSPI_STATUS_ERROR;
        }

        // Unused bits in most significant byte of alt
        uint8_t leftover_bits = (alt_bytes << 3) - command->alt.size;
        if (leftover_bits != 0) {
            // Account for dummy cycles that will be spent in the alt portion of the command
            uint8_t integrated_dummy_cycles = leftover_bits / alt_lines;
            if (st_command->DummyCycles < integrated_dummy_cycles) {
                // Not enough dummy cycles to account for a short alt
                error("Command param error: not enough dummy cycles to make up for given alt size\n");
                return OSPI_STATUS_ERROR;
            }
            st_command->DummyCycles -= integrated_dummy_cycles;

            // Align alt value to the end of the most significant byte
            st_command->AlternateBytes = command->alt.value << leftover_bits;
        } else {
            st_command->AlternateBytes = command->alt.value;
        }

        st_command->AlternateBytesSize = get_alt_bytes_size(alt_bytes);
    }

    switch (command->data.bus_width) {
        case OSPI_CFG_BUS_SINGLE:
            st_command->DataMode = HAL_OSPI_DATA_1_LINE;
            break;
        case OSPI_CFG_BUS_DUAL:
            st_command->DataMode = HAL_OSPI_DATA_2_LINES;
            break;
        case OSPI_CFG_BUS_QUAD:
            st_command->DataMode = HAL_OSPI_DATA_4_LINES;
            break;
        case OSPI_CFG_BUS_OCTA:
        case OSPI_CFG_BUS_OCTA_DTR:
            st_command->DataMode = HAL_OSPI_DATA_8_LINES;
            break;
        default:
            st_command->DataMode = HAL_OSPI_DATA_NONE;
            break;
    }

    debug_if(ospi_api_c_debug, "ospi_prepare_command Out: InstructionMode %x Instruction %x AddressMode %x AddressSize %x Address %x DataMode %x\n",
             st_command->InstructionMode, st_command->Instruction, st_command->AddressMode, st_command->AddressSize, st_command->Address, st_command->DataMode);

    return OSPI_STATUS_OK;
}


#if STATIC_PINMAP_READY
#define OSPI_INIT_DIRECT ospi_init_direct
ospi_status_t ospi_init_direct(ospi_t *obj, const ospi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#else
#define OSPI_INIT_DIRECT _ospi_init_direct
static ospi_status_t _ospi_init_direct(ospi_t *obj, const ospi_pinmap_t *pinmap, uint32_t hz, uint8_t mode)
#endif
{
    tr_debug("ospi_init mode %u", mode);

    // Reset handle internal state
    obj->handle.State = HAL_OSPI_STATE_RESET;

    // Set default OCTOSPI handle values
    obj->handle.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
//#if defined(TARGET_MX25LM512451G)
//   obj->handle.Init.MemoryType = HAL_OSPI_MEMTYPE_MACRONIX; // Read sequence in DTR mode: D1-D0-D3-D2
//#else
    obj->handle.Init.MemoryType = HAL_OSPI_MEMTYPE_MICRON;   // Read sequence in DTR mode: D0-D1-D2-D3
//#endif
    obj->handle.Init.ClockPrescaler = 4; // default value, will be overwritten in ospi_frequency
    obj->handle.Init.FifoThreshold = 4;
    obj->handle.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
    obj->handle.Init.DeviceSize = 32;
    obj->handle.Init.ChipSelectHighTime = 3;
    obj->handle.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
#if defined(HAL_OSPI_WRAP_NOT_SUPPORTED) // removed in STM32L4
    obj->handle.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
#endif
    obj->handle.Init.ClockMode = mode == 0 ? HAL_OSPI_CLOCK_MODE_0 : HAL_OSPI_CLOCK_MODE_3;
    obj->handle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
    obj->handle.Init.ChipSelectBoundary = 0;
#if defined(HAL_OSPI_DELAY_BLOCK_USED) // STM32L5
    obj->handle.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_USED;
#endif
#if defined(TARGET_STM32L5)
    obj->handle.Init.Refresh = 0;
#endif

    // tested all combinations, take first
    obj->ospi = pinmap->peripheral;

#if defined(OCTOSPI1)
    if (obj->ospi == OSPI_1) {
        obj->handle.Instance = OCTOSPI1;
    }
#endif
#if defined(OCTOSPI2)
    if (obj->ospi == OSPI_2) {
        obj->handle.Instance = OCTOSPI2;
    }
#endif

#if defined(OCTOSPI1)
    if (obj->ospi == OSPI_1) {
        __HAL_RCC_OSPI1_CLK_ENABLE();
        __HAL_RCC_OSPI1_FORCE_RESET();
        __HAL_RCC_OSPI1_RELEASE_RESET();
    }
#endif
#if defined(OCTOSPI2)
    if (obj->ospi == OSPI_2) {
        __HAL_RCC_OSPI2_CLK_ENABLE();
        __HAL_RCC_OSPI2_FORCE_RESET();
        __HAL_RCC_OSPI2_RELEASE_RESET();
    }
#endif

    // pinmap for pins (enable clock)
    obj->io0 = pinmap->data0_pin;
    pin_function(pinmap->data0_pin, pinmap->data0_function);
    pin_mode(pinmap->data0_pin, PullNone);
    obj->io1 = pinmap->data1_pin;
    pin_function(pinmap->data1_pin, pinmap->data1_function);
    pin_mode(pinmap->data1_pin, PullNone);
    obj->io2 = pinmap->data2_pin;
    pin_function(pinmap->data2_pin, pinmap->data2_function);
    pin_mode(pinmap->data2_pin, PullNone);
    obj->io3 = pinmap->data3_pin;
    pin_function(pinmap->data3_pin, pinmap->data3_function);
    pin_mode(pinmap->data3_pin, PullNone);
    obj->io4 = pinmap->data4_pin;
    pin_function(pinmap->data4_pin, pinmap->data4_function);
    pin_mode(pinmap->data4_pin, PullNone);
    obj->io5 = pinmap->data5_pin;
    pin_function(pinmap->data5_pin, pinmap->data5_function);
    pin_mode(pinmap->data5_pin, PullNone);
    obj->io6 = pinmap->data6_pin;
    pin_function(pinmap->data6_pin, pinmap->data6_function);
    pin_mode(pinmap->data6_pin, PullNone);
    obj->io7 = pinmap->data7_pin;
    pin_function(pinmap->data7_pin, pinmap->data7_function);
    pin_mode(pinmap->data7_pin, PullNone);

    obj->sclk  = pinmap->sclk_pin;
    pin_function(pinmap->sclk_pin, pinmap->sclk_function);
    pin_mode(pinmap->sclk_pin, PullNone);
    obj->ssel = pinmap->ssel_pin;
    pin_function(pinmap->ssel_pin, pinmap->ssel_function);
    pin_mode(pinmap->ssel_pin, PullNone);
    obj->dqs = pinmap->dqs_pin;
    pin_function(pinmap->dqs_pin, pinmap->dqs_function);
    pin_mode(pinmap->dqs_pin, PullNone);

#if defined(OCTOSPI2)
    __HAL_RCC_OSPIM_CLK_ENABLE();

    OSPIM_CfgTypeDef OSPIM_Cfg_Struct = {0};

    /* The OctoSPI IO Manager OCTOSPIM configuration is supported in a simplified mode in mbed-os
     * OSPI1 signals are mapped to port 1 and OSPI2 signals are mapped to port 2.
     * This  is coded in this way in PeripheralPins.c */
    if (obj->ospi == OSPI_1) {
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

    if (HAL_OSPIM_Config(&obj->handle, &OSPIM_Cfg_Struct, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        tr_error("HAL_OSPIM_Config error");
        return OSPI_STATUS_ERROR;
    }
#endif

    return ospi_frequency(obj, hz);
}

ospi_status_t ospi_init(ospi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName io4, PinName io5, PinName io6, PinName io7,
                        PinName sclk, PinName ssel, PinName dqs, uint32_t hz, uint8_t mode)
{
    OSPIName ospiio0name = (OSPIName)pinmap_peripheral(io0, PinMap_OSPI_DATA0);
    OSPIName ospiio1name = (OSPIName)pinmap_peripheral(io1, PinMap_OSPI_DATA1);
    OSPIName ospiio2name = (OSPIName)pinmap_peripheral(io2, PinMap_OSPI_DATA2);
    OSPIName ospiio3name = (OSPIName)pinmap_peripheral(io3, PinMap_OSPI_DATA3);
    OSPIName ospiio4name = (OSPIName)pinmap_peripheral(io4, PinMap_OSPI_DATA4);
    OSPIName ospiio5name = (OSPIName)pinmap_peripheral(io5, PinMap_OSPI_DATA5);
    OSPIName ospiio6name = (OSPIName)pinmap_peripheral(io6, PinMap_OSPI_DATA6);
    OSPIName ospiio7name = (OSPIName)pinmap_peripheral(io7, PinMap_OSPI_DATA7);
    OSPIName ospiclkname = (OSPIName)pinmap_peripheral(sclk, PinMap_OSPI_SCLK);
    OSPIName ospisselname = (OSPIName)pinmap_peripheral(ssel, PinMap_OSPI_SSEL);
    OSPIName ospidqsname = (OSPIName)pinmap_peripheral(dqs, PinMap_OSPI_DQS);

    OSPIName ospi_data_first = (OSPIName)pinmap_merge(ospiio0name, ospiio1name);
    OSPIName ospi_data_second = (OSPIName)pinmap_merge(ospiio2name, ospiio3name);
    OSPIName ospi_data_third = (OSPIName)pinmap_merge(ospiclkname, ospisselname);

    if (ospi_data_first != ospi_data_second || ospi_data_second != ospi_data_third ||
            ospi_data_first != ospi_data_third) {
        return OSPI_STATUS_INVALID_PARAMETER;
    }

    int peripheral = (int)ospi_data_first;
    int function_io0 = (int)pinmap_find_function(io0, PinMap_OSPI_DATA0);
    int function_io1 = (int)pinmap_find_function(io1, PinMap_OSPI_DATA1);
    int function_io2 = (int)pinmap_find_function(io2, PinMap_OSPI_DATA2);
    int function_io3 = (int)pinmap_find_function(io3, PinMap_OSPI_DATA3);
    int function_io4 = (int)pinmap_find_function(io4, PinMap_OSPI_DATA4);
    int function_io5 = (int)pinmap_find_function(io5, PinMap_OSPI_DATA5);
    int function_io6 = (int)pinmap_find_function(io6, PinMap_OSPI_DATA6);
    int function_io7 = (int)pinmap_find_function(io7, PinMap_OSPI_DATA7);

    int function_sclk = (int)pinmap_find_function(sclk, PinMap_OSPI_SCLK);
    int function_ssel = (int)pinmap_find_function(ssel, PinMap_OSPI_SSEL);
    int function_dqs = (int)pinmap_find_function(dqs, PinMap_OSPI_DQS);

    const ospi_pinmap_t static_pinmap = {peripheral, io0, function_io0, io1, function_io1, io2, function_io2, io3, function_io3,
                                         io4, function_io4, io5, function_io5, io6, function_io6, io7, function_io7,
                                         sclk, function_sclk, ssel, function_ssel, dqs, function_dqs
                                        };

    return OSPI_INIT_DIRECT(obj, &static_pinmap, hz, mode);
}


ospi_status_t ospi_free(ospi_t *obj)
{
    tr_debug("ospi_free");
    if (HAL_OSPI_DeInit(&obj->handle) != HAL_OK) {
        return OSPI_STATUS_ERROR;
    }

#if defined(OCTOSPI1)
    if (obj->ospi == OSPI_1) {
        __HAL_RCC_OSPI1_FORCE_RESET();
        __HAL_RCC_OSPI1_CLK_DISABLE();
    }
#endif
#if defined(OCTOSPI2)
    if (obj->ospi == OSPI_2) {
        __HAL_RCC_OSPI2_FORCE_RESET();
        __HAL_RCC_OSPI2_CLK_DISABLE();
    }
#endif

    // Configure GPIOs back to reset value
    pin_function(obj->io0, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io1, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io2, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io3, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io4, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io5, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io6, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->io7, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->sclk, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->ssel, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    pin_function(obj->dqs, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));

    (void)(obj);
    return OSPI_STATUS_OK;
}


ospi_status_t ospi_frequency(ospi_t *obj, int hz)
{
    tr_debug("ospi_frequency hz %d", hz);
    ospi_status_t status = OSPI_STATUS_OK;

    /* HCLK drives OSPI. OSPI clock depends on prescaler value:
    *  0: Freq = HCLK
    *  1: Freq = HCLK/2
    *  ...
    *  255: Freq = HCLK/256 (minimum value)
    */

    int div = HAL_RCC_GetHCLKFreq() / hz;
    if (div > 255) {
        div = 255;
    } else {
        if (div == 1) {
            div = div + 1;
        }
    }

    obj->handle.Init.ClockPrescaler = div;

    if (HAL_OSPI_Init(&obj->handle) != HAL_OK) {
        tr_error("HAL_OSPI_Init error");
        status = OSPI_STATUS_ERROR;
    }

    return status;
}


ospi_status_t ospi_write(ospi_t *obj, const ospi_command_t *command, const void *data, size_t *length)
{
    debug_if(ospi_api_c_debug, "ospi_write size %u\n", *length);

    OSPI_RegularCmdTypeDef st_command;
    ospi_status_t status = ospi_prepare_command(command, &st_command);
    if (status != OSPI_STATUS_OK) {
        return status;
    }

    st_command.NbData = *length;

    if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        tr_error("HAL_OSPI_Command error");
        status = OSPI_STATUS_ERROR;
    } else {
        if (HAL_OSPI_Transmit(&obj->handle, (uint8_t *)data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_OSPI_Transmit error");
            status = OSPI_STATUS_ERROR;
        }
    }

    return status;
}

ospi_status_t ospi_read(ospi_t *obj, const ospi_command_t *command, void *data, size_t *length)
{
    OSPI_RegularCmdTypeDef st_command;
    ospi_status_t status = ospi_prepare_command(command, &st_command);
    if (status != OSPI_STATUS_OK) {
        return status;
    }

    st_command.NbData = *length;

    if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        tr_error("HAL_OSPI_Command error");
        status = OSPI_STATUS_ERROR;
    } else {
        if (HAL_OSPI_Receive(&obj->handle, data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            tr_error("HAL_OSPI_Receive error %d", obj->handle.ErrorCode);
            status = OSPI_STATUS_ERROR;
        }
    }

    debug_if(ospi_api_c_debug, "ospi_read size %u\n", *length);

    return status;
}

ospi_status_t ospi_command_transfer(ospi_t *obj, const ospi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    tr_debug("ospi_command_transfer tx %u rx %u command %#04x", tx_size, rx_size, command->instruction.value);
    ospi_status_t status = OSPI_STATUS_OK;

    if ((tx_data == NULL || tx_size == 0) && (rx_data == NULL || rx_size == 0)) {
        // only command, no rx or tx
        OSPI_RegularCmdTypeDef st_command;
        status = ospi_prepare_command(command, &st_command);
        if (status != OSPI_STATUS_OK) {
            return status;
        }

        st_command.NbData = 1;
        st_command.DataMode = HAL_OSPI_DATA_NONE; /* Instruction only */
        if (HAL_OSPI_Command(&obj->handle, &st_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            status = OSPI_STATUS_ERROR;
            tr_error("HAL_OSPI_Command error");
            return status;
        }
    } else {
        // often just read a register, check if we need to transmit anything prior reading
        if (tx_data != NULL && tx_size) {
            size_t tx_length = tx_size;
            status = ospi_write(obj, command, tx_data, &tx_length);
            if (status != OSPI_STATUS_OK) {
                tr_error("qspi_write error");
                return status;
            }
        }

        if (rx_data != NULL && rx_size) {
            size_t rx_length = rx_size;
            status = ospi_read(obj, command, rx_data, &rx_length);
        }
    }
    return status;
}


const PinMap *ospi_master_sclk_pinmap()
{
    return PinMap_OSPI_SCLK;
}

const PinMap *ospi_master_ssel_pinmap()
{
    return PinMap_OSPI_SSEL;
}

const PinMap *ospi_master_dqs_pinmap()
{
    return PinMap_OSPI_DQS;
}

const PinMap *ospi_master_data0_pinmap()
{
    return PinMap_OSPI_DATA0;
}

const PinMap *ospi_master_data1_pinmap()
{
    return PinMap_OSPI_DATA1;
}

const PinMap *ospi_master_data2_pinmap()
{
    return PinMap_OSPI_DATA2;
}

const PinMap *ospi_master_data3_pinmap()
{
    return PinMap_OSPI_DATA3;
}

const PinMap *ospi_master_data4_pinmap()
{
    return PinMap_OSPI_DATA4;
}

const PinMap *ospi_master_data5_pinmap()
{
    return PinMap_OSPI_DATA5;
}

const PinMap *ospi_master_data6_pinmap()
{
    return PinMap_OSPI_DATA6;
}

const PinMap *ospi_master_data7_pinmap()
{
    return PinMap_OSPI_DATA7;
}

#endif

/** @}*/
