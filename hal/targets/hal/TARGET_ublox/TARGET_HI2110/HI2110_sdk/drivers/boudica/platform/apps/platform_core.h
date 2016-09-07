/******************************************************************************
 * @brief    BOUDICA APPS Platform Definitions
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/

#ifndef PLATFORM_CORE_H_
#define PLATFORM_CORE_H_

#include "platform_definitions.h"

/** @addtogroup PLATFORM
 * @{
 * */

/** Number of UARTS */
#define PLATFORM_NUM_UARTS 2

/** Functions supported by Application PINS */
typedef enum {
    FUNC_UNCLAIMED,
    FUNC_GPIO,
    FUNC_PWM0,
    FUNC_PWM0B,
    FUNC_PWM1,
    FUNC_PWM1B,
    FUNC_SSP0TXD,
    FUNC_SSP0RXD,
    FUNC_SSP0TRX,
    FUNC_SSP0CLK,
    FUNC_SSP0FSS,
    FUNC_SSP1CLK,
    FUNC_SSP1FSS,
    FUNC_SSP1TXD,
    FUNC_SSP1RXD,
    FUNC_SSP1TRX,
    FUNC_I2C_SDA,
    FUNC_I2C_SCK,
    FUNC_UART0_TXD,
    FUNC_UART0_TXIR,
    FUNC_UART0_O2,
    FUNC_UART0_O1,
    FUNC_UART0_RTS,
    FUNC_UART0_DTR,
    FUNC_UART0_RXD,
    FUNC_UART0_RXIR,
    FUNC_UART0_CTS,
    FUNC_UART0_DCD,
    FUNC_UART0_DSR,
    FUNC_UART0_RI,
    FUNC_UART1_TXD,
    FUNC_UART1_TXIR,
    FUNC_UART1_O2,
    FUNC_UART1_O1,
    FUNC_UART1_RTS,
    FUNC_UART1_DTR,
    FUNC_UART1_RXD,
    FUNC_UART1_RXIR,
    FUNC_UART1_CTS,
    FUNC_UART1_DCD,
    FUNC_UART1_DSR,
    FUNC_UART1_RI,
    FUNC_LPUART,
} FUNC;

/** @} */

#endif /* PLATFORM_CORE_H_ */
