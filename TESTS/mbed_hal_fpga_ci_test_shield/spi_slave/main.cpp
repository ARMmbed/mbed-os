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

#include "SPISlaveTester.h"
#include "pinmap.h"
#include "test_utils.h"
#include "fpga_config.h"
#include "fpga_spi_slave_test.h"

/* Half-duplex mode (3 wires mode) requires additional configuration. FPGA test shield uses
   Mosi pin as data line in Half duplex mode. Select pin which Mbed board uses for data line
   and uncomment half-duplex test case. */
#define MOSI 0
#define MISO 1
#define HD_DATA_LINE MISO

#define FREQ_200KHZ                 (200000ull)
#define FREQ_1MHZ                   (1000000ull)
#define FREQ_2MHZ                   (2000000ull)
#define FREQ_10MHZ                  (10000000ull)
#define FREQ_MIN                    ((uint32_t)0)
#define FREQ_MAX                    ((uint32_t)-1)

#define FILL_SYM                    (0xF5F5F5F5)
#define DUMMY_SYM                   (0xD5D5D5D5)

#define FPGA_FREQ                   (100000000ull) // 100 MHz
#define TRANSFER_START_DELAY_US     (200)
#define SPI_TRANSFERS               (5)

#define SYNC true
#define ASYNC false

#define TEST_CAPABILITY_BIT(MASK, CAP) ((1 << CAP) & (MASK))

const int TRANSFER_COUNT = 300;

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();
SPISlaveTester tester(form_factor, restricted);

/* Auxiliary function to check platform capabilities against test case. */
static bool check_capabilities(const spi_capabilities_t *capabilities, const test_config_t *tc_config)
{
    // Symbol size
    if (!TEST_CAPABILITY_BIT(capabilities->word_length, (tc_config->sym_size - 1))) {
        utest_printf("\n<Specified symbol size is not supported on this platform> skipped ");
        return false;
    }

    // SPI clock mode
    if (!TEST_CAPABILITY_BIT(capabilities->clk_modes, tc_config->spi_mode)) {
        utest_printf("\n<Specified spi clock mode is not supported on this platform> skipped ");
        return false;
    }

    // Bit order
    if (!TEST_CAPABILITY_BIT(capabilities->bit_order, tc_config->spi_bit_order)) {
        utest_printf("\n<Specified bit order is not supported on this platform> skipped ");
        return false;
    }

    // Frequency
    if (tc_config->spi_freq != FREQ_MAX && tc_config->spi_freq != FREQ_MIN && tc_config->spi_freq < capabilities->minimum_frequency && tc_config->spi_freq > capabilities->maximum_frequency) {
        utest_printf("\n<Specified frequency is not supported on this platform> skipped ");
        return false;
    }

    // Slave mode
    if (capabilities->support_slave_mode == false) {
        utest_printf("\n<Slave mode is not supported on this platform> skipped ");
        return false;
    }

    // Half duplex
    if (tc_config->sync_mode == false && capabilities->async_mode == false) {
        utest_printf("\n<Async mode is not supported on this platform> skipped ");
        return false;
    }

    // Async mode
    if (tc_config->duplex == SPITester::HalfDuplex && capabilities->half_duplex == false) {
        utest_printf("\n<Half Duplex mode is not supported on this platform> skipped ");
        return false;
    }

    return true;
}

#ifdef DEVICE_SPI_ASYNCH
/* Callback function for SPI async transfers. */
static uint32_t context;
static volatile bool async_transm_done;
static volatile uint32_t async_transfered;
static void spi_async_callback(spi_t *obj, void *ctx, spi_async_event_t *event)
{
    async_transm_done = true;
    async_transfered = event->transfered;

    TEST_ASSERT_EQUAL((void *)&context, ctx);

    // We don't expect any failure
    TEST_ASSERT_EQUAL(false, event->error);
}
#endif

/** Test that the spi-Slave can be initialized/de-initialized using all possible
 *  SPI pins.
 */
void fpga_spi_slave_test_init_free(PinName mosi, PinName miso, PinName sclk, PinName cs)
{
    spi_t spi;

    spi_init(&spi, true, mosi, miso, sclk, cs);
    spi_format(&spi, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_free(&spi);

    // try to re-init spi on the specified pins
    spi_init(&spi, true, mosi, miso, sclk, cs);
    spi_format(&spi, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_free(&spi);
}

/** Test that the SPI-Slave transfer can be performed in various configurations.
 */
template<typename T>
void fpga_spi_slave_test_common(const test_config_t *tc_config)
{
    spi_t spi;
    spi_capabilities_t capabilities;
    const uint32_t sym_mask = ((1 << tc_config->sym_size) - 1);
    uint32_t freq = tc_config->spi_freq;
    uint32_t count = 0;
    T tx_buf[TRANSFER_COUNT] = {0};
    T rx_buf[TRANSFER_COUNT] = {0};
    T fill_symbol = (T)FILL_SYM;
    void *p_tx = tx_buf;
    void *p_rx = rx_buf;
    uint32_t tx_cnt = TRANSFER_COUNT;
    uint32_t rx_cnt = TRANSFER_COUNT;
    uint32_t expected_sym_count = TRANSFER_COUNT;
    uint32_t expected_checksum = 0;
    uint16_t num_of_symbols = TRANSFER_COUNT;

    spi_get_capabilities(spi_get_module(tc_config->mosi, tc_config->miso, tc_config->sclk), tc_config->ssel, true, &capabilities);

    if (check_capabilities(&capabilities, tc_config) == false) {
        return;
    }

    switch (tc_config->test_buffers) {
        case (BUFFERS_COMMON):
            // nothing to change
            break;
        case (BUFFERS_TX_UNDEF):
            p_tx = NULL;
            break;
        case (BUFFERS_RX_UNDEF):
            p_rx = NULL;
            break;
        case (BUFFERS_TX_GT_RX):
            rx_cnt /= 2;
            break;
        case (BUFFERS_TX_LT_RX):
            tx_cnt /= 2;
            break;
        case (BUFFERS_ONE_SYM):
            tx_cnt = 1;
            rx_cnt = 1;
            num_of_symbols = 1;
            break;

        default:
            break;
    }

    // Remap pins for test and init spi interface
    tester.pin_map_reset();
    if (tc_config->duplex == SPITester::FullDuplex) {
        tester.pin_map_set(tc_config->mosi, MbedTester::LogicalPinSPIMosi);
        tester.pin_map_set(tc_config->miso, MbedTester::LogicalPinSPIMiso);
        tester.pin_map_set(tc_config->sclk, MbedTester::LogicalPinSPISclk);
        tester.pin_map_set(tc_config->ssel, MbedTester::LogicalPinSPISsel);
        spi_init(&spi, true, tc_config->mosi, tc_config->miso, tc_config->sclk, tc_config->ssel);
    } else {
        const PinName hd_data = HD_DATA_LINE == MOSI ? tc_config->mosi : tc_config->miso;
        tester.pin_map_set(hd_data, MbedTester::LogicalPinSPIMosi);
        tester.pin_map_set(tc_config->sclk, MbedTester::LogicalPinSPISclk);
        tester.pin_map_set(tc_config->ssel, MbedTester::LogicalPinSPISsel);

        if (HD_DATA_LINE == MOSI) {
            spi_init(&spi, true, tc_config->mosi, NC, tc_config->sclk, tc_config->ssel);
        } else {
            spi_init(&spi, true, NC, tc_config->miso, tc_config->sclk, tc_config->ssel);
        }
    }

    switch (tc_config->spi_freq) {
        case (FREQ_MIN):
            freq = capabilities.minimum_frequency;
            break;
        case (FREQ_MAX):
            freq = capabilities.maximum_frequency;
            break;
        default:
            break;
    }

    // Initialize mbed SPI transmission format
    spi_format(&spi, tc_config->sym_size, (spi_mode_t)tc_config->spi_mode, (spi_bit_ordering_t)tc_config->spi_bit_order);

    // Perform SPI transfers and verification
    for (uint32_t transfers = 0; transfers < SPI_TRANSFERS; transfers++) {
        expected_checksum = 0;

        // Configure FPGA SPI Slave tester module
        tester.set_mode(tc_config->spi_mode);
        tester.set_bit_order(tc_config->spi_bit_order);
        tester.set_sym_size(tc_config->sym_size);
        tester.set_duplex_mode(tc_config->duplex);
        tester.set_spi_master_freq(FPGA_FREQ / freq);
        tester.set_num_of_symbols(num_of_symbols);
        tester.set_start_delay_us(TRANSFER_START_DELAY_US);
        tester.set_sym_delay_ns(capabilities.slave_delay_between_symbols_ns);

        if (tc_config->duplex == SPITester::HalfDuplex) {
            tester.set_hd_tx_rx_cnt(num_of_symbols / 2, num_of_symbols / 2);
            expected_sym_count = TRANSFER_COUNT;
            tx_cnt = num_of_symbols / 2;
            rx_cnt = num_of_symbols / 2;
        }

        // Reset tester stats and select SPI
        tester.peripherals_reset();
        tester.select_peripheral(SPITester::PeripheralSPISlave);

        // Init rx/tx buffers
        for (uint32_t i = 0; i < TRANSFER_COUNT; i++) {
            tx_buf[i] = ((0 - i) & sym_mask);
            rx_buf[i] = (T)DUMMY_SYM;

            switch (tc_config->test_buffers) {
                case (BUFFERS_COMMON):
                case (BUFFERS_RX_UNDEF):
                case (BUFFERS_TX_GT_RX):
                    if (tc_config->duplex != SPITester::HalfDuplex || i < (num_of_symbols / 2)) {
                        expected_checksum += ((0 - i) & sym_mask);
                    }
                    break;
                case (BUFFERS_TX_UNDEF):
                    expected_checksum += (fill_symbol & sym_mask);
                    break;
                case (BUFFERS_TX_LT_RX):
                    if (i < tx_cnt) {
                        expected_checksum += ((0 - i) & sym_mask);
                    } else {
                        expected_checksum += (fill_symbol & sym_mask);
                    }
                    break;
                case (BUFFERS_ONE_SYM):
                    tx_buf[0] = (0x12345678 & sym_mask);
                    expected_checksum = (0x12345678 & sym_mask);
                    expected_sym_count = 1;
                    break;
                default:
                    break;
            }
        }

        // Send start request signal to FPGA SPI Master
        tester.start_transfer();

        // Perfrom sync/async spi transfer
        if (tc_config->sync_mode) {
            core_util_critical_section_enter();
            count = spi_transfer(&spi, p_tx, tx_cnt, p_rx, rx_cnt, &fill_symbol);
            core_util_critical_section_exit();
        } else {
#ifdef DEVICE_SPI_ASYNCH
            async_transm_done = false;
            async_transfered = 0;
            uint32_t loop_cnt = 0;;

            bool result = spi_transfer_async(&spi, p_tx, tx_cnt, p_rx, rx_cnt, &fill_symbol, spi_async_callback, &context, DMA_USAGE_NEVER);

            TEST_ASSERT_EQUAL(true, result);

            // Wait here for the end of the transmission.
            while (!async_transm_done) {
                loop_cnt++;
            }

            // Verify async transfer result
            TEST_ASSERT_EQUAL(true, result);

            count = async_transfered;
#endif
        }

        // RX buffer validation
        for (uint32_t i = 0; i < rx_cnt && p_rx; i++) {
            TEST_ASSERT_EQUAL(i & sym_mask, rx_buf[i]);
        }

        // Additional check for RX < TX case: check if remaining bytes in RX buffer are not changed
        for (int i = rx_cnt; i < TRANSFER_COUNT && p_rx; i++) {
            TEST_ASSERT_EQUAL((T)DUMMY_SYM, rx_buf[i]);
        }

        TEST_ASSERT_EQUAL(expected_sym_count, tester.get_transfer_count());
        TEST_ASSERT_EQUAL(expected_sym_count, count);
        TEST_ASSERT_EQUAL(expected_checksum, tester.get_receive_checksum());
    }
    spi_free(&spi);
    tester.pin_map_reset();
}

template<SPITester::SpiMode spi_mode, SPITester::SpiBitOrder spi_bit_order, uint32_t sym_size, uint32_t spi_freq, test_buffers_t test_buffers, bool sync_mode, SPITester::SpiDuplex duplex>
void fpga_spi_slave_test_common(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    const test_config_t tc_config = {mosi, miso, sclk, ssel, spi_mode, spi_bit_order, sym_size, spi_freq, test_buffers, sync_mode, duplex};

    if (sym_size <= 8) {
        fpga_spi_slave_test_common<uint8_t>(&tc_config);
    } else if (sym_size <= 16) {
        fpga_spi_slave_test_common<uint16_t>(&tc_config);
    } else {
        fpga_spi_slave_test_common<uint32_t>(&tc_config);
    }
}

Case cases[] = {
    // This will be run for all ports

    Case("SPI Slave - init/free test all pins", all_ports<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_init_free>),

    // This will be run for all peripherals
    Case("SPI Slave/Sync - basic test", all_peripherals<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - basic test", all_peripherals<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),

    // This will be run for single peripheral
    Case("SPI Slave/Sync - mode testing (MODE_1)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode1, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - mode testing (MODE_2)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode2, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - mode testing (MODE_3)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode3, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - bit order testing (LSBFirst)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::LSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - freqency testing (FREQ_200KHZ)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_200KHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - freqency testing (FREQ_2MHZ)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_2MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - freqency testing (FREQ_MIN)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MIN, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - freqency testing (FREQ_MAX)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MAX, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - symbol size testing (4)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 4, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - symbol size testing (12)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 12, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - symbol size testing (16)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 16, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - symbol size testing (20)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 20, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - symbol size testing (24)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 24, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - symbol size testing (28)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 28, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - symbol size testing (32)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 32, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - buffers testing (BUFFERS_TX_UNDEF)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_TX_UNDEF, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - buffers testing (BUFFERS_RX_UNDEF)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_RX_UNDEF, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - buffers testing (BUFFERS_TX_GT_RX)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_TX_GT_RX, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - buffers testing (BUFFERS_TX_LT_RX)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_TX_LT_RX, SYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Sync - buffers testing (BUFFERS_ONE_SYM)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_ONE_SYM, SYNC, SPITester::FullDuplex> >),
    //Case("SPI Slave/Sync - Half Duplex", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, SYNC, SPITester::HalfDuplex> >),

    Case("SPI Slave/Async - mode testing (MODE_1)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode1, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - mode testing (MODE_2)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode2, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - mode testing (MODE_3)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode3, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - bit order testing (LSBFirst)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::LSBFirst, 8, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - freqency testing (FREQ_200KHZ)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_200KHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - freqency testing (FREQ_2MHZ)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_2MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - freqency testing (FREQ_MIN)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MIN, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - freqency testing (FREQ_MAX)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MAX, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - symbol size testing (4)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 4, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - symbol size testing (12)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 12, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - symbol size testing (16)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 16, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - symbol size testing (20)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 20, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - symbol size testing (24)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 24, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - symbol size testing (28)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 28, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - symbol size testing (32)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 32, FREQ_1MHZ, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - buffers testing (BUFFERS_TX_UNDEF)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_TX_UNDEF, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - buffers testing (BUFFERS_RX_UNDEF)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_RX_UNDEF, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Slave/Async - buffers testing (BUFFERS_ONE_SYM)", one_peripheral<SPISlavePort, DefaultFormFactor, fpga_spi_slave_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, BUFFERS_ONE_SYM, ASYNC, SPITester::FullDuplex> >),
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