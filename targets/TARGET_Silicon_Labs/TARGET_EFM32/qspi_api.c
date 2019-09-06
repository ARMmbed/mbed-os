/***************************************************************************//**
 * @file rtc_rtcc.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_QSPI && defined(QSPI_PRESENT)

#include "stddef.h"
#include "qspi_api.h"
#include "mbed_error.h"
#include "em_cmu.h"
#include "em_qspi.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "pinmap_function.h"

#define SUPPORTED_ALT_SIZE 8u

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{

#if defined(QSPI_FLASH_EN)
    pin_mode(QSPI_FLASH_EN, PushPull);
    GPIO_PinOutSet((GPIO_Port_TypeDef)(QSPI_FLASH_EN >> 4 & 0xF), QSPI_FLASH_EN & 0xF);
#endif

    // There's only one QSPI per chip for now
    obj->instance = QSPI0;
    obj->io0 = io0;
    obj->io1 = io1;
    obj->io2 = io2;
    obj->io3 = io3;
    obj->ssel = ssel;
    obj->sclk = sclk;

    CMU_ClockEnable(cmuClock_GPIO, true);

#if (CORE_CLOCK_SOURCE == HFXO)
    CMU_ClockSelectSet(cmuClock_QSPI0REF, cmuSelect_HFXO);
#endif

    CMU_ClockEnable(cmuClock_QSPI0, true);
    CMU_ClockEnable(cmuClock_QSPI0REF, true);

    qspi_frequency(obj, hz);

    if (mode) {
        obj->instance->CONFIG |= QSPI_CONFIG_SELCLKPOL | QSPI_CONFIG_SELCLKPHASE;
    } else {
        obj->instance->CONFIG &= ~(QSPI_CONFIG_SELCLKPOL | QSPI_CONFIG_SELCLKPHASE);
    }

    uint32_t loc = pin_location(io0, PinMap_QSPI_DQ0);
    if (loc != pin_location(io1, PinMap_QSPI_DQ1) ||
        loc != pin_location(io2, PinMap_QSPI_DQ2) ||
        loc != pin_location(io3, PinMap_QSPI_DQ3) ||
        loc != pin_location(sclk, PinMap_QSPI_SCLK) ||
        loc != pin_location(ssel, PinMap_QSPI_CS0)) {
        // All pins need to be on the same location number
        qspi_free(obj);
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    // Configure QSPI pins
    GPIO_PinOutClear((GPIO_Port_TypeDef)(io0 >> 4 & 0xF), io0 & 0xF);
    pin_mode(io0, PushPull);

    GPIO_PinOutClear((GPIO_Port_TypeDef)(io1 >> 4 & 0xF), io1 & 0xF);
    pin_mode(io1, PushPull);

    GPIO_PinOutClear((GPIO_Port_TypeDef)(io2 >> 4 & 0xF), io2 & 0xF);
    pin_mode(io2, PushPull);

    GPIO_PinOutClear((GPIO_Port_TypeDef)(io3 >> 4 & 0xF), io3 & 0xF);
    pin_mode(io3, PushPull);

    GPIO_PinOutClear((GPIO_Port_TypeDef)(sclk >> 4 & 0xF), sclk & 0xF);
    pin_mode(sclk, PushPull);

    GPIO_PinOutSet((GPIO_Port_TypeDef)(ssel >> 4 & 0xF), ssel & 0xF);
    pin_mode(ssel, PushPull);


    // Configure QSPI routing to GPIO
    obj->instance->ROUTELOC0 = loc;
    obj->instance->ROUTEPEN  = QSPI_ROUTEPEN_SCLKPEN
                              | QSPI_ROUTEPEN_CS0PEN
                              | QSPI_ROUTEPEN_DQ0PEN
                              | QSPI_ROUTEPEN_DQ1PEN
                              | QSPI_ROUTEPEN_DQ2PEN
                              | QSPI_ROUTEPEN_DQ3PEN;

    // Configure direct read
    QSPI_ReadConfig_TypeDef readConfig = QSPI_READCONFIG_DEFAULT;
    QSPI_ReadConfig(obj->instance, &readConfig);

    // Configure direct write
    QSPI_WriteConfig_TypeDef writeConfig = QSPI_WRITECONFIG_DEFAULT;
    QSPI_WriteConfig(obj->instance, &writeConfig);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_free(qspi_t *obj)
{
    pin_mode(obj->io0, Disabled);
    pin_mode(obj->io1, Disabled);
    pin_mode(obj->io2, Disabled);
    pin_mode(obj->io3, Disabled);
    pin_mode(obj->ssel, Disabled);
    pin_mode(obj->sclk, Disabled);

    obj->instance->ROUTEPEN = 0;

    QSPI_Enable(obj->instance, false);
    CMU_ClockEnable(cmuClock_QSPI0REF, false);
    CMU_ClockEnable(cmuClock_QSPI0, false);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    if (hz <= 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    QSPI_Enable(obj->instance, false);

    // Need at least a DIV4 for non-PHY mode and SDR transfers
    uint32_t basefreq = CMU_ClockFreqGet(cmuClock_QSPI0REF);
    uint32_t basediv = 4;
    if ((uint32_t)hz < (basefreq / basediv)) {
        basediv = (basefreq / hz) + 1;
    }

    QSPI_Init_TypeDef initQspi = QSPI_INIT_DEFAULT;
    initQspi.divisor = basediv;
    QSPI_Init(obj->instance, &initQspi);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    QSPI_WriteConfig_TypeDef cfg = QSPI_WRITECONFIG_DEFAULT;
    uint32_t to_write = *length;

    // Enforce word-sized access
    if ((to_write & 0x3) != 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    cfg.dummyCycles = command->dummy_count;

    if (command->instruction.disabled) {
        cfg.opCode = 0x02;
    } else {
        cfg.opCode = command->instruction.value;
    }

    if (command->address.disabled) {
        return QSPI_STATUS_INVALID_PARAMETER;
    } else {
        if (command->address.bus_width == QSPI_CFG_BUS_SINGLE) {
            cfg.addrTransfer = qspiTransferSingle;
        } else if (command->address.bus_width == QSPI_CFG_BUS_DUAL) {
            cfg.addrTransfer = qspiTransferDual;
        } else if (command->address.bus_width == QSPI_CFG_BUS_QUAD) {
            cfg.addrTransfer = qspiTransferQuad;
        } else {
            return QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    if (command->data.bus_width == QSPI_CFG_BUS_SINGLE) {
        cfg.dataTransfer = qspiTransferSingle;
    } else if (command->data.bus_width == QSPI_CFG_BUS_DUAL) {
        cfg.dataTransfer = qspiTransferDual;
    } else if (command->data.bus_width == QSPI_CFG_BUS_QUAD) {
        cfg.dataTransfer = qspiTransferQuad;
    }

    QSPI_WriteConfig(obj->instance, &cfg);

    if (!command->alt.disabled) {
        // Do not support alt mode in write mode
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    // Do an indirect write
    obj->instance->INDAHBADDRTRIGGER = QSPI0_MEM_BASE;
    obj->instance->INDIRECTWRITEXFERSTART = command->address.value;
    obj->instance->INDIRECTWRITEXFERNUMBYTES = to_write;
    obj->instance->INDIRECTWRITEXFERCTRL = QSPI_INDIRECTWRITEXFERCTRL_START;

    // For the size of the transfer, poll the SRAM and fetch words from the SRAM
    for (uint32_t i = 0; i < to_write; i+=4) {
        // Wait for the QSPI in case we're writing too fast
        while (((obj->instance->SRAMFILL & _QSPI_SRAMFILL_SRAMFILLINDACWRITE_MASK) >> _QSPI_SRAMFILL_SRAMFILLINDACWRITE_SHIFT) >= 126);

        // Unaligned access is fine on CM3/CM4 provided we stick to LDR/STR
        // With the line below, the compiler can't really do anything else anyways
        *((uint32_t*)QSPI0_MEM_BASE) = ((uint32_t*)data)[i/4];
    }

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    QSPI_StigCmd_TypeDef cfg;

    if (tx_size > 8 || rx_size > 8) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    cfg.writeDataSize = tx_size;
    cfg.writeBuffer = (void*)tx_data;

    cfg.readDataSize = rx_size;
    cfg.readBuffer = rx_data;

    if (command->address.disabled) {
        cfg.addrSize = 0;
        cfg.address = 0;
    } else {
        if (command->address.size == QSPI_CFG_ADDR_SIZE_8) {
            cfg.addrSize = 1;
        } else if (command->address.size == QSPI_CFG_ADDR_SIZE_16) {
            cfg.addrSize = 2;
        } else if (command->address.size == QSPI_CFG_ADDR_SIZE_24) {
            cfg.addrSize = 3;
        } else if (command->address.size == QSPI_CFG_ADDR_SIZE_32) {
            cfg.addrSize = 4;
        } else {
            return QSPI_STATUS_INVALID_PARAMETER;
        }
        cfg.address = command->address.value;
    }

    if (command->instruction.disabled) {
        return QSPI_STATUS_INVALID_PARAMETER;
    } else {
        cfg.cmdOpcode = command->instruction.value;
    }

    cfg.dummyCycles = command->dummy_count;

    if (!command->alt.disabled) {
        cfg.modeBitEnable = true;
        obj->instance->MODEBITCONFIG = command->alt.value & _QSPI_MODEBITCONFIG_MODE_MASK;

        if(command->alt.size != SUPPORTED_ALT_SIZE) {
            //do not support 'alt' bigger than 8 bit
            return QSPI_STATUS_INVALID_PARAMETER;
        }
    } else {
        cfg.modeBitEnable = false;
    }

    QSPI_ExecStigCmd(obj->instance, &cfg);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    QSPI_ReadConfig_TypeDef cfg = QSPI_READCONFIG_DEFAULT;
    uint32_t to_read = *length;

    // Enforce word-sized access
    if ((to_read & 0x3) != 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    cfg.dummyCycles = command->dummy_count;

    if (command->instruction.disabled) {
        cfg.opCode = 0x03;
        cfg.instTransfer = qspiTransferSingle;
    } else {
        cfg.opCode = command->instruction.value;
        if (command->instruction.bus_width == QSPI_CFG_BUS_SINGLE) {
            cfg.instTransfer = qspiTransferSingle;
        } else if (command->instruction.bus_width == QSPI_CFG_BUS_DUAL) {
            cfg.instTransfer = qspiTransferDual;
        } else if (command->instruction.bus_width == QSPI_CFG_BUS_QUAD) {
            cfg.instTransfer = qspiTransferQuad;
        } else {
            return QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    if (command->address.disabled) {
        return QSPI_STATUS_INVALID_PARAMETER;
    } else {
        if (command->address.bus_width == QSPI_CFG_BUS_SINGLE) {
            cfg.addrTransfer = qspiTransferSingle;
        } else if (command->address.bus_width == QSPI_CFG_BUS_DUAL) {
            cfg.addrTransfer = qspiTransferDual;
        } else if (command->address.bus_width == QSPI_CFG_BUS_QUAD) {
            cfg.addrTransfer = qspiTransferQuad;
        } else {
            return QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    if (command->data.bus_width == QSPI_CFG_BUS_SINGLE) {
        cfg.dataTransfer = qspiTransferSingle;
    } else if (command->data.bus_width == QSPI_CFG_BUS_DUAL) {
        cfg.dataTransfer = qspiTransferDual;
    } else if (command->data.bus_width == QSPI_CFG_BUS_QUAD) {
        cfg.dataTransfer = qspiTransferQuad;
    }

    QSPI_ReadConfig(obj->instance, &cfg);

    if (!command->alt.disabled) {
        // Need to set up alt mode manually, called 'mode bits' in EFM32GG11 refman
        obj->instance->DEVINSTRRDCONFIG |= QSPI_DEVINSTRRDCONFIG_MODEBITENABLE;
        obj->instance->MODEBITCONFIG = command->alt.value & _QSPI_MODEBITCONFIG_MODE_MASK;

        if(command->alt.size != SUPPORTED_ALT_SIZE) {
            // Do not support 'alt' bigger than 8 bit
            return QSPI_STATUS_INVALID_PARAMETER;
        }
    }

    // Do an indirect read
    obj->instance->INDAHBADDRTRIGGER = QSPI0_MEM_BASE;
    obj->instance->INDIRECTREADXFERSTART = command->address.value;
    obj->instance->INDIRECTREADXFERNUMBYTES = to_read;
    obj->instance->INDIRECTREADXFERCTRL = QSPI_INDIRECTREADXFERCTRL_START;

    // For the size of the transfer, poll the SRAM and fetch words from the SRAM
    for (uint32_t i = 0; i < to_read; i+=4) {
        // Wait for the FIFO in case we're reading too fast
        while ((obj->instance->SRAMFILL & _QSPI_SRAMFILL_SRAMFILLINDACREAD_MASK) >> _QSPI_SRAMFILL_SRAMFILLINDACREAD_SHIFT == 0);

        // Unaligned access is fine on CM3/CM4 provided we stick to LDR/STR
        // With the line below, the compiler can't really do anything else anyways
        ((uint32_t*)data)[i/4] = *((uint32_t*)QSPI0_MEM_BASE);
    }

    return QSPI_STATUS_OK;
}

const PinMap *qspi_master_sclk_pinmap()
{
    return PinMap_QSPI_SCLK;
}

const PinMap *qspi_master_ssel_pinmap()
{
    return PinMap_QSPI_CS0;
}

const PinMap *qspi_master_data0_pinmap()
{
    return PinMap_QSPI_DQ0;
}

const PinMap *qspi_master_data1_pinmap()
{
    return PinMap_QSPI_DQ1;
}

const PinMap *qspi_master_data2_pinmap()
{
    return PinMap_QSPI_DQ2;
}

const PinMap *qspi_master_data3_pinmap()
{
    return PinMap_QSPI_DQ3;
}

#endif /* DEVICE_QSPI && QSPI_PRESENT */
