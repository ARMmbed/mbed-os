/* 
* BTBase.h
*
* Created: 29.11.2017 13:49:28
* Author: Patrick.Frischmann
*/

#ifndef __BTBASE_H__
#define __BTBASE_H__

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mbed.h"
#include "objects.h"
#include "DMASerial.h"
#include "ResettableTimeout.h"
#include "wd_logging.h"

/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define BT_BUFFER_SIZE					4096
#define BT_TX_QUEUE_SIZE				15
#define BT_TX_WRITE_TIMEOUT_MS			100
#define BT_TX_ECHO_TIMEOUT_MS			100

#define BT_DISCOVER_LISTENING_PERIOD_MS		2000

//Signals for the processing thread
#define BT_SIG_TX_PROCESSING_RELEASE	1

// Message Type
#define BT_MESSAGE_TYPE_DISCOVER		0xCC
#define BT_MESSAGE_TYPE_DISCOVER_FIN	0xCD
#define BT_MESSAGE_TYPE_APPDATA			0x33
#define BT_MESSAGE_TYPE_APPDATA_ACK		0x34

// Address
#define BT_FRAME_ADDRESS_BROADCAST		0xFFFFFFFFFFFFFFFF

// Frame
#define BT_FRAME_MESSAGE_TYPE_OFFSET	0
#define BT_FRAME_MESSAGE_TYPE_LENGTH	1
#define BT_FRAME_ADDRESS_OFFSET			1
#define BT_FRAME_ADDRESS_LENGTH			8

//typedef struct  {
	//uint8_t	message_type;
	//uint64_t address;
//} bt_frame_header;

class BTBase {
	
//variables
public:

protected:
	uint64_t _id;
	DMASerial *_dmaSerial;
	Mail<dma_frame_meta_t, BT_TX_QUEUE_SIZE> _dma_tx_frame_queue;
	char * _tx_frame_buffer;
	Callback<void()> _discoveryFinishedCallback;
	
	typedef enum {
		BT_STATE_INITIAL = 0,
		BT_STATE_DISCOVER,
		BT_STATE_READY
	} bt_state_t;
	bt_state_t _state;
	
private:
	char * _rx_buffer;
	char * _tx_buffer;
	unsigned int _tx_producer_pointer;
	unsigned int _tx_frame_size;

	rtos::Semaphore _tx_complete_sem;
	rtos::Semaphore _tx_echo_received_sem;
	rtos::Mutex _tx_buffer_mutex;
	rtos::Mutex _send_queue_mutex;

	DigitalOut _activity_led;
	ResettableTimeout * _activity_led_timeout;

//functions
public:
	BTBase(PinName Tx, PinName Rx, PinName Irq, PinName Led, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
	~BTBase();
	void bt_start(uint64_t id);
	void bt_transmit_frame(const void * data, size_t size, uint64_t id = 0);
	void bt_attach_discovery_finished_cb(Callback<void()> cb);
	void bt_await_ready_state(uint32_t timeout);
	
protected:
	virtual void _start(void) = 0;
	virtual void _frame_received_internal(const char * data, size_t size) = 0;	// caution runs in same thread!
	
	char _get_message_type(const char * data);
	uint64_t _get_address(const char * data);
	void _send_enqueue(uint8_t messageType, uint64_t slaveId, const void * data, size_t size);
	void _tx_buffer_flush(size_t size);
	void _tx_frame_queue_clear(void);
	void _indicate_activity(void);
	void _activity_led_on(void) { _activity_led.write(1); };
	void _activity_led_off(void) { _activity_led.write(0); };
	
private:
	BTBase( const BTBase &c );
	BTBase& operator=( const BTBase &c );

	void _tx_complete(int evt) { _tx_complete_sem.release(); };
	void _on_activity_led_timeout(void) { _activity_led.write(1); };
	
	void _tx_buffer_cpy(const void * src, size_t num);
	
	void _on_rx_frame_received(dma_frame_meta_t * frame_meta);

}; //BTBase

#endif //__BTBASE_H__
