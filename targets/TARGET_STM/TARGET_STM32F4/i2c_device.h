/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_I2C_DEVICE_H
#define MBED_I2C_DEVICE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_I2C

/*  Define IP version */
#define I2C_IP_VERSION_V1

#define I2C_IT_ALL (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR)

#endif // DEVICE_I2C

#ifdef __cplusplus
}
#endif

#endif
