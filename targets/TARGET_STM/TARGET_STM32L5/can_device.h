/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_CAN_DEVICE_H
#define MBED_CAN_DEVICE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_CAN

#define CAN_NUM 1 // Number of CAN peripherals present in the STM32 serie

#define CAN1_IRQ_RX_IRQN        CAN1_RX0_IRQn
#define CAN1_IRQ_RX_VECT        CAN1_RX0_IRQHandler
#define CAN1_IRQ_TX_IRQN        CAN1_TX_IRQn
#define CAN1_IRQ_TX_VECT        CAN1_TX_IRQHandler
#define CAN1_IRQ_ERROR_IRQN     CAN1_SCE_IRQn
#define CAN1_IRQ_ERROR_VECT     CAN1_SCE_IRQHandler
#define CAN1_IRQ_PASSIVE_IRQN   CAN1_SCE_IRQn
#define CAN1_IRQ_PASSIVE_VECT   CAN1_SCE_IRQHandler
#define CAN1_IRQ_BUS_IRQN       CAN1_SCE_IRQn
#define CAN1_IRQ_BUS_VECT       CAN1_SCE_IRQHandler

#endif // DEVICE_CAN

#endif
