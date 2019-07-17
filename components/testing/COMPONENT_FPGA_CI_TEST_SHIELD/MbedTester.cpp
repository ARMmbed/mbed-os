/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MbedTester.h"
#include "fpga_config.h"
#include "BlockDevice.h"
#include "rtos/ThisThread.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_error.h"
#include "drivers/MbedCRC.h"

#define mbed_tester_printf(...)

#define PHYSICAL_PINS           128
#define LOGICAL_PINS            8
#define FIRMWARE_SIZE           2192012
#define FIRMWARE_REGION_SIZE    0x220000
#define FIRMWARE_HEADER_SIZE    0x10000
#define FLASH_SECTOR_SIZE       0x1000
#define LENGTH_SIZE             0x4
#define CRC_SIZE                0x4
#define FLASH_SPI_FREQ_HZ       2000000
#define ANALOG_COUNT            4

#define PHYSICAL_NC    ((MbedTester::PhysicalIndex)0xFF)

static const uint8_t KEY[8] = {
    0x92, 0x9d, 0x9a, 0x9b,
    0x29, 0x35, 0xa2, 0x65
};

template<size_t width>
class MbedTesterBitMap {
public:

    MbedTesterBitMap()
    {
        for (size_t i = 0; i < _count; i++) {
            _bitmap[i] = 0;
        }
    }

    bool get(size_t index)
    {
        if (index >= width) {
            return false;
        }
        return _bitmap[index / 32] & (1 << (index % 32)) ? true : false;
    }

    void set(size_t index)
    {
        if (index >= width) {
            return;
        }
        _bitmap[index / 32] |= 1 << (index % 32);
    }

    void clear(size_t index)
    {
        if (index >= width) {
            return;
        }
        _bitmap[index / 32] &= ~(1 << (index % 32));
    }

private:

    static const size_t _count = (width + 31) / 32;
    uint32_t _bitmap[(width + 31) / 32];
};

static uint8_t spi_transfer(mbed::DigitalInOut *clk, mbed::DigitalInOut *mosi, mbed::DigitalInOut *miso, uint8_t data)
{
    uint8_t ret = 0;
    for (int i = 0; i < 8; i++) {
        *clk = 0;
        *mosi = (data >> (7 - i)) & 1;
        wait_ns(100);
        *clk = 1;
        ret |= *miso ? 1 << (7 - i) : 0;
        wait_ns(100);
    }
    return ret;
}

static void mbed_tester_command(mbed::DigitalInOut *clk, mbed::DigitalInOut *mosi, mbed::DigitalInOut *miso, uint8_t miso_index, uint32_t addr, bool write_n_read, uint8_t *data, uint8_t size)
{
    // 8 - Start Key
    for (uint32_t i = 0; i < sizeof(KEY); i++) {
        spi_transfer(clk, mosi, miso, KEY[i]);
    }

    // 1 - Physical pin index for MISO
    spi_transfer(clk, mosi, miso, miso_index);

    // 1 - Number of SPI transfers which follow (= N + 5)
    spi_transfer(clk, mosi, miso, size + 5);

    // 4 - Little endian address for transfer
    spi_transfer(clk, mosi, miso, (addr >> (8 * 0)) & 0xFF);
    spi_transfer(clk, mosi, miso, (addr >> (8 * 1)) & 0xFF);
    spi_transfer(clk, mosi, miso, (addr >> (8 * 2)) & 0xFF);
    spi_transfer(clk, mosi, miso, (addr >> (8 * 3)) & 0xFF);

    // 1 - direction
    spi_transfer(clk, mosi, miso, write_n_read ? 1 : 0);

    // N - Data to read or write
    if (write_n_read) {//read: false, write: true
        for (int i = 0; i < size; i++) {
            spi_transfer(clk, mosi, miso, data[i]);
        }
    } else {
        for (int i = 0; i < size; i++) {
            data[i] = spi_transfer(clk, mosi, miso, 0);
        }
    }
    *clk = 0;

}

static bool mbed_tester_test(mbed::DigitalInOut *clk, mbed::DigitalInOut *mosi, mbed::DigitalInOut *miso, uint8_t miso_index)
{
    uint8_t buf[4];
    memset(buf, 0, sizeof(buf));
    mbed_tester_command(clk, mosi, miso, miso_index, TESTER_CONTROL, false, buf, sizeof(buf));
    return memcmp(buf, "mbed", sizeof(buf)) == 0;
}


class MbedTesterBlockDevice : public BlockDevice {
public:

    MbedTesterBlockDevice(mbed::DigitalInOut &mosi, mbed::DigitalInOut &miso, mbed::DigitalInOut &clk, mbed::DigitalInOut &cs, uint32_t frequency)
        : _mosi(mosi), _miso(miso), _clk(clk), _cs(cs), _wait_ns(1000000000 / frequency / 2), _init(false)
    {

        // Set initial values
        _cs.write(1);
        _clk.write(0);

        // Set direction
        _mosi.output();
        _miso.input();
        _clk.output();
        _cs.output();
    }


    virtual int init()
    {
        if (_check_id()) {
            _init = true;
        }
        return _init ? BD_ERROR_OK : BD_ERROR_DEVICE_ERROR;
    }

    virtual int deinit()
    {
        _init = false;
        return BD_ERROR_OK;
    }

    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size)
    {
        if (!is_valid_read(addr, size) || !_init) {
            return BD_ERROR_DEVICE_ERROR;
        }

        _assert_cs(true);

        uint8_t cmd[] = {
            0x0B,                       // Fast read
            (uint8_t)(addr >> (2 * 8)), // Address
            (uint8_t)(addr >> (1 * 8)),
            (uint8_t)(addr >> (0 * 8)),
            0x00                        // Dummy
        };
        _write((char *)cmd, sizeof(cmd), NULL, 0);
        _write(NULL, 0, (char *)buffer, size);

        _assert_cs(false);

        return BD_ERROR_OK;
    }

    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size)
    {
        if (!is_valid_program(addr, size) || !_init) {
            return BD_ERROR_DEVICE_ERROR;
        }

        const bd_size_t max_program_size = 256;
        bd_size_t programmed = 0;
        while (programmed < size) {
            const bd_size_t size_left = size - programmed;
            const bd_size_t program_size = size_left < max_program_size ? size_left : max_program_size;

            _write_enable();
            _page_program(addr + programmed, (const uint8_t *)buffer, program_size);
            _wait_ready();
            programmed += program_size;
        }

        return BD_ERROR_OK;
    }

    virtual int erase(bd_addr_t addr, bd_size_t size)
    {
        if (!is_valid_erase(addr, size) || !_init) {
            return BD_ERROR_DEVICE_ERROR;
        }

        if ((addr == 0) && (size == FLASH_SECTOR_SIZE)) {
            // Allow 4K erase only on the first sector. The flash on the basys3 does
            // not allow sector erases at the higher addresses.
            _write_enable();
            _sector_erase(addr);
            _wait_ready();
            return BD_ERROR_OK;
        }

        if (!is_valid_erase(addr, size)) {
            return BD_ERROR_DEVICE_ERROR;
        }

        const uint32_t erase_size = get_erase_size();
        bd_size_t erased = 0;
        while (erased < erase_size) {
            _write_enable();
            _block_erase(addr + erased);
            _wait_ready();
            erased += erase_size;
        }
        return BD_ERROR_OK;
    }

    virtual bd_size_t get_read_size() const
    {
        return 1;
    }

    virtual bd_size_t get_program_size() const
    {
        return 1;
    }

    virtual bd_size_t get_erase_size() const
    {
        return 0x10000;
    }

    virtual bd_size_t get_erase_size(bd_addr_t addr) const
    {
        return get_erase_size();
    }

    virtual bd_size_t size() const
    {
        return 8 * 1024 * 1024;
    }

    virtual const char *get_type() const
    {
        return "MbedTesterBlockDevice";
    }

protected:

    void _write_enable()
    {
        uint8_t command[1];

        _assert_cs(true);

        command[0] = 0x06;
        _write((char *)command, 1, NULL, 0);

        _assert_cs(false);
    }

    void _sector_erase(uint32_t addr)
    {
        uint8_t command[4];

        _assert_cs(true);

        command[0] = 0x20;
        command[1] = (addr >> (2 * 8)) & 0xFF;
        command[2] = (addr >> (1 * 8)) & 0xFF;
        command[3] = (addr >> (0 * 8)) & 0xFF;
        _write((char *)command, 4, NULL, 0);

        _assert_cs(false);
    }

    void _block_erase(uint32_t addr)
    {
        uint8_t command[4];

        _assert_cs(true);

        command[0] = 0xD8;
        command[1] = (addr >> (2 * 8)) & 0xFF;
        command[2] = (addr >> (1 * 8)) & 0xFF;
        command[3] = (addr >> (0 * 8)) & 0xFF;
        _write((char *)command, 4, NULL, 0);

        _assert_cs(false);
    }

    void _page_program(uint32_t addr, const uint8_t *data, uint32_t size)
    {
        uint8_t command[4];

        _assert_cs(true);

        command[0] = 0x02;
        command[1] = (addr >> (2 * 8)) & 0xFF;
        command[2] = (addr >> (1 * 8)) & 0xFF;
        command[3] = (addr >> (0 * 8)) & 0xFF;
        _write((char *)command, 4, NULL, 0);
        _write((char *)data, size, NULL, 0);

        _assert_cs(false);
    }

    void _wait_ready()
    {
        uint8_t command[2];
        uint8_t response[2];

        // Wait for ready
        response[1] = 0xFF;
        do {
            _assert_cs(true);

            command[0] = 0x05;
            command[1] = 0;
            _write((char *)command, 2, (char *)response, 2);

            _assert_cs(false);

        } while (response[1] & (1 << 0));
    }

    bool _check_id()
    {
        uint8_t command[1];
        char id0[3];
        char id1[3];

        // Read ID twice and verify it is the same

        _assert_cs(true);

        command[0] = 0x9F;
        _write((char *)command, 1, NULL, 0);
        _write(NULL, 0, id0, sizeof(id0));

        _assert_cs(false);

        _assert_cs(true);

        command[0] = 0x9F;
        _write((char *)command, 1, NULL, 0);
        _write(NULL, 0, id1, sizeof(id1));

        _assert_cs(false);

        // Return failure if IDs are not the same
        for (size_t i = 0; i < sizeof(id0); i++) {
            if (id0[i] != id1[i]) {
                return false;
            }
        }

        // If all 0xFF return failure
        if ((id0[0] == 0xFF) && (id0[1] == 0xFF) && (id0[2] == 0xFF)) {
            return false;
        }

        // If all 0x00 return failure
        if ((id0[0] == 0x00) && (id0[1] == 0x00) && (id0[2] == 0x00)) {
            return false;
        }

        return true;
    }

    void _write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length)
    {
        int transfers = 0;
        if (tx_length > transfers) {
            transfers = tx_length;
        }
        if (rx_length > transfers) {
            transfers = rx_length;
        }

        for (int i = 0; i < transfers; i++) {
            uint8_t out = i < tx_length ? tx_buffer[i] : 0;
            uint8_t in = 0;
            for (int j = 0; j < 8; j++) {
                _mosi.write((out >> 7) & 1);
                out = out << 1;
                wait_ns(_wait_ns);

                _clk.write(1);
                in = (in << 1) | (_miso.read() ? 1 : 0);
                wait_ns(_wait_ns);

                _clk.write(0);
            }
            if (i < rx_length) {
                rx_buffer[i] = in;
            }
        }
    }

    void _assert_cs(bool asserted)
    {
        _clk = 0;
        wait_ns(_wait_ns);
        _cs = asserted ? 0 : 1;
        wait_ns(_wait_ns);
    }

    mbed::DigitalInOut &_mosi;
    mbed::DigitalInOut &_miso;
    mbed::DigitalInOut &_clk;
    mbed::DigitalInOut &_cs;
    uint32_t _wait_ns;
    bool _init;
};

static void dummy_progress(uint8_t)
{
    // Stub progress handler for firmware update/dump
}

// Header taken from app note XAPP1081. Information on the commands
// can be found in the 7 Series FPGA configuration user guide - UG470
static const uint8_t BANK_B_SELECT[] = {
    0x20, 0x00, 0x00, 0x00,     // 0x20000000   NOP
    0x30, 0x02, 0x00, 0x01,     // 0x30020001   WRITE to WBSTAR (Warm boot start address register)
    0x00, 0x23, 0x00, 0x00,     // 0x00230000       0x230000 = Second bank start address
    0x30, 0x00, 0x80, 0x01,     // 0x30008001   WRITE to CMD register
    0x00, 0x00, 0x00, 0x0F,     // 0x0000000F       0x0F = IPROG command (starts warm boot)
    0x20, 0x00, 0x00, 0x00,     // 0x20000000   NOP
    0x20, 0x00, 0x00, 0x00,     // 0x20000000   NOP
    0x20, 0x00, 0x00, 0x00      // 0x20000000   NOP
};

static const uint8_t SYNC_WORD[] = {
    0xAA, 0x99, 0x55, 0x66      // 0xAA995566   Sync word
};

static bool _firmware_header_valid(BlockDevice &flash, bool &valid)
{
    uint8_t buf[64];
    size_t pos = 0;
    size_t read_size;

    // Default to invalid
    valid = false;

    // Check that first portion is erased
    while (pos < FLASH_SECTOR_SIZE - sizeof(SYNC_WORD)) {
        read_size = FLASH_SECTOR_SIZE - pos;
        if (read_size > sizeof(buf)) {
            read_size = sizeof(buf);
        }
        if (flash.read(buf, pos, read_size) != BD_ERROR_OK) {
            return false;
        }
        pos += read_size;
        for (size_t i = 0; i < read_size; i++) {
            if (buf[i] != 0xFF) {
                valid = false;
                return true;
            }
        }
    }

    // Skip the sync word
    pos += sizeof(SYNC_WORD);

    // Check that BANK_B_SELECT is valid
    read_size = sizeof(BANK_B_SELECT);
    if (flash.read(buf, pos, read_size) != BD_ERROR_OK) {
        return false;
    }
    pos += read_size;
    if (memcmp(buf, BANK_B_SELECT, sizeof(BANK_B_SELECT)) != 0) {
        valid = false;
        return true;
    }

    // Check if the rest is 0xFF
    while (pos < FIRMWARE_HEADER_SIZE) {
        read_size = FIRMWARE_HEADER_SIZE - pos;
        if (read_size > sizeof(buf)) {
            read_size = sizeof(buf);
        }
        if (flash.read(buf, pos, read_size) != BD_ERROR_OK) {
            return false;
        }
        pos += read_size;
        for (size_t i = 0; i < read_size; i++) {
            if (buf[i] != 0xFF) {
                valid = false;
                return true;
            }
        }
    }

    valid = true;
    return true;
}

static bool _firmware_get_active_bank(BlockDevice &flash, bool &second_bank_active)
{
    uint8_t buf[sizeof(SYNC_WORD)];

    if (flash.read(buf, FLASH_SECTOR_SIZE - sizeof(SYNC_WORD), sizeof(SYNC_WORD)) != BD_ERROR_OK) {
        return false;
    }

    second_bank_active = memcmp(buf, SYNC_WORD, sizeof(SYNC_WORD)) == 0 ? true : false;
    return true;
}

static bool _firmware_set_active_bank(BlockDevice &flash, bool second_bank)
{
    bool valid = false;
    if (!_firmware_header_valid(flash, valid)) {
        return false;
    }
    if (!valid) {
        if (flash.erase(0, FIRMWARE_HEADER_SIZE) != BD_ERROR_OK) {
            return false;
        }
        if (flash.program(BANK_B_SELECT, FLASH_SECTOR_SIZE, sizeof(BANK_B_SELECT)) != BD_ERROR_OK) {
            return false;
        }
    }
    if (!flash.erase(0, FLASH_SECTOR_SIZE)) {
        return false;
    }


    if (second_bank) {
        // Write the sync word. Before the sync word is written the FPGA will boot from the first bank.
        // After the sync word is written the FPGA will boot from the second bank.
        if (flash.program(SYNC_WORD, FLASH_SECTOR_SIZE - sizeof(SYNC_WORD), sizeof(SYNC_WORD)) != BD_ERROR_OK) {
            return false;
        }
    }

    return true;
}

MbedTester::MbedTester(const PinList *form_factor, const PinList *exclude_pins)
    : _form_factor(form_factor), _exclude_pins(exclude_pins), _control_auto(true), _control_valid(false),
      _clk_index(PHYSICAL_NC), _mosi_index(PHYSICAL_NC), _miso_index(PHYSICAL_NC), _aux_index(PHYSICAL_NC),
      _clk(NULL), _mosi(NULL), _miso(NULL), _aux(NULL)
{
    _reset();
    _init_io_exp_rst_flag = 0;
}

MbedTester::~MbedTester()
{
    _free_control_pins();
}


void MbedTester::set_control_pins_auto()
{
    _control_auto = true;
}

void MbedTester::set_control_pins_manual(PinName clk, PinName mosi, PinName miso, PinName aux)
{
    int index;
    index = _form_factor.index(clk);
    if (index < 0) {
        error("Invalid CLK index");
    }
    PhysicalIndex clk_index = index;

    index = _form_factor.index(mosi);
    if (index < 0) {
        error("Invalid MOSI index");
    }
    PhysicalIndex mosi_index = index;

    index = _form_factor.index(miso);
    if (index < 0) {
        error("Invalid MISO index");
    }
    PhysicalIndex miso_index = index;

    index = _form_factor.index(aux);
    if (index < 0) {
        error("Invalid AUX index");
    }
    PhysicalIndex aux_index = index;

    if (clk_index + 1 != mosi_index) {
        error("MOSI pin index does not follow CLK as required");
    }

    if ((miso_index == clk_index) || (miso_index == mosi_index)) {
        error("MISO conflicts with a control channel");
    }
    if ((aux_index == clk_index) || (aux_index == mosi_index) || (aux_index == miso_index)) {
        error("AUX conflicts with a control channel");
    }

    // All criteria have been met so set the pins
    _control_auto = false;
    _free_control_pins();
    _clk_index = clk_index;
    _mosi_index = mosi_index;
    _miso_index = miso_index;
    _aux_index = aux_index;
    _setup_control_pins();
    _control_valid = true;
}

bool MbedTester::firmware_dump(mbed::FileHandle *dest, mbed::Callback<void(uint8_t)> progress)
{
    _update_control_pins();

    if (!progress) {
        progress = mbed::callback(dummy_progress);
    }

    // Mapping intentionally different from control channel to prevent
    // unintentional activation (clk and mosi flipped)
    MbedTesterBlockDevice flash(*_clk, *_miso, *_mosi, *_aux, FLASH_SPI_FREQ_HZ);
    sys_pin_mode_spi_serial_flash(_clk_index, _miso_index, _mosi_index, _aux_index);

    progress(0);

    if (flash.init() != BD_ERROR_OK) {
        sys_pin_mode_disabled();
        return false;
    }

    // Set the start of dump to the active bank
    bool second_bank_active;
    if (!_firmware_get_active_bank(flash, second_bank_active)) {
        // Error determining active bank
        sys_pin_mode_disabled();
        return false;
    }
    const uint32_t start = FIRMWARE_HEADER_SIZE + (second_bank_active ? FIRMWARE_REGION_SIZE : 0);

    // Get the firmware size
    uint32_t offset = 0;
    uint8_t buf[256];
    uint32_t prev_percent_done = 0;
    if (flash.read(buf, start + offset, LENGTH_SIZE) != BD_ERROR_OK) {
        sys_pin_mode_disabled();
        return false;
    }
    if (dest->write(buf, LENGTH_SIZE) != LENGTH_SIZE) {
        sys_pin_mode_disabled();
        return false;
    }
    offset += LENGTH_SIZE;
    uint32_t data_size = (buf[0] << (0 * 8)) |
                         (buf[1] << (1 * 8)) |
                         (buf[2] << (2 * 8)) |
                         (buf[3] << (3 * 8));
    if (data_size > FIRMWARE_REGION_SIZE - LENGTH_SIZE - CRC_SIZE) {
        data_size = FIRMWARE_REGION_SIZE - LENGTH_SIZE - CRC_SIZE;
    }
    const uint32_t firmware_size = data_size + LENGTH_SIZE + CRC_SIZE;

    // Dump firmware
    while (offset < firmware_size) {
        uint32_t read_size = firmware_size - offset;
        if (read_size > sizeof(buf)) {
            read_size = sizeof(buf);
        }
        if (flash.read(buf, start + offset, read_size) != BD_ERROR_OK) {
            sys_pin_mode_disabled();
            return false;
        }
        ssize_t write_size = dest->write(buf, read_size);
        if ((uint32_t)write_size != read_size) {
            sys_pin_mode_disabled();
            return false;
        }
        offset += read_size;

        const uint8_t percent_done = (offset * 100) / firmware_size;
        if (percent_done != prev_percent_done) {
            progress(percent_done);
            prev_percent_done = percent_done;
        }
    }

    progress(100);

    sys_pin_mode_disabled();
    return true;
}

bool MbedTester::firmware_dump_all(mbed::FileHandle *dest, mbed::Callback<void(uint8_t)> progress)
{
    _update_control_pins();

    if (!progress) {
        progress = mbed::callback(dummy_progress);
    }

    // Mapping intentionally different from control channel to prevent
    // unintentional activation (clk and mosi flipped)
    MbedTesterBlockDevice flash(*_clk, *_miso, *_mosi, *_aux, FLASH_SPI_FREQ_HZ);
    sys_pin_mode_spi_serial_flash(_clk_index, _miso_index, _mosi_index, _aux_index);

    progress(0);

    if (flash.init() != BD_ERROR_OK) {
        sys_pin_mode_disabled();
        return false;
    }

    uint32_t pos = 0;
    uint8_t buf[256];
    uint32_t prev_percent_done = 0;
    const uint32_t total_size = flash.size();
    while (pos < total_size) {
        uint32_t read_size = total_size - pos;
        if (read_size > sizeof(buf)) {
            read_size = sizeof(buf);
        }
        if (flash.read(buf, pos, read_size) != BD_ERROR_OK) {
            sys_pin_mode_disabled();
            return false;
        }
        ssize_t write_size = dest->write(buf, read_size);
        if ((uint32_t)write_size != read_size) {
            sys_pin_mode_disabled();
            return false;
        }
        pos += read_size;

        const uint8_t percent_done = (pos * 100) / total_size;
        if (percent_done != prev_percent_done) {
            progress(percent_done);
            prev_percent_done = percent_done;
        }
    }

    progress(100);

    sys_pin_mode_disabled();
    return true;
}

bool MbedTester::firmware_update(mbed::FileHandle *src, mbed::Callback<void(uint8_t)> progress)
{
    _update_control_pins();

    if (!progress) {
        progress = mbed::callback(dummy_progress);
    }

    // Mapping intentionally different from control channel to prevent
    // unintentional activation (clk and mosi flipped)
    MbedTesterBlockDevice flash(*_clk, *_miso, *_mosi, *_aux, FLASH_SPI_FREQ_HZ);
    sys_pin_mode_spi_serial_flash(_clk_index, _miso_index, _mosi_index, _aux_index);

    progress(0);

    if (flash.init() != BD_ERROR_OK) {
        sys_pin_mode_disabled();
        return false;
    }

    // Validate file size
    const uint32_t file_size = src->size();
    if (file_size > FIRMWARE_REGION_SIZE) {
        // Firmware image too big
        sys_pin_mode_disabled();
        return false;
    }
    if (file_size < LENGTH_SIZE + CRC_SIZE) {
        // Firmware image too small
        sys_pin_mode_disabled();
        return false;
    }

    // Set the start of programming to the inactive bank
    bool second_bank_active;
    if (!_firmware_get_active_bank(flash, second_bank_active)) {
        // Error determining active bank
        sys_pin_mode_disabled();
        return false;
    }
    const uint32_t start = FIRMWARE_HEADER_SIZE + (second_bank_active ? 0 : FIRMWARE_REGION_SIZE);

    // Setup CRC calculation
    uint32_t crc;
    mbed::MbedCRC<POLY_32BIT_ANSI, 32> ct;
    if (ct.compute_partial_start(&crc) != 0) {
        sys_pin_mode_disabled();
        return false;
    }

    uint8_t buf[256];
    const bd_size_t erase_size = flash.get_erase_size();
    uint32_t offset = 0;
    uint32_t prev_percent_done = 0;
    uint32_t stored_crc = 0;
    bool size_valid = false;
    while (offset < file_size) {

        // Prepare data
        uint32_t program_size = file_size - offset;
        if (program_size > sizeof(buf)) {
            program_size = sizeof(buf);
        }
        ssize_t read_size = src->read(buf, program_size);
        if (read_size < 0) {
            sys_pin_mode_disabled();
            return false;
        } else if (read_size == 0) {
            break;
        }
        program_size = read_size;

        // Record values and calculate checksum
        uint32_t crc_offset = 0;
        uint32_t crc_size = program_size;
        if (offset == 0) {
            // Overlap with the size field

            // Check that the data length is correct
            const size_t data_size = (buf[0] << (0 * 8)) |
                                     (buf[1] << (1 * 8)) |
                                     (buf[2] << (2 * 8)) |
                                     (buf[3] << (3 * 8));
            if (data_size != file_size - LENGTH_SIZE - CRC_SIZE) {
                // Invalid data length
                sys_pin_mode_disabled();
                return false;
            }
            size_valid = true;

            // Don't include the length in the checksum
            crc_offset += LENGTH_SIZE;
            crc_size -= LENGTH_SIZE;
        }
        if (offset + program_size > file_size - CRC_SIZE) {
            // Overlap with the CRC field
            for (uint32_t i = 0; i < CRC_SIZE; i++) {
                uint32_t byte_offset = file_size - CRC_SIZE + i;
                if ((byte_offset >= offset) && (byte_offset < offset + program_size)) {
                    uint32_t buf_pos = byte_offset - offset;
                    stored_crc |= buf[buf_pos] << (i * 8);

                    // Don't include the stored CRC in the CRC
                    crc_size--;
                }
            }
        }
        if (ct.compute_partial(buf + crc_offset, crc_size, &crc) != 0) {
            sys_pin_mode_disabled();
            return false;
        }

        // Write data to file
        const uint32_t addr = start + offset;
        if (addr % erase_size == 0) {
            if (flash.erase(addr, erase_size) != BD_ERROR_OK) {
                sys_pin_mode_disabled();
                return false;
            }
        }
        if (flash.program(buf, addr, read_size) != BD_ERROR_OK) {
            sys_pin_mode_disabled();
            return false;
        }

        offset += program_size;

        const uint8_t percent_done = (offset * 100) / file_size;
        if (percent_done != prev_percent_done) {
            progress(percent_done);
            prev_percent_done = percent_done;
        }
    }

    // Check that everything was good and if so switch active bank
    if (!size_valid) {
        sys_pin_mode_disabled();
        return false;
    }
    if (ct.compute_partial_stop(&crc) != 0) {
        sys_pin_mode_disabled();
        return false;
    }
    if (crc != stored_crc) {
        sys_pin_mode_disabled();
        return false;
    }
    if (!_firmware_set_active_bank(flash, !second_bank_active)) {
        sys_pin_mode_disabled();
        return false;
    }

    progress(100);

    sys_pin_mode_disabled();
    return true;
}

void MbedTester::pin_map_set(PinName physical, LogicalPin logical)
{
    int index = _form_factor.index(physical);
    if (index < 0) {
        error("Pin %i not in form factor", physical);
        return;
    }
    if (logical >= LogicalPinTotal) {
        error("Invalid logical pin %i", logical);
        return;
    }
    pin_map_index(index, logical);
}

void MbedTester::pin_map_reset()
{
    for (uint32_t i = 0; i < sizeof(_mapping) / sizeof(_mapping[0]); i++) {
        _mapping[i] = PHYSICAL_NC;
    }

    uint8_t pin_buf[PHYSICAL_PINS + LOGICAL_PINS];
    memset(pin_buf, 0xFF, sizeof(pin_buf));
    write(TESTER_REMAP, pin_buf, sizeof(pin_buf));
}

void MbedTester::peripherals_reset()
{
    uint8_t buf = TESTER_CONTROL_RESET_PERIPHERALS;
    write(TESTER_CONTROL_RESET, &buf, sizeof(buf));
}

void MbedTester::reset()
{
    // Reset pullup settings
    pin_pull_reset_all();

    // Reset the FPGA
    uint8_t buf = TESTER_CONTROL_RESET_ALL;
    write(TESTER_CONTROL_RESET, &buf, sizeof(buf));

    // Reset the pinmap
    // NOTE - this is only needed for compatibility with
    //        older firmware which resets the mapping
    //        of all pins to 0x00 rather than 0xFF.
    pin_map_reset();

    // Reset internal state variables
    _reset();
}

void MbedTester::reprogram()
{
    // Trigger reprogramming
    uint8_t buf = TESTER_CONTROL_REPROGRAM;
    write(TESTER_CONTROL_RESET, &buf, sizeof(buf));

    // Reset internal state variables
    _reset();
}

uint32_t MbedTester::version()
{
    uint32_t software_version;

    read(TESTER_CONTROL_VERSION, (uint8_t *)&software_version, sizeof(software_version));

    return software_version;
}

void MbedTester::select_peripheral(Peripheral peripheral)
{
    uint8_t data = peripheral;
    write(TESTER_PERIPHERAL_SELECT, &data, sizeof(data));
}

void MbedTester::pin_pull_reset_all()
{
    _init_io_exp_rst_flag = 1;
    sys_pin_write(I2CReset, 0, true);
    wait_us(1);
    sys_pin_write(I2CReset, 0, false);
}

int MbedTester::pin_set_pull(PinName pin, PullMode mode)
{
    int index = _form_factor.index(pin);
    if ((index < 0) || (index > 127)) {
        error("Pin %i not in form factor", pin);
        return -1;
    }

    return pin_set_pull_index(index, mode);
}

int MbedTester::pin_set_pull_index(int index, PullMode mode)
{
    // Reset IO expanders once after Mbed reset if user attempts
    // to read/write them without explicitly reseting them
    if (_init_io_exp_rst_flag == 0) {
        pin_pull_reset_all();
    }
    uint8_t chip_num;//can be 0-5
    uint16_t dev_addr;//can be 0x44 or 0x46
    uint8_t port_num;//can be 0-23
    uint8_t output_port_reg;//can be 4, 5, or 6
    uint8_t config_reg;//can be 12, 13, or 14
    uint8_t reg_bit;//can be 0-7
    uint8_t cmd0[2];//for writing configuration register
    uint8_t cmd1[2];//for writing output port register
    uint8_t i2c_index;//can be 0, 1, or 2 for TESTER_SYS_IO_MODE_I2C_IO_EXPANDER0/1/2

    chip_num = index / 24;
    if ((chip_num == 0) || (chip_num == 1)) {
        i2c_index = 0;
    } else if ((chip_num == 2) || (chip_num == 3)) {
        i2c_index = 1;
    } else if ((chip_num == 4) || (chip_num == 5)) {
        i2c_index = 2;
    } else {
        error("Corrupt index %i, should be 0-127\r\n", index);
        return -1;
    }
    dev_addr = (chip_num % 2) ? 0x44 : 0x46;
    port_num = index % 24;
    output_port_reg = 4 + (port_num / 8);
    config_reg = 12 + (port_num / 8);
    reg_bit = port_num % 8;

    uint8_t read_config_byte[1];
    uint8_t read_output_byte[1];
    if (io_expander_i2c_read(i2c_index, dev_addr, config_reg, read_config_byte, 1) != 0) {
        return -1;
    }
    if (io_expander_i2c_read(i2c_index, dev_addr, output_port_reg, read_output_byte, 1) != 0) {
        return -1;
    }
    cmd0[0] = config_reg;
    if ((mode == PullDown) || (mode == PullUp)) {
        cmd0[1] = read_config_byte[0] & ~(1 << reg_bit);
        cmd1[0] = output_port_reg;
        if (mode == PullDown) {
            cmd1[1] = read_output_byte[0] & ~(1 << reg_bit);
        } else if (mode == PullUp) {
            cmd1[1] = read_output_byte[0] | (1 << reg_bit);
        }
    } else if (mode == PullNone) {
        cmd0[1] = read_config_byte[0] | (1 << reg_bit);
    }

    //write configuration register for all 3 modes
    if (io_expander_i2c_write(i2c_index, dev_addr, cmd0, 2) != 0) {
        return -1;
    }
    //only write output register for pulldown and pullup
    if ((mode == PullDown) || (mode == PullUp)) {
        if (io_expander_i2c_write(i2c_index, dev_addr, cmd1, 2) != 0) {
            return -1;
        }
    }
    return 0;
}

uint8_t MbedTester::io_expander_read(PinName pin, IOExpanderReg reg_type)
{
    int index = _form_factor.index(pin);

    return io_expander_read_index(index, reg_type);
}

uint8_t MbedTester::io_expander_read_index(int index, IOExpanderReg reg_type)
{
    // Reset IO expanders once after Mbed reset if user attempts
    // to read/write them without explicitly reseting them
    if (_init_io_exp_rst_flag == 0) {
        pin_pull_reset_all();
    }
    uint8_t read_byte[1] = {0};
    uint8_t chip_num;//can be 0-5
    uint16_t dev_addr;//can be 0x44 or 0x46
    uint8_t port_num;//can be 0-23
    uint8_t input_port_reg;//can be 0, 1, or 2
    uint8_t output_port_reg;//can be 4, 5, or 6
    uint8_t config_reg;//can be 12, 13, or 14
    uint8_t reg_bit;//can be 0-7
    uint8_t i2c_index;

    chip_num = index / 24;
    if ((chip_num == 0) || (chip_num == 1)) {
        i2c_index = 0;
    } else if ((chip_num == 2) || (chip_num == 3)) {
        i2c_index = 1;
    } else if ((chip_num == 4) || (chip_num == 5)) {
        i2c_index = 2;
    } else {
        i2c_index = 0xFF;
        error("Invalid pin index, index should be in the range of 0-127");
    }
    dev_addr = (chip_num % 2) ? 0x44 : 0x46;
    port_num = index % 24;
    input_port_reg = (port_num / 8);
    output_port_reg = 4 + (port_num / 8);
    config_reg = 12 + (port_num / 8);
    reg_bit = port_num % 8;
    uint8_t reg;
    if (reg_type == RegInput) {
        reg = input_port_reg;
    } else if (reg_type == RegOutput) {
        reg = output_port_reg;
    } else if (reg_type == RegConfig) {
        reg = config_reg;
    } else {
        reg = 0xFF;
        error("Invalid register type, should be: INPUT, OUTPUT, or RegConfig");
    }

    int read_success = io_expander_i2c_read(i2c_index, dev_addr, reg, read_byte, 1);
    MBED_ASSERT(read_success == 0);
    uint8_t bit = (read_byte[0] & (1 << reg_bit)) >> reg_bit;
    return bit;
}

int MbedTester::io_expander_i2c_read(uint8_t i2c_index, uint8_t dev_addr, uint8_t start_reg, uint8_t *data, int length)
{
    _update_control_pins();
    //sda_in = _miso_index
    //sda_val = _aux_index
    //scl_in = _mosi_index (PHYSICAL_NC)
    //scl_val = _clk_index
    mbed::DigitalInOut *sda_in = _miso;
    mbed::DigitalInOut *sda_val = _aux;
    mbed::DigitalInOut *scl_val = _clk;
    sda_in->input();
    sda_val->output();
    *sda_val = 1;
    scl_val->output();
    sys_pin_mode_i2c_io_expander(i2c_index, _miso_index, _aux_index, PHYSICAL_NC, _clk_index);

    //start condition
    *scl_val = 1;
    wait_ns(2500);
    *sda_val = 0;
    wait_ns(2500);

    // begin writing data, dev_addr first
    uint8_t send_bit;
    for (int j = 0; j < 2; j += 1) {
        *scl_val = 0;
        *sda_val = 0;
        wait_ns(2500);
        for (int i = 7; i > -1; i -= 1) {
            if (j == 0) {
                send_bit = (dev_addr & (1 << i)) >> i;
            } else {
                send_bit = (start_reg & (1 << i)) >> i;
            }
            *sda_val = send_bit;
            wait_ns(500);

            *scl_val = 1;
            wait_ns(2500);
            *scl_val = 0;
            wait_ns(1000);
            *sda_val = 0;
            wait_ns(1000);
        }
        // receive ACK from IO extender
        *sda_val = 1;//make sda high z to receive ACK
        //clk the ACK
        *scl_val = 1;
        //read sda to check for ACK or NACK
        if (*sda_in) {
            return -1;//NACK - write failed
        }
        wait_ns(2500);
        *scl_val = 0;
        wait_ns(2500);
    }

    //start condition
    *sda_val = 1;
    *scl_val = 1;
    wait_ns(2500);
    *sda_val = 0;
    wait_ns(2500);

    // begin reading data, write (dev_addr | 1) first
    dev_addr |= 1;
    for (int j = -1; j < length; j += 1) {
        uint8_t read_byte = 0;
        for (int i = 7; i > -1; i -= 1) {
            if (j == -1) {
                *scl_val = 0;
                *sda_val = 0;
                send_bit = (dev_addr & (1 << i)) >> i;
                *sda_val = send_bit;
                wait_ns(500);

                *scl_val = 1;
                wait_ns(2500);
                *scl_val = 0;
                wait_ns(1000);
                *sda_val = 0;
                wait_ns(1000);
            } else {
                *scl_val = 1;
                read_byte |= (*sda_in << i);
                wait_ns(2500);
                *scl_val = 0;
                wait_ns(2500);
            }
        }
        if (j > -1) {
            data[j] = read_byte;
        }
        if (j == -1) {
            // receive ACK from IO extender
            *sda_val = 1;//make sda high z to receive ACK
            //clk the ACK
            *scl_val = 1;
            //read sda to check for ACK or NACK
            if (*sda_in) {
                return -1;//NACK - write failed
            }
            wait_ns(2500);
            *scl_val = 0;
            wait_ns(2500);
        } else {
            if (j == (length - 1)) { //NACK to signal end of read
                *sda_val = 1;
                wait_ns(1000);
                *scl_val = 1;
                wait_ns(2500);
                *scl_val = 0;
                wait_ns(1500);
            } else {//ACK to signal read will continue
                *sda_val = 0;
                wait_ns(1000);
                *scl_val = 1;
                wait_ns(2500);
                *scl_val = 0;
                wait_ns(500);
                *sda_val = 1;
                wait_ns(1000);
            }
        }
    }

    //stop condition
    *sda_val = 0;
    wait_ns(2500);
    *scl_val = 1;
    wait_ns(2500);
    *sda_val = 1;
    wait_ns(2500);

    sys_pin_mode_disabled();

    return 0;
}

int MbedTester::io_expander_i2c_write(uint8_t i2c_index, uint8_t dev_addr, uint8_t *data, int length)
{
    _update_control_pins();
    //sda_in = _miso_index
    //sda_val = _aux_index
    //scl_in = _mosi_index (PHYSICAL_NC)
    //scl_val = _clk_index
    mbed::DigitalInOut *sda_in = _miso;
    mbed::DigitalInOut *sda_val = _aux;
    mbed::DigitalInOut *scl_val = _clk;
    sda_in->input();
    sda_val->output();
    *sda_val = 1;
    scl_val->output();
    sys_pin_mode_i2c_io_expander(i2c_index, _miso_index, _aux_index, PHYSICAL_NC, _clk_index);

    //start condition
    *scl_val = 1;
    wait_ns(2500);
    *sda_val = 0;
    wait_ns(2500);

    // begin writing data, dev_addr first
    uint8_t send_bit;
    for (int j = -1; j < length; j += 1) {
        *scl_val = 0;
        *sda_val = 0;
        for (int i = 7; i > -1; i -= 1) {
            if (j == -1) {
                send_bit = (dev_addr & (1 << i)) >> i;
            } else {
                send_bit = (data[j] & (1 << i)) >> i;
            }

            *sda_val = send_bit;
            wait_ns(500);

            *scl_val = 1;
            wait_ns(2500);
            *scl_val = 0;
            wait_ns(1000);
            *sda_val = 0;
            wait_ns(1000);
        }
        // receive ACK from IO extender
        *sda_val = 1;//make sda high z to receive ACK
        //clk the ACK
        *scl_val = 1;
        //read sda to check for ACK or NACK
        if (*sda_in) {
            return -1;//NACK - write failed
        }
        wait_ns(2500);
        *scl_val = 0;
        wait_ns(2500);
    }

    //stop condition
    *sda_val = 0;
    wait_ns(2500);
    *scl_val = 1;
    wait_ns(2500);
    *sda_val = 1;
    wait_ns(2500);

    sys_pin_mode_disabled();

    return 0;
}

int MbedTester::pin_set_pull_bb(PinName pin, PullMode mode)
{
    int index = _form_factor.index(pin);
    if ((index < 0) || (index > 127)) {
        error("Pin %i not in form factor", pin);
        return -1;
    }
    uint8_t chip_num;//can be 0-5
    SystemPin sda;//can be I2CSda0, I2CSda1, or I2CSda2
    SystemPin scl;//can be I2CScl0, I2CScl1, or I2CScl2
    uint16_t dev_addr;//can be 0x44 or 0x46
    uint8_t port_num;//can be 0-23
    uint8_t output_port_reg;//can be 4, 5, or 6
    uint8_t config_reg;//can be 12, 13, or 14
    uint8_t reg_bit;//can be 0-7
    uint8_t cmd0[2];//for writing configuration register
    uint8_t cmd1[2];//for writing output port register

    chip_num = index / 24;
    if ((chip_num == 0) || (chip_num == 1)) {
        sda = I2CSda0;
        scl = I2CScl0;
    } else if ((chip_num == 2) || (chip_num == 3)) {
        sda = I2CSda1;
        scl = I2CScl1;
    } else if ((chip_num == 4) || (chip_num == 5)) {
        sda = I2CSda2;
        scl = I2CScl2;
    } else {
        error("Pin %i not in form factor", pin);
        return -1;
    }
    dev_addr = (chip_num % 2) ? 0x44 : 0x46;
    port_num = index % 24;
    output_port_reg = 4 + (port_num / 8);
    config_reg = 12 + (port_num / 8);
    reg_bit = port_num % 8;

    uint8_t read_config_byte[1];
    uint8_t read_output_byte[1];
    if (io_expander_i2c_read_bb(sda, scl, dev_addr, config_reg, read_config_byte, 1) != 0) {
        return -1;
    }
    if (io_expander_i2c_read_bb(sda, scl, dev_addr, output_port_reg, read_output_byte, 1) != 0) {
        return -1;
    }
    cmd0[0] = config_reg;
    if ((mode == PullDown) || (mode == PullUp)) {
        cmd0[1] = read_config_byte[0] & ~(1 << reg_bit);
        cmd1[0] = output_port_reg;
        if (mode == PullDown) {
            cmd1[1] = read_output_byte[0] & ~(1 << reg_bit);
        } else if (mode == PullUp) {
            cmd1[1] = read_output_byte[0] | (1 << reg_bit);
        }
    } else if (mode == PullNone) {
        cmd0[1] = read_config_byte[0] | (1 << reg_bit);
    }

    //write configuration register for all 3 modes
    if (io_expander_i2c_write_bb(sda, scl, dev_addr, cmd0, 2) != 0) {
        return -1;
    }
    //only write output register for pulldown and pullup
    if ((mode == PullDown) || (mode == PullUp)) {
        if (io_expander_i2c_write_bb(sda, scl, dev_addr, cmd1, 2) != 0) {
            return -1;
        }
    }
    return 0;
}

uint8_t MbedTester::io_expander_read_bb(PinName pin, IOExpanderReg reg_type)
{
    int index = _form_factor.index(pin);
    uint8_t read_byte[1] = {0};
    uint8_t chip_num;//can be 0-5
    SystemPin sda;//can be I2CSda0, I2CSda1, or I2CSda2
    SystemPin scl;//can be I2CScl0, I2CScl1, or I2CScl2
    uint16_t dev_addr;//can be 0x44 or 0x46
    uint8_t port_num;//can be 0-23
    uint8_t input_port_reg;//can be 0, 1, or 2
    uint8_t output_port_reg;//can be 4, 5, or 6
    uint8_t config_reg;//can be 12, 13, or 14
    uint8_t reg_bit;//can be 0-7

    chip_num = index / 24;
    if ((chip_num == 0) || (chip_num == 1)) {
        sda = I2CSda0;
        scl = I2CScl0;
    } else if ((chip_num == 2) || (chip_num == 3)) {
        sda = I2CSda1;
        scl = I2CScl1;
    } else if ((chip_num == 4) || (chip_num == 5)) {
        sda = I2CSda2;
        scl = I2CScl2;
    } else {
        sda = (SystemPin) - 1;
        scl = (SystemPin) - 1;
        error("Invalid pin index, index should be in the range of 0-127");
    }

    dev_addr = (chip_num % 2) ? 0x44 : 0x46;
    port_num = index % 24;
    input_port_reg = (port_num / 8);
    output_port_reg = 4 + (port_num / 8);
    config_reg = 12 + (port_num / 8);
    reg_bit = port_num % 8;
    uint8_t reg;
    if (reg_type == RegInput) {
        reg = input_port_reg;
    } else if (reg_type == RegOutput) {
        reg = output_port_reg;
    } else if (reg_type == RegConfig) {
        reg = config_reg;
    } else {
        reg = 0xFF;
        error("Invalid register type, should be: INPUT, OUTPUT, or CONFIG");
    }

    int read_success = io_expander_i2c_read_bb(sda, scl, dev_addr, reg, read_byte, 1);
    MBED_ASSERT(read_success == 0);
    uint8_t bit = (read_byte[0] & (1 << reg_bit)) >> reg_bit;
    return bit;
}

int MbedTester::io_expander_i2c_read_bb(SystemPin sda, SystemPin scl, uint8_t dev_addr, uint8_t start_reg, uint8_t *data, int length)
{
    //start condition
    sys_pin_write(sda, 0, false);
    sys_pin_write(scl, 0, false);
    sys_pin_write(sda, 0, true);

    // begin writing data, dev_addr first
    uint8_t send_bit;
    for (int j = 0; j < 2; j += 1) {
        sys_pin_write(scl, 0, true);
        sys_pin_write(sda, 0, true);
        for (int i = 7; i > -1; i -= 1) {
            if (j == 0) {
                send_bit = (dev_addr & (1 << i)) >> i;
            } else {
                send_bit = (start_reg & (1 << i)) >> i;
            }
            if (send_bit == 1) {
                sys_pin_write(sda, 0, false);
            } else if (send_bit == 0) {
                sys_pin_write(sda, 0, true);
            }
            sys_pin_write(scl, 0, false);
            sys_pin_write(scl, 0, true);
            sys_pin_write(sda, 0, true);
        }
        // receive ACK from IO extender
        sys_pin_write(sda, 0, false);//make sda high z to receive ACK
        //clk the ACK
        sys_pin_write(scl, 0, false);
        //read sda to check for ACK or NACK
        if (sys_pin_read(sda)) {
            return -1;//NACK - write failed
        }
        sys_pin_write(scl, 0, true);
    }

    //start condition
    sys_pin_write(sda, 0, false);
    sys_pin_write(scl, 0, false);
    sys_pin_write(sda, 0, true);

    // begin reading data, write (dev_addr | 1) first
    dev_addr |= 1;
    for (int j = -1; j < length; j += 1) {
        uint8_t read_byte = 0;
        for (int i = 7; i > -1; i -= 1) {
            if (j == -1) {
                sys_pin_write(scl, 0, true);
                sys_pin_write(sda, 0, true);
                send_bit = (dev_addr & (1 << i)) >> i;
                if (send_bit == 1) {
                    sys_pin_write(sda, 0, false);
                } else if (send_bit == 0) {
                    sys_pin_write(sda, 0, true);
                }
                sys_pin_write(scl, 0, false);
                sys_pin_write(scl, 0, true);
                sys_pin_write(sda, 0, true);
            } else {
                sys_pin_write(scl, 0, false);
                read_byte |= (sys_pin_read(sda) << i);
                sys_pin_write(scl, 0, true);
            }
        }
        if (j > -1) {
            data[j] = read_byte;
        }
        if (j == -1) {
            // receive ACK from IO extender
            sys_pin_write(sda, 0, false);//make sda high z to receive ACK
            //clk the ACK
            sys_pin_write(scl, 0, false);
            //read sda to check for ACK or NACK
            if (sys_pin_read(sda)) {
                return -1;//NACK - write failed
            }
            sys_pin_write(scl, 0, true);
        } else {
            if (j == (length - 1)) { //NACK to signal end of read
                sys_pin_write(sda, 0, false);
                sys_pin_write(scl, 0, false);
                sys_pin_write(scl, 0, true);
            } else {//ACK to signal read will continue
                sys_pin_write(sda, 0, true);
                sys_pin_write(scl, 0, false);
                sys_pin_write(scl, 0, true);
                sys_pin_write(sda, 0, false);
            }
        }
    }

    //stop condition
    sys_pin_write(sda, 0, true);
    sys_pin_write(scl, 0, false);
    sys_pin_write(sda, 0, false);
    return 0;
}

int MbedTester::io_expander_i2c_write_bb(SystemPin sda, SystemPin scl, uint8_t dev_addr, uint8_t *data, int length)
{
    //start condition
    sys_pin_write(sda, 0, false);
    sys_pin_write(scl, 0, false);
    sys_pin_write(sda, 0, true);

    // begin writing data, dev_addr first
    uint8_t send_bit;
    for (int j = -1; j < length; j += 1) {
        sys_pin_write(scl, 0, true);
        sys_pin_write(sda, 0, true);
        for (int i = 7; i > -1; i -= 1) {
            if (j == -1) {
                send_bit = (dev_addr & (1 << i)) >> i;
            } else {
                send_bit = (data[j] & (1 << i)) >> i;
            }
            if (send_bit == 1) {
                sys_pin_write(sda, 0, false);
            } else if (send_bit == 0) {
                sys_pin_write(sda, 0, true);
            }
            sys_pin_write(scl, 0, false);
            sys_pin_write(scl, 0, true);
            sys_pin_write(sda, 0, true);
        }
        // receive ACK from IO extender
        sys_pin_write(sda, 0, false);//make sda high z to receive ACK
        //clk the ACK
        sys_pin_write(scl, 0, false);
        //read sda to check for ACK or NACK
        if (sys_pin_read(sda)) {
            return -1;//NACK - write failed
        }
        sys_pin_write(scl, 0, true);
    }

    //stop condition
    sys_pin_write(sda, 0, true);
    sys_pin_write(scl, 0, false);
    sys_pin_write(sda, 0, false);
    return 0;
}

void MbedTester::set_analog_out(bool enable, float voltage)
{
    uint32_t cycles_high = (int)(100 * voltage);
    uint32_t period = 100;
    set_pwm_period_and_cycles_high(period, cycles_high);
    set_pwm_enable(enable);
}

int MbedTester::set_mux_addr(PinName pin)
{
    int index = _form_factor.index(pin);
    if ((index < 0) || (index > 127)) {
        error("Pin %i not in form factor", pin);
        return -1;
    }

    return set_mux_addr_index(index);
}

int MbedTester::set_mux_addr_index(int index)
{
    sys_pin_write(AnalogMuxAddr0, index & 0x01, true);
    sys_pin_write(AnalogMuxAddr1, (index & 0x02) >> 1, true);
    sys_pin_write(AnalogMuxAddr2, (index & 0x04) >> 2, true);
    sys_pin_write(AnalogMuxAddr3, (index & 0x08) >> 3, true);
    sys_pin_write(AnalogMuxAddr4, (index & 0x10) >> 4, true);
    sys_pin_write(AnalogMuxAddr5, (index & 0x20) >> 5, true);
    sys_pin_write(AnalogMuxAddr6, (index & 0x40) >> 6, true);
    sys_pin_write(AnalogMuxAddr7, (index & 0x80) >> 7, true);

    return 0;
}

void MbedTester::set_mux_enable(bool val)
{
    if (val == true) {
        sys_pin_write(AnalogMuxEnable, 0, true);//enable analog MUXes
    } else if (val == false) {
        sys_pin_write(AnalogMuxEnable, 1, true);//disable analog MUXes
    }
    wait_us(10);
}

void MbedTester::set_pwm_enable(bool val)
{
    uint8_t data;
    if (val == true) {
        data = 1;
    } else if (val == false) {
        data = 0;
    }
    write(TESTER_SYS_IO_PWM_ENABLE, &data, sizeof(data));
}

bool MbedTester::get_pwm_enable()
{
    uint8_t val = 0;
    read(TESTER_SYS_IO_PWM_ENABLE, &val, sizeof(val));
    if (val == 1) {
        return true;
    } else if (val == 0) {
        return false;
    } else {
        error("Corrupt pwm enable value");
        return false;
    }
}

void MbedTester::set_pwm_period_and_cycles_high(uint32_t period, uint32_t cycles_high)
{
    set_pwm_enable(false);
    uint32_t p = period - 1;//period in cycles
    uint32_t d = cycles_high;//number of cycles pwm out is high
    write(TESTER_SYS_IO_PWM_PERIOD, (uint8_t *)&p, sizeof(p));
    write(TESTER_SYS_IO_PWM_CYCLES_HIGH, (uint8_t *)&d, sizeof(d));
    set_pwm_enable(true);
}

uint32_t MbedTester::get_pwm_period()
{
    uint32_t period = 0;
    read(TESTER_SYS_IO_PWM_PERIOD, (uint8_t *)&period, sizeof(period));
    return period + 1;//clk cycles
}

uint8_t MbedTester::get_pwm_cycles_high()
{
    uint8_t cycles_high = 0;
    read(TESTER_SYS_IO_PWM_CYCLES_HIGH, &cycles_high, sizeof(cycles_high));
    return cycles_high;
}

uint16_t MbedTester::get_analogmuxin_measurement()
{
    rtos::ThisThread::sleep_for(1);//wait for value to stabalize
    //take snapshot of conversion value to make safe for reading
    set_snapshot();
    uint16_t an_mux_analogin_measurement = 0;
    read(TESTER_SYS_IO_AN_MUX_ANALOGIN_MEASUREMENT, (uint8_t *)&an_mux_analogin_measurement, sizeof(an_mux_analogin_measurement));
    return an_mux_analogin_measurement;
}

uint16_t MbedTester::get_anin_measurement(int index)
{
    //check index is in bounds
    if ((index < 0) || (index >= ANALOG_COUNT)) {
        error("AnalogIn index is out of bounds");
    }
    //take snapshot of conversion value to make safe for reading
    set_snapshot();
    uint16_t anin_measurement = 0;
    read((TESTER_SYS_IO_ANIN0_MEASUREMENT + (index * 10)), (uint8_t *)&anin_measurement, sizeof(anin_measurement)); //10 because sizeof measurement + sizeof measurements_sum = 10
    return anin_measurement;
}

void MbedTester::get_anin_sum_samples_cycles(int index, uint64_t *sum, uint32_t *samples, uint64_t *cycles)
{
    //check index is in bounds
    if ((index < 0) || (index >= ANALOG_COUNT)) {
        error("AnalogIn index is out of bounds");
    }
    //take snapshot of the sum/samples/cycles so that all 3 values are correct in relation to each other
    set_snapshot();
    read((TESTER_SYS_IO_ANIN0_MEASUREMENTS_SUM + (index * 10)), (uint8_t *)sum, sizeof(*sum)); //10 because sizeof measurement + sizeof measurements_sum = 10
    read(TESTER_SYS_IO_NUM_POWER_SAMPLES, (uint8_t *)samples, sizeof(*samples));
    read(TESTER_SYS_IO_NUM_POWER_CYCLES, (uint8_t *)cycles, sizeof(*cycles));
}

void MbedTester::set_snapshot()
{
    uint8_t data = 1;
    write(TESTER_SYS_IO_ADC_SNAPSHOT, &data, sizeof(data));
    wait_us(1);
}

void MbedTester::set_sample_adc(bool val)
{
    uint8_t data;
    if (val == true) {
        data = 1;
    } else if (val == false) {
        data = 0;
    }
    write(TESTER_SYS_IO_SAMPLE_ADC, &data, sizeof(data));
}

float MbedTester::get_analog_in()
{
    uint16_t data = get_analogmuxin_measurement();
    float data_f = (float)data / 4095.0f;
    return data_f;
}

float MbedTester::get_anin_voltage(int index)
{
    uint16_t data = get_anin_measurement(index);
    float data_f = (float)data / 4095.0f;
    return data_f;
}

int MbedTester::gpio_read(LogicalPin gpio)
{
    if (gpio >= LogicalPinCount) {
        error("Invalid pin for gpio_read");
        return 0;
    }
    uint8_t data = 0;
    read(TESTER_GPIO + gpio, &data, sizeof(data));
    return data;
}

void MbedTester::gpio_write(LogicalPin gpio, int value, bool drive)
{
    if (gpio >= LogicalPinCount) {
        error("Invalid pin for gpio_write");
        return;
    }
    uint8_t data = 0;
    data |= value ? (1 << 0) : 0;
    data |= drive ? (1 << 1) : 0;
    write(TESTER_GPIO + gpio, &data, sizeof(data));
}

void MbedTester::io_metrics_start()
{
    uint8_t data = TESTER_IO_METRICS_CTRL_RESET_BIT;
    write(TESTER_IO_METRICS_CTRL, &data, sizeof(data));

    data = TESTER_IO_METRICS_CTRL_ACTIVE_BIT;
    write(TESTER_IO_METRICS_CTRL, &data, sizeof(data));
}

void MbedTester::io_metrics_stop()
{
    uint8_t data = 0;
    write(TESTER_IO_METRICS_CTRL, &data, sizeof(data));
}

void MbedTester::io_metrics_continue()
{
    uint8_t data = TESTER_IO_METRICS_CTRL_ACTIVE_BIT;
    write(TESTER_IO_METRICS_CTRL, &data, sizeof(data));
}

uint32_t MbedTester::io_metrics_min_pulse_low(LogicalPin pin)
{
    if (pin >= LogicalPinCount) {
        error("Invalid pin for io_metrics");
        return 0;
    }

    uint32_t data = 0;
    read(TESTER_IO_METRICS_MIN_PULSE_LOW(pin), (uint8_t *)&data, sizeof(data));
    return data;
}

uint32_t MbedTester::io_metrics_min_pulse_high(LogicalPin pin)
{
    if (pin >= LogicalPinCount) {
        error("Invalid pin for io_metrics");
        return 0;
    }

    uint32_t data = 0;
    read(TESTER_IO_METRICS_MIN_PULSE_HIGH(pin), (uint8_t *)&data, sizeof(data));
    return data;
}

uint32_t MbedTester::io_metrics_max_pulse_low(LogicalPin pin)
{
    if (pin >= LogicalPinCount) {
        error("Invalid pin for io_metrics");
        return 0;
    }

    uint32_t data = 0;
    read(TESTER_IO_METRICS_MAX_PULSE_LOW(pin), (uint8_t *)&data, sizeof(data));
    return data;
}

uint32_t MbedTester::io_metrics_max_pulse_high(LogicalPin pin)
{
    if (pin >= LogicalPinCount) {
        error("Invalid pin for io_metrics");
        return 0;
    }

    uint32_t data = 0;
    read(TESTER_IO_METRICS_MAX_PULSE_HIGH(pin), (uint8_t *)&data, sizeof(data));
    return data;
}

uint32_t MbedTester::io_metrics_rising_edges(LogicalPin pin)
{
    if (pin >= LogicalPinCount) {
        error("Invalid pin for io_metrics");
        return 0;
    }

    uint32_t data = 0;
    read(TESTER_IO_METRICS_RISING_EDGES(pin), (uint8_t *)&data, sizeof(data));
    return data;
}

uint32_t MbedTester::io_metrics_falling_edges(LogicalPin pin)
{
    if (pin >= LogicalPinCount) {
        error("Invalid pin for io_metrics");
        return 0;
    }

    uint32_t data = 0;
    read(TESTER_IO_METRICS_FALLING_EDGES(pin), (uint8_t *)&data, sizeof(data));
    return data;
}

bool MbedTester::sys_pin_read(SystemPin pin)
{

    if (pin >= SystemPinCount) {
        error("Invalid pin for gpio_read");
        return 0;
    }
    uint8_t data = 0;
    read(TESTER_SYS_IO + pin, &data, sizeof(data));
    return data;
}

void MbedTester::sys_pin_write(SystemPin pin, int value, bool drive)
{
    if (pin >= SystemPinCount) {
        error("Invalid pin for gpio_write");
        return;
    }
    uint8_t data = 0;
    data |= value ? (1 << 0) : 0;
    data |= drive ? (1 << 1) : 0;
    write(TESTER_SYS_IO + pin, &data, sizeof(data));
}

void MbedTester::sys_pin_mode_disabled()
{
    const uint32_t base = LogicalPinTotal;

    pin_map_index(PHYSICAL_NC, (LogicalPin)(base + 0));
    pin_map_index(PHYSICAL_NC, (LogicalPin)(base + 1));
    pin_map_index(PHYSICAL_NC, (LogicalPin)(base + 2));
    pin_map_index(PHYSICAL_NC, (LogicalPin)(base + 3));

    uint8_t mode = TESTER_SYS_IO_MODE_DISABLED;
    write(TESTER_SYS_IO_MODE, &mode, sizeof(mode));
}

void MbedTester::sys_pin_mode_spi_serial_flash(PhysicalIndex mosi, PhysicalIndex miso, PhysicalIndex clk, PhysicalIndex ssel)
{
    const uint32_t base = LogicalPinTotal;

    pin_map_index(mosi, (LogicalPin)(base + 0));
    pin_map_index(miso, (LogicalPin)(base + 1));
    pin_map_index(clk, (LogicalPin)(base + 2));
    pin_map_index(ssel, (LogicalPin)(base + 3));

    uint8_t mode = TESTER_SYS_IO_MODE_SPI_SERIAL_FLASH;
    write(TESTER_SYS_IO_MODE, &mode, sizeof(mode));
}

void MbedTester::sys_pin_mode_i2c_io_expander(int index, PhysicalIndex sda_in, PhysicalIndex sda_val, PhysicalIndex scl_in, PhysicalIndex scl_val)
{
    const uint32_t base = LogicalPinTotal;

    pin_map_index(sda_in, (LogicalPin)(base + 0));
    pin_map_index(sda_val, (LogicalPin)(base + 1));
    pin_map_index(scl_in, (LogicalPin)(base + 2));
    pin_map_index(scl_val, (LogicalPin)(base + 3));

    uint8_t mode = 0;
    if (index == 0) {
        mode = TESTER_SYS_IO_MODE_I2C_IO_EXPANDER0;
    } else if (index == 1) {
        mode = TESTER_SYS_IO_MODE_I2C_IO_EXPANDER1;
    } else if (index == 2) {
        mode = TESTER_SYS_IO_MODE_I2C_IO_EXPANDER2;
    } else {
        error("Invalid index for sys_pin_mode_i2c_io_expander");
    }

    write(TESTER_SYS_IO_MODE, &mode, sizeof(mode));
}

void MbedTester::pin_map_index(PhysicalIndex physical_index, LogicalPin logical)
{
    uint8_t remap;
    if ((physical_index >= PHYSICAL_PINS) && (physical_index != PHYSICAL_NC)) {
        error("Invalid physical pin index %i", physical_index);
        return;
    }
    if (logical >= sizeof(_mapping) / sizeof(_mapping[0])) {
        error("Invalid logical pin %i", logical);
        return;
    }

    // Unmap the previous pin if it had been mapped
    if (_mapping[logical] < PHYSICAL_PINS) {
        remap = PHYSICAL_NC;
        write(TESTER_REMAP + _mapping[logical], &remap, sizeof(remap));
    }
    _mapping[logical] = physical_index;

    // Remap physical pin if it is not PHYSICAL_NC
    if (physical_index < PHYSICAL_PINS) {
        remap = logical;
        write(TESTER_REMAP + physical_index, &remap, sizeof(remap));
    }
    // Remap logical pin
    remap = physical_index;
    write(TESTER_REMAP + PHYSICAL_PINS + logical, &remap, sizeof(remap));
}

void MbedTester::write(uint32_t addr, const uint8_t *data, uint32_t size)
{
    _update_control_pins();

    mbed_tester_command(_clk, _mosi, _miso, _miso_index, addr, true, (uint8_t *)data, size);
}

void MbedTester::read(uint32_t addr, uint8_t *data, uint32_t size)
{
    _update_control_pins();

    mbed_tester_command(_clk, _mosi, _miso, _miso_index, addr, false, data, size);
}

bool MbedTester::self_test_all()
{
    return self_test_control_channels() && self_test_control_miso();
}

bool MbedTester::self_test_control_channels()
{
    for (uint32_t i = 0; i < _form_factor.count() / 2; i++) {
        const int clk_index = i * 2 + 0;
        const int mosi_index = i * 2 + 1;
        const PinName clk = _form_factor.get(clk_index);
        const PinName mosi = _form_factor.get(mosi_index);

        // Check if the control pair is allowed and skip if it is not
        if (_exclude_pins.has_pin(clk) || _exclude_pins.has_pin(mosi)) {
            mbed_tester_printf("Skipping pin indexes clk=%i, mosi=%i\r\n", i * 2 + 0, i * 2 + 1);
            continue;
        }

        // Find a pin to use as miso
        int miso_index = 0;
        PinName miso = NC;
        DynamicPinList more_restricted(_exclude_pins);
        more_restricted.add(clk);
        more_restricted.add(mosi);
        for (uint32_t j = 0; j < _form_factor.count(); j++) {
            miso_index = j;
            const PinName temp = _form_factor.get(miso_index);
            if (!more_restricted.has_pin(temp)) {
                miso = temp;
                break;
            }
        }
        if (miso == NC) {
            set_control_pins_auto();
            return false;
        }

        // Find a pin to use as aux
        int aux_index = 0;
        PinName aux = NC;
        more_restricted.add(miso);
        for (uint32_t j = 0; j < _form_factor.count(); j++) {
            aux_index = j;
            const PinName temp = _form_factor.get(aux_index);
            if (!more_restricted.has_pin(temp)) {
                aux = temp;
                break;
            }
        }
        if (aux == NC) {
            set_control_pins_auto();
            return false;
        }

        // Write and read back a value
        mbed_tester_printf("Testing clk_index=%2i, mosi_index=%2i, miso_index=%2i, aux_index=%2i\r\n", clk_index, mosi_index, miso_index, aux_index);
        set_control_pins_manual(clk, mosi, miso, aux);
        if (!self_test_control_current()) {
            mbed_tester_printf("  Fail\r\n");
            set_control_pins_auto();
            return false;
        }
        mbed_tester_printf("  Pass\r\n");
    }

    set_control_pins_auto();
    return true;
}

bool MbedTester::self_test_control_miso()
{
    for (uint32_t i = 0; i < _form_factor.count(); i++) {
        const int miso_index = i;
        const PinName miso = _form_factor.get(miso_index);

        if (_exclude_pins.has_pin(miso)) {
            mbed_tester_printf("Skipping miso index %i\r\n", i);
            continue;
        }

        // Find pins to use as clk and mosi
        int clk_index = 0;
        int mosi_index = 0;
        PinName clk = NC;
        PinName mosi = NC;
        DynamicPinList more_restricted(_exclude_pins);
        more_restricted.add(miso);
        for (uint32_t j = 0; j < _form_factor.count() / 2; j++) {
            clk_index = j * 2 + 0;
            mosi_index = j * 2 + 1;
            const PinName possible_clk = _form_factor.get(clk_index);
            const PinName possible_mosi = _form_factor.get(mosi_index);

            // Check if the control pair is allowed and skip if it is not
            if (!more_restricted.has_pin(possible_clk) && !more_restricted.has_pin(possible_mosi)) {
                clk = possible_clk;
                mosi = possible_mosi;
                break;
            }
        }

        if ((clk == NC) && (mosi == NC)) {
            set_control_pins_auto();
            return false;
        }

        // Find aux pin
        int aux_index = 0;
        PinName aux = NC;
        more_restricted.add(clk);
        more_restricted.add(mosi);
        for (uint32_t j = 0; j < _form_factor.count(); j++) {
            aux_index = j;
            const PinName possible_aux = _form_factor.get(aux_index);

            // Check if the control pair is allowed and skip if it is not
            if (!more_restricted.has_pin(possible_aux)) {
                aux = possible_aux;
                break;
            }
        }
        if (aux == NC) {
            set_control_pins_auto();
            return false;
        }


        mbed_tester_printf("Testing clk_index=%2i, mosi_index=%2i, miso_index=%2i, aux_index=%2i\r\n", clk_index, mosi_index, miso_index, aux_index);
        set_control_pins_manual(clk, mosi, miso, aux);
        if (!self_test_control_current()) {
            mbed_tester_printf("  Fail\r\n");
            set_control_pins_auto();
            return false;
        }
        mbed_tester_printf("  Pass\r\n");
    }
    set_control_pins_auto();
    return true;
}

bool MbedTester::self_test_control_current()
{
    uint8_t buf[4];

    read(TESTER_CONTROL, buf, sizeof(buf));

    return memcmp(buf, "mbed", sizeof(buf)) == 0;
}

bool MbedTester::_find_control_indexes(PhysicalIndex &clk_out, PhysicalIndex &mosi_out, PhysicalIndex &miso_out, PhysicalIndex &aux_out)
{
    MbedTesterBitMap<PHYSICAL_PINS> allowed;
    const size_t max_pins = _form_factor.count();
    const size_t max_controls = max_pins / 2;

    for (size_t i = 0; i < max_pins; i++) {
        PinName pin = _form_factor.get(i);
        if ((pin == NC) || _exclude_pins.has_pin(pin)) {
            // Skip these pins
            continue;
        }

        // Pin is allowed
        allowed.set(i);
    }
    for (size_t i = 0; i < LogicalPinTotal; i++) {
        PhysicalIndex index = _mapping[i];
        if (index < PHYSICAL_PINS) {
            allowed.clear(index);
        }
    }

    for (size_t i = 0; i < max_controls; i++) {
        uint8_t clk_index = i * 2 + 0;
        uint8_t mosi_index = i * 2 + 1;
        if (!allowed.get(clk_index) || !allowed.get(mosi_index)) {
            continue;
        }

        // Free CLK and MOSI pins found. Mark them as unavailable
        allowed.clear(clk_index);
        allowed.clear(mosi_index);
        mbed::DigitalInOut clk(_form_factor.get(clk_index), PIN_OUTPUT, ::PullNone, 0);
        mbed::DigitalInOut mosi(_form_factor.get(mosi_index), PIN_OUTPUT, ::PullNone, 0);

        for (uint8_t miso_index = 0; miso_index < max_pins; miso_index++) {
            if (!allowed.get(miso_index)) {
                continue;
            }

            mbed::DigitalInOut miso(_form_factor.get(miso_index));
            if (!mbed_tester_test(&clk, &mosi, &miso, miso_index)) {
                // Pin doesn't work
                continue;
            }

            // MISO found so find AUX starting from where miso left off
            for (uint8_t aux_index = miso_index + 1; aux_index < max_pins; aux_index++) {
                if (!allowed.get(aux_index)) {
                    continue;
                }

                mbed::DigitalInOut aux(_form_factor.get(aux_index));
                if (!mbed_tester_test(&clk, &mosi, &aux, aux_index)) {
                    // Pin doesn't work
                    continue;
                }

                // Found all 4 pins
                clk_out = clk_index;
                mosi_out = mosi_index;
                miso_out = miso_index;
                aux_out = aux_index;
                clk.input();
                mosi.input();
                return true;
            }

            // A valid control channel was found but the system
            // does not have enough working pins.
            clk.input();
            mosi.input();
            return false;
        }

        // Set CLK and MOSI pins don't work so set them back to available
        clk.input();
        mosi.input();
        allowed.set(clk_index);
        allowed.set(mosi_index);
    }
    return false;
}

void MbedTester::_setup_control_pins()
{
    _clk = new  mbed::DigitalInOut(_form_factor.get(_clk_index), PIN_OUTPUT, ::PullNone, 0);
    _mosi = new mbed::DigitalInOut(_form_factor.get(_mosi_index), PIN_OUTPUT, ::PullNone, 0);
    _miso = new  mbed::DigitalInOut(_form_factor.get(_miso_index));
    _aux = new  mbed::DigitalInOut(_form_factor.get(_aux_index));
}

void MbedTester::_free_control_pins()
{
    if (_clk) {
        _clk->input();
        delete _clk;
    }
    _clk = NULL;
    _clk_index = PHYSICAL_NC;
    if (_mosi) {
        _mosi->input();
        delete _mosi;
    }
    _mosi = NULL;
    _mosi_index = PHYSICAL_NC;
    if (_miso) {
        _miso->input();
        delete _miso;
    }
    _miso = NULL;
    _miso_index = PHYSICAL_NC;
    if (_aux) {
        _aux->input();
        delete _aux;
    }
    _aux = NULL;
    _aux_index = PHYSICAL_NC;
    _control_valid = false;
}

void MbedTester::_update_control_pins()
{
    if (_update_needed()) {

        if (!_control_auto) {
            error("Invalid control channel configuration");
        }

        _free_control_pins();
        if (_find_control_indexes(_clk_index, _mosi_index, _miso_index, _aux_index)) {
            mbed_tester_printf("Updating control pins to clk=%i, mosi=%i, miso=%i, aux=%i\r\n", _clk_index, _mosi_index, _miso_index, _aux_index);
            _setup_control_pins();
            _control_valid = true;
            return;
        } else {
            error("An MbedTester communication channel could not be created");
        }
    }
}

bool MbedTester::_update_needed()
{
    if (!_control_valid) {
        return true;
    }
    // Note - mappings beyond the the first two logical banks are allowed to overlap
    //        with the control channels. These mappings are used for firmware updates and
    //        IO expander control.
    for (size_t i = 0; i < LogicalPinTotal; i++) {
        PhysicalIndex pin = _mapping[i];
        if ((pin == _clk_index) || (pin == _mosi_index) || (pin == _miso_index) || (pin == _aux_index)) {
            return true;
        }
    }
    return false;
}

void MbedTester::_reset()
{
    for (uint32_t i = 0; i < sizeof(_mapping) / sizeof(_mapping[0]); i++) {
        _mapping[i] = PHYSICAL_NC;
    }
    _free_control_pins();
    _control_auto = true;
}
