/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !DEVICE_SPI
#error [NOT_SUPPORTED] SPI not supported for this target
#elif !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test
#elif !defined(TARGET_FF_ARDUINO) && !defined(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
#error [NOT_SUPPORTED] Test not supported for this form factor
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"

using namespace utest::v1;

#include "SPIMasterTester.h"
#include "pinmap.h"
#include "test_utils.h"

typedef enum {
    TRANSFER_SPI_MASTER_WRITE_SYNC,
    TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC,
    TRANSFER_SPI_MASTER_TRANSFER_ASYNC
} transfer_type_t;

#define FREQ_500_KHZ 500000
#define FREQ_1_MHZ 1000000
#define FREQ_2_MHZ 2000000

const int TRANSFER_COUNT = 300;
SPIMasterTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());

spi_t spi;
static volatile bool async_trasfer_done;

#if DEVICE_SPI_ASYNCH
void spi_async_handler()
{
    int event = spi_irq_handler_asynch(&spi);

    if (event & SPI_EVENT_COMPLETE) {
        async_trasfer_done = true;
    }
}
#endif

void spi_test_init_free(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    spi_init(&spi, mosi, miso, sclk, ssel);
    spi_format(&spi, 8, SPITester::Mode0, 0);
    spi_frequency(&spi, 1000000);
    spi_free(&spi);
}

void spi_test_common(PinName mosi, PinName miso, PinName sclk, PinName ssel, SPITester::SpiMode spi_mode, uint32_t sym_size, transfer_type_t transfer_type, uint32_t frequency)
{
    uint32_t sym_mask = ((1 << sym_size) - 1);

    // Remap pins for test
    tester.reset();
    tester.pin_map_set(mosi, MbedTester::LogicalPinSPIMosi);
    tester.pin_map_set(miso, MbedTester::LogicalPinSPIMiso);
    tester.pin_map_set(sclk, MbedTester::LogicalPinSPISclk);
    tester.pin_map_set(ssel, MbedTester::LogicalPinSPISsel);

    // Initialize mbed SPI pins

    spi_init(&spi, mosi, miso, sclk, ssel);
    spi_format(&spi, sym_size, spi_mode, 0);
    spi_frequency(&spi, frequency);

    // Configure spi_slave module
    tester.set_mode(spi_mode);
    tester.set_bit_order(SPITester::MSBFirst);
    tester.set_sym_size(sym_size);

    // Reset tester stats and select SPI
    tester.peripherals_reset();
    tester.select_peripheral(SPITester::PeripheralSPI);

    uint32_t checksum = 0;
    int result = 0;
    uint8_t tx_buf[TRANSFER_COUNT] = {0};
    uint8_t rx_buf[TRANSFER_COUNT] = {0};

    // Send and receive test data
    switch (transfer_type) {
        case TRANSFER_SPI_MASTER_WRITE_SYNC:
            for (int i = 0; i < TRANSFER_COUNT; i++) {
                uint32_t data = spi_master_write(&spi, (0 - i) & sym_mask);
                TEST_ASSERT_EQUAL(i & sym_mask, data);

                checksum += (0 - i) & sym_mask;
            }
            break;

        case TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC:
            for (int i = 0; i < TRANSFER_COUNT; i++) {
                tx_buf[i] = (0 - i) & sym_mask;
                checksum += (0 - i) & sym_mask;
                rx_buf[i] = 0xAA;
            }

            result = spi_master_block_write(&spi, (const char *)tx_buf, TRANSFER_COUNT, (char *)rx_buf, TRANSFER_COUNT, 0xF5);

            for (int i = 0; i < TRANSFER_COUNT; i++) {
                TEST_ASSERT_EQUAL(i & sym_mask, rx_buf[i]);
            }
            TEST_ASSERT_EQUAL(TRANSFER_COUNT, result);
            break;

#if DEVICE_SPI_ASYNCH
        case TRANSFER_SPI_MASTER_TRANSFER_ASYNC:
            for (int i = 0; i < TRANSFER_COUNT; i++) {
                tx_buf[i] = (0 - i) & sym_mask;
                checksum += (0 - i) & sym_mask;
                rx_buf[i] = 0xAA;
            }

            async_trasfer_done = false;

            spi_master_transfer(&spi, tx_buf, TRANSFER_COUNT, rx_buf, TRANSFER_COUNT, 8, (uint32_t)spi_async_handler, SPI_EVENT_COMPLETE, DMA_USAGE_NEVER);
            while (!async_trasfer_done);

            for (int i = 0; i < TRANSFER_COUNT; i++) {
                TEST_ASSERT_EQUAL(i & sym_mask, rx_buf[i]);
            }

            break;
#endif

        default:
            TEST_ASSERT_MESSAGE(0, "Unsupported transfer type.");
            break;

    }

    // Verify that the transfer was successful
    TEST_ASSERT_EQUAL(TRANSFER_COUNT, tester.get_transfer_count());
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());

    spi_free(&spi);
    tester.reset();
}

template<SPITester::SpiMode spi_mode, uint32_t sym_size, transfer_type_t transfer_type, uint32_t frequency>
void spi_test_common(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    spi_test_common(mosi, miso, sclk, ssel, spi_mode, sym_size, transfer_type, frequency);
}

Case cases[] = {
    // This will be run for all pins
    Case("SPI - init/free test all pins", all_ports<SPIPort, DefaultFormFactor, spi_test_init_free>),

    // This will be run for all peripherals
    Case("SPI - basic test", all_peripherals<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ> >),

    // This will be run for single pin configuration
    Case("SPI - mode testing (MODE_1)", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode1, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ> >),
    Case("SPI - mode testing (MODE_2)", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode2, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ> >),
    Case("SPI - mode testing (MODE_3)", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode3, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ> >),

    Case("SPI - symbol size testing (16)", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, 16, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ> >),

    Case("SPI - frequency testing (500 kHz)", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_500_KHZ> >),
    Case("SPI - frequency testing (2 MHz)", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_2_MHZ> >),

    Case("SPI - block write", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC, FREQ_1_MHZ> >),

#if DEVICE_SPI_ASYNCH
    Case("SPI - async mode", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_TRANSFER_ASYNC, FREQ_1_MHZ> >)
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif /* !DEVICE_SPI */
