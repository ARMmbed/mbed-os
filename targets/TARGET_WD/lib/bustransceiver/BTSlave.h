/* 
* BTSlave.h
*
* Created: 29.11.2017 13:49:00
* Author: Patrick.Frischmann
*/


#ifndef __BTSLAVE_H__
#define __BTSLAVE_H__

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "BTBase.h"
#include "ResettableTimeout.h"
#include <random>

/******************************************************
___________________DEFINES_____________________________
******************************************************/
#define BT_SLAVE_MAX_TX_ACTIVE_WINDOW_MS	1000
#define BT_SLAVE_TX_FRAME_QUEUE_TIMEOUT_MS	30

class BTSlave : public BTBase {
//variables
public:
protected:
private:
	DigitalOut _irq;
	ResettableTimeout * _tx_active_timeout;
	Thread _txQueueProcessingThread;
	default_random_engine _rng;

//functions
public:
	BTSlave(PinName Tx, PinName Rx, PinName Irq, PinName Led, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
	~BTSlave();
	void bt_handle_frame(const void * data, size_t size);
	
protected:
	void _start(void);
	void _frame_received_internal(const char * data, size_t size);
	void _tx_queue_process_loop(void);

private:
	BTSlave( const BTSlave &c );
	BTSlave& operator=( const BTSlave &c );
	
	int _get_random_int(int from, int to);
	
	void _bus_irq_set(void) { _irq.write(0); };
	void _bus_irq_clr(void) { _irq.write(1); };
	void _tx_release(void);
	void _tx_lock(void);
	void _send_app_data_ack(void);
	void _send_discover_ack(void);
	void _on_tx_active_timeout(void);
	
}; //BTSlave

#endif //__BTSLAVE_H__
