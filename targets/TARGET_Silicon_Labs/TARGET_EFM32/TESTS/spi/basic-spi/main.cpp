/***************************************************************************//**
 * @file main.cpp
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "mbed.h"

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_prs.h"
#include "em_timer.h"

#define SHORT_TRANSFER_FRAMES 16
#define LONG_TRANSFER_FRAMES 1842

#if defined(TARGET_EFM32GG_STK3700) || defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32WG_STK3800)

#define TEST 1

#define TEST_UART USART1
#define MOSI_PIN PD0
#define MISO_PIN PD1
#define CLK_PIN  PD2
#define CS_PIN   PD3

#define GPIO_PORT    gpioPortD
#define GPIO_PIN_TX  0
#define GPIO_PIN_CLK 2
#define GPIO_PRS_SRC_TX PRS_CH_CTRL_SOURCESEL_GPIOL
#define GPIO_PRS_SIG_TX PRS_CH_CTRL_SIGSEL_GPIOPIN0
#define GPIO_PRS_SRC_CLK PRS_CH_CTRL_SOURCESEL_GPIOL
#define GPIO_PRS_SIG_CLK PRS_CH_CTRL_SIGSEL_GPIOPIN2

#define TEST_TIMER TIMER1
#define TEST_TIMER_CLOCK cmuClock_TIMER1

#elif defined(TARGET_EFM32PG_STK3401)

#define TEST 1

#define TEST_UART USART1
#define MOSI_PIN PC6
#define MISO_PIN PC7
#define CLK_PIN  PC8
#define CS_PIN   PC9

#define GPIO_PORT    gpioPortC
#define GPIO_PIN_TX  6
#define GPIO_PIN_CLK 8
#define GPIO_PRS_SRC_TX PRS_CH_CTRL_SOURCESEL_GPIOL
#define GPIO_PRS_SIG_TX PRS_CH_CTRL_SIGSEL_GPIOPIN6
#define GPIO_PRS_SRC_CLK PRS_CH_CTRL_SOURCESEL_GPIOH
#define GPIO_PRS_SIG_CLK PRS_CH_CTRL_SIGSEL_GPIOPIN8

#define TEST_TIMER TIMER1
#define TEST_TIMER_CLOCK cmuClock_TIMER1

#else
#define TEST 0
#warning "Test config not defined; skipping test"
#endif

using namespace utest::v1;

#if TEST
DigitalOut cs(CS_PIN);
static volatile bool complete;
event_callback_t cb;

static uint8_t short_data_8[SHORT_TRANSFER_FRAMES];
static uint16_t short_data_16[SHORT_TRANSFER_FRAMES];
static uint32_t short_data_32[SHORT_TRANSFER_FRAMES];

static uint8_t short_data_8_rx[SHORT_TRANSFER_FRAMES];
static uint16_t short_data_16_rx[SHORT_TRANSFER_FRAMES];
static uint32_t short_data_32_rx[SHORT_TRANSFER_FRAMES];

static uint8_t long_data_8[LONG_TRANSFER_FRAMES];
static uint16_t long_data_16[LONG_TRANSFER_FRAMES];

static uint8_t long_data_8_rx[LONG_TRANSFER_FRAMES];
static uint16_t long_data_16_rx[LONG_TRANSFER_FRAMES];

void callbackFunction(int flags) {
    complete = true;
}

void init_timer() {
    CMU_ClockEnable(cmuClock_PRS, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(TEST_TIMER_CLOCK, true);

    // Setup USART TX pin as PRS producer
    GPIO_IntConfig(GPIO_PORT, GPIO_PIN_TX, false, false, false);
    PRS_SourceSignalSet(0, 
                        GPIO_PRS_SRC_TX,
                        GPIO_PRS_SIG_TX,
                        prsEdgeOff);

    // Setup USART CLK pin as PRS producer
    GPIO_IntConfig(GPIO_PORT, GPIO_PIN_CLK, false, false, false);
    PRS_SourceSignalSet(1, 
                        GPIO_PRS_SRC_CLK,
                        GPIO_PRS_SIG_CLK,
                        prsEdgeOff);

    // Setup timer to count on PRS pulses
    TIMER_Init_TypeDef timInit = TIMER_INIT_DEFAULT;
    timInit.enable = false;
    timInit.clkSel = timerClkSelCC1;

    TIMER_InitCC_TypeDef timInitCC = TIMER_INITCC_DEFAULT;
    timInitCC.prsInput = true;
    timInitCC.prsSel = timerPRSSELCh1;

    TIMER_Init(TEST_TIMER, &timInit);
    TIMER_InitCC(TEST_TIMER, 1, &timInitCC);

    TIMER_Enable(TEST_TIMER, true);
}

template<typename T>
void init_arrays(T * tx, T * rx, int len, uint32_t mask) {
    for (uint32_t i = 0; i < len; i++) {
        if (tx) {
            tx[i] = i & mask;
        }
        if (rx) {
            rx[i] = 0;
        }
    }
}

template<typename T>
void test_transfer(int bits, int polarity, int freq, DMAUsage dma, T * data_tx, T * data_rx, int len) {
    SPI spi(MOSI_PIN, MISO_PIN, CLK_PIN);

    spi.format(bits, polarity);
    spi.frequency(freq);

    spi.set_dma_usage(dma);

    // Clear RX buffer, setup tx pattern
    init_arrays(data_tx, data_rx, len, (1 << bits) - 1);

    // Set up PRS loopback of TX data to RX
    TEST_UART->INPUT = USART_INPUT_RXPRS | USART_INPUT_RXPRSSEL_PRSCH0;

    complete = false;
    TIMER1->CNT = 0;
    cs = 0;
    spi.transfer(data_tx, len, data_rx, data_rx ? len : 0, cb);

    while (!complete);

    uint32_t xferred = TIMER1->CNT;
    cs = 1;

    // Check that all bits were sent
    TEST_ASSERT_EQUAL(bits*len, xferred);

    // Check that all data was received correctly
    if (data_rx) {
        for (int i = 0; i < len; i++) {
            TEST_ASSERT_EQUAL(data_tx[i], data_rx[i]);
        }
    }
}

////////////////////////////////
// Short single transfers

void test_5bit_8bit_0_1mhz_short_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_NEVER, short_data_8, short_data_8_rx, SHORT_TRANSFER_FRAMES);
}

void test_5bit_8bit_0_1mhz_short_dma_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_8, short_data_8_rx, SHORT_TRANSFER_FRAMES);
}

void test_5bit_16bit_0_1mhz_short_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_NEVER, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

void test_5bit_16bit_0_1mhz_short_dma_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

void test_8bit_8bit_0_1mhz_short_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_NEVER, short_data_8, short_data_8_rx, SHORT_TRANSFER_FRAMES);
}

void test_8bit_8bit_0_1mhz_short_dma_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_8, short_data_8_rx, SHORT_TRANSFER_FRAMES);
}

void test_8bit_16bit_0_1mhz_short_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_NEVER, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

void test_8bit_16bit_0_1mhz_short_dma_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

////////////////////////////////
// Short extended/double transfers

void test_9bit_16bit_0_1mhz_short_transfer() {
    test_transfer(9, 0, 1000000, DMA_USAGE_NEVER, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

void test_9bit_16bit_0_1mhz_short_dma_transfer() {
    test_transfer(9, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

void test_9bit_32bit_0_1mhz_short_transfer() {
    test_transfer(9, 0, 1000000, DMA_USAGE_NEVER, short_data_32, short_data_32_rx, SHORT_TRANSFER_FRAMES);
}

void test_9bit_32bit_0_1mhz_short_dma_transfer() {
    test_transfer(9, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_32, short_data_32_rx, SHORT_TRANSFER_FRAMES);
}

void test_16bit_16bit_0_1mhz_short_transfer() {
    test_transfer(16, 0, 1000000, DMA_USAGE_NEVER, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

void test_16bit_16bit_0_1mhz_short_dma_transfer() {
    test_transfer(16, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_16, short_data_16_rx, SHORT_TRANSFER_FRAMES);
}

void test_16bit_32bit_0_1mhz_short_transfer() {
    test_transfer(16, 0, 1000000, DMA_USAGE_NEVER, short_data_32, short_data_32_rx, SHORT_TRANSFER_FRAMES);
}

void test_16bit_32bit_0_1mhz_short_dma_transfer() {
    test_transfer(16, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, short_data_32, short_data_32_rx, SHORT_TRANSFER_FRAMES);
}

////////////////////////////////
// Long single transfers

void test_5bit_8bit_0_1mhz_long_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_NEVER, long_data_8, long_data_8_rx, LONG_TRANSFER_FRAMES);
}

void test_5bit_8bit_0_1mhz_long_dma_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, long_data_8, long_data_8_rx, LONG_TRANSFER_FRAMES);
}

void test_5bit_16bit_0_1mhz_long_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_NEVER, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

void test_5bit_16bit_0_1mhz_long_dma_transfer() {
    test_transfer(5, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

void test_8bit_8bit_0_1mhz_long_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_NEVER, long_data_8, long_data_8_rx, LONG_TRANSFER_FRAMES);
}

void test_8bit_8bit_0_1mhz_long_dma_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, long_data_8, long_data_8_rx, LONG_TRANSFER_FRAMES);
}

void test_8bit_16bit_0_1mhz_long_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_NEVER, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

void test_8bit_16bit_0_1mhz_long_dma_transfer() {
    test_transfer(8, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

////////////////////////////////
// Long extended/double transfers

void test_9bit_16bit_0_1mhz_long_transfer() {
    test_transfer(9, 0, 1000000, DMA_USAGE_NEVER, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

void test_9bit_16bit_0_1mhz_long_dma_transfer() {
    test_transfer(9, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

void test_16bit_16bit_0_1mhz_long_transfer() {
    test_transfer(16, 0, 1000000, DMA_USAGE_NEVER, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

void test_16bit_16bit_0_1mhz_long_dma_transfer() {
    test_transfer(16, 0, 1000000, DMA_USAGE_OPPORTUNISTIC, long_data_16, long_data_16_rx, LONG_TRANSFER_FRAMES);
}

////////////////////////////////
#else
void test_dummy() {
    TEST_IGNORE_MESSAGE("This test is not compatible with this target.");
}
#endif

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
#if TEST
    Case("5-bit frames with 8-bit data, 1 MHz (short)", test_5bit_8bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("5-bit frames with 8-bit data, 1 MHz, DMA (short)", test_5bit_8bit_0_1mhz_short_dma_transfer, greentea_failure_handler),
    Case("5-bit frames with 16-bit data, 1 MHz (short)", test_5bit_16bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("5-bit frames with 16-bit data, 1 MHz, DMA (short)", test_5bit_16bit_0_1mhz_short_dma_transfer, greentea_failure_handler),
    Case("8-bit frames with 8-bit data, 1 MHz (short)", test_8bit_8bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("8-bit frames with 8-bit data, 1 MHz, DMA (short)", test_8bit_8bit_0_1mhz_short_dma_transfer, greentea_failure_handler),
    Case("8-bit frames with 16-bit data, 1 MHz (short)", test_8bit_16bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("8-bit frames with 16-bit data, 1 MHz, DMA (short)", test_8bit_16bit_0_1mhz_short_dma_transfer, greentea_failure_handler),
    Case("9-bit frames with 16-bit data, 1 MHz (short)", test_9bit_16bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("9-bit frames with 16-bit data, 1 MHz, DMA (short)", test_9bit_16bit_0_1mhz_short_dma_transfer, greentea_failure_handler),
    Case("9-bit frames with 32-bit data, 1 MHz (short)", test_9bit_32bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("9-bit frames with 32-bit data, 1 MHz, DMA (short)", test_9bit_32bit_0_1mhz_short_dma_transfer, greentea_failure_handler),
    Case("16-bit frames with 16-bit data, 1 MHz (short)", test_16bit_16bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("16-bit frames with 16-bit data, 1 MHz, DMA (short)", test_16bit_16bit_0_1mhz_short_dma_transfer, greentea_failure_handler),
    Case("16-bit frames with 32-bit data, 1 MHz (short)", test_16bit_32bit_0_1mhz_short_transfer, greentea_failure_handler),
    Case("16-bit frames with 32-bit data, 1 MHz, DMA (short)", test_16bit_32bit_0_1mhz_short_dma_transfer, greentea_failure_handler),

    Case("5-bit frames with 8-bit data, 1 MHz (long)", test_5bit_8bit_0_1mhz_long_transfer, greentea_failure_handler),
    Case("5-bit frames with 8-bit data, 1 MHz, DMA (long)", test_5bit_8bit_0_1mhz_long_dma_transfer, greentea_failure_handler),
    Case("5-bit frames with 16-bit data, 1 MHz (long)", test_5bit_16bit_0_1mhz_long_transfer, greentea_failure_handler),
    Case("5-bit frames with 16-bit data, 1 MHz, DMA (long)", test_5bit_16bit_0_1mhz_long_dma_transfer, greentea_failure_handler),
    Case("8-bit frames with 8-bit data, 1 MHz (long)", test_8bit_8bit_0_1mhz_long_transfer, greentea_failure_handler),
    Case("8-bit frames with 8-bit data, 1 MHz, DMA (long)", test_8bit_8bit_0_1mhz_long_dma_transfer, greentea_failure_handler),
    Case("8-bit frames with 16-bit data, 1 MHz (long)", test_8bit_16bit_0_1mhz_long_transfer, greentea_failure_handler),
    Case("8-bit frames with 16-bit data, 1 MHz, DMA (long)", test_8bit_16bit_0_1mhz_long_dma_transfer, greentea_failure_handler),
    Case("9-bit frames with 16-bit data, 1 MHz (long)", test_9bit_16bit_0_1mhz_long_transfer, greentea_failure_handler),
    Case("9-bit frames with 16-bit data, 1 MHz, DMA (long)", test_9bit_16bit_0_1mhz_long_dma_transfer, greentea_failure_handler),
    Case("16-bit frames with 16-bit data, 1 MHz (long)", test_16bit_16bit_0_1mhz_long_transfer, greentea_failure_handler),
    Case("16-bit frames with 16-bit data, 1 MHz, DMA (long)", test_16bit_16bit_0_1mhz_long_dma_transfer, greentea_failure_handler),
#else
    Case("Dummy case", test_dummy, greentea_failure_handler)
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(25, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
#if TEST
    cs = 1;
    cb.attach(callbackFunction);
    init_timer();
#endif
    Harness::run(specification);
}