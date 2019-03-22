
#include "mbed_assert.h"
#include "mbed_error.h"
#include "spi_api.h"



#if DEVICE_SPI
#include "cmsis.h"
#include "pinmap.h"
#include "objects.h"
//#include "pin_device.h"

#include "BlueNRG1_sysCtrl.h"
#include "hal_types.h"

//assertations

/* Consider 10ms as the default timeout for sending/receving 1 byte */
#define TIMEOUT_1_BYTE 10

#define SPI_SPEED 1000000

#define SPI_DUMMY_CHAR 0xFF

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel){

	GPIO_InitType GPIO_InitStructure;

	/* Enable SPI and GPIO clocks */
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO | CLOCK_PERIPH_SPI, ENABLE);

	/* Configure SPI pins */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = mosi; //SDK_EVAL_SPI_PERIPH_OUT_PIN = GPIO_Pin_2 = mosi;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;//SDK_EVAL_SPI_PERIPH_OUT_MODE;
	GPIO_InitStructure.GPIO_Pull = ENABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = miso; //SDK_EVAL_SPI_PERIPH_IN_PIN  = GPIO_Pin_3 = miso;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;//SDK_EVAL_SPI_PERIPH_IN_MODE;
	GPIO_Init(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = sclk; //SDK_EVAL_SPI_PERIPH_SCLK_PIN = GPIO_Pin_0 = ck;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;//SDK_EVAL_SPI_PERIPH_SCLK_MODE;
	GPIO_Init(&GPIO_InitStructure);
	if(ssel != NC){
		GPIO_InitStructure.GPIO_Pin = ssel; //SPI_CS_MS_DEMO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Output;
		GPIO_InitStructure.GPIO_HighPwr = ENABLE;
		GPIO_Init(&GPIO_InitStructure);
		GPIO_WriteBit(ssel, 1);
	}
	obj->pin_miso = miso;
	obj->pin_mosi = mosi;
	obj->pin_sclk = sclk;
	obj->pin_ssel = ssel;

	obj->init.SPI_Mode = SPI_Mode_Master;
	obj->init.SPI_DataSize = SPI_DataSize_8b ;
	obj->init.SPI_CPOL = SPI_CPOL_Low;
	obj->init.SPI_CPHA = SPI_CPHA_1Edge;
	obj->init.SPI_BaudRate = SPI_SPEED;
	SPI_Init(&(obj->init));

	// Clear RX and TX FIFO //
	SPI_ClearTXFIFO();
	SPI_ClearRXFIFO();

	// Set null character //
	obj->dummy_char = SPI_DUMMY_CHAR;
	SPI_SetDummyCharacter(obj->dummy_char);

	// Set communication mode //
	SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);

	// Enable SPI functionality //
	SPI_Cmd(ENABLE);
}

void spi_free(spi_t *obj){
	obj->dummy_char = 0;
	obj->pin_miso = NC;
	obj->pin_mosi = NC;
	obj->pin_sclk = NC;
	obj->pin_ssel = NC;
	obj->init.SPI_BaudRate = 0;
	obj->init.SPI_CPHA = 0;
	obj->init.SPI_CPOL = 0;
	obj->init.SPI_DataSize = 0;
	obj->init.SPI_Mode = 0;
	SPI_DeInit();
}

void spi_format(spi_t *obj, int bits, int mode, int slave){

	/* Configure SPI in master mode */
	obj->init.SPI_Mode = (slave) ? SPI_Mode_Slave : SPI_Mode_Master;
	obj->init.SPI_DataSize = (uint8_t) bits-1;

	switch (mode) {
	        case 0:
	        	obj->init.SPI_CPOL = SPI_CPOL_Low;
	        	obj->init.SPI_CPHA = SPI_CPHA_1Edge;
	            break;
	        case 1:
	        	obj->init.SPI_CPOL = SPI_CPOL_Low;
	        	obj->init.SPI_CPHA = SPI_CPHA_2Edge;
	            break;
	        case 2:
	        	obj->init.SPI_CPOL = SPI_CPOL_High;
	        	obj->init.SPI_CPHA = SPI_CPHA_1Edge;
	            break;
	        case 3:
	        	obj->init.SPI_CPOL = SPI_CPOL_High;
	        	obj->init.SPI_CPHA = SPI_CPHA_2Edge;
	            break;
	        default :
	        	break;

	    }
	obj->init.SPI_CPOL ? GPIO_SetBits(obj->pin_sclk) : GPIO_ResetBits(obj->pin_sclk);
	SPI_Init(&(obj->init));

}



void spi_frequency(spi_t *obj, int hz) {
	obj->init.SPI_BaudRate = hz;
	SPI_Init(&(obj->init));
}

int spi_master_write(spi_t *obj, int value){

	int received_data;

	// Set communication mode //
	SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);

	// start write: CS=0
	//GPIO_WriteBit(obj->pin_ssel, 0);

	// Write data to send to TX FIFO //
	while(RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));

	SPI_SendData(value);

	// read RX FIFO data //
	while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
	received_data = SPI_ReceiveData();

	// wait busy
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));

	// stop write: CS=1
	//GPIO_WriteBit(obj->pin_ssel, 1);
	return received_data;
}



int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill){

	obj->dummy_char = write_fill;
	SPI_SetDummyCharacter(write_fill);

	// Set communication mode //
	//SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);

	// start write: CS=0
	//GPIO_WriteBit(obj->pin_ssel, 0);
	// TX BUFFER //

	for (int i=0; i<tx_length; i++) {
		// Write data to send to TX FIFO //
		while(RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
		SPI_SendData(tx_buffer[i]);
		// discard RX FIFO
		while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
		SPI_ReceiveData();
	}

	// wait busy
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));

	// RX BUFFER //
	if (!rx_length)
		rx_buffer[0] = 0;
	else{
		for (int i=0; i<rx_length; i++) {
			// Write dummy_byte to send to TX FIFO //
			while(RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
			SPI_SendData(write_fill);

			// read RX FIFO data //
			while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
			rx_buffer[i] = SPI_ReceiveData();
		}
	}

	// wait busy
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));

	// stop write: CS=1
	//GPIO_WriteBit(obj->pin_ssel, 1);

	return tx_length > rx_length ? tx_length : rx_length;
}

int spi_slave_receive(spi_t *obj){
	return SPI_GetFlagStatus(SPI_FLAG_RNE); // != 0) ? 1: 0;
}

int spi_slave_read(spi_t *obj){
	  while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
	  return SPI_ReceiveData();
}

void spi_slave_write(spi_t *obj, int value){
	while(RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
	SPI_SendData(value);
}

int  spi_busy(spi_t *obj){
	return SPI_GetFlagStatus(SPI_FLAG_BSY);
}

#ifdef ANTONIO_armclang
uint8_t spi_get_module(spi_t *obj){
	return 1;
}

const PinMap *spi_master_mosi_pinmap(void){
	return 0;
}

const PinMap *spi_master_miso_pinmap(void){
	return 0;
}

const PinMap *spi_master_clk_pinmap(void){
	return 0;
}

const PinMap *spi_master_cs_pinmap(void){
	return 0;
}

const PinMap *spi_slave_mosi_pinmap(void){
	return 0;
}

const PinMap *spi_slave_miso_pinmap(void){
	return 0;
}

const PinMap *spi_slave_clk_pinmap(void){
	return 0;
}

const PinMap *spi_slave_cs_pinmap(void){
	return 0;
}
#endif //antonio_armclang


#ifdef DEVICE_SPI_ASYNCH

/// @returns the number of bytes transferred, or `0` if nothing transferred
static int spi_master_start_asynch_transfer(spi_t *obj, transfer_type_t transfer_type, const void *tx, void *rx, size_t length){

}

// asynchronous API
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint){}

inline uint32_t spi_irq_handler_asynch(spi_t *obj){}

uint8_t spi_active(spi_t *obj){}

void spi_abort_asynch(spi_t *obj){}

#endif //DEVICE_SPI_ASYNCH

#endif  //DEVICE_SPI
