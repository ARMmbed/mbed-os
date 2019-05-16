/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#if DEVICE_FLASH

#include "FlashIAPBlockDevice.h"
#include "mbed_atomic.h"
#include "mbed_error.h"

using namespace mbed;
#include <inttypes.h>

#define FLASHIAP_READ_SIZE 1

// Debug available
#ifndef FLASHIAP_DEBUG
#define FLASHIAP_DEBUG      0
#endif

#if FLASHIAP_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

FlashIAPBlockDevice::FlashIAPBlockDevice(uint32_t address, uint32_t size)
    : _flash(), _base(address), _size(size), _is_initialized(false), _init_ref_count(0)
{
    if ((address == 0xFFFFFFFF) || (size == 0)) {
        MBED_ERROR(MBED_ERROR_INVALID_ARGUMENT,
                   "Base address and size need to be set in flashiap-block-device configuration in order to use default constructor");
    }
}

FlashIAPBlockDevice::~FlashIAPBlockDevice()
{
    deinit();
}

int FlashIAPBlockDevice::init()
{
    DEBUG_PRINTF("init\r\n");

    if (!_is_initialized) {
        _init_ref_count = 0;
    }

    uint32_t val = core_util_atomic_incr_u32(&_init_ref_count, 1);

    if (val != 1) {
        return BD_ERROR_OK;
    }

    int ret = _flash.init();

    if (ret) {
        core_util_atomic_decr_u32(&_init_ref_count, 1);
        return ret;
    }

    if (_size + _base > _flash.get_flash_size() + _flash.get_flash_start()) {
        core_util_atomic_decr_u32(&_init_ref_count, 1);
        return BD_ERROR_DEVICE_ERROR;
    }

    if (_base < _flash.get_flash_start()) {
        core_util_atomic_decr_u32(&_init_ref_count, 1);
        return BD_ERROR_DEVICE_ERROR;
    }

    if (!_base) {
        _base = _flash.get_flash_start();
    }

    if (!_size) {
        _size = _flash.get_flash_size() - (_base - _flash.get_flash_start());
    }

    _is_initialized = true;
    return ret;
}

int FlashIAPBlockDevice::deinit()
{
    DEBUG_PRINTF("deinit\r\n");

    if (!_is_initialized) {
        _init_ref_count = 0;
        return 0;
    }

    uint32_t val = core_util_atomic_decr_u32(&_init_ref_count, 1);

    if (val) {
        return 0;
    }

    _is_initialized = false;

    return _flash.deinit();
}

int FlashIAPBlockDevice::read(void *buffer,
                              bd_addr_t virtual_address,
                              bd_size_t size)
{
    DEBUG_PRINTF("read: %" PRIX64 " %" PRIX64 "\r\n", virtual_address, size);

    /* Default to error return code; success must be set explicitly. */
    int result = BD_ERROR_DEVICE_ERROR;

    /* Check that the address and size are properly aligned and fit. */
    if (_is_initialized && is_valid_read(virtual_address, size)) {

        /* Convert virtual address to the physical address for the device. */
        bd_addr_t physical_address = _base + virtual_address;

        /* Read data using the internal flash driver. */
        result = _flash.read(buffer, physical_address, size);

        DEBUG_PRINTF("physical: %" PRIX64 "\r\n", physical_address);
    }

    return result;
}

int FlashIAPBlockDevice::program(const void *buffer,
                                 bd_addr_t virtual_address,
                                 bd_size_t size)
{
    DEBUG_PRINTF("program: %" PRIX64 " %" PRIX64 "\r\n", virtual_address, size);

    /* Default to error return code; success must be set explicitly. */
    int result = BD_ERROR_DEVICE_ERROR;

    /* Check that the address and size are properly aligned and fit. */
    if (_is_initialized && is_valid_program(virtual_address, size)) {

        /* Convert virtual address to the physical address for the device. */
        bd_addr_t physical_address = _base + virtual_address;

        /* Write data using the internal flash driver. */
        result = _flash.program(buffer, physical_address, size);

        DEBUG_PRINTF("physical: %" PRIX64 " %" PRIX64 "\r\n",
                     physical_address,
                     size);
    }

    return result;
}

int FlashIAPBlockDevice::erase(bd_addr_t virtual_address,
                               bd_size_t size)
{
    DEBUG_PRINTF("erase: %" PRIX64 " %" PRIX64 "\r\n", virtual_address, size);

    /* Default to error return code; success must be set explicitly. */
    int result = BD_ERROR_DEVICE_ERROR;

    /* Check that the address and size are properly aligned and fit. */
    if (_is_initialized && is_valid_erase(virtual_address, size)) {

        /* Convert virtual address to the physical address for the device. */
        bd_addr_t physical_address = _base + virtual_address;

        /* Erase sector */
        result = _flash.erase(physical_address, size);
    }

    return result;
}

bd_size_t FlashIAPBlockDevice::get_read_size() const
{
    DEBUG_PRINTF("get_read_size: %d\r\n", FLASHIAP_READ_SIZE);

    return FLASHIAP_READ_SIZE;
}

bd_size_t FlashIAPBlockDevice::get_program_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    uint32_t page_size = _flash.get_page_size();

    DEBUG_PRINTF("get_program_size: %" PRIX32 "\r\n", page_size);

    return page_size;
}

bd_size_t FlashIAPBlockDevice::get_erase_size() const
{
    if (!_is_initialized) {
        return 0;
    }

    uint32_t erase_size = _flash.get_sector_size(_base);

    DEBUG_PRINTF("get_erase_size: %" PRIX32 "\r\n", erase_size);

    return erase_size;
}

bd_size_t FlashIAPBlockDevice::get_erase_size(bd_addr_t addr) const
{
    if (!_is_initialized) {
        return 0;
    }

    uint32_t erase_size = _flash.get_sector_size(_base + addr);

    DEBUG_PRINTF("get_erase_size: %" PRIX32 "\r\n", erase_size);

    return erase_size;
}

int FlashIAPBlockDevice::get_erase_value() const
{
    if (!_is_initialized) {
        return -1;
    }

    uint8_t erase_val = _flash.get_erase_value();

    DEBUG_PRINTF("get_erase_value: %" PRIX8 "\r\n", erase_val);

    return erase_val;
}


bd_size_t FlashIAPBlockDevice::size() const
{
    DEBUG_PRINTF("size: %" PRIX64 "\r\n", _size);

    return _size;
}

const char *FlashIAPBlockDevice::get_type() const
{
    return "FLASHIAP";
}

bool FlashIAPBlockDevice::is_valid_erase(bd_addr_t addr, bd_size_t size) const
{
    /* Calculate address alignment for the full flash */
    bd_addr_t base_addr = addr + (_base - _flash.get_flash_start());

    return (
               addr + size <= this->size() &&
               base_addr % get_erase_size(addr) == 0 &&
               (base_addr + size) % get_erase_size(addr + size - 1) == 0);
}
#endif /* DEVICE_FLASH */
