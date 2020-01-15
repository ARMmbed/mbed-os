/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016-2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
*/

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#if !defined(MBED_ROM_START)
#define MBED_ROM_START  0x8000000
#endif

#if !defined(MBED_ROM_SIZE)
#if defined (TARGET_NUCLEO_G071RB)
#define MBED_ROM_SIZE  0x20000
#else
 //  0x10000 STM32G071C8Ux STM32G071K8TxN STM32G071R8Tx STM32G071K8Ux STM32G071G8UxN STM32G071C8Tx STM32G071K8Tx STM32G071K8UxN STM32G071G8Ux 
 //   0x8000 STM32G071K6Tx STM32G071G6Ux STM32G071C6Tx STM32G071K6Ux STM32G071R6Tx STM32G071C6Ux 
 //  0x20000 STM32G071GBUxN STM32G071CBUx STM32G071KBTx STM32G071KBUxN STM32G071RBTx STM32G071EBYx STM32G071GBUx STM32G071CBTx STM32G071KBUx STM32G071RBIx STM32G071KBTxN 
#error "MBED_ROM_SIZE not defined"
#endif
#endif

#if !defined(MBED_RAM_START)
#define MBED_RAM_START  0x20000000
#endif

#if !defined(MBED_RAM_SIZE)
#define MBED_RAM_SIZE  0x9000
#endif

#define NVIC_NUM_VECTORS        47
#define NVIC_RAM_VECTOR_ADDRESS MBED_RAM_START

#endif
