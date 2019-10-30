/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "SPIFBlockDevice.h"
#include "rtos/ThisThread.h"
#include "mbed_critical.h"

#include <string.h>
#include <inttypes.h>

#include "mbed_trace.h"
#include "mbed_debug.h"
#define TRACE_GROUP "SPIF"
using namespace mbed;

/* Default SPIF Parameters */
/****************************/
#define SPIF_DEFAULT_READ_SIZE  1
#define SPIF_DEFAULT_PROG_SIZE  1
#define SPIF_DEFAULT_PAGE_SIZE  256
#define SPIF_DEFAULT_SE_SIZE    4096
#define SPI_MAX_STATUS_REGISTER_SIZE 2
#ifndef UINT64_MAX
#define UINT64_MAX -1
#endif
#define SPI_NO_ADDRESS_COMMAND UINT64_MAX
// Status Register Bits
#define SPIF_STATUS_BIT_WIP 0x1 //Write In Progress
#define SPIF_STATUS_BIT_WEL 0x2 // Write Enable Latch

/* SFDP Header Parsing */
/***********************/
#define SPIF_SFDP_HEADER_SIZE 8
#define SPIF_PARAM_HEADER_SIZE 8

/* Basic Parameters Table Parsing */
/**********************************/
#define SFDP_DEFAULT_BASIC_PARAMS_TABLE_SIZE_BYTES 64 /* 16 DWORDS */
//READ Instruction support according to BUS Configuration
#define SPIF_BASIC_PARAM_TABLE_FAST_READ_SUPPORT_BYTE 2
#define SPIF_BASIC_PARAM_TABLE_QPI_READ_SUPPORT_BYTE 16
#define SPIF_BASIC_PARAM_TABLE_222_READ_INST_BYTE 23
#define SPIF_BASIC_PARAM_TABLE_122_READ_INST_BYTE 15
#define SPIF_BASIC_PARAM_TABLE_112_READ_INST_BYTE 13
#define SPIF_BASIC_PARAM_TABLE_PAGE_SIZE_BYTE 40
// Address Length
#define SPIF_ADDR_SIZE_3_BYTES 3
#define SPIF_ADDR_SIZE_4_BYTES 4
// Erase Types Params
#define SPIF_BASIC_PARAM_ERASE_TYPE_1_BYTE 29
#define SPIF_BASIC_PARAM_ERASE_TYPE_2_BYTE 31
#define SPIF_BASIC_PARAM_ERASE_TYPE_3_BYTE 33
#define SPIF_BASIC_PARAM_ERASE_TYPE_4_BYTE 35
#define SPIF_BASIC_PARAM_ERASE_TYPE_1_SIZE_BYTE 28
#define SPIF_BASIC_PARAM_ERASE_TYPE_2_SIZE_BYTE 30
#define SPIF_BASIC_PARAM_ERASE_TYPE_3_SIZE_BYTE 32
#define SPIF_BASIC_PARAM_ERASE_TYPE_4_SIZE_BYTE 34
#define SPIF_BASIC_PARAM_4K_ERASE_TYPE_BYTE 1

// Erase Types Per Region BitMask
#define ERASE_BITMASK_TYPE4 0x08
#define ERASE_BITMASK_TYPE1 0x01
#define ERASE_BITMASK_NONE  0x00
#define ERASE_BITMASK_ALL   0x0F

#define IS_MEM_READY_MAX_RETRIES 10000

enum spif_default_instructions {
    SPIF_NOP = 0x00, // No operation
    SPIF_PP = 0x02, // Page Program data
    SPIF_READ = 0x03, // Read data
    SPIF_SE   = 0x20, // 4KB Sector Erase
    SPIF_SFDP = 0x5a, // Read SFDP
    SPIF_WRSR = 0x01, // Write Status/Configuration Register
    SPIF_WRDI = 0x04, // Write Disable
    SPIF_RDSR = 0x05, // Read Status Register
    SPIF_WREN = 0x06, // Write Enable
    SPIF_RSTEN = 0x66, // Reset Enable
    SPIF_RST = 0x99, // Reset
    SPIF_RDID = 0x9f, // Read Manufacturer and JDEC Device ID
    SPIF_ULBPR = 0x98, // Clears all write-protection bits in the Block-Protection register,
    SPIF_4BEN = 0xB7, // Enable 4-byte address mode
    SPIF_4BDIS = 0xE9, // Disable 4-byte address mode
};

// Mutex is used for some SPI Driver commands that must be done sequentially with no other commands in between
// e.g. (1)Set Write Enable, (2)Program, (3)Wait Memory Ready
SingletonPtr<PlatformMutex> SPIFBlockDevice::_mutex;

// Local Function
static unsigned int local_math_power(int base, int exp);

//***********************
// SPIF Block Device APIs
//***********************
SPIFBlockDevice::SPIFBlockDevice(
    PinName mosi, PinName miso, PinName sclk, PinName csel, int freq)
    : _spi(mosi, miso, sclk), _cs(csel), _read_instruction(0), _prog_instruction(0), _erase_instruction(0),
      _erase4k_inst(0), _page_size_bytes(0), _device_size_bytes(0), _init_ref_count(0), _is_initialized(false)
{
    _address_size = SPIF_ADDR_SIZE_3_BYTES;
    // Initial SFDP read tables are read with 8 dummy cycles
    // Default Bus Setup 1_1_1 with 0 dummy and mode cycles
    _read_dummy_and_mode_cycles = 8;
    _write_dummy_and_mode_cycles = 0;
    _dummy_and_mode_cycles = _read_dummy_and_mode_cycles;

    _min_common_erase_size = 0;
    _regions_count = 1;
    _region_erase_types_bitfield[0] = ERASE_BITMASK_NONE;

    if (SPIF_BD_ERROR_OK != _spi_set_frequency(freq)) {
        tr_error("SPI Set Frequency Failed");
    }

    _cs = 1;
}

int SPIFBlockDevice::init()
{
    uint8_t vendor_device_ids[4];
    size_t data_length = 3;
    int status = SPIF_BD_ERROR_OK;
    uint32_t basic_table_addr = 0;
    size_t basic_table_size = 0;
    uint32_t sector_map_table_addr = 0;
    size_t sector_map_table_size = 0;
    spif_bd_error spi_status = SPIF_BD_ERROR_OK;

    _mutex->lock();

    if (!_is_initialized) {
        _init_ref_count = 0;
    }

    _init_ref_count++;

    if (_init_ref_count != 1) {
        goto exit_point;
    }

    // Soft Reset
    if (-1 == _reset_flash_mem()) {
        tr_error("init - Unable to initialize flash memory, tests failed");
        status = SPIF_BD_ERROR_DEVICE_ERROR;
        goto exit_point;
    } else {
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Initialize flash memory OK\n");
    }

    /* Read Manufacturer ID (1byte), and Device ID (2bytes)*/
    spi_status = _spi_send_general_command(SPIF_RDID, SPI_NO_ADDRESS_COMMAND, NULL, 0, (char *)vendor_device_ids,
                                           data_length);
    if (spi_status != SPIF_BD_ERROR_OK) {
        tr_error("init - Read Vendor ID Failed");
        status = SPIF_BD_ERROR_DEVICE_ERROR;
        goto exit_point;
    }

    switch (vendor_device_ids[0]) {
        case 0xbf:
            // SST devices come preset with block protection
            // enabled for some regions, issue global protection unlock to clear
            _set_write_enable();
            _spi_send_general_command(SPIF_ULBPR, SPI_NO_ADDRESS_COMMAND, NULL, 0, NULL, 0);
            break;
    }

    //Synchronize Device
    if (false == _is_mem_ready()) {
        tr_error("init - _is_mem_ready Failed");
        status = SPIF_BD_ERROR_READY_FAILED;
        goto exit_point;
    }

    /**************************** Parse SFDP Header ***********************************/
    if (0 != _sfdp_parse_sfdp_headers(basic_table_addr, basic_table_size, sector_map_table_addr, sector_map_table_size)) {
        tr_error("init - Parse SFDP Headers Failed");
        status = SPIF_BD_ERROR_PARSING_FAILED;
        goto exit_point;
    }


    /**************************** Parse Basic Parameters Table ***********************************/
    if (0 != _sfdp_parse_basic_param_table(basic_table_addr, basic_table_size)) {
        tr_error("init - Parse Basic Param Table Failed");
        status = SPIF_BD_ERROR_PARSING_FAILED;
        goto exit_point;
    }

    /**************************** Parse Sector Map Table ***********************************/
    _region_size_bytes[0] =
        _device_size_bytes; // If there's no region map, we have a single region sized the entire device size
    _region_high_boundary[0] = _device_size_bytes - 1;

    if ((sector_map_table_addr != 0) && (0 != sector_map_table_size)) {
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: init - Parsing Sector Map Table - addr: 0x%" PRIx32 "h, Size: %d", sector_map_table_addr,
                 sector_map_table_size);
        if (0 != _sfdp_parse_sector_map_table(sector_map_table_addr, sector_map_table_size)) {
            tr_error("init - Parse Sector Map Table Failed");
            status = SPIF_BD_ERROR_PARSING_FAILED;
            goto exit_point;
        }
    }

    // Configure  BUS Mode to 1_1_1 for all commands other than Read
    // Dummy And Mode Cycles Back default 0
    _dummy_and_mode_cycles = _write_dummy_and_mode_cycles;
    _is_initialized = true;
    tr_debug("Device size: %llu Kbytes", _device_size_bytes / 1024);

    if (_device_size_bytes > (1 << 24)) {
        tr_debug("Size is bigger than 16MB and thus address does not fit in 3 byte, switch to 4 byte address mode");
        _spi_send_general_command(SPIF_4BEN, SPI_NO_ADDRESS_COMMAND, NULL, 0, NULL, 0);
        _address_size = SPIF_ADDR_SIZE_4_BYTES;
    }

exit_point:
    _mutex->unlock();

    return status;
}


int SPIFBlockDevice::deinit()
{
    spif_bd_error status = SPIF_BD_ERROR_OK;

    _mutex->lock();

    if (!_is_initialized) {
        _init_ref_count = 0;
        goto exit_point;
    }

    _init_ref_count--;

    if (_init_ref_count) {
        goto exit_point;
    }

    // Disable Device for Writing
    status = _spi_send_general_command(SPIF_WRDI, SPI_NO_ADDRESS_COMMAND, NULL, 0, NULL, 0);
    if (status != SPIF_BD_ERROR_OK)  {
        tr_error("Write Disable failed");
    }
    _is_initialized = false;

exit_point:
    _mutex->unlock();

    return status;
}

int SPIFBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    int status = SPIF_BD_ERROR_OK;
    debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG Read - Inst: 0x%xh", _read_instruction);
    _mutex->lock();

    // Set Dummy Cycles for Specific Read Command Mode
    _dummy_and_mode_cycles = _read_dummy_and_mode_cycles;

    status = _spi_send_read_command(_read_instruction, static_cast<uint8_t *>(buffer), addr, size);

    // Set Dummy Cycles for all other command modes
    _dummy_and_mode_cycles = _write_dummy_and_mode_cycles;

    _mutex->unlock();
    return status;
}

int SPIFBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    bool program_failed = false;
    int status = SPIF_BD_ERROR_OK;
    uint32_t offset = 0;
    uint32_t chunk = 0;

    debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: program - Buff: 0x%" PRIx32 "h, addr: %llu, size: %llu", (uint32_t)buffer, addr, size);

    while (size > 0) {

        // Write on _page_size_bytes boundaries (Default 256 bytes a page)
        offset = addr % _page_size_bytes;
        chunk = (offset + size < _page_size_bytes) ? size : (_page_size_bytes - offset);

        _mutex->lock();

        //Send WREN
        if (_set_write_enable() != 0) {
            tr_error("Write Enabe failed");
            program_failed = true;
            status = SPIF_BD_ERROR_WREN_FAILED;
            goto exit_point;
        }

        _spi_send_program_command(_prog_instruction, buffer, addr, chunk);

        buffer = static_cast<const uint8_t *>(buffer) + chunk;
        addr += chunk;
        size -= chunk;

        if (false == _is_mem_ready()) {
            tr_error("Device not ready after write, failed");
            program_failed = true;
            status = SPIF_BD_ERROR_READY_FAILED;
            goto exit_point;
        }
        _mutex->unlock();
    }

exit_point:
    if (program_failed) {
        _mutex->unlock();
    }

    return status;
}

int SPIFBlockDevice::erase(bd_addr_t addr, bd_size_t in_size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    int type = 0;
    uint32_t offset = 0;
    uint32_t chunk = 4096;
    int cur_erase_inst = _erase_instruction;
    int size = (int)in_size;
    bool erase_failed = false;
    int status = SPIF_BD_ERROR_OK;
    // Find region of erased address
    int region = _utils_find_addr_region(addr);
    if (region < 0) {
        tr_error("no region found for address %llu", addr);
        return SPIF_BD_ERROR_INVALID_ERASE_PARAMS;
    }
    // Erase Types of selected region
    uint8_t bitfield = _region_erase_types_bitfield[region];

    debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: erase - addr: %llu, in_size: %llu", addr, in_size);

    if ((addr + in_size) > _device_size_bytes) {
        tr_error("erase exceeds flash device size");
        return SPIF_BD_ERROR_INVALID_ERASE_PARAMS;
    }

    if (((addr % get_erase_size(addr)) != 0) || (((addr + in_size) % get_erase_size(addr + in_size - 1)) != 0)) {
        tr_error("invalid erase - unaligned address and size");
        return SPIF_BD_ERROR_INVALID_ERASE_PARAMS;
    }

    // For each iteration erase the largest section supported by current region
    while (size > 0) {

        // iterate to find next Largest erase type ( a. supported by region, b. smaller than size)
        // find the matching instruction and erase size chunk for that type.
        type = _utils_iterate_next_largest_erase_type(bitfield, size, (unsigned int)addr, _region_high_boundary[region]);
        cur_erase_inst = _erase_type_inst_arr[type];
        offset = addr % _erase_type_size_arr[type];
        chunk = ((offset + size) < _erase_type_size_arr[type]) ? size : (_erase_type_size_arr[type] - offset);

        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: erase - addr: %llu, size:%d, Inst: 0x%xh, chunk: %" PRIu32 " , ",
                 addr, size, cur_erase_inst, chunk);
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: erase - Region: %d, Type:%d",
                 region, type);

        _mutex->lock();

        if (_set_write_enable() != 0) {
            tr_error("SPI Erase Device not ready - failed");
            erase_failed = true;
            status = SPIF_BD_ERROR_READY_FAILED;
            goto exit_point;
        }

        _spi_send_erase_command(cur_erase_inst, addr, size);

        addr += chunk;
        size -= chunk;

        if ((size > 0) && (addr > _region_high_boundary[region])) {
            // erase crossed to next region
            region++;
            bitfield = _region_erase_types_bitfield[region];
        }

        if (false == _is_mem_ready()) {
            tr_error("SPI After Erase Device not ready - failed");
            erase_failed = true;
            status = SPIF_BD_ERROR_READY_FAILED;
            goto exit_point;
        }

        _mutex->unlock();
    }

exit_point:
    if (erase_failed) {
        _mutex->unlock();
    }

    return status;
}

bd_size_t SPIFBlockDevice::get_read_size() const
{
    // Assuming all devices support 1byte read granularity
    return SPIF_DEFAULT_READ_SIZE;
}

bd_size_t SPIFBlockDevice::get_program_size() const
{
    // Assuming all devices support 1byte program granularity
    return SPIF_DEFAULT_PROG_SIZE;
}

bd_size_t SPIFBlockDevice::get_erase_size() const
{
    // return minimal erase size supported by all regions (0 if none exists)
    return _min_common_erase_size;
}

// Find minimal erase size supported by the region to which the address belongs to
bd_size_t SPIFBlockDevice::get_erase_size(bd_addr_t addr) const
{
    // Find region of current address
    int region = _utils_find_addr_region(addr);

    unsigned int min_region_erase_size = _min_common_erase_size;
    int8_t type_mask = ERASE_BITMASK_TYPE1;
    int i_ind = 0;

    if (region != -1) {
        type_mask = 0x01;

        for (i_ind = 0; i_ind < 4; i_ind++) {
            // loop through erase types bitfield supported by region
            if (_region_erase_types_bitfield[region] & type_mask) {

                min_region_erase_size = _erase_type_size_arr[i_ind];
                break;
            }
            type_mask = type_mask << 1;
        }

        if (i_ind == 4) {
            tr_error("no erase type was found for region addr");
        }
    }

    return (bd_size_t)min_region_erase_size;
}

bd_size_t SPIFBlockDevice::size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _device_size_bytes;
}

int SPIFBlockDevice::get_erase_value() const
{
    return 0xFF;
}

const char *SPIFBlockDevice::get_type() const
{
    return "SPIF";
}

/***************************************************/
/*********** SPI Driver API Functions **************/
/***************************************************/
spif_bd_error SPIFBlockDevice::_spi_set_frequency(int freq)
{
    _spi.frequency(freq);
    return SPIF_BD_ERROR_OK;
}

spif_bd_error SPIFBlockDevice::_spi_send_read_command(int read_inst, uint8_t *buffer, bd_addr_t addr, bd_size_t size)
{
    uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
    int dummy_byte = 0;

    // csel must go low for the entire command (Inst, Address and Data)
    _cs = 0;

    // Write 1 byte Instruction
    _spi.write(read_inst);

    // Write Address (can be either 3 or 4 bytes long)
    for (int address_shift = ((_address_size - 1) * 8); address_shift >= 0; address_shift -= 8) {
        _spi.write((addr >> address_shift) & 0xFF);
    }

    // Write Dummy Cycles Bytes
    for (uint32_t i = 0; i < dummy_bytes; i++) {
        _spi.write(dummy_byte);
    }

    // Read Data
    for (bd_size_t i = 0; i < size; i++) {
        buffer[i] = _spi.write(0);
    }

    // csel back to high
    _cs = 1;
    return SPIF_BD_ERROR_OK;
}

spif_bd_error SPIFBlockDevice::_spi_send_program_command(int prog_inst, const void *buffer, bd_addr_t addr,
                                                         bd_size_t size)
{
    // Send Program (write) command to device driver
    uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
    int dummy_byte = 0;
    uint8_t *data = (uint8_t *)buffer;

    // csel must go low for the entire command (Inst, Address and Data)
    _cs = 0;

    // Write 1 byte Instruction
    _spi.write(prog_inst);

    // Write Address (can be either 3 or 4 bytes long)
    for (int address_shift = ((_address_size - 1) * 8); address_shift >= 0; address_shift -= 8) {
        _spi.write((addr >> address_shift) & 0xFF);
    }

    // Write Dummy Cycles Bytes
    for (uint32_t i = 0; i < dummy_bytes; i++) {
        _spi.write(dummy_byte);
    }

    // Write Data
    for (bd_size_t i = 0; i < size; i++) {
        _spi.write(data[i]);
    }

    // csel back to high
    _cs = 1;

    return SPIF_BD_ERROR_OK;
}

spif_bd_error SPIFBlockDevice::_spi_send_erase_command(int erase_inst, bd_addr_t addr, bd_size_t size)
{
    debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Erase Inst: 0x%xh, addr: %llu, size: %llu", erase_inst, addr, size);
    addr = (((int)addr) & 0xFFFFF000);
    _spi_send_general_command(erase_inst, addr, NULL, 0, NULL, 0);
    return SPIF_BD_ERROR_OK;
}

spif_bd_error SPIFBlockDevice::_spi_send_general_command(int instruction, bd_addr_t addr, char *tx_buffer,
                                                         size_t tx_length, char *rx_buffer, size_t rx_length)
{
    // Send a general command Instruction to driver
    uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
    uint8_t dummy_byte = 0x00;

    // csel must go low for the entire command (Inst, Address and Data)
    _cs = 0;

    // Write 1 byte Instruction
    _spi.write(instruction);

    // Reading SPI Bus registers does not require Flash Address
    if (addr != SPI_NO_ADDRESS_COMMAND) {
        // Write Address (can be either 3 or 4 bytes long)
        for (int address_shift = ((_address_size - 1) * 8); address_shift >= 0; address_shift -= 8) {
            _spi.write((addr >> address_shift) & 0xFF);
        }

        // Write Dummy Cycles Bytes
        for (uint32_t i = 0; i < dummy_bytes; i++) {
            _spi.write(dummy_byte);
        }
    }

    // Read/Write Data
    _spi.write(tx_buffer, (int)tx_length, rx_buffer, (int)rx_length);

    // csel back to high
    _cs = 1;

    return SPIF_BD_ERROR_OK;
}

/*********************************************************/
/********** SFDP Parsing and Detection Functions *********/
/*********************************************************/
int SPIFBlockDevice::_sfdp_parse_sector_map_table(uint32_t sector_map_table_addr, size_t sector_map_table_size)
{
    uint8_t sector_map_table[SFDP_DEFAULT_BASIC_PARAMS_TABLE_SIZE_BYTES]; /* Up To 16 DWORDS = 64 Bytes */
    uint32_t tmp_region_size = 0;
    int i_ind = 0;
    int prev_boundary = 0;
    // Default set to all type bits 1-4 are common
    int min_common_erase_type_bits = ERASE_BITMASK_ALL;


    spif_bd_error status = _spi_send_read_command(SPIF_SFDP, sector_map_table, sector_map_table_addr /*address*/,
                                                  sector_map_table_size);
    if (status != SPIF_BD_ERROR_OK) {
        tr_error("init - Read SFDP First Table Failed");
        return -1;
    }

    // Currently we support only Single Map Descriptor
    if (!((sector_map_table[0] & 0x3) == 0x03) && (sector_map_table[1]  == 0x0)) {
        tr_error("Sector Map - Supporting Only Single! Map Descriptor (not map commands)");
        return -1;
    }

    _regions_count = sector_map_table[2] + 1;
    if (_regions_count > SPIF_MAX_REGIONS) {
        tr_error("Supporting up to %d regions, current setup to %d regions - fail",
                 SPIF_MAX_REGIONS, _regions_count);
        return -1;
    }

    // Loop through Regions and set for each one: size, supported erase types, high boundary offset
    // Calculate minimum Common Erase Type for all Regions
    for (i_ind = 0; i_ind < _regions_count; i_ind++) {
        tmp_region_size = ((*((uint32_t *)&sector_map_table[(i_ind + 1) * 4])) >> 8) & 0x00FFFFFF; // bits 9-32
        _region_size_bytes[i_ind] = (tmp_region_size + 1) * 256; // Region size is 0 based multiple of 256 bytes;
        _region_erase_types_bitfield[i_ind] = sector_map_table[(i_ind + 1) * 4] & 0x0F; // bits 1-4
        min_common_erase_type_bits &= _region_erase_types_bitfield[i_ind];
        _region_high_boundary[i_ind] = (_region_size_bytes[i_ind] - 1) + prev_boundary;
        prev_boundary = _region_high_boundary[i_ind] + 1;
    }

    // Calc minimum Common Erase Size from min_common_erase_type_bits
    uint8_t type_mask = ERASE_BITMASK_TYPE1;
    for (i_ind = 0; i_ind < 4; i_ind++) {
        if (min_common_erase_type_bits & type_mask) {
            _min_common_erase_size = _erase_type_size_arr[i_ind];
            break;
        }
        type_mask = type_mask << 1;
    }

    if (i_ind == 4) {
        // No common erase type was found between regions
        _min_common_erase_size = 0;
    }

    return 0;
}

int SPIFBlockDevice::_sfdp_parse_basic_param_table(uint32_t basic_table_addr, size_t basic_table_size)
{
    uint8_t param_table[SFDP_DEFAULT_BASIC_PARAMS_TABLE_SIZE_BYTES]; /* Up To 16 DWORDS = 64 Bytes */
    //memset(param_table, 0, SFDP_DEFAULT_BASIC_PARAMS_TABLE_SIZE_BYTES);

    spif_bd_error status = _spi_send_read_command(SPIF_SFDP, param_table, basic_table_addr /*address*/,
                                                  basic_table_size);
    if (status != SPIF_BD_ERROR_OK) {
        tr_error("init - Read SFDP First Table Failed");
        return -1;
    }

    // Check address size, currently only supports 3byte addresses
    if ((param_table[2] & 0x4) != 0 || (param_table[7] & 0x80) != 0) {
        tr_error("init - verify 3byte addressing Failed");
        return -1;
    }

    // Get device density (stored in bits - 1)
    uint32_t density_bits = (
                                (param_table[7] << 24) |
                                (param_table[6] << 16) |
                                (param_table[5] << 8) |
                                param_table[4]);
    _device_size_bytes = (density_bits + 1) / 8;
    tr_debug("Density bits: %" PRIu32 " , device size: %llu bytes", density_bits, _device_size_bytes);

    // Set Default read/program/erase Instructions
    _read_instruction = SPIF_READ;
    _prog_instruction = SPIF_PP;
    _erase_instruction = SPIF_SE;

    // Set Page Size (SPI write must be done on Page limits)
    _page_size_bytes = _sfdp_detect_page_size(param_table, basic_table_size);

    // Detect and Set Erase Types
    _sfdp_detect_erase_types_inst_and_size(param_table, basic_table_size, _erase4k_inst, _erase_type_inst_arr,
                                           _erase_type_size_arr);
    _erase_instruction = _erase4k_inst;

    // Detect and Set fastest Bus mode (default 1-1-1)
    _sfdp_detect_best_bus_read_mode(param_table, basic_table_size, _read_instruction);

    return 0;
}

int SPIFBlockDevice::_sfdp_parse_sfdp_headers(uint32_t &basic_table_addr, size_t &basic_table_size,
                                              uint32_t &sector_map_table_addr, size_t &sector_map_table_size)
{
    uint8_t sfdp_header[16];
    uint8_t param_header[SPIF_SFDP_HEADER_SIZE];
    size_t data_length = SPIF_SFDP_HEADER_SIZE;
    bd_addr_t addr = 0x0;

    // Set 1-1-1 bus mode for SFDP header parsing
    // Initial SFDP read tables are read with 8 dummy cycles
    _read_dummy_and_mode_cycles = 8;
    _dummy_and_mode_cycles = 8;

    spif_bd_error status = _spi_send_read_command(SPIF_SFDP, sfdp_header, addr /*address*/, data_length);
    if (status != SPIF_BD_ERROR_OK) {
        tr_error("init - Read SFDP Failed");
        return -1;
    }

    // Verify SFDP signature for sanity
    // Also check that major/minor version is acceptable
    if (!(memcmp(&sfdp_header[0], "SFDP", 4) == 0 && sfdp_header[5] == 1)) {
        tr_error("init - _verify SFDP signature and version Failed");
        return -1;
    } else {
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: init - verified SFDP Signature and version Successfully");
    }

    // Discover Number of Parameter Headers
    int number_of_param_headers = (int)(sfdp_header[6]) + 1;
    debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: number of Param Headers: %d", number_of_param_headers);

    addr += SPIF_SFDP_HEADER_SIZE;
    data_length = SPIF_PARAM_HEADER_SIZE;

    // Loop over Param Headers and parse them (currently supported Basic Param Table and Sector Region Map Table)
    for (int i_ind = 0; i_ind < number_of_param_headers; i_ind++) {

        status = _spi_send_read_command(SPIF_SFDP, param_header, addr, data_length);
        if (status != SPIF_BD_ERROR_OK) {
            tr_error("init - Read Param Table %d Failed", i_ind + 1);
            return -1;
        }

        // The SFDP spec indicates the standard table is always at offset 0
        // in the parameter headers, we check just to be safe
        if (param_header[2] != 1) {
            tr_error("Param Table %d - Major Version should be 1!", i_ind + 1);
            return -1;
        }

        if ((param_header[0] == 0) && (param_header[7] == 0xFF)) {
            // Found Basic Params Table: LSB=0x00, MSB=0xFF
            debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Found Basic Param Table at Table: %d", i_ind + 1);
            basic_table_addr = ((param_header[6] << 16) | (param_header[5] << 8) | (param_header[4]));
            // Supporting up to 64 Bytes Table (16 DWORDS)
            basic_table_size = ((param_header[3] * 4) < SFDP_DEFAULT_BASIC_PARAMS_TABLE_SIZE_BYTES) ? (param_header[3] * 4) : 64;

        } else if ((param_header[0] == 81) && (param_header[7] == 0xFF)) {
            // Found Sector Map Table: LSB=0x81, MSB=0xFF
            debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Found Sector Map Table at Table: %d", i_ind + 1);
            sector_map_table_addr = ((param_header[6] << 16) | (param_header[5] << 8) | (param_header[4]));
            sector_map_table_size = param_header[3] * 4;

        }
        addr += SPIF_PARAM_HEADER_SIZE;

    }
    return 0;
}

unsigned int SPIFBlockDevice::_sfdp_detect_page_size(uint8_t *basic_param_table_ptr, int basic_param_table_size)
{
    unsigned int page_size = SPIF_DEFAULT_PAGE_SIZE;

    if (basic_param_table_size > SPIF_BASIC_PARAM_TABLE_PAGE_SIZE_BYTE) {
        // Page Size is specified by 4 Bits (N), calculated by 2^N
        int page_to_power_size = ((int)basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_PAGE_SIZE_BYTE]) >> 4;
        page_size = local_math_power(2, page_to_power_size);
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Detected Page Size: %d", page_size);
    } else {
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Using Default Page Size: %d", page_size);
    }
    return page_size;
}

int SPIFBlockDevice::_sfdp_detect_erase_types_inst_and_size(uint8_t *basic_param_table_ptr, int basic_param_table_size,
                                                            int &erase4k_inst,
                                                            int *erase_type_inst_arr, unsigned int *erase_type_size_arr)
{
    erase4k_inst = 0xff;
    bool found_4Kerase_type = false;
    uint8_t bitfield = 0x01;

    // Erase 4K Inst is taken either from param table legacy 4K erase or superseded by erase Instruction for type of size 4K
    erase4k_inst = basic_param_table_ptr[SPIF_BASIC_PARAM_4K_ERASE_TYPE_BYTE];

    if (basic_param_table_size > SPIF_BASIC_PARAM_ERASE_TYPE_1_SIZE_BYTE) {
        // Loop Erase Types 1-4
        for (int i_ind = 0; i_ind < 4; i_ind++) {
            erase_type_inst_arr[i_ind] = 0xff; //0xFF default for unsupported type
            erase_type_size_arr[i_ind] = local_math_power(2,
                                                          basic_param_table_ptr[SPIF_BASIC_PARAM_ERASE_TYPE_1_SIZE_BYTE + 2 * i_ind]); // Size given as 2^N
            debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Erase Type(A) %d - Inst: 0x%xh, Size: %d", (i_ind + 1), erase_type_inst_arr[i_ind],
                     erase_type_size_arr[i_ind]);
            if (erase_type_size_arr[i_ind] > 1) {
                // if size==1 type is not supported
                erase_type_inst_arr[i_ind] = basic_param_table_ptr[SPIF_BASIC_PARAM_ERASE_TYPE_1_BYTE + 2 * i_ind];

                if ((erase_type_size_arr[i_ind] < _min_common_erase_size) || (_min_common_erase_size == 0)) {
                    //Set default minimal common erase for singal region
                    _min_common_erase_size = erase_type_size_arr[i_ind];
                }

                // SFDP standard requires 4K Erase type to exist and its instruction to be identical to legacy field erase instruction
                if (erase_type_size_arr[i_ind] == 4096) {
                    found_4Kerase_type = true;
                    if (erase4k_inst != erase_type_inst_arr[i_ind]) {
                        //Verify 4KErase Type is identical to Legacy 4K erase type specified in Byte 1 of Param Table
                        erase4k_inst = erase_type_inst_arr[i_ind];
                        tr_warning("_detectEraseTypesInstAndSize - Default 4K erase Inst is different than erase type Inst for 4K");

                    }
                }
                _region_erase_types_bitfield[0] |= bitfield; // If there's no region map, set region "0" types bitfield as defualt;
            }
            debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "INFO: Erase Type %d - Inst: 0x%xh, Size: %d", (i_ind + 1),
                     erase_type_inst_arr[i_ind], erase_type_size_arr[i_ind]);
            bitfield = bitfield << 1;
        }
    }

    if (false == found_4Kerase_type) {
        tr_warning("Couldn't find Erase Type for 4KB size");
    }
    return 0;
}

int SPIFBlockDevice::_sfdp_detect_best_bus_read_mode(uint8_t *basic_param_table_ptr, int basic_param_table_size,
                                                     int &read_inst)
{
    do {

        // TBD - SPIF Dual Read Modes Require SPI driver support
        /*
        uint8_t examined_byte;

        if (basic_param_table_size > SPIF_BASIC_PARAM_TABLE_QPI_READ_SUPPORT_BYTE) {
            examined_byte = basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_QPI_READ_SUPPORT_BYTE];
            if (examined_byte & 0x01) {
                //  Fast Read 2-2-2 Supported
                read_inst = basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_222_READ_INST_BYTE];
                _read_dummy_and_mode_cycles = (basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_222_READ_INST_BYTE - 1] >> 5)
                                         + (basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_222_READ_INST_BYTE - 1] & 0x1F);
                debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "\nDEBUG: Read Bus Mode set to 2-2-2, Instruction: 0x%xh", read_inst);
                break;
            }
        }
        examined_byte = basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_FAST_READ_SUPPORT_BYTE];
        if (examined_byte & 0x20) {
            //  Fast Read 1-2-2 Supported
            read_inst = basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_122_READ_INST_BYTE];
            _read_dummy_and_mode_cycles = (basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_122_READ_INST_BYTE - 1] >> 5)
                                     + (basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_122_READ_INST_BYTE - 1] & 0x1F);
            debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "\nDEBUG: Read Bus Mode set to 1-2-2, Instruction: 0x%xh", read_inst);
            break;
        }
        if (examined_byte & 0x01) {
            // Fast Read 1-1-2 Supported
            read_inst = basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_112_READ_INST_BYTE];
            _read_dummy_and_mode_cycles = (basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_112_READ_INST_BYTE - 1] >> 5)
                                     + (basic_param_table_ptr[SPIF_BASIC_PARAM_TABLE_112_READ_INST_BYTE - 1] & 0x1F);
            debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "\nDEBUG: Read Bus Mode set to 1-1-2, Instruction: 0x%xh", _read_instruction);
            break;
        }
         */
        _read_dummy_and_mode_cycles = 0;
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "\nDEBUG: Read Bus Mode set to 1-1-1, Instruction: 0x%xh", read_inst);
    } while (false);

    return 0;
}

int SPIFBlockDevice::_reset_flash_mem()
{
    // Perform Soft Reset of the Device prior to initialization
    int status = 0;
    char status_value[2] = {0};
    debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "INFO: _reset_flash_mem:\n");
    //Read the Status Register from device
    if (SPIF_BD_ERROR_OK == _spi_send_general_command(SPIF_RDSR, SPI_NO_ADDRESS_COMMAND, NULL, 0, status_value, 1)) {
        // store received values in status_value
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Reading Status Register Success: value = 0x%x\n", (int)status_value[0]);
    } else {
        debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "ERROR: Reading Status Register failed\n");
        status = -1;
    }

    if (0 == status) {
        //Send Reset Enable
        if (SPIF_BD_ERROR_OK == _spi_send_general_command(SPIF_RSTEN, SPI_NO_ADDRESS_COMMAND, NULL, 0, NULL, 0)) {
            // store received values in status_value
            debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Sending RSTEN Success\n");
        } else {
            tr_error("Sending RSTEN failed");
            status = -1;
        }

        if (0 == status) {
            //Send Reset
            if (SPIF_BD_ERROR_OK == _spi_send_general_command(SPIF_RST, SPI_NO_ADDRESS_COMMAND, NULL, 0, NULL, 0)) {
                // store received values in status_value
                debug_if(MBED_CONF_SPIF_DRIVER_DEBUG, "DEBUG: Sending RST Success\n");
            } else {
                tr_error("Sending RST failed");
                status = -1;
            }
            if (false == _is_mem_ready()) {
                tr_error("Device not ready, write failed");
                status = -1;
            }
        }
    }

    return status;
}

bool SPIFBlockDevice::_is_mem_ready()
{
    // Check Status Register Busy Bit to Verify the Device isn't Busy
    char status_value[2];
    int retries = 0;
    bool mem_ready = true;

    do {
        rtos::ThisThread::sleep_for(1);
        retries++;
        //Read the Status Register from device
        if (SPIF_BD_ERROR_OK != _spi_send_general_command(SPIF_RDSR, SPI_NO_ADDRESS_COMMAND, NULL, 0, status_value,
                                                          1)) {   // store received values in status_value
            tr_error("Reading Status Register failed");
        }
    } while ((status_value[0] & SPIF_STATUS_BIT_WIP) != 0 && retries < IS_MEM_READY_MAX_RETRIES);

    if ((status_value[0] & SPIF_STATUS_BIT_WIP) != 0) {
        tr_error("_is_mem_ready FALSE");
        mem_ready = false;
    }
    return mem_ready;
}

int SPIFBlockDevice::_set_write_enable()
{
    // Check Status Register Busy Bit to Verify the Device isn't Busy
    char status_value[2];
    int status = -1;

    do {
        if (SPIF_BD_ERROR_OK !=  _spi_send_general_command(SPIF_WREN, SPI_NO_ADDRESS_COMMAND, NULL, 0, NULL, 0)) {
            tr_error("Sending WREN command FAILED");
            break;
        }

        if (false == _is_mem_ready()) {
            tr_error("Device not ready, write failed");
            break;
        }

        memset(status_value, 0, 2);
        if (SPIF_BD_ERROR_OK != _spi_send_general_command(SPIF_RDSR, SPI_NO_ADDRESS_COMMAND, NULL, 0, status_value,
                                                          1)) {   // store received values in status_value
            tr_error("Reading Status Register failed");
            break;
        }

        if ((status_value[0] & SPIF_STATUS_BIT_WEL) == 0) {
            tr_error("_set_write_enable failed");
            break;
        }
        status = 0;
    } while (false);
    return status;
}

/*********************************************/
/************* Utility Functions *************/
/*********************************************/
int SPIFBlockDevice::_utils_find_addr_region(bd_size_t offset) const
{
    //Find the region to which the given offset belong to
    if ((offset > _device_size_bytes) || (_regions_count == 0)) {
        return -1;
    }

    if (_regions_count == 1) {
        return 0;
    }

    for (int i_ind = _regions_count - 2; i_ind >= 0; i_ind--) {

        if (offset > _region_high_boundary[i_ind]) {
            return (i_ind + 1);
        }
    }
    return -1;

}

int SPIFBlockDevice::_utils_iterate_next_largest_erase_type(uint8_t &bitfield, int size, int offset, int boundry)
{
    // Iterate on all supported Erase Types of the Region to which the offset belong to.
    // Iterates from highest type to lowest
    uint8_t type_mask = ERASE_BITMASK_TYPE4;
    int i_ind  = 0;
    int largest_erase_type = 0;
    for (i_ind = 3; i_ind >= 0; i_ind--) {
        if (bitfield & type_mask) {
            largest_erase_type = i_ind;
            if ((size > (int)(_erase_type_size_arr[largest_erase_type])) &&
                    ((boundry - offset) > (int)(_erase_type_size_arr[largest_erase_type]))) {
                break;
            } else {
                bitfield &= ~type_mask;
            }
        }
        type_mask = type_mask >> 1;
    }

    if (i_ind == 4) {
        tr_error("no erase type was found for current region addr");
    }
    return largest_erase_type;

}

/*********************************************/
/************** Local Functions **************/
/*********************************************/
static unsigned int local_math_power(int base, int exp)
{
    // Integer X^Y function, used to calculate size fields given in 2^N format
    int result = 1;
    while (exp) {
        result *= base;
        exp--;
    }
    return result;
}


