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
#include "mbed_assert.h"
#include "analogin_api.h"

#include "cmsis.h"
#include "pinmap.h"

#define ADC_12BIT_RANGE             0xFFF

static const PinMap PinMap_ADC[] = {
    {A0_0, ADC0_0,  0},
    {A0_1, ADC0_1,  0},
    {A0_2, ADC0_2,  0},
    {A0_3, ADC0_3,  0},
    {A0_4, ADC0_4,  0},
    {A0_5, ADC0_5,  0},
    {A1_0, ADC0_6,  0},
    {A1_1, ADC0_7,  0},
    {A1_2, ADC0_8,  0},
    {A1_3, ADC0_9,  0},
    {A1_4, ADC0_10, 0},
    {A1_5, ADC0_11, 0},
    {NC,    NC,     0}
};

static const PinMap PinMap_SPI_SCLK[] = {
    {ADC_SCLK , SPI_3, 0},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MOSI[] = {
    {ADC_MOSI, SPI_3, 0},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {ADC_MISO, SPI_3, 0},
    {NC   , NC   , 0}
};

static const PinMap PinMap_SPI_SSEL[] = {
    {ADC_SSEL, SPI_3, 0},
    {NC   , NC   , 0}
};

#define ADC_RANGE    ADC_12BIT_RANGE
int analog_spi_inited = 0;

void analogin_init(analogin_t *obj, PinName pin) {
    
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
  MBED_ASSERT(obj->adc != (ADCName)NC);
  obj->pin = pin;
    obj->pin_number = pin-600;
    obj->address = (0x0000 | (pin-600));
    
    SPIName adc_mosi = (SPIName)pinmap_peripheral(ADC_MOSI, PinMap_SPI_MOSI);
  SPIName adc_miso = (SPIName)pinmap_peripheral(ADC_MISO, PinMap_SPI_MISO);
  SPIName adc_sclk = (SPIName)pinmap_peripheral(ADC_SCLK, PinMap_SPI_SCLK);
  SPIName adc_ssel = (SPIName)pinmap_peripheral(ADC_SSEL, PinMap_SPI_SSEL);
  SPIName adc_data = (SPIName)pinmap_merge(adc_mosi, adc_miso);
  SPIName adc_cntl = (SPIName)pinmap_merge(adc_sclk, adc_ssel);
  obj->adc_spi = (MPS2_SSP_TypeDef*)pinmap_merge(adc_data, adc_cntl);
  
    if(analog_spi_inited == 0){
    obj->adc_spi->CR1       = 0;
    obj->adc_spi->CR0       = SSP_CR0_SCR_DFLT | SSP_CR0_FRF_MOT | SSP_CR0_DSS_16;
    obj->adc_spi->CPSR      = SSP_CPSR_DFLT; 
    obj->adc_spi->IMSC      = 0x8; 
    obj->adc_spi->DMACR     = 0;
    obj->adc_spi->CR1       = SSP_CR1_SSE_Msk;
    obj->adc_spi->ICR       = 0x3;  
    analog_spi_inited = 1;
    }
    
  pinmap_pinout(ADC_MOSI, PinMap_SPI_MOSI);
  pinmap_pinout(ADC_MISO, PinMap_SPI_MISO);
  pinmap_pinout(ADC_SCLK, PinMap_SPI_SCLK);
  pinmap_pinout(ADC_SSEL, PinMap_SPI_SSEL);
  pinmap_pinout(pin, PinMap_ADC);
}

static inline uint32_t adc_read(analogin_t *obj) {
    return 0;
}


float analogin_read(analogin_t *obj) {
    uint32_t value = adc_read(obj);
    return 0;
}

uint16_t analogin_read_u16(analogin_t *obj) {
    return 0;
}

