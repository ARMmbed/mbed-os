#include "OneWire.h"
#include "platform/critical.h"
#include "wd_logging.h"

#define OW_DELAY_US(value) wait_us(value)

OneWire::OneWire(PinName pinRx, PinName pinTx, PinName pinTxH)
	: _pinRx(pinRx, PIN_INPUT, PullNone, 0), _pinTx(pinTx, PIN_OUTPUT, OpenDrain, 0), _pinTxH(pinTxH, PIN_OUTPUT, OpenDrain, 1) {}

OneWire::~OneWire() {}

void OneWire::ow_write_bit(bool b) {
	
	this->ow_bus_low();
	
	if (b) {
		OW_DELAY_US(6);
		this->ow_bus_high();
		OW_DELAY_US(64);
	} else {
		OW_DELAY_US(60);
		this->ow_bus_high();
		OW_DELAY_US(10);
	}
	
}

bool OneWire::ow_read_bit(void) {
	
	bool response;
	
	this->ow_bus_low();
	OW_DELAY_US(6);
	this->ow_bus_high();
	OW_DELAY_US(9);
	response = _pinRx.read();
	OW_DELAY_US(55);
	
	return response;
	
}

char OneWire::ow_crc_8(char * data, size_t length) {
	
	char crc = 0x00;
	
	while (length--) {
		char inbyte = *data++;
		for (int i = 8; i; i--) {
			char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	
	return crc;
	
}

OW_STATUS_CODE OneWire::ow_reset(void) {
	/*
		Drive bus low, delay 480us
		Release bus, delay 70us
		Sample bus, 0 = device(s) present, 1 = no device present
		Delay 410us
	*/
	
	bool ow_device_present = false;
	
	core_util_critical_section_enter();
	
	this->ow_bus_low();
	OW_DELAY_US(480);
	this->ow_bus_high();
	
	OW_DELAY_US(70);
	
	if (_pinRx.read() == 0)
		ow_device_present = true;
	
	OW_DELAY_US(410);
	
	core_util_critical_section_exit();
	
	return ow_device_present ? OW_OK : OW_ERROR_CRC;
	
}

OW_STATUS_CODE OneWire::ow_write_byte(char data) {
	
	core_util_critical_section_enter();
    for (int i = 0; i < 8; i++) {
        this->ow_write_bit((data >> i) & 0x01);
    }
	core_util_critical_section_exit();
	
	return OW_OK;
	
}

OW_STATUS_CODE OneWire::ow_read_byte(char * buffer) {
	
	*buffer = 0x00;
	
	core_util_critical_section_enter();
	for (int i = 0; i < 8; i++) {
		bool bit = this->ow_read_bit();
		if(bit) (*buffer) |= (0x01 << i);
	}
	core_util_critical_section_exit();
	
	return OW_OK;
	
}

OW_STATUS_CODE OneWire::ow_read_bytes_with_crc_8(char * buffer, size_t length) {
	
	for(int i = 0; i < length; i++){
		this->ow_read_byte(buffer + i);
	}
	
	char crcRead;
	this->ow_read_byte(&crcRead);
	char crcCalculated = this->ow_crc_8(buffer, length);
	
	return crcRead == crcCalculated ? OW_OK : OW_ERROR_CRC;
	
}

OW_STATUS_CODE OneWire::ow_command(char command, char id[]) {
	
	if (id) { // write to specific device
		this->ow_write_byte(OW_MATCH_ROM);
		int i = OW_ROMCODE_SIZE;	
		do {
			this->ow_write_byte(*id);
			++id;
		} while (--i);
	} else { // write to all devices
		this->ow_write_byte(OW_SKIP_ROM);
	}
	
	this->ow_write_byte(command);
	
	return OW_OK;
	
}

char OneWire::ow_rom_search(char diff, char id[]) {
    char i, j, next_diff;
    char b;

    if (this->ow_reset() != OW_OK) {
        return OW_SEARCH_STATE_PRESENCE_ERR;					// error, no device found
	} else {
		
		this->ow_write_byte(OW_SEARCH_ROM);		// ROM search command
	    
		next_diff = OW_SEARCH_STATE_LAST_DEVICE;	// unchanged on last device
		i = OW_ROMCODE_SIZE * 8;                // bitwise iteration
    
		do {
			j = 8;                              // 8 bits
			do {
				b = this->ow_read_bit();        // read bit
				if (this->ow_read_bit()) {      // read complement bit; !b = 1
					if (b)						// 11
						return OW_SEARCH_STATE_DATA_ERR;	
				} else {						// !b = 0
					if (!b) {                   // 00 = 2 devices
						if ( diff > i || ((*id & 1) && diff != i) ) {
							b = 1;              // now go with 1
							next_diff = i;      // next iteration pass 0
						}
					}
				}
				this->ow_write_bit(b);			// write bit
				*id >>= 1;
				if (b) *id |= 0x80;				// store bit
				--i;
			} while ( --j );
			id++;								// next byte
		} while (i);
		
		return next_diff;						// to continue search
		
	}
}

OW_STATUS_CODE OneWire::ow_find_sensor(char *diff, char id[], char familyCode) {
	
	for (;;) // skip unwanted sensors
    {
        *diff = this->ow_rom_search(*diff, &id[0]);
        if ( *diff == OW_SEARCH_STATE_PRESENCE_ERR)
            return OW_ERROR;
        if ( *diff == OW_SEARCH_STATE_DATA_ERR )
            return OW_ERROR;
        if ( *diff == OW_SEARCH_STATE_LAST_DEVICE )
            return OW_OK ;
        if ( id[0] == familyCode ) 
            return OW_OK ;
    }
	
}

OW_STATUS_CODE OneWire::ow_search_sensors(int *nSensors, char *gSensorIDs, char familyCode) {
	
    char id[OW_ROMCODE_SIZE] = {};
    char diff;
    
	wd_log_debug("Scanning 1-Wire bus for devices");
	
    diff = OW_SEARCH_STATE_SEARCH_FIRST;
    
	for (*nSensors = 0; (diff != OW_SEARCH_STATE_LAST_DEVICE) && (*nSensors < OW_MAXSENSORS); ++(*nSensors)) {
        
		if (this->ow_find_sensor(&diff, &id[0], familyCode) != OW_OK) {
			wd_log_debug("An error occurred during search: %x", diff);
			return OW_ERROR;
		}

        for (int i=0; i<OW_ROMCODE_SIZE; i++)
            gSensorIDs[(*nSensors) * OW_ROMCODE_SIZE + i] = id[i];

    }
	
    return OW_OK;
	
}

OW_STATUS_CODE OneWire::ow_show_id(char id[], size_t n, char * text) {
	
	char hex[4];
	
	for (int i = 0; i < n; i++) {
        if ( i == 0 ) strcat(text, " FC: ");
        else if ( i == n-1 ) strcat(text, "CRC: ");
        if ( i == 1 ) strcat(text, " SN: ");
        sprintf(hex,"%2.2X ", id[i]);
        strcat(text, hex);
    }
    return OW_OK;
	
}
