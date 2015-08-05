/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "i2c_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "SMM_MPS2.h"
#include "wait_api.h"
#include "fpga.h"

// Types
#undef FALSE
#undef TRUE
#define FALSE   0
#define TRUE    1

// TSC I2C controller
#define TSC_I2C_ADDR          0x82
// AACI I2C controller I2C address
#define AAIC_I2C_ADDR          0x96

#define TSC_I2C_CID           0x0811

// TSC I2C controller registers
#define TSC_I2C_CRID          0x00


// TSSPCPSR Clock prescale register
#define TSSPCPSR_DFLT      0x0002      // Clock prescale (use with SCR)

// TSC defaults
#define TSC_XOFF           20          // X offset
#define TSC_YOFF           20          // Y offset
#define TSC_MAXVAL         37000       // 0x0FFF * 10 with TSC to LCD scaling

#define TSC_TSU            15          // Setup delay 600nS min
#define AAIC_TSU            25           // Setup delay 1000nS min


static const PinMap PinMap_I2C_SDA[] = {
    {TSC_SDA, I2C_0, 0},
    {AUD_SDA, I2C_1, 0},
//    {EXP_SDA, I2C_2, 0},		//only used in extended version
    {NC   , NC   , 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {TSC_SCL, I2C_0, 0},
    {AUD_SCL, I2C_1, 0},
//    {EXP_SCL, I2C_2, 0},		//only used in extended version
    {NC   , NC,    0}
};

static inline void i2c_send_byte(i2c_t *obj, unsigned char c)
{
    int loop;
		switch ((int)obj->i2c) {
			case I2C_0: 
        obj->i2c->CONTROLC = SCL;
        i2c_delay(TSC_TSU);
    
        for (loop = 0; loop < 8; loop++)
        {
            if (c & (1 << (7 - loop)))
                obj->i2c->CONTROLS = SDA;
            else
                obj->i2c->CONTROLC = SDA;
            i2c_delay(TSC_TSU);
            obj->i2c->CONTROLS = SCL;
            i2c_delay(TSC_TSU);
            obj->i2c->CONTROLC = SCL;
            i2c_delay(TSC_TSU);
        }
    
        obj->i2c->CONTROLS = SDA;
        i2c_delay(TSC_TSU);
        break;
      case I2C_1:
        for (loop = 0; loop < 8; loop++) {
            i2c_delay(AAIC_TSU);
            obj->i2c->CONTROLC = SCL;
            i2c_delay(AAIC_TSU);
            if (c & (1 << (7 - loop)))
                obj->i2c->CONTROLS = SDA;
            else
                obj->i2c->CONTROLC = SDA;
            i2c_delay(AAIC_TSU);
            obj->i2c->CONTROLS = SCL;
            i2c_delay(AAIC_TSU);
            obj->i2c->CONTROLC = SCL;
        }
     
        i2c_delay(AAIC_TSU);
        obj->i2c->CONTROLS = SDA;
        i2c_delay(AAIC_TSU);
				break;
    }
}

static inline unsigned char i2c_receive_byte(i2c_t *obj)
{
		int data_receive_byte, loop;
		switch ((int)obj->i2c) {
			case I2C_0: 
        obj->i2c->CONTROLS = SDA;
        i2c_delay(TSC_TSU);
    
        data_receive_byte = 0;
    
        for (loop = 0; loop < 8; loop++)
        {
            obj->i2c->CONTROLS = SCL;
            i2c_delay(TSC_TSU);
            if ((obj->i2c->CONTROL & SDA))
            	data_receive_byte += (1 << (7 - loop));
            obj->i2c->CONTROLC = SCL;
            i2c_delay(TSC_TSU);
        }
    
        obj->i2c->CONTROLC = SDA;
        i2c_delay(TSC_TSU);
        break;
      case I2C_1:
        obj->i2c->CONTROLS = SDA;
        data_receive_byte = 0;
    
        for (loop = 0; loop < 8; loop++) {
            i2c_delay(AAIC_TSU);
            obj->i2c->CONTROLC = SCL;
            i2c_delay(AAIC_TSU);
            obj->i2c->CONTROLS = SCL | SDA;
            i2c_delay(AAIC_TSU);
            if ((obj->i2c->CONTROL & SDA))
            	data_receive_byte += (1 << (7 - loop));
            i2c_delay(AAIC_TSU);
            obj->i2c->CONTROLC = SCL;
        }
    
        i2c_delay(AAIC_TSU);
        obj->i2c->CONTROLC = SDA;
        i2c_delay(AAIC_TSU);
    		break;
    }
    return data_receive_byte;
}

static inline int i2c_receive_ack(i2c_t *obj)
{
    int nack;
		int delay_value;
		switch ((int)obj->i2c) {
			case I2C_0: delay_value = TSC_TSU; break;
      case I2C_1: delay_value = AAIC_TSU; break;
		}

    i2c_delay(delay_value);
    obj->i2c->CONTROLS = SDA;
    i2c_delay(delay_value);
    obj->i2c->CONTROLC = SCL;
    i2c_delay(delay_value);
    obj->i2c->CONTROLS = SCL;
    i2c_delay(delay_value);
    nack = obj->i2c->CONTROL & SDA;
    i2c_delay(delay_value);
    obj->i2c->CONTROLC = SCL;
    i2c_delay(delay_value);
    obj->i2c->CONTROLS = SDA;
    i2c_delay(delay_value);
    if(nack==0)
        return 1;

    return 0;
}


static inline void i2c_send_ack(i2c_t *obj) 
{
		int delay_value;
		switch ((int)obj->i2c) {
			case I2C_0: delay_value = TSC_TSU; break;
      case I2C_1: delay_value = AAIC_TSU; break;
		}

    i2c_delay(delay_value);
    obj->i2c->CONTROLC = SCL;
    i2c_delay(delay_value);
    obj->i2c->CONTROLS = SDA;
    i2c_delay(delay_value);
    obj->i2c->CONTROLS = SCL;
    i2c_delay(delay_value);
    obj->i2c->CONTROLC = SCL;
    i2c_delay(delay_value);
    obj->i2c->CONTROLC = SDA;
    i2c_delay(delay_value);

}


void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
	

    // determine the SPI to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (MPS2_I2C_TypeDef *)pinmap_merge(i2c_sda, i2c_scl);
    
    if ((int)obj->i2c == NC) {
        error("I2C pin mapping failed");
    }
		
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
}

int i2c_start(i2c_t *obj) {

		int delay_value;
		switch ((int)obj->i2c) {
			case I2C_0: delay_value = TSC_TSU; break;
      case I2C_1: delay_value = AAIC_TSU; break;
		}
				i2c_delay(delay_value);
				obj->i2c->CONTROLS = SDA | SCL;
				i2c_delay(delay_value);
				obj->i2c->CONTROLC = SDA;
				i2c_delay(delay_value);



    return 0;
}

int i2c_start_tsc(i2c_t *obj) {

		int delay_value;
		switch ((int)obj->i2c) {
			case I2C_0: delay_value = TSC_TSU; break;
      case I2C_1: delay_value = AAIC_TSU; break;
		}
				i2c_delay(delay_value);
				obj->i2c->CONTROLC = SDA;
				i2c_delay(delay_value);
				obj->i2c->CONTROLC = SCL;
				i2c_delay(delay_value);

    return 0;
}

int i2c_stop(i2c_t *obj) {
	
		int delay_value;
		switch ((int)obj->i2c) {
			case I2C_0: delay_value = TSC_TSU; break;
      case I2C_1: delay_value = AAIC_TSU; break;
		}
		// Actual stop bit
		i2c_delay(delay_value);
		obj->i2c->CONTROLC = SDA;
		i2c_delay(delay_value);
		obj->i2c->CONTROLS = SCL;
		i2c_delay(delay_value);
		obj->i2c->CONTROLS = SDA;
		i2c_delay(delay_value);

		return 0;
}



void i2c_frequency(i2c_t *obj, int hz) {
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
		unsigned int loop, rxdata;
		int delay_value, sadr, ack, bytes_read;
		rxdata=0;
		switch ((int)obj->i2c) {
			case I2C_0: 
				delay_value = TSC_TSU; 
				sadr = TSC_I2C_ADDR; 
				break;
      case I2C_1: 
				delay_value = AAIC_TSU; 
				sadr = AAIC_I2C_ADDR; 
				break;
		}
    // Start bit
		i2c_start(obj);

    // Set serial and register address
		i2c_send_byte(obj,sadr);
		ack += i2c_receive_ack(obj);
		i2c_send_byte(obj, address);
		ack += i2c_receive_ack(obj);

    // Stop bit
		i2c_stop(obj);

    // Start bit
		i2c_start_tsc(obj);
		
    // Read from serial address
    i2c_send_byte(obj,sadr | 1);
		ack += i2c_receive_ack(obj);
		bytes_read = 0;
		
		switch ((int)obj->i2c) {
			case I2C_0: 
        rxdata = (i2c_receive_byte(obj) & 0xFF);
				data[((length-1)-bytes_read)] = (char)rxdata;
				bytes_read++;
        // Read multiple bytes
        if ((length > 1) && (length < 5))
        {
          for (loop = 1; loop <= (length - 1); loop++)
          {
              // Send ACK
                i2c_delay(delay_value);
                obj->i2c->CONTROLC = SDA;
                i2c_delay(delay_value);
                obj->i2c->CONTROLS = SCL;
                i2c_delay(delay_value);
                obj->i2c->CONTROLC = SCL;
                i2c_delay(delay_value);
     
              rxdata = i2c_receive_byte(obj);
							data[(length-1)-bytes_read] = (char)rxdata;
							bytes_read++;
						
          }
        }
        break;
      case I2C_1:
        rxdata = i2c_receive_byte(obj);
				data[bytes_read] = (char)rxdata;
				bytes_read++;
    		break;
    }
    i2c_send_ack(obj);

    // Actual stop bit
		i2c_stop(obj);

    return bytes_read;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
		int ack=0;
		int sadr;
		switch ((int)obj->i2c) {
			case I2C_0: sadr = TSC_I2C_ADDR; break;
      case I2C_1: sadr = AAIC_I2C_ADDR; break;
		}
		for(int i = 1; i<=length; i++)
		{
			i2c_start(obj);
		
			// Set serial and register address
			i2c_send_byte(obj,sadr);
			ack += i2c_receive_ack(obj);
			i2c_send_byte(obj, address);
			ack += i2c_receive_ack(obj);
			i2c_send_byte(obj, *data);
			ack += i2c_receive_ack(obj);
		}

		i2c_stop(obj);
		if(ack==3) { return 1; }
		else{ return 0; }

}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    return 0;
}

int i2c_byte_write(i2c_t *obj, int data) {
    return 0;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
}

int i2c_slave_receive(i2c_t *obj) {
	return 0;
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
	return 0;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
	return 0;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
}

