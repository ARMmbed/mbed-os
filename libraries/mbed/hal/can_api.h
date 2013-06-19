/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_CAN_API_H
#define MBED_CAN_API_H

#include "device.h"

#if DEVICE_CAN

#include "PinNames.h"
#include "PeripheralNames.h"
#include "can_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct can_s can_t;

void          can_init     (can_t *obj, PinName rd, PinName td);
void          can_free     (can_t *obj);
int           can_frequency(can_t *obj, int hz);
int           can_write    (can_t *obj, CAN_Message, int cc);
int           can_read     (can_t *obj, CAN_Message *msg);
void          can_reset    (can_t *obj);
unsigned char can_rderror  (can_t *obj);
unsigned char can_tderror  (can_t *obj);
void          can_monitor  (can_t *obj, int silent);

#ifdef __cplusplus
};
#endif

#endif    // MBED_CAN_API_H

#endif
