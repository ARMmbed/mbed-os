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

#include "AT45DBBlockDevice.h"

#include <inttypes.h>

/* constants */
#define AT45DB_READ_SIZE        1
#define AT45DB_PROG_SIZE        1
#define AT45DB_TIMEOUT          10000
#define AT45DB_ID_MATCH         0x1F20
#define AT45DB_ID_DENSITY_MASK  0x001F
#define AT45DB_PAGE_SIZE_256    0x0100
#define AT45DB_PAGE_SIZE_264    0x0108
#define AT45DB_PAGE_SIZE_512    0x0200
#define AT45DB_PAGE_SIZE_528    0x0210
#define AT45DB_BLOCK_SIZE_2K    0x0800
#define AT45DB_BLOCK_SIZE_2K1   0x0840
#define AT45DB_BLOCK_SIZE_4K    0x1000
#define AT45DB_BLOCK_SIZE_4K1   0x1080
#define AT45DB_PAGE_BIT_264     9
#define AT45DB_PAGE_BIT_528     10

/* enable debug */
#define AT45DB_DEBUG 0

#if AT45DB_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

void _print_status(uint16_t status);

/* non-exhaustive opcode list */
enum opcode {
    AT45DB_OP_NOP                       = 0x00,
    AT45DB_OP_STATUS                    = 0xD7,
    AT45DB_OP_ID                        = 0x9F,
    AT45DB_OP_READ_LOW_POWER            = 0x01,
    AT45DB_OP_READ_LOW_FREQUENCY        = 0x03,
    AT45DB_OP_PROGRAM_DIRECT            = 0x02, // Program through Buffer 1 without Built-In Erase
    AT45DB_OP_PROGRAM_DIRECT_WITH_ERASE = 0x82,
    AT45DB_OP_ERASE_BLOCK               = 0x50,
};

/* non-exhaustive command list */
enum command {
    AT45DB_COMMAND_WRITE_DISABLE        = 0x3D2A7F9A,
    AT45DB_COMMAND_WRITE_ENABLE         = 0x3D2A7FA9,
    AT45DB_COMMAND_BINARY_PAGE_SIZE     = 0x3D2A80A6,
    AT45DB_COMMAND_DATAFLASH_PAGE_SIZE  = 0x3D2A80A7,
};

/* bit masks for interpreting the status register */
enum status_bit {
    AT45DB_BIT_READY                = (0x01 << 15),
    AT45DB_BIT_COMPARE              = (0x01 << 14),
    AT45DB_BIT_DENSITY              = (0x0F << 10),
    AT45DB_BIT_PROTECT              = (0x01 <<  9),
    AT45DB_BIT_PAGE_SIZE            = (0x01 <<  8),

    AT45DB_BIT_ERASE_PROGRAM_ERROR  = (0x01 <<  5),
    AT45DB_BIT_SECTOR_LOCKDOWN      = (0x01 <<  3),
    AT45DB_BIT_PROGRAM_SUSPEND_2    = (0x01 <<  2),
    AT45DB_BIT_PROGRAM_SUSPEND_1    = (0x01 <<  1),
    AT45DB_BIT_ERASE_SUSPEND        = (0x01 <<  0),
};

/* bit masks for detecting density from status register */
enum status_density {
    AT45DB_STATUS_DENSITY_2_MBIT    = (0x05 << 10),
    AT45DB_STATUS_DENSITY_4_MBIT    = (0x07 << 10),
    AT45DB_STATUS_DENSITY_8_MBIT    = (0x09 << 10),
    AT45DB_STATUS_DENSITY_16_MBIT   = (0x0B << 10),
    AT45DB_STATUS_DENSITY_32_MBIT   = (0x0D << 10),
    AT45DB_STATUS_DENSITY_64_MBIT   = (0x0F << 10),
};

/* code for calculating density */
enum id_density {
    AT45DB_ID_DENSITY_2_MBIT    = 0x03,
    AT45DB_ID_DENSITY_4_MBIT    = 0x04,
    AT45DB_ID_DENSITY_8_MBIT    = 0x05,
    AT45DB_ID_DENSITY_16_MBIT   = 0x06,
    AT45DB_ID_DENSITY_32_MBIT   = 0x07,
    AT45DB_ID_DENSITY_64_MBIT   = 0x08,
};

/* typical duration in milliseconds for each operation */
enum timing {
    AT45DB_TIMING_ERASE_PROGRAM_PAGE   =    17,
    AT45DB_TIMING_PROGRAM_PAGE         =     3,
    AT45DB_TIMING_ERASE_PAGE           =    12,
    AT45DB_TIMING_ERASE_BLOCK          =    45,
    AT45DB_TIMING_ERASE_SECTOR         =   700,
    AT45DB_TIMING_ERASE_CHIP           = 45000
};

/* frequency domains */
enum frequency {
    AT45DB_LOW_POWER_FREQUENCY =  15000000,
    AT45DB_LOW_FREQUENCY       =  50000000,
    AT45DB_HIGH_FREQUENCY      =  85000000,
    AT45DB_HIGHEST_FREQUENCY   = 104000000
};

/* number of dummy bytes required in each frequency domain */
enum dummy {
    AT45DB_LOW_POWER_BYTES         = 0,
    AT45DB_LOW_FREQUENCY_BYTES     = 0,
    AT45DB_HIGH_FREQUENCY_BYTES    = 1,
    AT45DB_HIGHEST_FREQUENCY_BYTES = 2
};

AT45DBBlockDevice::AT45DBBlockDevice(PinName mosi,
                                     PinName miso,
                                     PinName sclk,
                                     PinName cs,
                                     int freq,
                                     PinName nwp)
    :   _spi(mosi, miso, sclk),
        _cs(cs, 1),
        _nwp(nwp),
        _device_size(0)
{
    /* check that frequency is within range */
    if (freq > AT45DB_LOW_FREQUENCY) {

        /* cap frequency at the highest supported one */
        _spi.frequency(AT45DB_LOW_FREQUENCY);

    } else {
        /* freqency is valid, use as-is */
        _spi.frequency(freq);
    }

    /* write protect chip if pin is connected */
    if (nwp != NC) {
        _nwp = 0;
    }
}

int AT45DBBlockDevice::init()
{
    DEBUG_PRINTF("init\r\n");

    int result = BD_ERROR_DEVICE_ERROR;

    /* read ID register to validate model and set dimensions */
    uint16_t id = _get_register(AT45DB_OP_ID);

    DEBUG_PRINTF("id: %04X\r\n", id & AT45DB_ID_MATCH);

    /* get status register to verify the page size mode */
    uint16_t status = _get_register(AT45DB_OP_STATUS);

    /* manufacture ID match */
    if ((id & AT45DB_ID_MATCH) == AT45DB_ID_MATCH) {

        /* calculate density */
        _device_size = 0x8000 << (id & AT45DB_ID_DENSITY_MASK);

        bool binary_page_size = true;

        /* check if device is configured for binary page sizes */
        if ((status & AT45DB_BIT_PAGE_SIZE) == AT45DB_BIT_PAGE_SIZE) {
            DEBUG_PRINTF("Page size is binary\r\n");

#if MBED_CONF_AT45DB_DATAFLASH_SIZE
            /* send reconfiguration command */
            _write_command(AT45DB_COMMAND_DATAFLASH_PAGE_SIZE, NULL, 0);

            /* wait for device to be ready and update return code */
            result = _sync();

            /* set binary flag */
            binary_page_size = false;
#else
            /* set binary flag */
            binary_page_size = true;
#endif
        } else {
            DEBUG_PRINTF("Page size is not binary\r\n");

#if MBED_CONF_AT45DB_BINARY_SIZE
            /* send reconfiguration command */
            _write_command(AT45DB_COMMAND_BINARY_PAGE_SIZE, NULL, 0);

            /* wait for device to be ready and update return code */
            result = _sync();

            /* set binary flag */
            binary_page_size = true;
#else
            /* set binary flag */
            binary_page_size = false;
#endif
        }

        /* set page program size and block erase size */
        switch (id & AT45DB_ID_DENSITY_MASK) {
            case AT45DB_ID_DENSITY_2_MBIT:
            case AT45DB_ID_DENSITY_4_MBIT:
            case AT45DB_ID_DENSITY_8_MBIT:
            case AT45DB_ID_DENSITY_64_MBIT:
                if (binary_page_size) {
                    _page_size = AT45DB_PAGE_SIZE_256;
                    _block_size = AT45DB_BLOCK_SIZE_2K;
                } else {
                    _page_size = AT45DB_PAGE_SIZE_264;
                    _block_size = AT45DB_BLOCK_SIZE_2K1;

                    /* adjust device size */
                    _device_size = (_device_size / AT45DB_PAGE_SIZE_256) *
                                    AT45DB_PAGE_SIZE_264;
                }
                break;
            case AT45DB_ID_DENSITY_16_MBIT:
            case AT45DB_ID_DENSITY_32_MBIT:
                if (binary_page_size) {
                    _page_size = AT45DB_PAGE_SIZE_512;
                    _block_size = AT45DB_BLOCK_SIZE_4K;
                } else {
                    _page_size = AT45DB_PAGE_SIZE_528;
                    _block_size = AT45DB_BLOCK_SIZE_4K1;

                    /* adjust device size */
                    _device_size = (_device_size / AT45DB_PAGE_SIZE_512) *
                                    AT45DB_PAGE_SIZE_528;
                }
                break;
            default:
                break;
        }

        DEBUG_PRINTF("density: %" PRIu16 "\r\n", id & AT45DB_ID_DENSITY_MASK);
        DEBUG_PRINTF("size: %" PRIu32 "\r\n", _device_size);
        DEBUG_PRINTF("page: %" PRIu16 "\r\n", _page_size);
        DEBUG_PRINTF("block: %" PRIu16 "\r\n", _block_size);

        /* device successfully detected, set OK error code */
        result = BD_ERROR_OK;
    }

    /* write protect device when idle */
    _write_enable(false);

    return result;
}

int AT45DBBlockDevice::deinit()
{
    DEBUG_PRINTF("deinit\r\n");

    return BD_ERROR_OK;
}

int AT45DBBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    DEBUG_PRINTF("read: %p %" PRIX64 " %" PRIX64 "\r\n", buffer, addr, size);

    int result = BD_ERROR_DEVICE_ERROR;

    /* check parameters are valid and the read is within bounds */
    if (is_valid_read(addr, size) && buffer) {

        uint8_t *external_buffer = static_cast<uint8_t *>(buffer);

        /* activate device */
        _cs = 0;

        /* send read opcode */
        _spi.write(AT45DB_OP_READ_LOW_FREQUENCY);

        /* translate address */
        uint32_t address = _translate_address(addr);

        DEBUG_PRINTF("address: %" PRIX32 "\r\n", address);

        /* send read address */
        _spi.write((address >> 16) & 0xFF);
        _spi.write((address >>  8) & 0xFF);
        _spi.write(address & 0xFF);

        /* clock out one byte at a time and store in external buffer */
        for (uint32_t index = 0; index < size; index++) {
            external_buffer[index] = _spi.write(AT45DB_OP_NOP);
        }

        /* deactivate device */
        _cs = 1;

        result = BD_ERROR_OK;
    }

    return result;
}

int AT45DBBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    DEBUG_PRINTF("program: %p %" PRIX64 " %" PRIX64 "\r\n", buffer, addr, size);

    int result = BD_ERROR_DEVICE_ERROR;

    /* check parameters are valid and the write is within bounds */
    if (is_valid_program(addr, size) && buffer) {

        const uint8_t * external_buffer = static_cast<const uint8_t*>(buffer);

        /* Each write command can only cover one page at a time.
           Find page and current page offset for handling unaligned writes.
         */
        uint32_t page_number = addr / _page_size;
        uint32_t page_offset = addr % _page_size;

        /* disable write protection */
        _write_enable(true);

        /* continue until all bytes have been written */
        uint32_t bytes_written = 0;
        while (bytes_written < size) {

            /* find remaining bytes to be written */
            uint32_t bytes_remaining = size - bytes_written;

            /* cap the value at the page size and offset */
            if (bytes_remaining > (_page_size - page_offset)) {
                bytes_remaining = _page_size - page_offset;
            }

            /* Write one page, bytes_written keeps track of the progress,
               page_number is the page address, and page_offset is non-zero for
               unaligned writes.
             */
            result = _write_page(&external_buffer[bytes_written],
                                 page_number,
                                 page_offset,
                                 bytes_remaining);

            /* update loop variables upon success otherwise break loop */
            if (result == BD_ERROR_OK) {
                bytes_written += bytes_remaining;
                page_number++;

                /* After the first successful write,
                   all subsequent writes will be aligned.
                 */
                page_offset = 0;
            } else {
                break;
            }
        }

        /* enable write protection */
        _write_enable(false);
    }

    return result;
}

int AT45DBBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    DEBUG_PRINTF("erase: %" PRIX64 " %" PRIX64 "\r\n", addr, size);

    int result = BD_ERROR_DEVICE_ERROR;

    /* check parameters are valid and the erase is within bounds */
    if (is_valid_erase(addr, size)) {

        /* disable write protection */
        _write_enable(true);

        /* erase one block at a time until the full size has been erased */
        uint32_t erased = 0;
        while (erased < size) {

            /* set block erase opcode */
            uint32_t command = AT45DB_OP_ERASE_BLOCK;

            /* translate address */
            uint32_t address = _translate_address(addr);

            /* set block address */
            command = (command << 8) | ((address >> 16) & 0xFF);
            command = (command << 8) | ((address >>  8) & 0xFF);
            command = (command << 8) | (address & 0xFF);

            /* send command to device */
            _write_command(command, NULL, 0);

            /* wait until device is ready and update return value */
            result = _sync();

            /* if erase failed, break loop */
            if (result != BD_ERROR_OK) {
                break;
            }

            /* update loop variables */
            addr += _block_size;
            erased -= _block_size;
        }

        /* enable write protection */
        _write_enable(false);
    }

    return result;
}

bd_size_t AT45DBBlockDevice::get_read_size() const
{
    DEBUG_PRINTF("read size: %d\r\n", AT45DB_READ_SIZE);

    return AT45DB_READ_SIZE;
}

bd_size_t AT45DBBlockDevice::get_program_size() const
{
    DEBUG_PRINTF("program size: %d\r\n", AT45DB_PROG_SIZE);

    return AT45DB_PROG_SIZE;
}

bd_size_t AT45DBBlockDevice::get_erase_size() const
{
    DEBUG_PRINTF("erase size: %" PRIX16 "\r\n", _block_size);

    return _block_size;
}

bd_size_t AT45DBBlockDevice::size() const
{
    DEBUG_PRINTF("device size: %" PRIX32 "\r\n", _device_size);

    return _device_size;
}

/**
 * @brief Function for reading a specific register.
 * @details Used for reading either the Status Register or Manufacture and ID Register.
 *
 * @param opcode Register to be read.
 * @return value.
 */
uint16_t AT45DBBlockDevice::_get_register(uint8_t opcode)
{
    DEBUG_PRINTF("_get_register: %" PRIX8 "\r\n", opcode);

    /* activate device */
    _cs = 0;

    /* write opcode */
    _spi.write(opcode);

    /* read and store result */
    int status = (_spi.write(AT45DB_OP_NOP));
    status = (status << 8) | (_spi.write(AT45DB_OP_NOP));

    /* deactivate device */
    _cs = 1;

    return status;
}

/**
 * @brief Function for sending command and data to device.
 * @details The command can be an opcode with address and data or
 *          a 4 byte command without data.
 *
 *          The supported frequencies and the opcode used do not
 *          require dummy bytes to be sent after command.
 *
 * @param command Opcode with address or 4 byte command.
 * @param buffer Data to be sent after command.
 * @param size Size of buffer.
 */
void AT45DBBlockDevice::_write_command(uint32_t command, const uint8_t *buffer, uint32_t size)
{
    DEBUG_PRINTF("_write_command: %" PRIX32 " %p %" PRIX32 "\r\n", command, buffer, size);

    /* activate device */
    _cs = 0;

    /* send command (opcode with data or 4 byte command) */
    _spi.write((command >> 24) & 0xFF);
    _spi.write((command >> 16) & 0xFF);
    _spi.write((command >>  8) & 0xFF);
    _spi.write(command & 0xFF);

    /* send optional data */
    if (buffer && size) {
        for (uint32_t index = 0; index < size; index++) {
            _spi.write(buffer[index]);
        }
    }

    /* deactivate device */
    _cs = 1;
}

/**
 * @brief Enable and disable write protection.
 *
 * @param enable Boolean for enabling or disabling write protection.
 */
void AT45DBBlockDevice::_write_enable(bool enable)
{
    DEBUG_PRINTF("_write_enable: %d\r\n", enable);

    /* enable writing, disable write protection */
    if (enable) {

        /* send 4 byte command enabling writes */
        _write_command(AT45DB_COMMAND_WRITE_ENABLE, NULL, 0);

        /* if not-write-protected pin is connected, deselect it */
        if (_nwp.is_connected()) {
            _nwp = 0;
        }

    } else {

        /* if not-write-protected pin is connected, select it */
        if (_nwp.is_connected()) {
            _nwp = 1;
        }

        /* send 4 byte command disabling writes */
        _write_command(AT45DB_COMMAND_WRITE_DISABLE, NULL, 0);
    }
}

/**
 * @brief Sleep and poll status register until device is ready for next command.
 *
 * @return BlockDevice compatible error code.
 */
int AT45DBBlockDevice::_sync(void)
{
    DEBUG_PRINTF("_sync\r\n");

    /* default return value if operation times out */
    int result = BD_ERROR_DEVICE_ERROR;

    /* Poll device until a hard coded timeout is reached.
       The polling interval is based on the typical page program time.
     */
    for (uint32_t timeout = 0;
         timeout < AT45DB_TIMEOUT;
         timeout += AT45DB_TIMING_ERASE_PROGRAM_PAGE) {

        /* get status register */
        uint16_t status = _get_register(AT45DB_OP_STATUS);

        /* erase/program bit set, exit with error code set */
        if (status & AT45DB_BIT_ERASE_PROGRAM_ERROR) {
            DEBUG_PRINTF("AT45DB_BIT_ERASE_PROGRAM_ERROR\r\n");
            break;
        /* device ready, set OK code set */
        } else if (status & AT45DB_BIT_READY) {
            DEBUG_PRINTF("AT45DB_BIT_READY\r\n");
            result = BD_ERROR_OK;
            break;
        /* wait the typical write period before trying again */
        } else {
            DEBUG_PRINTF("wait_ms: %d\r\n", AT45DB_TIMING_ERASE_PROGRAM_PAGE);
            wait_ms(AT45DB_TIMING_ERASE_PROGRAM_PAGE);
        }
    }

    return result;
}

/**
 * @brief Write single page.
 * @details Address can be unaligned.
 *
 * @param buffer Data to write.
 * @param addr Address to write from.
 * @param size Bytes to write. Can at most be the full page.
 * @return BlockDevice error code.
 */
int AT45DBBlockDevice::_write_page(const uint8_t *buffer,
                                   uint32_t page,
                                   uint32_t offset,
                                   uint32_t size)
{
    DEBUG_PRINTF("_write_page: %p %" PRIX32 " %" PRIX32 "\r\n", buffer, page, size);

    uint32_t command = AT45DB_OP_NOP;

    /* opcode for writing directly to device, in a single command,
       assuming the page has been erased before hand.
     */
    command = AT45DB_OP_PROGRAM_DIRECT;

    uint32_t address = 0;

    /* convert page number and offset into device address based on address format */
    if (_page_size == AT45DB_PAGE_SIZE_264) {
        address = (page << AT45DB_PAGE_BIT_264) | offset;
    } else if (_page_size == AT45DB_PAGE_SIZE_528) {
        address = (page << AT45DB_PAGE_BIT_528) | offset;
    } else {
        address = (page * _page_size) | offset;
    }

    /* set write address */
    command = (command << 8) | ((address >> 16) & 0xFF);
    command = (command << 8) | ((address >>  8) & 0xFF);
    command = (command << 8) | (address & 0xFF);

    /* send write command with data */
    _write_command(command, buffer, size);

    /* wait until device is ready before continuing */
    int result = _sync();

    return result;
}

/**
 * @brief Translate address.
 * @details If the device is configured for non-binary page sizes,
 *          the address is translated from binary to non-binary form.
 *
 * @param addr Binary address.
 * @return Address in format expected by device.
 */
uint32_t AT45DBBlockDevice::_translate_address(bd_addr_t addr)
{
    uint32_t address = addr;

    /* translate address if page size is non-binary */
    if (_page_size == AT45DB_PAGE_SIZE_264) {
        address = ((addr / AT45DB_PAGE_SIZE_264) << AT45DB_PAGE_BIT_264) |
                  (addr % AT45DB_PAGE_SIZE_264);
    } else if (_page_size == AT45DB_PAGE_SIZE_528) {
        address = ((addr / AT45DB_PAGE_SIZE_528) << AT45DB_PAGE_BIT_528) |
                  (addr % AT45DB_PAGE_SIZE_528);
    }

    return address;
}

/**
 * @brief Internal function for printing out each bit set in status register.
 *
 * @param status Status register.
 */
void _print_status(uint16_t status)
{
#if AT45DB_DEBUG
    DEBUG_PRINTF("%04X\r\n", status);

    /* device is ready (after write/erase) */
    if (status & AT45DB_BIT_READY) {
        DEBUG_PRINTF("AT45DB_BIT_READY\r\n");
    }

    /* Buffer comparison failed */
    if (status & AT45DB_BIT_COMPARE) {
        DEBUG_PRINTF("AT45DB_BIT_COMPARE\r\n");
    }

    /* device size is 2 MB */
    if (status & AT45DB_STATUS_DENSITY_2_MBIT) {
        DEBUG_PRINTF("AT45DB_STATUS_DENSITY_2_MBIT\r\n");
    }

    /* device size is 4 MB */
    if (status & AT45DB_STATUS_DENSITY_4_MBIT) {
        DEBUG_PRINTF("AT45DB_STATUS_DENSITY_4_MBIT\r\n");
    }

    /* device size is 8 MB */
    if (status & AT45DB_STATUS_DENSITY_8_MBIT) {
        DEBUG_PRINTF("AT45DB_STATUS_DENSITY_8_MBIT\r\n");
    }

    /* device size is 16 MB */
    if (status & AT45DB_STATUS_DENSITY_16_MBIT) {
        DEBUG_PRINTF("AT45DB_STATUS_DENSITY_16_MBIT\r\n");
    }

    /* device size is 32 MB */
    if (status & AT45DB_STATUS_DENSITY_32_MBIT) {
        DEBUG_PRINTF("AT45DB_STATUS_DENSITY_32_MBIT\r\n");
    }

    /* device size is 64 MB */
    if (status & AT45DB_STATUS_DENSITY_64_MBIT) {
        DEBUG_PRINTF("AT45DB_STATUS_DENSITY_64_MBIT\r\n");
    }

    /* sector protectino enabled */
    if (status & AT45DB_BIT_PROTECT) {
        DEBUG_PRINTF("AT45DB_BIT_PROTECT\r\n");
    }

    /* page size is a power of 2 */
    if (status & AT45DB_BIT_PAGE_SIZE) {
        DEBUG_PRINTF("AT45DB_BIT_PAGE_SIZE\r\n");
    }

    /* erase/program error */
    if (status & AT45DB_BIT_ERASE_PROGRAM_ERROR) {
        DEBUG_PRINTF("AT45DB_BIT_ERASE_PROGRAM_ERROR\r\n");
    }

    /* sector lockdown still possible */
    if (status & AT45DB_BIT_SECTOR_LOCKDOWN) {
        DEBUG_PRINTF("AT45DB_BIT_SECTOR_LOCKDOWN\r\n");
    }

    /* program operation suspended while using buffer 2 */
    if (status & AT45DB_BIT_PROGRAM_SUSPEND_2) {
        DEBUG_PRINTF("AT45DB_BIT_PROGRAM_SUSPEND_2\r\n");
    }

    /* program operation suspended while using buffer 1 */
    if (status & AT45DB_BIT_PROGRAM_SUSPEND_1) {
        DEBUG_PRINTF("AT45DB_BIT_PROGRAM_SUSPEND_1\r\n");
    }

    /* erase has been suspended */
    if (status & AT45DB_BIT_ERASE_SUSPEND) {
        DEBUG_PRINTF("AT45DB_BIT_ERASE_SUSPEND\r\n");
    }
#endif
}
