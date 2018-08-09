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

#include "SPIFBlockDevice.h"
#include "mbed_critical.h"

// Read/write/erase sizes
#define SPIF_READ_SIZE  1
#define SPIF_PROG_SIZE  1
#define SPIF_SE_SIZE    4096
#define SPIF_TIMEOUT    10000

// Debug available
#define SPIF_DEBUG      0

// MX25R Series Register Command Table. 
enum ops {
    SPIF_NOP  = 0x00, // No operation
    SPIF_READ = 0x03, // Read data
    SPIF_PROG = 0x02, // Program data
    SPIF_SE   = 0x20, // 4KB Sector Erase 
    SPIF_CE   = 0xc7, // Chip Erase 
    SPIF_SFDP = 0x5a, // Read SFDP 
    SPIF_WREN = 0x06, // Write Enable 
    SPIF_WRDI = 0x04, // Write Disable
    SPIF_RDSR = 0x05, // Read Status Register 
    SPIF_RDID = 0x9f, // Read Manufacturer and JDEC Device ID 
};

// Status register from RDSR
// [- stuff -| wel | wip ]
// [-   6   -|  1  |  1  ]
#define SPIF_WEL 0x2
#define SPIF_WIP 0x1

 
SPIFBlockDevice::SPIFBlockDevice(
    PinName mosi, PinName miso, PinName sclk, PinName cs, int freq)
    : _spi(mosi, miso, sclk), _cs(cs), _size(0), _is_initialized(false), _init_ref_count(0)
{
    _cs = 1;
    _spi.frequency(freq);
    _address_size = QSPI_CFG_ADDR_SIZE_24;
    // Initial SFDP read tables are read with 8 dummy cycles
    _read_dummy_and_mode_cycles = 8;
    _dummy_and_mode_cycles = 8;

}

int SPIFBlockDevice::init()
{

    uint8_t vendor_device_ids[4];
    size_t data_length = 3;
    int status = SPIF_BD_ERROR_OK;
    uint32_t basic_table_addr = NULL;
    size_t basic_table_size = 0;
    uint32_t sector_map_table_addr = NULL;
    size_t sector_map_table_size = 0;
    int qspi_status = QSPI_STATUS_OK;

    _mutex.lock();
    if (_is_initialized == true) {
        goto exit_point;
    }

    // Soft Reset
    /*
    if ( -1 == _reset_flash_mem()) {
        tr_error("ERROR: init - Unable to initialize flash memory, tests failed\n");
        status = QSPIF_BD_ERROR_DEVICE_ERROR;
        goto exit_point;
    } else {
        tr_info("INFO: Initialize flash memory OK\n");
    }
    */

    /* Read Manufacturer ID (1byte), and Device ID (2bytes)*/
    qspi_status = _qspi_send_read_command(QSPIF_RDID, (char *)vendor_device_ids, 0x0 /*address*/, data_length);
    if (qspi_status != QSPI_STATUS_OK) {
        tr_error("ERROR: init - Read Vendor ID Failed");
        status = QSPIF_BD_ERROR_DEVICE_ERROR;
        goto exit_point;
    }

    switch (vendor_device_ids[0]) {
        case 0xbf:
            // SST devices come preset with block protection
            // enabled for some regions, issue write disable instruction to clear
            _set_write_enable();
            _qspi_send_general_command(QSPIF_WRDI, -1, NULL, 0, NULL, 0);
            break;
    }

    //Synchronize Device
    if ( false == _is_mem_ready()) {
        tr_error("ERROR: init - _is_mem_ready Failed");
        status = QSPIF_BD_ERROR_READY_FAILED;
        goto exit_point;
    }

    /**************************** Parse SFDP Header ***********************************/
    if ( 0 != _sfdp_parse_sfdp_headers(basic_table_addr, basic_table_size, sector_map_table_addr, sector_map_table_size)) {
        tr_error("ERROR: init - Parse SFDP Headers Failed");
        status = QSPIF_BD_ERROR_PARSING_FAILED;
        goto exit_point;
    }


    /**************************** Parse Basic Parameters Table ***********************************/
    if ( 0 != _sfdp_parse_basic_param_table(basic_table_addr, basic_table_size) ) {
        tr_error("ERROR: init - Parse Basic Param Table Failed");
        status = QSPIF_BD_ERROR_PARSING_FAILED;
        goto exit_point;
    }

    /**************************** Parse Sector Map Table ***********************************/
    _region_size_bytes[0] =
        _device_size_bytes; // If there's no region map, we have a single region sized the entire device size
    _region_high_boundary[0] = _device_size_bytes - 1;

    if ( (sector_map_table_addr != NULL) && (0 != sector_map_table_size) ) {
        tr_info("INFO: init - Parsing Sector Map Table - addr: 0x%xh, Size: %d", sector_map_table_addr,
                sector_map_table_size);
        if (0 != _sfdp_parse_sector_map_table(sector_map_table_addr, sector_map_table_size) ) {
            tr_error("ERROR: init - Parse Sector Map Table Failed");
            status = QSPIF_BD_ERROR_PARSING_FAILED;
            goto exit_point;
        }
    }

    // Configure  BUS Mode to 1_1_1 for all commands other than Read
    _qspi_configure_format(QSPI_CFG_BUS_SINGLE, QSPI_CFG_BUS_SINGLE, QSPI_CFG_ADDR_SIZE_24, QSPI_CFG_BUS_SINGLE,
                           QSPI_CFG_ALT_SIZE_8, QSPI_CFG_BUS_SINGLE, 0);

    _is_initialized = true;

exit_point:
    _mutex.unlock();

    return status;


/**************************************************************************************************************/
    if (!_is_initialized) {
        _init_ref_count = 0;
    }

    uint32_t val = core_util_atomic_incr_u32(&_init_ref_count, 1);

    if (val != 1) {
        return BD_ERROR_OK;
    }

    // Initial SFDP read tables are read with 8 dummy cycles
    _read_dummy_and_mode_cycles = 8;

    // Check for vendor specific hacks, these should move into more general
    // handling when possible. RDID is not used to verify a device is attached.
    uint8_t id[3];
    _cmdread(SPIF_RDID, 0, 3, 0x0, id);

    switch (id[0]) {
        case 0xbf:
            // SST devices come preset with block protection
            // enabled for some regions, issue gbpu instruction to clear
            _wren();
            _cmdwrite(0x98, 0, 0, 0x0, NULL);
            break;
    }

    // Check that device is doing ok 
    int err = _sync();
    if (err) {
        return BD_ERROR_DEVICE_ERROR;
    }

    // Check JEDEC serial flash discoverable parameters for device
    // specific info
    uint8_t header[16];
    _cmdread(SPIF_SFDP, 4, 16, 0x0, header);

    // Verify SFDP signature for sanity
    // Also check that major/minor version is acceptable
    if (!(memcmp(&header[0], "SFDP", 4) == 0 && header[5] == 1)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    // The SFDP spec indicates the standard table is always at offset 0
    // in the parameter headers, we check just to be safe
    if (!(header[8] == 0 && header[10] == 1)) {
        return BD_ERROR_DEVICE_ERROR;
    }

    // Parameter table pointer, spi commands are BE, SFDP is LE,
    // also sfdp command expects extra read wait byte
    uint32_t table_addr = (
        (header[14] << 24) |
        (header[13] << 16) |
        (header[12] << 8 ));
    uint8_t table[8];
    _cmdread(SPIF_SFDP, 4, 8, table_addr, table);

    // Check erase size, currently only supports 4kbytes
    // TODO support erase size != 4kbytes?
    // TODO support other erase opcodes from the sector descriptions
    if ((table[0] & 0x3) != 0x1 || table[1] != SPIF_SE) {
        return BD_ERROR_DEVICE_ERROR;
    }

    // Check address size, currently only supports 3byte addresses
    // TODO support address > 3bytes?
    // TODO check for devices larger than 2Gbits?
    if ((table[2] & 0x4) != 0 || (table[7] & 0x80) != 0) {
        return BD_ERROR_DEVICE_ERROR;
    }

    // Get device density, stored as size in bits - 1
    uint32_t density = (
        (table[7] << 24) |
        (table[6] << 16) |
        (table[5] << 8 ) |
        (table[4] << 0 ));
    _size = (density/8) + 1;

    _is_initialized = true;
    return 0;
}

int SPIFBlockDevice::deinit()
{
    if (!_is_initialized) {
        _init_ref_count = 0;
        return 0;
    }

    uint32_t val = core_util_atomic_decr_u32(&_init_ref_count, 1);

    if (val) {
        return 0;
    }

    // Latch write disable just to keep noise
    // from changing the device
    _cmdwrite(SPIF_WRDI, 0, 0, 0x0, NULL);

    _is_initialized = false;
    return 0;
}



void SPIFBlockDevice::_cmdread(uint8_t inst, uint32_t addr, uint32_t data_size, uint8_t *data)
{
    _cs = 0;
    uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
    uint8_t dummy_byte = 0x00;
    uint8_t *addr_byte_ptr = &addr;
    addr_byte_ptr += (_address_size-1)

    // Write 1 byte Instruction
    _spi.write(inst);

    // Write Address (can be either 3 or 4 bytes long)
    for (uint32_t i = 0; i < _address_size; i++) {
        _spi.write(*addr_byte_ptr);
        addr_byte_ptr--;
    }

    // Write Dummy Cycles Bytes
    for (uint32_t i = 0; i < dummy_bytes; i++) {
        _spi.write(dummy_byte);
    }

    // Read Data
    for (uint32_t i = 0; i < data_size; i++) {
    	data[i] = _spi.write(0);
    }
    _cs = 1;
}

void SPIFBlockDevice::_cmdwrite(uint8_t inst, uint32_t addr, uint32_t data_size, const uint8_t *data)
{
    _cs = 0;
    uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
    uint8_t dummy_byte = 0x00;
    uint8_t *addr_byte_ptr = &addr;
    addr_byte_ptr += (_address_size-1)

    // Write 1 byte Instruction
    _spi.write(inst);

    // Write Address (can be either 3 or 4 bytes long)
    for (uint32_t i = 0; i < _address_size; i++) {
        _spi.write(*addr_byte_ptr);
        addr_byte_ptr--;
    }

    // Write Dummy Cycles Bytes
    for (uint32_t i = 0; i < dummy_bytes; i++) {
        _spi.write(dummy_byte);
    }

    // Write Data
    for (uint32_t i = 0; i < data_size; i++) {
        _spi.write(data[i]);
    }
    _cs = 1;
}

int SPIFBlockDevice::_sync()
{
    for (int i = 0; i < SPIF_TIMEOUT; i++) {
        // Read status register until write not-in-progress
        uint8_t status;
        _cmdread(SPIF_RDSR, 0, 1, 0x0, &status);

        // Check WIP bit
        if (!(status & SPIF_WIP)) {
            return 0;
        }

        wait_ms(1);
    }

    return BD_ERROR_DEVICE_ERROR;
}

int SPIFBlockDevice::_wren()
{
    _cmdwrite(SPIF_WREN, 0, 0, 0x0, NULL);

    for (int i = 0; i < SPIF_TIMEOUT; i++) {
        // Read status register until write latch is enabled
        uint8_t status;
        _cmdread(SPIF_RDSR, 0, 1, 0x0, &status);

        // Check WEL bit
        if (status & SPIF_WEL) {
            return 0;
        }

        wait_ms(1);
    }

    return BD_ERROR_DEVICE_ERROR;
}


void SPIFBlockDevice::_spi_send_read_command(uint8_t read_inst, void *buffer, bd_addr_t addr, bd_size_t size)
{
	uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
	uint8_t dummy_byte = 0x00;
	uint8_t *addr_byte_ptr = addr;
	uint8_t *data = (uint8_t *)buffer;
	addr_byte_ptr += (_address_size-1);

	// Write 1 byte Instruction
	_spi.write(read_inst);

	// Write Address (can be either 3 or 4 bytes long)
	for (uint32_t i = 0; i < _address_size; i++) {
		_spi.write(*addr_byte_ptr);
		addr_byte_ptr--;
	}

	// Write Dummy Cycles Bytes
	for (uint32_t i = 0; i < dummy_bytes; i++) {
		_spi.write(dummy_byte);
	}

	// Read Data
	for (bd_size_t i = 0; i < size; i++) {
		data[i] = _spi.write(0);
	}

	return;
}

void SPIFBlockDevice::_spi_send_program_command(unsigned int prog_inst, const void *buffer, bd_addr_t addr, bd_size_t size)
{
    // Send Program (write) command to device driver
	uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
	uint8_t dummy_byte = 0x00;
	uint8_t *addr_byte_ptr = addr;
	uint8_t *data = (uint8_t *)buffer;
	addr_byte_ptr += (_address_size-1)

	// Write 1 byte Instruction
	_spi.write(prog_inst);

	// Write Address (can be either 3 or 4 bytes long)
	for (uint32_t i = 0; i < _address_size; i++) {
		_spi.write(*addr_byte_ptr);
		addr_byte_ptr--;
	}

	// Write Dummy Cycles Bytes
	for (uint32_t i = 0; i < dummy_bytes; i++) {
		_spi.write(dummy_byte);
	}

	// Write Data
	for (bd_size_t i = 0; i < size; i++) {
		_spi.write(data[i]);
	}

    return;
}

qspi_status_t SPIFBlockDevice::_spi_send_erase_command(unsigned int erase_inst, bd_addr_t addr, bd_size_t size)
{
	tr_info("INFO: Inst: 0x%xh, addr: %llu, size: %llu", erase_inst, addr, size);
	_spi_send_general_command(erase_inst, addr)
	return;
}

qspi_status_t SPIFBlockDevice::_spi_send_general_command(unsigned int instruction, bd_addr_t addr)
{
    // Send a general command Instruction to driver
	uint32_t dummy_bytes = _dummy_and_mode_cycles / 8;
	uint8_t dummy_byte = 0x00;
	uint8_t *addr_byte_ptr = (((int)addr) & 0x00FFF000);
	uint8_t *data = (uint8_t *)buffer;
	addr_byte_ptr += (_address_size-1)

	// Write 1 byte Instruction
	_spi.write(instruction);

	// Write Address (can be either 3 or 4 bytes long)
	for (uint32_t i = 0; i < _address_size; i++) {
		_spi.write(*addr_byte_ptr);
		addr_byte_ptr--;
	}

	// Write Dummy Cycles Bytes
	for (uint32_t i = 0; i < dummy_bytes; i++) {
		_spi.write(dummy_byte);
	}

	return;
}



int SPIFBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    int status = SPIF_BD_ERROR_OK;

    tr_info("INFO Inst: 0x%xh", _read_instruction);

    _mutex.lock();

    // Set Dummy Cycles for Specific Read Command Mode
    _dummy_and_mode_cycles = _read_dummy_and_mode_cycles;

    _spi_send_read_command(_read_instruction, buffer, addr, size);

    // Set Dummy Cycles for all other command modes
    _dummy_and_mode_cycles = 0;

    _mutex.unlock();
    return status;

}
 
int SPIFBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    qspi_status_t result = QSPI_STATUS_OK;
	bool program_failed = false;
	int status = SPIF_BD_ERROR_OK;
	uint32_t offset = 0;
	uint32_t chunk = 0;

	tr_debug("DEBUG: program - Buff: 0x%x, addr: %llu, size: %llu", buffer, addr, size);

	while (size > 0) {

		// Write on _page_size_bytes boundaries (Default 256 bytes a page)
		offset = addr % _page_size_bytes;
		chunk = (offset + size < _page_size_bytes) ? size : (_page_size_bytes - offset);

		_mutex.lock();

		//Send WREN
		if (_set_write_enable() != 0) {
			tr_error("ERROR: Write Enabe failed\n");
			program_failed = true;
			status = QSPIF_BD_ERROR_WREN_FAILED;
			goto exit_point;
		}

		_spi_send_program_command(_prog_instruction, buffer, addr, chunk);

		buffer = static_cast<const uint8_t *>(buffer) + chunk;
		addr += chunk;
		size -= chunk;

		if ( false == _is_mem_ready()) {
			tr_error("ERROR: Device not ready after write, failed\n");
			program_failed = true;
			status = QSPIF_BD_ERROR_READY_FAILED;
			goto exit_point;
		}
		_mutex.unlock();
	}

exit_point:
    if (program_failed) {
        _mutex.unlock();
    }

    return status;
}

int SPIFBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    // Check the address and size fit onto the chip.
    MBED_ASSERT(is_valid_erase(addr, size));

    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    int type = 0;
    uint32_t chunk = 4096;
    unsigned int cur_erase_inst = _erase_instruction;
    int size = (int)in_size;
    bool erase_failed = false;
    int status = QSPIF_BD_ERROR_OK;
    // Find region of erased address
    int region = _utils_find_addr_region(addr);
    // Erase Types of selected region
    uint8_t bitfield = _region_erase_types_bitfield[region];

    tr_debug("DEBUG: erase - addr: %llu, in_size: %llu", addr, in_size);

    // For each iteration erase the largest section supported by current region
    while (size > 0) {

        // iterate to find next Largest erase type ( a. supported by region, b. smaller than size)
        // find the matching instruction and erase size chunk for that type.
        type = _utils_iterate_next_largest_erase_type(bitfield, size, (int)addr, _region_high_boundary[region]);
        cur_erase_inst = _erase_type_inst_arr[type];
        chunk = _erase_type_size_arr[type];

        tr_debug("DEBUG: erase - addr: %llu, size:%d, Inst: 0x%xh, chunk: %d , ",
                 addr, size, cur_erase_inst, chunk);
        tr_debug("DEBUG: erase - Region: %d, Type:%d",
                 region, type);

        _mutex.lock();

        if (_set_write_enable() != 0) {
            tr_error("ERROR: QSPI Erase Device not ready - failed");
            erase_failed = true;
            status = QSPIF_BD_ERROR_READY_FAILED;
            goto exit_point;
        }

        _spi_send_erase_command(cur_erase_inst, addr, size);

        addr += chunk;
        size -= chunk;

        if ( (size > 0) && (addr > _region_high_boundary[region]) ) {
            // erase crossed to next region
            region++;
            bitfield = _region_erase_types_bitfield[region];
        }

        if ( false == _is_mem_ready()) {
            tr_error("ERROR: QSPI After Erase Device not ready - failed\n");
            erase_failed = true;
            status = QSPIF_BD_ERROR_READY_FAILED;
            goto exit_point;
        }

        _mutex.unlock();
    }

exit_point:
    if (erase_failed) {
        _mutex.unlock();
    }

    return status;
}

bd_size_t SPIFBlockDevice::get_read_size() const
{
    return SPIF_READ_SIZE;
}

bd_size_t SPIFBlockDevice::get_program_size() const
{
    return SPIF_PROG_SIZE;
}

bd_size_t SPIFBlockDevice::get_erase_size() const
{
    return SPIF_SE_SIZE;
}

// Find minimal erase size supported by the region to which the address belongs to
bd_size_t SPIFBlockDevice::get_erase_size(bd_addr_t addr)
{
    // Find region of current address
    int region = _utils_find_addr_region(addr);

    int min_region_erase_size = _min_common_erase_size;
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
            tr_error("ERROR: no erase type was found for region addr");
        }
    }

    return (bd_size_t)min_region_erase_size;
}



bd_size_t SPIFBlockDevice::size() const
{
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }

    return _size;
}

int SPIFBlockDevice::get_erase_value() const
{
    return 0xFF;
}
