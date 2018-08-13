/*
 * SerialStreamAdapter.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created: 21.04.2017
 * Author:	Simon Pfeifhofer
 * EMail:	simon.pfeifhofer@world-direct.at
 *
 * Description:
 *		Creates an IOStream-adapter for a BufferedSerial-instance.
 *
 */ 

#include "SerialStreamAdapter.h"
#include "wd_logging.h"
#include "Cellular/core/errors.h"

SerialStreamAdapter::SerialStreamAdapter() : _tx_sem(1) {
	wd_log_debug("SerialStreamAdapter --> ctor");
	_serial = new DMASerial(GSM_TXD, GSM_RXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
	_serial->set_dma_usage_rx(DMA_USAGE_ALWAYS);
	_serial->set_dma_usage_tx(DMA_USAGE_NEVER);
}

int SerialStreamAdapter::abortRead() {
	wd_log_debug("SerialStreamAdapter --> SerialStreamAdapter read aborted");
	_serial->abort_read();
	_reading_started = false;
	return 0;
}

int SerialStreamAdapter::abortWrite() {
	wd_log_debug("SerialStreamAdapter --> abortWrite");
	_serial->abort_write();
	return 0;
}

int SerialStreamAdapter::waitAvailable(uint32_t timeout /* = osWaitForever */) {
	return 0;
}

size_t SerialStreamAdapter::available() {
	wd_log_debug("SerialStreamAdapter --> available");
	return 0;
}

void SerialStreamAdapter::attachRxCallback(Callback<void(dma_frame_meta_t *)> cb) {
	
	this->_serial->attachRxCallback(cb);
	if(!_reading_started){
		start_reading();
	}

}

void SerialStreamAdapter::detachRxCallback(void) {
	this->_serial->detachRxCallback();
}

void SerialStreamAdapter::getFrame(dma_frame_meta_t * frame_meta, char * buffer, int * length)
{
	this->_serial->getFrame(frame_meta, buffer, length);
}

int SerialStreamAdapter::waitSpace(uint32_t timeout /* = osWaitForever */) {
	wd_log_debug("SerialStreamAdapter --> waitSpace");
}

size_t SerialStreamAdapter::space() {
	wd_log_debug("SerialStreamAdapter --> space");
	return 0;
}

void SerialStreamAdapter::write_callback(int a) {
	this->_tx_sem.release();
}

void SerialStreamAdapter::start_reading() {
	_serial->startRead(_dma_buffer, DMA_BUFFER_SIZE);
	_reading_started = true;
}

int SerialStreamAdapter::read(uint8_t* buf, size_t* pLength, size_t maxLength, uint32_t timeout /* = osWaitForever */) {
	
	if(!_reading_started){
		start_reading();
	}
	
	_serial->popFrame(buf, pLength, timeout);
	
	if (*pLength > maxLength) {
		wd_log_error("SerialStreamAdapter: MaxLength exceeded!");	// todo: handle max length limit
	}
	
	if (*pLength == 0) {
		//wd_log_debug("SerialStreamAdapter: Length of frame is 0!");
		return NET_UNKNOWN;
	}
	
	return OK;
	
}

int SerialStreamAdapter::write(uint8_t* buf, size_t length, uint32_t timeout /* = osWaitForever */) {
	wd_log_debug("SerialStreamAdapter --> write");
	
	for(int i = 0; i < length; i++){
        _serial->putc(buf[i]);
    }

//	this->_serial->write(buf, length, callback(this, &SerialStreamAdapter::write_callback));
//	this->_tx_sem.wait(timeout);
//	this->_tx_sem.release();
	
	return OK;
}