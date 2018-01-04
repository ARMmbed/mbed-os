#pragma once

#include "mbed.h"
#include "PinNames.h"
#include <string>

enum OW_STATUS_CODE {
	OW_ERROR = 0,
	OW_OK = 1,
	OW_ERROR_NO_DEVICE,
	OW_ERROR_CRC,
	OW_ERROR_BAD_ID,
	OW_BUSY
};

#define OW_MAXSENSORS				8
#define OW_ROMCODE_SIZE				8	// rom-code size including CRC

#define OW_MATCH_ROM				0x55
#define OW_SKIP_ROM					0xCC
#define OW_SEARCH_ROM				0xF0
#define OW_READ_ROM					0x33
#define OW_CONDITIONAL_SEARCH		0xEC
#define OW_OVERDRIVE_SKIP_ROM		0x3C
#define OW_OVERDRIVE_MATCH_ROM		0x69

#define OW_SEARCH_STATE_LAST_DEVICE		0x00	// last device found
#define OW_SEARCH_STATE_PRESENCE_ERR	0x01
#define OW_SEARCH_STATE_DATA_ERR		0xFE
#define OW_SEARCH_STATE_SEARCH_FIRST	0xFF	// start new search

class OneWire {
public:
	OneWire(PinName pinRx, PinName pinTx, PinName pinTxH = NC);
	~OneWire();
	
	void ow_enable_strong_pullup(void) { return _pinTxH.write(0); };
	void ow_disable_strong_pullup(void) { return _pinTxH.write(1); };
	
	/** @brief Reset the 1-Wire bus slave devices and ready them for a command.
	 *
	 *	@retval 1 if device(s) present, 0 otherwise.
	 */
	OW_STATUS_CODE ow_reset(void);
	
	
	/** @brief Write a byte to the 1-Wire bus.
	 *
	 * @param data Data to write.
	 * 
	 * @return OW status code
	 *  @retval OW_OK Operation successful.
	 */
	OW_STATUS_CODE ow_write_byte(char data);
	
	
	/** @brief Read a byte from the 1-Wire bus.
	 *
	 * @param buffer Buffer that will hold the result.
	 * 
	 * @return OW status code
	 *  @retval OW_OK Operation successful.
	 */
	OW_STATUS_CODE ow_read_byte(char * buffer);
	
	
	/** @brief Read byte(s) from the 1-Wire bus with checksum validation.
	 *
	 * @param buffer Buffer that will hold the result.
	 * @param length Number of bytes to read.
	 * 
	 * @return OW status code
	 *  @retval OW_OK Operation successful.
	 *  @retval OW_ERROR_CRC Checksum validation failed.
	 */
	OW_STATUS_CODE ow_read_bytes_with_crc_8(char * buffer, size_t length);
	
	
	/** @brief Issue command to 1-Wire bus slave(s).
	 *
	 * @param command Command code.
	 * @param id ROM Id of device (if not set issue command to all devices on bus).
	 * 
	 * @return OW status code
	 *  @retval OW_OK Operation successful.
	 */
	OW_STATUS_CODE ow_command(char command, char id[]);
	
	
	OW_STATUS_CODE ow_find_sensor(char *diff, char id[], char familyCode);
	OW_STATUS_CODE ow_search_sensors(int *nSensors, char *gSensorIDs, char familyCode);

	OW_STATUS_CODE ow_show_id(char id[], size_t n, char * text);
	
	
private:
	
	void ow_bus_low(void) { return _pinTx.write(0); };
	void ow_bus_high(void) { return _pinTx.write(1); };
	
	void ow_write_bit(bool b);
	bool ow_read_bit(void);
	
	char ow_crc_8(char * data, size_t length);
	
	char ow_rom_search(char diff, char id[]);
	
	DigitalInOut _pinRx;
	DigitalInOut _pinTx;
	DigitalInOut _pinTxH;
};

