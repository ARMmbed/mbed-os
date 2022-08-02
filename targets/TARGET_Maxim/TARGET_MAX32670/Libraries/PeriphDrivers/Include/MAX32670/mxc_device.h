/**
 * @file    mxc_device.h
 * @brief   Device specific header file.
*/

/*******************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

#ifndef _MXC_DEVICE_H_
#define _MXC_DEVICE_H_

#include "max32670.h"
#include "mxc_errors.h"
#include "mxc_pins.h"

#if defined ( __ICCARM__ ) || (__CC_ARM)
#include "RTE_Components.h"
#endif 

#ifndef TARGET
#error TARGET NOT DEFINED
#endif

// Create a string definition for the TARGET
#define STRING_ARG(arg) #arg
#define STRING_NAME(name) STRING_ARG(name)
#if MBED_VERSION && MBED_VERSION < 51200
#define TARGET_NAME STRING_NAME(TARGET)
#endif

// Define which revisions of the IP we are using
#ifndef TARGET_REV
#error TARGET_REV NOT DEFINED
#endif

#if(TARGET_REV == 0x4131)
// A1
#define MXC_TMR_REV         0
#define MXC_UART_REV        0
#else

#error TARGET_REV NOT SUPPORTED

#endif /* if(TARGET_REV == ...) */

#endif  /* _MXC_DEVICE_H_ */
