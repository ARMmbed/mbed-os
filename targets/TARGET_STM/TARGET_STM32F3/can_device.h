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

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEVICE_CAN

#define CAN_NUM    1 // Number of CAN peripherals present in the STM32 serie

#define CAN1_IRQ_RX_IRQN        CAN_RX0_IRQn
#define CAN1_IRQ_RX_VECT        CAN_RX0_IRQHandler
#define CAN1_IRQ_TX_IRQN        CAN_TX_IRQn
#define CAN1_IRQ_TX_VECT        CAN_TX_IRQHandler
#define CAN1_IRQ_ERROR_IRQN     CAN_SCE_IRQn
#define CAN1_IRQ_ERROR_VECT     CAN_SCE_IRQHandler
#define CAN1_IRQ_PASSIVE_IRQN   CAN_SCE_IRQn
#define CAN1_IRQ_PASSIVE_VECT   CAN_SCE_IRQHandler
#define CAN1_IRQ_BUS_IRQN       CAN_SCE_IRQn
#define CAN1_IRQ_BUS_VECT       CAN_SCE_IRQHandler

#endif // DEVICE_CAN

#endif
