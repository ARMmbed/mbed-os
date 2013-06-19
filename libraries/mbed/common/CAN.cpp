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
#include "CAN.h"

#if DEVICE_CAN

#include "cmsis.h"

namespace mbed {

CAN::CAN(PinName rd, PinName td) {
    can_init(&_can, rd, td);
}

CAN::~CAN() {
    can_free(&_can);
}

int CAN::frequency(int f) {
    return can_frequency(&_can, f);
}

int CAN::write(CANMessage msg) {
    return can_write(&_can, msg, 0);
}

int CAN::read(CANMessage &msg) {
    return can_read(&_can, &msg);
}

void CAN::reset() {
    can_reset(&_can);
}

unsigned char CAN::rderror() {
    return can_rderror(&_can);
}

unsigned char CAN::tderror() {
    return can_tderror(&_can);
}

void CAN::monitor(bool silent) {
    can_monitor(&_can, (silent) ? 1 : 0);
}

static FunctionPointer* can_obj[2] = { NULL };

// Have to check that the CAN block is active before reading the Interrupt
// Control Register, or the mbed hangs
void can_irq(void) {
    uint32_t icr;

    if(LPC_SC->PCONP & (1 << 13)) {
        icr = LPC_CAN1->ICR;

        if(icr && (can_obj[0] != NULL)) {
            can_obj[0]->call();
        }
    }

    if(LPC_SC->PCONP & (1 << 14)) {
        icr = LPC_CAN2->ICR;
        if(icr && (can_obj[1] != NULL)) {
            can_obj[1]->call();
        }
    }

}

void CAN::setup_interrupt(void) {
    switch ((int)_can.dev) {
        case CAN_1: can_obj[0] = &_rxirq; break;
        case CAN_2: can_obj[1] = &_rxirq; break;
    }
    _can.dev->MOD |= 1;
    _can.dev->IER |= 1;
    _can.dev->MOD &= ~1;
    NVIC_SetVector(CAN_IRQn, (uint32_t) &can_irq);
    NVIC_EnableIRQ(CAN_IRQn);
}

void CAN::remove_interrupt(void) {
    switch ((int)_can.dev) {
        case CAN_1: can_obj[0] = NULL; break;
        case CAN_2: can_obj[1] = NULL; break;
    }

    _can.dev->IER &= ~(1);
    if ((can_obj[0] == NULL) && (can_obj[1] == NULL)) {
        NVIC_DisableIRQ(CAN_IRQn);
    }
}

void CAN::attach(void (*fptr)(void)) {
    if (fptr != NULL) {
        _rxirq.attach(fptr);
        setup_interrupt();
    } else {
        remove_interrupt();
    }
}

} // namespace mbed

#endif
