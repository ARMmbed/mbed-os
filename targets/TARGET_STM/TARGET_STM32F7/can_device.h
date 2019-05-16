/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_CAN_DEVICE_H
#define MBED_CAN_DEVICE_H

#include "cmsis.h"
#include "stm32f7xx.h"

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

#endif
