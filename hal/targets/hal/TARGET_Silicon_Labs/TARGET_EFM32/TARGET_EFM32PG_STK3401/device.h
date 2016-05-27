/***************************************************************************//**
 * @file device.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#define DEVICE_PORTIN           1
#define DEVICE_PORTOUT          1
#define DEVICE_PORTINOUT        1

#define DEVICE_INTERRUPTIN      1

#define DEVICE_ANALOGIN         1
#define DEVICE_ANALOGOUT        0

#define DEVICE_SERIAL           1
#define DEVICE_SERIAL_ASYNCH    1

#define DEVICE_I2C              1
#define DEVICE_I2CSLAVE         1
#define DEVICE_I2C_ASYNCH       1

#define DEVICE_SPI              1
#define DEVICE_SPISLAVE         1
#define DEVICE_SPI_ASYNCH       1

#define DEVICE_CAN              0

#define DEVICE_RTC              1

#define DEVICE_ETHERNET         0

#define DEVICE_PWMOUT           1

#define DEVICE_SLEEP            1

#define DEVICE_STDIO_MESSAGES   1

#define DEVICE_LOWPOWERTIMER    1

#define DEVICE_ERROR_PATTERN    1

#include "objects.h"
#include "Modules.h"
#include "device_peripherals.h"

#endif
