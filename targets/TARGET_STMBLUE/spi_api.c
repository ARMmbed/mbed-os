/*
 * spi_api.c
 *
 *  Created on: 02 feb 2018
 *      Author: Gianluca
 */


#include "mbed_assert.h"
#include "mbed_error.h"
#include "spi_api.h"

#if DEVICE_SPI
#include "cmsis.h"
#include "pinmap.h"
//#include "PeripheralPins.h"
//#include "spi_device.h"
#include "objects.h"
#include "pin_device.h"
#include "serial_mylib.h"


/* Consider 10ms as the default timeout for sending/receving 1 byte */
#define TIMEOUT_1_BYTE 10


//void init_spi(spi_t *obj){
//}

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

	/*
	// Configure SPI in master mode //
	//SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_BaudRate = 100000;
	//SPI_Init(&SPI_InitStructure);
	obj->init = &SPI_InitStructure;
	SPI_Init(obj->init);
	*/

	/*
	obj->init->SPI_Mode = SPI_Mode_Master;
	obj->init->SPI_DataSize = SPI_DataSize_8b;
	obj->init->SPI_CPOL = SPI_CPOL_Low;
	obj->init->SPI_CPHA = SPI_CPHA_1Edge;
	obj->init->SPI_BaudRate = 100000;
	SPI_Init(obj->init);
	*/
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
	//obj->init=NULL;
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

#ifdef cccc
/*
 * Only the IP clock input is family dependant so it computed
 * separately in spi_get_clock_freq
 */
extern int spi_get_clock_freq(spi_t *obj);

static const uint16_t baudrate_prescaler_table[] =	{SPI_BAUDRATEPRESCALER_2,
                                                    SPI_BAUDRATEPRESCALER_4,
                                                    SPI_BAUDRATEPRESCALER_8,
                                                    SPI_BAUDRATEPRESCALER_16,
                                                    SPI_BAUDRATEPRESCALER_32,
                                                    SPI_BAUDRATEPRESCALER_64,
                                                    SPI_BAUDRATEPRESCALER_128,
                                                    SPI_BAUDRATEPRESCALER_256};
#endif //ccc

void spi_frequency(spi_t *obj, int hz) {
	obj->init.SPI_BaudRate = hz;
	SPI_Init(&(obj->init));
}



#ifdef aaa
static inline int ssp_readable(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Check if data is received
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;
}

static inline int ssp_writeable(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Check if data is transmitted
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}

static inline int ssp_busy(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_BSY) != RESET) ? 1 : 0);
    return status;
}
#endif //aaaa

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


#ifdef bbbb
int spi_slave_receive(spi_t *obj)
{
    return ((ssp_readable(obj) && !ssp_busy(obj)) ? 1 : 0);
};

int spi_slave_read(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    while (!ssp_readable(obj));
    if (handle->Init.DataSize == SPI_DATASIZE_16BIT) {
        return LL_SPI_ReceiveData16(SPI_INST(obj));
    } else {
        return LL_SPI_ReceiveData8(SPI_INST(obj));
    }
}

void spi_slave_write(spi_t *obj, int value)
{
    SPI_TypeDef *spi = SPI_INST(obj);
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    while (!ssp_writeable(obj));
    if (handle->Init.DataSize == SPI_DATASIZE_8BIT) {
        // Force 8-bit access to the data register
        uint8_t *p_spi_dr = 0;
        p_spi_dr = (uint8_t *) & (spi->DR);
        *p_spi_dr = (uint8_t)value;
    } else { // SPI_DATASIZE_16BIT
        spi->DR = (uint16_t)value;
    }
}

int spi_busy(spi_t *obj)
{
    return ssp_busy(obj);
}

#ifdef DEVICE_SPI_ASYNCH
typedef enum {
    SPI_TRANSFER_TYPE_NONE = 0,
    SPI_TRANSFER_TYPE_TX = 1,
    SPI_TRANSFER_TYPE_RX = 2,
    SPI_TRANSFER_TYPE_TXRX = 3,
} transfer_type_t;


/// @returns the number of bytes transferred, or `0` if nothing transferred
static int spi_master_start_asynch_transfer(spi_t *obj, transfer_type_t transfer_type, const void *tx, void *rx, size_t length)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    bool is16bit = (handle->Init.DataSize == SPI_DATASIZE_16BIT);
    // the HAL expects number of transfers instead of number of bytes
    // so for 16 bit transfer width the count needs to be halved
    size_t words;

    DEBUG_PRINTF("SPI inst=0x%8X Start: %u, %u\r\n", (int)handle->Instance, transfer_type, length);

    obj->spi.transfer_type = transfer_type;

    if (is16bit) {
        words = length / 2;
    } else {
        words = length;
    }

    // enable the interrupt
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_DisableIRQ(irq_n);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_SetPriority(irq_n, 1);
    NVIC_EnableIRQ(irq_n);

    // enable the right hal transfer
    int rc = 0;
    switch(transfer_type) {
        case SPI_TRANSFER_TYPE_TXRX:
            rc = HAL_SPI_TransmitReceive_IT(handle, (uint8_t*)tx, (uint8_t*)rx, words);
            break;
        case SPI_TRANSFER_TYPE_TX:
            rc = HAL_SPI_Transmit_IT(handle, (uint8_t*)tx, words);
            break;
        case SPI_TRANSFER_TYPE_RX:
            // the receive function also "transmits" the receive buffer so in order
            // to guarantee that 0xff is on the line, we explicitly memset it here
            memset(rx, SPI_FILL_WORD, length);
            rc = HAL_SPI_Receive_IT(handle, (uint8_t*)rx, words);
            break;
        default:
            length = 0;
    }

    if (rc) {
        DEBUG_PRINTF("SPI: RC=%u\n", rc);
        length = 0;
    }

    return length;
}

// asynchronous API
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // TODO: DMA usage is currently ignored
    (void) hint;

    // check which use-case we have
    bool use_tx = (tx != NULL && tx_length > 0);
    bool use_rx = (rx != NULL && rx_length > 0);
    bool is16bit = (handle->Init.DataSize == SPI_DATASIZE_16BIT);

    // don't do anything, if the buffers aren't valid
    if (!use_tx && !use_rx)
        return;

    // copy the buffers to the SPI object
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = is16bit ? 16 : 8;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = obj->tx_buff.width;

    obj->spi.event = event;

    DEBUG_PRINTF("SPI: Transfer: %u, %u\n", tx_length, rx_length);

    // register the thunking handler
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_SetVector(irq_n, (uint32_t)handler);

    // enable the right hal transfer
    if (use_tx && use_rx) {
        // we cannot manage different rx / tx sizes, let's use smaller one
        size_t size = (tx_length < rx_length)? tx_length : rx_length;
        if(tx_length != rx_length) {
            DEBUG_PRINTF("SPI: Full duplex transfer only 1 size: %d\n", size);
            obj->tx_buff.length = size;
            obj->rx_buff.length = size;
        }
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TXRX, tx, rx, size);
    } else if (use_tx) {
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TX, tx, NULL, tx_length);
    } else if (use_rx) {
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_RX, NULL, rx, rx_length);
    }
}

inline uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    int event = 0;

    // call the CubeF4 handler, this will update the handle
    HAL_SPI_IRQHandler(&obj->spi.handle);

    if (obj->spi.handle.State == HAL_SPI_STATE_READY) {
        // When HAL SPI is back to READY state, check if there was an error
        int error = obj->spi.handle.ErrorCode;
        if(error != HAL_SPI_ERROR_NONE) {
            // something went wrong and the transfer has definitely completed
            event = SPI_EVENT_ERROR | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;

            if (error & HAL_SPI_ERROR_OVR) {
                // buffer overrun
                event |= SPI_EVENT_RX_OVERFLOW;
            }
        } else {
            // else we're done
            event = SPI_EVENT_COMPLETE | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
       }
       // enable the interrupt
       NVIC_DisableIRQ(obj->spi.spiIRQ);
       NVIC_ClearPendingIRQ(obj->spi.spiIRQ);
    }


    return (event & (obj->spi.event | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE));
}

uint8_t spi_active(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    HAL_SPI_StateTypeDef state = HAL_SPI_GetState(handle);

    switch(state) {
        case HAL_SPI_STATE_RESET:
        case HAL_SPI_STATE_READY:
        case HAL_SPI_STATE_ERROR:
            return 0;
        default:
            return 1;
    }
}

void spi_abort_asynch(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // disable interrupt
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);

    // clean-up
    __HAL_SPI_DISABLE(handle);
    HAL_SPI_DeInit(handle);
    HAL_SPI_Init(handle);
    __HAL_SPI_ENABLE(handle);
}
#endif //bbbb

#endif //DEVICE_SPI_ASYNCH

#endif
