/*
 * snwio.h
 *
 * Created: 04.07.2017 13:13:06
 *  Author: Guenter.Prossliner
 */ 


#ifndef SNWIO_H_
#define SNWIO_H_

#include <stddef.h>

struct snwio_stats {

	int rx_overruns;

	int rx_bytes;
	int tx_bytes;

	int tx_frames;
	int rx_frames_valid;
	int rx_frames_invalid;
};

void snwio_init(void);
void snwio_transfer_frame(const void * data, size_t size);
void snwio_handle_frame(const void * data, size_t size);
void snwio_loop_check(void);
void snwio_get_stats(snwio_stats * stats, bool reset);

#endif /* SNWIO_H_ */