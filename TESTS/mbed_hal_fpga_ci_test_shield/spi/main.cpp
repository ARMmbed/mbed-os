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
#include "SPIMasterTester.h"
#include "pinmap.h"
#include "hal/static_pinmap.h"
#include "test_utils.h"
#include "spi_fpga_test.h"

using namespace utest::v1;

typedef enum {
    TRANSFER_SPI_MASTER_WRITE_SYNC,
    TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC,
    TRANSFER_SPI_MASTER_TRANSFER_ASYNC
} transfer_type_t;

typedef enum {
    BUFFERS_COMMON,     // common case rx/tx buffers are defined and have the same size
    BUFFERS_TX_GT_RX,   // tx buffer length is greater than rx buffer length
    BUFFERS_TX_LT_RX,   // tx buffer length is less than rx buffer length
    BUFFERS_TX_ONE_SYM, // one symbol only is transmitted in both directions
} test_buffers_t;

#define FREQ_200_KHZ  (200000ull)
#define FREQ_500_KHZ  (500000)
#define FREQ_1_MHZ    (1000000)
#define FREQ_2_MHZ    (2000000)
#define FREQ_10_MHZ   (10000000ull)
#define FREQ_MIN      ((uint32_t)0)
#define FREQ_MAX      ((uint32_t)-1)
#define FILL_SYM      (0xF5F5F5F5)
#define DUMMY_SYM     (0xD5D5D5D5)

#define SS_ASSERT     (0)
#define SS_DEASSERT   (!(SS_ASSERT))

#define TEST_CAPABILITY_BIT(MASK, CAP) ((1 << CAP) & (MASK))

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

/* Function finds SS pin for manual SS handling. */
static PinName find_ss_pin(PinName mosi, PinName miso, PinName sclk)
{
    const PinList *ff_pins_list = pinmap_ff_default_pins();
    const PinList *restricted_pins_list = pinmap_restricted_pins();
    uint32_t cs_pin_idx;

    for (cs_pin_idx = 0; cs_pin_idx < ff_pins_list->count; cs_pin_idx++) {
        if (ff_pins_list->pins[cs_pin_idx] == mosi ||
                ff_pins_list->pins[cs_pin_idx] == miso ||
                ff_pins_list->pins[cs_pin_idx] == sclk) {
            continue;
        }

        bool restricted_pin = false;
        for (uint32_t i = 0; i < restricted_pins_list->count ; i++) {
            if (ff_pins_list->pins[cs_pin_idx] == restricted_pins_list->pins[i]) {
                restricted_pin = true;
            }
        }

        if (restricted_pin) {
            continue;
        } else {
            break;
        }
    }

    PinName ssel = (cs_pin_idx == ff_pins_list->count ? NC : ff_pins_list->pins[cs_pin_idx]);

    TEST_ASSERT_MESSAGE(ssel != NC, "Unable to find pin for Chip Select");

    return ssel;
}

/* Function handles ss line if ss is specified. */
static void handle_ss(DigitalOut *ss, bool select)
{
    if (ss) {
        if (select) {
            *ss = SS_ASSERT;
        } else {
            *ss = SS_DEASSERT;
        }
    }
}

/* Auxiliary function to check platform capabilities against test case. */
static bool check_capabilities(const spi_capabilities_t *capabilities, SPITester::SpiMode spi_mode, uint32_t sym_size, transfer_type_t transfer_type, uint32_t frequency, test_buffers_t test_buffers)
{
    // Symbol size
    if (!TEST_CAPABILITY_BIT(capabilities->word_length, (sym_size - 1))) {
        utest_printf("\n<Specified symbol size is not supported on this platform> skipped. ");
        return false;
    }

    // SPI clock mode
    if (!TEST_CAPABILITY_BIT(capabilities->clk_modes, spi_mode)) {
        utest_printf("\n<Specified spi clock mode is not supported on this platform> skipped. ");
        return false;
    }

    // Frequency
    if (frequency != FREQ_MAX && frequency != FREQ_MIN && frequency < capabilities->minimum_frequency && frequency > capabilities->maximum_frequency) {
        utest_printf("\n<Specified frequency is not supported on this platform> skipped. ");
        return false;
    }

    // Async mode
    if (transfer_type == TRANSFER_SPI_MASTER_TRANSFER_ASYNC && capabilities->async_mode == false) {
        utest_printf("\n<Async mode is not supported on this platform> skipped. ");
        return false;
    }

    if ((test_buffers == BUFFERS_TX_GT_RX || test_buffers == BUFFERS_TX_LT_RX) && capabilities->tx_rx_buffers_equal_length == true) {
        utest_printf("\n<RX length != TX length is not supported on this platform> skipped. ");
        return false;
    }

    return true;
}

void fpga_spi_test_init_free(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    spi_init(&spi, mosi, miso, sclk, ssel);
    spi_format(&spi, 8, 0, 0);
    spi_frequency(&spi, 1000000);
    spi_free(&spi);
}

void fpga_spi_test_init_free_cs_nc(PinName mosi, PinName miso, PinName sclk)
{
    spi_init(&spi, mosi, miso, sclk, NC);
    spi_format(&spi, 8, 0, 0);
    spi_frequency(&spi, 1000000);
    spi_free(&spi);
}

void fpga_spi_test_init_free_cs_nc_miso_nc_mosi_nc(PinName mosi, PinName miso, PinName sclk)
{
    utest_printf("\nTesting: MOSI = NC. ");

    spi_init(&spi, NC, miso, sclk, NC);
    spi_format(&spi, 8, 0, 0);
    spi_frequency(&spi, 1000000);
    spi_free(&spi);

    utest_printf("Testing: MISO = NC. ");

    spi_init(&spi, mosi, NC, sclk, NC);
    spi_format(&spi, 8, 0, 0);
    spi_frequency(&spi, 1000000);
    spi_free(&spi);
}


void fpga_spi_test_common(PinName mosi, PinName miso, PinName sclk, PinName ssel, SPITester::SpiMode spi_mode, uint32_t sym_size, transfer_type_t transfer_type, uint32_t frequency, test_buffers_t test_buffers, bool auto_ss, bool init_direct)
{
    spi_capabilities_t capabilities;
    uint32_t freq = frequency;
    uint32_t tx_cnt = TRANSFER_COUNT;
    uint32_t rx_cnt = TRANSFER_COUNT;
    uint8_t fill_symbol = (uint8_t)FILL_SYM;
    PinName ss_pin = (auto_ss ? ssel : NC);
    DigitalOut *ss = NULL;

    spi_get_capabilities(ssel, false, &capabilities);

    if (check_capabilities(&capabilities, spi_mode, sym_size, transfer_type, frequency, test_buffers) == false) {
        return;
    }

    uint32_t sym_mask = ((1 << sym_size) - 1);

    switch (frequency) {
        case (FREQ_MIN):
            freq = capabilities.minimum_frequency;
            break;
        case (FREQ_MAX):
            freq = capabilities.maximum_frequency;
            break;
        default:
            break;
    }

    switch (test_buffers) {
        case (BUFFERS_COMMON):
            // nothing to change
            break;
        case (BUFFERS_TX_GT_RX):
            rx_cnt /= 2;
            break;
        case (BUFFERS_TX_LT_RX):
            tx_cnt /= 2;
            break;
        case (BUFFERS_TX_ONE_SYM):
            tx_cnt = 1;
            rx_cnt = 1;
            break;

        default:
            break;
    }

    // Remap pins for test
    tester.reset();
    tester.pin_map_set(mosi, MbedTester::LogicalPinSPIMosi);
    tester.pin_map_set(miso, MbedTester::LogicalPinSPIMiso);
    tester.pin_map_set(sclk, MbedTester::LogicalPinSPISclk);
    tester.pin_map_set(ssel, MbedTester::LogicalPinSPISsel);

    // Manually handle SS pin
    if (!auto_ss) {
        ss = new DigitalOut(ssel, SS_DEASSERT);
    }

    if (init_direct) {
        const spi_pinmap_t pinmap = get_spi_pinmap(mosi, miso, sclk, ss_pin);
        spi_init_direct(&spi, &pinmap);
    } else {
        spi_init(&spi, mosi, miso, sclk, ss_pin);
    }

    spi_format(&spi, sym_size, spi_mode, 0);
    spi_frequency(&spi, freq);

    // Configure spi_slave module
    tester.set_mode(spi_mode);
    tester.set_bit_order(SPITester::MSBFirst);
    tester.set_sym_size(sym_size);

    // Reset tester stats and select SPI
    tester.peripherals_reset();
    tester.select_peripheral(SPITester::PeripheralSPI);

    uint32_t checksum = 0;
    uint32_t sym_count = TRANSFER_COUNT;
    int result = 0;
    uint8_t tx_buf[TRANSFER_COUNT] = {0};
    uint8_t rx_buf[TRANSFER_COUNT] = {0};

    // Send and receive test data
    switch (transfer_type) {
        case TRANSFER_SPI_MASTER_WRITE_SYNC:
            handle_ss(ss, true);
            for (int i = 0; i < TRANSFER_COUNT; i++) {
                uint32_t data = spi_master_write(&spi, (0 - i) & sym_mask);
                TEST_ASSERT_EQUAL(i & sym_mask, data);
                checksum += (0 - i) & sym_mask;
            }
            handle_ss(ss, false);
            break;

        case TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC:
            for (int i = 0; i < TRANSFER_COUNT; i++) {
                tx_buf[i] = (0 - i) & sym_mask;
                rx_buf[i] = 0xFF;

                switch (test_buffers) {
                    case (BUFFERS_COMMON):
                    case (BUFFERS_TX_GT_RX):
                        checksum += ((0 - i) & sym_mask);
                        break;
                    case (BUFFERS_TX_LT_RX):
                        if (i < tx_cnt) {
                            checksum += ((0 - i) & sym_mask);
                        } else {
                            checksum += (fill_symbol & sym_mask);
                        }
                        break;
                    case (BUFFERS_TX_ONE_SYM):
                        tx_buf[0] = 0xAA;
                        checksum = 0xAA;
                        sym_count = 1;
                        break;
                    default:
                        break;
                }
            }

            handle_ss(ss, true);
            result = spi_master_block_write(&spi, (const char *)tx_buf, tx_cnt, (char *)rx_buf, rx_cnt, 0xF5);
            handle_ss(ss, false);

            for (int i = 0; i < rx_cnt; i++) {
                TEST_ASSERT_EQUAL(i & sym_mask, rx_buf[i]);
            }

            for (int i = rx_cnt; i < TRANSFER_COUNT; i++) {
                TEST_ASSERT_EQUAL(0xFF, rx_buf[i]);
            }

            TEST_ASSERT_EQUAL(sym_count, result);
            break;

#if DEVICE_SPI_ASYNCH
        case TRANSFER_SPI_MASTER_TRANSFER_ASYNC:
            for (int i = 0; i < TRANSFER_COUNT; i++) {
                tx_buf[i] = (0 - i) & sym_mask;
                checksum += (0 - i) & sym_mask;
                rx_buf[i] = 0xAA;
            }

            async_trasfer_done = false;

            handle_ss(ss, true);
            spi_master_transfer(&spi, tx_buf, TRANSFER_COUNT, rx_buf, TRANSFER_COUNT, 8, (uint32_t)spi_async_handler, SPI_EVENT_COMPLETE, DMA_USAGE_NEVER);

            while (!async_trasfer_done);
            handle_ss(ss, false);

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
    TEST_ASSERT_EQUAL(sym_count, tester.get_transfer_count());
    TEST_ASSERT_EQUAL(checksum, tester.get_receive_checksum());

    spi_free(&spi);
    tester.reset();
}

template<SPITester::SpiMode spi_mode, uint32_t sym_size, transfer_type_t transfer_type, uint32_t frequency, test_buffers_t test_buffers, bool auto_ss, bool init_direct>
void fpga_spi_test_common(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    fpga_spi_test_common(mosi, miso, sclk, ssel, spi_mode, sym_size, transfer_type, frequency, test_buffers, auto_ss, init_direct);
}

template<SPITester::SpiMode spi_mode, uint32_t sym_size, transfer_type_t transfer_type, uint32_t frequency, test_buffers_t test_buffers, bool auto_ss, bool init_direct>
void fpga_spi_test_common_no_ss(PinName mosi, PinName miso, PinName sclk)
{
    PinName ssel = find_ss_pin(mosi, miso, sclk);

    fpga_spi_test_common(mosi, miso, sclk, ssel, spi_mode, sym_size, transfer_type, frequency, test_buffers, auto_ss, init_direct);
}

Case cases[] = {
    // This will be run for all pins
    Case("SPI - init/free test all pins", all_ports<SPIPort, DefaultFormFactor, fpga_spi_test_init_free>),
    Case("SPI - init/free test all pins (CS == NC)", all_ports<SPINoCSPort, DefaultFormFactor, fpga_spi_test_init_free_cs_nc>),
    Case("SPI - init/free test all pins (CS == NC, MISO/MOSI == NC)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_init_free_cs_nc_miso_nc_mosi_nc>),

    // This will be run for all peripherals
    Case("SPI - basic test", all_peripherals<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - basic test (direct init)", all_peripherals<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, true> >),

    // This will be run for single pin configuration
    Case("SPI - mode testing (MODE_1)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode1, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - mode testing (MODE_2)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode2, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - mode testing (MODE_3)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode3, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - symbol size testing (4)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 4, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - symbol size testing (12)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 12, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - symbol size testing (16)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 16, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - symbol size testing (24)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 24, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - symbol size testing (32)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 32, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - buffers tx > rx", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_TX_GT_RX, false, false> >),
    Case("SPI - buffers tx < rx", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_TX_LT_RX, false, false> >),
    Case("SPI - frequency testing (200 kHz)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_200_KHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - frequency testing (2 MHz)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_2_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - frequency testing (capabilities min)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_MIN, BUFFERS_COMMON, false, false> >),
    Case("SPI - frequency testing (capabilities max)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_MAX, BUFFERS_COMMON, false, false> >),
    Case("SPI - block write", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - block write(one sym)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_TX_ONE_SYM, false, false> >),
    Case("SPI - hardware ss handling", one_peripheral<SPIPort, DefaultFormFactor, fpga_spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, true, false> >),
    Case("SPI - hardware ss handling(block)", one_peripheral<SPIPort, DefaultFormFactor, fpga_spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_BLOCK_WRITE_SYNC, FREQ_1_MHZ, BUFFERS_COMMON, true, false> >),
#if DEVICE_SPI_ASYNCH
    Case("SPI - async mode (sw ss)", one_peripheral<SPINoCSPort, DefaultFormFactor, fpga_spi_test_common_no_ss<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_TRANSFER_ASYNC, FREQ_1_MHZ, BUFFERS_COMMON, false, false> >),
    Case("SPI - async mode (hw ss)", one_peripheral<SPIPort, DefaultFormFactor, fpga_spi_test_common<SPITester::Mode0, 8, TRANSFER_SPI_MASTER_TRANSFER_ASYNC, FREQ_1_MHZ, BUFFERS_COMMON, true, false> >)
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
