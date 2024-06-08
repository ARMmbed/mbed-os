/* mbed Microcontroller Library
 * Copyright (c) 2022, Arm Limited and affiliates.
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

#include "blockdevice/PowerManagementBlockDevice.h"
#include "platform/mbed_error.h"

#if COMPONENT_QSPIF
#include "drivers/QSPI.h"
#define QSPI_CMD_WREN    0x06
#define QSPI_CMD_RDCR0   0x15
#define QSPI_CMD_WRSR    0x01
#define QSPI_CMD_RDSR    0x05
#define QSPI_CMD_NOP     0x00
#define QSPI_CMD_DP      0xB9
#define QSPI_LH_BIT_MASK   0x02

mbed::QSPI _qspif(MBED_CONF_QSPIF_QSPI_IO0,
                  MBED_CONF_QSPIF_QSPI_IO1,
                  MBED_CONF_QSPIF_QSPI_IO2,
                  MBED_CONF_QSPIF_QSPI_IO3,
                  MBED_CONF_QSPIF_QSPI_SCK,
                  MBED_CONF_QSPIF_QSPI_CSN,
                  MBED_CONF_QSPIF_QSPI_POLARITY_MODE);
#endif

namespace mbed {

PowerManagementBlockDevice::PowerManagementBlockDevice(BlockDevice *bd)
    : _bd(bd)
{
    MBED_ASSERT(_bd);
}

PowerManagementBlockDevice::~PowerManagementBlockDevice()
{
    deinit();
}

int PowerManagementBlockDevice::init()
{
    return _bd->init();
}

int PowerManagementBlockDevice::deinit()
{
    return _bd->deinit();
}

int PowerManagementBlockDevice::sync()
{
    return _bd->sync();
}

int PowerManagementBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    return _bd->read(buffer, addr, size);
}

int PowerManagementBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    return _bd->program(buffer, addr, size);
}

int PowerManagementBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return _bd->erase(addr, size);
}

bd_size_t PowerManagementBlockDevice::get_read_size() const
{
    return _bd->get_read_size();
}

bd_size_t PowerManagementBlockDevice::get_program_size() const
{
    return _bd->get_program_size();
}

bd_size_t PowerManagementBlockDevice::get_erase_size() const
{
    return _bd->get_erase_size();
}

bd_size_t PowerManagementBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return _bd->get_erase_size(addr);
}

int PowerManagementBlockDevice::get_erase_value() const
{
    return _bd->get_erase_value();
}

bd_size_t PowerManagementBlockDevice::size() const
{
    return _bd->size();
}

const char *PowerManagementBlockDevice::get_type() const
{
    if (_bd != NULL) {
        return _bd->get_type();
    }

    return NULL;
}

int PowerManagementBlockDevice::switch_power_management_mode(int pm_mode)
{
#if COMPONENT_QSPIF
    qspi_status_t status = QSPI_STATUS_OK ;
    uint8_t wren_inst = QSPI_CMD_WREN;
    uint8_t sr_reg[3] = {0};
    uint8_t rdcr_inst = QSPI_CMD_RDCR0, wrsr_inst = QSPI_CMD_WRSR, rdsr_inst = QSPI_CMD_RDSR;
    uint8_t dp_inst = 0;

    if (QSPI_STATUS_OK != _qspif.configure_format(QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE, QSPI_CFG_ADDR_SIZE_24, QSPI_CFG_BUS_SINGLE,
                                                  0, QSPI_CFG_BUS_SINGLE, 0)) {
        return -1;
    }

    if (QSPI_STATUS_OK != _qspif.command_transfer(wren_inst, -1, NULL, 0, NULL, 0)) {
        return -1;
    }

    switch (pm_mode) {
        case QSPIF_HIGH_PERFORMANCE_MODE :
            if (QSPI_STATUS_OK != _qspif.command_transfer(rdsr_inst, -1, NULL, 0, (const char *)&sr_reg[0], 1)) {
                return -1;
            }

            if (QSPI_STATUS_OK != _qspif.command_transfer(rdcr_inst, -1, NULL, 0, (const char *)&sr_reg[1], 2)) {
                return -1;
            }

            sr_reg[2] = QSPI_LH_BIT_MASK;

            if (QSPI_STATUS_OK != _qspif.command_transfer(wrsr_inst, -1, (const char *)&sr_reg[0], 3, NULL, 0)) {
                return -1;
            }
            break;
        case QSPIF_LOW_POWER_MODE :
            if (QSPI_STATUS_OK != _qspif.command_transfer(rdsr_inst, -1, NULL, 0, (const char *)&sr_reg[0], 1)) {
                return -1;
            }

            if (QSPI_STATUS_OK != _qspif.command_transfer(rdcr_inst, -1, NULL, 0, (const char *)&sr_reg[1], 2)) {
                return -1;
            }

            sr_reg[2] = 0;

            if (QSPI_STATUS_OK != _qspif.command_transfer(wrsr_inst, -1, (const char *)&sr_reg[0], 3, NULL, 0)) {
                return -1;
            }
            break;
        case QSPIF_DEEP_DOWN_MODE :
            dp_inst = QSPI_CMD_DP;
            if (QSPI_STATUS_OK != _qspif.command_transfer(dp_inst, -1, NULL, 0, NULL, 0)) {
                return -1;
            }

            for (int i, k = 0; i < 10000; i++) {
                k++;
            }
            break;
        case QSPIF_STANDBY_MODE :
            dp_inst = QSPI_CMD_NOP;
            if (QSPI_STATUS_OK != _qspif.command_transfer(dp_inst, -1, NULL, 0, NULL, 0)) {
                return -1;
            }

            for (int i, k = 0; i < 10000; i++) {
                k++;
            }
            break;
        default :
            break;
    }

    return 0;
#endif
}

} // namespace mbed

/** @}*/
