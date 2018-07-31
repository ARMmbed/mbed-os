/* 
* BTMaster.cpp
*
* Created: 29.11.2017 13:48:48
* Author: Patrick.Frischmann
*/

#include "BTMaster.h"

// default constructor
BTMaster::BTMaster(PinName Tx, PinName Rx, PinName Irq, PinName Led, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) 
	:	BTBase(Tx, Rx, Irq, Led, baud),
		_irq(Irq),
		_slaveCollection(),
		_lastAddressedSlave(0) {
	
	_state = BT_STATE_INITIAL;
			
	// register interrupt handler for falling edge on bus interrupt
	wd_log_heap_stats("BTmaster before eq");
	events::EventQueue * eq = mbed_event_queue();
	wd_log_heap_stats("BTmaster after eq");
	_irq.fall(eq->event(callback(this, &BTMaster::_on_bus_irq)));
	_txQueueLockTimeout = new ResettableTimeout(eq->event(callback(this, &BTMaster::_on_tx_queue_lock_timeout)), BT_MASTER_TX_ACK_TIMEOUT_US);
	_txQueueLockTimeout->stop();
	
} //BTMaster

// default destructor
BTMaster::~BTMaster() {
	_mainLoopThread.terminate();
} //~BTMaster

void BTMaster::_main_loop(void) {
	
	uint64_t tempId;

	while(true) {
		
		switch(_state) {
	
			case BT_STATE_INITIAL:
		
				wd_log_info("BTMaster -> BT_STATE_INITIAL");
			
				// clear memorized slaves
				_slaveCollection.clear();
				_lastAddressedSlave = 0;
				
				// lock tx queue processing and clear queue
				_tx_lock();
				_tx_frame_queue_clear();
			
				// continue with discover state
				_state = BT_STATE_DISCOVER;
				_activity_led_off();
			
				break;
			
			case BT_STATE_DISCOVER:
			
				wd_log_info("BTMaster -> BT_STATE_DISCOVER");
			
				// send discover broadcast message
				_send_discover_broadcast();
		
				// wait while listening for responses
				wait_ms(BT_DISCOVER_LISTENING_PERIOD_MS);
			
				// send discover finish message to all acquired board addresses
				_send_discover_fin();
			
				// wait while allowing slaves time to process discover finish message
				wait_ms(BT_MASTER_DISCOVER_FIN_TIMEOUT_MS);
			
				// check bus interrupt line (undiscovered clients pull line low)
				if (_irq.read() > 0) {
				
					// all slaves discovered, inform upper layer
					_discoveryFinishedCallback.call();
				
					// allow master to send data
					_tx_release();
					
					// change state to ready
					_state = BT_STATE_READY;
					wd_log_info("BTMaster -> BT_STATE_READY");
					_activity_led_on();
				
				}
				else {
					
					_state = BT_STATE_DISCOVER_FAILED;

				}
			
				break;
			
			case BT_STATE_DISCOVER_FAILED:

				wd_log_error("BTMaster -> BT_STATE_DISCOVER_FAILED");

				// At least one slave didn't get the discovery-confirmation from the master and pushes the BUS_IRQ still to 0
				// Therefore the assumption is that the read of the discover-confirmation of the slave could not be read by the master
				// The restart of the rx-processing is the current solution
				this->_start_rx_processing();

				_state = BT_STATE_DISCOVER;

				break;
			
			case BT_STATE_READY:
		
				// address slaves alternately for app data handling
				// todo: check errors and communication timeouts
		
				// wait for tx release
				Thread::signal_wait(BT_SIG_TX_PROCESSING_RELEASE);
				
				tempId = _slaveCollection.getCommunicationTimeoutReached(BT_MASTER_MIN_SLAVE_ADDRESS_INTERVAL_S);
				if (tempId != 0) {
					wd_log_warn("BTMaster -> detected communication timeout for slave %.8X%.8X, enforcing slave addressing now", (uint32_t)(tempId >> 32), (uint32_t)(tempId));
					_send_select_slave(tempId);
				} else {
				
					// wait and check if a frame is in tx queue
					osEvent evt = _dma_tx_frame_queue.get(BT_MASTER_TX_FRAME_QUEUE_TIMEOUT_MS);
					if (evt.status == osEventMail) {

						wd_log_debug("BTMaster -> sending app data to slave, locking tx afterwards for slave response window");
					
						size_t size;	
						dma_frame_meta_t * frame_meta = (dma_frame_meta_t *) evt.value.p;

						wd_log_debug("GET frame %d bytes", frame_meta->frame_size);

						_dmaSerial->getFrame(frame_meta, _tx_frame_buffer, &size);
						_dma_tx_frame_queue.free(frame_meta);
					
						_lastAddressedSlave = _get_address(_tx_frame_buffer);
						_slaveCollection.reportMsgSent(_lastAddressedSlave);		
						_tx_buffer_flush(size);
						
						// workaround for sw download because response (especially of initial block) takes longer
						if(size > 1000) {
							_tx_lock(BT_MASTER_TX_ACK_TIMEOUT_EXTENDED_US);
							break;
						}
						
					} else { // otherwise select next slave to provide communication window
						_send_select_slave();
					}
				}
			
				_tx_lock();
		
				break;
	
		default:
			break;
		
		}
	}
}

void BTMaster::_on_bus_irq(void) {

	// in case of bus interrupt restart discover routine
	wd_log_warn("BTMaster -> received bus interrupt, initializing discover routine");
	_state = BT_STATE_INITIAL;

}

void BTMaster::_on_tx_queue_lock_timeout(void) {
	
	if(_lastAddressedSlave != 0) {
		wd_log_warn("BTMaster -> ACK timeout for slave %.8X%.8X detected!", (uint32_t)(_lastAddressedSlave >> 32), (uint32_t)(_lastAddressedSlave));
		_slaveCollection.reportErroneousCommunication(_lastAddressedSlave);
	}
	_tx_release();
	
}

void BTMaster::_send_discover_broadcast(void) {

	// copy message type
	_tx_frame_buffer[0] = BT_MESSAGE_TYPE_DISCOVER;
	
	// copy id
	uint64_t id = BT_FRAME_ADDRESS_BROADCAST;
	memcpy(_tx_frame_buffer + BT_FRAME_MESSAGE_TYPE_LENGTH, &id, BT_FRAME_ADDRESS_LENGTH);

	_tx_buffer_flush(BT_FRAME_MESSAGE_TYPE_LENGTH + BT_FRAME_ADDRESS_LENGTH);

}

void BTMaster::_send_discover_fin(void) {

	for (int i = 0; i < _slaveCollection.getCount(); i++) {
		
		// copy message type
		_tx_frame_buffer[0] = BT_MESSAGE_TYPE_DISCOVER_FIN;
	
		// copy id
		uint64_t id = _slaveCollection.getId(i);
		memcpy(_tx_frame_buffer + BT_FRAME_MESSAGE_TYPE_LENGTH, &id, BT_FRAME_ADDRESS_LENGTH);

		_tx_buffer_flush(BT_FRAME_MESSAGE_TYPE_LENGTH + BT_FRAME_ADDRESS_LENGTH);
		
		wait_ms(5);
		
	}

}

void BTMaster::_send_select_slave(uint64_t id) {

	// if no id is passed, just select next slave
	if (id == 0) {
		id = _slaveCollection.selectNext();
	}
	// if there is no slave present, just return
	if (id == 0) return;
	
	// check error count and remove slave (also remove from bus bridge!!)
	if (_slaveCollection.isErrorCountExhausted(id)) {
		wd_log_warn("BTMaster -> error count for slave %.8X%.8X exhausted, trigger re-discovery (maybe slave was disconnected?)", (uint32_t)(id >> 32), (uint32_t)(id));
		_state = BT_STATE_INITIAL;
		return;
	}
	
	wd_log_debug("BTMaster -> addressing slave %.8X%.8X", (uint32_t)(id >> 32), (uint32_t)(id));
	
	// copy message type
	_tx_frame_buffer[0] = BT_MESSAGE_TYPE_APPDATA;
	
	// copy id
	memcpy(_tx_frame_buffer + BT_FRAME_MESSAGE_TYPE_LENGTH, &id, BT_FRAME_ADDRESS_LENGTH);

	_lastAddressedSlave = id;
	_slaveCollection.reportMsgSent(id);
	
	_tx_buffer_flush(BT_FRAME_MESSAGE_TYPE_LENGTH + BT_FRAME_ADDRESS_LENGTH);

}

void BTMaster::_tx_release(void) {
//	wd_log_error("_release_tx_queue_processing");
	_mainLoopThread.signal_set(BT_SIG_TX_PROCESSING_RELEASE);
}

void BTMaster::_tx_lock(unsigned int timeout) {
//	wd_log_error("_lock_tx_queue_processing");
	_mainLoopThread.signal_clr(BT_SIG_TX_PROCESSING_RELEASE);
	_txQueueLockTimeout->reset(timeout);
}

void BTMaster::_frame_received_internal(const char * data, size_t size) {

	// info: frame is already considered valid by previous chain
	
	if(size < BT_FRAME_MESSAGE_TYPE_LENGTH + BT_FRAME_ADDRESS_LENGTH) {
		wd_log_error("BTMaster::_frame_received_internal() -> received invalid frame (length %d)", size);
		return;
	}
	
	// extract address
	uint64_t address = _get_address(data);
	
	switch(_get_message_type(data)) {
		
		case BT_MESSAGE_TYPE_DISCOVER:
			wd_log_info("BTMaster::_frame_received_internal() -> received discover message");
		
			// add slave
			_slaveCollection.add(address);
			
			break;
		
		case BT_MESSAGE_TYPE_APPDATA:
			wd_log_debug("BTMaster::_frame_received_internal() -> received app data message");
			
			_indicate_activity();
			
			// is payload included or is message just ack? 
			if(size > BT_FRAME_MESSAGE_TYPE_LENGTH + BT_FRAME_ADDRESS_LENGTH) {
				
				wd_log_debug("BTMaster::_frame_received_internal() -> received app data");
				
				// continue locking tx queue as slave has more data to submit
				_tx_lock();
				
				// forward frame to upper layer for further processing, thereby skipping message type and address
				bt_handle_frame(address, data + BT_FRAME_MESSAGE_TYPE_LENGTH + BT_FRAME_ADDRESS_LENGTH, size - BT_FRAME_MESSAGE_TYPE_LENGTH - BT_FRAME_ADDRESS_LENGTH);
				
			} else {
			
				wd_log_warn("BTMaster::_frame_received_internal() -> received app data without payload!");
			
			}
			
			break;
		
		case BT_MESSAGE_TYPE_APPDATA_ACK:
			wd_log_debug("BTMaster::_frame_received_internal() -> received app data ack");
			
			// slave finished tx process
			_slaveCollection.reportSuccessfulCommunication(address);
			_tx_release();
			
			break;
			
		default:
			wd_log_error("BTMaster::_frame_received_internal() -> unknown message type!");
			break;
		
	}
	
}

void BTMaster::_start(void) {
	
	_state = BT_STATE_INITIAL;
	_mainLoopThread.start(mbed::Callback<void()>(this, &BTMaster::_main_loop));

}

// implement this in the upper layer to handle valid frames
__weak void BTMaster::bt_handle_frame(uint64_t sender, const void * data, size_t size) {}