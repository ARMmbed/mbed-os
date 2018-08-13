/* 
* BTBase.cpp
*
* Created: 29.11.2017 13:49:27
* Author: Patrick.Frischmann
*/


#include "BTBase.h"

extern "C" {
	// there is no extern "C" in lib_crc.h, so we have to put it here
	// because I don't want to modify the .h file
	#include "lib_crc.h"
}

static void donothing(void) {}

// default constructor
BTBase::BTBase(PinName Tx, PinName Rx, PinName Irq, PinName Led, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
	: _tx_producer_pointer(0),
	_tx_complete_sem(0),
	_tx_echo_received_sem(0),
	_tx_buffer_mutex(),
	_send_queue_mutex(),
	_activity_led(Led, 0) {
	
	wd_log_heap_stats("BTBase(1)");
	this->_activity_led_timeout = new ResettableTimeout(callback(this, &BTBase::_on_activity_led_timeout), 40000);
	this->_activity_led_timeout->stop();
	
	// init
	wd_log_heap_stats("BTBase(2)");
	this->_rx_buffer = new char[BT_BUFFER_SIZE]();
	this->_tx_buffer = new char[BT_BUFFER_SIZE]();
	this->_tx_frame_buffer = new char[BT_BUFFER_SIZE/2]();
	wd_log_heap_stats("BTBase(3)");

	// configure DMA usage
	this->_dmaSerial = new DMASerial(Tx, Rx, baud);
	wd_log_heap_stats("BTBase(4)");
	this->_dmaSerial->set_dma_usage_rx(DMA_USAGE_ALWAYS);
	wd_log_heap_stats("BTBase(5)");
	this->_dmaSerial->set_dma_usage_tx(DMA_USAGE_ALWAYS);
	wd_log_heap_stats("BTBase(6)");
	this->_dmaSerial->attachRxCallback(Callback<void(dma_frame_meta_t *)>(this, &BTBase::_on_rx_frame_received));
	wd_log_heap_stats("BTBase(7)");
	
	bt_attach_discovery_finished_cb(donothing);
		
} //BTBase

// default destructor
BTBase::~BTBase() {
	
	delete this->_rx_buffer;
	delete this->_tx_frame_buffer;
	delete this->_tx_buffer;
	delete this->_dmaSerial;
	
} //~BTBase

void BTBase::_indicate_activity(void) {
	if (_state != BT_STATE_READY) return;	// led is only switch on after discovery
	
	this->_activity_led.write(0);
	this->_activity_led_timeout->reset();
}

void BTBase::_tx_buffer_cpy(const void * src, size_t num) {
	
	if (num == 0) return;
	
	_tx_buffer_mutex.lock();
		
	if (this->_tx_producer_pointer + num < BT_BUFFER_SIZE) {
		memcpy(this->_tx_buffer + this->_tx_producer_pointer, src, num);
		this->_tx_producer_pointer += num;
	} else {
		int len = BT_BUFFER_SIZE - this->_tx_producer_pointer;
		memcpy(this->_tx_buffer + this->_tx_producer_pointer, src, len);
		memcpy(this->_tx_buffer, (uint8_t *)src + len, num - len);
		this->_tx_producer_pointer = num - len;
	}
	
	_tx_buffer_mutex.unlock();
	
}

void BTBase::_on_rx_frame_received(dma_frame_meta_t * frame_meta) {
	
	if (frame_meta->frame_size < 5) {// crc already 4 bytes
		wd_log_warn("BTBase::_on_rx_frame_received() -> frame is too short to be valid (length: %d)", frame_meta->frame_size);
		return;
	}
	
	// read frame
	char rxBuffer[frame_meta->frame_size] = {};
	size_t rxSize;
	_dmaSerial->getFrame(frame_meta, rxBuffer, &rxSize);
	
	// crc check
	uint32_t crc = UINT32_MAX;
	for (int i = 0; i < rxSize; i++) {
		crc = update_crc_32(crc, rxBuffer[i]);
	}
	
	if (crc != 0) {
		wd_log_error("BTBase::_on_rx_frame_received() -> discarding frame due to crc error (length: %d, crc: %x, first byte: %x)", rxSize, crc, rxBuffer[0]);
		return;
	}
	
	// echo check
	if (rxSize == _tx_frame_size) { // only perform echo check if frame sizes match
		
		if (memcmp(_tx_frame_buffer, rxBuffer, rxSize) == 0) { // Tx echo
			wd_log_debug("BTBase::_on_rx_frame_received() -> discarding frame due to echo detection (length: %d, crc: %x, first byte: %x)", rxSize, crc, rxBuffer[0]);
//			memset(_tx_frame_buffer, 0, rxSize); // clean tx buffer
			this->_tx_echo_received_sem.release();
			return;
		}
		
	}
	
	// exclude crc for next layer
	_frame_received_internal(rxBuffer, rxSize-4);
	
}

void BTBase::_tx_buffer_flush(size_t size) {

	// crc computation
	uint32_t crc = UINT32_MAX;
	
	for (size_t i = 0; i < size; i++) {
		char c = ((char*)this->_tx_frame_buffer)[i];
		crc = update_crc_32(crc, c);
	}
	
	// write the crc
	union {
		uint32_t crc;
		char data[4];
	} crcdata = { crc };
		
	memcpy(this->_tx_frame_buffer + size, crcdata.data, 4);
	size += 4;
		
	this->_tx_frame_size = size;
		
	// send frame
	this->_dmaSerial->write(this->_tx_frame_buffer, this->_tx_frame_size, callback(this, &BTBase::_tx_complete));
		
	// wait for transmit completion
	this->_tx_complete_sem.wait(BT_TX_WRITE_TIMEOUT_MS);
		
	// wait for echo reception
	this->_tx_echo_received_sem.wait(BT_TX_ECHO_TIMEOUT_MS);

}

void BTBase::_tx_frame_queue_clear(void) {

	for(;;) {
		
		osEvent evt = _dma_tx_frame_queue.get(1);
		wd_log_debug("GET frame");
		
		if (evt.status == osEventMail) {
			dma_frame_meta_t * frame_meta = (dma_frame_meta_t *) evt.value.p;
			_dma_tx_frame_queue.free(frame_meta);
		} else {
			break;
		}
		
	}

}

void BTBase::_start_rx_processing(){

	// start rx processing
	this->_dmaSerial->startRead(
		this->_rx_buffer,
		BT_BUFFER_SIZE
	);

}

void BTBase::bt_start(uint64_t id) {
	
	// set board id
	this->_id = id;
	
	this->_start_rx_processing();

	this->_start();
	
}

void BTBase::bt_transmit_frame(const void * data, size_t size, uint64_t id) {

	if (_state == BT_STATE_READY) {
		this->_send_enqueue(BT_MESSAGE_TYPE_APPDATA, id == 0 ? _id : id, data, size);
	}

}

void BTBase::bt_await_ready_state(uint32_t timeout) {

	while(timeout-- > 0 && _state != BT_STATE_READY){
		wait_ms(1);
	}

}

void BTBase::bt_attach_discovery_finished_cb(Callback<void()> cb) {
	
	if (cb){
		_discoveryFinishedCallback = cb;
		} else {
		_discoveryFinishedCallback = donothing;
	}
	
}

char BTBase::_get_message_type(const char * data) {
	
	return data[BT_FRAME_MESSAGE_TYPE_OFFSET];
	
}

uint64_t BTBase::_get_address(const char * data) {

	uint64_t id;
	std::memcpy(&id, data + BT_FRAME_ADDRESS_OFFSET, BT_FRAME_ADDRESS_LENGTH);
	return id;
	
}

void BTBase::_send_enqueue(uint8_t messageType, uint64_t slaveId, const void * data, size_t size) {

//	wd_log_error("BTBase::_send_enqueue() -> entry");
	
	_send_queue_mutex.lock();

	dma_frame_meta_t * frame_meta = _dma_tx_frame_queue.alloc();
	if (frame_meta != NULL) {
		frame_meta->buffer = this->_tx_buffer;
		frame_meta->buffer_size = BT_BUFFER_SIZE;
		frame_meta->frame_start_pos = this->_tx_producer_pointer;
		frame_meta->frame_size = 0;
		
		// copy message type
		_tx_buffer_cpy(&messageType, BT_FRAME_MESSAGE_TYPE_LENGTH);
		frame_meta->frame_size += BT_FRAME_MESSAGE_TYPE_LENGTH;
		
		// copy id
		_tx_buffer_cpy(&slaveId, BT_FRAME_ADDRESS_LENGTH);
		frame_meta->frame_size += BT_FRAME_ADDRESS_LENGTH;
		
		// copy payload
		_tx_buffer_cpy(data, size);
		frame_meta->frame_size += size;
		
		// crc computation will be performed in final processing loop
		
		if (_dma_tx_frame_queue.put(frame_meta) != osOK) {
			wd_log_error("BTBase::_send_enqueue() -> Unable to enqueue frame!");
		}

		wd_log_debug("PUT frame %d bytes", frame_meta->frame_size);
		
	} else {
		wd_log_error("BTBase::_send_enqueue() -> Error allocating memory for frame queue!");
	}
	
	_send_queue_mutex.unlock();
	
}
