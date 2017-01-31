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

#include "MX25RBlockDevice.h"

// Read/write/erase sizes
#define MX25R_READ_SIZE     1
#define MX25R_PROGRAM_SIZE  256
#define MX25R_ERASE_SIZE1   4096
#define MX25R_ERASE_SIZE2   32768
#define MX25R_ERASE_SIZE3   65536

#define MX25R_TIMEOUT       10000

// MX25R RDSR definitions
enum mx25r_rdsr {
    MX25R_WEL = 0x2,
    MX25R_WIP = 0x1,
};

// MX25R Series Register Command Table. 
enum mx25r_cmd {
    MX25R_READ   = 0x03, // x1 Normal Read Data Byte 
    MX25R_FREAD  = 0x0b, // x1 Fast Read Data Byte
    MX25R_2READ  = 0xbb, // x2 2READ 
    MX25R_DREAD  = 0x3b, // x2 DREAD 
    MX25R_4READ  = 0xeb, // x4 4READ 
    MX25R_QREAD  = 0x6b, // x4 QREAD 
    MX25R_PP     = 0x02, // Page Program 
    MX25R_4PP    = 0x38, // x4 PP
    MX25R_SE     = 0x20, // 4KB Sector Erase 
    MX25R_32KBE  = 0x52, // 32KB Block Erase 
    MX25R_BE     = 0xd8, // 64KB Block Erase 
    MX25R_CE     = 0xc7, // Chip Erase 
    MX25R_RDSFDP = 0x5a, // Read SFDP 
    MX25R_WREN   = 0x06, // Write Enable 
    MX25R_WRDI   = 0x04, // Write Disable
    MX25R_RDSR   = 0x05, // Read Status Register 
    MX25R_RDCR   = 0x15, // Read Configuration Register 
    MX25R_WRSR   = 0x01, // Write Status Register
    MX25R_PESUS  = 0xb0, // Program/Erase Suspend 
    MX25R_PERES  = 0x30, // Program/Erase Resume
    MX25R_DP     = 0xb9, // Enter Deep Power Down 
    MX25R_SBL    = 0xc0, // Set Burst Length 
    MX25R_RDID   = 0x9f, // Read Manufacturer and JDEC Device ID 
    MX25R_REMS   = 0x90, // Read Electronic Manufacturer and Device ID
    MX25R_RES    = 0xab, // Read Electronic ID
    MX25R_ENSO   = 0xb1, // Enter Secure OTP
    MX25R_EXSO   = 0xc1, // Exit Secure OTP
    MX25R_RDSCUR = 0x2b, // Read Security Register
    MX25R_WRSCUR = 0x2f, // Write Security Register
    MX25R_NOP    = 0x00, // No Operation
    MX25R_RSTEN  = 0x66, // Reset Enable 
    MX25R_RST    = 0x99, // Reset 
    MX25R_RRE    = 0xff, // Release Read Enhanced Mode
};
 
 
MX25RBlockDevice::MX25RBlockDevice(PinName mosi, PinName miso, PinName sclk, PinName cs)
    : _spi(mosi, miso, sclk), _cs(cs), _size(0)
{
}

bd_error_t MX25RBlockDevice::init()
{
    bd_error_t err = _sync();
    if (err) {
        return err;
    }

    // Find size from device ID
    uint8_t id[3];

    _cs = 0;
    _spi.write(MX25R_RDID);
    for (uint32_t i = 0; i < 3; i++) {
        id[i] = _spi.write(0);
    }
    _cs = 1;

    // Count in bits is stored as power of 2 in BCD
    _size = 1ULL << ((id[1] >> 4)*10 + (0xf & id[0]) - 3);

    return 0;
}

bd_error_t MX25RBlockDevice::deinit()
{
    return 0;
}

bd_error_t MX25RBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the address and size fit onto the chip.
    if (!is_valid_read(addr, size)) {
        return BD_ERROR_PARAMETER;
    }

    _cs = 0;
    _spi.write(MX25R_FREAD);
    _spi.write((char)(0xff & (addr >> 16)));
    _spi.write((char)(0xff & (addr >> 8 )));
    _spi.write((char)(0xff & (addr >> 0 )));
    _spi.write(0);

    for (bd_size_t i = 0; i < size; i++) {
        static_cast<uint8_t*>(buffer)[i] = _spi.write(0);
    }
    _cs = 1;

    return 0;
}
 
bd_error_t MX25RBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Check the address and size fit onto the chip.
    if (!is_valid_program(addr, size)) {
        return BD_ERROR_PARAMETER;
    }

    for (bd_size_t page = 0; page < size; page += MX25R_PROGRAM_SIZE) {
        bd_error_t err = _wren();
        if (err) {
            return err;
        }

        // Write 256 byte pages
        _cs = 0;
        _spi.write(MX25R_PP);
        _spi.write((char)(0xff & ((addr+page) >> 16)));
        _spi.write((char)(0xff & ((addr+page) >> 8 )));
        _spi.write((char)(0xff & ((addr+page) >> 0 )));
        
        for (uint32_t i = 0; i < MX25R_PROGRAM_SIZE; i++) {
            _spi.write(static_cast<const uint8_t*>(buffer)[page+i]);
        }
        _cs = 1;

        err = _sync();
        if (err) {
            return err;
        }
    }

    return 0;
}

bd_error_t MX25RBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    // Check the address and size fit onto the chip.
    if (!is_valid_erase(addr, size)) {
        return BD_ERROR_PARAMETER;
    }

    while (size > 0) {
        bd_error_t err = _wren();
        if (err) {
            return err;
        }
    
        bd_size_t chunk;
        if (size >= MX25R_ERASE_SIZE3 && !(addr & (MX25R_ERASE_SIZE3 - 1))) {
            // Erase a 64k block
            _cs = 0;
            _spi.write(MX25R_BE);
            _spi.write((char)(0xff & (addr >> 16)));
            _spi.write((char)(0xff & (addr >> 8 )));
            _spi.write((char)(0xff & (addr >> 0 )));
            _cs = 1;

            chunk = MX25R_ERASE_SIZE3;
        } else if (size >= MX25R_ERASE_SIZE2 && !(addr & (MX25R_ERASE_SIZE2 - 1))) {
            // Erase a 32k block
            _cs = 0;
            _spi.write(MX25R_32KBE);
            _spi.write((char)(0xff & (addr >> 16)));
            _spi.write((char)(0xff & (addr >> 8 )));
            _spi.write((char)(0xff & (addr >> 0 )));
            _cs = 1;

            chunk = MX25R_ERASE_SIZE2;
        } else if (size >= MX25R_ERASE_SIZE1 && !(addr & (MX25R_ERASE_SIZE1 - 1))) {
            // Erase a 4k block
            _cs = 0;
            _spi.write(MX25R_SE);
            _spi.write((char)(0xff & (addr >> 16)));
            _spi.write((char)(0xff & (addr >> 8 )));
            _spi.write((char)(0xff & (addr >> 0 )));
            _cs = 1;

            chunk = MX25R_ERASE_SIZE1;
        } else {
            MBED_UNREACHABLE;
        }

        err = _sync();
        if (err) {
            return err;
        }

        addr += chunk;
        size -= chunk;
    }

    return 0;
}

bd_error_t MX25RBlockDevice::_wren()
{
    _cs = 0;
    _spi.write(MX25R_WREN);
    _cs = 1;

    for (int i = 0; i < MX25R_TIMEOUT; i++) {
        // Read status register until write not-in-progress
        _cs = 0;
        _spi.write(MX25R_RDSR);
        uint8_t status = _spi.write(0);
        _cs = 1;

        // Check WEL bit
        if (status & MX25R_WEL) {
            return 0;
        }

        wait_ms(1);
    }

    return BD_ERROR_DEVICE_ERROR;
}

bd_error_t MX25RBlockDevice::_sync()
{
    for (int i = 0; i < MX25R_TIMEOUT; i++) {
        // Read status register until write not-in-progress
        _cs = 0;
        _spi.write(MX25R_RDSR);
        uint8_t status = _spi.write(0);
        _cs = 1;

        // Check WIP bit
        if (!(status & MX25R_WIP)) {
            return 0;
        }

        wait_ms(1);
    }

    return BD_ERROR_DEVICE_ERROR;
}

bd_size_t MX25RBlockDevice::read_size()
{
    return MX25R_READ_SIZE;
}

bd_size_t MX25RBlockDevice::program_size()
{
    return MX25R_PROGRAM_SIZE;
}

bd_size_t MX25RBlockDevice::erase_size()
{
    return MX25R_ERASE_SIZE1;
}

bd_size_t MX25RBlockDevice::size()
{
    return _size;
}
