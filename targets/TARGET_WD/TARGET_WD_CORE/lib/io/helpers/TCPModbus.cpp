/* 
* TCPModbus.cpp
*
* Created: 04.12.2017 14:04:08
* Author: Simon.Pfeifhofer
*/


#include "TCPModbus.h"

#define MODBUS_TCP_RECV_TIMEOUT_MS	1000
#define MODBUS_TCP_DEFAULT_PORT		502

// default constructor
TCPModbus::TCPModbus(EthernetInterface * ethernetInterface)
{
	_socket.set_timeout(MODBUS_TCP_RECV_TIMEOUT_MS);
	_slaveAddress.set_port(MODBUS_TCP_DEFAULT_PORT);
	_ethernetInterface = ethernetInterface;
} //TCPModbus

// default destructor
TCPModbus::~TCPModbus()
{
} //~TCPModbus

uint8_t TCPModbus::Read(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t* result_buffer){
	
	if(_socket.open(_ethernetInterface) != 0){
		return SocketClose(Modbus::TCPOpen);
	}
	
	if(_socket.connect(_slaveAddress) != 0){
		return SocketClose(Modbus::TCPConnect);
	}
	
	// Remark: 
	// Modbus requires big-endianess on the wire (http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b.pdf)
	// The used ARM Cortex-microcontrollers uses little-endian
	// Therefore memcpy will not be used (it would be necessary to switch the bytes of every word)
	
	uint16_t transaction_identifier = 1;
	uint16_t protocol_identifier = 0;
	uint16_t length = 6;
	
	uint8_t request_datagram[] = {
		(uint8_t)((transaction_identifier >> 8) & 0xFF),	// MBAP-Header: Transaction-Identifier (HIGH)
		(uint8_t)((transaction_identifier) & 0xFF),			// MBAP-Header: Transaction-Identifier (LOW)
		(uint8_t)((protocol_identifier >> 8) & 0xFF),		// MBAP-Header: Protocol-Identifier (HIGH)
		(uint8_t)((protocol_identifier) & 0xFF),			// MBAP-Header: Protocol-Identifier (LOW)
		(uint8_t)((length >> 8) & 0xFF),					// MBAP-Header: Length (HIGH)
		(uint8_t)((length) & 0xFF),							// MBAP-Header: Length (LOW)
		slave_id,											// MBAP-Header:	Unit-Identifier
		MODBUS_FC_READHOLDINGREGISTERS,						// MODBUS-PDU: Function code (read holding registers)
		(uint8_t)((start_address >> 8) & 0xFF),				// MODBUS-PDU: Starting address (HIGH)
		(uint8_t)((start_address) & 0xFF),					// MODBUS-PDU: Starting address (LOW)
		(uint8_t)((register_count >> 8) & 0xFF),			// MODBUS-PDU: Quantity of Registers (HIGH)
		(uint8_t)((register_count) & 0xFF)					// MODBUS-PDU: Quantity of Registers (HIGH)
	};
	
	// Send request
	if (_socket.send(request_datagram, sizeof(request_datagram)) < 0) {
		return SocketClose(Modbus::TCPSend);
	}
	
	uint8_t response_datagram[
		sizeof(uint16_t) +									// MBAP-Header:		Transaction-Identifier
		sizeof(uint16_t) +									// MBAP-Header:		Protocol-Identifier
		sizeof(uint16_t) +									// MBAP-Header:		Length
		sizeof(uint8_t) +									// MBAP-Header:		Unit-Identifier
		sizeof(uint8_t) +									// MODBUS-PDU:		Function code
		sizeof(uint8_t) +									// MODBUS-PDU:		Byte-count
		register_count * sizeof(uint16_t)					// MODBUS-PDU:		Registers
	];
	
	// Read
	int16_t real_size = 0;	
	if ((real_size = _socket.recv(response_datagram, sizeof(response_datagram))) < 0) {
		return this->SocketClose(Modbus::TCPReceive);
	}
	
	// Check length
	if (real_size != sizeof(response_datagram)) {
		return SocketClose(Modbus::TCPLengthRawRead);
	}
	
	// Check Transaction-Identifier
	uint16_t response_transaction_identifier = 
		(uint16_t)(response_datagram[0] << 8) + 
		(uint16_t)(response_datagram[1]);
	
	if (response_transaction_identifier != transaction_identifier) {
		return SocketClose(Modbus::TCPTransactionIdentifier);
	}
	
	// Check Protocol-Identifier
	uint16_t response_protocol_identifier = 
		(uint16_t)(response_datagram[2] << 8) + 
		(uint16_t)(response_datagram[3]);
	
	if (response_protocol_identifier != protocol_identifier) {
		return SocketClose(Modbus::TCPProtocolIdentifier);
	}
	
	// Check Length	
	uint16_t response_payload_length = 
		(uint16_t)(response_datagram[4] << 8) + 
		(uint16_t)(response_datagram[5]);
	if (response_payload_length != (sizeof(uint8_t) /*Unit-Identifier*/ + sizeof(uint8_t) /*Function-Code*/ + sizeof(uint8_t) /*ByteCount*/ + (register_count * sizeof(uint16_t)))) {
		return SocketClose(Modbus::TCPLengthPayload);
	}
	
	// Check Unit-Identifier
	if (response_datagram[6] != slave_id) {
		return SocketClose(Modbus::SlaveId);
	}
	
	// Check Function-Code
	if (response_datagram[7] != MODBUS_FC_READHOLDINGREGISTERS) {
		return SocketClose(response_datagram[7]);
	}
	
	// Check Byte-Count
	if (response_datagram[8] != sizeof(uint16_t)*register_count) {
		return SocketClose(Modbus::TCPByteCount);
	}
	
	// Copy payload
	memcpy(result_buffer, response_datagram + 9, sizeof(uint16_t) * register_count);
	
	return SocketClose(Modbus::Success);
	
}

uint8_t TCPModbus::Write(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t * write_buffer){
	
	if (_socket.open(_ethernetInterface) != 0) {
		return SocketClose(Modbus::TCPOpen);
	}
	
	if (_socket.connect(_slaveAddress) != 0) {
		return SocketClose(Modbus::TCPConnect);
	}
	
	// Remark: 
	// Modbus requires big-endianess on the wire (http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b.pdf)
	// The used ARM Cortex-microcontrollers uses little-endian
	// Therefore memcpy will not be used (it would be necessary to switch the bytes of every word)
	
	uint16_t transaction_identifier = 2;
	uint16_t protocol_identifier = 0;
	uint16_t length = 7 + register_count * sizeof(uint16_t);
	
	uint8_t request_datagram[13 + sizeof(uint16_t) * register_count];
	
	request_datagram[0] =	(uint8_t)((transaction_identifier >> 8) & 0xFF);		// MBAP-Header: Transaction-Identifier (HIGH)
	request_datagram[1] = 	(uint8_t)((transaction_identifier) & 0xFF);				// MBAP-Header: Transaction-Identifier (LOW)
	request_datagram[2] = 	(uint8_t)((protocol_identifier >> 8) & 0xFF);			// MBAP-Header: Protocol-Identifier (HIGH)
	request_datagram[3] = 	(uint8_t)((protocol_identifier) & 0xFF),				// MBAP-Header: Protocol-Identifier (LOW)
	request_datagram[4] = 	(uint8_t)((length >> 8) & 0xFF);						// MBAP-Header: Length (HIGH)
	request_datagram[5] = 	(uint8_t)((length) & 0xFF);								// MBAP-Header: Length (LOW)
	request_datagram[6] = 	slave_id;												// MBAP-Header:	Unit-Identifier
	request_datagram[7] = 	MODBUS_FC_WRITEHOLDINGREGISTERS;						// MODBUS-PDU: Function code (write holding registers)
	request_datagram[8] = 	(uint8_t)((start_address >> 8) & 0xFF);					// MODBUS-PDU: Starting address (HIGH)
	request_datagram[9] = 	(uint8_t)((start_address) & 0xFF);						// MODBUS-PDU: Starting address (LOW)
	request_datagram[10] = 	(uint8_t)((register_count >> 8) & 0xFF);				// MODBUS-PDU: Quantity of Registers (HIGH)
	request_datagram[11] = 	(uint8_t)((register_count) & 0xFF);						// MODBUS-PDU: Quantity of Registers (HIGH)
	request_datagram[12] = 	(uint8_t)((register_count * sizeof(uint16_t)) & 0xFF);	// MODBUS-PDU: Byte count
	
	memcpy(request_datagram + 13, write_buffer, register_count * sizeof(uint16_t));
	
	// Send request
	if (_socket.send(request_datagram, sizeof(request_datagram)) < 0) {
		return SocketClose(Modbus::TCPSend);
	}
	
	uint8_t response_datagram[
		sizeof(uint16_t) +									// MBAP-Header:		Transaction identifier
		sizeof(uint16_t) +									// MBAP-Header:		Protocol identifier
		sizeof(uint16_t) +									// MBAP-Header:		Length
		sizeof(uint8_t) +									// MBAP-Header:		Unit identifier
		sizeof(uint8_t) +									// MODBUS-PDU:		Function code
		sizeof(uint8_t)	+									// MODBUS-PDU:		Start address (HIGH)
		sizeof(uint8_t)	+									// MODBUS-PDU:		Start address (LOW)
		sizeof(uint8_t)	+									// MODBUS-PDU:		Register count (HIGH)
		sizeof(uint8_t)										// MODBUS-PDU:		Register count (LOW)
	];
	
	// Read
	int16_t real_size = 0;	
	if ((real_size = _socket.recv(response_datagram, sizeof(response_datagram))) < 0) {
		return this->SocketClose(Modbus::TCPReceive);
	}
	
	// Check length
	if (real_size != sizeof(response_datagram)) {
		return SocketClose(Modbus::TCPLengthRawRead);
	}
	
	// Check Transaction-Identifier
	uint16_t response_transaction_identifier = 
		(uint16_t)(response_datagram[0] << 8) + 
		(uint16_t)(response_datagram[1]);
	
	if (response_transaction_identifier != transaction_identifier) {
		return SocketClose(Modbus::TCPTransactionIdentifier);
	}
	
	// Check Protocol-Identifier
	uint16_t response_protocol_identifier = 
		(uint16_t)(response_datagram[2] << 8) + 
		(uint16_t)(response_datagram[3]);
	
	if (response_protocol_identifier != protocol_identifier) {
		return SocketClose(Modbus::TCPProtocolIdentifier);
	}
	
	// Check Length	
	uint16_t response_payload_length = 
		(uint16_t)(response_datagram[4] << 8) + 
		(uint16_t)(response_datagram[5]);
	if (response_payload_length != (sizeof(uint8_t) /*Unit identifier*/ + sizeof(uint8_t) /*Function code*/ + sizeof(uint16_t) /*Start address*/ + sizeof(uint16_t) /*Register count*/)) {
		return SocketClose(Modbus::TCPLengthPayload);
	}
	
	// Check Unit-Identifier
	if (response_datagram[6] != slave_id) {
		return SocketClose(Modbus::SlaveId);
	}
	
	// Check Function-Code
	if (response_datagram[7] != MODBUS_FC_WRITEHOLDINGREGISTERS) {
		return SocketClose(response_datagram[7]);
	}
	
	// Check Start address
	uint16_t response_start_address =
		(uint16_t)(response_datagram[8] << 8) +
		(uint16_t)(response_datagram[9]);
	if (response_start_address != start_address) {
		return SocketClose(Modbus::StartAddress);
	}
	
	// Check Register count
	uint16_t response_register_count =
		(uint16_t)(response_datagram[10] << 8) +
		(uint16_t)(response_datagram[11]);
	if (response_register_count != register_count) {
		return SocketClose(Modbus::RegisterCount);
	}
	
	return SocketClose(Modbus::Success);
	
}

char* TCPModbus::GetIpAddress(){
	return _slaveAddress.get_ip_address();
}

void TCPModbus::SetIpAddress(char * ip_address, int length){
	char ip_address_terminated[length];
	memcpy(ip_address_terminated, ip_address, length);
	ip_address_terminated[length] = '\0';
	_slaveAddress.set_ip_address(ip_address_terminated);
}

int TCPModbus::GetPort(){
	return _slaveAddress.get_port();
}

void TCPModbus::SetPort(int port){
	_slaveAddress.set_port(port);
}

uint8_t TCPModbus::SocketClose(ModbusErrorCode error_code) {
	
	// the passed "error_code" indicates more specifically the reason
	if (_socket.close() != 0 && 
		error_code == Modbus::Success) {
		error_code = Modbus::TCPClose;
	}
	return error_code;
	
}
