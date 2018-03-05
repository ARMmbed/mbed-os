/* 
* BTMaster.h
*
* Created: 29.11.2017 13:48:48
* Author: Patrick.Frischmann
*/


#ifndef __BTMASTER_H__
#define __BTMASTER_H__

/******************************************************
___________________INCLUDES____________________________
******************************************************/
#include "BTBase.h"
#include <iterator>
#include "ResettableTimeout.h"
#include "BTSlaveCollection.h"

/******************************************************
___________________DEFINES_____________________________
******************************************************/

#define BT_MASTER_DISCOVER_FIN_TIMEOUT_MS			500
#define BT_MASTER_TX_FRAME_QUEUE_TIMEOUT_MS			30
#define BT_MASTER_TX_ACK_TIMEOUT_US					3000000
#define BT_MASTER_TX_ACK_TIMEOUT_EXTENDED_US		6000000
#define BT_MASTER_MIN_SLAVE_ADDRESS_INTERVAL_S		3

class BTMaster : public BTBase
{
//variables
public:
protected:
private:
	InterruptIn _irq;
	rtos::Thread _mainLoopThread;
	ResettableTimeout * _txQueueLockTimeout;
	BTSlaveCollection _slaveCollection;
	uint64_t _lastAddressedSlave;
	
//functions
public:
	BTMaster(PinName Tx, PinName Rx, PinName Irq, PinName Led, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
	~BTMaster();
	void bt_handle_frame(uint64_t sender, const void * data, size_t size);
	int bt_get_slave_count(void) { return _slaveCollection.getCount(); };
	void bt_get_slave_ids(uint64_t * buffer) { return _slaveCollection.getIds(buffer); };
	
protected:
	void _start(void);
	void _frame_received_internal(const char * data, size_t size);
	

private:
	BTMaster( const BTMaster &c );
	BTMaster& operator=( const BTMaster &c );
	
	void _main_loop(void);
	
	void _on_bus_irq(void);
	void _on_tx_queue_lock_timeout(void);
	void _send_discover_broadcast(void);
	void _send_discover_fin(void);
	void _send_select_slave(uint64_t id = 0);
	void _tx_release(void);
	void _tx_lock(unsigned int timeout = BT_MASTER_TX_ACK_TIMEOUT_US);

}; //BTMaster

#endif //__BTMASTER_H__
