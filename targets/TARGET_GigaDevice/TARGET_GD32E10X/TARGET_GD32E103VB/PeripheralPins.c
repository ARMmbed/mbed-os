/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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


/*  void pin_function(PinName pin, int function);
    configure the speed, mode,and remap function of pins
    the parameter function contains the configuration information,show as below
    bit 0:2   gpio mode
    bit 3:8   remap
    bit 9:10  gpio speed
    bit 11:15 adc  /timer channel
*/
const int GD_GPIO_REMAP[] = {
    0x00000000,
    GPIO_SPI0_REMAP,              /* 1 */
    GPIO_I2C0_REMAP,              /* 2 */
    GPIO_USART0_REMAP,            /* 3 */
    GPIO_USART1_REMAP,            /* 4 */
    GPIO_USART2_PARTIAL_REMAP,    /* 5 */
    GPIO_USART2_FULL_REMAP,       /* 6 */
    GPIO_TIMER0_PARTIAL_REMAP,    /* 7 */
    GPIO_TIMER0_FULL_REMAP,       /* 8 */
    GPIO_TIMER1_PARTIAL_REMAP0,   /* 9 */
    GPIO_TIMER1_PARTIAL_REMAP1,   /* 10 */
    GPIO_TIMER1_FULL_REMAP,       /* 11 */
    GPIO_TIMER2_PARTIAL_REMAP,    /* 12 */
    GPIO_TIMER2_FULL_REMAP,       /* 13 */
    GPIO_TIMER3_REMAP,            /* 14 */
    GPIO_PD01_REMAP,              /* 15 */
    GPIO_TIMER4CH3_IREMAP,        /* 16 */
    GPIO_ADC0_ETRGINS_REMAP,      /* 17 */
    GPIO_ADC0_ETRGREG_REMAP,      /* 18 */
    GPIO_ADC1_ETRGINS_REMAP,      /* 19 */
    GPIO_ADC1_ETRGREG_REMAP,      /* 20 */
    GPIO_SWJ_NONJTRST_REMAP,      /* 21 */
    GPIO_SWJ_SWDPENABLE_REMAP,    /* 22 */
    GPIO_SWJ_DISABLE_REMAP,       /* 23 */
    GPIO_CAN0_PARTIAL_REMAP,      /* 24 */
    GPIO_CAN0_FULL_REMAP,         /* 25 */
    GPIO_CAN1_REMAP,              /* 26 */
    GPIO_SPI2_REMAP,              /* 27 */
    GPIO_TIMER1ITR0_REMAP,        /* 28 */
    GPIO_TIMER8_REMAP,            /* 29 */
    GPIO_EXMC_NADV_REMAP,         /* 30 */
    GPIO_CTC_REMAP0,              /* 31 */
};

/* GPIO MODE */
const int GD_GPIO_MODE[] = {
    GPIO_MODE_AIN,                /* 0 */
    GPIO_MODE_IN_FLOATING,        /* 1 */
    GPIO_MODE_IPD,                /* 2 */
    GPIO_MODE_IPU,                /* 3 */
    GPIO_MODE_OUT_OD,             /* 4 */
    GPIO_MODE_OUT_PP,             /* 5 */
    GPIO_MODE_AF_OD,              /* 6 */
    GPIO_MODE_AF_PP,              /* 7 */
};

/* GPIO SPEED */
const int GD_GPIO_SPEED[] = {
    GPIO_OSPEED_50MHZ,            /* 0 */
    GPIO_OSPEED_10MHZ,            /* 1 */
    GPIO_OSPEED_2MHZ,             /* 2 */
};

/* ADC PinMap */
const PinMap PinMap_ADC[] = {
    {PORTA_0, ADC_0, 0 | (0 << 11)},    /* ADC0_IN0 */
    {PORTA_1, ADC_0, 0 | (1 << 11)},    /* ADC0_IN1 */
    {PORTA_2, ADC_0, 0 | (2 << 11)},    /* ADC0_IN2 */
    {PORTA_3, ADC_0, 0 | (3 << 11)},    /* ADC0_IN3 */
    {PORTA_4, ADC_0, 0 | (4 << 11)},    /* ADC0_IN4 */
    {PORTA_5, ADC_0, 0 | (5 << 11)},    /* ADC0_IN5 */
    {PORTA_6, ADC_0, 0 | (6 << 11)},    /* ADC0_IN6 */
    {PORTA_7, ADC_0, 0 | (7 << 11)},    /* ADC0_IN7 */
    {PORTB_0, ADC_0, 0 | (8 << 11)},    /* ADC0_IN8 */
    {PORTB_1, ADC_0, 0 | (9 << 11)},    /* ADC0_IN9 */
    {PORTC_0, ADC_0, 0 | (10 << 11)},   /* ADC0_IN10 */
    {PORTC_1, ADC_0, 0 | (11 << 11)},   /* ADC0_IN11 */
    {PORTC_2, ADC_0, 0 | (12 << 11)},   /* ADC0_IN12 */
    {PORTC_3, ADC_0, 0 | (13 << 11)},   /* ADC0_IN13 */
    {PORTC_4, ADC_0, 0 | (14 << 11)},   /* ADC0_IN14 */
    {PORTC_5, ADC_0, 0 | (15 << 11)},   /* ADC0_IN15 */
    {ADC_TEMP, ADC_0, 0 | (16 << 11)},  /* ADC0_IN16 */
    {ADC_VREF, ADC_0, 0 | (17 << 11)},  /* ADC0_IN17 */

    {PORTA_0_MUL0, ADC_1, 0 | (0 << 11)},   /* ADC1_IN0 */
    {PORTA_1_MUL0, ADC_1, 0 | (1 << 11)},   /* ADC1_IN1 */
    {PORTA_2_MUL0, ADC_1, 0 | (2 << 11)},   /* ADC1_IN2 */
    {PORTA_3_MUL0, ADC_1, 0 | (3 << 11)},   /* ADC1_IN3 */
    {PORTA_4_MUL0, ADC_1, 0 | (4 << 11)},   /* ADC1_IN4 */
    {PORTA_5_MUL0, ADC_1, 0 | (5 << 11)},   /* ADC1_IN5 */
    {PORTA_6_MUL0, ADC_1, 0 | (6 << 11)},   /* ADC1_IN6 */
    {PORTA_7_MUL0, ADC_1, 0 | (7 << 11)},   /* ADC1_IN7 */
    {PORTB_0_MUL0, ADC_1, 0 | (8 << 11)},   /* ADC1_IN8 */
    {PORTB_1_MUL0, ADC_1, 0 | (9 << 11)},   /* ADC1_IN9 */
    {PORTC_0_MUL0, ADC_1, 0 | (10 << 11)},  /* ADC1_IN10 */
    {PORTC_1_MUL0, ADC_1, 0 | (11 << 11)},  /* ADC1_IN11 */
    {PORTC_2_MUL0, ADC_1, 0 | (12 << 11)},  /* ADC1_IN12 */
    {PORTC_3_MUL0, ADC_1, 0 | (13 << 11)},  /* ADC1_IN13 */
    {PORTC_4_MUL0, ADC_1, 0 | (14 << 11)},  /* ADC1_IN14 */
    {PORTC_5_MUL0, ADC_1, 0 | (15 << 11)},  /* ADC1_IN15 */
    {NC,   NC,    0}
};

/* DAC PinMap */
const PinMap PinMap_DAC[] = {
    {PORTA_4,       DAC_0, 0 | (0 << 11)},    /* DAC_OUT0 */
    {PORTA_5,       DAC_0, 0 | (1 << 11)},    /* DAC_OUT1 */
    {NC, NC, 0}
};


/* I2C PinMap */
const PinMap PinMap_I2C_SDA[] = {
    {PORTB_7,  I2C_0, 6},
    {PORTB_9,  I2C_0, 6 | (2 << 3)},    /* GPIO_I2C0_REMAP */
    {PORTB_11, I2C_1, 6},
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PORTB_6,  I2C_0, 6},
    {PORTB_8,  I2C_0, 6 | (2 << 3)},    /* GPIO_I2C0_REMAP */
    {PORTB_10, I2C_1, 6},
    {NC,    NC,    0}
};

/* PWM PinMap */
const PinMap PinMap_PWM[] = {
    {PORTA_8,  PWM_0, 7 | (0 << 11)},   /* TIMER0_CH0 - Default */
    {PORTA_9,  PWM_0, 7 | (1 << 11)},   /* TIMER0_CH1 - Default */
    {PORTA_10, PWM_0, 7 | (2 << 11)},   /* TIMER0_CH2 - Default */
    {PORTA_11, PWM_0, 7 | (3 << 11)},   /* TIMER0_CH3 - Default */
    {PORTE_9,  PWM_0, 7 | (8 << 3) | (0 << 11)},    /* TIMER0_CH0 - GPIO_TIMER0_FULL_REMAP */
    {PORTE_11, PWM_0, 7 | (8 << 3) | (1 << 11)},    /* TIMER0_CH1 - GPIO_TIMER0_FULL_REMAP */
    {PORTE_13, PWM_0, 7 | (8 << 3) | (2 << 11)},    /* TIMER0_CH2 - GPIO_TIMER0_FULL_REMAP */
    {PORTE_14, PWM_0, 7 | (8 << 3) | (3 << 11)},    /* TIMER0_CH3 - GPIO_TIMER0_FULL_REMAP */

    {PORTA_0,  PWM_1, 7 | (0 << 11)},   /* TIMER1_CH0_ETI - Default */
    {PORTA_1,  PWM_1, 7 | (1 << 11)},   /* TIMER1_CH1_ETI - Default */
    {PORTA_2,  PWM_1, 7 | (2 << 11)},   /* TIMER1_CH2_ETI - Default */
    {PORTA_3,  PWM_1, 7 | (3 << 11)},   /* TIMER1_CH3_ETI - Default */
    {PORTA_15, PWM_1, 7 | (9 << 3) | (0 << 11)},    /* TIMER1_CH0_ETI- GPIO_TIMER1_PARTIAL_REMAP0 */
    {PORTB_3,  PWM_1, 7 | (9 << 3) | (1 << 11)},    /* TIMER1_CH1 - GPIO_TIMER1_PARTIAL_REMAP0 */
    {PORTB_10, PWM_1, 7 | (10 << 3) | (2 << 11)},   /* TIMER1_CH2 - GPIO_TIMER1_PARTIAL_REMAP1 */
    {PORTB_11, PWM_1, 7 | (10 << 3) | (3 << 11)},   /* TIMER1_CH3 - GPIO_TIMER1_PARTIAL_REMAP1 */
    {PORTA_15, PWM_1, 7 | (11 << 3) | (0 << 11)},   /* TIMER1_CH0_ETI - GPIO_TIMER1_FULL_REMAP */
    {PORTB_3,  PWM_1, 7 | (11 << 3) | (1 << 11)},   /* TIMER1_CH1 - GPIO_TIMER1_FULL_REMAP */
    {PORTB_10, PWM_1, 7 | (11 << 3) | (2 << 11)},   /* TIMER1_CH2 - GPIO_TIMER1_FULL_REMAP */
    {PORTB_11, PWM_1, 7 | (11 << 3) | (3 << 11)},   /* TIMER1_CH3 - GPIO_TIMER1_FULL_REMAP */

    {PORTA_6,  PWM_2, 7 | (0 << 11)},   /* TIMER2_CH0 - Default */
    {PORTA_7,  PWM_2, 7 | (1 << 11)},   /* TIMER2_CH1 - Default */
    {PORTB_0,  PWM_2, 7 | (2 << 11)},   /* TIMER2_CH2 - Default */
    {PORTB_1,  PWM_2, 7 | (3 << 11)},   /* TIMER2_CH3 - Default */
    {PORTB_4,  PWM_2, 7 | (12 << 3) | (0 << 11)},   /* TIMER2_CH0 - GPIO_TIMER2_PARTIAL_REMAP */
    {PORTB_5,  PWM_2, 7 | (12 << 3) | (1 << 11)},   /* TIMER2_CH1 - GPIO_TIMER2_PARTIAL_REMAP */
    {PORTC_6,  PWM_2, 7 | (13 << 3) | (0 << 11)},   /* TIMER2_CH0 - GPIO_TIMER2_FULL_REMAP */
    {PORTC_7,  PWM_2, 7 | (13 << 3) | (1 << 11)},   /* TIMER2_CH1 - GPIO_TIMER2_FULL_REMAP */
    {PORTC_8,  PWM_2, 7 | (13 << 3) | (2 << 11)},   /* TIMER2_CH2 - GPIO_TIMER2_FULL_REMAP */
    {PORTC_9,  PWM_2, 7 | (13 << 3) | (3 << 11)},   /* TIMER2_CH3 - GPIO_TIMER2_FULL_REMAP */

    {PORTB_6,  PWM_3, 7 | (0 << 11)},   /* TIMER3_CH0 - Default */
    {PORTB_7,  PWM_3, 7 | (1 << 11)},   /* TIMER3_CH1 - Default */
    {PORTB_8,  PWM_3, 7 | (2 << 11)},   /* TIMER3_CH2 - Default */
    {PORTB_9,  PWM_3, 7 | (3 << 11)},   /* TIMER3_CH3 - Default */
    {PORTD_12, PWM_3, 7 | (14 << 3) | (0 << 11)},   /* TIMER3_CH0 - GPIO_TIMER3_REMAP */
    {PORTD_13, PWM_3, 7 | (14 << 3) | (1 << 11)},   /* TIMER3_CH1 - GPIO_TIMER3_REMAP */
    {PORTD_14, PWM_3, 7 | (14 << 3) | (2 << 11)},   /* TIMER3_CH2 - GPIO_TIMER3_REMAP */
    {PORTD_15, PWM_3, 7 | (14 << 3) | (3 << 11)},   /* TIMER3_CH3 - GPIO_TIMER3_REMAP */

    {PORTA_0_MUL0,  PWM_4, 7 | (0 << 11)},  /* TIMER4_CH0 - Default */
    {PORTA_1_MUL0,  PWM_4, 7 | (1 << 11)},  /* TIMER4_CH1 - Default */
    {PORTA_2_MUL0,  PWM_4, 7 | (2 << 11)},  /* TIMER4_CH2 - Default */
    {PORTA_3_MUL0,  PWM_4, 7 | (3 << 11)},  /* TIMER4_CH3 - Default */

    {PORTC_6_MUL0,  PWM_5, 7 | (0 << 11)},  /* TIMER7_CH0 - Default */
    {PORTC_7_MUL0,  PWM_5, 7 | (1 << 11)},  /* TIMER7_CH1 - Default */
    {PORTC_8_MUL0,  PWM_5, 7 | (2 << 11)},  /* TIMER7_CH2 - Default */
    {PORTC_9_MUL0,  PWM_5, 7 | (3 << 11)},  /* TIMER7_CH3 - Default */

    {PORTA_2_MUL1,  PWM_6, 7 | (0 << 11)},  /* TIMER8_CH0 - Default */
    {PORTA_3_MUL1,  PWM_6, 7 | (1 << 11)},  /* TIMER8_CH1 - Default */
    {PORTE_5,  PWM_6, 7 | (29 << 3) | (0 << 11)},   /* TIMER8_CH0 - GPIO_TIMER8_REMAP */
    {PORTE_6,  PWM_6, 7 | (29 << 3) | (1 << 11)},   /* TIMER8_CH1 - GPIO_TIMER8_REMAP */

    {PORTB_8_MUL0,  PWM_7, 7 | (0 << 11)},  /* TIMER9_CH0 - Default */

    {PORTB_9_MUL0,  PWM_8, 7 | (0 << 11)},  /* TIMER10_CH0 - Default */

    {PORTB_14,  PWM_9, 7 | (0 << 11)},  /* TIMER11_CH0 - Default */
    {PORTB_15,  PWM_9, 7 | (1 << 11)},  /* TIMER11_CH1 - Default */

    {PORTA_6_MUL0,  PWM_10, 7 | (0 << 11)}, /* TIMER12_CH0 - Default */

    {PORTA_7_MUL0,  PWM_11, 7 | (0 << 11)}, /* TIMER13_CH0 - Default */

    {NC,    NC,    0}
};

/* USART PinMap */
const PinMap PinMap_UART_TX[] = {
    {PORTA_9,  UART_0, 7},
    {PORTB_6,  UART_0, 7 | (3 << 3)},   /* GPIO_USART0_TX_REMAP */
    {PORTA_2,  UART_1, 7},
    {PORTD_5,  UART_1, 7 | (4 << 3)},   /* GPIO_USART1_TX_REMAP */
    {PORTB_10, UART_2, 7},
    {PORTC_10, UART_2, 7 | (5 << 3)},   /* GPIO_USART2_TX_PARTIAL_REMAP */
    {PORTD_8,  UART_2, 7 | (6 << 3)},   /* GPIO_USART2_TX_FULL_REMAP */
    {PORTC_10_MUL0, UART_3, 7},
    {PORTC_12, UART_4, 7},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PORTA_10, UART_0, 1},
    {PORTB_7,  UART_0, 1 | (3 << 3)},   /* GPIO_USART0_RX_REMAP */
    {PORTA_3,  UART_1, 1},
    {PORTD_6,  UART_1, 1 | (4 << 3)},   /* GPIO_USART1_RX_REMAP */
    {PORTB_11, UART_2, 1},
    {PORTC_11, UART_2, 1 | (5 << 3)},   /* GPIO_USART2_RX_PARTIAL_REMAP */
    {PORTD_9,  UART_2, 1 | (6 << 3)},   /* GPIO_USART2_RX_FULL_REMAP */
    {PORTC_11_MUL0, UART_3, 1},
    {PORTD_2,  UART_4, 1},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
    {PORTA_12, UART_0, 7},
    {PORTA_1,  UART_1, 7},
    {PORTD_4,  UART_1, 7 | (4 << 3)},   /* GPIO_USART1_RTS_REMAP */
    {PORTB_14, UART_2, 7},
    {PORTD_12, UART_2, 7 | (6 << 3)},   /* GPIO_USART2_RTS_FULL_REMAP */
    {NC,    NC,    0}
};

const PinMap PinMap_UART_CTS[] = {
    {PORTA_11, UART_0, 7},
    {PORTA_0,  UART_1, 7},
    {PORTD_3,  UART_1, 7 | (4 << 3)},   /* GPIO_USART1_CTS_REMAP */
    {PORTB_13, UART_2, 7},
    {PORTD_11, UART_2, 7 | (6 << 3)},   /* GPIO_USART2_CTS_FULL_REMAP */
    {NC,    NC,    0}
};

/* SPI PinMap */
const PinMap PinMap_SPI_MOSI[] = {
    {PORTA_7,  SPI_0, 7},
    {PORTB_5,  SPI_0, 7 | (1 << 3)},     /* GPIO_SPI0_REMAP */
    {PORTB_15, SPI_1, 7},
    {PORTC_12, SPI_2, 7 | (27 << 3)},    /* GPIO_SPI2_REMAP */
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PORTA_6,  SPI_0, 7},
    {PORTB_4,  SPI_0, 7 | (1 << 3)},     /* GPIO_SPI0_REMAP */
    {PORTB_14, SPI_1, 7},
    {PORTC_11, SPI_2, 7 | (27 << 3)},    /* GPIO_SPI2_REMAP */
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {PORTA_5,  SPI_0, 7},
    {PORTB_3,  SPI_0, 7 | (1 << 3)},     /* GPIO_SPI0_REMAP */
    {PORTB_13, SPI_1, 7},
    {PORTC_10, SPI_2, 7 | (27 << 3)},    /* GPIO_SPI2_REMAP */
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PORTA_4,  SPI_0, 7},
    {PORTA_15, SPI_0, 7 | (1 << 3)},         /* GPIO_SPI0_REMAP */
    {PORTB_12, SPI_1, 7},
    {PORTA_4_MUL0, SPI_2, 7 | (27 << 3)},    /* GPIO_SPI2_REMAP */
    {NC,    NC,    0}
};

/* CAN PinMap */
const PinMap PinMap_CAN_RD[] = {
    {PORTA_11, CAN_0, 3},
    {PORTB_8, CAN_0, 3 | (24 << 3)},    /* GPIO_CAN0_PARTIAL_REMAP */
    {PORTD_0, CAN_0, 3 | (25 << 3)},    /* GPIO_CAN0_FULL_REMAP */
    {PORTB_12, CAN_1, 3},
    {PORTB_5, CAN_1, 3 | (26 << 3)},    /* GPIO_CAN1_REMAP */
    {NC,    NC,    0}
};

const PinMap PinMap_CAN_TD[] = {
    {PORTA_12, CAN_0, 7},
    {PORTB_9, CAN_0, 7 | (24 << 3)},    /* GPIO_CAN0_PARTIAL_REMAP */
    {PORTD_1, CAN_0, 7 | (25 << 3)},    /* GPIO_CAN0_FULL_REMAP */
    {PORTB_13, CAN_1, 7},
    {PORTB_6, CAN_1, 7 | (26 << 3)},    /* GPIO_CAN1_REMAP */
    {NC,    NC,    0}
};

