/*
 * W25X40CL.c
 *
 * Created: 14.07.2015 15:58:19
 *  Author: patrick.frischmann
 *
 * =========================================================
 * Serial Flash Memory Block Diagram
 * ------------------------------
 * | 07FF00h			07FFFFh |
 * |							|	Block 7 (64k)
 * | 070000h			0700FFh |
 * ------------------------------
 *				...
 *				...
 * ------------------------------
 * | 00FF00h			00FFFFh |
 * |							|	Block 0 (64k)
 * | 000000h			0000FFh |
 * ------------------------------
 *
 * Each Block is divided into 16 sectors of 4k memory each.
 * ------------------------------
 * | xxFF00h			xxFFFFh |
 * |							|	Sector 15 (4k)
 * | xxF000h			xxF0FFh |
 * ------------------------------
 *				...
 *				...
 * ------------------------------
 * | xx0F00h			xx0FFFh |
 * |							|	Sector 0 (4k)
 * | xx0000h			xx00FFh |
 * ------------------------------
 *
 * Page size is 256 bytes.
 * =========================================================
 */ 


#ifndef __W25X40CL_H_
#define __W25X40CL_H_

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stdint.h"
#include "PinNames.h"
#include "mc_res.h"

#define FLASH_SECTOR_SIZE_BYTES		4096
#define FLASH_SECTOR_COUNT			100

/** @brief Initialize flash device.
 */
MC_RES flash_init(PinName cs, PinName mosi, PinName miso, PinName sck);

/** @brief Write data to flash memory.
 *
 * @param address 24-bit start address of memory region.
 * @param data Pointer to data that should be written.
 * @param length Length of data to write in bytes.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_write(uint32_t address, uint8_t *data, size_t length);

/** @brief Read data from flash memory.
 *
 * @param address 24-bit start address of memory region.
 * @param buffer Pointer to buffer where retrieved data should be stored.
 * @param length Length of data to read in bytes.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_read(uint32_t address, uint8_t *buffer, size_t length);

/** @brief Read device id.
 *
 * @param buffer Pointer to buffer where retrieved data should be stored.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_read_device_id(uint8_t *buffer);

/** @brief Read unique 64-bit id.
 *
 * @param buffer Pointer to buffer where retrieved data should be stored.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_read_unique_id(uint64_t *buffer);

/** @brief Read JEDEC ID (0x00|Manufacturer ID|Memory Type ID|Capacity ID).
 *
 * @param buffer Pointer to buffer where retrieved data should be stored.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_read_jedec_id(uint32_t *buffer);

/** @brief Erase sector (4k). Sets all memory within sector to erased state of all 1s.
 *
 * @param address 24-bit address of memory region.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_erase_sector(uint32_t address);

/** @brief Erase block (32k). Sets all memory within a specified block to erased state of all 1s.
 *
 * @param address 24-bit address of memory region.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_erase_block_32k(uint32_t address);

/** @brief Erase block (64k). Sets all memory within a specified block to erased state of all 1s.
 *
 * @param address 24-bit address of memory region.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_erase_block_64k(uint32_t address);

/** @brief Erase entire chip. Sets all memory within the device to erased state of all 1s.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_erase_chip(void);

/** @brief Power down chip to reduce power consumption. Only release from power down will restore the device to normal operation.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_power_down(void);

/** @brief Release the device from power down state.
 *
 * @retval MC_RES_OK if okay, MC_RES_ERROR if failed
 */
MC_RES flash_release_power_down(void);

#ifdef __cplusplus
}
#endif

#endif /* __W25X40CL_H_ */