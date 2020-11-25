/**
 * \file
 *
 * \brief PIC32CXMTG_EK Board Definition.
 *
 * Copyright (c) 2020 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef PIC32CXMTG_EK_H_INCLUDED
#define PIC32CXMTG_EK_H_INCLUDED

#include "pic32cx.h"

/**
 * \ingroup group_common_boards
 * \defgroup pic32cxmtg_ek_group "PIC32CXMTG_EK"
 * Develop Board for metrology devices.
 *
 * @{
 */

/**
 * \defgroup PIC32CXMTG_EK_board_info_group "PIC32CXMTG_EK - Board informations"
 * Definitions related to the board description.
 *
 * @{
 */

/** Name of the board */
#define BOARD_NAME "PIC32CXMTG_EK"
/** Board definition */
#define pic32cxmtg_ek /** Family definition (already defined) */
#define pic32cx
/** Core definition */
#define cortexm4
/** Board revision definition */
#define BOARD_REV_1     1

#ifndef BOARD_REV
#define BOARD_REV BOARD_REV_1
#endif

/* @} */

/**
 *  \defgroup PIC32CXMTG_EK_opfreq_group "PIC32CXMTG_EK - Operating frequencies"
 *  Definitions related to the board operating frequency.
 *
 *  @{
 */

/**
 * \name Board oscillator settings
 * @{
 */
#define BOARD_FREQ_SLCK_XTAL        (32768U)
#define BOARD_FREQ_SLCK_BYPASS      (32768U)
#define BOARD_FREQ_MAINCK_XTAL      (16384000U)
#define BOARD_FREQ_MAINCK_BYPASS    (16384000U)
/* @} */

/** Master clock frequency */
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX

/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US        15625U

/* @} */

/**
 * \defgroup PIC32CXMTG_EK_features_group "PIC32CXMTG_EK - Features"
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/**
 * \name GREEN LED #0 pin definition
 * @{
 */
#define LED0_GPIO                        (PIO_PD16_IDX)
#define LED0_ACTIVE_LEVEL                IOPORT_PIN_LEVEL_LOW
#define LED0_INACTIVE_LEVEL              IOPORT_PIN_LEVEL_HIGH

/* Wrapper macros to ensure common naming across all boards */
#define LED_0_NAME                       "User LED GREEN"
#define PIN_LED_0                        {PIO_PD16, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_0_MASK                   PIO_PD16
#define PIN_LED_0_PIO                    PIOD
#define PIN_LED_0_ID                     ID_PIOD
#define PIN_LED_0_TYPE                   PIO_OUTPUT_1
#define PIN_LED_0_ATTR                   PIO_DEFAULT
/* @} */

/**
 * \name BLUE LED #1 pin definition
 * @{
 */
#define LED1_GPIO                        (PIO_PD19_IDX)
#define LED1_ACTIVE_LEVEL                IOPORT_PIN_LEVEL_LOW
#define LED1_INACTIVE_LEVEL              IOPORT_PIN_LEVEL_HIGH

/* Wrapper macros to ensure common naming across all boards */
#define LED_1_NAME                       "User LED BLUE"
#define PIN_LED_1                        {PIO_PD19, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK                   PIO_PD19
#define PIN_LED_1_PIO                    PIOD
#define PIN_LED_1_ID                     ID_PIOD
#define PIN_LED_1_TYPE                   PIO_OUTPUT_1
#define PIN_LED_1_ATTR                   PIO_DEFAULT
/* @} */

/**
 * \name Push button #1 definition
 * Attributes = pull-up + debounce + interrupt on rising edge.
 * @{
 */
#define PUSHBUTTON_1_NAME        "USER BUTTON"
#define GPIO_PUSH_BUTTON_1       (PIO_PA7_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define GPIO_PUSH_BUTTON_1_SENSE (IOPORT_SENSE_FALLING)

#define PIN_PUSHBUTTON_1       {PIO_PA7, PIOA, ID_PIOA, PIO_INPUT, \
				PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALLING}
#define PIN_PUSHBUTTON_1_MASK  PIO_PA7
#define PIN_PUSHBUTTON_1_PIO   PIOA
#define PIN_PUSHBUTTON_1_ID    ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE  PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR  (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALLING)
#define PIN_PUSHBUTTON_1_IRQn  PIOA_IRQn
/* @} */

/** List of all push button definitions. */
#define PINS_PUSHBUTTONS    {PIN_PUSHBUTTON_1}

/**
 * \name Console Port USART pins definitions
 * @{
 */
#define PINS_CONSOLE_UART                  (PIO_PA4A_FLEXCOM0_IO0 | PIO_PA5A_FLEXCOM0_IO1)
#define PINS_CONSOLE_UART_FLAGS            (IOPORT_MODE_MUX_A)

#define PINS_CONSOLE_UART_PORT             IOPORT_PIOA
#define PINS_CONSOLE_UART_MASK             (PIO_PA4A_FLEXCOM0_IO0 | PIO_PA5A_FLEXCOM0_IO1)
#define PINS_CONSOLE_UART_PIO              PIOA
#define PINS_CONSOLE_UART_ID               ID_PIOA
#define PINS_CONSOLE_UART_TYPE             IOPORT_MODE_MUX_A
#define PINS_CONSOLE_UART_ATTR             IOPORT_MODE_DEFAULT
/* @} */

/**
 * \PWM0 pin definitions.
 * @{
 */
#define PIN_PWM_OUT0_GPIO                PIO_PD17_IDX
#define PIN_PWM_OUT0_FLAGS               (IOPORT_MODE_MUX_A)
#define PIN_PWM_OUT0_CHANNEL             PWM_CHANNEL_0
/* @} */

/**
 * \PWM1 pin definitions.
 * @{
 */
#define PIN_PWM_OUT1_GPIO                PIO_PD18_IDX
#define PIN_PWM_OUT1_FLAGS               (IOPORT_MODE_MUX_A)
#define PIN_PWM_OUT1_CHANNEL             PWM_CHANNEL_1
/* @} */

/**
 * \PWM2 pin definitions.
 * @{
 */
#define PIN_PWM_OUT2_GPIO                PIO_PD19_IDX
#define PIN_PWM_OUT2_FLAGS               (IOPORT_MODE_MUX_A)
#define PIN_PWM_OUT2_CHANNEL             PWM_CHANNEL_2
/* @} */

/**
 * \name TC pins definition
 * @{
 */
#define PIN_TC0_TIOA0_GPIO               (PIO_PA25_IDX)
#define PIN_TC0_TIOA0_MUX                (IOPORT_MODE_MUX_D)
#define PIN_TC0_TIOA0_FLAGS              (IOPORT_MODE_MUX_D)

#define PIN_TC0_TIOA1_GPIO               (PIO_PA31_IDX)
#define PIN_TC0_TIOA1_MUX                (IOPORT_MODE_MUX_D)
#define PIN_TC0_TIOA1_FLAGS              (IOPORT_MODE_MUX_D)

#define PIN_TC0_TIOA2_GPIO               (PIO_PB2_IDX)
#define PIN_TC0_TIOA2_MUX                (IOPORT_MODE_MUX_D)
#define PIN_TC0_TIOA2_FLAGS              (IOPORT_MODE_MUX_D)
/* @} */

/**
 * \name QSPI pins definitions
 * @{
 */
#define QSPI_QSCK_GPIO                  PIO_PC15_IDX
#define QSPI_QSCK_FLAGS                 (IOPORT_MODE_MUX_A)
#define QSPI_QCS_GPIO                   PIO_PC14_IDX
#define QSPI_QCS_FLAGS                  (IOPORT_MODE_MUX_A)
#define QSPI_QIO0_GPIO                  PIO_PC13_IDX
#define QSPI_QIO0_FLAGS                 (IOPORT_MODE_MUX_A)
#define QSPI_QIO1_GPIO                  PIO_PC12_IDX
#define QSPI_QIO1_FLAGS                 (IOPORT_MODE_MUX_A)
#define QSPI_QIO2_GPIO                  PIO_PC11_IDX
#define QSPI_QIO2_FLAGS                 (IOPORT_MODE_MUX_A)
#define QSPI_QIO3_GPIO                  PIO_PC10_IDX
#define QSPI_QIO3_FLAGS                 (IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP)
/* @} */

/* ! \name Xplain PRO Extension header pin definitions */
/* @{ */
#define XPLAIN_PRO_PIN_3                PIO_PA14_IDX
#define XPLAIN_PRO_PIN_4                PIO_PA15_IDX
#define XPLAIN_PRO_PIN_5                PIO_PA28_IDX
#define XPLAIN_PRO_PIN_6                PIO_PA31_IDX
#define XPLAIN_PRO_PIN_7                PIO_PB1_IDX
#define XPLAIN_PRO_PIN_8                PIO_PA25_IDX
#define XPLAIN_PRO_PIN_9                PIO_PA26_IDX
#define XPLAIN_PRO_PIN_10               PIO_PA27_IDX
#define XPLAIN_PRO_PIN_11               PIO_PA0_IDX
#define XPLAIN_PRO_PIN_12               PIO_PA1_IDX
#define XPLAIN_PRO_PIN_13               PIO_PD15_IDX
#define XPLAIN_PRO_PIN_14               PIO_PA6_IDX
#define XPLAIN_PRO_PIN_15               PIO_PA21_IDX
#define XPLAIN_PRO_PIN_16               PIO_PA24_IDX
#define XPLAIN_PRO_PIN_17               PIO_PA23_IDX
#define XPLAIN_PRO_PIN_18               PIO_PA22_IDX
/* @} */

/* ! \name Xplain PRO Extension header pin definitions by function */
/* @{ */
#define XPLAIN_PRO_PIN_GPIO_1           XPLAIN_PRO_PIN_3
#define XPLAIN_PRO_PIN_GPIO_2           XPLAIN_PRO_PIN_4
#define XPLAIN_PRO_PIN_GPIO_3           XPLAIN_PRO_PIN_5
#define XPLAIN_PRO_PIN_GPIO_4           XPLAIN_PRO_PIN_6
#define XPLAIN_PRO_PIN_GPIO_5           XPLAIN_PRO_PIN_7
#define XPLAIN_PRO_PIN_GPIO_6           XPLAIN_PRO_PIN_8
#define XPLAIN_PRO_PIN_GPIO_7           XPLAIN_PRO_PIN_9
#define XPLAIN_PRO_PIN_GPIO_8           XPLAIN_PRO_PIN_10
#define XPLAIN_PRO_PIN_TWI_SDA          XPLAIN_PRO_PIN_11
#define XPLAIN_PRO_PIN_TWI_SCL          XPLAIN_PRO_PIN_12
#define XPLAIN_PRO_PIN_USART_RX         XPLAIN_PRO_PIN_11
#define XPLAIN_PRO_PIN_USART_TX         XPLAIN_PRO_PIN_12
#define XPLAIN_PRO_PIN_SPI_SS_0         XPLAIN_PRO_PIN_15
#define XPLAIN_PRO_PIN_SPI_MOSI         XPLAIN_PRO_PIN_16
#define XPLAIN_PRO_PIN_SPI_MISO         XPLAIN_PRO_PIN_17
#define XPLAIN_PRO_PIN_SPI_SCK          XPLAIN_PRO_PIN_18
/* @} */

/**
 * \name XPLAIN Port USART pins definitions
 * @{
 */
#define PINS_XPLAIN_USART                 (PIO_PA0A_FLEXCOM6_IO0 | PIO_PA1A_FLEXCOM6_IO1)
#define PINS_XPLAIN_USART_FLAGS           (IOPORT_MODE_MUX_A)

#define PINS_XPLAIN_USART_PORT            IOPORT_PIOA
#define PINS_XPLAIN_USART_MASK            (PIO_PA0A_FLEXCOM6_IO0 | PIO_PA1A_FLEXCOM6_IO1)
#define PINS_XPLAIN_USART_PIO             PIOA
#define PINS_XPLAIN_USART_ID              ID_PIOA
#define PINS_XPLAIN_USART_TYPE            IOPORT_MODE_MUX_A
#define PINS_XPLAIN_USART_ATTR            IOPORT_MODE_DEFAULT
/* @} */

/**
 * \name XPLAIN Port TWI pins definitions
 * @{
 */
#define PINS_XPLAIN_TWI                   (PIO_PA0A_FLEXCOM6_IO0 | PIO_PA1A_FLEXCOM6_IO1)
#define PINS_XPLAIN_TWI_FLAGS             (IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP)

#define PINS_XPLAIN_TWI_PORT              IOPORT_PIOA
#define PINS_XPLAIN_TWI_MASK              (PIO_PA0A_FLEXCOM6_IO0 | PIO_PA1A_FLEXCOM6_IO1)
#define PINS_XPLAIN_TWI_PIO               PIOA
#define PINS_XPLAIN_TWI_ID                ID_PIOA
#define PINS_XPLAIN_TWI_TYPE              IOPORT_MODE_MUX_A
#define PINS_XPLAIN_TWI_ATTR              IOPORT_MODE_PULLUP
/* @} */

/**
 * \name XPLAIN Port SPI pins definitions
 * @{
 */
#define XPLAIN_SPI_MOSI_GPIO              (XPLAIN_PRO_PIN_SPI_MOSI)
#define XPLAIN_SPI_MOSI_FLAGS             (IOPORT_MODE_MUX_B)
#define XPLAIN_SPI_MISO_GPIO              (XPLAIN_PRO_PIN_SPI_MISO)
#define XPLAIN_SPI_MISO_FLAGS             (IOPORT_MODE_MUX_B)
#define XPLAIN_SPI_SPCK_GPIO              (XPLAIN_PRO_PIN_SPI_SCK)
#define XPLAIN_SPI_SPCK_FLAGS             (IOPORT_MODE_MUX_B)
#define XPLAIN_SPI_NPCS0_GPIO             (XPLAIN_PRO_PIN_SPI_SS_0)
#define XPLAIN_SPI_NPCS0_FLAGS            (IOPORT_MODE_MUX_B)
#define XPLAIN_SPI_NPCS1_GPIO             (XPLAIN_PRO_PIN_GPIO_8)
#define XPLAIN_SPI_NPCS1_FLAGS            (IOPORT_MODE_MUX_A)
/* @} */

/* ! \name mikroBUS1 Extension header pin definitions */
/* @{ */
#define MIKROBUS1_PIN_1                  PIO_PA29_IDX
#define MIKROBUS1_PIN_2                  PIO_PA30_IDX
#define MIKROBUS1_PIN_3                  PIO_PB12_IDX
#define MIKROBUS1_PIN_4                  PIO_PB11_IDX
#define MIKROBUS1_PIN_5                  PIO_PB10_IDX
#define MIKROBUS1_PIN_6                  PIO_PB9_IDX
#define MIKROBUS1_PIN_11                 PIO_PA8_IDX
#define MIKROBUS1_PIN_12                 PIO_PA9_IDX
#define MIKROBUS1_PIN_13                 PIO_PD2_IDX
#define MIKROBUS1_PIN_14                 PIO_PD1_IDX
#define MIKROBUS1_PIN_15                 PIO_PC7_IDX
#define MIKROBUS1_PIN_16                 PIO_PD17_IDX
/* @} */

/* ! \name mikroBUS1 Extension header pin definitions by function */
/* @{ */
#define MIKROBUS1_PIN_AN                 MIKROBUS1_PIN_1
#define MIKROBUS1_PIN_RST                MIKROBUS1_PIN_2
#define MIKROBUS1_PIN_SPI_CS             MIKROBUS1_PIN_3
#define MIKROBUS1_PIN_SPI_SCK            MIKROBUS1_PIN_4
#define MIKROBUS1_PIN_SPI_MISO           MIKROBUS1_PIN_5
#define MIKROBUS1_PIN_SPI_MOSI           MIKROBUS1_PIN_6
#define MIKROBUS1_PIN_PWM                MIKROBUS1_PIN_16
#define MIKROBUS1_PIN_INT                MIKROBUS1_PIN_15
#define MIKROBUS1_PIN_UART_RX            MIKROBUS1_PIN_14
#define MIKROBUS1_PIN_UART_TX            MIKROBUS1_PIN_13
#define MIKROBUS1_PIN_TWI_SCL            MIKROBUS1_PIN_12
#define MIKROBUS1_PIN_TWI_SDA            MIKROBUS1_PIN_11
/* @} */

/**
 * \name MIKROBUS1 Port USART pins definitions
 * @{
 */
#define PINS_MIKROBUS1_USART                 (PIO_PD1A_URXD | PIO_PD2A_UTXD)
#define PINS_MIKROBUS1_USART_FLAGS           (IOPORT_MODE_MUX_A)

#define PINS_MIKROBUS1_USART_PORT            IOPORT_PIOD
#define PINS_MIKROBUS1_USART_MASK            (PIO_PD1A_URXD | PIO_PD2A_UTXD)
#define PINS_MIKROBUS1_USART_PIO             PIOD
#define PINS_MIKROBUS1_USART_ID              ID_PIOD
#define PINS_MIKROBUS1_USART_TYPE            IOPORT_MODE_MUX_D
#define PINS_MIKROBUS1_USART_ATTR            IOPORT_MODE_DEFAULT
/* @} */

/**
 * \name MIKROBUS1 Port TWI pins definitions
 * @{
 */
#define PINS_MIKROBUS1_TWI                   (PIO_PA8A_FLEXCOM1_IO0 | PIO_PA9A_FLEXCOM1_IO1)
#define PINS_MIKROBUS1_TWI_FLAGS             (IOPORT_MODE_MUX_A)

#define PINS_MIKROBUS1_TWI_PORT              IOPORT_PIOA
#define PINS_MIKROBUS1_TWI_MASK              (PIO_PA8A_FLEXCOM1_IO0 | PIO_PA9A_FLEXCOM1_IO1)
#define PINS_MIKROBUS1_TWI_PIO               PIOA
#define PINS_MIKROBUS1_TWI_ID                ID_PIOA
#define PINS_MIKROBUS1_TWI_TYPE              IOPORT_MODE_MUX_A
#define PINS_MIKROBUS1_TWI_ATTR              IOPORT_MODE_DEFAULT
/* @} */

/**
 * \name MIKROBUS1 Port SPI pins definitions
 * @{
 */
#define MIKROBUS1_SPI_MOSI_GPIO              (MIKROBUS1_PIN_SPI_MOSI)
#define MIKROBUS1_SPI_MOSI_FLAGS             (IOPORT_MODE_MUX_B)
#define MIKROBUS1_SPI_MISO_GPIO              (MIKROBUS1_PIN_SPI_MISO)
#define MIKROBUS1_SPI_MISO_FLAGS             (IOPORT_MODE_MUX_B)
#define MIKROBUS1_SPI_SPCK_GPIO              (MIKROBUS1_PIN_SPI_SCK)
#define MIKROBUS1_SPI_SPCK_FLAGS             (IOPORT_MODE_MUX_B)
#define MIKROBUS1_SPI_NPCS0_GPIO             (MIKROBUS1_PIN_SPI_CS)
#define MIKROBUS1_SPI_NPCS0_FLAGS            (IOPORT_MODE_MUX_B)
/* @} */


/* ! \name mikroBUS2 Extension header pin definitions */
/* @{ */
#define MIKROBUS2_PIN_1                  PIO_PB0_IDX
#define MIKROBUS2_PIN_2                  PIO_PB5_IDX
#define MIKROBUS2_PIN_3                  PIO_PC0_IDX
#define MIKROBUS2_PIN_4                  PIO_PB26_IDX
#define MIKROBUS2_PIN_5                  PIO_PB25_IDX
#define MIKROBUS2_PIN_6                  PIO_PB24_IDX
#define MIKROBUS2_PIN_11                 PIO_PA12_IDX
#define MIKROBUS2_PIN_12                 PIO_PA13_IDX
#define MIKROBUS2_PIN_13                 PIO_PB4_IDX
#define MIKROBUS2_PIN_14                 PIO_PB3_IDX
#define MIKROBUS2_PIN_15                 PIO_PB6_IDX
#define MIKROBUS2_PIN_16                 PIO_PD18_IDX
/* @} */

/* ! \name mikroBUS2 Extension header pin definitions by function */
/* @{ */
#define MIKROBUS2_PIN_AN                 MIKROBUS2_PIN_1
#define MIKROBUS2_PIN_RST                MIKROBUS2_PIN_2
#define MIKROBUS2_PIN_SPI_CS             MIKROBUS2_PIN_3
#define MIKROBUS2_PIN_SPI_SCK            MIKROBUS2_PIN_4
#define MIKROBUS2_PIN_SPI_MISO           MIKROBUS2_PIN_5
#define MIKROBUS2_PIN_SPI_MOSI           MIKROBUS2_PIN_6
#define MIKROBUS2_PIN_PWM                MIKROBUS2_PIN_16
#define MIKROBUS2_PIN_INT                MIKROBUS2_PIN_15
#define MIKROBUS2_PIN_UART_RX            MIKROBUS2_PIN_14
#define MIKROBUS2_PIN_UART_TX            MIKROBUS2_PIN_13
#define MIKROBUS2_PIN_TWI_SCL            MIKROBUS2_PIN_12
#define MIKROBUS2_PIN_TWI_SDA            MIKROBUS2_PIN_11
/* @} */

/**
 * \name MIKROBUS2 Port USART pins definitions
 * @{
 */
#define PINS_MIKROBUS2_USART                 (PIO_PB3B_FLEXCOM7_IO1 | PIO_PB4B_FLEXCOM7_IO0)
#define PINS_MIKROBUS2_USART_FLAGS           (IOPORT_MODE_MUX_B)

#define PINS_MIKROBUS2_USART_PORT            IOPORT_PIOB
#define PINS_MIKROBUS2_USART_MASK            (PIO_PB3B_FLEXCOM7_IO1 | PIO_PB4B_FLEXCOM7_IO0)
#define PINS_MIKROBUS2_USART_PIO             PIOB
#define PINS_MIKROBUS2_USART_ID              ID_PIOB
#define PINS_MIKROBUS2_USART_TYPE            IOPORT_MODE_MUX_B
#define PINS_MIKROBUS2_USART_ATTR            IOPORT_MODE_DEFAULT
/* @} */

/**
 * \name MIKROBUS2 Port TWI pins definitions
 * @{
 */
#define PINS_MIKROBUS2_TWI                   (PIO_PA12A_FLEXCOM2_IO0 | PIO_PA13A_FLEXCOM2_IO1)
#define PINS_MIKROBUS2_TWI_FLAGS             (IOPORT_MODE_MUX_A)

#define PINS_MIKROBUS2_TWI_PORT              IOPORT_PIOA
#define PINS_MIKROBUS2_TWI_MASK              (PIO_PA12A_FLEXCOM2_IO0 | PIO_PA13A_FLEXCOM2_IO1)
#define PINS_MIKROBUS2_TWI_PIO               PIOA
#define PINS_MIKROBUS2_TWI_ID                ID_PIOA
#define PINS_MIKROBUS2_TWI_TYPE              IOPORT_MODE_MUX_A
#define PINS_MIKROBUS2_TWI_ATTR              IOPORT_MODE_DEFAULT
/* @} */

/**
 * \name MIKROBUS2 Port SPI pins definitions
 * @{
 */
#define MIKROBUS2_SPI_MOSI_GPIO        (MIKROBUS2_PIN_SPI_MOSI)
#define MIKROBUS2_SPI_MOSI_FLAGS       (IOPORT_MODE_MUX_B)
#define MIKROBUS2_SPI_MISO_GPIO        (MIKROBUS2_PIN_SPI_MISO)
#define MIKROBUS2_SPI_MISO_FLAGS       (IOPORT_MODE_MUX_B)
#define MIKROBUS2_SPI_SPCK_GPIO        (MIKROBUS2_PIN_SPI_SCK)
#define MIKROBUS2_SPI_SPCK_FLAGS       (IOPORT_MODE_MUX_B)
#define MIKROBUS2_SPI_NPCS0_GPIO       (MIKROBUS2_PIN_SPI_CS)
#define MIKROBUS2_SPI_NPCS0_FLAGS      (IOPORT_MODE_MUX_B)
/* @} */

/**
 * \name ATSENSE301 pin definitions
 * @{
 */
/** ATSENSE301 SPCK */
#define ATSENSE301_MOSI_GPIO    (PIO_PD5_IDX)
#define ATSENSE301_MOSI_FLAGS   (IOPORT_MODE_MUX_A)
/** ATSENSE301 MOSI */
#define ATSENSE301_MISO_GPIO    (PIO_PD7_IDX)
#define ATSENSE301_MISO_FLAGS   (IOPORT_MODE_MUX_A)
/** ATSENSE301 MISO */
#define ATSENSE301_SPCK_GPIO    (PIO_PD6_IDX)
#define ATSENSE301_SPCK_FLAGS   (IOPORT_MODE_MUX_A)
/** ATSENSE301 NCPS */
#define ATSENSE301_NPCS0_GPIO   (PIO_PD8_IDX)
#define ATSENSE301_NPCS0_FLAGS  (IOPORT_MODE_MUX_A)
/** ATSENSE301 ITOUT */
#define ATSENSE301_ITOUT_GPIO   (PIO_PD0_IDX)
/** ATSENSE301 MCLK (PCK2) */
#define ATSENSE301_MCLK_GPIO    (PIO_PD4_IDX)
#define ATSENSE301_MCLK_FLAGS   (IOPORT_MODE_MUX_B)
/* @} */

/**
 * \name MCSPI pin definitions
 * @{
 */
/** MCSPI SPCK */
#define MCSPI_SPCK_GPIO    (PIO_PD5_IDX)
#define MCSPI_SPCK_FLAGS   (IOPORT_MODE_MUX_A)
/** MCSPI MISO */
#define MCSPI_MISO_GPIO    (PIO_PD6_IDX)
#define MCSPI_MISO_FLAGS   (IOPORT_MODE_MUX_A)
/** MCSPI MOSI0 */
#define MCSPI_MOSI0_GPIO   (PIO_PD7_IDX)
#define MCSPI_MOSI0_FLAGS  (IOPORT_MODE_MUX_A)
/** MCSPI MOSI1 */
#define MCSPI_MOSI1_GPIO   (PIO_PD8_IDX)
#define MCSPI_MOSI1_FLAGS  (IOPORT_MODE_MUX_B)
/** MCSPI MOSI2 */
#define MCSPI_MOSI2_GPIO   (PIO_PD9_IDX)
#define MCSPI_MOSI2_FLAGS  (IOPORT_MODE_MUX_B)
/** MCSPI MOSI3 */
#define MCSPI_MOSI3_GPIO   (PIO_PD10_IDX)
#define MCSPI_MOSI3_FLAGS  (IOPORT_MODE_MUX_B)
/** MCSPI NCPS0 */
#define MCSPI_NPCS0_GPIO   (PIO_PD8_IDX)
#define MCSPI_NPCS0_FLAGS  (IOPORT_MODE_MUX_A)
/** MCSPI NCPS1 */
#define MCSPI_NPCS1_GPIO   (PIO_PD9_IDX)
#define MCSPI_NPCS1_FLAGS  (IOPORT_MODE_MUX_A)
/** MCSPI NCPS2 */
#define MCSPI_NPCS2_GPIO   (PIO_PD10_IDX)
#define MCSPI_NPCS2_FLAGS  (IOPORT_MODE_MUX_A)
/** MCSPI NCPS3 */
#define MCSPI_NPCS3_GPIO   (PIO_PD11_IDX)
#define MCSPI_NPCS3_FLAGS  (IOPORT_MODE_MUX_A)
/* @} */

/**
 * \name PCK pin definitions
 * @{
 */
#define PIN_PCK0        (PIO_PC22_IDX)
#define PIN_PCK0_MUX    (IOPORT_MODE_MUX_A)

#define PIN_PCK1        (PIO_PB1_IDX)
#define PIN_PCK1_MUX    (IOPORT_MODE_MUX_C)

#define PIN_PCK2        (PIO_PC5_IDX)
#define PIN_PCK2_MUX    (IOPORT_MODE_MUX_A)
/* @} */

/**
 * \name Voltage Monitor pins definition
 * @{
 */
#define VDD_SENSE_GPIO                  PIO_PD16_IDX
/* @} */

/* @} */ /* End of PIC32CXMTG_EK_features_group */

/* @} */ /* End of PIC32CXMTG_EK_group */

#endif  /* PIC32CXMTG_EK_H_INCLUDED */
