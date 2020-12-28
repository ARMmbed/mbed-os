
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include "pinmap.h"

#if DEVICE_CAN

#include "PinNames.h"
#include "PeripheralNames.h"
#include "hal/can_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IRQ_RX,
    IRQ_TX,
    IRQ_ERROR,
    IRQ_OVERRUN,
    IRQ_WAKEUP,
    IRQ_PASSIVE,
    IRQ_ARB,
    IRQ_BUS,
    IRQ_READY
} CanIrqType;


typedef enum {
    MODE_RESET,
    MODE_NORMAL,
    MODE_SILENT,
    MODE_TEST_LOCAL,
    MODE_TEST_GLOBAL,
    MODE_TEST_SILENT
} CanMode;

typedef struct {
    int peripheral;
    PinName rd_pin;
    int rd_function;
    PinName td_pin;
    int td_function;
} can_pinmap_t;

typedef void (*can_irq_handler)(uint32_t id, CanIrqType type);

typedef struct can_s can_t;

void          can_init(can_t *obj, PinName rd, PinName td);
void          can_init_direct(can_t *obj, const can_pinmap_t *pinmap);
void          can_init_freq(can_t *obj, PinName rd, PinName td, int hz);
void          can_init_freq_direct(can_t *obj, const can_pinmap_t *pinmap, int hz);
void          can_free(can_t *obj);
int           can_frequency(can_t *obj, int hz);

void          can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id);
void          can_irq_free(can_t *obj);
void          can_irq_set(can_t *obj, CanIrqType irq, uint32_t enable);

int           can_write(can_t *obj, CAN_Message, int cc);
int           can_read(can_t *obj, CAN_Message *msg, int handle);
int           can_mode(can_t *obj, CanMode mode);
int           can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle);
void          can_reset(can_t *obj);
unsigned char can_rderror(can_t *obj);
unsigned char can_tderror(can_t *obj);
void          can_monitor(can_t *obj, int silent);

/** Get the pins that support CAN RD
 *
 * Return a PinMap array of pins that support CAN RD. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *can_rd_pinmap(void);

/** Get the pins that support CAN TD
 *
 * Return a PinMap array of pins that support CAN TD. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *can_td_pinmap(void);

#ifdef __cplusplus
}
#endif

#endif    // MBED_CAN_API_H

#endif

/** @}*/
