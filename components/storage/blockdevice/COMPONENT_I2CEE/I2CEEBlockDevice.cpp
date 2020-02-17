/* Simple access class for I2C EEPROM chips like Microchip 24LC
 * Copyright (c) 2015 Robin Hourahane
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
#include "I2CEEBlockDevice.h"
#include "rtos/ThisThread.h"
using namespace mbed;

#define I2CEE_TIMEOUT 10000


I2CEEBlockDevice::I2CEEBlockDevice(
    PinName sda, PinName scl, uint8_t addr,
    bd_size_t size, bd_size_t block, int freq,
    bool address_is_eight_bit)
    : _i2c_addr(addr), _size(size), _block(block),
      _address_is_eight_bit(address_is_eight_bit)
{
    _i2c = new (_i2c_buffer) I2C(sda, scl);
    _i2c->frequency(freq);
}

I2CEEBlockDevice::I2CEEBlockDevice(
    I2C *i2c_obj, uint8_t addr,
    bd_size_t size, bd_size_t block,
    bool address_is_eight_bit)
    : _i2c_addr(addr), _size(size), _block(block),
      _address_is_eight_bit(address_is_eight_bit)
{
    _i2c = i2c_obj;
}
I2CEEBlockDevice::~I2CEEBlockDevice()
{
    if (_i2c == (I2C *)_i2c_buffer) {
        _i2c->~I2C();
    }
}

int I2CEEBlockDevice::init()
{
    return _sync();
}

int I2CEEBlockDevice::deinit()
{
    return 0;
}

int I2CEEBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the address and size fit onto the chip.
    MBED_ASSERT(is_valid_read(addr, size));

    auto *charBuffer = reinterpret_cast<char *>(buffer);

    auto const handler = [&](const bd_addr_t &pagedStart, const bd_size_t &pagedLength,
                             const uint8_t &pagedDeviceAddress) -> int
    {
        _i2c->start();

        if (1 != _i2c->write(pagedDeviceAddress))
        {
            return BD_ERROR_DEVICE_ERROR;
        }

        if (!_address_is_eight_bit && 1 != _i2c->write((char) (pagedStart >> 8u)))
        {
            return BD_ERROR_DEVICE_ERROR;
        }

        if (1 != _i2c->write((char) (pagedStart & 0xffu)))
        {
            return BD_ERROR_DEVICE_ERROR;
        }

        _i2c->stop();

        auto err = _sync();
        if (err) {
            return err;
        }

        if (0 != _i2c->read(_i2c_addr, charBuffer, pagedLength)) {
            return BD_ERROR_DEVICE_ERROR;
        }

        charBuffer += size;

        return BD_ERROR_OK;
    };

    return do_paged(addr, size, handler);
}

int I2CEEBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the addr and size fit onto the chip.
    MBED_ASSERT(is_valid_program(addr, size));

    auto const *charBuffer = reinterpret_cast<char const *>(buffer);

    auto const handler = [&](const bd_addr_t &pagedStart, const bd_size_t &pagedLength,
                             const uint8_t &pagedDeviceAddress) -> int
    {
        // While we have some more data to write.
        while (size > 0)
        {
            uint32_t off = addr % _block;
            uint32_t chunk = (off + size < _block) ? size : (_block - off);

            _i2c->start();

            if (1 != _i2c->write(pagedDeviceAddress))
            {
                return BD_ERROR_DEVICE_ERROR;
            }

            if (!_address_is_eight_bit && 1 != _i2c->write((char) (pagedStart >> 8u)))
            {
                return BD_ERROR_DEVICE_ERROR;
            }

            if (1 != _i2c->write((char) (addr & 0xffu)))
            {
                return BD_ERROR_DEVICE_ERROR;
            }

            for (unsigned i = 0; i < chunk; i++)
            {
                if (1 != _i2c->write(charBuffer[i]))
                {
                    return BD_ERROR_DEVICE_ERROR;
                }
            }

            _i2c->stop();

            int err = _sync();

            if (err) {
                return err;
            }

            addr += chunk;
            size -= chunk;
            charBuffer += chunk;
        }

        return BD_ERROR_OK;
    };

    auto const originalSize = size;
    return do_paged(addr, originalSize, handler);
}

int I2CEEBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    // No erase needed
    return 0;
}

int I2CEEBlockDevice::_sync()
{
    // The chip doesn't ACK while writing to the actual EEPROM
    // so loop trying to do a zero byte write until it is ACKed
    // by the chip.
    for (int i = 0; i < I2CEE_TIMEOUT; i++) {
        if (_i2c->write(_i2c_addr | 0, 0, 0) < 1) {
            return 0;
        }

        rtos::ThisThread::sleep_for(1);
    }

    return BD_ERROR_DEVICE_ERROR;
}

bd_size_t I2CEEBlockDevice::get_read_size() const
{
    return 1;
}

bd_size_t I2CEEBlockDevice::get_program_size() const
{
    return 1;
}

bd_size_t I2CEEBlockDevice::get_erase_size() const
{
    return 1;
}

bd_size_t I2CEEBlockDevice::size() const
{
    return _size;
}

const char *I2CEEBlockDevice::get_type() const
{
    return "I2CEE";
}

int I2CEEBlockDevice::do_paged(const bd_addr_t &startAddress,
                               const bd_size_t &length,
                               const paged_handler &handler)
{
    // This helper is only used for eight bit mode.
    if (!this->_address_is_eight_bit) {
        return handler(startAddress, length, get_paged_device_address(0));
    }

    auto currentStartAddress = startAddress;

    auto const pageSize = 256;
    bd_size_t lengthDone = 0;
    while (lengthDone != length)
    {
        /* Integer division => Round down */
        uint8_t const currentPage = currentStartAddress / 256;
        bd_addr_t const nextPageBegin = (currentPage + 1) * pageSize;
        bd_addr_t const currentReadEndAddressExclusive = std::min(nextPageBegin, startAddress + length);
        bd_size_t const currentLength = currentReadEndAddressExclusive - currentStartAddress;
        bd_addr_t const pagedBegin = currentStartAddress - (currentPage * pageSize);
        uint8_t const pagedDeviceAddress = get_paged_device_address(currentPage);

        auto const handlerReturn = handler(pagedBegin, currentLength, pagedDeviceAddress);
        if (handlerReturn != BD_ERROR_OK)
        {
            return handlerReturn;
        }

        currentStartAddress = currentReadEndAddressExclusive;
        lengthDone += currentLength;
    }

    return BD_ERROR_OK;
}

uint8_t I2CEEBlockDevice::get_paged_device_address(const uint8_t &page)
{
    if (!this->_address_is_eight_bit)
    {
        return this->_i2c_addr;
    }
    else
    {
        // This method uses a dynamically created bit mask for the page given.
        // This ensures compatibility with all sizes of ICs.
        // E. g. the 512K variants have two user address bits and one page bit.
        // We don't want to forcefully override the two user address bits.

        // Create a mask to cover all bits required to set page
        // i starts at one because the LSB is used for R/W in I2C
        uint8_t i = 1;
        uint8_t addressMask = 0;
        auto p = page;
        while (p != 0u)
        {
            addressMask |= (1u << i);
            p >>= 1u;
            i++;
        }

        uint8_t pagedDeviceAddress = this->_i2c_addr & static_cast<uint8_t>(~addressMask);
        // Assert page < 0b111, because we don't have
        // more bits for page encoding
        // Don't actually write 0b111, this is a nonstandard extension.
        MBED_ASSERT(page < 0x7);
        pagedDeviceAddress |= static_cast<uint8_t>(page << 1u);

        return pagedDeviceAddress;
    }
}
