/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "drivers/QSPI.h"
#include "platform/mbed_critical.h"

#if DEVICE_QSPI
#define IS_BUS_WIDTH_VALID(width)          ((width == 1) || (width == 2) || (width == 4))    
#define IS_SIZE_VALID(size)                ((size == 8) || (size == 16) || (size == 24) || (size == 32))      
#define IS_ALT_SIZE_VALID(alt_size)        ((alt_size == 0) || (alt_size == 8) || (alt_size == 16) || (alt_size == 24) || (alt_size == 32))      

namespace mbed {

QSPI* QSPI::_owner = NULL;
SingletonPtr<PlatformMutex> QSPI::_mutex;  
  
QSPI::QSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel) :
        _qspi(),
        _inst_width(QSPI_DEFAULT_INST_WIDTH),
        _address_width(QSPI_DEFAULT_ADDRESS_WIDTH),
        _address_size(QSPI_DEFAULT_ADDRESS_SIZE),
        _alt_width(QSPI_DEFAULT_ALT_WIDTH),
        _alt_size(QSPI_DEFAULT_ALT_SIZE),
        _data_width(QSPI_DEFAULT_DATA_WIDTH),
        _num_dummy_cycles(QSPI_DEFAULT_DUMMY_CYCLES),
        _hz(QSPI_DEFAULT_HZ) {
    // No lock needed in the constructor
    _qspi_io0 = io0;
    _qspi_io1 = io1;
    _qspi_io2 = io2;
    _qspi_io3 = io3;
    _qspi_clk = sclk;
    _qspi_cs = ssel;            
}
        
bool QSPI::configure_format(int inst_width, 
                     int address_width, int address_size,  
                     int alt_width, int alt_size,
                     int data_width,
                     int dummy_cycles,
                     int mode ) {
    if(!IS_BUS_WIDTH_VALID(inst_width)) return false;
    if(!IS_BUS_WIDTH_VALID(address_width)) return false;
    if(!IS_SIZE_VALID(address_size)) return false;
    if(!IS_BUS_WIDTH_VALID(alt_width)) return false;
    if(!IS_ALT_SIZE_VALID(alt_size)) return false;
    if(!IS_BUS_WIDTH_VALID(data_width)) return false;
    if(dummy_cycles < 0) return false;                         
    if(mode != 0 && mode != 1) return false;                            
    
    lock();
    switch(inst_width) {
        case 1:_inst_width = QSPI_CFG_BUS_SINGLE; break;
        case 2:_inst_width = QSPI_CFG_BUS_DUAL; break;
        case 4:_inst_width = QSPI_CFG_BUS_QUAD; break;            
        default:_inst_width = QSPI_CFG_BUS_SINGLE;
    }
    
    switch(address_width) {
        case 1:_address_width = QSPI_CFG_BUS_SINGLE; break;
        case 2:_address_width = QSPI_CFG_BUS_DUAL; break;
        case 4:_address_width = QSPI_CFG_BUS_QUAD; break;            
        default:_address_width = QSPI_CFG_BUS_SINGLE;
    }
    
    switch(address_size) {
        case 8:_address_size = QSPI_CFG_ADDR_SIZE_8; break;
        case 16:_address_size = QSPI_CFG_ADDR_SIZE_16; break;
        case 24:_address_size = QSPI_CFG_ADDR_SIZE_24; break;
        case 32:_address_size = QSPI_CFG_ADDR_SIZE_32; break;            
        default:_address_size = QSPI_CFG_ADDR_SIZE_8;
    }
    
    switch(alt_width) {
        case 1:_alt_width = QSPI_CFG_BUS_SINGLE; break;
        case 2:_alt_width = QSPI_CFG_BUS_DUAL; break;
        case 4:_alt_width = QSPI_CFG_BUS_QUAD; break;            
        default:_alt_width = QSPI_CFG_BUS_SINGLE;
    }
    
    switch(alt_size) {
        case 0:_alt_size = QSPI_CFG_ALT_SIZE_NONE; break;
        case 8:_alt_size = QSPI_CFG_ALT_SIZE_8; break;
        case 16:_alt_size = QSPI_CFG_ALT_SIZE_16; break;
        case 24:_alt_size = QSPI_CFG_ALT_SIZE_24; break;
        case 32:_alt_size = QSPI_CFG_ALT_SIZE_32; break;            
        default:_alt_size = QSPI_CFG_ALT_SIZE_NONE;
    }
    
    switch(data_width) {
        case 1:_data_width = QSPI_CFG_BUS_SINGLE; break;
        case 2:_data_width = QSPI_CFG_BUS_DUAL; break;
        case 4:_data_width = QSPI_CFG_BUS_QUAD; break;            
        default:_data_width = QSPI_CFG_BUS_SINGLE; 
    }
    
    _num_dummy_cycles = dummy_cycles;
    _mode = mode;
    unlock();
    
    return true;
}

bool QSPI::set_frequency(int hz) {
    
    lock();
    _hz = hz;
    
    //If the same owner, just change freq.
    //Otherwise we may have to change mode as well, so call _acquire
    if (_owner == this) {
        qspi_frequency(&_qspi, _hz);
    } else {
        _acquire();
    }
    unlock();
    
    return true;
}

bool QSPI::initialize() {
    lock();
    qspi_status_t ret = qspi_init(&_qspi, _qspi_io0, _qspi_io1, _qspi_io2, _qspi_io3, _qspi_clk, _qspi_cs, _hz, _mode );
    unlock();
    
    return ( ret == QSPI_STATUS_OK )? true:false;
}

int QSPI::read(unsigned int address, char *rx_buffer, size_t *rx_length) {
    int ret = 0;
    
    if( (rx_length != NULL) && (rx_buffer != NULL) ) {
        if(*rx_length != 0) {
            lock();
            if( true == _acquire()) {
                qspi_command_t *qspi_cmd = _build_qspi_command(-1, address, -1);
                if(QSPI_STATUS_OK == qspi_read(&_qspi, qspi_cmd, rx_buffer, rx_length)) {
                    ret = 1;
                }
            }
            unlock();
        }
    }
    
    return ret;
}

int QSPI::write(unsigned int address, const char *tx_buffer, size_t *tx_length) {
    int ret = 0;
    
    if( (tx_length != NULL) && (tx_buffer != NULL) ) {
        if(*tx_length != 0) {
            lock();
            if(true == _acquire()) {
                qspi_command_t *qspi_cmd = _build_qspi_command(-1, address, -1);
                if(QSPI_STATUS_OK == qspi_write(&_qspi, qspi_cmd, tx_buffer, tx_length)) {
                    ret = 1;
                }
            }
            unlock();
        }
    }
    
    return ret;
}

int QSPI::read(unsigned int instruction, unsigned int address, unsigned int alt, char *rx_buffer, size_t *rx_length) {
    int ret = 0;
    
    if( (rx_length != NULL) && (rx_buffer != NULL) ) {
        if(*rx_length != 0) {
            lock();
            if( true == _acquire()) {
                qspi_command_t *qspi_cmd = _build_qspi_command(instruction, address, alt);
                if(QSPI_STATUS_OK == qspi_read(&_qspi, qspi_cmd, rx_buffer, rx_length)) {
                    ret = 1;
                }
            }
            unlock();
        }
    }
    
    return ret;
}

int QSPI::write(unsigned int instruction, unsigned int address, unsigned int alt, const char *tx_buffer, size_t *tx_length) {
    int ret = 0;
    
    if( (tx_length != NULL) && (tx_buffer != NULL) ) {
        if(*tx_length != 0) {
            lock();
            if(true == _acquire()) {
                qspi_command_t *qspi_cmd = _build_qspi_command(instruction, address, alt);
                if(QSPI_STATUS_OK == qspi_write(&_qspi, qspi_cmd, tx_buffer, tx_length)) {
                    ret = 1;
                }
            }
            unlock();
        }
    }
    
    return ret;
}

int QSPI::command_transfer(unsigned int instruction, const char *tx_buffer, size_t tx_length, const char *rx_buffer, size_t rx_length) {
    int ret = 1;
    
    lock();
    if(true == _acquire()) {
        qspi_command_t *qspi_cmd = _build_qspi_command(instruction, -1, -1); //We just need the command
        if(QSPI_STATUS_OK != qspi_command_transfer(&_qspi, qspi_cmd, (const void *)tx_buffer, tx_length, (void *)rx_buffer, rx_length)) {
            //We got error status, return 0
            ret = 0;
        }
    } else {
        ret = 0;
    }
    unlock();
    
    return ret;
}

void QSPI::lock() {
    _mutex->lock();
}

void QSPI::unlock() {
    _mutex->unlock();
}

// Note: Private function with no locking
bool QSPI::_acquire() {
    qspi_status_t ret = QSPI_STATUS_OK;
    
    if (_owner != this) {
        //This will set freq as well
        ret = qspi_init(&_qspi, _qspi_io0, _qspi_io1, _qspi_io2, _qspi_io3, _qspi_clk, _qspi_cs, _hz, _mode );
        _owner = this;
    }
        
    return ( ret == QSPI_STATUS_OK )? true:false;
}

qspi_command_t *QSPI::_build_qspi_command(int instruction, int address, int alt) {

    memset( &_qspi_command, 0,  sizeof(qspi_command_t) );
    //Set up instruction phase parameters
    _qspi_command.instruction.bus_width = _inst_width; 
    if(instruction != -1) {
        _qspi_command.instruction.value = instruction; 
    } else {
        _qspi_command.instruction.value = 0; 
    }
        
    //Set up address phase parameters
    _qspi_command.address.bus_width = _address_width;
    _qspi_command.address.size = _address_size;
    if(address != -1) {
        _qspi_command.address.value = address; 
    } else {
        _qspi_command.address.value = 0; 
    }
    
    //Set up alt phase parameters
    _qspi_command.alt.bus_width = _alt_width; 
    _qspi_command.alt.size = _alt_size;
    if(alt != -1) {
        _qspi_command.alt.value = alt;  
    } else {
        //In the case alt phase is absent, set the alt size to be NONE
        _qspi_command.alt.value = 0; 
    }
    
    //Set up dummy cycle count
    _qspi_command.dummy_count = _num_dummy_cycles;
    
    //Set up bus width for data phase
    _qspi_command.data.bus_width = _data_width;
    
    return &_qspi_command;
}

} // namespace mbed

#endif
