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

// <q>Configure Pinmuxing for UART.
// <i>Enable pinmux configuration for UART on first output.
#define ADI_UART_SETUP_PINMUX 1

// <q>Raise Breakpoint on exit()
// <i>Cause a breakpoint event in exit() rather than looping forever.
#define ADI_UART_EXIT_BREAKPOINT 1


// </h>

#endif /* RETARGET_UART_CONFIG_H */
