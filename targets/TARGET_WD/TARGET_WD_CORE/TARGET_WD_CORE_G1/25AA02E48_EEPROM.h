/*
 * 25AA02E48_EEPROM.h
 *
 * Created: 05.05.2015 09:31:06
 *  Author: patrick.frischmann
 *
 * =========================================================
 * Serial Flash Memory Block Diagram
 * ------------------------------
 * |						00h |
 * |							|
 * |							|
 * |	  Standard EEPROM		|
 * |							|
 * |							|
 * |						C0h |
 * ------------------------------
 * |	  Write-protected		|
 * |	    EUI-48 Block		|	
 * |						FFh |
 * ------------------------------
 *
 *
 * MAC address range:
 *	Company
 *		Microchip Technology Inc.
 *	Address
 *		Chandler AZ 85224
 *		M/S 4-2-A
 *		UNITED STATES
 *	Range
 *		D8:80:39:00:00:00 - D8:80:39:FF:FF:FF
 *	Type
 *		IEEE MA-L
 * =========================================================
 */ 


#ifndef __25AA02E48_EEPROM_H_
#define __25AA02E48_EEPROM_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
	 
/** @brief	Read data from eeprom memory.
 *
 * @param	address: 8-bit start address of memory region.
 * @param	buffer: Pointer to buffer where retrieved data should be stored.
 * @param	length: Length of data to read in bytes.
 *
 * @retval	0 if okay, -1 if failed
 */
int eeprom_read(uint8_t address, uint8_t *buffer, uint8_t length);

/** @brief	Read unique EUI-48 node address from eeprom memory.
 *
 * @param	buffer: Pointer to buffer where retrieved data should be stored.
 *
 * @retval	0 if okay, -1 if failed
 */
int eeprom_read_eui48_node_address(uint8_t *buffer);

/** @brief	Read unique EUI-64 node address from eeprom memory.
 *
 * @param	buffer: Pointer to buffer where retrieved data should be stored.
 *
 * @retval	0 if okay, -1 if failed
 */
int eeprom_read_eui64_node_address(uint8_t *buffer);
	 
#ifdef __cplusplus
}
#endif

#endif /* __25AA02E48_EEPROM_H_ */