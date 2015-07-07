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

#include "PeripheralPins.h"

#define SERCOM_NULL 0xFF
#define MUX_NULL    0xFF

#define SERCOM_USART_CTRLA_RXPO_Pos 20           /**< \brief (SERCOM_USART_CTRLA) Receive Data Pinout */
#define SERCOM_USART_CTRLA_RXPO_Msk (0x3ul << SERCOM_USART_CTRLA_RXPO_Pos)
#define SERCOM_USART_CTRLA_RXPO(value) ((SERCOM_USART_CTRLA_RXPO_Msk & ((value) << SERCOM_USART_CTRLA_RXPO_Pos)))

#define SERCOM_USART_CTRLA_TXPO_Pos 16           /**< \brief (SERCOM_USART_CTRLA) Transmit Data Pinout */
#define SERCOM_USART_CTRLA_TXPO_Msk (0x3ul << SERCOM_USART_CTRLA_TXPO_Pos)
#define SERCOM_USART_CTRLA_TXPO(value) ((SERCOM_USART_CTRLA_TXPO_Msk & ((value) << SERCOM_USART_CTRLA_TXPO_Pos)))


/************RTC***************/
const PinMap PinMap_RTC[] = {
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
};

const PinMap PinMap_I2C_SCL[] = {
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PA04,  UART_0, 0}
};

const PinMap PinMap_UART_RX[] = {
    {PA05,  UART_0, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
};

const PinMap PinMap_SPI_MOSI[] = {
};

const PinMap PinMap_SPI_MISO[] = {
};

const PinMap PinMap_SPI_SSEL[] = {
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
};

/********SERCOM MAPPING*********/
struct pin_sercom {
    uint8_t pad_num;  // a pin always mapped to a pad
    uint8_t com_num[2]; // a pin always mapped to maximum of 2 sercoms
//	uint8_t pin_mux[2]; // Mux setting for the pin A,B...H ---> 0,1...7
};
struct pin_values {
    uint8_t pin;
    uint8_t pad;
    uint8_t com;
};

struct pin_sercom SAM21[] = {{0, {1, SERCOM_NULL}/*, {3, MUX_NULL}*/},    // PA00
    {1, {1, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PA01
    {0, {0, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PA04
    {1, {0, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PA05
    {2, {0, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PA06
    {3, {0, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PA07
    {0, {0, 2}/*, {2, 3}*/},                  // PA08
    {1, {0, 2}/*, {2, 3}*/},                  // PA09
    {0, {2, SERCOM_NULL}/*, {2, MUX_NULL}*/}, // PA12
    {1, {2, SERCOM_NULL}/*, {2, MUX_NULL}*/}, // PA13
    {2, {2, SERCOM_NULL}/*, {2, MUX_NULL}*/}, // PA14
    {3, {2, SERCOM_NULL}/*, {2, MUX_NULL}*/}, // PA15
    {0, {1, 3}/*, {2, 3}*/},                  // PA16
    {1, {1, 3}/*, {2, 3}*/},                  // PA17
    {2, {1, 3}/*, {2, 3}*/},                  // PA18
    {3, {1, 3}/*, {2, 3}*/},                  // PA19
    {0, {3, 5}/*, {2, 3}*/},                  // PA22
    {1, {3, 5}/*, {2, 3}*/},                  // PA23
    {2, {3, 5}/*, {2, 3}*/},                  // PA24
    {3, {3, 5}/*, {2, 3}*/},                  // PA25
    {0, {3, SERCOM_NULL}/*, {5, MUX_NULL}*/}, // PA27
    {1, {3, SERCOM_NULL}/*, {5, MUX_NULL}*/}, // PA28
    {2, {1, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PA30
    {3, {1, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PA31
    {0, {5, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PB02
    {1, {5, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PB03
    {2, {5, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PB22
    {3, {5, SERCOM_NULL}/*, {3, MUX_NULL}*/}, // PB23
    {2, {4, SERCOM_NULL}/*, {5, MUX_NULL}*/}, // PB30
    {1, {4, SERCOM_NULL}/*, {5, MUX_NULL}*/}, // PB31
    {3, {4, SERCOM_NULL}/*, {5, MUX_NULL}*/}, // PC18
    {0, {4, SERCOM_NULL}/*, {5, MUX_NULL}*/}  // PC19
};
const PinMap PinMap_SERCOM_PINS[] = {
    {PA00},
    {PA01},
    {PA04},
    {PA05},
    {PA06},
    {PA07},
    {PA08},
    {PA09},
    {PA12},
    {PA13},
    {PA14},
    {PA15},
    {PA16},
    {PA17},
    {PA18},
    {PA19},
    {PA22},
    {PA23},
    {PA24},
    {PA25},
    {PA27},
    {PA28},
    {PA30},
    {PA31},

    {PB02},
    {PB03},
    {PB22},
    {PB23},
    {PB30},
    {PB31},

    {PC18},
    {PC19}

};

uint32_t pinmap_find_sercom_index (PinName pin, const PinMap* map)
{
    uint8_t count = 0;
    while (map->pin != NC) {
        if (map->pin == pin)
            return count;
        map++;
        count++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_sercom_peripheral (PinName pin1, PinName pin2)
{
    uint8_t index1 = 0, index2 = 0;

    if ((pin1 == (PinName)NC) || (pin2 == (PinName)NC)) {
        return (uint32_t)NC;
    }

    index1 = pinmap_find_sercom_index(pin1, PinMap_SERCOM_PINS);
    index2 = pinmap_find_sercom_index(pin2, PinMap_SERCOM_PINS);

    if (SAM21[index1].com_num[1] == SERCOM_NULL) {
        return SAM21[index1].com_num[0];
    } else {
        if ((SAM21[index1].com_num[0] == SAM21[index2].com_num[0]) || (SAM21[index1].com_num[0] == SAM21[index2].com_num[1])) {
            return SAM21[index1].com_num[0];
        } else {
            return SAM21[index1].com_num[1];
        }
    }
}

uint32_t pinmap_sercom_pad (PinName pin)
{
    uint8_t index = 0;

    if (pin == (PinName)NC)
        return (uint32_t)NC;

    index = pinmap_find_sercom_index(pin, PinMap_SERCOM_PINS);
    return SAM21[index].pad_num;
}

uint32_t find_sercom_pinmux (struct pin_values* PinValues)
{
    switch (PinValues->com) {
        case 0:  // SERCOM0
            switch (PinValues->pin) {
                case PA04:
                    return PINMUX_PA04D_SERCOM0_PAD0;
                    break;
                case PA08:
                    return PINMUX_PA08C_SERCOM0_PAD0;
                    break;
                case PA05:
                    return PINMUX_PA05D_SERCOM0_PAD1;
                    break;
                case PA09:
                    return PINMUX_PA09C_SERCOM0_PAD1;
                    break;
                case PA06:
                    return PINMUX_PA06D_SERCOM0_PAD2;
                    break;
                case PA10:
                    return PINMUX_PA10C_SERCOM0_PAD2;
                    break;
                case PA07:
                    return PINMUX_PA07D_SERCOM0_PAD3;
                    break;
                case PA11:
                    return PINMUX_PA11C_SERCOM0_PAD3;
                    break;
                default:
                    break;
            }
            break;
        case 1:  // SERCOM1
            switch (PinValues->pin) {
                case PA16:
                    return PINMUX_PA16C_SERCOM1_PAD0;
                    break;
                case PA00:
                    return PINMUX_PA00D_SERCOM1_PAD0;
                    break;
                case PA17:
                    return PINMUX_PA17C_SERCOM1_PAD1;
                    break;
                case PA01:
                    return PINMUX_PA01D_SERCOM1_PAD1;
                    break;
                case PA30:
                    return PINMUX_PA30D_SERCOM1_PAD2;
                    break;
                case PA18:
                    return PINMUX_PA18C_SERCOM1_PAD2;
                    break;
                case PA31:
                    return PINMUX_PA31D_SERCOM1_PAD3;
                    break;
                case PA19:
                    return PINMUX_PA19C_SERCOM1_PAD3;
                    break;
                default:
                    break;
            }
            break;
        case 2:  // SERCOM2
            switch (PinValues->pin) {
                case PA08:
                    return PINMUX_PA08D_SERCOM2_PAD0;
                    break;
                case PA12:
                    return PINMUX_PA12C_SERCOM2_PAD0;
                    break;
                case PA09:
                    return PINMUX_PA09D_SERCOM2_PAD1;
                    break;
                case PA13:
                    return PINMUX_PA13C_SERCOM2_PAD1;
                    break;
                case PA10:
                    return PINMUX_PA10D_SERCOM2_PAD2;
                    break;
                case PA14:
                    return PINMUX_PA14C_SERCOM2_PAD2;
                    break;
                case PA11:
                    return PINMUX_PA11D_SERCOM2_PAD3;
                    break;
                case PA15:
                    return PINMUX_PA15C_SERCOM2_PAD3;
                    break;
                default:
                    break;
            }
            break;
        case 3:  // SERCOM3
            switch (PinValues->pin) {
                case PA16:
                    return PINMUX_PA16D_SERCOM3_PAD0;
                    break;
                case PA22:
                    return PINMUX_PA22C_SERCOM3_PAD0;
                    break;
                case PA27:
                    return PINMUX_PA27F_SERCOM3_PAD0;
                    break;
                case PA17:
                    return PINMUX_PA17D_SERCOM3_PAD1;
                    break;
                case PA23:
                    return PINMUX_PA23C_SERCOM3_PAD1;
                    break;
                case PA28:
                    return PINMUX_PA28F_SERCOM3_PAD1;
                    break;
                case PA18:
                    return PINMUX_PA18D_SERCOM3_PAD2;
                    break;
                case PA20:
                    return PINMUX_PA20D_SERCOM3_PAD2;
                    break;
                case PA24:
                    return PINMUX_PA24C_SERCOM3_PAD2;
                    break;
                case PA19:
                    return PINMUX_PA19D_SERCOM3_PAD3;
                    break;
                case PA25:
                    return PINMUX_PA25C_SERCOM3_PAD3;
                    break;
                default:
                    break;
            }
            break;
        case 4:  // SERCOM4
            switch (PinValues->pin) {
                case PA12:
                    return PINMUX_PA12D_SERCOM4_PAD0;
                    break;
                case PB08:
                    return PINMUX_PB08D_SERCOM4_PAD0;
                    break;
                case PC19:
                    return PINMUX_PC19F_SERCOM4_PAD0;
                    break;
                case PA13:
                    return PINMUX_PA13D_SERCOM4_PAD1;
                    break;
                case PB09:
                    return PINMUX_PB09D_SERCOM4_PAD1;
                    break;
                case PB31:
                    return PINMUX_PB31F_SERCOM4_PAD1;
                    break;
                case PA14:
                    return PINMUX_PA14D_SERCOM4_PAD2;
                    break;
                case PB14:
                    return PINMUX_PB14C_SERCOM4_PAD2;
                    break;
                case PB30:
                    return PINMUX_PB30F_SERCOM4_PAD2;
                    break;
                case PA15:
                    return PINMUX_PA15D_SERCOM4_PAD3;
                    break;
                case PB15:
                    return PINMUX_PB15C_SERCOM4_PAD3;
                    break;
                case PC18:
                    return PINMUX_PC18F_SERCOM4_PAD3;
                    break;
                default:
                    break;
            }
            break;
        case 5:  // SERCOM5
            switch (PinValues->pin) {
                case PB16:
                    return PINMUX_PB16C_SERCOM5_PAD0;
                    break;
                case PA22:
                    return PINMUX_PA22D_SERCOM5_PAD0;
                    break;
                case PB02:
                    return PINMUX_PB02D_SERCOM5_PAD0;
                    break;
                case PB30:
                    return PINMUX_PB30D_SERCOM5_PAD0;
                    break;
                case PB17:
                    return PINMUX_PB17C_SERCOM5_PAD1;
                    break;
                case PA23:
                    return PINMUX_PA23D_SERCOM5_PAD1;
                    break;
                case PB03:
                    return PINMUX_PB03D_SERCOM5_PAD1;
                    break;
                case PB31:
                    return PINMUX_PB31D_SERCOM5_PAD1;
                    break;
                case PA24:
                    return PINMUX_PA24D_SERCOM5_PAD2;
                    break;
                case PB00:
                    return PINMUX_PB00D_SERCOM5_PAD2;
                    break;
                case PB22:
                    return PINMUX_PB22D_SERCOM5_PAD2;
                    break;
                case PA20:
                    return PINMUX_PA20C_SERCOM5_PAD2;
                    break;
                case PA25:
                    return PINMUX_PA25D_SERCOM5_PAD3;
                    break;
                case PB23:
                    return PINMUX_PB23D_SERCOM5_PAD3;
                    break;
                default:
                    break;
            }
            break;
    }
}
uint32_t find_mux_setting (PinName output, PinName input, PinName clock, PinName chipsel)
{
    struct pin_values input_values, output_values, clock_values, chipsel_values;
    uint32_t mux_setting = 0;

    input_values.pin = input;
    output_values.pin = output;
    clock_values.pin = clock;
    chipsel_values.pin = chipsel;

    input_values.com = pinmap_sercom_peripheral(input, output);
    output_values.com = input_values.com;
    clock_values.com = input_values.com;
    chipsel_values.com = input_values.com;

    input_values.pad = pinmap_sercom_pad(input);
    output_values.pad = pinmap_sercom_pad(output);
    clock_values.pad = pinmap_sercom_pad(clock);
    chipsel_values.pad = pinmap_sercom_pad(chipsel);

    switch(input_values.pad) {      //TODO: Condition for hardware flow control enabled is different.
        case 0:
            mux_setting |= SERCOM_USART_CTRLA_RXPO(0);
            break;
        case 1:
            mux_setting |= SERCOM_USART_CTRLA_RXPO(1);
            break;
        case 2:
            mux_setting |= SERCOM_USART_CTRLA_RXPO(2);
            break;
        case 3:
            mux_setting |= SERCOM_USART_CTRLA_RXPO(3);
            break;
    }

    if ((clock == NC) && (chipsel == NC)) { // condition for no hardware control and uart
        if ((output_values.pad == 0)) {  // condition for hardware enable and usart is different
            mux_setting |= SERCOM_USART_CTRLA_TXPO(0);
        } else if((output_values.pad == 2)) {
            mux_setting |= SERCOM_USART_CTRLA_TXPO(1);
        } else {
            mux_setting = mux_setting;  // dummy condition
        }
    } else { // for hardware flow control and uart // expecting the tx in pad 0, rts in pad2 and cts in pad 3
        if((output_values.pad == 0) && (clock_values.pad/*rts pin*/ == 2) && (chipsel_values.pad/*cts pin*/ == 3)) {
            mux_setting |= SERCOM_USART_CTRLA_TXPO(2);
        }
    }

    return mux_setting;
}

void find_pin_settings (PinName output, PinName input, PinName clock, PinName chipsel, uint32_t* pad_pinmuxes)
{
    struct pin_values input_values, output_values, clock_values, chipsel_values;
    uint8_t i = 0;

    for (i = 0; i < 4 ; i++ ) { // load default values for the pins
        pad_pinmuxes[i] = 0xFFFFFFFF; //PINMUX_UNUSED
    }

    input_values.pin = input;
    output_values.pin = output;
    clock_values.pin = clock;
    chipsel_values.pin = chipsel;

    input_values.com = pinmap_sercom_peripheral(input, output);
    output_values.com = input_values.com;
    clock_values.com = input_values.com;
    chipsel_values.com = input_values.com;

    input_values.pad = pinmap_sercom_pad(input);
    output_values.pad = pinmap_sercom_pad(output);
    clock_values.pad = pinmap_sercom_pad(clock);
    chipsel_values.pad = pinmap_sercom_pad(chipsel);

    if (input_values.pad < 0x04)
        pad_pinmuxes[input_values.pad] = find_sercom_pinmux(&input_values);
    if (output_values.pad < 0x04)
        pad_pinmuxes[output_values.pad] = find_sercom_pinmux(&output_values);
    if (clock_values.pad < 0x04)
        pad_pinmuxes[clock_values.pad] = find_sercom_pinmux(&clock_values);
    if (chipsel_values.pad < 0x04)
        pad_pinmuxes[chipsel_values.pad] = find_sercom_pinmux(&chipsel_values);

}





