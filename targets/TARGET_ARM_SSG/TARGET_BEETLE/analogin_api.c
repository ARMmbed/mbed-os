/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "cmsis.h"
#include "mbed_assert.h"
#include "mbed_wait_api.h"
#include "analogin_api.h"
#include "gpio_api.h"
#include "spi_api.h"
#include "pinmap.h"

#if DEVICE_ANALOGIN

/*
 * Channel Address for the next acquisition:
 * XXAAAXXX XXXXXXXX
 */
#define ADC_SPI_ADDRESS  11

/* ADC Resolution */
#define ADC_RESOLUTION  0xFFF

/* ADC Voltage Divider */
#define ADC_DIV   819.0

/* PinMap structure for ADC IN */
static const PinMap PinMap_ADC[] = {
    {A0, ADC0_0, 0},
    {A1, ADC0_1, 0},
    {A2, ADC0_2, 0},
    {A3, ADC0_3, 0},
    {A4, ADC0_4, 0},
    {A5, ADC0_5, 0},
    {NC, NC, 0}
};

/* ADC SPI Private Data */
typedef struct {
    /* ADC SPI */
    spi_t analogin_spi;
    /* ADC SPI CS */
    gpio_t adc_spi_cs_gpio;
    /* ADC SPI State */
    uint32_t analog_spi_inited;
} analogin_spi_t;
/* ADC SPI Device */
static analogin_spi_t analogin_spi_dev;

/*
 * ADC SPI CS
 */
#define ADC_SPI_CS  P1_2

/*
 * Initialize the analogin peripheral
 * Configures the pin used by analogin.
 * obj: The analogin object to initialize
 * pin: The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin)
{
    /* Initialize ADC Pin */
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    /* Verify if pin is valid */
    MBED_ASSERT(obj->adc != (ADCName)NC);

    /* Initialize the ADC SPI */
    if(analogin_spi_dev.analog_spi_inited == 0){
        /* Initialize SPI for ADC */
        spi_init(&(analogin_spi_dev.analogin_spi), ADC_SPI_MOSI,
                ADC_SPI_MISO, ADC_SPI_SCK, ADC_SPI_nCS);
        spi_format(&(analogin_spi_dev.analogin_spi), 16, 3, 0);
        /* Set SPI to MAX Freq */
        spi_frequency(&(analogin_spi_dev.analogin_spi), 0);

        /* Initialize CS GPIO */
        gpio_init_out(&(analogin_spi_dev.adc_spi_cs_gpio), ADC_SPI_CS);

        analogin_spi_dev.analog_spi_inited = 1;
    }

    /* If pin is valid assign it to the ADC data structure */
    obj->pin = pin;
    obj->pin_number = pin-600;
    obj->address = (0x0000 | (pin-600));

    /* Configure the pinout */
    pinmap_pinout(pin, PinMap_ADC);
}

/*
 * Read the value from analogin pin, represented as an unsigned 16bit value
 * obj: The analogin object
 * @return: An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t result = 0;

    /*
     * The ADC SPI hw is 8 bit format, 16 bit emulation is required
     * in the SPI driver.
     */
    /* CS = 1 */
    gpio_write(&(analogin_spi_dev.adc_spi_cs_gpio), 1);
    /* Do the first read */
    (void)spi_master_write(&(analogin_spi_dev.analogin_spi),
        ((obj->pin_number) << ADC_SPI_ADDRESS));
    /* CS = 0 */
    gpio_write(&(analogin_spi_dev.adc_spi_cs_gpio), 0);
    /* Wait 50 us */
    wait_us(50);
    /* CS = 1 */
    gpio_write(&(analogin_spi_dev.adc_spi_cs_gpio), 1);
    /* The second read provides the result */
    result = spi_master_write(&(analogin_spi_dev.analogin_spi),
        ((obj->pin_number) << ADC_SPI_ADDRESS));
    /* CS = 0 */
    gpio_write(&(analogin_spi_dev.adc_spi_cs_gpio), 0);

    return result;
}

/*
 * Read the input voltage, represented as a float in the range [0.0, 1.0]
 * obj: The analogin object
 * @return: A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj)
{
    uint16_t result = analogin_read_u16(obj);
    return (float)((result & ADC_RESOLUTION) * 1.0f) / ADC_DIV;
}

#endif
