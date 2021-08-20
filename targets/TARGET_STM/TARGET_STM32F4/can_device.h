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

#ifndef MBED_CAN_DEVICE_H
#define MBED_CAN_DEVICE_H

#include "cmsis.h"
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_CAN

#if defined(CAN3_BASE)

#define CAN_NUM 3 // Number of CAN peripherals present in the STM32 serie

#define CAN3_IRQ_RX_IRQN        CAN3_RX0_IRQn
#define CAN3_IRQ_RX_VECT        CAN3_RX0_IRQHandler
#define CAN3_IRQ_TX_IRQN        CAN3_TX_IRQn
#define CAN3_IRQ_TX_VECT        CAN3_TX_IRQHandler
#define CAN3_IRQ_ERROR_IRQN     CAN3_SCE_IRQn
#define CAN3_IRQ_ERROR_VECT     CAN3_SCE_IRQHandler
#define CAN3_IRQ_PASSIVE_IRQN   CAN3_SCE_IRQn
#define CAN3_IRQ_PASSIVE_VECT   CAN3_SCE_IRQHandler
#define CAN3_IRQ_BUS_IRQN       CAN3_SCE_IRQn
#define CAN3_IRQ_BUS_VECT       CAN3_SCE_IRQHandler

#else

#define CAN_NUM 2 // Number of CAN peripherals present in the STM32 serie

#endif

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

#define CAN2_IRQ_RX_IRQN        CAN2_RX0_IRQn
#define CAN2_IRQ_RX_VECT        CAN2_RX0_IRQHandler
#define CAN2_IRQ_TX_IRQN        CAN2_TX_IRQn
#define CAN2_IRQ_TX_VECT        CAN2_TX_IRQHandler
#define CAN2_IRQ_ERROR_IRQN     CAN2_SCE_IRQn
#define CAN2_IRQ_ERROR_VECT     CAN2_SCE_IRQHandler
#define CAN2_IRQ_PASSIVE_IRQN   CAN2_SCE_IRQn
#define CAN2_IRQ_PASSIVE_VECT   CAN2_SCE_IRQHandler
#define CAN2_IRQ_BUS_IRQN       CAN2_SCE_IRQn
#define CAN2_IRQ_BUS_VECT       CAN2_SCE_IRQHandler

#endif // DEVICE_CAN

#ifdef __cplusplus
}
#endif

#endif
