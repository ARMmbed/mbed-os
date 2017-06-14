/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

/*
 * This HAL implementation uses the AD7490 analog-to-digital converter
 * available on the MPS2 Adapter for Arduino shields.
 */

#include "analogin_api.h"
#include "gpio_api.h"
#include "spi_api.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"
#include "pinmap.h"

/*
 * There is only one AD7490 controller to read the analog pins in both shields.
 * The AD7490 documentation (AD7490.pdf, page 12) tells us the right control
 * register to send.
 */

/* Output conversion is straight binary */
#define CODING                     (1 << 0)
/* Analog input range from 0 to REF_IN volts  */
#define RANGE                      (1 << 1)
/* DOUT line state, weakly driven or three-state */
#define WEAK_TRI                   (1 << 2)
/* Access to the shadow register */
#define SHADOW                     (1 << 3)
/* Normal operation power mode */
#define PM0                        (1 << 4)
/* Normal operation power mode */
#define PM1                        (1 << 5)
/* Write control register */
#define WRITE                      (1 << 11)
#define NORMAL_CONTROL_REGISTER    (CODING | RANGE | PM0 | PM1 | WRITE)
/* The ADC will ignore the write of this control register */
#define NO_WRITE_CONTROL_REGISTER  0x000
/* Bit position of the channel number in the control register */
#define CHANNEL_NUMBER_POSITION    6
/* CS signal of the ADC needs to be put low during transfers */
#define CS_LOW                     0
#define CS_HIGH                    1
/* The ADC expects a 16 bits word but only read the 12 most significant bits */
#define USELESS_ADC_BITS           4
/* The ADC result is on the 12 least significant bits */
#define OUTPUT_DATA_MASK           0xFFF
/* The maximum value is the biggest value than can be coded on 12 bits */
#define MAXIMUM_VALUE_12_BITS      OUTPUT_DATA_MASK
#define FRAME_16_BITS              16
#define NO_POLARITY_NO_PHASE       0
#define MASTER_MODE                0
/* Maximal SPI frequency as written in the ADC documentation */
#define MAXIMAL_SPI_FREQUENCY_HZ   12000000

/* The value of the peripheral constant linked with one analog pins is the
 * channel number of that pin on the ADC:
 * A0_0 is channel 0
 * ...
 * A0_5 is channel 5
 * A1_0 is channel 6
 * ...
 * A1_5 is channel 11
 */
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
    {NC  , NC,      0}
};

/* mbed OS gpio_t structure for the CS pin linked to the ADC */
static gpio_t adc_cs;

/* mbed OS spi_t structure to communicate with the ADC */
static spi_t adc_spi;

void analogin_init(analogin_t *obj, PinName pin)
{
    uint16_t control_register = NORMAL_CONTROL_REGISTER;
    uint32_t channel_number = pinmap_peripheral(pin, PinMap_ADC);

    if (channel_number == (uint32_t)NC) {
        error("pin %d is not connected to the ADC", pin);
    }

    /* Add the channel number to the control register */
    control_register |= (channel_number << CHANNEL_NUMBER_POSITION);
    /* Only the 12 first bits are taken into account */
    control_register <<= USELESS_ADC_BITS;
    obj->ctrl_register = control_register;

    spi_init(&adc_spi, ADC_MOSI, ADC_MISO, ADC_SCLK, NC);
    spi_format(&adc_spi, FRAME_16_BITS, NO_POLARITY_NO_PHASE, MASTER_MODE);
    spi_frequency(&adc_spi, MAXIMAL_SPI_FREQUENCY_HZ);

    gpio_init_out(&adc_cs, ADC_SSEL);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t result;

    /* Request conversion */
    gpio_write(&adc_cs, CS_LOW);
    /* Only write the control register, ignore the previous results */
    (void)spi_master_write(&adc_spi, obj->ctrl_register);
    gpio_write(&adc_cs, CS_HIGH);

    /*
     * According to the documentation, t_QUIET (50 ns) time needs to pass before
     * accessing to the SPI bus again. We wait here 1 us as we can not wait a
     * shorter time than that.
     */
    wait_us(1);

    /* Read conversion result */
    gpio_write(&adc_cs, CS_LOW);
    /* Only read the results without writing the control register */
    result = spi_master_write(&adc_spi, NO_WRITE_CONTROL_REGISTER);
    gpio_write(&adc_cs, CS_HIGH);

    return (result & OUTPUT_DATA_MASK);
}

float analogin_read(analogin_t *obj)
{
    uint16_t result = analogin_read_u16(obj);

    return (result * (1. / MAXIMUM_VALUE_12_BITS));
}
