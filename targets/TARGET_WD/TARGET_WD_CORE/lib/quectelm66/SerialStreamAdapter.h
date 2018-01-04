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

#ifndef SERIALSTREAMADAPTER_H_
#define SERIALSTREAMADAPTER_H_

#include <stdint.h>
#include "mbed.h"
#include "IOStream.h"
#include "DMASerial.h"

#define DMA_BUFFER_SIZE						DMASERIAL_RX_BUFFER_SIZE
#define DMA_READ_SEM_COMPLETE_TIMEOUT_MS	100
#define DMA_WRITE_SEM_COMPLETE_TIMEOUT_MS	100

class SerialStreamAdapter : public IOStream
{
	
	private:
	
		char _dma_buffer[DMA_BUFFER_SIZE] = {};
		bool _reading_started = false;
        
		DMASerial* _serial;
		Semaphore _tx_sem;
		void start_reading(void);
		void write_callback(int a);
	
	public: 
		
		SerialStreamAdapter();
		virtual int abortRead() override;
		virtual int read(uint8_t* buf, size_t* pLength, size_t maxLength, uint32_t timeout /* = osWaitForever */) override;
		virtual int waitAvailable(uint32_t timeout /* = osWaitForever */) override;
		virtual int abortWrite() override;
		virtual int waitSpace(uint32_t timeout /* = osWaitForever */) override;
		virtual int write(uint8_t* buf, size_t length, uint32_t timeout /* = osWaitForever */) override;
		virtual size_t space() override;
		virtual size_t available() override;
		void attachRxCallback(Callback<void(dma_frame_meta_t *)> cb);
		void detachRxCallback(void);
		void getFrame(dma_frame_meta_t * frame_meta, char * buffer, int * length);
};

#endif // SERIALSTREAMADAPTER_H_