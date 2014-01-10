/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//#include <math.h>
#include "spi_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

static const PinMap PinMap_SPI_SCLK[] = {
    {SPI_PSELSCK0 , SPI_0, 0x01},
    {SPI_PSELSCK1, SPI_1, 0x02},
	{SPIS_PSELSCK, SPIS, 0x03},   
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {SPI_PSELMOSI0 , SPI_0, 0x01},
    {SPI_PSELMOSI1, SPI_1, 0x02},
	{SPIS_PSELMOSI, SPIS, 0x03},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {SPI_PSELMISO0 , SPI_0, 0x01},
    {SPI_PSELMISO1, SPI_1, 0x02},
	{SPIS_PSELMISO, SPIS, 0x03},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {SPIS_PSELSS, SPIS, 0x03},
    {NC   , NC   , 0}
};
//   {SPI_PSELSS0 , SPI_0, 0x01},
#define SPIS_MESSAGE_SIZE 1
volatile uint8_t m_tx_buf[SPIS_MESSAGE_SIZE] = {0};//{0,0,0,0};
volatile uint8_t m_rx_buf[SPIS_MESSAGE_SIZE] = {0};//{0,0,0,0};


void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
    // determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    SPIName spi = (SPIName)pinmap_merge(spi_data, spi_cntl);
	//SPIName 
	if(ssel==NC){
		obj->spi = (NRF_SPI_Type*)spi;
		obj->spis = (NRF_SPIS_Type*)NC;
	}
	else{
		obj->spi = (NRF_SPI_Type*)NC;
		obj->spis = (NRF_SPIS_Type*)spi;		
	}
    
    if ((int)obj->spi == NC && (int)obj->spis == NC) {
        error("SPI pinout mapping failed");
    }    
	  // pin out the spi pins	
    if (ssel != NC) {//slave
		obj->spis->POWER=1;

        NRF_GPIO->DIR &= ~(1<<mosi);
		NRF_GPIO->DIR &= ~(1<<miso);
		NRF_GPIO->DIR &= ~(1<<sclk);
		NRF_GPIO->DIR &= ~(1<<ssel);
		obj->spis->PSELMOSI = mosi;
		obj->spis->PSELMISO = miso;
		obj->spis->PSELSCK = sclk;
		obj->spis->PSELCSN = ssel;
		
		obj->spis->EVENTS_END=0;
		obj->spis->EVENTS_ACQUIRED=0;
		obj->spis->MAXRX=SPIS_MESSAGE_SIZE;
		obj->spis->MAXTX=SPIS_MESSAGE_SIZE;
		obj->spis->TXDPTR = (uint32_t)&m_tx_buf[0];
		obj->spis->RXDPTR = (uint32_t)&m_rx_buf[0];  
	   
    }
	else{//master
		obj->spi->POWER=1;
		NRF_GPIO->DIR |= (1<<mosi);	
		obj->spi->PSELMOSI = mosi;
		NRF_GPIO->DIR |= (1<<sclk);
		obj->spi->PSELSCK = sclk;
		NRF_GPIO->DIR &= ~(1<<miso);
		obj->spi->PSELMISO = miso;
		obj->spi->EVENTS_READY = 0U;		
	}
    
    // set default format and frequency
    if (ssel == NC) {
        spi_format(obj, 8, 0, 0);  // 8 bits, mode 0, master
		spi_frequency(obj, 1000000);
    } else {
        spi_format(obj, 8, 0, 1);  // 8 bits, mode 0, slave
    }           
		
}

void spi_free(spi_t *obj) {}

static inline void spi_disable(spi_t *obj, int slave) {
	if(slave){
		obj->spis->ENABLE = (SPIS_ENABLE_ENABLE_Disabled << SPIS_ENABLE_ENABLE_Pos);
	}
	else{
		obj->spi->ENABLE = (SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos);
    }
}

static inline void spi_enable(spi_t *obj, int slave) {
	if(slave){
		obj->spis->ENABLE = (SPIS_ENABLE_ENABLE_Enabled << SPIS_ENABLE_ENABLE_Pos);
	}
	else{
		obj->spi->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
	}
}

void spi_format(spi_t *obj, int bits, int mode, int slave) {
	uint32_t config_mode;
    spi_disable(obj,slave);
    
     if (bits != 8) {
        error("Only 8bits SPI supported");
    }
	
	switch (mode)
    {
        case 0:
            config_mode = (SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos);
            break;
        case 1:
            config_mode = (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos);
            break;
        case 2:
            config_mode = (SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
            break;
        case 3:
            config_mode = (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos) | (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
            break;
        default:
            error("SPI format error");
            break;
    }
	//default to msb first
	if(slave){
		obj->spis->CONFIG = (config_mode | (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos));	
	}
	else{
		obj->spi->CONFIG = (config_mode | (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos));	
	}
    
    spi_enable(obj,slave);
}

void spi_frequency(spi_t *obj, int hz) {
	if((int)obj->spi==NC)
		return;
    spi_disable(obj,0);
	
	if(hz<250000) { //125Khz
		obj->spi->FREQUENCY = (uint32_t)( 0x02000000UL); 
	}
	else if(hz<500000){//250KHz
		obj->spi->FREQUENCY  = (uint32_t)( 0x02000000UL << 1 );
	}
	else if(hz<1000000){//500KHz
		obj->spi->FREQUENCY  = (uint32_t)( 0x02000000UL << 2 );
	}
	else if(hz<2000000){//1Mhz
		obj->spi->FREQUENCY = (uint32_t)( 0x02000000UL << 3 );
	}
	else if(hz<4000000){//2MHz
		obj->spi->FREQUENCY = (uint32_t)( 0x02000000UL << 4 );
	}
	else if(hz<8000000){//4MHz
		obj->spi->FREQUENCY = (uint32_t)( 0x02000000UL << 5 );
	}
	else{//8MHz
		obj->spi->FREQUENCY = (uint32_t)( 0x02000000UL << 6 ); 
	}
	
	spi_enable(obj,0);
}

static inline int spi_readable(spi_t *obj) {
    return (obj->spi->EVENTS_READY == 1);
}

static inline int spi_writeable(spi_t *obj) {
    return (obj->spi->EVENTS_READY == 0);
}


static inline int spi_read(spi_t *obj) {
    while (!spi_readable(obj)); //timeout ?
	obj->spi->EVENTS_READY =0;
    return (int)obj->spi->RXD;
}

int spi_master_write(spi_t *obj, int value) {
	while (!spi_writeable(obj));
    obj->spi->TXD = (uint32_t)value;
    return spi_read(obj);
}

static inline int spis_writeable(spi_t *obj) {	
	return (obj->spis->EVENTS_ACQUIRED==1);
}

int spi_slave_receive(spi_t *obj) {
	//return (((NRF_GPIO->IN >> obj->spis->PSELCSN) & 1UL)==0);
	int res = (obj->spis->EVENTS_END==1);
	obj->spis->EVENTS_END=0;
	return res;
	//return (obj->spis->EVENTS_ACQUIRED==1);
};

int spi_slave_read(spi_t *obj) {
	//&m_rx_buf[0] = obj->spis->RXDPTR;
	int val =m_rx_buf[0];// m_rx_buf[3]<<24 | m_rx_buf[2] <<16 | m_rx_buf[1] <<8  | m_rx_buf[0];
	//m_rx_buf[0] =0;
	//m_rx_buf[1] =0;
	//m_rx_buf[2] =0;
	//m_rx_buf[3] =0;
    return val;//m_rx_buf[3];//obj->spis->RXDPTR;//
}

void spi_slave_write(spi_t *obj, int value) {
	obj->spis->TASKS_ACQUIRE=1;
	while (!spis_writeable(obj)) ;
	//obj->spis->TXDPTR = value;
	m_tx_buf[0]= value & 0xFF;
	//m_tx_buf[1]= value & 0xFF00;
	//m_tx_buf[2]= value & 0xFF0000;
	//m_tx_buf[3]= value & 0xFF000000;
	obj->spis->TXDPTR = (uint32_t)m_tx_buf;
	//obj->spis->RXDPTR = (uint32_t)m_rx_buf;  	
	obj->spis->TASKS_RELEASE=1;
}
