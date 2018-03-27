/**************************************************************************//**
 * @file     LPC17xx.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File for
 *           NXP LPC17xx Device Series
 * @version: V1.09
 * @date:    17. March 2010

 *
 * @note
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef __SIMULATOR_DEVICE_H__
#define __SIMULATOR_DEVICE_H__

#include <stdint.h>
#include "cmsis.h"
#include "objects.h"
#include "gpio_object.h"
#include "PeripheralNames.h"
#include "PortNames.h"

#define osWaitForever         0xFFFFFFFFU ///< Wait forever timeout value.
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE     9600

#define DEVICE_INTERRUPTIN    1
#define DEVICE_ANALOGIN       1
#define DEVICE_ANALOGOUT      1
#define DEVICE_PWMOUT         1
// #define DEVICE_SERIAL         1

#endif  // __SIMULATOR_DEVICE_H__
