///*
// * snwio.c
// *
// * Created: 04.07.2017 13:12:41
// *  Author: Guenter.Prossliner
// */ 
//
//#include "snwio.h"
//#include "snwconf.h"
//#include "mbed.h"
//#include "RawSerial.h"
//#include "serial_api.h"
//#include "snwconf.h"
//#include "swtimer.h"
//#include "Mutex.h"
//#include "wd_logging.h"
//
//extern "C" {
//	// there is no extern "C" in lib_crc.h, so we have to put it here
//	// because I don't want to modify the .h file
//	#include "lib_crc.h"
//}
//
//
//static RawSerial m_serial(SNWIO_CONF_PIN_TX, SNWIO_CONF_PIN_RX, SNWIO_CONF_BAUD);
//static char	m_rx_buffer[SNWIO_CONF_RX_BUFFER_SIZE];
//static char m_tx_buffer[SNWIO_CONF_TX_BUFFER_SIZE];
//static size_t m_tx_size;
//static int m_us_pause_time;
//static snwio_stats m_stats;
//
//// Buffered RX members (I wanted to use DMA, but this is somehow really bad in mbed.... )
//static char m_rxisr_buffer[SNWIO_CONF_RXISR_BUFFER_SIZE];
//static char * m_rxisr_consumer;	// points beyond the last read char
//static char * m_rxisr_producer;	// points beyond the last written char
//
//static Mutex m_txmutex;
//
//static void _serial_interrupt()
//{
//	char c = m_serial.getc();
//	*m_rxisr_producer++ = c;
//
//	if(m_rxisr_producer == m_rxisr_buffer + SNWIO_CONF_RXISR_BUFFER_SIZE)
//		m_rxisr_producer = m_rxisr_buffer;
//}
//
//static int _serial_readable()
//{
//	return m_rxisr_consumer != m_rxisr_producer;
//}
//
//static char _serial_getc()
//{
//	while(!_serial_readable());
//	char c = *m_rxisr_consumer++;
//
//	if(m_rxisr_consumer == m_rxisr_buffer + SNWIO_CONF_RXISR_BUFFER_SIZE)
//		m_rxisr_consumer = m_rxisr_buffer;
//
//	return c;
//}
//
//void snwio_init()
//{
//
//	// calculated pause time
//	//m_us_pause_time = (SNWIO_CONF_FRAME_PAUSE_CHARS * SystemCoreClock) / 
//		//(SNWIO_CONF_BAUD * 10); // 8 Data, 1 Start, 1 Stop-Bit 
//	m_us_pause_time = 3000;
//
//	m_rxisr_consumer = m_rxisr_producer = m_rxisr_buffer;
//
//	// attach RX Interrupt
//	m_serial.attach(mbed::Callback<void()>(_serial_interrupt), SerialBase::RxIrq);
//
//}
//
//// implement this in the upper layer to handle valid frames
//__weak void snwio_handle_frame(const void * data, size_t size) {}
//
//
//typedef struct {
//	char value;
//	struct {
//		int timeout_occured : 1;
//	};
//} _snwio_char_t;
//
//// returns char 0-FF, or -1 if timeout
//static _snwio_char_t _snwio_readchar()
//{
//	_snwio_char_t ret;
//	ret.timeout_occured = 0;
//
//	swtimer_t timer;
//	swtimer_init(&timer);
//
//	while(!swtimer_elapsed(timer, m_us_pause_time)){
//		if(_serial_readable()){
//			char c = _serial_getc();
//			m_stats.rx_bytes++;
//			ret.value = c;
//			return ret;
//		}
//	};
//
//	ret.timeout_occured = 1;
//	return ret;
//}
//
//static _snwio_char_t _snwio_writechar(char c){
//	m_serial.putc(c);
//	// poll for SR (TC bit=6) 0x40
//	 //volatile uint32_t * sr = (volatile uint32_t*)0x40004c00;
//	 //while (!(*sr & 0x40));
//
//	_snwio_char_t echoc = _snwio_readchar();
//	if(echoc.timeout_occured){
//		// THIS IS A FATAL ERROR, WE SHOULD ALWAYS READ OUR CHAR!
//		wd_log_error("_snwio_readchar timeout occurred!");
//		return echoc;
//	}
//
//	return echoc;
//}
//
//static void _snwio_readframe()
//{
//	uint32_t crc = UINT32_MAX;
//
//	int i = 0;
//	for(;i<SNWIO_CONF_RX_BUFFER_SIZE; i++){
//		_snwio_char_t c =  _snwio_readchar();
//		if(c.timeout_occured) {
//			// done with frame!
//			wd_log_error("done with frame, size: %d", i);
//			break;
//		}
//		crc = update_crc_32(crc, c.value);
//
//		m_rx_buffer[i] = c.value;
//	}
//
//	if(i == SNWIO_CONF_RX_BUFFER_SIZE){
//		// OVERRUN!
//		m_stats.rx_overruns++;
//		wd_log_error("OVERRUN");
//		// let's read the rest, so that we may handle the next frame
//		while(!_snwio_readchar().timeout_occured);
//		return;
//	}
//
//	if (i < 4) {
//		// FRAME SIZE ERROR!!! this could be no valid frame, since size < crc.
//		// even a frame < 8 will never happen, but we let it pass, even if size == 0
//		m_stats.rx_frames_invalid++;
//		wd_log_error("Length less than 4, size %d", i);
//	} else if(crc != 0) {
//		// CRC Error!!!
//		m_stats.rx_frames_invalid++;
//		wd_log_error("CRC-error, lenght %d, crc %d, first byte: %x", i, crc, m_rx_buffer[0]);
//	} else {
//		// OK!!!, let it get handled by the upper layer, but we will hide the CRC from the size
//		m_stats.rx_frames_valid++;
//		snwio_handle_frame(m_rx_buffer, i - 4);
//	}
//	
//	
//}
//
//static void _snwio_writeframe()
//{
//
//	m_txmutex.lock();
//
//	uint32_t crc = UINT32_MAX;
//
//	for(size_t i=0; i<m_tx_size; i++){
//		char c = m_tx_buffer[i];
//		if(_snwio_writechar(c).value != c){
//			// collision, may exit early!
//			wd_log_error("_snwio_writeframe collision");
//		}
//		crc = update_crc_32(crc, c);
//	}
//
//	// write the crc
//	union {
//		uint32_t crc;
//		char data[4];
//	} crcdata = {crc};
//
//	for(int i=0; i<4; i++){
//		_snwio_writechar(crcdata.data[i]);
//		m_stats.tx_bytes++;
//	}
//
//	m_tx_size = 0;
//
//	m_txmutex.unlock();
//	
//	// let's wait sync the pause chars, we also could wait "optimistic", but this would required to handle overflow of sysclk
//	wait_us(m_us_pause_time);
//	m_stats.tx_frames++;
//}
//
//void snwio_transfer_frame(const void * data, size_t size)
//{
//	if(size > SNWIO_CONF_TX_BUFFER_SIZE){
//		// OVERFLOW!
//		wd_log_error("Size of bytes to transfer (%d) > SNWIO_CONF_TX_BUFFER_SIZE (%d)!", size, SNWIO_CONF_TX_BUFFER_SIZE);
//		return;
//	}
//
//	m_txmutex.lock();
//
//	memcpy(m_tx_buffer, data, size);
//	m_tx_size = size;	// this will trigger tx in next loop
//
//	m_txmutex.unlock();
//}
//
//void snwio_loop_check()
//{
//	// RX started from other peer?
//	if(_serial_readable()){
//		wd_log_error("snwio_loop_check --> RX");
//		_snwio_readframe();
//	}
//
//	// start tx?
//	if(m_tx_size > 0){
//		wd_log_error("snwio_loop_check --> TX");
//		_snwio_writeframe();
//	}
//}
//
//void snwio_get_stats(snwio_stats * stats, bool reset)
//{
//	// copy over
//	*stats = m_stats;
//
//	if(reset){
//		m_stats = {};
//	}
//
//}