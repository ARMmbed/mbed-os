/**
 ******************************************************************************
 * @file uart.h
 * @brief Defines common properties of any UART driver.
 * @internal
 * @author ON Semiconductor
 * $Rev: 2074 $
 * $Date: 2013-07-10 18:06:15 +0530 (Wed, 10 Jul 2013) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @details
 * A UART driver must comply to the generic driver template (see driver.h), and
 * more specifically to the character driver template (see char_driver.h). All
 * UART drivers share some properties; defining these is the purpose of this file.
 *
 * The options passed when opening a UART device should at least include the
 * flow control type and the baud rate. These are included in the uart_options_t
 * data type. The type can be extended by a UART driver implementation.
 *
 * The UART driver implementation must make its driver publicly available with
 * an external global variable.
 *
 * @ingroup uart
 */

#ifndef UART_H_
#define UART_H_

//#include "char_driver.h"

/** Type listing the supported kinds of flow control. */
typedef enum {
    /** No flow control */
    none,
    /** use hardware CTS (External CPU indicates it is ok for the modem to transmit)
     * and RTS (modem requests to sent to external CPU) flow control.
     */
    rtscts,
    /** use hardware CTS/RTS flow control, but CTS is no response to RTS,
     * RTS and CTS are used to indicate intent to transmit.
     */
    rtscts_e
} flow_control_t;

/** A set of options to be passed when creating a uart device instance. */
typedef struct uart_options {
    uint32_t baudrate; /**< The expected baud rate. */
    flow_control_t control;/**< Defines type of flow control, none or rtscts */
} uart_options_t, *uart_options_pt;

#endif /* UART_H_ */
