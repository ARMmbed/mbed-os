/*
 * W25X40CL.c
 *
 * Created: 14.07.2015 15:58:19
 *  Author: patrick.frischmann
 */ 

#include "W25X40CL.h"
#include "hal/spi_api.h"
#include "hal/gpio_api.h"
#include "platform/wait_api.h"

// INSTRUCTION COMMANDS
#define CMD_WRITE_ENABLE								0x06
#define CMD_WRITE_ENABLE_VOLATILE_STATUS_REGISTER		0x50
#define CMD_WRITE_DISABLE								0x04
#define CMD_READ_STATUS_REGISTER						0x05
#define CMD_WRITE_STATUS_REGISTER						0x01
#define CMD_READ_DATA									0x03
#define CMD_FAST_READ									0x0B
#define CMD_FAST_READ_DUAL_OUTPUT						0x3B
#define CMD_FAST_READ_DUAL_IO							0xBB
#define CMD_PAGE_PROGRAM								0x02
#define CMD_SECTOR_ERASE_4KB							0x20
#define CMD_BLOCK_ERASE_32KB							0x52
#define CMD_BLOCK_ERASE_64KB							0xD8
#define CMD_CHIP_ERASE									0xC7
#define CMD_POWER_DOWN									0xB9
#define CMD_RELEASE_POWER_DOWN							0xAB
#define CMD_READ_MANUFACTURER_ID						0x90
#define CMD_READ_MANUFACTURER_ID_DUAL_IO				0x92
#define CMD_READ_JEDEC_ID								0x9F
#define CMD_READ_UNIQUE_ID								0x4B

// STATUS REGISTER MASKS
#define STATUS_REGISTER_BUSY_MASK						0x01
#define STATUS_REGISTER_WRITE_ENABLE_MASK				0x02
#define STATUS_REGISTER_BLOCK_PROTECT_MASK				0x1C
#define STATUS_REGISTER_TOPBOTTOM_PROTECT_MASK			0x20
#define STATUS_REGISTER_PROTECT_MASK					0x80

// BLOCK PROTECTION MASKS
#define BLOCK_PROTECTION_BIT_2_MASK						0x10

// BLOCK PROTECTION STATES
#define BLOCK_PROTECTION_NONE							0x00
#define BLOCK_PROTECTION_7								0x04
#define BLOCK_PROTECTION_6_AND_7						0x08
#define BLOCK_PROTECTION_4_THRU_7						0x0C
#define BLOCK_PROTECTION_0								0x24
#define BLOCK_PROTECTION_0_AND_1						0x28
#define BLOCK_PROTECTION_0_THRU_3						0x2C
#define BLOCK_PROTECTION_0_THRU_7						0x10

/** Time-out value (number of attempts). */
#define FLASH_TIMEOUT			15000
#define FLASH_PAGESIZE			256
#define DUMMY_DATA				0xFF

static gpio_t _cs;
static spi_t _spi;

MC_RES flash_init(PinName cs, PinName mosi, PinName miso, PinName sck) {
	gpio_init_out_ex(&_cs, cs, 1);
	spi_init(&_spi, mosi, miso, sck, NC);
	flash_release_power_down();
	return MC_RES_OK;
}

static void select_chip(void) {
	gpio_write(&_cs, 0);
}

static void deselect_chip(void) {
	gpio_write(&_cs, 1);
}

static MC_RES read_bytes(uint8_t *buffer, size_t length) {
	
	for (int i = 0; i < length; i++) {
		int ret = spi_master_write(&_spi, DUMMY_DATA);
		if (ret < 0) return MC_RES_ERROR;
		*(buffer + i) = ret;
	}
	return MC_RES_OK;
}

static MC_RES write_bytes(uint8_t *buffer, size_t length) {
	
	uint32_t i = 0;
	uint8_t val;
	
	while (length) {
		val = buffer[i];
		MC_RETURN_ON_ERROR(spi_master_write(&_spi, val));
		i++;
		length--;
	}
	
	return MC_RES_OK;
}

static MC_RES read_status_register(uint8_t *buf) {
	select_chip();
	MC_RETURN_ON_ERROR(spi_master_write(&_spi, CMD_READ_STATUS_REGISTER));
	MC_RETURN_ON_ERROR(read_bytes(buf, 1));
	deselect_chip();
	return MC_RES_OK;
}

static uint8_t is_busy(void){
	uint8_t buf;
	read_status_register(&buf);
	return (buf & STATUS_REGISTER_BUSY_MASK);
}

static uint8_t is_write_enabled(void) {
	uint8_t buf;
	read_status_register(&buf);
	return (buf & STATUS_REGISTER_WRITE_ENABLE_MASK);
}

static uint8_t is_any_memory_protected(void) {
	uint8_t buf;
	read_status_register(&buf);
	return ((buf & STATUS_REGISTER_BLOCK_PROTECT_MASK) != BLOCK_PROTECTION_NONE);
}

static uint8_t are_memory_regions_overlapping(uint32_t address, uint16_t length, uint32_t mem_addr_start, uint32_t mem_addr_end) {
	if (((address + (length - 1)) < mem_addr_start) || (address > mem_addr_end)) // outside boundaries
		return 0;
	return 1;
}

static uint8_t is_memory_region_protected(uint32_t address, uint16_t length) {
	uint8_t buf;
	read_status_register(&buf);
	
	if ((buf & STATUS_REGISTER_BLOCK_PROTECT_MASK) == BLOCK_PROTECTION_NONE) 
		return 0;
	
	if ((buf & BLOCK_PROTECTION_BIT_2_MASK) == BLOCK_PROTECTION_0_THRU_7)
		return are_memory_regions_overlapping(address, length, 0x00000000, 0x0007FFFF);
	
	switch (buf & (STATUS_REGISTER_BLOCK_PROTECT_MASK | STATUS_REGISTER_TOPBOTTOM_PROTECT_MASK))
	{
		case BLOCK_PROTECTION_7: return are_memory_regions_overlapping(address, length, 0x00070000, 0x0007FFFF);
		case BLOCK_PROTECTION_6_AND_7: return are_memory_regions_overlapping(address, length, 0x00060000, 0x0007FFFF);
		case BLOCK_PROTECTION_4_THRU_7: return are_memory_regions_overlapping(address, length, 0x00040000, 0x0007FFFF);
		case BLOCK_PROTECTION_0: return are_memory_regions_overlapping(address, length, 0x00000000, 0x0000FFFF);
		case BLOCK_PROTECTION_0_AND_1: return are_memory_regions_overlapping(address, length, 0x000700000, 0x0001FFFF);
		case BLOCK_PROTECTION_0_THRU_3: return are_memory_regions_overlapping(address, length, 0x00000000, 0x0003FFFF);
	}
	
	return 0;
}

static int flash_enable_write(void){
	select_chip();
	MC_RETURN_ON_ERROR(spi_master_write(&_spi, CMD_WRITE_ENABLE));
	deselect_chip();
	return MC_RES_OK;
}

static int flash_write_page(uint32_t address, uint8_t *data, size_t length){
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	if (!is_write_enabled()){
		MC_RETURN_ON_ERROR(flash_enable_write());
	}
		
	// if whole page is to be written, set last address byte to 0 to prevent number of clocks exceeding the remaining page length
	if (length == 256) address &= 0xFFFFFF00;
	
	uint8_t instruction_data[4] = { CMD_PAGE_PROGRAM, (uint8_t)(address>>16), (uint8_t)(address>>8), (uint8_t)(address)};
	select_chip();
	MC_RETURN_ON_ERROR(write_bytes(instruction_data, sizeof(instruction_data)));
	MC_RETURN_ON_ERROR(write_bytes(data, length));
	deselect_chip();
	
	return MC_RES_OK;
}

MC_RES flash_write(uint32_t address, uint8_t *data, size_t length) {
	if (is_memory_region_protected(address, length)) return MC_RES_ERROR;
	
	uint8_t* internal_data = data;
	
	size_t remaining_page_space;
	size_t current_page_size = 0;
	size_t bytes_remaining = length;
	
	while (bytes_remaining > 0)
	{
		remaining_page_space = FLASH_PAGESIZE - (address & 0x000000FF);
		
		if (bytes_remaining > remaining_page_space){
			current_page_size = remaining_page_space;
		} else {
			current_page_size = bytes_remaining;
		}
		
		MC_RETURN_ON_ERROR(flash_write_page(address, internal_data, current_page_size));
		
		internal_data += current_page_size;
		address += current_page_size;
		
		bytes_remaining -= current_page_size;
		
	}
	return MC_RES_OK;
}

MC_RES flash_read(uint32_t address, uint8_t *buffer, size_t length) {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	uint8_t instruction_data[4] = { CMD_READ_DATA, (uint8_t)(address>>16), (uint8_t)(address>>8), (uint8_t)(address)};
	select_chip();
	MC_RETURN_ON_ERROR(write_bytes(instruction_data, sizeof(instruction_data)));
	MC_RETURN_ON_ERROR(read_bytes(buffer, length));
	deselect_chip();
	return MC_RES_OK;
}

MC_RES flash_read_device_id(uint8_t *buffer) {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	uint8_t tmp[2];
	uint8_t instruction_data[4] = { CMD_READ_MANUFACTURER_ID, 0x00, 0x00, 0x00};
	select_chip();
	MC_RETURN_ON_ERROR(write_bytes(instruction_data, sizeof(instruction_data)));
	MC_RETURN_ON_ERROR(read_bytes(tmp, sizeof(tmp)));
	deselect_chip();
	*buffer = tmp[1];
	return MC_RES_OK;
}

MC_RES flash_read_unique_id(uint64_t *buffer) {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	uint8_t tmp[8];
	uint8_t instruction_data[5] = { CMD_READ_UNIQUE_ID, 0x00, 0x00, 0x00, 0x00};
	select_chip();
	MC_RETURN_ON_ERROR(write_bytes(instruction_data, sizeof(instruction_data)));
	MC_RETURN_ON_ERROR(read_bytes(tmp, sizeof(tmp)));
	deselect_chip();
	
	*buffer =	(((uint64_t)tmp[0]) << 56) | 
				(((uint64_t)tmp[1]) << 48) | 
				(((uint64_t)tmp[2]) << 40) |
				(((uint64_t)tmp[3]) << 32) |
				(((uint64_t)tmp[4]) << 24) |
				(((uint64_t)tmp[5]) << 16) |
				(((uint64_t)tmp[6]) << 8) |
				((uint64_t)tmp[7]);
				
	return MC_RES_OK;
}

MC_RES flash_read_jedec_id(uint32_t *buffer) {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	uint8_t tmp[3];
	select_chip();
	MC_RETURN_ON_ERROR(spi_master_write(&_spi, CMD_READ_JEDEC_ID));
	MC_RETURN_ON_ERROR(read_bytes(tmp, sizeof(tmp)));
	deselect_chip();
	
	*buffer =	(((uint32_t)tmp[0]) << 16) |
				(((uint32_t)tmp[1]) << 8) |
				((uint32_t)tmp[2]);
	
	return MC_RES_OK;
}

MC_RES flash_erase_sector(uint32_t address) {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	if (!is_write_enabled()){
		MC_RETURN_ON_ERROR(flash_enable_write());
	}
	
	// ensure correct sector start address
	if ((address & 0x00000FFF) != 0x0000)
	{
		return MC_RES_ERROR;
	}
	
	if (is_memory_region_protected(address, 0x1000))
	{
		return MC_RES_ERROR;
	}
	
	uint8_t instruction_data[4] = { CMD_SECTOR_ERASE_4KB, (uint8_t)(address>>16), (uint8_t)(address>>8), (uint8_t)(address)};
	select_chip();
	MC_RETURN_ON_ERROR(write_bytes(instruction_data, sizeof(instruction_data)));
	deselect_chip();
	
	return MC_RES_OK;
}

MC_RES flash_erase_block_32k(uint32_t address) {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	if (!is_write_enabled()){
		MC_RETURN_ON_ERROR(flash_enable_write());
	}
	
	// ensure correct 32k block start address (-> xx0000 || xx8000)
	if (!(((address & 0x0000FFFF) == 0x0000) || ((address & 0x0000FFFF) == 0x8000)))
	{
		return MC_RES_ERROR;
	}
	
	if (is_memory_region_protected(address, 0x8000))
	{
		return MC_RES_ERROR;
	}
	
	uint8_t instruction_data[4] = { CMD_BLOCK_ERASE_32KB, (uint8_t)(address>>16), (uint8_t)(address>>8), (uint8_t)(address)};
	select_chip();
	MC_RETURN_ON_ERROR(write_bytes(instruction_data, sizeof(instruction_data)));
	deselect_chip();
	return MC_RES_OK;
}

MC_RES flash_erase_block_64k(uint32_t address) {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	if (!is_write_enabled()){
		MC_RETURN_ON_ERROR(flash_enable_write());
	}
	
	// ensure correct 64k block start address xx0000
	if ((address & 0x0000FFFF) != 0x0000)
	{
		return MC_RES_ERROR;
	}
	
	if (is_memory_region_protected(address, 0xFFFF))
	{
		return MC_RES_ERROR;
	}
	
	uint8_t instruction_data[4] = { CMD_BLOCK_ERASE_64KB, (uint8_t)(address>>16), (uint8_t)(address>>8), (uint8_t)(address)};
	select_chip();
	MC_RETURN_ON_ERROR(write_bytes(instruction_data, sizeof(instruction_data)));
	deselect_chip();
	return MC_RES_OK;
}

MC_RES flash_erase_chip() {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	if (!is_write_enabled()){
		MC_RETURN_ON_ERROR(flash_enable_write());
	}
	
	if (is_any_memory_protected())
	{
		return MC_RES_ERROR;
	}
	
	select_chip();
	MC_RETURN_ON_ERROR(spi_master_write(&_spi, CMD_CHIP_ERASE));
	deselect_chip();
	
	return MC_RES_OK;
}

MC_RES flash_power_down() {
	uint32_t timeout = FLASH_TIMEOUT;
	
	while (is_busy()) {
		if (!timeout--) {
			return MC_RES_ERROR;
		}
	}
	
	select_chip();
	MC_RETURN_ON_ERROR(spi_master_write(&_spi, CMD_POWER_DOWN));
	deselect_chip();
	return MC_RES_OK;
}

MC_RES flash_release_power_down() {
	select_chip();
	MC_RETURN_ON_ERROR(spi_master_write(&_spi, CMD_RELEASE_POWER_DOWN));
	// chip select must remain high for at least 3us, so we better be save and wait here
	wait_us(3); 
	deselect_chip();
	return MC_RES_OK;
}

