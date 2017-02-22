/*
** I/O redirection support over UART, via SSL/DD.
** Copyright (C) 2017 Analog Devices, Inc. All Rights Reserved.
**
** This file is intended for use with the ARM:Compiler:IO:*:User
** components, which set up redirection of stdout and stderr.
*/

#ifndef RETARGET_UART_CONFIG_H
#define RETARGET_UART_CONFIG_H

// --- <<< Use Configuration Wizard in Context Menu >>> ---

// <h>UART Configuration for STDOUT and STDERR

// <o>Configure Pinmuxing for UART. <0-1>
// <i>Choose 0 if your application will configure UART pinmux, or 1 to config UART pinmux on first output.
#define ADI_UART_SETUP_PINMUX 1

// </h>

#endif /* RETARGET_UART_CONFIG_H */
