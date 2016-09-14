/******************************************************************************
 * @brief    BOUDICA SARA MODULE PCB CONFIGURATION
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/


#ifndef SRC_LIB_PCB_CONFIG_BOUDICA_SARA_PCB_CONFIG_H_
#define SRC_LIB_PCB_CONFIG_BOUDICA_SARA_PCB_CONFIG_H_

#include "core.h"

#if CORE == SECURITY

#define UART_RX_PIN     PIN_7
#define UART_TX_PIN     PIN_6
#define UART_CTS_PIN    PIN_12
#define UART_RTS_PIN    PIN_11

#elif CORE == PROTOCOL

#define UART_RX_PIN     PIN_7
#define UART_TX_PIN     PIN_6
#define UART_CTS_PIN    PIN_12
#define UART_RTS_PIN    PIN_11

// The following defines are used in the test_suite :
// We can remove them when we change the build system to include some tests or not depending on the platform:

/* CLOCK */
#define CLOCK_ROUTE_PIN                  PIN_NONE

/* SPI  */
#define SPI_UNIDIR_OUT_PIN               PIN_NONE
#define SPI_CLK_PIN                      PIN_NONE
#define SPI_CSB_PIN                      PIN_NONE
#define SPI_UNIDIR_IN_PIN                PIN_NONE

/* QUAD SPI STANDARD */
#define QUAD_SPI_STD_UNIDIR_IN_PIN       PIN_NONE
#define QUAD_SPI_STD_UNIDIR_OUT_PIN      PIN_NONE
#define QUAD_SPI_STD_CLK_PIN             PIN_NONE
#define QUAD_SPI_STD_CSB_PIN             PIN_NONE
#define QUAD_SPI_STD_HOLD_PIN            PIN_NONE

/* QUAD SPI QUAD */
#define QUAD_SPI_BIDIR_DATA0_PIN         PIN_NONE
#define QUAD_SPI_BIDIR_DATA1_PIN         PIN_NONE
#define QUAD_SPI_BIDIR_DATA2_PIN         PIN_NONE
#define QUAD_SPI_BIDIR_DATA3_PIN         PIN_NONE
#define QUAD_SPI_HOLD_PIN                PIN_NONE
#define QUAD_SPI_CLK_PIN                 PIN_NONE
#define QUAD_SPI_CSB_PIN                 PIN_NONE
#define QUAD_SPI_UNIDIR_IN_PIN           PIN_NONE
#define QUAD_SPI_UNIDIR_OUT_PIN          PIN_NONE

/* CONTROL */
#define SS_PIN               			 PIN_NONE

#elif CORE == APPS

#define UART_RX_PIN     PIN_7
#define UART_TX_PIN     PIN_6
#define UART_CTS_PIN    PIN_12
#define UART_RTS_PIN    PIN_11

/* LPUART */
#define LPUART_RX_PIN                PIN_13
#define VIRTUAL_UART_MAIN_TX_PIN     PIN_18

// The following defines are used in the test_suite :
// We can remove them when we change the build system to include some tests or not depending on the platform:
#define SPP_CLK_PIN          PIN_NONE
#define SPP_MASTER_TXD_PIN   PIN_NONE
#define SPP_MASTER_RXD_PIN   PIN_NONE
#define SPP_SLAVE_RXD_PIN    PIN_NONE
#define SPP_SLAVE_TXD_PIN    PIN_NONE
#define SPP_SLAVE_FSS_PIN    PIN_NONE
#define I2C_SDA_PIN          PIN_NONE
#define I2C_SCK_PIN          PIN_NONE
#define GPIO_TEST_PIN        PIN_NONE
#define SS_PIN               PIN_NONE
#define TRINNY_RESET_PIN     PIN_NONE
#define TRINNY_UART_RX_PIN   PIN_NONE
#define TRINNY_UART_TX_PIN   PIN_NONE
#endif

#endif /* SRC_LIB_PCB_CONFIG_BOUDICA_SARA_PCB_CONFIG_H_ */
