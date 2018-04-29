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

// Serial configuration
#ifndef MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE     9600
#endif

// LoRaWAN configuration
#ifndef MBED_CONF_LORA_PHY
#define MBED_CONF_LORA_PHY                          0
#endif

#ifndef MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION
#define MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION      false
#endif

#ifndef MBED_CONF_LORA_NB_TRIALS
#define MBED_CONF_LORA_NB_TRIALS                    12
#endif

#ifndef MBED_CONF_LORA_DEVICE_EUI
#define MBED_CONF_LORA_DEVICE_EUI                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

#ifndef MBED_CONF_LORA_APPLICATION_EUI
#define MBED_CONF_LORA_APPLICATION_EUI              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

#ifndef MBED_CONF_LORA_APPLICATION_KEY
#define MBED_CONF_LORA_APPLICATION_KEY              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

#ifndef MBED_CONF_LORA_DEVICE_ADDRESS
#define MBED_CONF_LORA_DEVICE_ADDRESS               0x00000000
#endif

#ifndef MBED_CONF_LORA_NWKSKEY
#define MBED_CONF_LORA_NWKSKEY                      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

#ifndef MBED_CONF_LORA_APPSKEY
#define MBED_CONF_LORA_APPSKEY                      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

#ifndef MBED_CONF_LORA_APP_PORT
#define MBED_CONF_LORA_APP_PORT                     15
#endif

#ifndef MBED_CONF_LORA_TX_MAX_SIZE
#define MBED_CONF_LORA_TX_MAX_SIZE                  255
#endif

#ifndef MBED_CONF_LORA_ADR_ON
#define MBED_CONF_LORA_ADR_ON                       1
#endif

#ifndef MBED_CONF_LORA_PUBLIC_NETWORK
#define MBED_CONF_LORA_PUBLIC_NETWORK               true
#endif

#ifndef MBED_CONF_LORA_DUTY_CYCLE_ON
#define MBED_CONF_LORA_DUTY_CYCLE_ON                true
#endif

#ifndef MBED_CONF_LORA_LBT_ON
#define MBED_CONF_LORA_LBT_ON                       false
#endif

// Tracing (enable here, so mbed_trace is compiled in)
#ifndef MBED_CONF_MBED_TRACE_ENABLE
#define MBED_CONF_MBED_TRACE_ENABLE                 1
#endif

// Device features

#define DEVICE_INTERRUPTIN    1
#define DEVICE_ANALOGIN       1
#define DEVICE_ANALOGOUT      1
#define DEVICE_PWMOUT         1
#define DEVICE_SERIAL         1

#endif  // __SIMULATOR_DEVICE_H__
