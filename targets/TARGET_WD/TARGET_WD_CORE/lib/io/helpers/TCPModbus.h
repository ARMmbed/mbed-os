/* 
* TCPModbus.h
*
* Created: 04.12.2017 14:04:08
* Author: Simon.Pfeifhofer
*/


#ifndef __TCPMODBUS_H__
#define __TCPMODBUS_H__

#include "mbed.h"
#include "EthernetInterface.h"
#include "Modbus.h"

class TCPModbus : public Modbus
{
//variables
public:
protected:
private:
	SocketAddress _slaveAddress;
	TCPSocket _socket;
	EthernetInterface * _ethernetInterface;

//functions
public:
	TCPModbus(EthernetInterface * ethernetInterface);
	~TCPModbus();
	char* GetIpAddress();
	void SetIpAddress(char * ip_address, int length);
	int GetPort();
	void SetPort(int port);
	uint8_t Read(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t* result_buffer);
	uint8_t Write(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t* write_buffer);
protected:
private:
	TCPModbus( const TCPModbus &c );
	TCPModbus& operator=( const TCPModbus &c );
	uint8_t SocketClose(ModbusErrorCode error_code);

}; //TCPModbus

#endif //__TCPMODBUS_H__
