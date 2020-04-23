/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RFBITS_H_
#define RFBITS_H_

#include "DigitalIn.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "SPI.h"
#include <Timer.h>
#include "Timeout.h"
#include "rtos.h"

using namespace mbed;
using namespace rtos;

class UnlockedSPI : public SPI {
public:
    UnlockedSPI(PinName mosi, PinName miso, PinName sclk) :
        SPI(mosi, miso, sclk) { }
    virtual void lock() { }
    virtual void unlock() { }
};

class RFBits {
public:
    RFBits(PinName spi_mosi, PinName spi_miso,
           PinName spi_sclk, PinName spi_cs,
           PinName spi_rst, PinName spi_slp, PinName spi_irq);
    UnlockedSPI spi;
    DigitalOut CS;
    DigitalOut RST;
    DigitalOut SLP_TR;
    InterruptIn IRQ;
    Timeout ack_timer;
    Timeout cal_timer;
    Timeout cca_timer;
    Timer tx_timer;
    int init_215_driver(RFBits *_rf, TestPins *_test_pins, const uint8_t mac[8], uint8_t *rf_part_num);
#ifdef MBED_CONF_RTOS_PRESENT
    Thread irq_thread;
    Thread irq_thread_215;
    Mutex mutex;
    void rf_if_irq_task();
    void rf_irq_task();
#endif
};

class TestPins {
public:
    TestPins(PinName test_pin_1, PinName test_pin_2, PinName test_pin_3, PinName test_pin_4, PinName test_pin_5);
    DigitalOut TEST1;
    DigitalOut TEST2;
    DigitalOut TEST3;
    DigitalOut TEST4;
    DigitalOut TEST5;
};

#endif /* RFBITS_H_ */
