/*
 * spi_api.c
 *
 *  Created on: 02 feb 2019
 *      Author: Antonio
 */


#include "mbed_assert.h"
#include "mbed_error.h"
#include "spi_api.h"

#if DEVICE_SPI
#include "cmsis.h"
#include "pinmap.h"
#include "objects.h"
#include "pin_device.h"


/* Consider 10ms as the default timeout for sending/receving 1 byte */
#define TIMEOUT_1_BYTE 10

#define SPI_DEFAULT_BAUD         1000000
#define SPI_DEFAULT_DUMMY_CHAR   0xFF

#ifndef SPI_BAUDRATE
#define SPI_BAUDRATE SPI_DEFAULT_BAUD
#endif

#ifndef SPI_DUMMY_CHAR
#define SPI_DUMMY_CHAR SPI_DEFAULT_DUMMY_CHAR
#endif


void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel){
	GPIO_InitType GPIO_InitStructure;

	/* Enable SPI and GPIO clocks */
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO | CLOCK_PERIPH_SPI, ENABLE);

	/* Configure SPI pins */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = getGpioPin(mosi); //SDK_EVAL_SPI_PERIPH_OUT_PIN = GPIO_Pin_2 = mosi;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;//SDK_EVAL_SPI_PERIPH_OUT_MODE;
	GPIO_InitStructure.GPIO_Pull = ENABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = getGpioPin(miso); //SDK_EVAL_SPI_PERIPH_IN_PIN  = GPIO_Pin_3 = miso;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;//SDK_EVAL_SPI_PERIPH_IN_MODE;
	GPIO_Init(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = getGpioPin(sclk); //SDK_EVAL_SPI_PERIPH_SCLK_PIN = GPIO_Pin_0 = ck;
	GPIO_InitStructure.GPIO_Mode = Serial0_Mode;//SDK_EVAL_SPI_PERIPH_SCLK_MODE;
	GPIO_Init(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = getGpioPin(ssel); //SPI_CS_MS_DEMO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Output;
	GPIO_InitStructure.GPIO_HighPwr = ENABLE;
	GPIO_Init(&GPIO_InitStructure);
	GPIO_SetBits(getGpioPin(ssel)); //SPI_CS_MS_DEMO_PIN);

	obj->pin_miso = miso;
	obj->pin_mosi = mosi;
	obj->pin_sclk = sclk;
	obj->pin_ssel = ssel;

	obj->init.SPI_Mode = SPI_Mode_Master;
	obj->init.SPI_DataSize = SPI_DataSize_8b ;
	obj->init.SPI_CPOL = SPI_CPOL_Low;
	obj->init.SPI_CPHA = SPI_CPHA_1Edge;
	obj->init.SPI_BaudRate = SPI_BAUDRATE;
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
	obj->init.SPI_CPOL ? GPIO_SetBits(getGpioPin(obj->pin_sclk)) : GPIO_ResetBits(getGpioPin(obj->pin_sclk));
	SPI_Init(&(obj->init));
}

void spi_frequency(spi_t *obj, int hz) {
	obj->init.SPI_BaudRate = hz;
	SPI_Init(&(obj->init));
}

int spi_master_write(spi_t *obj, int value){
	int received_data;
	/* Set communication mode */
	SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);
	//GPIO_ResetBits(getGpioPin(obj->pin_ssel));
	// Write data to send to TX FIFO //
	while(RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
	SPI_SendData(value);
	while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
	received_data = SPI_ReceiveData();
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
	//GPIO_SetBits(getGpioPin(obj->pin_ssel));
	return received_data;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill){

	obj->dummy_char = write_fill;
	SPI_SetDummyCharacter(write_fill);

	GPIO_ResetBits(getGpioPin(obj->pin_ssel));
	for (int i=0; i<=tx_length; i++) {
		// Write data to send to TX FIFO //
		while(RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
		SPI_SendData(tx_buffer[i]);
		while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
		SPI_ReceiveData();
	}
	for (int i=0; i<=rx_length; i++) {
	    while(RESET == SPI_GetFlagStatus(SPI_FLAG_TFE));
	    SPI_SendData(write_fill);
	    while(RESET == SPI_GetFlagStatus(SPI_FLAG_RNE));
	    rx_buffer[i] = SPI_ReceiveData();
	  }
	while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
	//GPIO_SetBits(getGpioPin(obj->pin_ssel));
	return SUCCESS;
}

#endif //DEVICE_SPI
