/***************************************************************************//**
* \file CyReservedRegionBlockDevice.cpp
*
* \brief
* Block device for working via an underlying block device without altering
* a reserved region.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "CyReservedRegionBlockDevice.h"

CyReservedRegionBlockDevice::CyReservedRegionBlockDevice(BlockDevice *underlying_bd, mbed::bd_size_t reserved_region_size)
{
    _underlying_bd = underlying_bd;
    _reserved_region_size = reserved_region_size;
    _reserved_region_end = 0;
}

int CyReservedRegionBlockDevice::init()
{
    int status = _underlying_bd->init();
    if (status == BD_ERROR_OK) {
        // Round up to start usable region on an erase boundary
        // May need to wait until after init() to determine erase size (e.g. QSPI)
        if (_reserved_region_size % get_erase_size() != 0) {
            _reserved_region_end = _reserved_region_size + get_erase_size() - (_reserved_region_size % get_erase_size());
        } else {
            _reserved_region_end = _reserved_region_size;
        }
    }
    return status;
}

int CyReservedRegionBlockDevice::deinit()
{
    int status = _underlying_bd->deinit();
    if (status == BD_ERROR_OK) {
        _reserved_region_end = 0;
    }
    return status;
}

int CyReservedRegionBlockDevice::read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size)
{
    return _underlying_bd->read(buffer, addr + _reserved_region_end, size);
}

int CyReservedRegionBlockDevice::program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size)
{
    return _underlying_bd->program(buffer, addr + _reserved_region_end, size);
}

int CyReservedRegionBlockDevice::erase(mbed::bd_addr_t addr, mbed::bd_size_t size)
{
    return _underlying_bd->erase(addr + _reserved_region_end, size);
}

mbed::bd_size_t CyReservedRegionBlockDevice::get_read_size() const
{
    return _underlying_bd->get_read_size();
}

mbed::bd_size_t CyReservedRegionBlockDevice::get_program_size() const
{
    return _underlying_bd->get_program_size();
}

mbed::bd_size_t CyReservedRegionBlockDevice::get_erase_size() const
{
    return _underlying_bd->get_erase_size();
}

mbed::bd_size_t CyReservedRegionBlockDevice::get_erase_size(mbed::bd_addr_t addr) const
{
    return _underlying_bd->get_erase_size(addr + _reserved_region_end);
}

int CyReservedRegionBlockDevice::get_erase_value() const
{
    return _underlying_bd->get_erase_value();
}

mbed::bd_size_t CyReservedRegionBlockDevice::size() const
{
    return _underlying_bd->size() - _reserved_region_end;
}

const char *CyReservedRegionBlockDevice::get_type() const
{
    return _underlying_bd->get_type();
}

int CyReservedRegionBlockDevice::reserved_read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size)
{
    if (addr + size > _reserved_region_end || addr % get_read_size() != 0 || size % get_read_size() != 0) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _underlying_bd->read(buffer, addr, size);
}

mbed::bd_addr_t CyReservedRegionBlockDevice::reserved_region_end() const
{
    return _reserved_region_end;
}
