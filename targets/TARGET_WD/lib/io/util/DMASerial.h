/*
 * DMASerial.h
 *
 * Created: 31.07.2017 11:01:37
 *  Author: Simon.Pfeifhofer
 */ 


#ifndef DMASERIAL_H_
#define DMASERIAL_H_

#include "mbed.h"
#include "device.h"
#include "rtos.h"
#include "wd_logging.h"

#define DMASERIAL_RX_BUFFER_SIZE	4096
#define DMASERIAL_RX_QUEUE_SIZE		40	// huge amount of small rx due to addressing

typedef struct {
	uint8_t * buffer;
	size_t buffer_size;
	unsigned int frame_start_pos;
	size_t frame_size;
} dma_frame_meta_t;

class DMASerial : public RawSerial{
    
public:
    DMASerial(PinName tx, PinName rx, int baud);
	~DMASerial();
	
	void popFrame(char * buffer, int * length, uint32_t timeout = osWaitForever);
	void getFrame(dma_frame_meta_t * frame_meta, char * buffer, int * length);
	void attachRxCallback(Callback<void(dma_frame_meta_t *)> func);
	void detachRxCallback(void);
	int startRead(uint8_t *buffer, int buffer_size);

private:
	Mail<dma_frame_meta_t, DMASERIAL_RX_QUEUE_SIZE> _dma_rx_frame_queue;
	int consumer_pointer;
	uint8_t * _read_buffer;
	int _read_buffer_size;
	
	Thread _rxQueueProcessingThread;
	
	Callback<void(dma_frame_meta_t *)> _rx_cb; 
	
	void _dma_rx_capture(int evt);
	void _rx_queue_process_loop(void);
	
};


#endif /* DMASERIAL_H_ */