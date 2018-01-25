/* 
* SerialModbus.cpp
*
* Created: 17.10.2017 09:09:47
* Author: Simon.Pfeifhofer
*/

#include "SerialModbus.h"
#include "wd_logging.h"

extern "C" {
	#include "lib_crc.h"
}

#define MODBUS_FRAME_INTERVAL_MS				20
#define MODBUS_DMA_RX_RAW_BUFFER_LENGTH			512
#define MODBUS_DMA_RX_FRAME_BUFFER_LENGTH		512
#define MODBUS_DMA_WRITE_TIMEOUT_MS				500
#define MODBUS_DMA_ECHO_TIMEOUT_MS				500
#define MODBUS_DMA_READ_TIMEOUT_MS				500

// default constructor
SerialModbus::SerialModbus(PinName tx, PinName rx, int baud, int stopBits, SerialBase::Parity parity, int bits): 
	_serial(tx, rx, baud),
	_tx_complete_sem(0),
	_rs485_en(RS485_En)
{
	_baud = baud;
	_parity = parity;
	_stopBits = stopBits;
	
	_serial.format(bits, parity, stopBits);
	
	_serial_raw_buffer = new char[MODBUS_DMA_RX_RAW_BUFFER_LENGTH]();
	_serial_frame_buffer = new char[MODBUS_DMA_RX_FRAME_BUFFER_LENGTH]();
	
	_serial.startRead(
		_serial_raw_buffer,
		512
	);

	
} //SerialModbus

// default destructor
SerialModbus::~SerialModbus()
{
	
} //~SerialModbus

uint8_t SerialModbus::Read(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t* result_buffer){

	if(!_mutex.trylock()){
		return Modbus::Lock;
	}

	// build base request
	uint8_t request_datagram[]  = {
		slave_id,											// slave address
		MODBUS_FC_READHOLDINGREGISTERS,						// read input register
		(uint8_t)((start_address >> 8) & 0xFF),				// register start address HIGH
		(uint8_t)((start_address) & 0xFF),					// register start address LOW
		(uint8_t)((register_count >> 8) & 0xFF),			// register count HIGH
		(uint8_t)((register_count) & 0xFF),					// register count LOW
		0x00,												// reserced for CRC LOW byte
		0x00												// reserved for CRC HIGH byte
	};
	
	// set CRC
	place_CRC(request_datagram, sizeof(request_datagram) - sizeof(uint16_t));
	
	// ensure frame interval
	Thread::wait(MODBUS_FRAME_INTERVAL_MS);
	
	Modbus::ModbusErrorCode ret;
	// write request
	if ((ret = write_request(request_datagram, sizeof(request_datagram))) != Modbus::Success) {
		return unlock_return(ret);
	}
	
	// declare response
	uint8_t response_datagram[
		3 +											// header
		(register_count * sizeof(uint16_t)) +		// registers
		sizeof(uint16_t)							// CRC
	];
	
	// read response
	if ((ret = read_response(response_datagram, sizeof(response_datagram))) != Modbus::Success) {
		return unlock_return(ret);
	}
	
	// check CRC, slave_id and function-code
	if(uint8_t code = check_response(response_datagram, sizeof(response_datagram), slave_id, MODBUS_FC_READHOLDINGREGISTERS) != Modbus::Success){
		return unlock_return(code);
	}
	
	// check byte-count
	if(response_datagram[2] != (register_count * 2)){
		return unlock_return(Modbus::ByteCount);
	}
	
	// copy datagram
	memcpy(
		result_buffer, 
		response_datagram + (sizeof(uint8_t) * 3), 
		register_count * sizeof(uint16_t)
	);
	
	return unlock_return(Modbus::Success);
	
}

uint8_t SerialModbus::Write(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t * write_buffer){
	
	if(!_mutex.trylock()){
		return Modbus::Lock;
	}
	
	// build base request
	uint8_t request_datagram[7 + sizeof(uint16_t) * register_count + 2];
	request_datagram[0] = slave_id;											// slave address
	request_datagram[1] = MODBUS_FC_WRITEHOLDINGREGISTERS;					// function code
	request_datagram[2] = (uint8_t)((start_address >> 8) & 0xFF);			// register start address HIGH
	request_datagram[3]	= (uint8_t)((start_address) & 0xFF);				// register start address LOW
	request_datagram[4] = (uint8_t)((register_count >> 8) & 0xFF);			// register count HIGH
	request_datagram[5] = (uint8_t)((register_count) & 0xFF);				// register count LOW
	request_datagram[6] = (uint8_t)(register_count * sizeof(uint16_t));		// byte count
	
	memcpy(request_datagram + 7, write_buffer, register_count * sizeof(uint16_t));
	
	// set CRC
	place_CRC(request_datagram, sizeof(request_datagram) - sizeof(uint16_t));
	
	// ensure frame interval
	Thread::wait(MODBUS_FRAME_INTERVAL_MS);
	
	Modbus::ModbusErrorCode ret;
	
	// write request
	if((ret = write_request(request_datagram, sizeof(request_datagram))) != Modbus::Success){
		return unlock_return(ret);
	}

	// declare response
	uint8_t response_datagram[8];
	
	// read response
	if ((ret = read_response(response_datagram, sizeof(response_datagram))) != Modbus::Success) {
		return unlock_return(ret);
	}
	
	// check CRC, slave_id and function-code
	if(uint8_t code = check_response(response_datagram, sizeof(response_datagram), slave_id, MODBUS_FC_WRITEHOLDINGREGISTERS) != Modbus::Success){
		return unlock_return(code);
	}
	
	// check start-address
	if(
		(((uint16_t)response_datagram[2] << 8) +
		(uint16_t)response_datagram[3])
			!=
		start_address
	){
		return unlock_return(Modbus::StartAddress);
	}
	
	// check register-count
	if(
		(((uint16_t)response_datagram[4] << 8) +
		(uint16_t)response_datagram[5])
		!=
		register_count
	){
		return unlock_return(Modbus::RegisterCount);
	}
	
	return unlock_return(Modbus::Success);
	
}

int SerialModbus::GetBaud(){
	return _baud;
}

int SerialModbus::GetStopBits(){
	return _stopBits;
}

SerialBase::Parity SerialModbus::GetParity(){
	return _parity;
}

void SerialModbus::_serial_tx_complete(int evt){
	_rs485_en.write(0);
	_tx_complete_sem.release();
}

Modbus::ModbusErrorCode SerialModbus::write_request(uint8_t * request_datagram, size_t length){
	
	_rs485_en.write(1);
	_serial.write(request_datagram, length, callback(this, &SerialModbus::_serial_tx_complete));
	_tx_complete_sem.wait(MODBUS_DMA_WRITE_TIMEOUT_MS);
	_rs485_en.write(0);
	
	size_t echo_length;
	_serial.popFrame(_serial_frame_buffer, &echo_length, MODBUS_DMA_ECHO_TIMEOUT_MS);
	
	if(echo_length == 0){
		return Modbus::Timeout;
	}
	
	if(length != echo_length){
		return Modbus::Echo;
	}
	
	for(uint i = 0; i < length; i++){
		if (request_datagram[i] != _serial_frame_buffer[i]) {
			return Modbus::Echo;
		}
	}
	
	return Modbus::Success;
	
}

Modbus::ModbusErrorCode SerialModbus::read_response(uint8_t * response_datagram, size_t length){
	
	size_t response_length;
	_serial.popFrame(_serial_frame_buffer, &response_length, MODBUS_DMA_READ_TIMEOUT_MS);
	
	if(response_length == 0){
		return Modbus::Timeout;
	}
	
	if(response_length != length){
		return Modbus::Unknown;
	}
	
	memcpy(response_datagram, _serial_frame_buffer, length);
	
	return Modbus::Success;
	
}

uint8_t SerialModbus::unlock_return(uint8_t return_code){
	
	_mutex.unlock();
	return return_code;
	
}

uint16_t SerialModbus::calculate_CRC(uint8_t * buffer, int length){
	
	uint16_t crc = 0xFFFF;
	for(int i = 0; i < length; i++){
		crc = update_crc_16(crc, buffer[i]);
	}
	return crc;
	
}

void SerialModbus::place_CRC(uint8_t * buffer, int payload_length){
	
	uint16_t crc = calculate_CRC(buffer, payload_length);
	buffer[payload_length] = (uint8_t)((crc) & 0xFF);
	buffer[payload_length + 1] = (uint8_t)((crc >> 8) & 0xFF);
	
}

bool SerialModbus::check_CRC(uint8_t * buffer, int length){
	
	return calculate_CRC(buffer, length) == 0;
	
}

uint8_t SerialModbus::check_response(uint8_t * response_datagram, size_t length, uint8_t slave_id, uint8_t function_code){
	
	// check CRC
	if(!check_CRC(response_datagram, length)){
		return Modbus::CRCValidation;
	}
	
	// check response codes
	if(response_datagram[0] != slave_id){
		return Modbus::SlaveId;
	}
	
	// check function-code (probably specified --> function-code > 0x80)
	if(response_datagram[1] > 0x80){
		return response_datagram[2];
	}
	
	// check function-code (undefined error)
	if (response_datagram[1] != function_code) {
		return Modbus::FunctionCode;
	}
	
	return Modbus::Success;
	
}
