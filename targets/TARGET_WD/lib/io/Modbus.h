/*
 * Modbus.h
 *
 * Created: 17.10.2017 08:46:09
 *  Author: Simon.Pfeifhofer
 */ 


#ifndef MODBUS_H_
#define MODBUS_H_

#define MODBUS_FC_READHOLDINGREGISTERS			3
#define MODBUS_FC_WRITEHOLDINGREGISTERS			16

class Modbus{

	public:
		
		enum ModbusErrorCode : uint8_t {
			Success = 0x00,
			TCPByteCount = 0xEC,
			TCPLengthPayload = 0xED,
			TCPProtocolIdentifier = 0xEE,
			TCPTransactionIdentifier = 0xEF,
			TCPLengthRawRead = 0xF0,
			TCPClose = 0xF1,
			TCPReceive = 0xF2,
			TCPSend = 0xF3,
			TCPConnect = 0xF4,
			TCPOpen = 0xF5,
			Lock = 0xF6,
			FunctionCode = 0xF7,
			Echo = 0xF8,
			RegisterCount = 0xF9,
			StartAddress = 0xFA,
			ByteCount = 0xFB,
			SlaveId = 0xFC,
			CRCValidation = 0xFD,
			Timeout = 0xFE,
			Unknown = 0xFF
		};
		
		// Returns 
		// -> 0 in case of success
		// -> 0xFE (254) in case of CRC-error
		// -> 0xFF (255) in case of timeout
		// -> otherwise the exception code accoring to the modbus-specification
		virtual uint8_t Read(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t* result_buffer) = 0;
		
		// Returns
		// -> 0 in case of success
		// -> 0xFF = 255 in case of timeout
		// -> otherwise the exception code accoring to the modbus-specification
		virtual uint8_t Write(uint8_t slave_id, uint16_t start_address, uint16_t register_count, uint8_t* write_buffer) = 0;
	
};


#endif /* MODBUS_H_ */