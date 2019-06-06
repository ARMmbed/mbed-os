
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

/* The test is designed to verify new SPI HAL API. Please enable the test manually. */
#define TEST_ENABLED 0

#if !DEVICE_SPI
#error [NOT_SUPPORTED] SPI not supported for this target
#elif !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"

using namespace utest::v1;

#include "SPIMasterTester.h"
#include "pinmap.h"
#include "test_utils.h"
#include "fpga_config.h"

/* Half-duplex mode (3 wires mode) requires additional configuration. FPGA test shield uses
   Mosi pin as data line in Half duplex mode. Select pin which Mbed board uses for data line
   and uncomment half-duplex test case. */
#define MOSI 0
#define MISO 1
#define HD_DATA_LINE MOSI

#define FREQ_200KHZ   (200000ull)
#define FREQ_1MHZ     (1000000ull)
#define FREQ_2MHZ     (2000000ull)
#define FREQ_10MHZ    (10000000ull)
#define FREQ_MIN      ((uint32_t)0)
#define FREQ_MAX      ((uint32_t)-1)

#define SS_ASSERT     (0)
#define SS_DEASSERT   (!(SS_ASSERT))

#define FILL_SYM      (0xF5F5F5F5)
#define DUMMY_SYM     (0xD5D5D5D5)

#define SYNC true
#define ASYNC false

#define HW_SS true
#define SW_SS false

#define PERIOD_NS(freq) ((uint32_t)(1000000000ull/(freq)))
#define METRICS_FREQ(HIGH_COUNT, LOW_COUNT)  ((uint32_t)(( (1.0f / (((HIGH_COUNT) + (LOW_COUNT)) / 100.0f)) * 1000000)))
#define MIN_PULSE_US(freq)  ((uint32_t)(1.0f / (freq) * 1000000 / 2))
#define METRICS_TICKS_TO_US(ticks)  ((uint32_t)(ticks) * 100)
#define DELTA_FACTOR 10 // 10%

#define SPI_TRANSFERS               (5)

#define TEST_CAPABILITY_BIT(MASK, CAP) ((1 << CAP) & (MASK))

#define SCLK_START_POS 0
#define SCLK_STOP_POS 2
#define SCLK_SIZE 1
#define MODE_SCLK_POLARITY_POS 1
#define MODE_SCLK_POLARITY_SIZE 1
#define TEST_SCLK(MODE, VAL) ((((VAL>>SCLK_START_POS) & SCLK_SIZE) == ((MODE >> MODE_SCLK_POLARITY_POS) & MODE_SCLK_POLARITY_SIZE)) && (((VAL>>SCLK_STOP_POS) & SCLK_SIZE) == ((MODE >> MODE_SCLK_POLARITY_POS) & MODE_SCLK_POLARITY_SIZE)))

typedef enum {
    BUFFERS_COMMON,     // common case rx/tx buffers are defined and have the same size
    BUFFERS_TX_UNDEF,   // tx buffer is undefined
    BUFFERS_RX_UNDEF,   // rx buffer is undefined
    BUFFERS_TX_GT_RX,   // tx buffer length is greater than rx buffer length
    BUFFERS_TX_LT_RX,   // tx buffer length is less than rx buffer length
    BUFFERS_TX_ONE_SYM, // one symbol only is transmitted in both directions
} test_buffers_t;

typedef struct {
    PinName mosi;
    PinName miso;
    PinName sclk;
    PinName ssel;
    SPITester::SpiMode spi_mode;
    SPITester::SpiBitOrder spi_bit_order;
    uint32_t sym_size;
    uint32_t spi_freq;
    bool auto_ss;
    test_buffers_t test_buffers;
    bool sync_mode;
    SPITester::SpiDuplex duplex;
} test_config_t;

const int TRANSFER_COUNT = 300;

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();
SPIMasterTester tester(form_factor, restricted);

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
        utest_printf("\n<Specified spi clock mode is not supported on this platform> skipped");
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

    // Async mode
    if (tc_config->auto_ss == true && capabilities->hw_cs_handle == false) {
        utest_printf("\n<Hardware CS handling is not supported on this platform> skipped ");
        return false;
    }

    return true;
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

    return (cs_pin_idx == ff_pins_list->count ? NC : ff_pins_list->pins[cs_pin_idx]);
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

/** Test that the spi-Master can be initialized/de-initialized using all possible
 *  SPI pins.
 *
 * Given board provides SPI-Master support.
 * When SPI-Master is initialized (and then de-initialized) using valid set of SPI pins.
 * Then the operation is successfull.
 *
 */
void spi_test_init_free(PinName mosi, PinName miso, PinName sclk)
{
    spi_t spi;
    spi_init(&spi, false, mosi, miso, sclk, NC);
    spi_format(&spi, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_frequency(&spi, FREQ_1MHZ);
    spi_free(&spi);

    // try to re-init spi on the specified pins
    spi_init(&spi, false, mosi, miso, sclk, NC);
    spi_free(&spi);
}

/** Test that the spi_get_capabilities() indicates that slave mode is unsupported if the given `ssel`
 * pin cannot be managed by hardware.
 *
 * Given board provides SPI support.
 * When `ssel` pin passed to spi_get_capabilities() cannot be managed by hardware.
 * Then capabilities should indicate that slave mode is unsupported.
 *
 */
void spi_test_capabilities(PinName mosi, PinName miso, PinName sclk)
{
    spi_capabilities_t capabilities;

    const PinList *ff_pins_list = pinmap_ff_default_pins();
    const PinMap *cs_pins = spi_master_cs_pinmap();
    PinName pin = NC;
    uint32_t cs_pin_idx;

    // Find pin which is not miso/mosi/sclk/cs handled by hardware
    for (uint32_t i = 0; i < ff_pins_list->count; i++) {
        if (ff_pins_list->pins[i] == mosi ||
                ff_pins_list->pins[i] == miso ||
                ff_pins_list->pins[i] == sclk) {
            continue;
        }

        cs_pin_idx = 0;

        while (cs_pins[cs_pin_idx].pin != NC) {
            if (cs_pins[cs_pin_idx].pin == ff_pins_list->pins[i]) {
                break;
            }
            cs_pin_idx++;
        }

        if (cs_pins[cs_pin_idx].pin != NC) {
            continue;
        } else {
            pin = ff_pins_list->pins[i];
            break;
        }
    }

    if (pin != NC) {
        capabilities.support_slave_mode = true;
        spi_get_capabilities(spi_get_module(mosi, miso, sclk), pin, false, &capabilities);
        TEST_ASSERT_EQUAL(false, capabilities.support_slave_mode);
    }
}

/** Test that the spi_frequency() returns actual SPI frequency.
 *
 * Given board provides SPI-Master support.
 * When specified frequency is selected by means of spi_frequency().
 * Then frequency returned by spi_frequency() is used for the transmission.
 *
 * Additionally we check that spi_format() updates the configuration of the peripheral (format)
 * except the baud rate generator.
 */
void spi_test_freq(PinName mosi, PinName miso, PinName sclk)
{
    spi_t spi;
    uint32_t count = 0;
    uint8_t rx_buf = (uint8_t)DUMMY_SYM;
    uint8_t tx_buf = (uint8_t)DUMMY_SYM;
    uint8_t fill_symbol = (uint8_t)FILL_SYM;
    uint32_t actual_freq;
    uint32_t freq_list[] = {FREQ_200KHZ, FREQ_1MHZ, FREQ_2MHZ, FREQ_10MHZ};
    const PinName ssel = find_ss_pin(mosi, miso, sclk);

    printf("CS=(%s) \n", pinmap_ff_default_pin_to_string(ssel));

    // Remap pins for test
    tester.pin_map_reset();
    tester.pin_map_set(mosi, MbedTester::LogicalPinSPIMosi);
    tester.pin_map_set(miso, MbedTester::LogicalPinSPIMiso);
    tester.pin_map_set(sclk, MbedTester::LogicalPinSPISclk);
    tester.pin_map_set(ssel, MbedTester::LogicalPinSPISsel);

    tester.pin_map_set(sclk, MbedTester::LogicalPinIOMetrics4);

    // Initialize mbed SPI pins
    spi_init(&spi, false, mosi, miso, sclk, NC);

    for (uint32_t i = 0; i < (sizeof(freq_list) / sizeof(freq_list[0])); i++) {
        // Initialize mbed SPI format
        spi_format(&spi, 8, SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_LSB_FIRST);

        actual_freq = spi_frequency(&spi, freq_list[i]);

        // Configure tester SPI format
        tester.set_mode(SPITester::Mode1);
        tester.set_bit_order(SPITester::LSBFirst);
        tester.set_sym_size(8);
        tester.set_duplex_mode(SPITester::FullDuplex);

        // Handle ss pin manually
        DigitalOut ss(ssel);
        ss = SS_DEASSERT;

        // Reset tester stats and select SPI
        tester.peripherals_reset();
        tester.select_peripheral(SPITester::PeripheralSPI);

        ss = SS_ASSERT;

        // Perform SPI transfer and trace clock line
        tester.io_metrics_start();
        count = spi_transfer(&spi, &tx_buf, 1, &rx_buf, 1, &fill_symbol);
        tester.io_metrics_stop();

        core_util_critical_section_exit();

        ss = SS_DEASSERT;

        const uint32_t min_pulse_low_ticks = tester.io_metrics_min_pulse_low(MbedTester::LogicalPinIOMetrics4);
        const uint32_t min_pulse_high_ticks = tester.io_metrics_min_pulse_high(MbedTester::LogicalPinIOMetrics4);

        TEST_ASSERT_EQUAL(1, tester.get_transfer_count());
        TEST_ASSERT_EQUAL(1, count);
        TEST_ASSERT_EQUAL(tx_buf, tester.get_receive_checksum());
        TEST_ASSERT_EQUAL(0, rx_buf);

        TEST_ASSERT_UINT32_WITHIN(actual_freq / DELTA_FACTOR, METRICS_FREQ(min_pulse_low_ticks, min_pulse_high_ticks), actual_freq);
        TEST_ASSERT(METRICS_TICKS_TO_US(min_pulse_low_ticks) >= MIN_PULSE_US(actual_freq));
        TEST_ASSERT(METRICS_TICKS_TO_US(min_pulse_high_ticks) >= MIN_PULSE_US(actual_freq));
    }

    spi_free(&spi);
    tester.pin_map_reset();
}

/** Test that the SPI-Master transfer can be performed in various configurations.
 *
 * Given board provides SPI-Master support.
 * When SPI transmission is performed using different settings.
 * Then data is successfully transfered.
 *
 * Additionally we check that spi_frequency() updates the baud rate generator
 * leaving other configurations (format) unchanged.
 */
template<typename T>
void spi_test_common(const test_config_t *tc_config)
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
    PinName ss_pin = (tc_config->auto_ss ? tc_config->ssel : NC);
    DigitalOut *ss = NULL;

    spi_get_capabilities(spi_get_module(tc_config->mosi, tc_config->miso, tc_config->sclk), ss_pin, false, &capabilities);

    if (check_capabilities(&capabilities, tc_config) == false) {
        return;
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
        case (BUFFERS_TX_ONE_SYM):
            tx_cnt = 1;
            rx_cnt = 1;
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
        spi_init(&spi, false, tc_config->mosi, tc_config->miso, tc_config->sclk, ss_pin);
    } else {
        const PinName hd_data = HD_DATA_LINE == MOSI ? tc_config->mosi : tc_config->miso;
        tester.pin_map_set(hd_data, MbedTester::LogicalPinSPIMosi);
        tester.pin_map_set(tc_config->sclk, MbedTester::LogicalPinSPISclk);
        tester.pin_map_set(tc_config->ssel, MbedTester::LogicalPinSPISsel);

        if (HD_DATA_LINE == MOSI) {
            spi_init(&spi, false, tc_config->mosi, NC, tc_config->sclk, ss_pin);
        } else {
            spi_init(&spi, false, NC, tc_config->miso, tc_config->sclk, ss_pin);
        }
    }

    // Initialize mbed SPI transmission format and frequency
    spi_format(&spi, tc_config->sym_size, (spi_mode_t)tc_config->spi_mode, (spi_bit_ordering_t)tc_config->spi_bit_order);
    const uint32_t actual_freq = spi_frequency(&spi, freq);

    // Configure spi_slave module
    tester.set_mode(tc_config->spi_mode);
    tester.set_bit_order(tc_config->spi_bit_order);
    tester.set_sym_size(tc_config->sym_size);
    tester.set_duplex_mode(tc_config->duplex);

    // Manually handle SS pin
    if (!tc_config->auto_ss) {
        ss = new DigitalOut(tc_config->ssel);
        *ss = SS_DEASSERT;
    }

    // Perform SPI transfers and verification
    for (uint32_t transfers = 0; transfers < SPI_TRANSFERS; transfers++) {
        expected_checksum = 0;

        // Reset tester stats and select SPI
        tester.peripherals_reset();
        tester.select_peripheral(SPITester::PeripheralSPI);

        if (tc_config->duplex == SPITester::HalfDuplex) {
            tester.set_hd_tx_rx_cnt(TRANSFER_COUNT, TRANSFER_COUNT);
            expected_sym_count = (2 * TRANSFER_COUNT);
        }

        // Init rx/tx buffers
        for (int i = 0; i < TRANSFER_COUNT; i++) {
            tx_buf[i] = ((0 - i) & sym_mask);
            rx_buf[i] = (T)DUMMY_SYM;

            switch (tc_config->test_buffers) {
                case (BUFFERS_COMMON):
                case (BUFFERS_RX_UNDEF):
                case (BUFFERS_TX_GT_RX):
                    expected_checksum += ((0 - i) & sym_mask);
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
                case (BUFFERS_TX_ONE_SYM):
                    tx_buf[0] = (0x12345678 & sym_mask);
                    expected_checksum = (0x12345678 & sym_mask);
                    expected_sym_count = 1;
                    break;
                default:
                    break;
            }
        }

        // Perfrom sync/async spi transfer
        handle_ss(ss, true);
        if (tc_config->sync_mode) {
            count = spi_transfer(&spi, p_tx, tx_cnt, p_rx, rx_cnt, &fill_symbol);
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

        if (ss) {
            // Wait one sclk period before CS de-assertion.
            wait_ns(PERIOD_NS(freq));
            handle_ss(ss, false);
        }

        // RX buffer validation
        for (int i = 0; i < rx_cnt && p_rx; i++) {
            TEST_ASSERT_EQUAL(i & sym_mask, rx_buf[i]);
        }

        // Additional check for RX < TX case: check if remaining bytes in RX buffer are not changed.
        for (int i = rx_cnt; i < TRANSFER_COUNT && p_rx; i++) {
            TEST_ASSERT_EQUAL((T)DUMMY_SYM, rx_buf[i]);
        }

        TEST_ASSERT_EQUAL(expected_sym_count, tester.get_transfer_count());
        TEST_ASSERT_EQUAL(expected_sym_count, count);
        TEST_ASSERT_EQUAL(expected_checksum, tester.get_receive_checksum());

        // Check times between CS assertion and first sclk edge and last sclk edge and CS de-assertion.
        if (tc_config->auto_ss) {
            TEST_ASSERT(tester.get_cs_to_first_clk_edge_ns() >= (PERIOD_NS(actual_freq) / 2));
            TEST_ASSERT(tester.get_last_clk_edge_to_cs_ns() >= (PERIOD_NS(actual_freq) / 2));
        }

        // Check if SCLK state on CS assertion and de-assertion is valid (according to specified mode).
        TEST_ASSERT(TEST_SCLK(tc_config->spi_mode, tester.get_start_stop_stats()));
    }

    spi_free(&spi);
    tester.pin_map_reset();
}

template<SPITester::SpiMode spi_mode, SPITester::SpiBitOrder spi_bit_order, uint32_t sym_size, uint32_t spi_freq, bool auto_ss, test_buffers_t test_buffers, bool sync_mode, SPITester::SpiDuplex duplex>
void spi_test_common(PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    const test_config_t tc_config = {mosi, miso, sclk, ssel, spi_mode, spi_bit_order, sym_size, spi_freq, auto_ss, test_buffers, sync_mode, duplex};

    if (sym_size <= 8) {
        spi_test_common<uint8_t>(&tc_config);
    } else if (sym_size <= 16) {
        spi_test_common<uint16_t>(&tc_config);
    } else {
        spi_test_common<uint32_t>(&tc_config);
    }
}

template<SPITester::SpiMode spi_mode, SPITester::SpiBitOrder spi_bit_order, uint32_t sym_size, uint32_t spi_freq, bool auto_ss, test_buffers_t test_buffers, bool sync_mode, SPITester::SpiDuplex duplex>
void spi_test_common_no_ss(PinName mosi, PinName miso, PinName sclk)
{
    PinName ss = find_ss_pin(mosi, miso, sclk);
    const test_config_t tc_config = {mosi, miso, sclk, ss, spi_mode, spi_bit_order, sym_size, spi_freq, auto_ss, test_buffers, sync_mode, duplex};

    printf("CS=(%s) \n", pinmap_ff_default_pin_to_string(ss));

    if (ss != NC) {
        if (sym_size <= 8) {
            spi_test_common<uint8_t>(&tc_config);
        } else if (sym_size <= 16) {
            spi_test_common<uint16_t>(&tc_config);
        } else {
            spi_test_common<uint32_t>(&tc_config);
        }
    }
}

Case cases[] = {
    // This will be run for all ports
    Case("SPI Master - init/free test all pins", all_ports<SPINoCSPort, DefaultFormFactor, spi_test_init_free>),

    // This will be run for all peripherals
    Case("SPI Master/Sync - basic test", all_peripherals<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - basic test", all_peripherals<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),

    // This will be run for single peripheral
    Case("SPI Master/Sync - mode testing (MODE_1)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode1, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - mode testing (MODE_2)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode2, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - mode testing (MODE_3)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode3, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - bit order testing (LSBFirst)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::LSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - hardware ss handling", one_peripheral<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, HW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - freqency testing (FREQ_200KHZ)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_200KHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - freqency testing (FREQ_2MHZ)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_2MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - freqency testing (FREQ_MIN)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MIN, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - freqency testing (FREQ_MAX)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MAX, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - symbol size testing (4)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 4, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - symbol size testing (12)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 12, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - symbol size testing (16)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 16, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - symbol size testing (20)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 20, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - symbol size testing (24)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 24, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - symbol size testing (28)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 28, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - symbol size testing (32)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 32, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - buffers testing (BUFFERS_TX_UNDEF)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_TX_UNDEF, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - buffers testing (BUFFERS_RX_UNDEF)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_RX_UNDEF, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - buffers testing (BUFFERS_TX_GT_RX)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_TX_GT_RX, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - buffers testing (BUFFERS_TX_LT_RX)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_TX_LT_RX, SYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Sync - buffers testing (BUFFERS_TX_ONE_SYM)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_TX_ONE_SYM, SYNC, SPITester::FullDuplex> >),
    //Case("SPI Master/Sync - Half Duplex", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, SYNC, SPITester::HalfDuplex> >),

    Case("SPI Master/Async - mode testing (MODE_1)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode1, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - mode testing (MODE_2)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode2, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - mode testing (MODE_3)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode3, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - bit order testing (LSBFirst)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::LSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - hardware ss handling", all_peripherals<SPIPort, DefaultFormFactor, spi_test_common<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, HW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - freqency testing (FREQ_200KHZ)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_200KHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - freqency testing (FREQ_2MHZ)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_2MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - freqency testing (FREQ_MIN)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MIN, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - freqency testing (FREQ_MAX)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_MAX, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - symbol size testing (4)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 4, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - symbol size testing (12)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 12, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - symbol size testing (16)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 16, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - symbol size testing (20)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 20, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - symbol size testing (24)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 24, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - symbol size testing (28)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 28, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - symbol size testing (32)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 32, FREQ_1MHZ, SW_SS, BUFFERS_COMMON, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - buffers testing (BUFFERS_TX_UNDEF)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_TX_UNDEF, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - buffers testing (BUFFERS_RX_UNDEF)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_RX_UNDEF, ASYNC, SPITester::FullDuplex> >),
    Case("SPI Master/Async - buffers testing (BUFFERS_TX_ONE_SYM)", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_common_no_ss<SPITester::Mode0, SPITester::MSBFirst, 8, FREQ_1MHZ, SW_SS, BUFFERS_TX_ONE_SYM, ASYNC, SPITester::FullDuplex> >),

    Case("SPI Master - actual clock freq", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_freq>),
    Case("get_capabilities(): SS cannot be managed by hardware", one_peripheral<SPINoCSPort, DefaultFormFactor, spi_test_capabilities>),
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