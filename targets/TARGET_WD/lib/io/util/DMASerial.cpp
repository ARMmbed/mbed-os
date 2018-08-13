/*
 * DMASerial.cpp
 *
 * Created: 31.07.2017 11:02:43
 *  Author: Simon.Pfeifhofer
 */ 

#include "DMASerial.h"
extern UART_HandleTypeDef uart_handlers[8];

DMASerial::DMASerial(PinName tx, PinName rx, int baud)
	: RawSerial(tx, rx, baud),
	_rxQueueProcessingThread(
		osPriorityNormal,
        OS_STACK_SIZE,
        NULL, 
		"dma_serial_rx"
	),
	consumer_pointer(0) { }

DMASerial::~DMASerial() {

	detachRxCallback();
	_rxQueueProcessingThread.terminate();
	abort_read();

}

void DMASerial::popFrame(char * buffer, int * length, uint32_t timeout) {
	
	osEvent evt = _dma_rx_frame_queue.get(timeout);
	
	if (evt.status == osEventMail) {

		dma_frame_meta_t * frame_meta = (dma_frame_meta_t *) evt.value.p;

		getFrame(frame_meta, buffer, length);
		
		_dma_rx_frame_queue.free(frame_meta);
		
	} else {
		*length = 0;
		return;
	}
	
}

void DMASerial::getFrame(dma_frame_meta_t * frame_meta, char * buffer, int * length) {
	*length = frame_meta->frame_size;

	if (frame_meta->frame_start_pos + frame_meta->frame_size < frame_meta->buffer_size) {
		memcpy(buffer, frame_meta->buffer + frame_meta->frame_start_pos, frame_meta->frame_size);
	} else {
		int len = frame_meta->buffer_size - frame_meta->frame_start_pos;
		memcpy(buffer, frame_meta->buffer + frame_meta->frame_start_pos, len);
		memcpy(buffer + len, frame_meta->buffer, frame_meta->frame_size - len);
	}
}

void DMASerial::attachRxCallback(Callback<void(dma_frame_meta_t *)> func) {

	if (func){
		this->_rx_cb = func;
		this->_rxQueueProcessingThread.start(mbed::Callback<void()>(this, &DMASerial::_rx_queue_process_loop));
	}

}

void DMASerial::detachRxCallback(void) {
	
	this->_rx_cb = NULL;
	this->_rxQueueProcessingThread.terminate();
	
}

int DMASerial::startRead(uint8_t *buffer, int buffer_size) {
	this->_read_buffer = buffer;
	this->_read_buffer_size = buffer_size;
	
	this->read(
		buffer, 
		buffer_size, 
		callback(this, &DMASerial::_dma_rx_capture), 
		(
			SERIAL_EVENT_RX_IDLE |
			SERIAL_EVENT_RX_OVERFLOW |
			SERIAL_EVENT_RX_PARITY_ERROR |
			SERIAL_EVENT_RX_FRAMING_ERROR
		)
	);
}

void DMASerial::_dma_rx_capture(int evt) {
	
	UART_HandleTypeDef * huart = &uart_handlers[this->_serial.serial.index];
	DMA_HandleTypeDef * hdma = (DMA_HandleTypeDef *) huart->hdmarx;
	
	if (evt & SERIAL_EVENT_RX_IDLE) {	// IDLE line interrupt processing
		
		uint16_t producer_pointer = huart->RxXferSize - __HAL_DMA_GET_COUNTER(hdma);
	
		dma_frame_meta_t * frame_meta = _dma_rx_frame_queue.alloc();
		//wd_log_error("_dma_rx_frame_queue.alloc() %d", this->_serial.serial.index);
		
		size_t frame_size;
		if (consumer_pointer < producer_pointer) {
			frame_size = producer_pointer - consumer_pointer;
			} else if (consumer_pointer > producer_pointer){
			frame_size = huart->RxXferSize - consumer_pointer + producer_pointer;
			} else {
			return;
		}
	
		if (frame_meta != NULL) {
			frame_meta->buffer = huart->pRxBuffPtr;
			frame_meta->buffer_size = huart->RxXferSize;
			frame_meta->frame_start_pos = consumer_pointer;
			frame_meta->frame_size = frame_size;
			if (_dma_rx_frame_queue.put(frame_meta) != osOK) {
				wd_log_error("DMASerial: Unable to enqueue frame!");
			}
		} else {
			wd_log_error("DMASerial: Error allocating memory for frame queue!");

			// TODO: 
			// Inspect why the queue is full
			// It should be processed by _rx_queue_process_loop
			
			mbed_die();
		}
	
		consumer_pointer = (consumer_pointer + frame_size) % huart->RxXferSize;
	} else {	// Error interrupt -> restart read and hence register interrupts and configure DMA
		wd_log_warn("DMASerial: Receiver error (%d) -> restarting read operation", evt);
		consumer_pointer = 0;
		this->abort_read();
		this->read(
			this->_read_buffer, 
			this->_read_buffer_size, 
			callback(this, &DMASerial::_dma_rx_capture), 
			(
				SERIAL_EVENT_RX_IDLE |
				SERIAL_EVENT_RX_OVERFLOW |
				SERIAL_EVENT_RX_PARITY_ERROR |
				SERIAL_EVENT_RX_FRAMING_ERROR
			)
		);
	}
	
}

void DMASerial::_rx_queue_process_loop(void) {
	
	while(true) {
		
		// we will wait here for frame reception
		osEvent evt = _dma_rx_frame_queue.get(osWaitForever);
		
		if (evt.status == osEventMail) {
			
			dma_frame_meta_t * frame_meta = (dma_frame_meta_t *) evt.value.p;
			if (_rx_cb)	{
				this->_rx_cb.call(frame_meta);
			}
			_dma_rx_frame_queue.free(frame_meta);
		}
		
	}

}