/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include <math.h>

#include "spi_api.h"
#include "spi_def.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"

static const PinMap PinMap_SPI_SCLK[] = {
    {SCLK_SPI , SPI_0, 0},
    {CLCD_SCLK , SPI_1, 0},
    {ADC_SCLK , SPI_2, 0},
    {SHIELD_0_SPI_SCK , SPI_3, 0},
    {SHIELD_1_SPI_SCK , SPI_4, 0},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {MOSI_SPI, SPI_0, 0},
    {CLCD_MOSI, SPI_1, 0},
    {ADC_MOSI, SPI_2, 0},
    {SHIELD_0_SPI_MOSI, SPI_3, 0},
    {SHIELD_1_SPI_MOSI, SPI_4, 0},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {MISO_SPI, SPI_0, 0},
    {CLCD_MISO, SPI_1, 0},
    {ADC_MISO, SPI_2, 0},
    {SHIELD_0_SPI_MISO, SPI_3, 0},
    {SHIELD_1_SPI_MISO, SPI_4, 0},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {SSEL_SPI, SPI_0, 0},
    {CLCD_SSEL, SPI_1, 0},
    {ADC_SSEL, SPI_2, 0},
    {SHIELD_0_SPI_nCS, SPI_3, 0},
    {SHIELD_1_SPI_nCS, SPI_4, 0},
    {NC   , NC   , 0}
};

static inline int ssp_disable(spi_t *obj);
static inline int ssp_enable(spi_t *obj);

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
    
        int altfunction[4];
        // determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
    obj->spi = (MPS2_SSP_TypeDef*)pinmap_merge(spi_data, spi_cntl);
    if ((int)obj->spi == NC) {
        error("SPI pinout mapping failed");
    }
    
    // enable power and clocking
    switch ((int)obj->spi) {
      case (int)SPI_0: 
            obj->spi->CR1       = 0;
            obj->spi->CR0       = SSP_CR0_SCR_DFLT | SSP_CR0_FRF_MOT | SSP_CR0_DSS_8;
            obj->spi->CPSR      = SSP_CPSR_DFLT; 
            obj->spi->IMSC      = 0x8; 
            obj->spi->DMACR     = 0;
            obj->spi->CR1       = SSP_CR1_SSE_Msk;
            obj->spi->ICR       = 0x3;  
            break;
      case (int)SPI_1:
                      /* Configure SSP used for LCD                                               */
            obj->spi->CR1   =   0;                 /* Synchronous serial port disable  */
            obj->spi->DMACR =   0;                 /* Disable FIFO DMA                 */
            obj->spi->IMSC  =   0;                 /* Mask all FIFO/IRQ interrupts     */
            obj->spi->ICR   = ((1ul <<  0) |       /* Clear SSPRORINTR interrupt       */
                                (1ul <<  1) );      /* Clear SSPRTINTR interrupt        */
              obj->spi->CR0   = ((7ul <<  0) |       /* 8 bit data size                  */
                                (0ul <<  4) |       /* Motorola frame format            */
                                (0ul <<  6) |       /* CPOL = 0                         */
                                (0ul <<  7) |       /* CPHA = 0                         */
                                (1ul <<  8) );      /* Set serial clock rate            */
            obj->spi->CPSR  =  (2ul <<  0);        /* set SSP clk to 6MHz (6.6MHz max) */
            obj->spi->CR1   = ((1ul <<  1) |       /* Synchronous serial port enable   */
                                (0ul <<  2) );      /* Device configured as master      */
            break;
      case (int)SPI_2: 
            obj->spi->CR1       = 0;
            obj->spi->CR0       = SSP_CR0_SCR_DFLT | SSP_CR0_FRF_MOT | SSP_CR0_DSS_8;
            obj->spi->CPSR      = SSP_CPSR_DFLT; 
            obj->spi->IMSC      = 0x8; 
            obj->spi->DMACR     = 0;
            obj->spi->CR1       = SSP_CR1_SSE_Msk;
            obj->spi->ICR       = 0x3;  
            break;
      case (int)SPI_3: 
            obj->spi->CR1       = 0;
            obj->spi->CR0       = SSP_CR0_SCR_DFLT | SSP_CR0_FRF_MOT | SSP_CR0_DSS_8;
            obj->spi->CPSR      = SSP_CPSR_DFLT; 
            obj->spi->IMSC      = 0x8; 
            obj->spi->DMACR     = 0;
            obj->spi->CR1       = SSP_CR1_SSE_Msk;
            obj->spi->ICR       = 0x3;  
            break;
      case (int)SPI_4: 
            obj->spi->CR1       = 0;
            obj->spi->CR0       = SSP_CR0_SCR_DFLT | SSP_CR0_FRF_MOT | SSP_CR0_DSS_8;
            obj->spi->CPSR      = SSP_CPSR_DFLT; 
            obj->spi->IMSC      = 0x8; 
            obj->spi->DMACR     = 0;
            obj->spi->CR1       = SSP_CR1_SSE_Msk;
            obj->spi->ICR       = 0x3;  
            break;
    }
    
        if(mosi != NC){ altfunction[0] = 1;}else{ altfunction[0] = 0;}
        if(miso != NC){ altfunction[1] = 1;}else{ altfunction[1] = 0;}
        if(sclk != NC){ altfunction[2] = 1;}else{ altfunction[2] = 0;}
        if(ssel != NC){ altfunction[3] = 1;}else{ altfunction[3] = 0;}
        
    // enable alt function
    switch ((int)obj->spi) {
      case (int)SPI_2:
                CMSDK_GPIO1->ALTFUNCSET |= (altfunction[2]<<5 | altfunction[0]<<4 | altfunction[1]<<3 | altfunction[3]<<2);
            break;
      case (int)SPI_3:
                CMSDK_GPIO0->ALTFUNCSET |= (altfunction[2]<<13 | altfunction[1]<<12 | altfunction[0]<<11 | altfunction[3]<<10);
            break;
      case (int)SPI_4:
                CMSDK_GPIO4->ALTFUNCSET |= (altfunction[2]<<3 | altfunction[1]<<2 | altfunction[0]<<1 | altfunction[3]);
            break;
        }
        
    // set default format and frequency
    if (ssel == NC) {
        spi_format(obj, 8, 0, 0);  // 8 bits, mode 0, master
    } else {
        spi_format(obj, 8, 0, 1);  // 8 bits, mode 0, slave
    }
    spi_frequency(obj, 1000000);
    
    // enable the ssp channel
    ssp_enable(obj);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }
}

void spi_free(spi_t *obj) {}

void spi_format(spi_t *obj, int bits, int mode, int slave) {
    ssp_disable(obj);
    if (!(bits >= 4 && bits <= 16) || !(mode >= 0 && mode <= 3)) {
        error("SPI format error");
    }
    
    int polarity = (mode & 0x2) ? 1 : 0;
    int phase = (mode & 0x1) ? 1 : 0;
    
    // set it up
    int DSS = bits - 1;            // DSS (data select size)
    int SPO = (polarity) ? 1 : 0;  // SPO - clock out polarity
    int SPH = (phase) ? 1 : 0;     // SPH - clock out phase
    
    int FRF = 0;                   // FRF (frame format) = SPI
    uint32_t tmp = obj->spi->CR0;
    tmp &= ~(0xFFFF);
    tmp |= DSS << 0
        | FRF << 4
        | SPO << 6
        | SPH << 7;
    obj->spi->CR0 = tmp;
    
    tmp = obj->spi->CR1;
    tmp &= ~(0xD);
    tmp |= 0 << 0                   // LBM - loop back mode - off
        | ((slave) ? 1 : 0) << 2   // MS - master slave mode, 1 = slave
        | 0 << 3;                  // SOD - slave output disable - na
    obj->spi->CR1 = tmp;
    
    ssp_enable(obj);
}

void spi_frequency(spi_t *obj, int hz) {
    ssp_disable(obj);
   
    uint32_t PCLK = SystemCoreClock;
    
        int prescaler;
    
    for (prescaler = 2; prescaler <= 254; prescaler += 2) {
        int prescale_hz = PCLK / prescaler;
        
        // calculate the divider
        int divider = floor(((float)prescale_hz / (float)hz) + 0.5f);
        
        // check we can support the divider
        if (divider < 256) {
            // prescaler
            obj->spi->CPSR = prescaler;
            
            // divider
            obj->spi->CR0 &= ~(0xFFFF << 8);
            obj->spi->CR0 |= (divider - 1) << 8;
            ssp_enable(obj);
            return;
        }
    }
    error("Couldn't setup requested SPI frequency");
}

static inline int ssp_disable(spi_t *obj) {
    return obj->spi->CR1 &= ~(1 << 1);
}

static inline int ssp_enable(spi_t *obj) {
    return obj->spi->CR1 |= SSP_CR1_SSE_Msk;
}

static inline int ssp_readable(spi_t *obj) {
    return obj->spi->SR & (1 << 2);
}

static inline int ssp_writeable(spi_t *obj) {
    return obj->spi->SR & SSP_SR_BSY_Msk;
}

static inline void ssp_write(spi_t *obj, int value) {
    obj->spi->DR = value;
    while (ssp_writeable(obj));
}
static inline int ssp_read(spi_t *obj) {
    int read_DR = obj->spi->DR;
    return read_DR;
}

static inline int ssp_busy(spi_t *obj) {
    return (obj->spi->SR & (1 << 4)) ? (1) : (0);
}

int spi_master_write(spi_t *obj, int value) {
    ssp_write(obj, value);
    while (obj->spi->SR & SSP_SR_BSY_Msk);  /* Wait for send to finish      */
    return (ssp_read(obj));
}

int spi_slave_receive(spi_t *obj) {
    return (ssp_readable(obj) && !ssp_busy(obj)) ? (1) : (0);
}

int spi_slave_read(spi_t *obj) {
    return obj->spi->DR;
}

void spi_slave_write(spi_t *obj, int value) {
    while (ssp_writeable(obj) == 0) ;
    obj->spi->DR = value;
}

int spi_busy(spi_t *obj) {
    return ssp_busy(obj);
}
